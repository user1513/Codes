#include "timer.h"

void TIM5_Int_Init(u16 arr,u16 psc)  
{  
	RCC->APB1ENR|=1<<3;	//TIM5ʱ��ʹ��    
 	TIM5->ARR=arr;      //�趨�������Զ���װֵ   
	TIM5->PSC=psc;      //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	TIM5->DIER|=1<<0;   //��������ж�
	TIM5->DIER|=1<<6;   //�������ж�	
	TIM5->CR1|=0x01;    //ʹ�ܶ�ʱ��
	MY_NVIC_Init(1,1,TIM5_IRQn,2);
	
}  


