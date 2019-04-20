#include "led.h"


static void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��PD �˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				     //LED-->PD.2 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOD.2
	GPIO_SetBits(GPIOC,GPIO_Pin_13);						           //PC.13 ����ߣ�������
}

static void BUZZER_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��PD �˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				     //LED-->PD.2 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOD.2
	GPIO_SetBits(GPIOA, GPIO_Pin_1);						           //PC.13 ����ߣ�������
}

void Periph_Init(void)
{
	LED_Init();
	BUZZER_Init();
}






