#ifndef _UART_H_
#define _UART_H_

#ifndef uchar
#define uchar unsigned char 
#endif

#ifndef uint
#define uint unsigned int 
#endif

#include <REGX51.H>


#define FOSC 11059200L      //System frequency
#define BAUD 9600           //UART baudrate


void Uart_Init(void);
void SendData(uchar dat);

#endif