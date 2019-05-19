#ifndef _USART2_H
#define _USART2_H

#include "main.h"
void Usart2_Init(u32 bound);
void Usart2_Send_Byte(u8 data);
void Usart2_String_Send(u8 *data);
void Usart2_String_Send1(u8 *data,u8 length);
extern u8 array[];
#endif



