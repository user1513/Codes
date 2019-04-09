#ifndef _LCD12864_H
#define _LCD12864_H

#include <STC12C5A60S2.H>
#include <intrins.h>
#define LCMdata P0
#define Busy 0x80  
sbit RS=P2^5;
sbit RW=P2^6;
sbit E=P2^7;
void Lcd12864Clean(void);
void Wr_Data(unsigned char wrdata);
void Wr_Command(unsigned char wrcommand,busyc);
void RDbf(void);
void Wr_Data_String(unsigned char wrcommand,unsigned char *str);
void Lcm_Init(void);

#endif