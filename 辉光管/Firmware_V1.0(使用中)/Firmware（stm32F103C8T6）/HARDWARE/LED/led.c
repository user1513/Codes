#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); //使能PD 端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				     //LED-->PD.2 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					       //根据设定参数初始化GPIOD.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				     //LED-->PD.2 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					       //根据设定参数初始化GPIOD.2
	GPIO_SetBits(GPIOA,GPIO_Pin_0);						           //PD.2 输出高，不亮灯
}


 




