#ifndef _VS1838B_H
#define _VS1838B_H

#include <STC12C5A60S2.H>
#include "intrins.h"
#include "usart.h"

sbit IRIN = P3^2;         //���������������

void vs1838b_Init(void);
void delay_014ms(unsigned char x);    //x*0.14MS,��ͬ��Ƭ������ͬ����Ƶ�ʣ���Ҫ���ò�ͬ��delay




#endif


