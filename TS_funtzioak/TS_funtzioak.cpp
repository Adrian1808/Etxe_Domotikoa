#include "TS_funtzioak.h"
#include "TFT_funtzioak.h"
#include "FPS_funtzioak.h"
#include "RFID_funtzioak.h"
#include <Adafruit_ILI9341.h>
#include <Adafruit_PN532.h>
#include <Password.h>
#include <VarSpeedServo.h>
#define TS_MINX 116*2
#define TS_MAXX 890*2
#define TS_MINY 83*2
#define TS_MAXY 913*2
#define SARRAILA 22
#define SERBO 24

char TS_teklatu_kontrola(Adafruit_ILI9341* tft, Password *passwd, int x, int y, int* k_passwd) {
	Serial.print("-------->k_passwd: "); Serial.println(*k_passwd);
  	char zenbakia;
    Serial.println("Teklatu kontrola");
  	if(x>10 && x<78) {
    	if(y>215 && y<300) {
      		zenbakia = '9';
    	} else if(y>105 && y<210) {
      		zenbakia = '8';
    	} else if(y>10 && y<100) {
      		zenbakia = '7';
    	}
    	TS_tekla_gorde(tft, passwd, zenbakia, k_passwd);
  	} else if(x>83 && x<150) {
    	if(y>215 && y<300) {
      		zenbakia = '6';
    	} else if(y>105 && y<210) {
      		zenbakia = '5';
    	} else if(y>10 && y<100) {
      		zenbakia = '4';
    	}
    	TS_tekla_gorde(tft, passwd, zenbakia, k_passwd);
  	} else if(x>155 && x<230) {
    	if(y>215 && y<300) {
      		zenbakia = '3';
    	} else if(y>105 && y<210) {
      		zenbakia = '2';
    	} else if(y>10 && y<100) {
      		zenbakia = '1';
    	}
    	TS_tekla_gorde(tft, passwd, zenbakia, k_passwd);
  	}    
	
	return zenbakia;
}

void TS_tekla_gorde(Adafruit_ILI9341* tft, Password *passwd, char zenbakia, int* k_passwd) {
  Serial.println("Tekla gorde kontrola");
  passwd->append(zenbakia);
  Serial.print("%%%k_passwd: "); Serial.println(*k_passwd);
  (*k_passwd)++;
  TFT_zenbakia_berrestu(tft, zenbakia);
  if((*k_passwd) < 4) {
  	TFT_teklatua_marraztu(tft);
  }
}

void TS_Sarrera_Kontrola(Adafruit_ILI9341* tft,TouchScreen* ts,FPS_GT511C3* fps,Adafruit_PN532* nfc,RTC_DS1307* rtc, uint8_t* data,Password *passwd,int* k_passwd, int* k_sarrera,volatile int* f_sarrera, volatile int* f_printed_sarrera,volatile float* t,int* k_numero,int* pertsiana,int x,int y){
	int f_printed_konf_sarrera=0;
	while (!f_printed_konf_sarrera){
		//Serial.println("entro en el WHILE SARRERA");
		Point p = ts->getPoint();
		p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
		p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
		if(p.z > __PRESURE) {
			if(p.x>150 && p.x< 220){
			//RFID
				if(p.y>40 && p.y<120){
					TFT_sarrera_RFID(tft);
						while (!f_printed_konf_sarrera){
							//Serial.println("entro en el WHILE RFID");
							Point p = ts->getPoint();
							p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
							p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
							TS_sarrera_RFID_Kontrola(tft,ts,nfc,rtc,data,p.x, p.y, &f_printed_konf_sarrera,k_sarrera,f_sarrera,f_printed_sarrera,t);  
						}
					Serial.println("salgo del WHILE RFID");
					f_printed_konf_sarrera=0;
					TFT_sarrera(tft);
				}
			//HATZ
				else if(p.y>160 && p.y<240){
					TFT_sarrera_FPS(tft);
					while (!f_printed_konf_sarrera){
						//Serial.println("entro en el WHILE HATZ");
						Point p = ts->getPoint();
						p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
						p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
						TS_sarrera_FPS_Kontrola(tft,ts,fps,p.x, p.y, &f_printed_konf_sarrera);
					}
					Serial.println("salgo del WHILE HATZ");
					f_printed_konf_sarrera=0;
					TFT_sarrera(tft);   
				}
			}
			else if(p.x>30 && p.x<107)
			//ATEA
				if(p.y>40 && p.y<120){
					TFT_atea(tft);
					Serial.println("Atea kontrola");
					while (!f_printed_konf_sarrera){
						Point p = ts->getPoint();
						p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
						p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320); 
						TS_sarrera_Atea_Kontrola(tft,ts,pertsiana,&f_printed_konf_sarrera,p.x,p.y);
					}
					Serial.println("salgo del WHILE atea");
					f_printed_konf_sarrera=0;
					TFT_sarrera(tft);		
				}
			//PASAHITZA
				else if(p.y>160 && p.y<240){
					TFT_pasahitza(tft);
					Serial.println("Pasahitza kontrola");
					while (!f_printed_konf_sarrera){
						Point p = ts->getPoint();
						p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
						p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320); 
						TS_Password_Kontrola (tft,ts, passwd,&f_printed_konf_sarrera,k_passwd,k_sarrera,k_numero, p.x, p.y);  
					}
					Serial.println("salgo del WHILE password");
					f_printed_konf_sarrera=0;
					TFT_sarrera(tft);   
			}
 
			//ITZULI
			else if(p.x>0 && p.x<40 && p.y>240 && p.y<320){
				Serial.println("entro itzuli teklado");
				f_printed_konf_sarrera++;
			}
		}
	} 
}

