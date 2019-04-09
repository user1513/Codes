#ifndef _VS1838B_H
#define _VS1838B_H

#include <STC12C5A60S2.H>
#include "intrins.h"
#include "usart.h"

sbit IRIN = P3^2;         //红外接收器数据线

void vs1838b_Init(void);
void delay_014ms(unsigned char x);    //x*0.14MS,不同单片机，不同晶振频率，需要配置不同的delay




#endif


