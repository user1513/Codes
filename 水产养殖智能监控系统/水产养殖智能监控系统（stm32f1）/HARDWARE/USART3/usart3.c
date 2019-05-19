#include "usart3.h"

void Usart3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitTypeState;
	USART_InitTypeDef USART_InitTypeState;
	NVIC_InitTypeDef NVIC_InitTypeState;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeState.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitTypeState.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitTypeState.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitTypeState);
	
	GPIO_InitTypeState.GPIO_Mode=GPIO_Mode_IN_FLOATING;;
	GPIO_InitTypeState.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitTypeState);
	
	USART_InitTypeState.USART_BaudRate=bound;
	USART_InitTypeState.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitTypeState.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitTypeState.USART_Parity=USART_Parity_No;
	USART_InitTypeState.USART_StopBits=USART_StopBits_1;
	USART_InitTypeState.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART3,&USART_InitTypeState);
	
	NVIC_InitTypeState.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitTypeState.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitTypeState.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitTypeState.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitTypeState);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART3,ENABLE);
	
}

void Usart3_Send_Byte(u8 data)
{
	USART3->SR;
	USART3->DR=data;
	while((USART3->SR&0x40)==0);
	USART3->SR &=~0X40;
}

void Usart3_String_Send(u8 *data)
{
    while(*data!='\0')
        {
            Usart3_Send_Byte(*data);
            data++;
        }
}


void Usart3_String_Send1(u8 *data,u8 length)
{
    while(length--)
        {
            Usart3_Send_Byte(*data);
            data++;
        }
}
u8 Usart3_Num=0;
u8 Usart3_array[7];


void USART2_NewIstr(void)

{  

    if (USART_GetFlagStatus(USART2, USART_FLAG_PE) != RESET)
   {
	USART_ReceiveData(USART2);
	USART_ClearFlag(USART2, USART_FLAG_PE);
   }
   if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
   {
     USART_ReceiveData(USART2);
     USART_ClearFlag(USART2, USART_FLAG_ORE);
   }

    if (USART_GetFlagStatus(USART2, USART_FLAG_FE) != RESET)
   {
       USART_ReceiveData(USART2);
      USART_ClearFlag(USART2, USART_FLAG_FE);

   }
}


void USART3_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!= RESET)
	{
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
}

















