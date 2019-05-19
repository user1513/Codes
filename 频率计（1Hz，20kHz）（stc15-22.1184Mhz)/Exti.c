#include "Exti.h"
//#include "reg51.h"


//-----------------------------------------
//中断服务程序
void exint0() interrupt 0       //INT0中断入口
{
   // FLAG = P32;                //保存INT0口的状态, INT0=0(下降沿); INT0=1(上升沿)
}

//-----------------------------------------------

void Exti0_Init(void)
{
 //   FLAG = 0;
	P32=0;
    IT0 = 0;                    //设置INT0的中断类型 (1:仅下降沿 0:上升沿和下降沿)
    EX0 = 1;                    //使能INT0中断
    EA = 1;
}



