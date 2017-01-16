/* 
	FPS_Enroll.ino - Library example for controlling the GT-511C3 Finger Print Scanner (FPS)
	Created by Josh Hawley, July 23rd 2013
	Licensed for non-commercial use, must include this license message
	basically, Feel free to hack away at it, but just give me credit for my work =)
	TLDR; Wil Wheaton's Law

	This sketch will attempt to identify a previously enrolled fingerprint.
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
	delay(100);
	fps.Open();
	fps.SetLED(true);
}

void loop()
{

	// Identify fingerprint test
	if (fps.IsPressFinger())
	{
		fps.CaptureFinger(false);
		int id = fps.Identify1_N();
		if (id <200)
		{
			Serial.print("Verified ID:");
			Serial.println(id);
		}
		else
		{
			Serial.println("Finger not found");
		}
	}
	else
	{
		Serial.println("Please press finger");
	}
	delay(100);
}