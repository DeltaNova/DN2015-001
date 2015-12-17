// ssd1306.h - Header file for ssd1306.cpp
// Library for SSD1306 OLED Display Driver
// Author: M. Tunstall
#include <stdint.h>         // Enable fixed width integers.
//#include <Wire.h>           // TWI Library
#include "twi.h"            // My wrapper around <Wire.h>
#include <avr/pgmspace.h>
#ifndef SSD1306_H //If ssd1306.h hasn't been included elsewhere, define it now.
#define SSD1306_H


// Content goes here


// DEV NOTE: Move the references to the Arduino Wire library into class
//           functions. Aim is to make it easier to switch primary code
//           and library to another i2c/twi library at a later date.

template< typename TWI_T >
class SSD1306
{
    protected: // accessed by member functions and derived classes.
        TWI_T TWI; // Reference to an instance of the TWI/I2C interface.
    private: // accessed by member functions but not derived classes.

    public: // accessed by anybody.
        SSD1306(){} // Default Constructor
        ~SSD1306(){}  // Destructor
        void disp_setup(uint8_t OLED_ADDR);
        void clear_buffer(uint8_t OLED_ADDR);
        void setAddressMode(uint8_t OLED_ADDR, uint8_t mode);
        void setCursor(uint8_t OLED_ADDR, uint8_t row_start = 0x00, uint8_t col_start = 0x00, uint8_t col_end = 0x7F, uint8_t row_end = 0x07);
        void setPageStartCol(uint8_t OLED_ADDR, uint8_t pscol);
        void PROGMEMwriteBuf(uint8_t OLED_ADDR, const uint8_t* buffer_to_write);
        void writeLine(uint8_t OLED_ADDR, const uint8_t* buffer_name, uint8_t buffer_length, const uint8_t* symbol_buffer);

};

template< typename TWI_T >
void SSD1306<TWI_T>::disp_setup(uint8_t OLED_ADDR){
    // Initialisation of OLED Display
    TWI.start(OLED_ADDR); // Start Transmitting
    // Initialisation Based upon Application Note Example
    TWI.send(0x00);                   // Send Command Byte Stream
    // --
    TWI.send(0xAE);                   // Turn Display Off
    // ---
    TWI.send(0xA8);                   // Set Multiplex Ratio
    TWI.send(0x3f);                   // 1/64 duty cycle
    // ---
    TWI.send(0xD3);                   // Set Display Offset
    TWI.send(0x00);                   // No offset applied
    // ---
    TWI.send(0x40);                   // Set Display Start Line #0
    // ---
    TWI.send(0xA1);                   // Set Segment Remap (Flips Display) A0/A1
    // ---
    TWI.send(0xC8);                   // COM Scan Direction c0/c8
    // ---
    TWI.send(0xDA);                   // Set COM pins
    TWI.send(0x12);                   // 0x12 - See Application Note SW INIT
    // ---
    TWI.send(0x81);                   // Set Contrast
    TWI.send(0x7F);                   // Default Contrast Level 127/255
    // ---
    TWI.send(0xA4);                   // Entire Display On - Output follows RAM Contents
    // ---
    TWI.send(0xA6);                   // Set Normal Display 0xA7 inverts
    // ---
    TWI.send(0xD5);                   // Display Clk Div
    TWI.send(0x80);                   // Default Value 0x80
    // ---
    TWI.send(0x8D);                   // Setup Charge Pump - See SSD1306 Application Note
    TWI.send(0x14);                   // Enable Charge Pump during display on.
    // ---
    TWI.send(0xD9);                   // Setup Precharge
    TWI.send(0x22);                   //
    // ---
    TWI.send(0xDB);                   //VCOMH DESELECT
    TWI.send(0x30);                   //
    // ---
    TWI.send(0x20);                   // Set Mem Addr Mode
    TWI.send(0x00);                   // Horzontal

    TWI.send(0xAF);                   // Display On
    // ---
    TWI.stop(); // Stop Transmitting
}

