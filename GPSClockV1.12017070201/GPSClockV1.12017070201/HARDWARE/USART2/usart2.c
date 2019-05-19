#include "USART2.h"
//#include "GPS.h"
#include "led.h"
/**************************************************************
*��������USART2_Init----���ڳ�ʼ��
*��  ����bound----������
*��  ����USART2_Init(115200); //����3��ʼ��,����������Ϊ115200bps
**************************************************************/
void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//����ʱ��ʹ��,GPIOʱ��ʹ��,����ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//���ڸ�λ
	USART_DeInit(USART2);
	
	//GPIO�˿�ģʽ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //USART2_TX----PA.2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��PA.2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //USART2_RX----PA.3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��PA.3

   //Usart2 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//���ڲ�����ʼ��
	USART_InitStructure.USART_BaudRate = bound; //����������
	USART_InitStructure.USART_WordLength =  USART_WordLength_8b; //�ֳ�Ϊ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx; //�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); //��ʼ������
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
}



