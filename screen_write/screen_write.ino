// screen_write.ino - An arduino sketch that uses an i2c oled display.
/*
  Copyright (C) 2015  Matthew Tunstall

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
    Development Notes
    -----------------
    The OLED display is 128x64 pixels and uses a SSD1306 driver
    The OLED module I2C Address (7bit) is 0x3C or 0x3D
*/
#include <avr/pgmspace.h>
#include <Wire.h> // Arduino Library with Serial & I2C interfaces
#include <stdint.h>         // Enable fixed width integers.
#include "ascii_buffer.h" // Include 'buffer' ASCII Chars & Symbols
#define OLED_ADDR 0x3C // Address of I2C OLED Display

// ---------------------------------------------------------------------
// PROGMEM Strings/Buffers
// -----------------------
// "Hello World!"
const uint8_t databuffer[12] PROGMEM = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21};
// "
// 128 x 64 Rodent Pattern
const uint8_t buffer2[1024] PROGMEM = {

    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x08, 0x10, 0x20, 0x20, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x28, 0x30, 0x20, 0x00, 0x00, // Reversed Rodent
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,

    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,

    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,

    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,

    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,

    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,

    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,

    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x30, 0x28, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00
    };
// 128 x 64 Test Pattern
const uint8_t buffer3[1024] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x20, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x80, 0x04, 0x08,
    0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0e, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xf0, 0x0c, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0c, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02,
    0x04, 0x08, 0x10, 0x20, 0x41, 0x82, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x80,
    0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x08, 0x10, 0x20, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x40, 0x40, 0x40, 0x20, 0x10, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x06, 0x18, 0x10, 0x20, 0x20, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x10, 0x18, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x82, 0x44, 0x28, 0x10, 0x28, 0x45, 0x82, 0x05, 0x08,
    0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xc0, 0xc0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02,
    0x04, 0x08, 0x10, 0x20, 0x41, 0x82, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0x10, 0x08, 0x04, 0x02,
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
    0x02, 0x04, 0x08, 0x10, 0x60, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x30, 0x0c, 0x02, 0x02, 0x0c, 0x30, 0xc0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08,
    0x10, 0x20, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xc0, 0x30, 0x0c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0c,
    0x30, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x18, 0x20, 0x40, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x40, 0x20, 0x18, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x30,
    0x0c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x0c, 0x30, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x02, 0x02, 0x04, 0x04, 0x04, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x02, 0x02,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void disp_setup();
void draw_buffer();
void draw_buffer2();
void clear_buffer();
void setCursor(uint8_t row);
void resetCursor();

// ---------------------------------------------------------------------
void setup() {
    //Serial.begin(9600); // Create a serial connection
    Wire.begin();       // Join I2C Bus as Master
    // Set I2C to 400KHz mode
    TWBR=12;
    disp_setup();       // Setup Display
}

