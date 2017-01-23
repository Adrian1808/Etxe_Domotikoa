#include "TFT_funtzioak.h"
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

void TFT_abiarazi(Adafruit_ILI9341* tft) {
	tft->begin();
	tft->fillScreen(ILI9341_BLACK);
	tft->setRotation(45);
	Serial.println("TFT abiarazi da");
}

void TFT_sarrera_baliogabea(Adafruit_ILI9341* tft) {
	tft->fillScreen(ILI9341_BLACK);
	tft->fillCircle(170, 120, 80, ILI9341_RED);
    tft->setCursor(115,110);
    tft->setTextSize(2);
    tft->setTextColor(ILI9341_WHITE);
    tft->println("Ezezaguna");
}

void TFT_sarrera_baliozkoa(Adafruit_ILI9341* tft, uint8_t* data) {
	tft->fillScreen(ILI9341_BLACK);
	tft->fillCircle(170, 120, 80, ILI9341_GREEN);
	tft->setCursor(100,115);
	tft->setTextSize(2);
	tft->setTextColor(ILI9341_WHITE);
	for(int i=0; i<16; i++) { 
		tft->print(char(*data));
		data++;
	}
}

void TFT_teklatua_marraztu(Adafruit_ILI9341* tft) {
	tft->fillScreen(ILI9341_BLACK);
	//Marrak
	tft->fillRect(0, 0, 320, 10, ILI9341_WHITE);//GOI_L_HOR
	tft->fillRect(0, 230, 320, 10, ILI9341_WHITE);//BEH_L_HOR
	tft->fillRect(0, 0, 10, 240, ILI9341_WHITE);//EZK_L_BER
	tft->fillRect(310, 0, 10, 240, ILI9341_WHITE);//ESK_L_BER
	tft->fillRect(108, 0, 5, 240, ILI9341_WHITE);//ESK_BarneL_BER
	tft->fillRect(212, 0, 5, 240, ILI9341_WHITE);//EZK_BarneL_BER
	tft->fillRect(0, 154, 320, 5, ILI9341_WHITE);//GOI_BarneL_HOR
	tft->fillRect(0, 78, 320, 5, ILI9341_WHITE);//GOI_BarneL_HOR
	//Zenbakiak
	int i, j = 0;
	int x[3] = {46, 148, 248};
	int y[3] = {26, 100, 174};
	for(int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			int zenbakia = i + j*3 + 1;
			tft->setCursor(x[i], y[j]);
			tft->setTextSize(6);
			tft->setTextColor(ILI9341_WHITE);
			tft->print(zenbakia);
		}
	}
}

void TFT_zenbakia_berrestu(Adafruit_ILI9341* tft, char zenbakia) {
	tft->fillScreen(ILI9341_BLACK);
	tft->setCursor(130,90);
	tft->setTextSize(12);
	tft->setTextColor (ILI9341_ORANGE);
	tft->print(zenbakia);
	delay(300);
}

void TFT_mezua(Adafruit_ILI9341* tft, String mezua) {
	tft->fillScreen(ILI9341_BLACK);
    tft->setCursor(50,110);
    tft->setTextSize(2);
    tft->setTextColor(ILI9341_WHITE);
    tft->println(mezua);
}

void TFT_konf_etxea(Adafruit_ILI9341* tft) {
 	tft->fillScreen(ILI9341_BLACK);
 	// 1 Fila
 	tft->fillRect(25, 25, 120, 80, ILI9341_ORANGE);
 	tft->setCursor(44,60);
 	tft->setTextSize(2);
 	tft->setTextColor (ILI9341_WHITE);
 	tft->println("SARRERA");
 	tft->fillRect(175, 25, 120, 80, ILI9341_PURPLE);
 	tft->setCursor(188,60);
 	tft->setTextSize(2);
 	tft->setTextColor (ILI9341_WHITE);
 	tft->println("EGONGELA");
 	//2 Fila
 	tft->fillRect(100, 140, 120, 80, ILI9341_BLUE);
 	tft->setCursor(124,175);
 	tft->setTextSize(2);
 	tft->setTextColor (ILI9341_WHITE);
 	tft->println("LOGELA");
}

