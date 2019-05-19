#include "mp3play.h"

void MP3_Init(u32 Bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//����ʱ��ʹ��,GPIOʱ��ʹ��,����ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//���ڸ�λ
	USART_DeInit(USART3);
	
	//GPIO�˿�ģʽ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //USART2_TX----PB.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //USART3_RX----PB.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

   //Usart2 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//���ڲ�����ʼ��
	USART_InitStructure.USART_BaudRate = Bound; //����������
	USART_InitStructure.USART_WordLength =  USART_WordLength_8b; //�ֳ�Ϊ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx; //�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 


}

void USART3_IRQHandler(void)
{
	u8 res;	      
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		res =USART_ReceiveData(USART3);		 
	}  				 											 
}   



void MP3_Play(u8 Song)
{
	u8 i;
	u8 Com_Data[7] = {0x7E,0x05,0x41,0x00,0x00,0x00,0xEF};
	
	Com_Data[3] = Song / 256;
	Com_Data[4] = Song % 256;
	
	Com_Data[5] = Com_Data[1] ^ Com_Data[2] ^ Com_Data[3] ^ Com_Data[4];
	
	for(i=0; i<7; i++)
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		USART_SendData(USART3,Com_Data[i]);
	}
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}

void MP3_SetVolume(u8 Volume)
{
	u8 i;
	u8 Com_Data[6] = {0x7E,0x04,0x31,0x00,0x00,0xEF};
	
	Com_Data[3] = Volume;

	Com_Data[4] = Com_Data[1] ^ Com_Data[2] ^ Com_Data[3];
	
	for(i=0; i<6; i++)
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		USART_SendData(USART3,Com_Data[i]);
	}
}

void MP3_Stop(void)
{
	u8 i;
	u8 Com_Data[5] = {0x7E,0x03,0x1E,0x1D,0xEF};
	for(i=0; i<5; i++)
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		USART_SendData(USART3,Com_Data[i]);
	}
}



