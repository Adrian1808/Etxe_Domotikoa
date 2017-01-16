/*
  RTC_DS1307_Demo.ino - Demo for using LinkSpriteCameraDemo library
                             for using Link Sprite JPEG Color Camera
                             serial UART Interface with Infrared
  Created by Oskar Casquero and Adrian Ruiz, November 9, 2016.
  Released into the public domain.
*/

#include <RTC_DS1307.h>
#include "Wire.h"

RTC_DS1307 rtc;

void setup() {
  while(!Serial); //for Yun and Leonardo
  Serial.begin(115200);
  
  Wire.begin();

  rtc.setTime(50,10,15,5,22,6,79);
}

void loop() {
  byte seconds, minutes, hours, day, date, month, year;
  
  rtc.getTime(&seconds, &minutes, &hours, &day, &date, &month, &year);

  Serial.print(day); Serial.print(", ");
  Serial.print(date); Serial.print("/"); Serial.print(month); Serial.print("/"); Serial.print(year); Serial.print(", ");
  Serial.print(hours); Serial.print(":"); Serial.print(minutes); Serial.print(":"); Serial.println(seconds);;
}