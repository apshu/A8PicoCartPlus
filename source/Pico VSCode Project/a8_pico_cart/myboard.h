// myboard.h

// setting first overrides the value in the default header

//#define PICO_FLASH_SPI_CLKDIV 2     // use for winbond flash
#define PICO_FLASH_SPI_CLKDIV 4     // use for slower flash (e.g. zbit)

// I2C settings for the PicoCart+ board
#define PICO_DEFAULT_I2C            0       // GP28 and GP29 default to I2C0
#define PICO_DEFAULT_I2C_SCL_PIN    28      // SCL on the Plus board
#define PICO_DEFAULT_I2C_SDA_PIN    29      // SDA on the Plus board

// pick up the rest of the settings
#include "boards/pico.h"
