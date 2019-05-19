#include "mp3play.h"

void MP3_Init(u32 Bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//串口时钟使能,GPIO时钟使能,复用时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//串口复位
	USART_DeInit(USART3);
	
	//GPIO端口模式设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //USART2_TX----PB.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //USART3_RX----PB.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);

   //Usart2 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	//串口参数初始化
	USART_InitStructure.USART_BaudRate = Bound; //波特率设置
	USART_InitStructure.USART_WordLength =  USART_WordLength_8b; //字长为8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx; //收发模式

	USART_Init(USART3, &USART_InitStructure); //初始化串口
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART3, ENABLE);                    //使能串口 


}

void USART3_IRQHandler(void)
{
	u8 res;	      
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
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
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
		USART_SendData(USART3,Com_Data[i]);
	}
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
}

void MP3_SetVolume(u8 Volume)
{
	u8 i;
	u8 Com_Data[6] = {0x7E,0x04,0x31,0x00,0x00,0xEF};
	
	Com_Data[3] = Volume;

	Com_Data[4] = Com_Data[1] ^ Com_Data[2] ^ Com_Data[3];
	
	for(i=0; i<6; i++)
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
		USART_SendData(USART3,Com_Data[i]);
	}
}

void MP3_Stop(void)
{
	u8 i;
	u8 Com_Data[5] = {0x7E,0x03,0x1E,0x1D,0xEF};
	for(i=0; i<5; i++)
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
		USART_SendData(USART3,Com_Data[i]);
	}
}



