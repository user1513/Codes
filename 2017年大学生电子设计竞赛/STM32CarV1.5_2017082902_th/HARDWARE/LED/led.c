#include "led.h"

//��ʼ��PB9Ϊ�����.��ʹ�����IO�ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
	RCC->APB2ENR |= 1<<3;		//ʹ��PORTBʱ��	
	
	GPIOB->CRH &= 0XFFFFFF0F; 
	GPIOB->CRH |= 0X00000030;	//PB.9 �������   	 
	GPIOB->ODR |= 1<<9;		//PB.9 �����
}
 
