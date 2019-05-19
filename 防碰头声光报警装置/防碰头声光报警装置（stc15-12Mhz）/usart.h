#ifndef _USART_H
#define _USART_H


#include <STC15F2K60S2.H>

//-----------------------------------------
//define baudrate const
//BAUD = 65536 - FOSC/3/BAUDRATE/M (1T:M=1; 12T:M=12)
//NOTE: (FOSC/3/BAUDRATE) must be greater then 98, (RECOMMEND GREATER THEN 110)

//#define BAUD  0xF400                  // 1200bps @ 11.0592MHz
//#define BAUD  0xFA00                  // 2400bps @ 11.0592MHz
//#define BAUD  0xFD00                  // 4800bps @ 11.0592MHz
#define BAUD  0xFE80                  // 9600bps @ 11.0592MHz
//#define BAUD  0xFF40                  //19200bps @ 11.0592MHz
//#define BAUD  0xFFA0                    //38400bps @ 11.0592MHz

//#define BAUD  0xEC00                  // 1200bps @ 18.432MHz
//#define BAUD  0xF600                  // 2400bps @ 18.432MHz
//#define BAUD  0xFB00                  // 4800bps @ 18.432MHz
//#define BAUD  0xFD80                  // 9600bps @ 18.432MHz
//#define BAUD  0xFEC0                  //19200bps @ 18.432MHz
//#define BAUD    0xFF60                //38400bps @ 18.432MHz

//#define BAUD  0xE800                  // 1200bps @ 22.1184MHz
//#define BAUD  0xF400                  // 2400bps @ 22.1184MHz
//#define BAUD  0xFA00                  // 4800bps @ 22.1184MHz
//#define BAUD  0xFD00                  // 9600bps @ 22.1184MHz
//#define BAUD  0xFE80                  //19200bps @ 22.1184MHz
//#define BAUD  0xFF40                  //38400bps @ 22.1184MHz
//#define BAUD  0xFF80                  //57600bps @ 22.1184MHz

//sfr AUXR = 0x8E;
sbit RXB = P3^0;                        //define UART TX/RX port
sbit TXB = P3^1;

typedef bit BOOL;
typedef unsigned char BYTE;
typedef unsigned int WORD;

void Usart_Init(void);
void UserSendString(BYTE buf[],BYTE Length);
void UserSendChar(char Data);
void UART_INIT(void);
void UserSendString1(char *str );








#endif