template< typename TWI_T >
void SSD1306<TWI_T>::clear_buffer(uint8_t OLED_ADDR){
    // Clear GFX Buffer
    TWI.start(OLED_ADDR);
    TWI.send(0x00);   // Control Byte Command Stream
    TWI.send(0x21);    // Setup Column Addresses
    TWI.send(0x00);    // Col Start Addr
    TWI.send(0x7F);    // Col End Addr
    TWI.send(0x22);    // Set Page Addr
    TWI.send(0x00);    // Start Page 0
    TWI.send(0x07);    // End Page 7
    TWI.stop();

    for (uint16_t i=0; i<1024; i++){
        TWI.start(OLED_ADDR);
        TWI.send(0x40);      // Control Byte Data Stream
        for (uint8_t x=0; x<16; x++) {
            TWI.send(0x00);
            i++;
        }
        i--;
        TWI.stop();
    }
}

template< typename TWI_T >
void SSD1306<TWI_T>::setAddressMode(uint8_t OLED_ADDR, uint8_t mode){
    // 0 = Horizontal (Default/Fallback)
    // 1 = Vertical
    // 2 = Page
    TWI.start(OLED_ADDR);
    TWI.send(0x00);       // Control Byte Command Stream
    TWI.send(0x20);       // Setup Address Mode
    if (mode == 0){         // Setup Horizontal Mode
        TWI.send(0x00);   // Horizontal
    }
    else if (mode == 1){    // Setup Vertical Mode
        TWI.send(0x01);   // Vertical
    }
    else if (mode == 2){    // Setup Page Mode
        TWI.send(0x02);   // Page
    }
    else{                   // Setup Horizontal Mode as fallback option
        TWI.send(0x00);   // Horizontal
    }
    TWI.stop();
}

template< typename TWI_T >
void SSD1306<TWI_T>::setCursor(uint8_t OLED_ADDR, uint8_t row_start, uint8_t col_start, uint8_t col_end, uint8_t row_end){
    // Set Cursor Position
    TWI.start(OLED_ADDR);
    TWI.send(0x00); // Control Byte Command Stream
    TWI.send(0x21); // Set Column Address
    TWI.send(col_start); // Start at column 0
    TWI.send(col_end); // End at column 127
    TWI.send(0x22); // Set Page (Row) address
    TWI.send(row_start); // Start Page
    TWI.send(row_end); // End Page
    TWI.stop();
}

template< typename TWI_T >
void SSD1306<TWI_T>::setPageStartCol(uint8_t OLED_ADDR, uint8_t pscol){
    uint8_t upper_nibble = (((pscol & 0xF0)>>4)|0x10);
    uint8_t lower_nibble = pscol & 0x0F;
    TWI.start(OLED_ADDR);
    TWI.send(0x00); // Control Byte Command Stream
    TWI.send(lower_nibble); // Set Lower Col Start Addr (Page Addr Mode)
    TWI.send(upper_nibble); // Set Upper Col Start Addr (Page Addr Mode)
    TWI.stop();
    }

template< typename TWI_T >
void SSD1306<TWI_T>::PROGMEMwriteBuf(uint8_t OLED_ADDR, const uint8_t* buffer_to_write)
{
    for (uint16_t i=0; i<1024; i++){
        TWI.start(OLED_ADDR);
        TWI.send(0x40);      // Control Byte Data Stream
        for (uint8_t x=0; x<16; x++) {
            TWI.send(pgm_read_byte(&buffer_to_write[i]));
            i++;
        }
        i--;
        TWI.stop();
    }
}

template< typename TWI_T >
void SSD1306<TWI_T>::writeLine(uint8_t OLED_ADDR, const uint8_t* buffer_name, uint8_t buffer_length, const uint8_t* symbol_buffer){
    // Write Text Display Line
    // Note: Set cursor position before writing.

    // For each item in data buffer
    for (uint16_t i=0; i<buffer_length; i++){
        TWI.start(OLED_ADDR);
        TWI.send(0x40);                   // Control Byte Data Stream
        // Multiply by 6 to get char start position in the ascii_buffer
        uint16_t offset = pgm_read_byte(&buffer_name[i]) * 6;
        // Print each byte that makes up the character
        for ( uint16_t x =0; x<6; x++) {
            TWI.send(pgm_read_byte(&symbol_buffer[offset+x]));
        }
        TWI.stop();
    }
    // Note: Reset the cursor position to Row 0, Col 0. Otherwise the
    //       buffers become offset on the next loop.
}


#endif // End of ifndef SSD1306_H
