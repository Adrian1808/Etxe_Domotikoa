/*
  LinkSpriteCamera.h - Library for using Link Sprite JPEG Color Camera
                       Serial UART Interface with Infrared
  Created by Oskar Casquero and Adrian Ruiz, November 9, 2016.
  Released into the public domain.
*/

#ifndef LinkSpriteCamera_h
#define LinkSpriteCamera_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class LinkSpriteCamera {
  public:
    LinkSpriteCamera(HardwareSerial* hwserial);
    LinkSpriteCamera(SoftwareSerial* swserial);
    void begin(unsigned long baudrate);
    void reset();
    void takePicture();
    unsigned int readFileSize();
    int readFileContent(byte* chunk, unsigned int fileAddress, int chunkSize);
    void stopTakingPictures();
    void changeBaudRate(unsigned long  baudrate);
    
  private:
  	Stream* _serial;
  	HardwareSerial* _hwSerial;
  	SoftwareSerial* _swSerial;
    void sendCommand(byte* command, int command_size);
    void readResponse(int length);
    void printByte(byte b);
};

#endif
