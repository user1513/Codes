#include "timer.h"

extern uint32_t STATE_FLAG;

void Timer0Init(void)		//50毫秒@11.0592MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x4C;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	IE |=0X82;
}

void tim0() interrupt 1
{
    static uint8_t _TimeOut_500ms = 0;
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x4C;		//设置定时初值
	
	if(_TimeOut_500ms ++ > 9)
	{
		_TimeOut_500ms = 0;
		STATE_FLAG |= 1 << 0;
	}
	
}

