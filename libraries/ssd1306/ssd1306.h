// ssd1306.h - Header file for ssd1306.cpp
// Library for SSD1306 OLED Display Driver
// Author: M. Tunstall
#include <stdint.h>         // Enable fixed width integers.


#ifndef SSD1306_H //If ssd1306.h hasn't been included elsewhere, define it now.
#define SSD1306_H


// Content goes here


// DEV NOTE: Move the references to the Arduino Wire library into class
//           functions. Aim is to make it easier to switch primary code
//           and library to another i2c/twi library at a later date.


class SSD1306
{
    protected: // accessed by member functions and derived classes.

    private: // accessed by member functions but not derived classes.

    public: // accessed by anybody.
        SSD1306() // Default Constructor
        {

        }
        ~SSD1306(){}  // Destructor

};

#endif // End of ifndef SSD1306_H
