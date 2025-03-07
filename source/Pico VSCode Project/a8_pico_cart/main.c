/**
 *    _   ___ ___ _       ___          _   
 *   /_\ ( _ ) _ (_)__ _ / __|__ _ _ _| |_ 
 *  / _ \/ _ \  _/ / _/_\ (__/ _` | '_|  _|
 * /_/ \_\___/_| |_\__\_/\___\__,_|_|  \__|
 *                                         
 * 
 * Atari 8-bit cartridge for Raspberry Pi Pico (16MB clone with all GPIO)
 *
 * Robin Edwards 2023
 *
 * Needs to be a release NOT debug build for the cartridge emulation to work
 * 
 * Edit myboard.h depending on the type of flash memory on the pico clone
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/time.h"
#include "tusb.h"

#include "atari_cart.h"
#include "fatfs_disk.h"

#include "hardware/i2c.h"
#include "ssd1306.h"

void cdc_task(void);

int main(void)
{
    // check to see if we are plugged into Atari 8-bit
    // by checking for high on PHI2 gpio for 100ms
    gpio_init(ATARI_PHI2_PIN);
    gpio_set_dir(ATARI_PHI2_PIN, GPIO_IN);

    // Configuring the I2C host mode
    i2c_init(i2c_default, DEFAULT_I2C_CLK_SPEED);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);

    while (to_ms_since_boot(get_absolute_time()) < 100)
    {
      if (gpio_get(ATARI_PHI2_PIN))
        atari_cart_main();
    }

  // we are presumably powered from USB
  // enter USB mass storage mode

  stdio_init_all();   // for serial output, via printf()
  printf("Start up\n");  
  
  if (SSD1306_init()) {     // Init GFX display
    // Initialize render area for entire frame (SSD1306_WIDTH pixels by SSD1306_NUM_PAGES pages)
    uint8_t lcd_buf[SSD1306_BUF_LEN];
    // zero the entire display buffer
    memset(lcd_buf, 0, SSD1306_BUF_LEN);
    GFX_render_area_t frame_area = {
      start_col: 0,
      end_col : SSD1306_WIDTH - 1,
      start_page : 0,
      end_page : SSD1306_NUM_PAGES - 1
    };

    GFX_calc_render_area_buflen(&frame_area);
    // Render the text
    GFX_WriteString(lcd_buf, 15, (SSD1306_HEIGHT>>1) - 10, "Drag and drop");
    GFX_WriteString(lcd_buf, 20, (SSD1306_HEIGHT>>1) + 10, "Atari files");
    // Show the buffer on screen
    SSD1306_render(lcd_buf, &frame_area);
  }
  
  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);

  while (1)
  {
    tud_task(); // tinyusb device task

    cdc_task();
  }

  return 0;
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  printf("Device mounted\n"); 
  if (!mount_fatfs_disk()) {
    // Issues with FS, complete FS format
    create_fatfs_disk();
    // Reset EERAM regardless of NOAUTOBOOT button
    reset_eeram(true); 
  }
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  printf("Device unmounted\n");  
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
//  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
//  blink_interval_ms = BLINK_MOUNTED;
}


//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
void cdc_task(void)
{
  // connected() check for DTR bit
  // Most but not all terminal client set this when making connection
  // if ( tud_cdc_connected() )
  {
    // connected and there are data available
    if ( tud_cdc_available() )
    {
      // read data
      char buf[64];
      uint32_t count = tud_cdc_read(buf, sizeof(buf));
      (void) count;

      // Echo back
      // Note: Skip echo by commenting out write() and write_flush()
      // for throughput test e.g
      //    $ dd if=/dev/zero of=/dev/ttyACM0 count=10000
      tud_cdc_write(buf, count);
      tud_cdc_write_flush();
    }
  }
}

// Invoked when cdc when line state changed e.g connected/disconnected
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
  (void) itf;
  (void) rts;

  // TODO set some indicator
  if ( dtr )
  {
    // Terminal connected
  }else
  {
    // Terminal disconnected
  }
}

// Invoked when CDC interface received data from host
void tud_cdc_rx_cb(uint8_t itf)
{
  (void) itf;
}

