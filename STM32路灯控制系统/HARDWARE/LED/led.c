#include "led.h"

/************************************************
 ����Zigbee��·�ƿ���ϵͳ
 ����ģ��
 ע�⣬LED��ʼ������ 
 //�޸�����:2018/4/6
 //�汾��V1.0
 ���ߣ�Ԭ�г�
************************************************/


//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��

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
