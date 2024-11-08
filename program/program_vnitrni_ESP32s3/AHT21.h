
  // AHTxx -  https://github.com/enjoyneering/AHTxx/blob/main/examples/AHT20_Serial/AHT20_Serial.ino

#include <Wire.h>
#include <AHTxx.h>

#define I2C_SDA 8
#define I2C_SCL 9

float ahtValue;

AHTxx aht20(AHTXX_ADDRESS_X38, AHT2x_SENSOR);

void aht_SETUP() {
  int aht_connect = 0;
  while ((aht20.begin(I2C_SDA, I2C_SCL) != true) && (aht_connect < 3)) {
    //Serial.println(F("AHT2x not connected or fail to load calibration coefficient")); //(F()) save string to flash & keeps dynamic memory free
    aht_connect++;
    delay(3000);
  }
  //if(aht_connect < 3) Serial.println(F("AHT21 OK"));
}

void printStatus() {
  switch (aht20.getStatus())
  {
    case AHTXX_NO_ERROR:
      Serial.println(F("no error"));
      break;

    case AHTXX_BUSY_ERROR:
      Serial.println(F("sensor busy, increase polling time"));
      break;

    case AHTXX_ACK_ERROR:
      Serial.println(F("sensor didn't return ACK, not connected, broken, long wires (reduce speed), bus locked by slave (increase stretch limit)"));
      break;

    case AHTXX_DATA_ERROR:
      Serial.println(F("received data smaller than expected, not connected, broken, long wires (reduce speed), bus locked by slave (increase stretch limit)"));
      break;

    case AHTXX_CRC8_ERROR:
      Serial.println(F("computed CRC8 not match received CRC8, this feature supported only by AHT2x sensors"));
      break;

    default:
      Serial.println(F("unknown status"));    
      break;
  }
}

float aht_TEMP() {
  ahtValue = aht20.readTemperature(); //read 6-bytes via I2C, takes 80 milliseconds
  
  if (ahtValue != AHTXX_ERROR) //AHTXX_ERROR = 255, library returns 255 if error occurs
  {
    ahtValue -= 3.5;
    return ahtValue;
  }
  else
  {
    //printStatus(); //print temperature command status
    return 1000;
  }
}

float aht_HUM() {
  ahtValue = aht20.readHumidity(); //read 6-bytes via I2C, takes 80 milliseconds
  
  if (ahtValue != AHTXX_ERROR) //AHTXX_ERROR = 255, library returns 255 if error occurs
  {
    return ahtValue;
  }
  else
  {
    //printStatus(); //print temperature command status
    return 1000;
  }
}