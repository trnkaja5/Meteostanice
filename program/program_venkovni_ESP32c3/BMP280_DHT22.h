  // BMP280 -  https://navody.dratek.cz/navody-k-produktum/senzor-tlaku-a-teploty-bmp280.html
  // DHT22  -  https://navody.dratek.cz/navody-k-produktum/teplotni-senzor-dht11.html

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#include <DHT.h>

#define I2C_SDA 5
#define I2C_SCL 6

#define BMP280_ADRESA (0x76)

Adafruit_BMP280 bmp;

DHT dht(1, DHT22);

float korekce = 19.65;

int bmp_connect = 0;


void bmp_dht_SETUP() {
  Wire.begin(I2C_SDA, I2C_SCL);
  while ((!bmp.begin(BMP280_ADRESA)) && (bmp_connect < 3)) {
    //Serial.println("BMP280 senzor nenalezen, zkontrolujte zapojeni!");
    bmp_connect++;
    delay(2000);
  }
  dht.begin();
}

float bmp_TLAK() {
  return (bmp.readPressure()/100.00) + korekce;
}