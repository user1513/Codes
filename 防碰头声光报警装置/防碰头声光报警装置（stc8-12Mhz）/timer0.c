#include "timer0.h"


void Timer0Init(void)		//1毫秒@24.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x9A;		//设置定时初值
	TH0 = 0xA9;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0 = 1;                                    //使能定时器中断
    EA = 1;
}



u16 Time0_500ms=0;
void Timer1() interrupt 1
{
	TL0 = 0x9A;		//设置定时初值
	TH0 = 0xA9;		//设置定时初值
	
	if(++Time0_500ms==500)
	{
		Time0_500ms=0;
		LEDPORT=~LEDPORT;		
	}
	TF0=0;
}

