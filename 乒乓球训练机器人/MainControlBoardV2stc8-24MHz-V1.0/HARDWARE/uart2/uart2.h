#ifndef __UART2_H
#define __UART2_H

#include <STC8.H>

#include "intrins.h"
#include "pbdata.h"






void Uart2Init(void);
void Uart2Send(u8 dat);
void Uart2SendStr(u8 *p);
void Uart2SendStrLen(u8 *p,u8 length);
#endif
