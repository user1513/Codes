#include "usart3.h"
#include "main.h"

/**************************************************************************
�������ܣ�����3��ʼ��
��ڲ�����pclk2:PCLK2 ʱ��Ƶ��(Mhz)    bound:������
����  ֵ����
**************************************************************************/
void uart3_init(u32 pclk2,u32 bound)
{  	 
	
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<3;   //ʹ��PORTB��ʱ��  
	RCC->APB1ENR|=1<<18;  //ʹ�ܴ���ʱ�� 
	GPIOB->CRH&=0XFFFF00FF; 
	GPIOB->CRH|=0X00008B00;//IO״̬����
	GPIOB->ODR|=1<<10;	  
	RCC->APB1RSTR|=1<<18;   //��λ����1
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	      	   
	//����������
 	USART3->BRR=mantissa; // ����������	 
	USART3->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж� 
	USART3->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,0,USART3_IRQn,2);//��2��������ȼ� 
}

u8 Usart3ReceiveCounter;
u8 Usart3Data[8];

/**************************************************************************
�������ܣ�����3�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
void USART3_IRQHandler(void)
{
	u8 res;
	u8 checknumber = 0x0;//�ۼ�У��ֲ�����
	u8 i;
	if(USART3->SR&(1<<5))//���յ�����
	{	  
		res = USART3->DR;
		Usart3Data[Usart3ReceiveCounter] = res;
		switch(Usart3ReceiveCounter)
		{
			case 0:
				if(Usart3Data[Usart3ReceiveCounter] == 0x5A)
				{
					Usart3ReceiveCounter++;
				}else
				{
					Usart3ReceiveCounter = 0;
				}
				break;
			case 1:
				if(Usart3Data[Usart3ReceiveCounter] == 0x5A)
				{
					Usart3ReceiveCounter++;
				}else
				{
					Usart3ReceiveCounter = 0;
				}
				break;
			case 2:
				if(Usart3Data[Usart3ReceiveCounter] == 0x15)
				{
					Usart3ReceiveCounter++;
				}else
				{
					Usart3ReceiveCounter = 0;
				}
				break;
			case 3:
				if(Usart3Data[Usart3ReceiveCounter] == 0x03)
				{
					Usart3ReceiveCounter++;
				}else
				{
					Usart3ReceiveCounter = 0;
				}
				break;
			case 4:
				Usart3ReceiveCounter++;
				break;
			case 5:
				Usart3ReceiveCounter++;
				break;
			case 6:
				Usart3ReceiveCounter++;
				break;
			case 7:
				checknumber = 0x00;//����
				for(i=1; i<Usart3ReceiveCounter; i++)
				{
					checknumber += Usart3Data[i];
				}
				checknumber = checknumber & 0xFF;//����У��λ��0x00-0xff֮��
				if(Usart3Data[Usart3ReceiveCounter] == checknumber)
				{
					//�ɹ�
					Laser1_Distance = Usart3Data[4]<<8;
					Laser1_Distance |= Usart3Data[5];
					Laser1_Mode = Usart3Data[6];
					
				}else 
				{
					//ʧ��
				}
				Usart3ReceiveCounter = 0;
				break;
			default:
				Usart3ReceiveCounter = 0;
				break;
		}
	}
} 

