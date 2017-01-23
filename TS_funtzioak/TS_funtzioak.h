#include <Adafruit_ILI9341.h>
#include "Etxea_funtzioak.h"
#include <FPS_GT511C3.h>
#include <Password.h>
#include <Adafruit_PN532.h>
#include <SeeedTouchScreen.h>


char TS_teklatu_kontrola(Adafruit_ILI9341* tft, Password *passwd, int x, int y, int* k_passwd);

void TS_tekla_gorde(Adafruit_ILI9341* tft, Password *passwd, char zenbakia, int* k_passwd);

void TS_Sarrera_Kontrola(Adafruit_ILI9341* tft,TouchScreen* ts,FPS_GT511C3* fps,Adafruit_PN532* nfc,RTC_DS1307* rtc, uint8_t* data,Password *passwd,int* k_passwd, int* k_sarrera,volatile int* f_sarrera, volatile int* f_printed_sarrera,volatile float* t,int* k_numero,int* pertsiana,int x,int y);

void TS_sarrera_RFID_Kontrola (Adafruit_ILI9341* tft,TouchScreen* ts,Adafruit_PN532* nfc,RTC_DS1307* rtc, uint8_t* data,int x, int y, int* f_printed_konf_sarrera,int* k_sarrera, volatile int* f_sarrera, volatile int* f_printed_sarrera,volatile float* t);

void TS_sarrera_FPS_Kontrola (Adafruit_ILI9341* tft,TouchScreen* ts,FPS_GT511C3* fps,int x, int y, int* f_printed_konf_sarrera);

void TS_sarrera_Atea_Kontrola (Adafruit_ILI9341* tft,TouchScreen* ts,int* pertsiana,int* f_printed_konf_sarrera,int x,int y);

void TS_Password_Kontrola (Adafruit_ILI9341* tft,TouchScreen* ts,Password *passwd,int* f_printed_konf_sarrera,int* k_passwd, int* k_sarrera,int* k_numero,int x,int y);

void TS_Egongela_Kontrola (Adafruit_ILI9341* tft,TouchScreen* ts,int* pertsiana,int x,int y);