void TS_sarrera_RFID_Kontrola (Adafruit_ILI9341* tft,TouchScreen* ts,Adafruit_PN532* nfc,RTC_DS1307* rtc, uint8_t* data,int x, int y, int* f_printed_konf_sarrera,int* k_sarrera, volatile int* f_sarrera, volatile int* f_printed_sarrera,volatile float* t){
	Point p = ts->getPoint();
	p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
	p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
	if(p.z > __PRESURE) {
		if(p.x>40 && p.x<160 && p.y>140 && p.y<240){
			TFT_mezua(tft, "TXARTELA JARRI");
			f_printed_sarrera=0;
			while (!f_printed_sarrera){
				uint8_t data[16];
				uint8_t success;
				RFID_irakurri(nfc, data);
					if(*data != '&') {
						ETXEA_sarrera_egiaztapena(tft, rtc, data,k_sarrera, f_sarrera, f_printed_sarrera,t);
						delay(500);
						f_printed_sarrera++;
					}
			}
			tft->fillScreen(ILI9341_BLACK);
			TFT_sarrera_RFID(tft);
			TFT_itzuli(tft);	
		} 
		else if(p.x>0 && p.x<40 && p.y>240 && p.y<320){
			Serial.println("entro itzuli ");
			*f_printed_konf_sarrera=1;
		}
	}
}

void TS_sarrera_FPS_Kontrola (Adafruit_ILI9341* tft,TouchScreen* ts,FPS_GT511C3* fps,int x, int y, int* f_printed_konf_sarrera){
	Point p = ts->getPoint();
	p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
	p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
	if(p.z > __PRESURE) {
		if(p.x>80 && p.x<160){
			if(p.y>5 && p.y<100){
				Serial.println("BERRIA");
				TFT_mezua(tft, "HATZ berria gorde");
				delay(1000);
				FPS_Berria(tft,fps);     
			}
			else if(p.y>110 && p.y<200){
				Serial.println("ID");
				TFT_mezua(tft, "HATZ-a identifikatu");
				delay(1000);
				FPS_ID(tft,fps,f_printed_konf_sarrera);         
			}
			else if(p.y>230 && p.y<310){
				Serial.println("EZABATU");
				TFT_mezua(tft, "HATZ-a ezabatu");
				delay(1000);
				FPS_Ezabatu(tft,fps,f_printed_konf_sarrera);
			}  
			tft->fillScreen(ILI9341_BLACK);
			TFT_sarrera_FPS(tft);
			TFT_itzuli(tft);  
		}
		else if(p.x>0 && p.x<40 && p.y>240 && p.y<320){
            Serial.println("entro itzuli");
            *f_printed_konf_sarrera=1;
		}
	}
}

