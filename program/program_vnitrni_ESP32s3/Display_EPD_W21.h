#ifndef _DISPLAY_EPD_W21_H_
#define _DISPLAY_EPD_W21_H_

  // Gooddisplay example codes -  https://www.good-display.com/companyfile/24/


#define EPD_WIDTH   240 
#define EPD_HEIGHT  416
#define EPD_ARRAY  EPD_WIDTH*EPD_HEIGHT/8  


//Full screen refresh display
void EPD_Init(void); 
void EPD_Init_180(void);  
void EPD_WhiteScreen_ALL(const unsigned char *datas);
void EPD_WhiteScreen_White(void);
void EPD_WhiteScreen_Black(void);
void EPD_DeepSleep(void);
//Partial refresh display 
void EPD_Init_Part(void);
void EPD_SetRAMValue_BaseMap(const unsigned char * datas);
void EPD_Dis_PartAll(const unsigned char * datas);
void EPD_Dis_Part(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_COLUMN,unsigned int PART_LINE, int j);
void EPD_Dis_Part_Time(unsigned int x_start,unsigned int y_start,
                        const unsigned char * datas_A,const unsigned char * datas_B,
                        const unsigned char * datas_C,const unsigned char * datas_D,const unsigned char * datas_E,
                        unsigned char num,unsigned int PART_COLUMN,unsigned int PART_LINE);                    
//Fast refresh display
void EPD_Init_Fast(void);
void EPD_WhiteScreen_ALL_Fast(const unsigned char *datas);   

                         
#endif