#include "led.h"

/************************************************
 基于Zigbee的路灯控制系统
 工程模板
 注意，LED初始化函数 
 //修改日期:2018/4/6
 //版本：V1.0
 作者：袁有成
************************************************/


//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTypeInit;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitTypeInit.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitTypeInit.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitTypeInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitTypeInit);
	GPIO_InitTypeInit.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitTypeInit.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitTypeInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitTypeInit);
	GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
}
