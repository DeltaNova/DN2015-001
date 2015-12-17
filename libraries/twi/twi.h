// twi.h - Header file for twi.cpp
// Library for  TWI/I2C Interface
// Author: M. Tunstall
#include <stdint.h>         // Enable fixed width integers.
#include <Wire.h>           // Arduino TWI Library

#ifndef TWI_H //If twi.h hasn't been included elsewhere, define it now.
#define TWI_H


// Content goes here


// DEV NOTE: Move the references to the Arduino Wire library into class
//           functions. Aim is to make it easier to switch primary code
//           and library to another i2c/twi library at a later date.


class TWI
{
    protected: // accessed by member functions and derived classes.

    private: // accessed by member functions but not derived classes.

    public: // accessed by anybody.
        TWI() // Default Constructor
        {

        }
        ~TWI(){}  // Destructor

};

#endif // End of ifndef TWI_H
