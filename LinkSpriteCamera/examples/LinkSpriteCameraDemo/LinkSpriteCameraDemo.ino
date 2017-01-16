/*
  LinkSpriteCameraDemo.ino - Demo for using LinkSpriteCameraDemo library
                             for using Link Sprite JPEG Color Camera
                             serial UART Interface with Infrared
  Created by Oskar Casquero and Adrian Ruiz, November 9, 2016.
  Released into the public domain.
*/

#include <LinkSpriteCamera.h>

int CHUNK_SIZE = 64;

// if you are using software serial, uncomment the following two lines
//SoftwareSerial swSerial(10, 11); // RX, TX
//LinkSpriteCamera camera(&swSerial);

// if you are using hardware serial, uncomment the following line
LinkSpriteCamera camera(&Serial1);

void setup() {
  while(!Serial); //for Yun and Leonardo
  Serial.begin(115200);
  
  Serial.println("CAMERA: Begin");
  camera.begin(38400);
  Serial.println();

  Serial.println("CAMERA: Reset");
  camera.reset();
  Serial.println();
}

void loop() {
  Serial.println("CAMERA: Take Picture");
  camera.takePicture();
  Serial.println();

  Serial.println("CAMERA: Read File Size");
  unsigned int fileSize = camera.readFileSize();
  Serial.print(fileSize); Serial.println();

  Serial.println("CAMERA: Read File Content");
  byte chunk[CHUNK_SIZE];
  unsigned int fileAddress = 0;
  while(fileAddress < fileSize) {
    int cont = camera.readFileContent(chunk, fileAddress, CHUNK_SIZE);
    for(int i=0; i<cont; i++) {
      if(chunk[i] < 0x10)
        Serial.print("0");
      Serial.print(chunk[i], HEX); //Serial.print(" ");
    }
    fileAddress+=CHUNK_SIZE;
    Serial.println();
  }

  Serial.println("CAMERA: Stop Taking Pictures");
  camera.stopTakingPictures();
  Serial.println();

  Serial.println("SEND 'c' VIA SERIAL MONITOR TO TAKE ANOTHER PICTURE");
  while(Serial.read() != 'c') {}
}
