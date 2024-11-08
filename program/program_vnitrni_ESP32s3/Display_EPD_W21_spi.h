
  // Gooddisplay example codes -  https://www.good-display.com/companyfile/24/

#ifndef _DISPLAY_EPD_W21_SPI_
#define _DISPLAY_EPD_W21_SPI_
#include "Arduino.h"

#define isEPD_W21_BUSY digitalRead(38)  //BUSY
#define EPD_W21_RST_0 digitalWrite(37,LOW)  //RES
#define EPD_W21_RST_1 digitalWrite(37,HIGH)
#define EPD_W21_DC_0  digitalWrite(16,LOW) //DC
#define EPD_W21_DC_1  digitalWrite(16,HIGH)
#define EPD_W21_CS_0 digitalWrite(15,LOW) //CS
#define EPD_W21_CS_1 digitalWrite(15,HIGH)


void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char datas);
void EPD_W21_WriteCMD(unsigned char command);


#endif 
