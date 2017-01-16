#include <Adafruit_ILI9341.h>
#include <RTC_DS1307.h>
#include <LinkSpriteCamera.h>
#include <SeeedTouchScreen.h>
#include <Password.h>
#include <FPS_GT511C3.h>
#include <Adafruit_PN532.h>

void ETXEA_sarrera_egiaztapena(Adafruit_ILI9341* tft, RTC_DS1307* rtc, uint8_t* data, 
							   int* k_sarrera, volatile int* f_sarrera, volatile int* f_printed_sarrera,
							   volatile float* t);
							   
void ETXEA_set_timer3_interrupt();

void ETXEA_sarrera(Adafruit_ILI9341* tft,TouchScreen* ts,FPS_GT511C3* fps,Adafruit_PN532* nfc,RTC_DS1307* rtc, uint8_t* data,Password *passwd,int* k_passwd, int* k_sarrera,volatile int* f_sarrera,volatile int* f_printed_konf_sarrera,volatile int* f_printed_sarrera,volatile float* t,int* k_numero,int x,int y);

void ETXEA_egongela(Adafruit_ILI9341* tft,TouchScreen* ts,int x,int y);

void ETXEA_logela(Adafruit_ILI9341* tft, TouchScreen* ts, LinkSpriteCamera* camera, RTC_DS1307* rtc);
	
