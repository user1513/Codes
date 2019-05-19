#include "timer.h"
bit flag = 0;
void Timer2Init(void)		//1毫秒@11.0592MHz
{
//    AUXR |= 0x04;                   //定时器2为1T模式
  AUXR &= ~0x04;                  //定时器2为12T模式
    T2L = 0;                     //初始化计时值
    T2H = 0;
    //AUXR |= 0x10;                   //定时器2开始计时
    AUXR &= ~0x10;                   //定时器2停止计时
    IE2 |= 0x04;                    //开定时器2中断
    EA = 1;

}

//-----------------------------------------------
//中断服务程序
void t2int() interrupt 12           //中断入口
{
	flag=1;
}

