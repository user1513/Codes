#include "uart1.h"


void UartIsr() interrupt 4 using 1
{
    if (RI)
    {
        RI = 0;
		UltrasonicDataFunc(SBUF);
		//Uart1_Send(SBUF);
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
    while (*p!='\0')
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