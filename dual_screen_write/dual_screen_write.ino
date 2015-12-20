// dual_screen_write.ino - An arduino sketch that uses an i2c oled display.
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
// Signal to Screen Buffers to use AVR_PROGMEM versions
#define AVR_PROGMEM
#define OLED_ADDR 0x3C      // Address of I2C OLED Display #1
#define OLED2_ADDR 0x3D     // Address of I2C OLED Display #2
#include <avr/pgmspace.h>   // AVR PROGMEM Library
#include <Wire.h>           // Arduino Serial & I2C library
#include <stdint.h>         // Enable fixed width integers.
#include "ssd1306.h"        // SSD1306 Library
// Screen Buffers
#include "ascii_buffer.h"        // Include ASCII Chars & Symbols buffer
#include "rodent_buffer.h"       // Include rodent image buffer
#include "test_pattern_buffer.h" // Include test_pattern buffer

// ---------------------------------------------------------------------
// PROGMEM Strings/Buffers
// -----------------------
// "Hello World!"
const uint8_t databuffer[12] PROGMEM = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21};
// ---------------------------------------------------------------------
class I2C {
    // Specific to Atmega328p, makes use of the Arduino Wire Library.
    // DEV NOTE: The code within each class function will need to be
    //           changed depending on hardware.
    public:
        I2C(){} // Constructor
        ~I2C(){} // Destructor
        void begin(){ Wire.begin(); }
        void beginTransmission(uint8_t addr){ Wire.beginTransmission(addr); }
        void write(uint8_t byte){ Wire.write(byte); }
        void endTransmission(){ Wire.endTransmission(); }
        void busfreq(uint8_t TWBRbyte){TWBR = TWBRbyte;};
};
// ---------------------------------------------------------------------
SSD1306<I2C> OLED(OLED_ADDR); // Pass I2C Class into SSD1306 Class Template
SSD1306<I2C> OLED2(OLED2_ADDR); // Pass I2C Class into SSD1306 Class Template
// ---------------------------------------------------------------------
void setup() {
    // These commands act on the I2C bus settings and don't need to be
    // run again for the second display
    OLED.init();                        // Join I2C Bus as Master
    OLED.init_bus(12);                  // Set I2C to 400KHz mode
    // Setup each display
    OLED.disp_setup();         // Setup Display #1
    OLED2.disp_setup();        // Setup Display #2
}

void loop(){
    // Run sequence on display #1
    // Write data to display buffer from program memory
    OLED.PROGMEMwriteBuf(ascii_buffer);
    delay(2000);
    OLED.PROGMEMwriteBuf(rodent);
    delay(2000);
    OLED.PROGMEMwriteBuf(test_pattern);
    delay(2000);
    OLED.clear_buffer();     // Clear display buffer
    delay(2000);
    OLED.setCursor(0,0);
    OLED.writeLine(databuffer,12,ascii_buffer);
    OLED.setCursor(2,20);
    OLED.writeLine(databuffer,12,ascii_buffer);
    OLED.setCursor(4,40);
    OLED.writeLine(databuffer,12,ascii_buffer);
    // DEV Note:
    // When setting the cursor location for this last item it is with the
    // knowledge that there is too much text to write than columns available on screen.
    // If the same page start and end address are provided then the additional
    // data begins to overwrite itself from the staring column.
    // In this case the data is allowed to continue onto the next page
    // but it begins to write to this page at the same starting column as before.
    OLED.setCursor(6,80,127,7);
    OLED.writeLine(databuffer,12,ascii_buffer);
    OLED.setCursor(); // Reset Cursor
    delay(3000);
    OLED.clear_buffer();     // Clear display buffer

    // Run sequence on display #2
    // Write data to display buffer from program memory
    OLED2.PROGMEMwriteBuf(ascii_buffer);
    delay(2000);
    OLED2.PROGMEMwriteBuf(rodent);
    delay(2000);
    OLED2.PROGMEMwriteBuf(test_pattern);
    delay(2000);
    OLED2.clear_buffer();     // Clear display buffer
    delay(2000);
    OLED2.setCursor(0,0);
    OLED2.writeLine(databuffer,12,ascii_buffer);
    OLED2.setCursor(2,20);
    OLED2.writeLine(databuffer,12,ascii_buffer);
    OLED2.setCursor(4,40);
    OLED2.writeLine(databuffer,12,ascii_buffer);
    // DEV Note:
    // When setting the cursor location for this last item it is with the
    // knowledge that there is too much text to write than columns available on screen.
    // If the same page start and end address are provided then the additional
    // data begins to overwrite itself from the staring column.
    // In this case the data is allowed to continue onto the next page
    // but it begins to write to this page at the same starting column as before.
    OLED2.setCursor(6,80,127,7);
    OLED2.writeLine(databuffer,12,ascii_buffer);
    OLED2.setCursor(); // Reset Cursor
    delay(3000);
    OLED2.clear_buffer();     // Clear display buffer
}
