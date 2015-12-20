// ssd1306.h - Header file for ssd1306.cpp
// Library for SSD1306 OLED Display Driver
// Author: M. Tunstall
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
#include <stdint.h>         // Enable fixed width integers.
#include <avr/pgmspace.h>
#ifndef SSD1306_H //If ssd1306.h hasn't been included elsewhere, define it now.
#define SSD1306_H

// DEV NOTE: References to the I2C library have been templated to make switching hardware easier.
// DEV NOTE: Some functions are dependant on <avr/pgmspace.h> and will not work on all hardware.


template< typename TWI_T >
class SSD1306
{
    protected: // accessed by member functions and derived classes.
        TWI_T TWI; // Reference to an instance of the TWI/I2C interface.
    private: // accessed by member functions but not derived classes.
        uint8_t SSD1306_ADDR;
    public: // accessed by anybody.
        SSD1306(){ // Default Constructor
            SSD1306_ADDR = 0x3C; // Default address of development display
        }
        SSD1306(uint8_t addr){
            SSD1306_ADDR = addr; // Specify another address for another display
        }
        ~SSD1306(){}     // Destructor
        void init(){TWI.begin();}
        void init_bus(uint8_t bus_mode){TWI.busfreq(bus_mode);}
        void disp_setup();
        void clear_buffer();
        void setAddressMode(uint8_t mode);
        void setCursor(uint8_t row_start = 0x00, uint8_t col_start = 0x00, uint8_t col_end = 0x7F, uint8_t row_end = 0x07);
        void setPageStartCol(uint8_t pscol);
        void PROGMEMwriteBuf(const uint8_t* buffer_to_write);
        void writeLine(const uint8_t* buffer_name, uint8_t buffer_length, const uint8_t* symbol_buffer);

};

template< typename TWI_T >
void SSD1306<TWI_T>::disp_setup(){
    // Initialisation of OLED Display
    TWI.beginTransmission(SSD1306_ADDR); // Start Transmitting
    // Initialisation Based upon Application Note Example
    TWI.write(0x00);       // Send Command Byte Stream
    TWI.write(0xAE);       // Turn Display Off
    TWI.write(0xA8);       // Set Multiplex Ratio
    TWI.write(0x3f);       // 1/64 duty cycle
    TWI.write(0xD3);       // Set Display Offset
    TWI.write(0x00);       // No offset applied
    TWI.write(0x40);       // Set Display Start Line #0
    TWI.write(0xA1);       // Set Segment Remap (Flips Display) A0/A1
    TWI.write(0xC8);       // COM Scan Direction c0/c8
    TWI.write(0xDA);       // Set COM pins
    TWI.write(0x12);       // 0x12 - See Application Note SW INIT
    TWI.write(0x81);       // Set Contrast
    TWI.write(0x7F);       // Default Contrast Level 127/255
    TWI.write(0xA4);       // Entire Display On - Output follows RAM Contents
    TWI.write(0xA6);       // Set Normal Display 0xA7 inverts
    TWI.write(0xD5);       // Display Clk Div
    TWI.write(0x80);       // Default Value 0x80
    TWI.write(0x8D);       // Setup Charge Pump - See SSD1306 Application Note
    TWI.write(0x14);       // Enable Charge Pump during display on.
    TWI.write(0xD9);       // Setup Precharge
    TWI.write(0x22);       //
    TWI.write(0xDB);       //VCOMH DESELECT
    TWI.write(0x30);       //
    TWI.write(0x20);       // Set Mem Addr Mode
    TWI.write(0x00);       // Horzontal
    TWI.write(0xAF);       // Display On
    TWI.endTransmission(); // Stop Transmitting
}

