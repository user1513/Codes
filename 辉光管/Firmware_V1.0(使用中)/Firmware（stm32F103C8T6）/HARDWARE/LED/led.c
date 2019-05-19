#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PD �˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				     //LED-->PD.2 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOD.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				     //LED-->PD.2 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOD.2
	GPIO_SetBits(GPIOA,GPIO_Pin_0);						           //PD.2 ����ߣ�������
}


 




