
  // NiceRF -  https://www.1cip.radekc.eu/clanek/arduino-a-bezdratovy-prenos-s-nicerf

#include <RH_ASK.h>

//(rychlost, rx_pin, tx_pin, 0);
RH_ASK NiceRF(2000, 9, 4, 0);     //esp32 nepouzivat pin 11 nebo 2


void NiceRF_SETUP() {
  if (!NiceRF.init()) {
    //Serial.println("NiceRF init failed");
  }
}

void radioSend(char typ, float hodnota) {
  
  char str[20];
  sprintf(str,"%c%.2f",typ, hodnota);
  NiceRF.send((uint8_t *)str, strlen(str));
  NiceRF.waitPacketSent();
}