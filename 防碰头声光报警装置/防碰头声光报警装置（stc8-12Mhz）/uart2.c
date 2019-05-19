#include "uart2.h"


void Uart2Isr() interrupt 8 using 1
{
    if (S2CON & 0x01)
    {
        S2CON &= ~0x01;
    }
}

void Uart2Init()
{
    S2CON |= 0x50;
    T2L = BRT;
    T2H = BRT >> 8;
    AUXR |= 0x14;
	IE2 |= 0x01;
    EA = 1;
}

void Uart2Send(u8 dat)
{
	S2BUF = dat;
    while ((S2CON & 0x02)!=0x02);
    S2CON &= ~0x02; 
}

void Uart2SendStr(u8 *p)
{
    while (*p!='\0')
    {
        Uart2Send(*p++);
    }
}

void Uart2SendStrLen(u8 *p,u8 length)
{
    while (length--)
    {
        Uart2Send(*p++);
    }
}
