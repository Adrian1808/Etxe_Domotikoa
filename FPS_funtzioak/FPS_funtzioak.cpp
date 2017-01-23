#include <FPS_GT511C3.h>
#include "TFT_funtzioak.h"
uint8_t id0[16] = {' ',' ',' ','A','D','R','I','A','N'};
uint8_t id1[16] = {' ',' ','V','I','O','L','E','T','A'};
uint8_t id2[16] = {' ',' ',' ',' ','I','R','E','N','E'};
uint8_t id3[16] = {' ',' ',' ',' ','J','O','N',' ',' ',};
uint8_t id4[16] = {' ',' ',' ',' ','A','S','I','S',' ',};
uint8_t id5[16] = {' ',' ',' ',' ','L','A','G','U','N'};

void FPS_abiarazi(FPS_GT511C3* fps) {
	fps->Open();
	fps->SetLED(true);
	Serial.println(F("FPS abiarazi da"));
}

void FPS_irakurri(FPS_GT511C3* fps, uint8_t* data_aux) {
	if(fps->IsPressFinger()) {
		fps->CaptureFinger(false); 
		int id = fps->Identify1_N();
		if(id < 200) {
			Serial.print(F("Reading FPS ID: ")); Serial.print(id); Serial.print(" ");
			uint8_t* id_aux;
			if(id==0) {
				id_aux = id0;
			}
			if(id==1) {
				id_aux = id1;
			}
			else if(id==2) {
				id_aux = id2;
			}
			else if(id==3) {
				id_aux = id3;
			}
			else if(id==4) {
				id_aux = id4;
			}
			else if(4<id<10) {
				id_aux = id5;
			}
			for(int i=0; i<16; i++) {
				if(i<16) {
					Serial.print(char(*id_aux));
					*data_aux = *id_aux;
				} else {
					Serial.print(" ");
					*data_aux = ' ';
				}
				id_aux++;
			  	data_aux++;
			}
			Serial.println("");
			data_aux -= 16;
		} else {
			Serial.println(F("Finger not enrolled"));
			*data_aux = '%';
		}
	} else {
		//Serial.println(F("Finger not pressed"));
		*data_aux = '&';
	}
}
void FPS_Berria(Adafruit_ILI9341* tft,FPS_GT511C3* fps)
{
	// Enroll test

	// find open enroll id
	int enrollid = 0;
	bool usedid = true;
	while (usedid == true){
		usedid = fps->CheckEnrolled(enrollid);
		if (usedid==true) enrollid++;
	}
	fps->EnrollStart(enrollid);
	// enroll
	Serial.print("Jarri HATZ-a gordetzeko #");
	Serial.println(enrollid);
	TFT_mezua(tft, "Jarri HATZ-a gordetzeko #");
	delay(500);
	while(fps->IsPressFinger() == false) delay(100);
	bool bret = fps->CaptureFinger(true);
	int iret = 0;
	if (bret != false){
		Serial.println("Kendu HATZ-a");
		TFT_mezua(tft, "Kendu HATZ-a");
		delay(500);
		fps->Enroll1(); 
		while(fps->IsPressFinger() == true) delay(100);
			Serial.println("Jarri HATZ berdina (2)");
			TFT_mezua(tft, "Jarri HATZ berdina (2)");
			delay(500);
		while(fps->IsPressFinger() == false) delay(100);
			bret = fps->CaptureFinger(true);
			if (bret != false){
				Serial.println("Kendu hatz-a");
				TFT_mezua(tft, "Kendu hatz-a");
				delay(500);
				fps->Enroll2();
				while(fps->IsPressFinger() == true) delay(100);
					Serial.println("Jarri HATZ berdina berriro(3)");
					TFT_mezua(tft, "Jarri HATZ berdina berriro (3)");
					delay(500);
					while(fps->IsPressFinger() == false) delay(100);
					bret = fps->CaptureFinger(true);
						if (bret != false){
							Serial.println("Kendu hatz-a");
							TFT_mezua(tft, "Kendu hatz-a");
							delay(500);
							iret = fps->Enroll3();
							if (iret == 0){
								Serial.println("HATZ berria gordeta");
								TFT_mezua(tft, "HATZ berria gordeta");
								delay(1000);
							}else{
								Serial.print("Ez da ondo gorde:");
								TFT_mezua(tft, "Ez da ondo gorde");
								delay(500);
								Serial.println(iret);
							}
						}else{ Serial.println("Failed to capture third finger");
							TFT_mezua(tft, "Akatsa 3. gordeketan");
							delay(500);
						}
			}
			else {Serial.println("Failed to capture second finger");
				TFT_mezua(tft, "Akatsa 2. gordeketan");
				delay(500);}
	}
	else{ Serial.println("Failed to capture first finger");
		TFT_mezua(tft, "Akatsa 1. gordeketan");
		delay(500);
	}
}

void FPS_ID(Adafruit_ILI9341* tft,FPS_GT511C3* fps,int* f_printed_konf_sarrera){
	TFT_mezua(tft, "HATZ-a jarri");
	delay(500);
	f_printed_konf_sarrera=0;
	while(!f_printed_konf_sarrera){
		if (fps->IsPressFinger()){
			fps->CaptureFinger(false);
			int id = fps->Identify1_N();
			if (id <200){
				Serial.print("Verified ID:");
				tft->fillScreen(ILI9341_BLACK);
				tft->setCursor(100,115);
				tft->setTextSize(4);
				tft->print("ID:");
				tft->print(id);
				Serial.println(id);
				delay(1000);
				f_printed_konf_sarrera++;
			}else{
				Serial.println("Finger not found");
				TFT_mezua(tft, "Ez da aurkitu");
				delay(500);
				TFT_mezua(tft, "HATZ-a jarri");
			}
		}
	}
	f_printed_konf_sarrera=0;
	delay(100);	
}

void FPS_Ezabatu(Adafruit_ILI9341* tft,FPS_GT511C3* fps,int* f_printed_konf_sarrera){
	TFT_mezua(tft, "HATZ-a jarri");
	delay(500);
	f_printed_konf_sarrera=0;
	while(!f_printed_konf_sarrera){
		if (fps->IsPressFinger()){
			fps->CaptureFinger(false);
			int id = fps->Identify1_N();
			if (id <200){
				Serial.print("Verified ID:");
				TFT_mezua(tft, "Identifikatu ID:");
				delay(500);
				Serial.println(id);
				fps->DeleteID(id);
				Serial.print("Deleted ID:");
				TFT_mezua(tft, "Ezabatu ID:");
				delay(1000);
				f_printed_konf_sarrera++;		
			}else{
				Serial.println("Finger not found");
				TFT_mezua(tft, "Ez da aurkitu HATZ-ik");
				delay(500);
				TFT_mezua(tft, "HATZ-a jarri");
			}
		}
	}
	f_printed_konf_sarrera=0;  
	delay(100);
}