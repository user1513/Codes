#include "motor.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
void MiniBalance_Motor_Init(void)
{
  RCC->APB2ENR|=1<<2;        //PORTAʱ��ʹ��     
	GPIOA->CRH&=0XFFFF0000;    //PORTA�������
	GPIOA->CRH|=0X0000BBBB;    //PORTA 8 9 10 11�������

	RCC->APB2ENR|=1<<4;        //PORTCʱ��ʹ��     
	GPIOC->CRH&=0XFFFFFF00;    //PORTC8�������
	GPIOC->CRH|=0X000000BB;    //PORTC8�������
	
	GPIOC->CRL&=0X00FFFFFF;    //PORTC6 7�������
	GPIOC->CRL|=0XBB000000;    //PORTC6 7�������
}
void MiniBalance_PWM_Init(u16 arr,u16 psc)
{		 					 
	MiniBalance_Motor_Init();  //��ʼ�������������IO
	RCC->APB2ENR|=1<<11;       //ʹ��TIM1ʱ��    
	RCC->APB2ENR|=1<<13;       //ʹ��TIM8ʱ��    

	
	TIM1->ARR=arr;             //�趨�������Զ���װֵ 
	TIM1->PSC=psc;             //Ԥ��Ƶ������Ƶ
	TIM1->CCMR1|=6<<4;         //CH1 PWM1ģʽ	
	TIM1->CCMR1|=6<<12;        //CH2 PWM1ģʽ	
	TIM1->CCMR2|=6<<4;         //CH3 PWM1ģʽ	
	TIM1->CCMR2|=6<<12;        //CH4 PWM1ģʽ	
	
	TIM1->CCMR1|=1<<3;         //CH1Ԥװ��ʹ��	  
	TIM1->CCMR1|=1<<11;        //CH2Ԥװ��ʹ��	 
	TIM1->CCMR2|=1<<3;         //CH3Ԥװ��ʹ��	  
	TIM1->CCMR2|=1<<11;        //CH4Ԥװ��ʹ��	  
	
	TIM1->CCER|=1<<0;         //CH1���ʹ��	
	TIM1->CCER|=1<<4;         //CH2���ʹ��	   
	TIM1->CCER|=1<<8;         //CH3���ʹ��	 
	TIM1->CCER|=1<<12;        //CH4���ʹ��
	TIM1->BDTR |= 1<<15;      //TIM����Ҫ��仰�������PWM
	TIM1->CR1=0x8000;         //ARPEʹ�� 
	TIM1->CR1|=0x01;          //ʹ�ܶ�ʱ��1 			
	
	
	TIM8->ARR=arr;             //�趨�������Զ���װֵ 
	TIM8->PSC=psc;             //Ԥ��Ƶ������Ƶ
	TIM8->CCMR1|=6<<4;         //CH1 PWM1ģʽ	
	TIM8->CCMR1|=6<<12;        //CH2 PWM1ģʽ	
	TIM8->CCMR2|=6<<4;         //CH3 PWM1ģʽ	
	TIM8->CCMR2|=6<<12;        //CH4 PWM1ģʽ	
	
	TIM8->CCMR1|=1<<3;         //CH1Ԥװ��ʹ��	  
	TIM8->CCMR1|=1<<11;        //CH2Ԥװ��ʹ��	 
	TIM8->CCMR2|=1<<3;         //CH3Ԥװ��ʹ��	  
	TIM8->CCMR2|=1<<11;        //CH4Ԥװ��ʹ��	  
	
	TIM8->CCER|=1<<0;         //CH1���ʹ��	
	TIM8->CCER|=1<<4;         //CH2���ʹ��	   
	TIM8->CCER|=1<<8;         //CH3���ʹ��	 
	TIM8->CCER|=1<<12;        //CH4���ʹ��
	TIM8->BDTR |= 1<<15;      //TIM����Ҫ��仰�������PWM
	TIM8->CR1=0x8000;         //ARPEʹ�� 
	TIM8->CR1|=0x01;          //ʹ�ܶ�ʱ�� 			
} 

