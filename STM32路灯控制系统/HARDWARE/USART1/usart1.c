#include "usart1.h"
#include "stm32f10x.h"
//RTrobot.org
//禁止用于商业行为

char GPS_Uart1_Data[256]={ 0 };
char GPS_Data[256]={ 0 };
unsigned int GPS_Uart1_Data_Num=0;
unsigned char GPS_Uart1_Start_flag=0;
unsigned char GPS_Stop_flag=0;

/********************************************************************************************
串口2发送函数
********************************************************************************************/
void Usart1_Send(unsigned char *str)
{
	while(*str != '\0')
	{		
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
		USART_SendData(USART1 , *str);
		str++;
	}
}


void usart1_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

void USART1_IRQHandler(void)
{
	unsigned char Data_char_usart1;	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Data_char_usart1=USART1->DR;
		if(Data_char_usart1=='$')
		{
			GPS_Uart1_Start_flag=1;
			GPS_Uart1_Data_Num=0;
		}
		if(GPS_Uart1_Start_flag==1)
		{
			GPS_Uart1_Data[GPS_Uart1_Data_Num]=Data_char_usart1;
			GPS_Uart1_Data_Num++;			
			if(Data_char_usart1=='\n')
			{
				GPS_Stop_flag=1;
				GPS_Uart1_Data_Num=0;
				GPS_Uart1_Start_flag=0;
			}	
		}
	}	
}
