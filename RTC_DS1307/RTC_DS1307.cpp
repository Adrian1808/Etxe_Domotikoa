/*
  RTC_DS1307.cpp - Library for using RTC_DS1307
  Created by Oskar Casquero and Adrian Ruiz, December 12, 2016.
  Released into the public domain.
*/

#include "RTC_DS1307.h"
#include "Wire.h"

RTC_DS1307::RTC_DS1307() {
}

void RTC_DS1307::getTime(byte* seconds, byte* minutes, byte* hours,
                         byte* day,
						 byte* date, byte* month, byte* year) {
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission();
  
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
  *seconds = bcd2dec(Wire.read() & 0x7F);
  *minutes = bcd2dec(Wire.read());
  *hours = bcd2dec(Wire.read() & 0x3F);
  *day = bcd2dec(Wire.read());
  *date = bcd2dec(Wire.read());
  *month = bcd2dec(Wire.read());
  *year = bcd2dec(Wire.read());
}

void RTC_DS1307::setTime(byte seconds, byte minutes, byte hours,
                         byte day,
						 byte date, byte month, byte year) {
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0x00); 
  Wire.write(dec2bcd(seconds));
  Wire.write(dec2bcd(minutes));
  Wire.write(dec2bcd(hours) & 0x3F);
  Wire.write(dec2bcd(day)); 
  Wire.write(dec2bcd(date)); 
  Wire.write(dec2bcd(month));
  Wire.write(dec2bcd(year)); 
  Wire.endTransmission();
}

byte RTC_DS1307::bcd2dec(byte bcd) {
  byte units = bcd & 0x0F; 
  byte tens = bcd >> 4;
  
  return (tens*10)+units;
}

byte RTC_DS1307::dec2bcd(byte dec) {
   byte units = dec%10;
   byte tens = (dec/10) << 4;
   
   return tens|units;
}
  