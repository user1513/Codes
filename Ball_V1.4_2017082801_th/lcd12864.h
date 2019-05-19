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
//Ĭ���ǲ���

void Display_String(uchar line,uchar *string);//��ʾ�ַ��� 
void Wr_Data(uchar wrdata); //д���� 
void Wr_Command(uchar wrcommand,busyc); //дָ�� 
void RDbf(void); //��æ״̬ 
void Lcm_Init(void);//12864��ʼ��
void Delay5Ms(void); //��ʱ 5ms
void LCD12864_Clear(void);
#endif