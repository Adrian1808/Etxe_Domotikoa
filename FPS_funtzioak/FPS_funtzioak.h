#include <FPS_GT511C3.h>

void FPS_abiarazi(FPS_GT511C3* fps);

void FPS_irakurri(FPS_GT511C3* fps,uint8_t* data_aux);

void FPS_Berria(Adafruit_ILI9341* tft,FPS_GT511C3* fps);

void FPS_ID(Adafruit_ILI9341* tft,FPS_GT511C3* fps,int* f_printed_konf_sarrera);

void FPS_Ezabatu(Adafruit_ILI9341* tft,FPS_GT511C3* fps,int* f_printed_konf_sarrera);