#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "eeram_i2c.h"

static i2c_inst_t *EERAMI2C_i2c =
#ifdef i2c_default
i2c_default
#else
NULL
#endif
;

#define EERAMI2C_SRAM_IIC_BASE_ADDRESS (0b1010000)
#define EERAMI2C_CONTROL_REG_IIC_BASE_ADDRESS (0b0011000)

#define EERAMI2C_CONTROL_REG_ADDRESS_STATUS (0x00)
#define EERAMI2C_CONTROL_REG_ADDRESS_COMMAND (0x55)
#define EERAMI2C_CONTROL_COMMAND_SOFTWARE_STORE (0b00110011)
#define EERAMI2C_CONTROL_COMMAND_SOFTWARE_RECALL (0b11011101)

// Using default_i2c if defined. May be changed any time. i2c == NULL resets to default_i2c. If no default_i2c defined, all calls return false until seti2c called with valid bus
void EERAMI2C_setI2Cinstance(i2c_inst_t *i2c) {
    EERAMI2C_i2c = i2c;
}

bool EERAMI2C_isChipDetected(EERAMI2C_A1A2_e chip_addr) {
    invalid_params_if(EERAMI2C, chip_addr > 3);
    if (EERAMI2C_i2c) {
        uint8_t dst;
        uint8_t iic_addr = ((chip_addr << 1) & 6) | EERAMI2C_CONTROL_REG_IIC_BASE_ADDRESS;
        if (i2c_read_timeout_us(EERAMI2C_i2c, iic_addr, &dst, 1, true, 20000) == 1) { // Read with 20ms timeout
            iic_addr += EERAMI2C_SRAM_IIC_BASE_ADDRESS - EERAMI2C_CONTROL_REG_IIC_BASE_ADDRESS;
            return (i2c_read_timeout_us(EERAMI2C_i2c, iic_addr, &dst, 1, false, 20000) == 1); // Read with 20ms timeout
        }
    }
    return false;
}

bool EERAMI2C_readBuffer(void *buf, uint16_t bufLen, uint16_t from_address, EERAMI2C_A1A2_e chip_addr) {
    invalid_params_if(EERAMI2C, chip_addr > 3);
    if (EERAMI2C_writeBuffer((const void*)!(buf && bufLen), 0, from_address, chip_addr)) {
        if (buf && bufLen) {
            uint8_t iic_addr = ((chip_addr << 1) & 6) | EERAMI2C_SRAM_IIC_BASE_ADDRESS;
            return (i2c_read_blocking_until(EERAMI2C_i2c, iic_addr, buf, bufLen, false, make_timeout_time_ms(100)));
        }
        return true;
    }
    return false;
}

bool EERAMI2C_writeBuffer(const void *buf, uint16_t bufLen, uint16_t to_address, EERAMI2C_A1A2_e chip_addr) {
    invalid_params_if(EERAMI2C, chip_addr > 3);
    if (EERAMI2C_i2c) {
        uint8_t addrBuf[bufLen + 2];
        memcpy(addrBuf, &to_address, 2);
        if (buf && bufLen) {
            memcpy(addrBuf + 2, buf, bufLen);
        } else {
            bufLen = 0;
        }
        uint8_t iic_addr = ((chip_addr << 1) & 6) | EERAMI2C_SRAM_IIC_BASE_ADDRESS;
        return (i2c_write_blocking_until(EERAMI2C_i2c, iic_addr, addrBuf, bufLen + 2, !buf, make_timeout_time_ms(100)) == (bufLen + 2)); //No STOP if buf == NULL
    }
    return false;
}

static bool EERAMI2C_controlWrite(EERAMI2C_A1A2_e chip_addr, uint8_t reg_addr, uint8_t data) {
    invalid_params_if(EERAMI2C, chip_addr > 3);
    if (EERAMI2C_i2c) {
        uint8_t iic_addr = ((chip_addr << 1) & 6) | EERAMI2C_CONTROL_REG_IIC_BASE_ADDRESS;
        uint8_t iic_buf[2] = {reg_addr, data};
        return (i2c_write_blocking_until(EERAMI2C_i2c, iic_addr, iic_buf, 2, false, make_timeout_time_ms(100)) == 2);
    }
    return false;
}

bool EERAMI2C_storeNow(EERAMI2C_A1A2_e chip_addr) {
    invalid_params_if(EERAMI2C, chip_addr > 3);
    return (EERAMI2C_controlWrite(chip_addr, EERAMI2C_CONTROL_REG_ADDRESS_COMMAND, EERAMI2C_CONTROL_COMMAND_SOFTWARE_STORE));
}

bool EERAMI2C_recallNow(EERAMI2C_A1A2_e chip_addr) {
    invalid_params_if(EERAMI2C, chip_addr > 3);
    return (EERAMI2C_controlWrite(chip_addr, EERAMI2C_CONTROL_REG_ADDRESS_COMMAND, EERAMI2C_CONTROL_COMMAND_SOFTWARE_RECALL));
}

bool EERAMI2C_writeStatusReg(EERAMI2C_status_reg_t status_reg, EERAMI2C_A1A2_e chip_addr) {
    invalid_params_if(EERAMI2C, chip_addr > 3);
    return (EERAMI2C_controlWrite(chip_addr, EERAMI2C_CONTROL_REG_ADDRESS_STATUS, status_reg.asByte));
}

bool EERAMI2C_readStatusReg(EERAMI2C_status_reg_t *status_reg, EERAMI2C_A1A2_e chip_addr) {
    invalid_params_if(EERAMI2C, chip_addr > 3);
    if (EERAMI2C_i2c && status_reg) {
        uint8_t iic_addr = ((chip_addr << 1) & 6) | EERAMI2C_CONTROL_REG_IIC_BASE_ADDRESS;
        return (i2c_read_blocking_until(EERAMI2C_i2c, iic_addr, &status_reg->asByte, 1, false, make_timeout_time_ms(100)));
    }
    return false;
}
