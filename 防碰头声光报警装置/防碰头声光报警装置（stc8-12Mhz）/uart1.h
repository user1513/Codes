#ifndef _UART1_H
#define _UART1_H


#include "pbdata.h"



#define FOSC            22118400UL
#define BRT             (65536 - FOSC / 9600 / 4)


void Uart1_Init();
void Uart1_Send(u8 dat);
void Uart1_SendStr(u8 *p);
void Uart1SendStrLen(u8 *p,u8 length);

























#endif