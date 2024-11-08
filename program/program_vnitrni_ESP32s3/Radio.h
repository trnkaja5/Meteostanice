
  // NiceRF -  https://www.1cip.radekc.eu/clanek/arduino-a-bezdratovy-prenos-s-nicerf

#include <RH_ASK.h>

#define RF_CS 5

//(rychlost, rx_pin, tx_pin, 0);
RH_ASK NiceRF(2000, 4, 15, 0);

float bmpTeplota;
float bmpTlak;
float dhtTeplota;
float dhtVlhkost;
float rychlostVetru;
float narazVetru;
float akuVoltage;

int akuProcenta;

float bmpTlakZ;
int zaokrouhleni;

uint8_t BMPTeplota[RH_ASK_MAX_MESSAGE_LEN];
uint8_t BMPTlak[RH_ASK_MAX_MESSAGE_LEN];
uint8_t DHTTeplota[RH_ASK_MAX_MESSAGE_LEN];
uint8_t DHTVlhkost[RH_ASK_MAX_MESSAGE_LEN];
uint8_t RYCHLOSTVetru[RH_ASK_MAX_MESSAGE_LEN];
uint8_t NARAZVetru[RH_ASK_MAX_MESSAGE_LEN];
uint8_t AKUVoltage[RH_ASK_MAX_MESSAGE_LEN];

bool newData = false;

unsigned long casPrijmu = 0;


void NiceRF_SETUP() {
  pinMode(RF_CS, OUTPUT);
  digitalWrite(RF_CS, HIGH);

  if (!NiceRF.init()) {
      //Serial.println("NiceRF init failed");
  }
}

void NiceRF_prijem() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  while (NiceRF.recv(buf, &buflen)) {    // Non-blocking
    buf[buflen]=0;

    switch (buf[0]) {
      case 'a':
        for (uint8_t i = 0; i < buflen; i++) {
          BMPTeplota[i] = buf[i];
        }
        break;
      case 'b':
        for (uint8_t i = 0; i < buflen; i++) {
          BMPTlak[i] = buf[i];
        }
        break;
      case 'c':
        for (uint8_t i = 0; i < buflen; i++) {
          DHTTeplota[i] = buf[i];
        }
        break;
      case 'd':
        for (uint8_t i = 0; i < buflen; i++) {
          DHTVlhkost[i] = buf[i];
        }
        break;
      case 'e':
        for (uint8_t i = 0; i < buflen; i++) {
          RYCHLOSTVetru[i] = buf[i];
        }
        break;
      case 'f':
        for (uint8_t i = 0; i < buflen; i++) {
          NARAZVetru[i] = buf[i];
        }
        break;
      case 'g':
        for (uint8_t i = 0; i < buflen; i++) {
          AKUVoltage[i] = buf[i];
        }
        newData = true;
        break;
    }
  }
}

void convertData() {
  bmpTeplota = atof(reinterpret_cast<const char*>(&BMPTeplota[1]));
  bmpTlak = atof(reinterpret_cast<const char*>(&BMPTlak[1]));
  dhtTeplota = atof(reinterpret_cast<const char*>(&DHTTeplota[1]));
  dhtVlhkost = atof(reinterpret_cast<const char*>(&DHTVlhkost[1]));
  rychlostVetru = atof(reinterpret_cast<const char*>(&RYCHLOSTVetru[1]));
  narazVetru = atof(reinterpret_cast<const char*>(&NARAZVetru[1]));
  akuVoltage = atof(reinterpret_cast<const char*>(&AKUVoltage[1]));

  if(akuVoltage < 3.7) akuProcenta = 0;
  else akuProcenta = (akuVoltage - 3.7) / 0.005;
  if(akuProcenta > 100) akuProcenta = 100;

  zaokrouhleni = round(bmpTlak);
  bmpTlakZ = zaokrouhleni + 0.0;
  
  newData = false;
  casPrijmu = millis();
}