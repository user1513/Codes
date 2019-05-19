#include "exti.h"
#include "delay.h" 
#include "key.h"
#include "main.h"

extern u8 Collision;
extern u8 pz_up_static;
//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	pz_up_static = 1;
//	delay_ms(15);	//消抖
	
	
	
//	if(KEY == 1)
//	{				 
//		Collision = Collision + 1;
//		//发送
//		USART1->DR=0x55;
//		while((USART1->SR&0X40)==0);//等待发送结束
//		USART1->DR=0x02;
//		while((USART1->SR&0X40)==0);//等待发送结束
//		USART1->DR=0xFF;
//		while((USART1->SR&0X40)==0);//等待发送结束
//		USART1->DR=Collision;
//		while((USART1->SR&0X40)==0);//等待发送结束
//		USART1->DR=(u8)(Collision+0xFF+0x02);
//		while((USART1->SR&0X40)==0);//等待发送结束
//		USART1->DR=0xAA;
//		while((USART1->SR&0X40)==0);//等待发送结束
//		
//	}		 
	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
}
//外部中断初始化程序
void EXTIX_Init(void)
{
 	Ex_NVIC_Config(GPIO_B,0,RTIR); 	//下降沿触发
	MY_NVIC_Init(2,3,EXTI0_IRQn,2);	//抢占2，子优先级3，组2 
	//发送
	USART1->DR=0x55;
	while((USART1->SR&0X40)==0);//等待发送结束
	USART1->DR=0x02;
	while((USART1->SR&0X40)==0);//等待发送结束
	USART1->DR=0xFF;
	while((USART1->SR&0X40)==0);//等待发送结束
	USART1->DR=Collision;
	while((USART1->SR&0X40)==0);//等待发送结束
	USART1->DR=(u8)(Collision+0xFF+0x02);
	while((USART1->SR&0X40)==0);//等待发送结束
	USART1->DR=0xAA;
	while((USART1->SR&0X40)==0);//等待发送结束	
}

