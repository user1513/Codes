#include "uart3.h"

#define FOSC            24000000UL
#define BRT3             (65536 - FOSC / 9600 / 4)

void Uart3Init(void)		//9600bps@24.000MHz
{
	S3CON = 0x50;
    T3L = BRT3;
    T3H = BRT3 >> 8;
    T4T3M |= 0x0a;
}

void Uart3Send(char dat)
{
	S3BUF = dat;
    while ((S3CON & 0x02)!=0x02);
    S3CON &= ~0x02; 
}

void Uart3SendStr(char *p)
{
    while (*p!='\0')
    {
        Uart3Send(*p++);
    }
}

void Uart3SendStrLen(u8 *p,u8 length)
{
    while (length--)
    {
        Uart3Send(*p++);
    }
}

