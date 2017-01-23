#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <VarSpeedServo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_PN532.h>
#include <FPS_GT511C3.h>
#include <Password.h>
#include <LinkSpriteCamera.h>
#include <RTC_DS1307.h>
#include "TS_funtzioak.h"
#include "Etxea_funtzioak.h"
#include "TFT_funtzioak.h"
#include "RFID_funtzioak.h"
#include "FPS_funtzioak.h"
#include "LSC_funtzioak.h"

#define TFT_DC 6
#define TFT_CS 5
#define YP A2 
#define XM A1  
#define YM 54   
#define XP 57 
#define TS_MINX 116*2
#define TS_MAXX 890*2
#define TS_MINY 83*2
#define TS_MAXY 913*2
#define PN532_IRQ 2
#define PN532_RESET 3
#define PIR 26
#define SD_CS 4
#define SARRAILA 22
#define SERBO 24
#define MOTORRA_Enable 8
#define MOTORRA_A 40
#define MOTORRA_B 42

int k_sarrera = 0;
int k_passwd = 0;
int k_numero=0;
int LDR=14;//A14
int val=0;
int final =0;
int pertsiana = 1;
volatile int k_ordua = 1;
volatile int k_printed_sarrera = 0;
volatile int f_printed_sarrera = 0;
volatile int f_printed_konf =0;
volatile int f_printed_konf_sarrera = 0;
volatile int f_RFID_FPS=0;
volatile int f_printed_passwd = 0;
volatile int f_sarrera = 1;
volatile int f_egongela = 1;
volatile int f_argazkia = 0;
volatile float t;



Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
TouchScreen ts = TouchScreen(XP, YP, XM, YM);
Adafruit_PN532 nfc( PN532_IRQ, PN532_RESET);
FPS_GT511C3 fps(&Serial2);
Password passwd = Password("1234");
LinkSpriteCamera camera(&Serial3);
RTC_DS1307 rtc;

VarSpeedServo miServo;

void setup() {
  Serial.begin(115200);
  Serial.println(F("---> setup()"));

  Wire.begin();
   
  TFT_abiarazi(&tft);
  if (SD.begin(SD_CS)) {
    Serial.println("SD abiarazi da");
  } else {
    Serial.println("SD card initialization failed");
    while(1);
  }
  RFID_abiarazi(&nfc);
  FPS_abiarazi(&fps);
  LSC_abiarazi(&camera);
  /*
  Serial.println("Ordua eta Data jarri");
  while(Serial.available()==0){
  }
  char seconds = Serial.parseInt();
  char minutes = Serial.parseInt();
  char hours = Serial.parseInt();
  char day = Serial.parseInt();
  char date = Serial.parseInt();
  char month = Serial.parseInt();
  char year = Serial.parseInt();
  rtc.setTime(seconds,minutes,hours,day,date,month,year);
*/
  rtc.setTime(40,43,11,1,23,1,17);
  
  Serial.println("ORDUA");
  byte seconds, minutes, hours, day, date, month, year;
  rtc.getTime(&seconds, &minutes, &hours, &day, &date, &month, &year);
  Serial.print(day); Serial.print(", ");
  Serial.print(date); Serial.print("/"); Serial.print(month); Serial.print("/"); Serial.print(year); Serial.print(", ");
  Serial.print(hours); Serial.print(":"); Serial.print(minutes); Serial.print(":"); Serial.println(seconds);

  pinMode(PIR, INPUT); 
  //pinMode(PIR, INPUT_PULLUP); // Arduinoren PULL UP barne erresistentzia aktibatu
  attachInterrupt(5, argazkia_atera, FALLING); // int.5 --> pin 18

  miServo.attach(SERBO, 650, 2400);
  pinMode(SARRAILA, OUTPUT); 
  
  Serial.println(F("<--- setup()"));
}

