
  // MHZ19B -  https://github.com/Erriez/ErriezMHZ19B/tree/master/examples

#include <ErriezMHZ19B.h>

#define mhzSerial           Serial1

#define RX1 41
#define TX1 42

ErriezMHZ19B mhz19b(&mhzSerial);


void mhz_SETUP() {
  char firmwareVersion[5];
  int mhz_connect = 0;

  //Serial.println("mh-z19b setup...");

  mhzSerial.begin(9600, SERIAL_8N1, RX1, TX1);

  while ((!mhz19b.detect()) && (mhz_connect < 3)) {
    //Serial.println(F("Detecting MH-Z19B sensor..."));
    mhz_connect++;
    delay(2000);
  }



  //Serial.print(F("  Firmware: "));
  mhz19b.getVersion(firmwareVersion, sizeof(firmwareVersion));
  //Serial.println(firmwareVersion);

  //Serial.print(F("  Range: "));
  //Serial.print(mhz19b.getRange());
  //Serial.println(F("ppm"));

  //Serial.print(F("  Auto calibrate: "));
  //Serial.println(mhz19b.getAutoCalibration() ? F("On") : F("Off"));

  if (mhz19b.isWarmingUp()) {
    //Serial.println(F("Warming up..."));
  };
}

void printErrorCode(int16_t result) {
  // Print error code
  switch (result) {
    case MHZ19B_RESULT_ERR_CRC:
      //Serial.println(F("CRC error"));
      break;
    case MHZ19B_RESULT_ERR_TIMEOUT:
      //Serial.println(F("RX timeout"));
      break;
    default:
      //Serial.print(F("Error: "));
      //Serial.println(result);
      break;
    }
}

float mhz_read_CO2() {

  int16_t result;
  float sum;
  float Result;

  for(int i = 0; i < 5; i++) {
    if(mhz19b.isReady()) {

      result = mhz19b.readCO2();

      if(result < 0) {
        printErrorCode(result);
      } else {
        sum += result;
      }
    }
    else i--;
  }
  Result = sum / 5.0;
  sum = 0;

  return Result;
}

