#ifndef _DS1302_H
#define _DS1302_H

#include "main.h"




#define SCLK PBout(13)
#define RST PBout(15)
#define IOIN PBin(14)
#define IOOUT PBout(14)

#define IO_IN()  {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=(u32)8<<24;}
#define IO_OUT() {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=(u32)3<<24;}

void DS1302_Initial(void);
void DS1302_SetTime(u8 *p);
void DS1302_GetTime(u8 *p);
void Ds1302_GpioInit(void);

#endif



