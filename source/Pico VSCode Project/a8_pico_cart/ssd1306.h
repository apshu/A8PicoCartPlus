#ifndef __SSD1306_H__
#define __SSD1306_H__

// Define the size of the display we have attached. This can vary, make sure you
// have the right size defined or the output will look rather odd!
// Code has been tested on 128x32 and 128x64 OLED displays
#define SSD1306_HEIGHT              64
#define SSD1306_WIDTH               128

#define SSD1306_I2C_ADDR            _u(0x3C)

typedef struct {
    uint8_t start_col;
    uint8_t end_col;
    uint8_t start_page;
    uint8_t end_page;

    int buflen;
} GFX_render_area_t;

bool SSD1306_init();        // Initializes the display, returns true if success
bool SSD1306_send_cmd(uint8_t cmd);
bool SSD1306_send_cmd_list(uint8_t *buf, int num);
void SSD1306_send_buf(uint8_t buf[], int buflen);
void SSD1306_scroll(bool on);
void SSD1306_render(uint8_t *buf, GFX_render_area_t *area);

void GFX_calc_render_area_buflen(GFX_render_area_t *area);
void GFX_SetPixel(uint8_t *buf, int x,int y, bool on);
void GFX_DrawLine(uint8_t *buf, int x0, int y0, int x1, int y1, bool on);
void GFX_WriteChar(uint8_t *buf, int16_t x, int16_t y, uint8_t ch);
void GFX_WriteString(uint8_t *buf, int16_t x, int16_t y, char *str);

#endif
