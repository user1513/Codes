#ifndef _UART2_H
#define _UART2_H



#include "pbdata.h"




#define FOSC            22118400UL
#define BRT             (65536 - FOSC / 9600 / 4)

void Uart2Init(void);
void Uart2Send(u8 dat);
void Uart2SendStr(u8 *p);
void Uart2SendStrLen(u8 *p,u8 length);


















#endif