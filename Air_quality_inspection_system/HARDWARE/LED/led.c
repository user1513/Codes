#include "led.h"


void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��PD �˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				     //LED-->PD.2 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOD.2
	GPIO_SetBits(GPIOC,GPIO_Pin_13);						           //PC.13 ����ߣ�������
}


void PM2_5_Led_Io_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 		 //ʹ��PA �˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				     //LED-->PD.2 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 	 //��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					     //�����趨������ʼ��GPIOD.2
	GPIO_SetBits(GPIOA,GPIO_Pin_1);								 //����ߵ�ƽ
}






