
#include "uart1.h"

#define FOSC            24000000UL
#define BRT             (65536 - FOSC / 100000 / 4)




void UartIsr() interrupt 4 using 1
{
    if (RI)
    {
        RI = 0;
		DR16DataAnalysis(SBUF);
    }
}

void Uart1_Init()
{
    SCON |= 0x50;
    TMOD |= 0x00;
    TL1 = BRT;
    TH1 = BRT >> 8;
    TR1 = 1;
    AUXR |= 0x40;
	ES=1;
	EA=1;
}

void Uart1_Send(u8 dat)
{
    SBUF = dat;
	while (!TI);
    TI=0;
    
}

void Uart1_SendStr(u8 *p)
{
    while (*p)
    {
        Uart1_Send(*p++);
    }
}

void Uart1SendStrLen(u8 *p,u8 length)
{
    while (length--)
    {
        Uart1_Send(*p++);
    }
}







