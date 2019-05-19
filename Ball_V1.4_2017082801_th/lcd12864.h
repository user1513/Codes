#ifndef __LCD12864_H__
#define __LCD12864_H__

#include <STC12C5A60S2.H>

#define uchar unsigned char 
#define uint unsigned int 


#define Busy 0x80  


#define LCMdata P0 
sbit RS=P2^7;  
sbit RW=P2^6;  
sbit E=P2^5;  
sbit REST=P4^6;  
//默认是并行

void Display_String(uchar line,uchar *string);//显示字符串 
void Wr_Data(uchar wrdata); //写数据 
void Wr_Command(uchar wrcommand,busyc); //写指令 
void RDbf(void); //读忙状态 
void Lcm_Init(void);//12864初始化
void Delay5Ms(void); //延时 5ms
void LCD12864_Clear(void);
#endif