void TS_sarrera_Atea_Kontrola (Adafruit_ILI9341* tft,TouchScreen* ts,int* pertsiana,int* f_printed_konf_sarrera,int x,int y){ 
	VarSpeedServo miServo;
	Point p = ts->getPoint(); 
	p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
	p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
	if(p.z > __PRESURE) {
		//if(Serial.available() > 0) {
		//char orden = Serial.read();
        if(p.x>60 && p.x< 160){
			if((p.y>20 && p.y<120)&& *pertsiana==0){
				Serial.println("IREKITZEN...");
				TFT_mezua(tft, "IREKITZEN...");		
				digitalWrite(SARRAILA, HIGH);
				miServo.slowmove(145,40);
				digitalWrite(SARRAILA, LOW); 
			}
			else if((p.y>145 && p.y<245)&& *pertsiana==1){
				Serial.println("IXTEN...");
				TFT_mezua(tft, "IXTEN...");
				miServo.slowmove(74,82);				
			}
			tft->fillScreen(ILI9341_BLACK);
			TFT_sarrera(tft);  
			}
		else if(p.x>0 && p.x<40 && p.y>240 && p.y<320){
			Serial.println("entro itzuli ATEA");
			*f_printed_konf_sarrera=1;
		}
	}   
}

void TS_Password_Kontrola (Adafruit_ILI9341* tft, TouchScreen* ts, Password *passwd, int* f_printed_konf_sarrera, int* k_passwd, int* k_sarrera, int* k_numero, int x, int y){ 
	Point p = ts->getPoint();
	p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
	p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
	int f_printed_passwd=0;
	if(x>60 && x< 160){
		if(y>20 && y<120){
			Serial.println("BERRIA");
			TFT_mezua(tft, "PASAHITZA ZAHARRA");
			delay(1000);
			if(!f_printed_passwd) {
				Serial.println("Set password screen");
				TFT_teklatua_marraztu(tft);
				while(!f_printed_passwd){
					Point p = ts->getPoint();
					if(p.z > __PRESURE) {
						Serial.println("Botoia sakatu da");
						p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
						p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
						TS_teklatu_kontrola(tft, passwd, p.x, p.y, k_passwd);
						if((*k_passwd) == 4) {
							if(passwd->evaluate()) {
								Serial.println("Password correct");
								TFT_mezua(tft, "PASAHITZA EGOKIA");
								delay(500);
								passwd->reset();
								*k_sarrera = 0;
								*k_passwd = 0;
								*k_numero = 0;
								TFT_mezua(tft, "PASAHITZA BERRIA");
								delay(1000);
								TFT_teklatua_marraztu(tft);
								char* new_pass = (char*) malloc(5 * sizeof(char));
								Serial.print("~$$->k_passwd: "); Serial.println(*k_passwd);
								while(!f_printed_passwd){
									Point p = ts->getPoint();
									if(p.z > __PRESURE) {
										Serial.println();
										p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
										p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
										Serial.print("/////->k_passwd: "); Serial.println(*k_passwd);
										char aux = TS_teklatu_kontrola(tft,passwd, p.x, p.y, k_passwd);
										Serial.print("aux: "); Serial.println(aux);
										Serial.print("^[][]k_passwd: "); Serial.println(*k_passwd);

										if((*k_passwd)-1 < 4) {
											*new_pass = aux;
											new_pass++;
											Serial.print("?¿?¿k_passwd: "); Serial.println(*k_passwd);
										} 
										if((*k_passwd) == 4){
											*new_pass = '\0';
											new_pass-=4;
											Serial.print("PASAHITZA BERRIA: ");
											for(int k=0; k<4; k++) {
												Serial.print(*new_pass);
												new_pass++;
											}
											Serial.println();
											
											new_pass-=4;
											*passwd = Password(new_pass);
											Serial.println(" PASS BERRIA");
											f_printed_passwd = 1;
										}
									}
						
								}
								
							} 
							else {
								Serial.println("Password incorrect");
								TFT_mezua(tft, "PASAHITZA OKERRA");
								delay(500); /*
								if(*k_numero == 2){
									Serial.println("Password alarma");
									TFT_alarma(tft);
									while(*k_numero ==2){}
								}
								if((*k_numero)<2){
									Serial.println("Password incorrect numero");
									(*k_numero) ++;
									passwd->reset();
									*k_sarrera = 0;
									*k_passwd = 0;
									f_printed_passwd = 1;
								}*/
							}
						}
					}	
				}
				Serial.println("Salgo del while");
				
			}
			Serial.println("Salgo del IF_PASSW");
			passwd->reset();
			*k_sarrera = 0;
			*k_passwd = 0;
			*k_numero = 0;
			f_printed_passwd = 0;
			TFT_pasahitza(tft);
		}
		else if(y>145 && y<245){
			TFT_mezua(tft, "PASAHITZA");
			delay(1000);
			if(!f_printed_passwd) {
				Serial.println("Set password screen");
				TFT_teklatua_marraztu(tft);
				while(!f_printed_passwd){
					Point p = ts->getPoint();
					if(p.z > __PRESURE) {
						Serial.println("Botoia sakatu da");
						p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
						p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
						TS_teklatu_kontrola(tft, passwd, p.x, p.y, k_passwd);
						if((*k_passwd) == 4) {
							if(passwd->evaluate()) {
								Serial.println("Password correct");
								TFT_mezua(tft, "PASAHITZA EGOKIA");
								delay(500);
								passwd->reset();
								*k_sarrera = 0;
								*k_passwd = 0;
								*k_numero = 0;
								f_printed_passwd=1;
							} else {
								while(!f_printed_passwd){
									Serial.println("Password incorrect");
									TFT_mezua(tft, "PASAHITZA OKERRA");
									delay(500);
									TFT_teklatua_marraztu(tft);
									passwd->reset();
									*k_sarrera = 0;
									*k_passwd = 0;
									*k_numero = 0;
									while(!f_printed_passwd){
										if((*k_numero)<2){
											Serial.println("Password incorrect numero");
											Point p = ts->getPoint();
											if(p.z > __PRESURE) {
												Serial.println("Botoia sakatu da");
												p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
												p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
												TS_teklatu_kontrola(tft, passwd, p.x, p.y, k_passwd);
												Serial.print("?¿?¿k_numero: "); Serial.println(*k_numero);
												if((*k_passwd) == 4) {
													if(passwd->evaluate()) {
														Serial.println("Password correct");
														TFT_mezua(tft, "PASAHITZA EGOKIA");
														delay(500);
														passwd->reset();
														*k_sarrera = 0;
														*k_passwd = 0;
														*k_numero = 0;
														f_printed_passwd=1;
													}else{
														(*k_numero)++;
														if((*k_numero) == 2){
															Serial.println("Password alarma");
															TFT_alarma(tft);
															while(*k_numero ==2){
															TFT_alarma;
															}
														}
														TFT_mezua(tft, "PASAHITZA OKERRA");
														delay(500);
														TFT_teklatua_marraztu(tft);
														passwd->reset();
														*k_sarrera = 0;
														*k_passwd = 0;
														
													}
												}
											}
										}
										if((*k_numero) == 2){
											Serial.println("Password alarma");
											TFT_alarma(tft);
											while(*k_numero ==2){
												TFT_alarma;
												
											}
										}
									}
								}
							}
						}
					}     
				}
			}
			tft->fillScreen(ILI9341_BLACK);
			TFT_pasahitza(tft);  
		}
	}
	else if(p.x>0 && p.x<40 && p.y>240 && p.y<320){
		Serial.println("entro itzuli");
		*f_printed_konf_sarrera=1;
	}
}
	