void disp_setup(){
    // Initialisation of OLED Display
    Wire.beginTransmission(OLED_ADDR); // Start Transmitting
    //Wire.write(0x00);                   // Low Col = 0
    //Wire.write(0x10);                   // Hi Col = 0

    // ---
    //Wire.write(0x20);                   // Set Mem Addr Mode
    //Wire.write(0x00);                   // 0x00 Horizontal

    // Initialisation Based upon Application Note Example
    Wire.write(0x00);                   // Send Command Byte Stream
    // --
    Wire.write(0xAE);                   // Turn Display Off
    // ---
    Wire.write(0xA8);                   // Set Multiplex Ratio
    Wire.write(0x3f);                   // 1/64 duty cycle
    // ---
    Wire.write(0xD3);                   // Set Display Offset
    Wire.write(0x00);                   // No offset applied
    // ---
    Wire.write(0x40);                   // Set Display Start Line #0
    // ---
    Wire.write(0xA1);                   // Set Segment Remap (Flips Display) A0/A1
    // ---
    Wire.write(0xC8);                   // COM Scan Direction c0/c8
    // ---
    Wire.write(0xDA);                   // Set COM pins
    Wire.write(0x12);                   // 0x12 - See Application Note SW INIT
    // ---
    Wire.write(0x81);                   // Set Contrast
    Wire.write(0x7F);                   // Default Contrast Level 127/255
    // ---
    Wire.write(0xA4);                   // Entire Display On - Output follows RAM Contents
    // ---
    Wire.write(0xA6);                   // Set Normal Display 0xA7 inverts
    // ---
    Wire.write(0xD5);                   // Display Clk Div
    Wire.write(0x80);                   // Default Value 0x80
    // ---
    Wire.write(0x8D);                   // Setup Charge Pump - See SSD1306 Application Note
    Wire.write(0x14);                   // Enable Charge Pump during display on.
    // ---
    Wire.write(0xD9);                   // Setup Precharge
    Wire.write(0x22);                   //
    // ---
    Wire.write(0xDB);                   //VCOMH DESELECT
    Wire.write(0x30);                   //
    // ---
    Wire.write(0x20);                   // Set Mem Addr Mode
    Wire.write(0x00);                   // Horzontal


    Wire.write(0xAF);                   // Display On
    // ---
    Wire.endTransmission(); // Stop Transmitting
}
/*
void draw_buffer(){
    // Draw buffer on display
    Wire.beginTransmission(OLED_ADDR);
    Wire.write(0x00);    // Control Byte Command Stream
    Wire.write(0x21);    // Setup Column Addresses
    Wire.write(0x00);    // Col Start Addr
    Wire.write(0x7F);    // Col End Addr
    Wire.write(0x22);    // Set Page Addr
    Wire.write(0x00);    // Start Page 0
    Wire.write(0x07);    // End Page 7
    Wire.endTransmission();

    for (uint16_t i=0; i<1024; i++){
        Wire.beginTransmission(OLED_ADDR);
        Wire.write(0x40);      // Control Byte Data Stream
        for (uint8_t x=0; x<16; x++) {
            Wire.write(buffer[i]);
            i++;
        }
        i--;
        Wire.endTransmission();
    }
}
*/

void clear_buffer(){
    // Clear GFX Buffer
    Wire.beginTransmission(OLED_ADDR);
    Wire.write(0x00);   // Control Byte Command Stream
    Wire.write(0x21);    // Setup Column Addresses
    Wire.write(0x00);    // Col Start Addr
    Wire.write(0x7F);    // Col End Addr
    Wire.write(0x22);    // Set Page Addr
    Wire.write(0x00);    // Start Page 0
    Wire.write(0x07);    // End Page 7
    Wire.endTransmission();

    for (uint16_t i=0; i<1024; i++){
        Wire.beginTransmission(OLED_ADDR);
        Wire.write(0x40);      // Control Byte Data Stream
        for (uint8_t x=0; x<16; x++) {
            Wire.write(0x00);
            i++;
        }
        i--;
        Wire.endTransmission();
    }
}

void PROGMEMwriteBuf(const uint8_t* buffer_to_write)
{
    for (uint16_t i=0; i<1024; i++){
        Wire.beginTransmission(OLED_ADDR);
        Wire.write(0x40);      // Control Byte Data Stream
        for (uint8_t x=0; x<16; x++) {
            Wire.write(pgm_read_byte(&buffer_to_write[i]));
            i++;
        }
        i--;
        Wire.endTransmission();
    }
}

