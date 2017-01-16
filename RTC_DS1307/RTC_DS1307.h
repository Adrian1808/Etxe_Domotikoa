/*
  RTC_DS1037.cpp - Library for using RTC_DS1307
  Created by Oskar Casquero and Adrian Ruiz, December 12, 2016.
  Released into the public domain.
*/

#ifndef RTC_DS1037_h
#define RTC_DS1037_h

#include "Arduino.h"

#define DS1307_I2C_ADDRESS B1101000

class RTC_DS1307 {
  public:
    RTC_DS1307();
    void getTime(byte* seconds, byte* minutes, byte* hours,
                 byte* day,
				 byte* date, byte* month, byte* year);
    void setTime(byte seconds, byte minutes, byte hours,
                 byte day,
				 byte date, byte month, byte year);
    
  private:
    byte bcd2dec(byte bcd);
	byte dec2bcd(byte dec);
};

#endif