template< typename TWI_T >
void SSD1306<TWI_T>::clear_buffer(){
    // Clear GFX Buffer
    TWI.beginTransmission(SSD1306_ADDR);
    TWI.write(0x00);   // Control Byte Command Stream
    TWI.write(0x21);    // Setup Column Addresses
    TWI.write(0x00);    // Col Start Addr
    TWI.write(0x7F);    // Col End Addr
    TWI.write(0x22);    // Set Page Addr
    TWI.write(0x00);    // Start Page 0
    TWI.write(0x07);    // End Page 7
    TWI.endTransmission();

    for (uint16_t i=0; i<1024; i++){
        TWI.beginTransmission(SSD1306_ADDR);
        TWI.write(0x40);      // Control Byte Data Stream
        for (uint8_t x=0; x<16; x++) {
            TWI.write(0x00);
            i++;
        }
        i--;
        TWI.endTransmission();
    }
}

template< typename TWI_T >
void SSD1306<TWI_T>::setAddressMode(uint8_t mode){
    // 0 = Horizontal (Default/Fallback)
    // 1 = Vertical
    // 2 = Page
    TWI.beginTransmission(SSD1306_ADDR);
    TWI.write(0x00);       // Control Byte Command Stream
    TWI.write(0x20);       // Setup Address Mode
    if (mode == 0){         // Setup Horizontal Mode
        TWI.write(0x00);   // Horizontal
    }
    else if (mode == 1){    // Setup Vertical Mode
        TWI.write(0x01);   // Vertical
    }
    else if (mode == 2){    // Setup Page Mode
        TWI.write(0x02);   // Page
    }
    else{                   // Setup Horizontal Mode as fallback option
        TWI.write(0x00);   // Horizontal
    }
    TWI.endTransmission();
}

template< typename TWI_T >
void SSD1306<TWI_T>::setCursor(uint8_t row_start, uint8_t col_start, uint8_t col_end, uint8_t row_end){
    // Set Cursor Position
    TWI.beginTransmission(SSD1306_ADDR);
    TWI.write(0x00); // Control Byte Command Stream
    TWI.write(0x21); // Set Column Address
    TWI.write(col_start); // Start at column 0
    TWI.write(col_end); // End at column 127
    TWI.write(0x22); // Set Page (Row) address
    TWI.write(row_start); // Start Page
    TWI.write(row_end); // End Page
    TWI.endTransmission();
}

template< typename TWI_T >
void SSD1306<TWI_T>::setPageStartCol(uint8_t pscol){
    uint8_t upper_nibble = (((pscol & 0xF0)>>4)|0x10);
    uint8_t lower_nibble = pscol & 0x0F;
    TWI.beginTransmission(SSD1306_ADDR);
    TWI.write(0x00); // Control Byte Command Stream
    TWI.write(lower_nibble); // Set Lower Col Start Addr (Page Addr Mode)
    TWI.write(upper_nibble); // Set Upper Col Start Addr (Page Addr Mode)
    TWI.endTransmission();
    }

template< typename TWI_T >
void SSD1306<TWI_T>::PROGMEMwriteBuf(const uint8_t* buffer_to_write)
{
    for (uint16_t i=0; i<1024; i++){
        TWI.beginTransmission(SSD1306_ADDR);
        TWI.write(0x40);      // Control Byte Data Stream
        for (uint8_t x=0; x<16; x++) {
            TWI.write(pgm_read_byte(&buffer_to_write[i]));
            i++;
        }
        i--;
        TWI.endTransmission();
    }
}

template< typename TWI_T >
void SSD1306<TWI_T>::writeLine(const uint8_t* buffer_name, uint8_t buffer_length, const uint8_t* symbol_buffer){
    // Write Text Display Line
    // Note: Set cursor position before writing.

    // For each item in data buffer
    for (uint16_t i=0; i<buffer_length; i++){
        TWI.beginTransmission(SSD1306_ADDR);
        TWI.write(0x40);                   // Control Byte Data Stream
        // Multiply by 6 to get char start position in the ascii_buffer
        uint16_t offset = pgm_read_byte(&buffer_name[i]) * 6;
        // Print each byte that makes up the character
        for ( uint16_t x =0; x<6; x++) {
            TWI.write(pgm_read_byte(&symbol_buffer[offset+x]));
        }
        TWI.endTransmission();
    }
    // Note: Reset the cursor position to Row 0, Col 0. Otherwise the
    //       buffers become offset on the next loop.
}


#endif // End of ifndef SSD1306_H
