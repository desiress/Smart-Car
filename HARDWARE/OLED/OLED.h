#ifndef _OLED_H_
#define _OLED_H_

#include "common.h"
#include "gpio.h"

#define OLED_D0   PDout(3) 
#define OLED_D1   PDout(2) 
#define OLED_RST  PDout(1)
#define OLED_DS   PDout(0)
#define OLED_CS   PEout(4)


#define OLED_SCLK   OLED_D0
#define OLED_SDIN   OLED_D1
#define OLED_RES    OLED_RST
#define OLED_DC     OLED_DS


#define  CMD   0
#define  DATA1  1
#define uint8 uint8_t
#define int16 int16_t
#define int32 uint32_t
#define int8 int8_t
#define uint16 uint16_t

void OLED_init(void);
void OLED_fill_ram(uint8 data);
void OLED_display_string_6x8(uint8 row,uint8 col,char * str);
void OLED_display_number_6x8(uint8 row,uint8 col,int16 number);

void OLED_display_string_8x16(uint8 row,uint8 col,char * str);
void OLED_display_number_8x16(uint8 row,uint8 col,int16 number);
void OLED_display_chinese(uint8 row,uint8 col,uint8 start,uint8 len);
void OLED_display_128pixel(uint8 row,uint8 *pixel,uint8 threshold);

//void OLED_test();

#endif
