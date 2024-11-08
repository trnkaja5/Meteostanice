#include <SPI.h>
///////EPD///////
#include "Pictures.h"
#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"

#include "Radio.h"
#include "TimeSD.h"
#include "AHT21.h"
#include "MH-Z19B.h"
#include "Webserver.h"

int MaxMin = 0;      // zapis max a min hodnot

const char* ZAPIS;

unsigned long webserver_oldTime = 0;
int webserverDELAY = 10;

unsigned long oldTime = 0;
int DELAY = 180000;

void setup() 
{
  Serial.begin(115200);
  Serial.println("setup started");
  microSD_SETUP();
  time_SETUP();
  webserver_SETUP();
  aht_SETUP();
  mhz_SETUP();
  NiceRF_SETUP();

  read_Data();
  write_Data();
  casZapnuti();

    //////EPD//////
  pinMode(38, INPUT);  //BUSY
  pinMode(37, OUTPUT); //RES 
  pinMode(16, OUTPUT); //DC   
  pinMode(15, OUTPUT); //CS  

  EPD_Init();
  EPD_WhiteScreen_White();
  EPD_DeepSleep();
  delay(2000);

  EPD_Init();
  EPD_SetRAMValue_BaseMap(gImage_meteo_basemap);
  EPD_DeepSleep();
  display_Data(aht_temp, co2Z, dhtTeplota, bmpTlakZ);

  attachInterrupt(4, NiceRF_prijem, RISING);

  Alojz();

  Serial.println("setup ended");
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {

  checkWebserver();

  if((oldTime + DELAY) <= millis()) {
    read_Data();
    Alojz();
    checkWebserver();
    display_Data(aht_temp, co2Z, dhtTeplota, bmpTlakZ);
    oldTime = millis();
    checkWifi();
  }

  if (newData == true) {
    convertData();
    read_Data();
    write_Data();
    checkWebserver();
    display_Data(aht_temp, co2Z, dhtTeplota, bmpTlakZ);
    oldTime = millis();
    checkWifi();
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////

void read_Data() {
  aht_temp = aht_TEMP();
  aht_hum = aht_HUM();
  if(!mhz19b.isWarmingUp()) co2 = mhz_read_CO2();
  zaokrouhleni = round(co2);
  co2Z = zaokrouhleni + 0.0;
}

void write_Data() {
  getTime();
  char zapis_buffer[80];
  sprintf(zapis_buffer, "%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n", aht_temp, aht_hum, co2, bmpTeplota, bmpTlak, dhtTeplota, dhtVlhkost, rychlostVetru, narazVetru, akuVoltage);
  ZAPIS = zapis_buffer;
  appendFile(SD, NAZEV, ZAPIS);
  if (append_SD == 1) SD_ukazatel = "green";
  else SD_ukazatel = "red";

  if(MaxMin < 5) MaxMin++;
  if(MaxMin == 2 ) {
    maxTeplota = dhtTeplota;
    minTeplota = dhtTeplota;
    maxVlhkost = dhtVlhkost;
    minVlhkost = dhtVlhkost;
    maxTlak = bmpTlak;
    minTlak = bmpTlak;
  }
  if(dhtTeplota > maxTeplota) maxTeplota = dhtTeplota;
  if(dhtTeplota < minTeplota) minTeplota = dhtTeplota;
  if(dhtVlhkost > maxVlhkost) maxVlhkost = dhtVlhkost;
  if(dhtVlhkost < minVlhkost) minVlhkost = dhtVlhkost;
  if(bmpTlak > maxTlak) maxTlak = bmpTlak;
  if(bmpTlak < minTlak) minTlak = bmpTlak;
}

void checkWebserver() {
  if((webserver_oldTime + webserverDELAY) <= millis()) {
    server.handleClient();
    webserver_oldTime = millis();
  }
}

void display_Data(float teplotaIN, float co2, float teplotaOUT, float tlak) {
  
  int x = 0;    //sourX
  int y = 0;    //sourY
  int j = 0;    //pozice
  char data_buffer[10];

////////////////////////////////////////////////////////////////////////  teplotaIN:
  x=40; y=60; j=10;
  snprintf(data_buffer, sizeof(data_buffer), "%f", teplotaIN);

  EPD_Init();
  for(int i=0; i<10; i++) {

    switch(data_buffer[i]) {
      case '0': EPD_Dis_Part(x, y, Num[0], 32, 64, j); break;
      case '1': EPD_Dis_Part(x, y, Num[1], 32, 64, j); break;
      case '2': EPD_Dis_Part(x, y, Num[2], 32, 64, j); break;
      case '3': EPD_Dis_Part(x, y, Num[3], 32, 64, j); break;
      case '4': EPD_Dis_Part(x, y, Num[4], 32, 64, j); break;
      case '5': EPD_Dis_Part(x, y, Num[5], 32, 64, j); break;
      case '6': EPD_Dis_Part(x, y, Num[6], 32, 64, j); break;
      case '7': EPD_Dis_Part(x, y, Num[7], 32, 64, j); break;
      case '8': EPD_Dis_Part(x, y, Num[8], 32, 64, j); break;
      case '9': EPD_Dis_Part(x, y, Num[9], 32, 64, j); break;
      case '.': EPD_Dis_Part(x, y, gImage_des_carka, 32, 64, j); break;
      case '-': EPD_Dis_Part(x, y, gImage_minus, 32, 64, j); break;
    }
    x += 32;
    j++;
    //Serial.println(data_buffer[i]);

    if((data_buffer[i-1] == '.') && (j == 33)) EPD_Dis_Part(x, y, gImage_white, 32, 64, j);
    if(data_buffer[i-1] == '.') break;
  }

checkWebserver();
//////////////////////////////////////////////////////////////////////////  co2:
  x=40; y=150; j=20;
  snprintf(data_buffer, sizeof(data_buffer), "%f", co2);

  for(int i=0; i<10; i++) {

    switch(data_buffer[i]) {
      case '0': EPD_Dis_Part(x, y, Num[0], 32, 64, j); break;
      case '1': EPD_Dis_Part(x, y, Num[1], 32, 64, j); break;
      case '2': EPD_Dis_Part(x, y, Num[2], 32, 64, j); break;
      case '3': EPD_Dis_Part(x, y, Num[3], 32, 64, j); break;
      case '4': EPD_Dis_Part(x, y, Num[4], 32, 64, j); break;
      case '5': EPD_Dis_Part(x, y, Num[5], 32, 64, j); break;
      case '6': EPD_Dis_Part(x, y, Num[6], 32, 64, j); break;
      case '7': EPD_Dis_Part(x, y, Num[7], 32, 64, j); break;
      case '8': EPD_Dis_Part(x, y, Num[8], 32, 64, j); break;
      case '9': EPD_Dis_Part(x, y, Num[9], 32, 64, j); break;
      case '.': EPD_Dis_Part(x, y, gImage_des_carka, 32, 64, j); break;
      case '-': EPD_Dis_Part(x, y, gImage_minus, 32, 64, j); break;
    }
    x += 32;
    j++;
    //Serial.println(data_buffer[i]);

    if((data_buffer[i+1] == '.') && (j == 23)) EPD_Dis_Part(x, y, gImage_white, 32, 64, j);
    if(data_buffer[i+1] == '.') break;
  }

checkWebserver();
///////////////////////////////////////////////////////////////////////////  teplotaOUT:
  x=250; y=60; j=30;
  snprintf(data_buffer, sizeof(data_buffer), "%f", teplotaOUT);

  for(int i=0; i<10; i++) {

    if((data_buffer[0] == '-') && (j == 30)) x -= 32;

    switch(data_buffer[i]) {
      case '0': EPD_Dis_Part(x, y, Num[0], 32, 64, j); break;
      case '1': EPD_Dis_Part(x, y, Num[1], 32, 64, j); break;
      case '2': EPD_Dis_Part(x, y, Num[2], 32, 64, j); break;
      case '3': EPD_Dis_Part(x, y, Num[3], 32, 64, j); break;
      case '4': EPD_Dis_Part(x, y, Num[4], 32, 64, j); break;
      case '5': EPD_Dis_Part(x, y, Num[5], 32, 64, j); break;
      case '6': EPD_Dis_Part(x, y, Num[6], 32, 64, j); break;
      case '7': EPD_Dis_Part(x, y, Num[7], 32, 64, j); break;
      case '8': EPD_Dis_Part(x, y, Num[8], 32, 64, j); break;
      case '9': EPD_Dis_Part(x, y, Num[9], 32, 64, j); break;
      case '.': EPD_Dis_Part(x, y, gImage_des_carka, 32, 64, j); break;
      case '-': EPD_Dis_Part(x, y, gImage_minus, 32, 64, j); break;
    }
    x += 32;
    j++;
    //Serial.println(data_buffer[i]);

    if((data_buffer[i-1] == '.') && (j == 33)) EPD_Dis_Part(x, y, gImage_white, 32, 64, j);
    if((data_buffer[0] == '-') && (j == 34)) EPD_Dis_Part(x, y, gImage_white, 32, 64, j);
    if(data_buffer[i-1] == '.') break;
  }

checkWebserver();
/////////////////////////////////////////////////////////////////////////////  tlak:
  
 if(tlak < 10000) { 
  x=250; y=150; j=40;
  snprintf(data_buffer, sizeof(data_buffer), "%f", tlak);

  for(int i=0; i<10; i++) {

    switch(data_buffer[i]) {
      case '0': EPD_Dis_Part(x, y, Num[0], 32, 64, j); break;
      case '1': EPD_Dis_Part(x, y, Num[1], 32, 64, j); break;
      case '2': EPD_Dis_Part(x, y, Num[2], 32, 64, j); break;
      case '3': EPD_Dis_Part(x, y, Num[3], 32, 64, j); break;
      case '4': EPD_Dis_Part(x, y, Num[4], 32, 64, j); break;
      case '5': EPD_Dis_Part(x, y, Num[5], 32, 64, j); break;
      case '6': EPD_Dis_Part(x, y, Num[6], 32, 64, j); break;
      case '7': EPD_Dis_Part(x, y, Num[7], 32, 64, j); break;
      case '8': EPD_Dis_Part(x, y, Num[8], 32, 64, j); break;
      case '9': EPD_Dis_Part(x, y, Num[9], 32, 64, j); break;
      case '.': EPD_Dis_Part(x, y, gImage_des_carka, 32, 64, j); break;
      case '-': EPD_Dis_Part(x, y, gImage_minus, 32, 64, j); break;
    }
    x += 32;
    j++;
    //Serial.println(data_buffer[i]);

    if((data_buffer[i+1] == '.') && (j == 43)) EPD_Dis_Part(x, y, gImage_white, 32, 64, j);
    if(data_buffer[i+1] == '.') break;
  }
 }
  EPD_DeepSleep();
}

