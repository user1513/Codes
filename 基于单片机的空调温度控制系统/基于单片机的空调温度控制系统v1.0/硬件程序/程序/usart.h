#ifndef __USART_H_
#define __USART_H_


#include <STC12C5A60S2.H>


void UartInit(void);
void SendByte(unsigned char byte);
void Send_String(unsigned char *str);
void Send_String_Len(unsigned char *str, unsigned char Len);
#endif
