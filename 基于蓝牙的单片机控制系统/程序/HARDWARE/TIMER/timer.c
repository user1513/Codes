#include "timer.h"

void Timer0Init(void)		//10毫秒@12.000MHz
{
    AUXR &= 0x7F;		//定时器时钟12T模式
    TMOD &= 0xF0;		//设置定时器模式
    TMOD |= 0x01;		//设置定时器模式
    TL0 = 0xF0;		//设置定时初值
    TH0 = 0xD8;		//设置定时初值
    TF0 = 0;		//清除TF0标志
    TR0 = 1;		//定时器0开始计时
    IE |=0X82;
}

void tim0() interrupt 1
{

    TL0 = 0xF0;		//设置定时初值
    TH0 = 0xD8;		//设置定时初值

}

