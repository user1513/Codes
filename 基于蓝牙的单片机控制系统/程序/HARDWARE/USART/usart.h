#ifndef __USART_H_
#define __USART_H_


#include "app.h"

void Uart2Init(void);

void Uart1Init(void);

///////////////////////////////////////////////////////////////////
void SendByte(unsigned char byte);
void Send_String(unsigned char *str);
void Send_String_Len(unsigned char *str, unsigned char Len);

///////////////////////////////////////////////////////////////////
void UART_2SendOneByte(unsigned char _Data);
void UART_2SendString(char *s);

void GetUart2DataTimeout(uint8_t * men_able);

#endif
