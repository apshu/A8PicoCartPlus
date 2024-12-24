#ifndef __SSD1306_H__
#define __SSD1306_H__

// Define the size of the display we have attached. This can vary, make sure you
// have the right size defined or the output will look rather odd!
// Code has been tested on 128x32 and 128x64 OLED displays
#define SSD1306_HEIGHT              64
#define SSD1306_WIDTH               128

#define SSD1306_I2C_ADDR            _u(0x3C)

bool SSD1306_init();        // Initializes the display, returns true if success

#endif
