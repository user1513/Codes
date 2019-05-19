#ifndef __USART_H_
#define __USART_H_


#include "app.h" 


void UartInit(void);
void SendByte(unsigned char byte);
void Send_String(unsigned char *str);
void Send_String_Length(unsigned char *str,u8 length);
#endif
