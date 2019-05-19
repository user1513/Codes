#ifndef __UART1_H
#define __UART1_H

#include <STC8.H>
#include "pbdata.h"
#include "djiReceiver_dr16.h"






void Uart1_Init();
void Uart1_Send(u8 dat);
void Uart1_SendStr(u8 *p);
void Uart1SendStrLen(u8 *p,u8 length);

#endif
