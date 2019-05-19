#include "timer.h"
extern unsigned int delay_time;



void Timer2Init(void)		//250微秒@11.0592MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xD7;		//设置定时初值
	T2H = 0xFD;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 |= 0x04;                    //开定时器2中断
    EA = 1;
}


//-----------------------------------------------
//中断服务程序
void t2int() interrupt 12           //中断入口
{
	static int i = 0 , t = 0;
	
	T2L = 0xD7;		//设置定时初值
	T2H = 0xFD;		//设置定时初值
	
	t = 20000 / delay_time ;
	if( ++i >= t) 
	{
		i = 0;
		P32 = ~P32;
		P34 = ~P34;
	}
		
	
			
}