void writeLine(uint8_t* buffer_name, uint8_t buffer_length){
    // Write Text Display Line

    // Setup
    //Wire.beginTransmission(OLED_ADDR);
    //Wire.write(0x00); // Control Byte Command Stream
    //Wire.write(0x21); // Set Column Address
    //Wire.write(0x00); // Start at column 0
    //Wire.write(0x7F); // End at column 127
    //Wire.write(0x22); // Set Page (Row) address
    //Wire.write(0x01); // Start on Page (Row) 1
    //Wire.write(0x01); // End on Page (Row) 1
    //Wire.endTransmission();


    for (uint16_t i=0; i<buffer_length; i++){ // For each item in data buffer
        Wire.beginTransmission(OLED_ADDR);
        Wire.write(0x40); // Control Byte Data Stream
        uint16_t offset = pgm_read_byte(&buffer_name[i]) * 6; // Multiply by 6 to get the character start position in the ascii_buffer
        for ( uint16_t x =0; x<6; x++) { // Print each byte that makes up the character
            //Wire.write(pgm_read_byte(&buffer[432+x]));
            Wire.write(pgm_read_byte(&buffer[offset+x]));
        }
        Wire.endTransmission();
    }

    // Note: Reset the cursor position to Row 0, Col 0. Otherwise the buffers become offset on the next loop.


}

void setCursor(uint8_t row_start = 0x00, uint8_t col_start = 0x00, uint8_t col_end = 0x7F, uint8_t row_end = 0x08){
    // Set Cursor Position
    Wire.beginTransmission(OLED_ADDR);
    Wire.write(0x00); // Control Byte Command Stream
    Wire.write(0x21); // Set Column Address
    Wire.write(col_start); // Start at column 0
    Wire.write(col_end); // End at column 127
    Wire.write(0x22); // Set Page (Row) address - In this case both the same as writing to a single row.
    Wire.write(row_start); // Start Page
    Wire.write(row_end); // End Page
    Wire.endTransmission();
    }

void resetCursor() {
    // Reset the cursor position to Row 0, Col 0.
    // Allow data to be written to entire screen
    Wire.beginTransmission(OLED_ADDR);
    Wire.write(0x00); // Control Byte Command Stream
    Wire.write(0x21); // Set Column Address
    Wire.write(0x00); // Start at column 0
    Wire.write(0x7F); // End at column 127
    Wire.write(0x22); // Set Page (Row) address
    Wire.write(0x00); // Start on Page (Row) 0
    Wire.write(0x07); // End on Page (Row) 7
    Wire.endTransmission();

    }

void setAddressMode(uint8_t mode){
    // 0 = Horizontal (Default/Fallback)
    // 1 = Vertical
    // 2 = Page
    Wire.beginTransmission(OLED_ADDR);
    Wire.write(0x00);       // Control Byte Command Stream
    Wire.write(0x20);       // Setup Address Mode
    if (mode == 0){         // Setup Horizontal Mode
        Wire.write(0x00);   // Horizontal
    }
    else if (mode == 1){    // Setup Vertical Mode
        Wire.write(0x01);   // Vertical
    }
    else if (mode == 2){    // Setup Page Mode
        Wire.write(0x02);   // Page
    }
    else{                   // Setup Horizontal Mode as fallback option
        Wire.write(0x00);   // Horizontal
    }
    Wire.endTransmission();
}

void loop(){
    //draw_buffer();
    PROGMEMwriteBuf(buffer);  // Write data to display buffer from program memory
    delay(2000);
    PROGMEMwriteBuf(buffer2);
    delay(2000);
    PROGMEMwriteBuf(buffer3);
    delay(2000);
    clear_buffer();     // Clear display buffer
    delay(2000);
    setCursor(0,0);
    writeLine(databuffer,12);
    setCursor(2,20);
    writeLine(databuffer,12);
    setCursor(4,40);
    writeLine(databuffer,12);
    // DEV Note:
    // When setting the cursor location for this last item it is with the
    // knowledge that there is too much text to write than columns available on screen.
    // If the same page start and end address are provided then the additional
    // data begins to overwrite itself from the staring column.
    // In this case the data is allowed to continue onto the next page
    // but it begins to write to this page at the same starting column as before.
    setCursor(6,80,127,7);
    writeLine(databuffer,12);
    resetCursor();

    delay(3000);
}
