#include <Adafruit_PN532.h>
#include "RFID_funtzioak.h"
uint8_t success;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;

void RFID_abiarazi(Adafruit_PN532* nfc) {
	nfc->begin();
	uint32_t versiondata = nfc->getFirmwareVersion();
	if(!versiondata) {
		Serial.println(F("Ez da RFID txartel irakurlea aurkitu"));
		while(1);
	}
	nfc->SAMConfig(); // configure board to read RFID tags
	Serial.println(F("RFID abiarazi da"));
}

void RFID_irakurri(Adafruit_PN532* nfc, uint8_t* data_aux) {
	success = nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30);
	if(success) {
		Serial.println(F("ISO14443A txartela aurkitu da"));
		if(uidLength == 4) {
			uint8_t keya[6] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
			success = nfc->mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
			if(success) {
				Serial.println(F("Sector 1 (Blocks 4..7) has been authenticated"));
				uint8_t data[16];
				success = nfc->mifareclassic_ReadDataBlock(4, data);
				if(success) {
					Serial.print(F("Reading Block 4: ")); 
					for(int i=0; i<16; i++) {
						Serial.print(char(data[i]));
						*data_aux = data[i];
						data_aux++;
					}
					Serial.println("");
					data_aux -= 16;
				} else {
					Serial.println(F("Block 4 could not be read"));
					*data_aux = '#'; 
				}
		    } else {
				Serial.println(F("Sector 1 (Blocks 4..7) could not be authenticated"));
				*data_aux = '$';
			}
		} else {
			Serial.println(F("Card UID is not 4 bytes long"));
			*data_aux = '%';
		}
    } else {
		//Serial.println(F("Card could not be read"));
		*data_aux = '&';
	}
}