#include "app.h"

//串口相关*************************************************
extern uint8_t Uart2Num;
extern uint8_t Uart1Num;

extern uint8_t StrPack[][40];
//---变量初始化
uint8_t USART2_Rx_Progress_DateLength;

uint8_t Rx2_Last_DataLength = 0;

uint8_t Uart2portData[100] = {0};           //串口接收缓存区
uint8_t Usart2Data_MEM[100] = {0};		   //串口接收待处理
uint8_t cdata[USART2_DATAFRAME_LENGTH_MAX] = {0};//串口数据帧处理数组

///////////////////////////////////////////////////////////////
//串口1有关配置
uint8_t Uart1portData[UART1RX_LENMAX];	//蓝牙接收数据区
tTimer usart1Timer;
uint8_t USART1_Rx_Progress_DateLength;
uint8_t Rx1_Last_DataLength = 0;
uint8_t Usart1Data_MEM[100] = {0};		   //串口接收待处理
uint8_t UART1_Rx_Progress_DateLength;






//---函数申明
uint8_t IsDataPackLoss(uint8_t *arg, uint8_t length);
//END******************************************************

//定时器相关***********************************************
//---变量初始化
tList tTimerHardList;
tTimer usart2Timer;
tTimer LEDTimer;
tTimer ReceivingTimeout_Timer;
//END***********************************************

//SerialPortScreen标志位
uint8_t CRC_Ack_Flag = 0;
uint32_t SerialPortScreen_Flag = 0;
uint8_t SerialPortScreen_Reg[11];

uint16_t key_val = 0;
uint8_t  version_num = 0;
void led_switch(void * arg)
{
	LED0 = ~LED0;
}

void usart2Data_Handle(void *arg)
{
	Rx2_Last_DataLength = 0;
	
	USART2_Rx_Progress_DateLength = Uart2Num;
	
	Uart2Num = 0;

	memcpy(Usart2Data_MEM,(uint8_t *)arg,USART2_Rx_Progress_DateLength);

}

void usart1Data_Handle(void *arg)
{
	Rx1_Last_DataLength = 0;
	
	USART1_Rx_Progress_DateLength = Uart1Num;
	
	Uart1Num = 0;

	memcpy(Usart1Data_MEM,(uint8_t *)arg,USART1_Rx_Progress_DateLength);

}

void ReceivingTimeout_Handle(void *arg)
{
	if(ReceivingTimeout_Timer.Count ++ < RECEIVINGTIMEOUT_NUM)
	{
		tTimerStop(&ReceivingTimeout_Timer);
	  SerialPortScreen_Usart_Send((uint8_t *)ReceivingTimeout_Timer.arg, ReceivingTimeout_Timer.Length);

	}
	else
	{
		tTimerStop(&ReceivingTimeout_Timer);
		ReceivingTimeout_Timer.Count = 0;
		SerialPortScreen_Flag |=0x00000001;
	}
}



