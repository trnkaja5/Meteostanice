
  // Získávání času       -  https://randomnerdtutorials.com/esp32-ntp-client-date-time-arduino-ide/
  // práce se soubory(SD) -  https://randomnerdtutorials.com/esp32-microsd-card-arduino/#webserversdcard


#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <time.h>

const char* ssid     = "Wifi_405";       //realme GT Master Edition
const char* password = "2005*2007";      //p6smi4ia

long timezone = 1; 
byte daysavetime = 1;

const char* TIME;

struct tm tmstruct;

int denZ;               ///ZAPNUTI
int mesicZ;
int rokZ;                 
int hodinaZ;
int minutaZ;

int denA = 100;              ///AKTUALNI
int mesicA;
int rokA;
int hodinaA;

bool append_SD = 0;         //Zapis na SD
char cesta[40];
char nazev[40];
const char* CESTA;
const char* NAZEV;

float maxTeplota;           //MaxMin hodnoty
float maxVlhkost;
float maxTlak;
float minTeplota;
float minVlhkost;
float minTlak;


void writeFile(fs::FS &fs, const char * path, const char * message) {
    //Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        //Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        //Serial.println("File written");
    } else {
        //Serial.println("Write failed");
    }
    file.close();
}
void createDir(fs::FS &fs, const char * path){
    //Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        //Serial.println("Dir created");
    } else {
        //Serial.println("mkdir failed");
    }
}
void appendFile(fs::FS &fs, const char * path, const char * message) {

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        //Serial.println("FAIL path");
        append_SD = 0;
        return;
    }
    if(file.print(message)){
        append_SD = 1;
    } else {
        append_SD = 0;
    }
    file.close();
}

void microSD_SETUP() {
  if(!SD.begin(7)){
    return;
  }
  writeFile(SD, "/info.txt", " dd.mm.rrrr hh:mm:ss aht_temp, aht_hum, co2, bmpTeplota, bmpTlak, dhtTeplota, dhtVlhkost, rychlostVetru, narazVetru, akuVoltage");
}

void time_SETUP() {
  int wifi_connect = 0;

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED) && (wifi_connect < 20)) {
    wifi_connect++;
    if(wifi_connect >= 20) {
      Serial.println("wifi not connected!");
      return;
    }
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Contacting Time Server");
  configTime(3600*timezone, daysavetime*3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
  struct tm tmstruct;
  delay(2000);
  tmstruct.tm_year = 0;
}

void checkWifi() {
  int Wifi_connect = 0;

  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi not connected!!");

    WiFi.begin(ssid, password);

    while ((WiFi.status() != WL_CONNECTED) && (Wifi_connect < 20)) {
      Wifi_connect++;
      if(Wifi_connect >= 20) {
        Serial.println("wifi not connected!");
        return;
      }
      delay(300);
      Serial.print(".");
    }
  }
}

void getTime() {

  if(WiFi.status() == WL_CONNECTED) getLocalTime(&tmstruct, 5000);

  char buffer[50];
  sprintf(buffer, "%02d.%02d.%d %02d:%02d:%02d ",tmstruct.tm_mday,( tmstruct.tm_mon)+1,(tmstruct.tm_year)+1900,tmstruct.tm_hour , tmstruct.tm_min, tmstruct.tm_sec);
  TIME = buffer;

  if((denA) < tmstruct.tm_mday) {      // Nový den (přepsání MaxMin hodnot)
    maxTeplota = dhtTeplota;
    minTeplota = dhtTeplota;
    maxVlhkost = dhtVlhkost;
    minVlhkost = dhtVlhkost;
    maxTlak = bmpTlak;
    minTlak = bmpTlak;

    EPD_Init();
    EPD_SetRAMValue_BaseMap(gImage_meteo_basemap);
    EPD_DeepSleep();
  }
  denA = tmstruct.tm_mday;              // Přepsání aktuálního času
  mesicA = tmstruct.tm_mon + 1;
  rokA = tmstruct.tm_year + 1900;
  hodinaA = tmstruct.tm_hour;
  if(hodinaA == 0) {
    maxTeplota = dhtTeplota;
    minTeplota = dhtTeplota;
    maxVlhkost = dhtVlhkost;
    minVlhkost = dhtVlhkost;
    maxTlak = bmpTlak;
    minTlak = bmpTlak;
  }

  if(WiFi.status() == WL_CONNECTED) {
    sprintf(cesta, "/%02d_%d", mesicA, rokA);                                         //  Cesta souboru
    CESTA = cesta;
    sprintf(nazev, "/%02d_%d/%02d_%02d_%d.txt", mesicA, rokA, denA, mesicA, rokA);        //  Název soubou
    NAZEV = nazev;
  }

  createDir(SD, CESTA);

  appendFile(SD, NAZEV, TIME);                                                      // Zápis času na SD
}

void casZapnuti() {                                                                 // Čas zapnutí
  struct tm tmstruct;
  getLocalTime(&tmstruct, 5000);

  denZ = tmstruct.tm_mday;
  mesicZ = tmstruct.tm_mon + 1;
  rokZ = tmstruct.tm_year + 1900;
  hodinaZ = tmstruct.tm_hour;
  minutaZ = tmstruct.tm_min;
}

