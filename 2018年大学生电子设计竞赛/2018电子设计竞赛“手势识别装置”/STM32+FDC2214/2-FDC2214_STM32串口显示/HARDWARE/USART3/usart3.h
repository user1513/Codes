#ifndef _USART3_H
#define _USART3_H

#include "sys.h"

void Usart3_Init(u32 bound);
void Usart3_String_Send(u8 *data);
void Usart3_Send_Byte(u8 data);
void Usart3_String_Send1(u8 *data,u8 length);

#endif
