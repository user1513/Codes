#ifndef _LCD12864_H
#define _LCD12864_H

#include "app.h"
#include <intrins.h>
#define LCMdata P0 
#define Busy 0x80  
sbit RS=P2^4;
sbit RW=P2^5;
sbit E=P2^6;
sbit RES=P2^3;

void   initinal(void);           //LCD字库初始化程序
void lcd_mesg(unsigned char code *adder1);
void   TransferData(char data1,bit DI);  //传送数据或者命令,当DI=0是,传送命令,当DI=1,传送数据.
void lcd_write(uint8_t adder, uint8_t Data);


#endif