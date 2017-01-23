#include "Etxea_funtzioak.h"
#include "TFT_funtzioak.h"
#include "LSC_funtzioak.h"
#include "TS_funtzioak.h"
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <Adafruit_ILI9341.h>
#include <RTC_DS1307.h>
#include <LinkSpriteCamera.h>
#include <Adafruit_PN532.h>
#include <VarSpeedServo.h>

#define TS_MINX 116*2
#define TS_MAXX 890*2
#define TS_MINY 83*2
#define TS_MAXY 913*2
#define SARRAILA 22
#define SERBO 24

void ETXEA_sarrera_egiaztapena(Adafruit_ILI9341* tft, RTC_DS1307* rtc, uint8_t* data, 
							   int* k_sarrera, volatile int* f_sarrera, volatile int* f_printed_sarrera,
							   volatile float* t) {
    if(*data == '%' || *data == '$' || *data == '#') {
		TFT_sarrera_baliogabea(tft);
		(*k_sarrera)++;
    } else {
		TFT_sarrera_baliozkoa(tft, data); 
		byte seconds, minutes, hours, day, date, month, year;
		rtc->getTime(&seconds, &minutes, &hours, &day, &date, &month, &year);
		String file_name = String(year) + String("_") 
						 + String(month) + String("_")
						 + String(date) + ".log";
		File logFile = SD.open(file_name, FILE_WRITE);
		String timestamp = String(year) + String("-") 
						 + String(month) + String("-")
						 + String(date) + String("_")
						 + String(hours) + String(":") 
						 + String(minutes) + String(":")
						 + String(seconds) + String("_");
		for(int i=0; i<16; i++) { 
			timestamp += String(char(*data));
			data++;
		}
		Serial.println(timestamp);
		logFile.println(timestamp);
		logFile.close();
		*k_sarrera = 0;
		
		/*digitalWrite(SARRAILA, HIGH);
		miServo.slowmove(145,40);
		digitalWrite(SARRAILA, LOW);
		//INTERRUPCION DE 5 SEGUNDOS
		miServo.slowmove(74,82);*/
		
    }
    *f_sarrera = 0;
	*f_printed_sarrera = 1;
    ETXEA_set_timer3_interrupt();
    *t = micros();
}

void ETXEA_set_timer3_interrupt() {
  cli();
  
  // Timer2 clk/1024 --> t_BIT_TCNT2: 1 / (16*10^6 / 1024) = 64us
  // OCR2A? --> OCR2A * 64us = 1s --> OCR2A = 15625
  // OCR2A 8bit-eko erregistroa da... --> 15625/255=61 --> 1s
  //										       183 --> 3s
  
  // Table 20-8. Waveform Generation Mode Bit Description
  // Mode WGM22 WGM21 WGM20 Timer/Counter_Mode_of_Operation  TOP
  // 0    0     0     0     CTC                              0xFF
  
  // 20.10.1 TCCR2A – Timer/Counter2 Control Register A
  // COM2A1 COM2A0 COM2B1 COM2B0 X X WGM21 WGM20
  // Table 20-2. Compare Output Mode, non-PWM
  // COM2A1 COM2A0 Description
  // 0      0      Normal port operation, OC2A disconnected
  TCCR2A &= (~(_BV(COM2A1)) & ~(_BV(COM2A0)));
  TCCR2A &= (~(_BV(WGM21)) & ~(_BV(WGM20)));
  
  // 20.10.2 TCCR2B – Timer/Counter1 Control Register B
  // X X - - WGM22 CS22 CS21 CS20
  TCCR2B &= ~(_BV(WGM22));
  
  // Table 20-9. Clock Select Bit Description
  // CS22 CS21 CS20 Description
  // 1    1    1    clk/1024
  TCCR2B |= (_BV(CS22) | _BV(CS21) | _BV(CS20));

  TCNT2 = 0;
  TIMSK2 |= 1<<OCIE2A;
  
  sei();
}

void ETXEA_sarrera(Adafruit_ILI9341* tft,TouchScreen* ts,FPS_GT511C3* fps,Adafruit_PN532* nfc,RTC_DS1307* rtc, uint8_t* data,Password *passwd,int* k_passwd, int* k_sarrera,volatile int* f_sarrera,volatile int* f_printed_konf_sarrera,volatile int* f_printed_sarrera,volatile float* t,int* k_numero,int* pertsiana,int x,int y) {
	Serial.println("SARRERA KONTROLA");
	TFT_sarrera(tft);
	Point p = ts->getPoint(); 
	p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
	TS_Sarrera_Kontrola(tft,ts,fps,nfc,rtc,data,passwd,k_passwd, k_sarrera,f_sarrera,f_printed_sarrera,t,k_numero,pertsiana,p.x,p.y);
	
}

void ETXEA_egongela(Adafruit_ILI9341* tft,TouchScreen* ts,int* pertsiana,int x,int y) {
	int f_printed_konf_sarrera=0;
	Serial.println("EGONGELA KONTROLA");
	TFT_egongela(tft);
	Point p = ts->getPoint(); 
	p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
	TS_Egongela_Kontrola (tft,ts,pertsiana,p.x,p.y);
}

void ETXEA_logela(Adafruit_ILI9341* tft, TouchScreen* ts, LinkSpriteCamera* camera, RTC_DS1307* rtc) {
	Serial.println("LOGELA KONTROLA");  
	TFT_logela(tft);
	int f_argazkia = 1;
	while(f_argazkia) {
		Point p = ts->getPoint();
		p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
		p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
		if(p.z > __PRESURE) {
			if(p.x>40 && p.x<160 && p.y>140 && p.y<240){
			f_argazkia = 0;
			Serial.println("Botoia sakatu da");
			TFT_mezua(tft, "Argazkia ateratzen...");
			LSC_argazkia_atera(camera, rtc);
			}
			
		    else if(p.x>0 && p.x<40 && p.y>240 && p.y<320){
              Serial.println("entro itzuli");
              f_argazkia = 0;
			  
            }
		}	
	}
}
