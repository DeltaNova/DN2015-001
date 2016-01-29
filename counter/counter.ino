// counter.ino - An arduino sketch that writes a count to an i2c oled display.
/*
  Copyright (C) 2016  Matthew Tunstall

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
#define OLED_ADDR 0x3C      // Address of I2C OLED Display
#include <string.h>
#include <avr/pgmspace.h>   // AVR PROGMEM Library
#include <Wire.h>           // Arduino Serial & I2C library
#include <stdint.h>         // Enable fixed width integers.
#include "ssd1306.h"        // SSD1306 Library
// Screen Buffers
#include "ascii_buffer.h"        // Include ASCII Chars & Symbols buffer
#include "rodent_buffer.h"       // Include rodent image buffer
#include "test_pattern_buffer.h" // Include test_pattern buffer

void count();

// ---------------------------------------------------------------------
// PROGMEM Strings/Buffers
// -----------------------
// "Hello World!"
//const uint8_t databuffer[12] PROGMEM = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21};
// Project Reference: "DN2015-001" // String Method #1 (HEX)
const uint8_t proj_ref[10] PROGMEM = {0x44, 0x4E, 0x32, 0x30, 0x31, 0x35, 0x2D, 0x30, 0x30, 0x31};
// Project Name: "counter" // String Method #2 (ASCII)
const uint8_t proj_name[] PROGMEM = "counter";
// Empty Count Buffer
const uint8_t empty_buffer[6] PROGMEM = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
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
// ---------------------------------------------------------------------
void setup() {
    OLED.init();                        // Join I2C Bus as Master
    OLED.init_bus(12);                  // Set I2C to 400KHz mode
    OLED.disp_setup();         // Setup Display
}

void loop(){

    // Start by clearing the display bufffer, solves any startup corruption.
    OLED.clear_buffer();
    // Set initial cursor position
    OLED.setCursor(0,36);
    delay(1000);
    // Write Project Reference to display
    OLED.PROGMEMwriteLine(proj_ref,10,ascii_buffer);
    delay(2000);

    // Move cursor position & write project name
    OLED.setCursor(2,42);
    OLED.PROGMEMwriteLine(proj_name,7,ascii_buffer);
    delay(3000);

    // Move cursor to count line
    OLED.setCursor(4,0);
    // The count is indicated by 'count' being displayed on the screen.
    // This reuses the first 5 chars of the project name, hopefully saving some space.
    // Should the project name change a string for "Count" would need to be defined.
    OLED.PROGMEMwriteLine(proj_name,5,ascii_buffer);
    // Set the position that the count value will be written.
    //OLED.setCursor(4, 42);
    // DEV NOTE:    The count will need to be rewritten when it is updated.
    //              Changing the column/row write limits would prevent the count
    //              overwriting other parts of the screen.
    //OLED.writeLine(proj_name,5,ascii_buffer); // TODO:Replace with count value.
    count();
    delay(1000);


}

void count(){
    // Count up and display count on screen
    uint8_t count_buffer[6]; // TODO: Adjust later to handle bigger count.

    for (uint8_t i = 0; i<101; i++){
        // Clear count_buffer
        strncpy(count_buffer,empty_buffer,6);
        // Set count write position
        OLED.setCursor(4,42);
        // Write the cleared count_buffer (erase previous count)
        OLED.PROGMEMwriteLine(empty_buffer,6,ascii_buffer);
        //delay(2000);
        // Reset count write position
        OLED.setCursor(4,42);
        // Convert integer to string (decimal)
        itoa(i,count_buffer,10); // Store i in count buffer as decimal.
        // Write the count to screen
        OLED.writeLine(count_buffer,6,ascii_buffer);
        delay(1000);

        }




}
