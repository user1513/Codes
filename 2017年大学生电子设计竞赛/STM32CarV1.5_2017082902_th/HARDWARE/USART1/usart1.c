#include "usart1.h"
#include "main.h"

//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void uart1_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0XFFFFF00F;//IO״̬����
	GPIOA->CRH|=0X000008B0;//IO״̬���� 
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж� 
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ� 
}
u8 Usart1ReceiveCounter;
u8 Usart1Data[8];

extern u8 Turn_left_90_value;
extern u8 Turn_right_90_value;

//�������ݰ������ж�
void USART1_IRQHandler(void)
{
	u8 res;	
	u8 checknumber = 0x0;//�ۼ�У��ֲ�����
	u8 i;
	if(USART1->SR&(1<<5))	//���յ�����
	{	 
		res = USART1->DR;
		Usart1Data[Usart1ReceiveCounter] = res;
		switch(Usart1ReceiveCounter)
		{
			case 0:
				if(Usart1Data[Usart1ReceiveCounter] == 0x55)
				{
					Usart1ReceiveCounter++;
				}else
				{
					Usart1ReceiveCounter = 0;
				}
				break;
			case 1:
				if(Usart1Data[Usart1ReceiveCounter] == 0x01)
				{
					Usart1ReceiveCounter++;
				}else
				{
					Usart1ReceiveCounter = 0;
				}
				break;
			case 2:
				Usart1ReceiveCounter++;
				break;
			case 3:
				Usart1ReceiveCounter++;
				break;
			case 4:
				checknumber = 0x00;//����
				for(i=1; i<Usart1ReceiveCounter; i++)
				{
					checknumber += Usart1Data[i];
				}
				checknumber = checknumber & 0xFF;//����У��λ��0x00-0xff֮��
				if(Usart1Data[Usart1ReceiveCounter] == checknumber)
				{
					//�ɹ�
					Usart1ReceiveCounter++;
					switch(Usart1Data[2])
					{
						case 0x00:
							if((Usart1Data[3]>=1)&&(Usart1Data[3]<=6))
							{
								if(Car_Garage == 0)
								{
									Car_Garage = Usart1Data[3];
								}
							}else if(Usart1Data[3] == 7)
							{
								__set_FAULTMASK(1);
								NVIC_SystemReset();
							}else if((Usart1Data[3] >= 8)&&(Usart1Data[3] <= 9))
							{
								if(Car_Garage == 0)
								{
									Car_Garage = Usart1Data[3];
								}								
							}
							break;
						case 0x01:
								Turn_left_90_value = Usart1Data[3];
							break;
						case 0x02:
								Turn_right_90_value = Usart1Data[3];
							break;
						default:
							break;
					}
				}else 
				{
					//ʧ��
					Usart1ReceiveCounter = 0;
				}
				break;
			case 5://��β
				if(Usart1Data[Usart1ReceiveCounter] == 0xAA)//��β
				{
					//OK
					//��ʼ����
				}
				Usart1ReceiveCounter = 0;
				break;
			default:break;
		}								     
	}
} 