void loop() {
  //Serial.println("ENTRA LOOP");
  if(f_sarrera && k_sarrera < 5) {  
    Serial.println("ENTRA RFID");  
    uint8_t data[16]; 
    RFID_irakurri(&nfc, data);
    Serial.println("PASARFID");
    if(*data != '&') {
      ETXEA_sarrera_egiaztapena(&tft, &rtc, data, 
                                &k_sarrera, &f_sarrera, &f_printed_sarrera, 
                                &t);
        if(*data != '%' || *data != '$' || *data != '#') {
            digitalWrite(SARRAILA, HIGH);
            miServo.slowmove(145,40);
            digitalWrite(SARRAILA, LOW);
            delay(5000);
            miServo.slowmove(74,82);
          }             
      Serial.print("k_sarrera = "); Serial.println(k_sarrera);
  
    } else {
      Serial.println("ENTRA fps");  
      FPS_irakurri(&fps, data);
      Serial.println("PASAR fps");
      if(*data != '&') {
        ETXEA_sarrera_egiaztapena(&tft, &rtc, data, 
                                  &k_sarrera, &f_sarrera, &f_printed_sarrera, 
                                  &t);
          if(*data != '%' || *data != '$' || *data != '#') {
            digitalWrite(SARRAILA, HIGH);
            miServo.slowmove(145,40);
            digitalWrite(SARRAILA, LOW);
            delay(5000);
            miServo.slowmove(74,82);
          }
        Serial.print("k_sarrera = "); Serial.println(k_sarrera);
      }
    }
  } else if(k_sarrera == 5) {
    if(!f_printed_passwd) {
      Serial.println("Set password screen");
      TFT_teklatua_marraztu(&tft);
      f_printed_passwd = 1;
    }
    Point p = ts.getPoint();
      if(p.z > __PRESURE) {
        Serial.println("Botoia sakatu da");
        p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
        p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);

        TS_teklatu_kontrola(&tft, &passwd, p.x, p.y, &k_passwd) ;

        if(k_passwd == 4) {
          if(passwd.evaluate()) {
            Serial.println("Password correct");
            passwd.reset();
            k_sarrera = 0;
            k_passwd = 0;
            f_printed_passwd = 0;
          } else {
            Serial.println("Password incorrect");
            TFT_alarma(&tft);
          }
        }
      } 
    }

  if(!f_printed_sarrera && k_sarrera < 5) {
    Serial.println("Set configuration screen");
    TFT_konf_etxea(&tft);
    f_printed_sarrera = 1;
  }
  
  if(f_argazkia){
    detachInterrupt(5); // int.5 --> pin 18
    TFT_mezua(&tft, "INTRUSION DETECTED!");
    LSC_argazkia_atera(&camera, &rtc);
    f_argazkia = 0;
    f_printed_sarrera = 0;
    attachInterrupt(5, argazkia_atera, FALLING); // int.5 --> pin 18
  }

  if(k_sarrera < 5) {
    uint8_t data[16];
    uint8_t id_aux;
    Point p = ts.getPoint();
    if(p.z > __PRESURE) {
      Serial.println("Botoia sakatu da");
      p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
      p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
      if(p.x>160 && p.x<240) {
        if(p.y>30 && p.y<140){
          ETXEA_sarrera(&tft,&ts,&fps,&nfc,&rtc,data,&passwd,&k_passwd,&k_sarrera,&f_sarrera,&f_printed_konf_sarrera,&f_printed_sarrera,&t,&k_numero,&pertsiana,p.x,p.y);
          Point p = ts.getPoint();
          p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
          p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
        } else if(p.y>180 && p.y<300){
          ETXEA_egongela(&tft,&ts,&pertsiana,p.x,p.y);  
        }
      } else if(p.x>20 && p.x<120 && p.y>100 && p.y<200){
        ETXEA_logela(&tft, &ts, &camera, &rtc);
      }
      f_printed_sarrera = 0;
    }   
  } 
 
  if(f_egongela){
    val=analogRead(LDR);
    final=(val/4);
    //analogWrite(LED,final);
    //Serial.println(final);
    Pertsianen_Kontrola();
    //Serial.println("SALE EGONGELA");
  }
}

ISR(TIMER2_COMPA_vect) {
  if(k_printed_sarrera == 61) {
    TIMSK2 &= ~(1<<OCIE2A);
    f_sarrera = 1;
    f_printed_sarrera = 0;
    k_printed_sarrera = 0;
    Serial.println(micros()-t);
  } else {
    k_printed_sarrera++;
  }
}

void argazkia_atera() {
  f_argazkia = 1;
}

