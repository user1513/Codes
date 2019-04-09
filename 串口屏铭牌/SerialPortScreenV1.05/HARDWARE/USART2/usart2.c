#include "app.h"

extern uint8_t Uart2portData[];
extern tTimer usart2Timer;
extern uint8_t USART2_Rx_Progress_DateLength;

void Usart2_Init(u32 bound){
  //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//使能USART2

	//USART2_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.2

	//USART2_RX	  GPIOA.3初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.3  

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1 ;//抢占优先级11
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART2, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART2, ENABLE);                    //使能串口1 

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

u8 Uart2Num=0;
void USART2_IRQHandler(void)                	//串口2中断服务程序
{
		if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)
	{
		 Uart2portData[Uart2Num++] = USART_ReceiveData(USART2);
         if(Uart2Num > UART2RX_LENMAX)
            Uart2Num = 0;
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}

extern uint8_t Rx2_Last_DataLength;

void IsUsart2ReadIdeal(void)
{
	if(Uart2Num > 0)
	{
		if(Rx2_Last_DataLength == Uart2Num)
		{
			tTimerStart(&usart2Timer);
		}
		else if(USART2_Rx_Progress_DateLength ==0)
		{
			tTimerStop(&usart2Timer);
			
			Rx2_Last_DataLength = Uart2Num;
		}
	}
}





