uint8_t num_121 = 0;
void usartEvent_Handle(void)
{
	uint8_t datelength = 0;
	uint8_t datelength_temp = 0;
	if (USART2_Rx_Progress_DateLength>0)
	{              
    while(USART2_Rx_Progress_DateLength >=USART2_DATAFRAME_LENGTH_MIN)
    { 
    /*
    由于数据的处理和接收不同步，如果串口的数据来源较多，并且数据的处理比较复杂，可能出现一次接收了好几帧的数据，所以要按照数据帧长度进行截断提取，然后处理,  找到帧头才能处理，所以先找到帧头数据的位置
    */
    if(Usart2Data_MEM[datelength] ==  DATA_FRAME_HEADER>>8)//找到帧头
    {
			datelength++;
				if(Usart2Data_MEM[datelength] == (DATA_FRAME_HEADER&0x000000FF))
				{
					if(CRC_Ack_Flag == 1 && Usart2Data_MEM[datelength + 1] == 0x02)
					{
						 datelength_temp = USART2_ACK_DATAFRAME_LENGTH;
					}
					else if(Usart2Data_MEM[datelength + 1] != 0x02)
					{
						datelength_temp = Usart2Data_MEM[datelength + 1] + 3;
					}
					memcpy(cdata, Usart2Data_MEM + datelength - 1, datelength_temp);
					//这里可以进行的数据的处理，尽量写成函数调用，方便阅读，（个人建议）
					#if ISUSE_MODBUS_CHECK == 1
					if(IsDataPackLoss(cdata ,datelength_temp))
					{
						if(Usart2Data_MEM[datelength + 1] == 0x02)
						{
							CRC_Ack_Flag = 0;
							tTimerStop(&ReceivingTimeout_Timer);
							if(Usart2Data_MEM[datelength + 3] == 0x00)
							{
								SerialPortScreen_Flag |=0x00000002;
							}
						}
						else
						{
							if(((Usart2Data_MEM[datelength + 3] << 8) + Usart2Data_MEM[datelength + 4]) == 0x1000)
							{
								key_val = (Usart2Data_MEM[datelength + 6] << 8) + Usart2Data_MEM[datelength + 7];
							}
							else if(Usart2Data_MEM[datelength + 3] == 0x00)
							{
								version_num = (Usart2Data_MEM[datelength + 5]>>4) * 10 + (Usart2Data_MEM[datelength + 5] &0x0f);
							}
						}
					}
					#endif
					datelength += datelength_temp - 1;             
					USART2_Rx_Progress_DateLength -= datelength_temp;  //找到帧头，就可以一次截取一帧的长度。进行下一次的处理					
				}
				else
				{
					USART2_Rx_Progress_DateLength -=1;
				}
    }
    else   // 在接收到数据的数组里一一的进行扫面，不是的就扔掉。（针对于特殊标志在帧头，其他情况自行改动）
    {
        datelength +=1;
        USART2_Rx_Progress_DateLength -=1;
    }
    }
    USART2_Rx_Progress_DateLength =0;
    datelength=0;
	}   
}
#if ISUSE_MODBUS_CHECK == 1
uint8_t IsDataPackLoss(uint8_t *arg, uint8_t length)
{
	uint16_t checkVal = 0;
	checkVal = (*(arg + length - 2)<<8) | *(arg + length - 1);
	if(checkVal == ModBus_CRC(arg + 3, length - 5))
			return 1;
	else
			return 0;
}
#endif

//uint8_t test[] = "001,单位：南京工业职业技术学院,名字：黄杰,职称：航空工程学院院长;";

int main(void)
{
	//LED初始化
	LED_Init(); 				
	//DELAY初始化
	delay_init();		  
	//串口2初始化	
	Usart2_Init(9600);	
	//串口1初始化	
	uart_init(9600);
	//定时器链表初始化
	tTimerModuleInit();	
	//LED定时器初始化
	tTimerInit(&LEDTimer, 500, led_switch, (void *)NULL, CONFIG_LOOP);  
	//串口2接收定时器初始化
	tTimerInit(&usart2Timer, UART2TIME_PROPORTION, usart2Data_Handle, (void *)Uart2portData, CONFIG_ONLYONE);
	//串口1接收定时器初始化
	tTimerInit(&usart1Timer, UART1TIME_PROPORTION, usart1Data_Handle, (void *)Uart1portData, CONFIG_ONLYONE);
	//串口屏数据接收超时定时器初始化
	tTimerInit(&ReceivingTimeout_Timer, RECEIVINGTIMEOUT, ReceivingTimeout_Handle, (void *)NULL, CONFIG_ONLYONE);
	//LED定时启动
	tTimerStart(&LEDTimer);
	//定时器2初始化
  TIM2_Int_Init(719, 100);
	//delay_ms(1000);

	
	while(1)
	{
		if(USART1_Rx_Progress_DateLength>0)
	{ 
		Data_Parsing_Func(Usart1Data_MEM);
		USART1_Rx_Progress_DateLength = 0;
		SerialPortScreen_Send_China_Language(0x0000,strlen((const char *)StrPack[1]),(const char *)StrPack[1]);
		delay_ms(100);
		SerialPortScreen_Send_China_Language(0x0100,strlen((const char *)StrPack[2]),(const char *)StrPack[2]);
		delay_ms(100);
		SerialPortScreen_Send_China_Language(0x0200,strlen((const char *)StrPack[3]),(const char *)StrPack[3]);		
		delay_ms(100);
	}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		/*
		//串口事件处理函数
		usartEvent_Handle();
		//串口屏状态函数
		SerialPortScreen_State(SerialPortScreen_Flag);
		if(key_val != 0)
		{
			key_val = 0; 
			SerialPortScreen_ReadWrite_Reg_com(Reg_Com_Red, Version, 1, 1);
		}
		if(version_num != 0)
		{
			SerialPortScreen_ReadWrite_Variable_Reg_com(Variable_Reg_Com_Write, 0x2000, 2, version_num);
			version_num = 0;
		}
		*/
	}
}