void Pertsianen_Kontrola(){
  byte seconds, minutes, hours, day, date, month, year;
  Serial.println("PASO BYTE");
  rtc.getTime(&seconds, &minutes, &hours, &day, &date, &month, &year);
  Serial.println("ENTRO PERTSIANA");
  if(month>5 && month<10){
    if((final>120 || (hours==10 && minutes==0 && seconds==0))&& !pertsiana) {
      Serial.println("IGOTZEN...");
      //TFT_mezua(tft, "IGOTZEN...");   
      digitalWrite(MOTORRA_A, HIGH);
      digitalWrite(MOTORRA_B, LOW);
      analogWrite(MOTORRA_Enable, 130); //PWM
      delay(150);
      analogWrite(MOTORRA_Enable, 110); //PWM
      delay(250);
      analogWrite(MOTORRA_Enable, 90); //PWM
      delay(510);
      analogWrite(MOTORRA_Enable, 82); //PWM
      delay(410);
      analogWrite(MOTORRA_Enable, 0); //PWM
      pertsiana=1;
    }
    else if((final>100 && hours==12  && minutes==0 && seconds==0)&& pertsiana){ 
      Serial.println("JAISTEN...");
      //TFT_mezua(tft, "JAISTEN...");
      digitalWrite(MOTORRA_A, LOW);
      digitalWrite(MOTORRA_B, HIGH);     
      analogWrite(MOTORRA_Enable, 80); //PWM
      delay(100);
      analogWrite(MOTORRA_Enable, 75); //PWM
      delay(200);
      analogWrite(MOTORRA_Enable, 70); //PWM
      delay(330);
      analogWrite(MOTORRA_Enable, 0); //PWM
      pertsiana=0;  
    }
    else if((final<175 || (hours==18 && minutes==0 && seconds==0))&& !pertsiana){
      Serial.println("IGOTZEN...");
      //TFT_mezua(tft, "IGOTZEN...");   
      digitalWrite(MOTORRA_A, HIGH);
      digitalWrite(MOTORRA_B, LOW);
      analogWrite(MOTORRA_Enable, 130); //PWM
      delay(150);
      analogWrite(MOTORRA_Enable, 110); //PWM
      delay(250);
      analogWrite(MOTORRA_Enable, 90); //PWM
      delay(510);
      analogWrite(MOTORRA_Enable, 82); //PWM
      delay(410);
      analogWrite(MOTORRA_Enable, 0); //PWM
      pertsiana=0; 
    } 
    else if((final<180 || (hours==20 && minutes==0 && seconds==0))&& pertsiana){
      Serial.print("pertsiana jeitsi");
      //TFT_mezua(tft, "JAISTEN...");
      digitalWrite(MOTORRA_A, LOW);
      digitalWrite(MOTORRA_B, HIGH);     
      analogWrite(MOTORRA_Enable, 80); //PWM
      delay(100);
      analogWrite(MOTORRA_Enable, 75); //PWM
      delay(200);
      analogWrite(MOTORRA_Enable, 70); //PWM
      delay(330);
      analogWrite(MOTORRA_Enable, 0); //PWM
      pertsiana=1;  
    }   
  }
  else{
    if((final<120 || (hours==8 && minutes==0 && seconds==0))&& !pertsiana){
      Serial.println("IGOTZEN...");
      //TFT_mezua(&tft, "IGOTZEN...");   
      digitalWrite(MOTORRA_A, HIGH);
      digitalWrite(MOTORRA_B, LOW);
      analogWrite(MOTORRA_Enable, 130); //PWM
      delay(150);
      analogWrite(MOTORRA_Enable, 110); //PWM
      delay(250);
      analogWrite(MOTORRA_Enable, 90); //PWM
      delay(510);
      analogWrite(MOTORRA_Enable, 82); //PWM
      delay(410);
      analogWrite(MOTORRA_Enable, 0); //PWM
      Serial.print(day); Serial.print(", ");
      Serial.print(date); Serial.print("/"); Serial.print(month); Serial.print("/"); Serial.print(year); Serial.print(", ");
      Serial.print(hours); Serial.print(":"); Serial.print(minutes); Serial.print(":"); Serial.println(seconds);
      pertsiana=1;
      f_printed_sarrera = 0;
      Serial.print("pertsiana: ");      
    }
    else if((final>253  || (hours==20 && minutes==0 && seconds==0))&& pertsiana){
      Serial.print("pertsiana jeitsi");
      //TFT_mezua(&tft, "JAISTEN...");
      digitalWrite(MOTORRA_A, LOW);
      digitalWrite(MOTORRA_B, HIGH);     
      analogWrite(MOTORRA_Enable, 80); //PWM
      delay(100);
      analogWrite(MOTORRA_Enable, 75); //PWM
      delay(200);
      analogWrite(MOTORRA_Enable, 70); //PWM
      delay(330);
      analogWrite(MOTORRA_Enable, 0); //PWM
      Serial.print(day); Serial.print(", ");
      Serial.print(date); Serial.print("/"); Serial.print(month); Serial.print("/"); Serial.print(year); Serial.print(", ");
      Serial.print(hours); Serial.print(":"); Serial.print(minutes); Serial.print(":"); Serial.println(seconds);
      pertsiana=0;
      f_printed_sarrera = 0; 
    }
  }
}

