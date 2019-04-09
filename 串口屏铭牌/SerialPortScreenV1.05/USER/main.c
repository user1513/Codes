#include "app.h"

//�������*************************************************
extern uint8_t Uart2Num;
extern uint8_t Uart1Num;

extern uint8_t StrPack[][40];
//---������ʼ��
uint8_t USART2_Rx_Progress_DateLength;

uint8_t Rx2_Last_DataLength = 0;

uint8_t Uart2portData[100] = {0};           //���ڽ��ջ�����
uint8_t Usart2Data_MEM[100] = {0};		   //���ڽ��մ�����
uint8_t cdata[USART2_DATAFRAME_LENGTH_MAX] = {0};//��������֡��������

///////////////////////////////////////////////////////////////
//����1�й�����
uint8_t Uart1portData[UART1RX_LENMAX];	//��������������
tTimer usart1Timer;
uint8_t USART1_Rx_Progress_DateLength;
uint8_t Rx1_Last_DataLength = 0;
uint8_t Usart1Data_MEM[100] = {0};		   //���ڽ��մ�����
uint8_t UART1_Rx_Progress_DateLength;






//---��������
uint8_t IsDataPackLoss(uint8_t *arg, uint8_t length);
//END******************************************************

//��ʱ�����***********************************************
//---������ʼ��
tList tTimerHardList;
tTimer usart2Timer;
tTimer LEDTimer;
tTimer ReceivingTimeout_Timer;
//END***********************************************

//SerialPortScreen��־λ
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
    �������ݵĴ���ͽ��ղ�ͬ����������ڵ�������Դ�϶࣬�������ݵĴ���Ƚϸ��ӣ����ܳ���һ�ν����˺ü�֡�����ݣ�����Ҫ��������֡���Ƚ��нض���ȡ��Ȼ����,  �ҵ�֡ͷ���ܴ����������ҵ�֡ͷ���ݵ�λ��
    */
    if(Usart2Data_MEM[datelength] ==  DATA_FRAME_HEADER>>8)//�ҵ�֡ͷ
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
					//������Խ��е����ݵĴ�������д�ɺ������ã������Ķ��������˽��飩
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
					USART2_Rx_Progress_DateLength -= datelength_temp;  //�ҵ�֡ͷ���Ϳ���һ�ν�ȡһ֡�ĳ��ȡ�������һ�εĴ���					
				}
				else
				{
					USART2_Rx_Progress_DateLength -=1;
				}
    }
    else   // �ڽ��յ����ݵ�������һһ�Ľ���ɨ�棬���ǵľ��ӵ���������������־��֡ͷ������������иĶ���
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

//uint8_t test[] = "001,��λ���Ͼ���ҵְҵ����ѧԺ,���֣��ƽ�,ְ�ƣ����չ���ѧԺԺ��;";

int main(void)
{
	//LED��ʼ��
	LED_Init(); 				
	//DELAY��ʼ��
	delay_init();		  
	//����2��ʼ��	
	Usart2_Init(9600);	
	//����1��ʼ��	
	uart_init(9600);
	//��ʱ�������ʼ��
	tTimerModuleInit();	
	//LED��ʱ����ʼ��
	tTimerInit(&LEDTimer, 500, led_switch, (void *)NULL, CONFIG_LOOP);  
	//����2���ն�ʱ����ʼ��
	tTimerInit(&usart2Timer, UART2TIME_PROPORTION, usart2Data_Handle, (void *)Uart2portData, CONFIG_ONLYONE);
	//����1���ն�ʱ����ʼ��
	tTimerInit(&usart1Timer, UART1TIME_PROPORTION, usart1Data_Handle, (void *)Uart1portData, CONFIG_ONLYONE);
	//���������ݽ��ճ�ʱ��ʱ����ʼ��
	tTimerInit(&ReceivingTimeout_Timer, RECEIVINGTIMEOUT, ReceivingTimeout_Handle, (void *)NULL, CONFIG_ONLYONE);
	//LED��ʱ����
	tTimerStart(&LEDTimer);
	//��ʱ��2��ʼ��
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
		//�����¼�������
		usartEvent_Handle();
		//������״̬����
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