void TFT_sarrera(Adafruit_ILI9341* tft){
	tft->fillScreen(ILI9341_BLACK);
	// 1 Fila
	tft->fillRect(35,35,85,65,ILI9341_BLUE);
	tft->setCursor(55,60);
	tft->setTextSize(2);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("RFID");
	tft->fillRect(150,35,85,65,ILI9341_ORANGE);
	tft->setCursor(170,60);
	tft->setTextSize(2);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("HATZ");
	//2 Fila
	tft->fillRect(35,150,85,65,ILI9341_MAROON);
	tft->setCursor(55,175);
	tft->setTextSize(2);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("ATEA");
	tft->fillRect(150,150,85,65,ILI9341_OLIVE);
	tft->setCursor(170,175);
	tft->setTextSize(2);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("PASS");
	TFT_itzuli(tft);
	
}

void TFT_sarrera_RFID(Adafruit_ILI9341* tft){
	tft->fillScreen(ILI9341_BLACK);  
	tft->fillCircle(170, 115, 75, ILI9341_ORANGE);
	tft->setCursor(145,100);
	tft->setTextSize(4);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("ID");
	TFT_itzuli(tft);
}
	
void TFT_sarrera_FPS(Adafruit_ILI9341* tft){
	tft->fillScreen(ILI9341_BLACK);
	tft->fillCircle(50,120,45,(ILI9341_GREEN));
	tft->setCursor(15,115);
	tft->setTextSize(2);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("BERRIA");
	tft->fillCircle(160,120,45,(ILI9341_ORANGE));
	tft->setCursor(145,115);
	tft->setTextSize(2);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("ID");
	tft->fillCircle(270,120,45,(ILI9341_RED));
	tft->setCursor(230,115);
	tft->setTextSize(2);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("EZABATU");
	TFT_itzuli(tft);
}
void TFT_atea(Adafruit_ILI9341* tft){
	tft->fillScreen(ILI9341_BLACK);
	//Igo Botoia
	tft->fillCircle(100,120,50,(ILI9341_GREEN));
	tft->setCursor(70,115);
	tft->setTextSize(2);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("IREKI");
	//Jeitsi Botoia
	tft->fillCircle(230,120,50,(ILI9341_RED));
	tft->setCursor(205,115);
	tft->setTextSize(2);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("ITXI");
	TFT_itzuli(tft);
}
void TFT_pasahitza(Adafruit_ILI9341* tft){
	tft->fillScreen(ILI9341_BLACK);
	//Berria Botoia
	tft->fillCircle(100,120,50,(ILI9341_GREEN));
	tft->setCursor(65,115);
	tft->setTextSize(2);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("BERRIA");
	//ID Botoia
	tft->fillCircle(230,120,50,(ILI9341_ORANGE));
	tft->setCursor(210,112);
	tft->setTextSize(3);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("ID");
	TFT_itzuli(tft);
}

void TFT_egongela (Adafruit_ILI9341* tft){
	tft->fillScreen(ILI9341_BLACK);
	//Igo Botoia
	tft->fillCircle(100,120,50,(ILI9341_GREEN));
	tft->setCursor(70,110);
	tft->setTextSize(3);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("IGO");
	//Jeitsi Botoia
	tft->fillCircle(230,120,50,(ILI9341_RED));
	tft->setCursor(195,110);
	tft->setTextSize(2);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("JAITSI");
	TFT_itzuli(tft);
}

void TFT_logela(Adafruit_ILI9341* tft) {
	tft->fillScreen(ILI9341_BLACK);
	tft->fillCircle(170, 115, 75, ILI9341_DARKCYAN);
	tft->setCursor(100,110);
	tft->setTextSize(3);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("ARGAZKIA");
	TFT_itzuli(tft);
}

void TFT_itzuli(Adafruit_ILI9341* tft){
	tft->fillRect(240, 200, 80, 40, ILI9341_MAGENTA);
	tft->setCursor(245,212);
	tft->setTextSize(2);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("ITZULI");
}
void TFT_alarma(Adafruit_ILI9341* tft){
	tft->fillScreen(ILI9341_BLACK);
	tft->fillCircle(170, 115, 75, ILI9341_RED);
	tft->setCursor(115,110);
	tft->setTextSize(3);
	tft->setTextColor (ILI9341_WHITE);
	tft->println("ALARMA");
}