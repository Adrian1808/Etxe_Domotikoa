#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <LinkSpriteCamera.h>
#include <RTC_DS1307.h>

#define CHUNK_SIZE 64

void LSC_abiarazi(LinkSpriteCamera* camera) {
  Serial.println("CAMERA: Begin");
  camera->begin(38400);
  Serial.print("CAMERA: Reset");
  camera->reset();
}

void LSC_argazkia_atera(LinkSpriteCamera* camera, RTC_DS1307* rtc) {
	Serial.print("CAMERA: Take Picture");
	camera->takePicture();
	
	byte seconds, minutes, hours, day, date, month, year;
	rtc->getTime(&seconds, &minutes, &hours, &day, &date, &month, &year);
	String folder_name = String(year) + String("_") + String(month) + String("_") + String(date);
	String file_name = String(hours) + String("_") + String(minutes) + String("_") + String(seconds) + String(".jpg");
	Serial.print(folder_name); Serial.println(" "); Serial.println(file_name);

    if(!SD.exists(folder_name))	{
		SD.mkdir(folder_name);
	}
	File dataFile = SD.open(folder_name+String("/")+file_name, FILE_WRITE);

	Serial.print("CAMERA: Read File Size");
	unsigned int fileSize = camera->readFileSize();
	Serial.print(fileSize); Serial.println();
	
	Serial.println("CAMERA: Read File Content");
	byte chunk[CHUNK_SIZE];
	unsigned int fileAddress = 0;
	while(fileAddress < fileSize){
	    int cont = camera->readFileContent(chunk, fileAddress, CHUNK_SIZE);
	    for(int i=0; i<cont; i++) {
			dataFile.write(chunk[i]);
	    }
	    fileAddress+=CHUNK_SIZE;
		Serial.print(".");
	}
	dataFile.close();
	Serial.println("");
	
	Serial.print("CAMERA: Stop Taking Pictures");
	camera->stopTakingPictures();
}