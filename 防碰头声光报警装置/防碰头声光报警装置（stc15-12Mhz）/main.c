#include "intrins.h"
#include "usart.h"
#include <STC15F2K60S2.H>
#include "string.h"
#include "Ultrasonic.h"
#include "timer.h"
#include "Exti.h"
#include "HC_SR04.h"
#include "stdio.h"

void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}




void main()
{
	int i = 0;
	u16 jl=0;
	P3M0|=0x08;
	P3M1&=~0x08;
	Usart_Init();
	Timer2Init();
	TRIG = 0;
	ECHO = 1;
	Delay100ms();
	Delay100ms();
	while(1)
	{
		for(i = 0; i < 5; i++)
		{
			StartModule();
			while(!ECHO);		//当RX为零时等待
			AUXR |= 0x10;                   //定时器2开始计时
			while(ECHO);			//当RX为1计数并等待
			AUXR &= ~0x10;
			jl = Conut();
			if(jl != 0)
				UltrasonicDataPage(jl);
			Delay100ms();
		}
		P34=~P34;	
	}
}