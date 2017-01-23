/* 
	FPS_Enroll.ino - Library example for controlling the GT-511C3 Finger Print Scanner (FPS)
	Created by Josh Hawley, July 23rd 2013
	Licensed for non-commercial use, must include this license message
	basically, Feel free to hack away at it, but just give me credit for my work =)
	TLDR; Wil Wheaton's Law

	This simple sketch turns the LED on and off similar to the Arduino blink sketch.
	It is used to show that communications are working.
*/

#include "FPS_GT511C3.h"

// if you are using software serial, uncomment the following two lines
//SoftwareSerial swSerial(10, 11); // RX, TX
//FPS_GT511C3 fps(&swSerial);

// if you are using hardware serial, uncomment the following line
FPS_GT511C3 fps(&Serial1);

void setup()
{
	Serial.begin(9600);
	fps.UseSerialDebug = true; // so you can see the messages in the serial debug screen
	fps.Open();
}


void loop()
{
	// FPS Blink LED Test
	fps.SetLED(true); // turn on the LED inside the fps
	delay(1000);
	fps.SetLED(false);// turn off the LED inside the fps
	delay(1000);
}