void TS_Egongela_Kontrola (Adafruit_ILI9341* tft,TouchScreen* ts,int* pertsiana,int x,int y){ 
  int f_printed_konf_sarrera=0;
  while(!f_printed_konf_sarrera){
		Point p = ts->getPoint(); 
		p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
		p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
		//if(Serial.available() > 0) {
		//char orden = Serial.read();
        if(p.x>60 && p.x< 160){
			if((p.y>20 && p.y<120)&& *pertsiana==0){
				Serial.println("IGOTZEN...");
				TFT_mezua(tft, "IGOTZEN...");		
				digitalWrite(40, HIGH);
				digitalWrite(42, LOW);
				analogWrite(8, 130); //PWM
				delay(150);
				analogWrite(8, 110); //PWM
				delay(250);
				analogWrite(8, 90); //PWM
				delay(510);
				analogWrite(3, 82); //PWM
				delay(410);
				analogWrite(8, 0); //PWM
				*pertsiana=1; 
			}
			else if((p.y>145 && p.y<245)&& *pertsiana==1){
				Serial.println("JAISTEN...");
				TFT_mezua(tft, "JAISTEN...");
				digitalWrite(40, LOW);
				digitalWrite(42, HIGH);     
				analogWrite(8, 80); //PWM
				delay(100);
				analogWrite(8, 75); //PWM
				delay(200);
				analogWrite(8, 70); //PWM
				delay(330);
				analogWrite(8, 0); //PWM
				*pertsiana=0;				
			}
			tft->fillScreen(ILI9341_BLACK);
			TFT_egongela(tft);  
			}
		else if(p.x>0 && p.x<40 && p.y>240 && p.y<320){
			Serial.println("entro itzuli");
			f_printed_konf_sarrera=1;
		}
	  //}
    }
}
