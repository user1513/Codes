#include "uart2.h"

#define FOSC            22118400UL
#define BRT             (65536 - FOSC / 9600 / 4)

u8 Uart2Num=0;


void Uart2Isr() interrupt 8 
{
	u8 _uart2val=0;
    if (S2CON & 0x01)
    {
        S2CON &= ~0x01;
		
		if(My1680uFlag==Enable)
		{
			_uart2val=S2BUF;
			switch(Uart2Num)
			{
				case 0:if('O'==_uart2val) Uart2Num++;else Uart2Num=0;break;
				case 1:if('K'==_uart2val) {My1680uFlag=Disable;Uart2Num=0;}else Uart2Num=0; break;
				default:Uart2Num=0;break;
			}
			
		}
		Uart1_Send(_uart2val);
		
		
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
        Uart2Send(*p);
		p++;
    }
}

void Uart2SendStrLen(u8 *p,u8 length)
{
    while (length--)
    {
        Uart2Send(*p);
		p++;
    }
}
