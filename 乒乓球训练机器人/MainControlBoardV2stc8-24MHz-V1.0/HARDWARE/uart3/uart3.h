#ifndef _UART3_H
#define _UART3_H

#include <STC8.H>
#include "pbdata.h"

void Uart3Init(void);
void Uart3Send(char dat);
void Uart3SendStr(char *p);
void Uart3SendStrLen(u8 *p,u8 length);














#endif