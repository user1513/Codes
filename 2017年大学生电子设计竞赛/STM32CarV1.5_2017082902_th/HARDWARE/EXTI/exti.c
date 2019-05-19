#include "exti.h"
#include "delay.h" 
#include "key.h"
#include "main.h"

extern u8 Collision;
extern u8 pz_up_static;
//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	pz_up_static = 1;
//	delay_ms(15);	//����
	
	
	
//	if(KEY == 1)
//	{				 
//		Collision = Collision + 1;
//		//����
//		USART1->DR=0x55;
//		while((USART1->SR&0X40)==0);//�ȴ����ͽ���
//		USART1->DR=0x02;
//		while((USART1->SR&0X40)==0);//�ȴ����ͽ���
//		USART1->DR=0xFF;
//		while((USART1->SR&0X40)==0);//�ȴ����ͽ���
//		USART1->DR=Collision;
//		while((USART1->SR&0X40)==0);//�ȴ����ͽ���
//		USART1->DR=(u8)(Collision+0xFF+0x02);
//		while((USART1->SR&0X40)==0);//�ȴ����ͽ���
//		USART1->DR=0xAA;
//		while((USART1->SR&0X40)==0);//�ȴ����ͽ���
//		
//	}		 
	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
}
//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{
 	Ex_NVIC_Config(GPIO_B,0,RTIR); 	//�½��ش���
	MY_NVIC_Init(2,3,EXTI0_IRQn,2);	//��ռ2�������ȼ�3����2 
	//����
	USART1->DR=0x55;
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���
	USART1->DR=0x02;
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���
	USART1->DR=0xFF;
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���
	USART1->DR=Collision;
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���
	USART1->DR=(u8)(Collision+0xFF+0x02);
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���
	USART1->DR=0xAA;
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���	
}

