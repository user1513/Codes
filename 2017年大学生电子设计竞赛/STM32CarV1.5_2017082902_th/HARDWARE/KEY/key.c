#include "key.h"
#include "delay.h"

								    
//������ʼ������
void KEY_Init(void)
{
	RCC->APB2ENR |= 1<<3;		
	
	GPIOB->CRL&=0XFFFFFFF0;
	GPIOB->CRL|=0X00000008; 				   
	GPIOB->ODR|=0<<0;			   
} 
