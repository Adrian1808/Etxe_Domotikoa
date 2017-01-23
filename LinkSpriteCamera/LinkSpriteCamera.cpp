/*
  LinkSpriteCamera.cpp - Library for using Link Sprite JPEG Color Camera
                       serial UART Interface with Infrared
  Created by Oskar Casquero and Adrian Ruiz, November 9, 2016.
  Released into the public domain.
*/

#include "LinkSpriteCamera.h"

byte RESET_CAMERA[4] = {0x56, 0x00, 0x26, 0x00};
byte TAKE_PICTURE[5] = {0x56, 0x00, 0x36, 0x01, 0x00};
byte READ_FILE_SIZE[5] = {0x56, 0x00, 0x34, 0x01, 0x00};
byte READ_FILE_CONTENT[8] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00};
byte STOP_TAKING_PICTURES[5] = {0x56, 0x00, 0x36, 0x01, 0x03};
byte CHANGE_BAUD_RATE[5] = {0x56, 0x00, 0x24, 0x03, 0x01};

LinkSpriteCamera::LinkSpriteCamera(HardwareSerial* hwSerial) {
	_serial = hwSerial;
	_hwSerial = hwSerial;
	_swSerial = NULL;
}

LinkSpriteCamera::LinkSpriteCamera(SoftwareSerial* swSerial) {
	_serial = swSerial;
	_hwSerial = NULL;
	_swSerial = swSerial;
}

void LinkSpriteCamera::begin(unsigned long baudrate) {
  delay(1000);  // one second delay to let the sensor 'boot up'

  if(_hwSerial) 
    _hwSerial->begin(baudrate);
  else if(_swSerial) 
    _swSerial->begin(baudrate);
}

void LinkSpriteCamera::reset() {
  sendCommand(RESET_CAMERA, sizeof(RESET_CAMERA));  
  Serial.println();
  readResponse(71);
}

void LinkSpriteCamera::takePicture() {
  sendCommand(TAKE_PICTURE, sizeof(TAKE_PICTURE));
  Serial.println();
  readResponse(5);
}

unsigned int LinkSpriteCamera::readFileSize() {
  sendCommand(READ_FILE_SIZE, sizeof(READ_FILE_SIZE));
  Serial.println();
  readResponse(7);
  
  int i=0;
  byte b[2];
  while(i<2) {
    if(_serial->available()>0) {
      b[i] = _serial->read();
	  //printByte(b);
      i++;
    }
  }
  
  unsigned int fileSize = (b[0] << 8) | b[1];
 
  return fileSize; 
}

int LinkSpriteCamera::readFileContent(byte* chunk, unsigned int fileAddress, int chunkSize) {
  sendCommand(READ_FILE_CONTENT, sizeof(READ_FILE_CONTENT));
  
  _serial->write((byte)(fileAddress>>8));
  _serial->write((byte)fileAddress);
  _serial->write((byte)0x00);
  _serial->write((byte)0x00);
  _serial->write((byte)(chunkSize>>8));
  _serial->write((byte)chunkSize);
  _serial->write((byte)0x00);
  _serial->write((byte)0x0A);
  
  //printByte((byte)(fileAddress>>8));
  //printByte((byte)fileAddress);
  //printByte((byte)0x00);
  //printByte((byte)0x00);
  //printByte((byte)(chunkSize>>8));
  //printByte((byte)chunkSize);
  //printByte((byte)0x00);
  //printByte((byte)0x0A);
  
  
  readResponse(5);
    
  int i=0;
  int cont=0;
  boolean eof=false;
  while(i<chunkSize) {
    if(_serial->available()>0) {
      if(!eof) {
	    *chunk = _serial->read();
    	if((*(chunk-1)==0xFF) && (*chunk==0xD9))
          eof=true;
      	//printByte(*chunk);
        chunk++;
        cont++;
      } else {
      	_serial->read();
      }
      i++;
    }
  }
  
  readResponse(5);
  
  return cont;
}

void LinkSpriteCamera::stopTakingPictures() {
  sendCommand(STOP_TAKING_PICTURES, sizeof(STOP_TAKING_PICTURES));
  Serial.println();
  readResponse(5);
}

void LinkSpriteCamera::changeBaudRate(unsigned long baudrate) {
  sendCommand(CHANGE_BAUD_RATE, sizeof(CHANGE_BAUD_RATE));
  
  if(baudrate==9600){
	_serial->write((byte)0xAE);
  	_serial->write((byte)0xC8);
  } else if(baudrate==19200) {
  	_serial->write((byte)0x56);
  	_serial->write((byte)0xE4);
  } else if(baudrate==38400) {
  	_serial->write((byte)0x2A);
  	_serial->write((byte)0xF2);
  } else if(baudrate==57600) {
  	_serial->write((byte)0x1C);
  	_serial->write((byte)0x4C);
  } else if(baudrate==115200) {
  	_serial->write((byte)0x0D);
  	_serial->write((byte)0xA6);
  }
  
  Serial.println();
  
  readResponse(5);
  
  begin(baudrate);
}

void LinkSpriteCamera::sendCommand(byte* command, int command_size) {
  for(int i=0; i<command_size; i++) {
    _serial->write(*command);
    //printByte(*command);
    command++;
  }
}

void LinkSpriteCamera::readResponse(int length) {
  int i=0;
  while(i<length) {
    if(_serial->available()>0) {
      byte b = _serial->read();
      //printByte(b);
      i++;
    }
  }
}
  
void LinkSpriteCamera::printByte(byte b){
  if(b < 0x10)
	Serial.print("0");
  Serial.print(b, HEX); Serial.print(" ");
}

