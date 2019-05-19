#ifndef _USART2_H
#define _USART2_H

#include "sys.h"
#include "Data_Check.h"


void Usart3_Init(u32 bound);
void Usart3_String_Send(u8 *data);
void Usart3_Send_Byte(u8 data);
void Usart3_String_Send1(u8 *data,u8 length);

#endif



