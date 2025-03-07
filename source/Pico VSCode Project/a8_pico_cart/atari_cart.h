/**
 *    _   ___ ___ _       ___          _   
 *   /_\ ( _ ) _ (_)__ _ / __|__ _ _ _| |_ 
 *  / _ \/ _ \  _/ / _/_\ (__/ _` | '_|  _|
 * /_/ \_\___/_| |_\__\_/\___\__,_|_|  \__|
 *                                         
 * 
 * Atari 8-bit cartridge for Raspberry Pi Pico
 *
 * Robin Edwards 2023
 *
 * Needs to be a release NOT debug build for the cartridge emulation to work
 */

#ifndef __ATARI_CART_H__
#define __ATARI_CART_H__

#define ATARI_PHI2_PIN        22    // used on boot to check if we are plugged into an atari or usb

#define DEFAULT_I2C_CLK_SPEED       (400*1000) // Default I2C speed is 400kHz

void atari_cart_main();
bool reset_eeram(bool eraseIfDisabledAutoboot);

#endif