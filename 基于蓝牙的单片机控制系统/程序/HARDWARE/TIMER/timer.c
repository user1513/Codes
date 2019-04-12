#include "timer.h"
#include "usart.h"
extern uint8_t TimeOut_status;

#define UART2TIMEOUTMAX   200 //串口2接收超时检测默认10ms
void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	IE |=0X82;
}

void tim0() interrupt 1
{
	static uint16_t UartTimeOutVal = 0; 
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	
	if(TimeOut_status & 0x01)
	{
		UartTimeOutVal++;
		if(UartTimeOutVal > UART2TIMEOUTMAX)
		{
			TimeOut_status |= 0x02;
			UartTimeOutVal= 0;
		}
	}
	else
	{
		UartTimeOutVal= 0;
	}
}

