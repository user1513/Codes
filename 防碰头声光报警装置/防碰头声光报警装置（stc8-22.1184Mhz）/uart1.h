#ifndef _UART1_H
#define _UART1_H


#include "pbdata.h"






void Uart1_Init();
void Uart1_Send(u8 dat);
void Uart1_SendStr(u8 *p);
void Uart1SendStrLen(u8 *p,u8 length);



extern xdata u8 UltrasonicArray[];
extern xdata u8 UartCmdArray[];









#endif