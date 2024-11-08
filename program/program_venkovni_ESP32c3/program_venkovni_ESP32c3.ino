#include "BMP280_DHT22.h"
#include "NiceRF.h"
#include "Anemo.h"

#define TIME_TO_SLEEP 60 // v sekundach    //60
#define AKU_PIN 2

float bmpTeplota;
float bmpTlak;
float dhtTeplota;
float dhtVlhkost;
float akuVoltage;

void setup() {
  //Serial.begin(115200);
  pinMode(AKU_PIN, INPUT);
  bmp_dht_SETUP();
  NiceRF_SETUP();
  pinMode(ANEMO_PIN, INPUT);

  ReadWindSpeed();

  ReadSendData();

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * 1000000);
  esp_deep_sleep_start();
}

void loop() {

}

void ReadSendData() {
  bmpTeplota = bmp.readTemperature();
  //Serial.print("teplota: ");
  //Serial.println(bmpTeplota);

  bmpTlak = bmp_TLAK();
  //Serial.print("tlak: ");
  //Serial.println(bmpTlak);

  dhtTeplota = dht.readTemperature();
  //Serial.print("DHTteplota: ");
  //Serial.println(dhtTeplota);

  dhtVlhkost = dht.readHumidity();
  //Serial.print("DHTvlhkost: ");
  //Serial.println(dhtVlhkost);

  akuVoltage = read_akuVoltage();


  radioSend('a', bmpTeplota);
  radioSend('b', bmpTlak);
  radioSend('c', dhtTeplota);
  radioSend('d', dhtVlhkost);
  radioSend('e', rychlostVetru);
  radioSend('f', narazVetru);
  radioSend('g', akuVoltage);
}

float read_akuVoltage() {
  int hodnota = 0;
  float napeti = 0;

  for(int i = 0; i < 5; i++) {
    hodnota += analogRead(AKU_PIN);
  }
  hodnota = hodnota / 5;

  napeti = hodnota / 40.95;
  napeti = napeti * 0.03296; //0.03296
  napeti = napeti * 2 - 0.5;

  //Serial.print("hodnota: ");
  //Serial.println(hodnota);
  //Serial.print("napeti: ");
  //Serial.println(napeti);

  return napeti;
}






