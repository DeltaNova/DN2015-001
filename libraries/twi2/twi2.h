// twi.h - Header file for twi.cpp
// Library for  TWI/I2C Interface
// Author: M. Tunstall


#ifndef TWI2_H //If twi.h hasn't been included elsewhere, define it now.
#define TWI2_H
// ---------------------------------------------------------------------
// DEV NOTE: Move the references to the Arduino Wire library into class
//           functions. Aim is to make it easier to switch primary code
//           and library to another i2c/twi library at a later date.
// ---------------------------------------------------------------------
// DEV NOTE - DEC 2015
//          Library ATMega Specific Currently
//
// ---------------------------------------------------------------------

#include <stdint.h>         // Enable fixed width integers.
#include <avr/io.h> // Needed for register references
#include <Wire.h>           // Arduino TWI Library

class TWI
{
    protected: // accessed by member functions and derived classes.

    private: // accessed by member functions but not derived classes.

    public: // accessed by anybody.
        TWI() // Default Constructor
        {
            // Anything that needs to be executed when the class
            // is instantiated.
        }
        ~TWI(){}  // Destructor
    void init();
    void setSCLfreq(uint8_t TWBR);
    void start(uint8_t address);
    void send(uint8_t byte);
    void stop();
};

void TWI::init(){
    Wire.begin();
};

void TWI::setSCLfreq(uint8_t TWBRval){
    // Set SCL Freq
    // TODO: finish
    TWBR = TWBRval; // Set TWBR Register
};

void TWI::start(uint8_t address){
    Wire.beginTransmission(address);
};

void TWI::send(uint8_t byte){
    Wire.write(byte);
};

void TWI::stop(){
    Wire.endTransmission();
};

#endif // End of ifndef TWI_H
