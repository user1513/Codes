#include "timer.h"

void TIM5_Int_Init(u16 arr,u16 psc)  
{  
	RCC->APB1ENR|=1<<3;	//TIM5时钟使能    
 	TIM5->ARR=arr;      //设定计数器自动重装值   
	TIM5->PSC=psc;      //预分频器7200,得到10Khz的计数时钟
	TIM5->DIER|=1<<0;   //允许更新中断
	TIM5->DIER|=1<<6;   //允许触发中断	
	TIM5->CR1|=0x01;    //使能定时器
	MY_NVIC_Init(1,1,TIM5_IRQn,2);
	
}  


