#include "timer1.h"

bit  UsartFlag=0;

//65536-24/1000

void Timer0Init(void)		//1毫秒@24.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x40;		//设置定时初值
	TH0 = 0xA2;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0 = 1;                                    //使能定时器中断
    EA = 1;
}




u16 Time1_50ms=0;
u16 Time1_500ms=0;
void Timer1() interrupt 1
{
	TL0 = 0x40;		//设置定时初值
	TH0 = 0xA2;		//设置定时初值
	
	if(++Time1_50ms==50)
	{
		Time1_50ms=0;
		UsartFlag=Enable;
	}
	if(++Time1_500ms==500)
	{
		Time1_500ms=0;
		P34=~P34;		
	}
	TF0=0;
}
