#ifndef __EERAM_I2C_H__
#define __EERAM_I2C_H__

    #include <stdint.h>
    #include <stdbool.h>
    #include "hardware/i2c.h"

#ifndef PARAM_ASSERTIONS_ENABLED_EERAMI2C
#define PARAM_ASSERTIONS_ENABLED_EERAMI2C 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        A1A2_LL,
        A1A2_LH,
        A1A2_HL,
        A1A2_HH,
    } EERAMI2C_A1A2_e;

    typedef union  __attribute__((packed)) {
        uint8_t asByte;
        struct {
            uint8_t EVENT : 1;
            uint8_t ASE : 1;
            uint8_t BP : 3;
            uint8_t : 2;
            uint8_t AM : 1;
        };
    } EERAMI2C_status_reg_t;

    void EERAMI2C_setI2Cinstance(i2c_inst_t *i2c);  // Using default_i2c if defined. May be changed any time. i2c == NULL resets to default_i2c. If no default_i2c defined, all calls return false until seti2c called with valid bus
    bool EERAMI2C_isChipDetected(EERAMI2C_A1A2_e chip_addr);
    bool EERAMI2C_readBuffer(void *buf, uint16_t bufLen, uint16_t from_address, EERAMI2C_A1A2_e chip_addr);
    bool EERAMI2C_writeBuffer(const void *buf, uint16_t bufLen, uint16_t to_address, EERAMI2C_A1A2_e chip_addr);
    bool EERAMI2C_storeNow(EERAMI2C_A1A2_e chip_addr);
    bool EERAMI2C_recallNow(EERAMI2C_A1A2_e chip_addr);
    bool EERAMI2C_readStatusReg(EERAMI2C_status_reg_t *status_reg, EERAMI2C_A1A2_e chip_addr);
    bool EERAMI2C_writeStatusReg(EERAMI2C_status_reg_t status_reg, EERAMI2C_A1A2_e chip_addr);

#ifdef __cplusplus
}
#endif

#endif