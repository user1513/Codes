#include "app.h"


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTypeInit;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeInit.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitTypeInit.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitTypeInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitTypeInit);
}
