#include "led.h"

//初始化PB9为输出口.并使能这个IO口的时钟		    
//LED IO初始化
void LED_Init(void)
{
	RCC->APB2ENR |= 1<<3;		//使能PORTB时钟	
	
	GPIOB->CRH &= 0XFFFFFF0F; 
	GPIOB->CRH |= 0X00000030;	//PB.9 推挽输出   	 
	GPIOB->ODR |= 1<<9;		//PB.9 输出高
}
 
