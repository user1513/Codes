#include  "usart2.h"


void Usart2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitTypeState;
	USART_InitTypeDef USART_InitTypeState;
	NVIC_InitTypeDef NVIC_InitTypeState;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeState.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitTypeState.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitTypeState.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitTypeState);
	
	GPIO_InitTypeState.GPIO_Mode=GPIO_Mode_IN_FLOATING;;
	GPIO_InitTypeState.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitTypeState);
	
	USART_InitTypeState.USART_BaudRate=bound;
	USART_InitTypeState.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitTypeState.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitTypeState.USART_Parity=USART_Parity_No;
	USART_InitTypeState.USART_StopBits=USART_StopBits_1;
	USART_InitTypeState.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2,&USART_InitTypeState);
	
	NVIC_InitTypeState.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitTypeState.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitTypeState.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitTypeState.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitTypeState);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);
	
}

void Usart2_Send_Byte(u8 data)
{
	USART2->SR;
	USART2->DR=data;
	while((USART2->SR&0x40)==0);
	USART2->SR &=~0X40;
}

void Usart2_String_Send(u8 *data)
{
    while(*data!='\0')
        {
            Usart2_Send_Byte(*data);
            data++;
        }
}

void Usart2_String_Send1(u8 *data,u8 length)
{
    while(length--)
        {
            Usart2_Send_Byte(*data);
            data++;
        }
}

u8 array[9];
int arraynum = 0;
void USART2_IRQHandler(void)
{
	u8 res;	      
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		res =USART_ReceiveData(USART2);
		switch(arraynum)
		{
						case 0 : if ( res == 0x55) {array[arraynum] = res; arraynum++;}else arraynum = 0;  break;
						case 1 :if ( res == 0xAA) {array[arraynum] = res; arraynum++;}else arraynum = 0;break;
						case 2 :
						case 3 :
						case 4 :
						case 5 :
						case 6 :{array[arraynum] = res; arraynum++;}break;
						case 7 :if ( res == 0xFF) {array[arraynum] = res; array[arraynum + 1] =0XFF; arraynum = 0; }else arraynum = 0;break;
						default:break;
		}
	}  				 											 
}   
