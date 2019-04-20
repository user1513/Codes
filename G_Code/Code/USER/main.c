#include "main.h"
#include "bmp.h"
/************************************************  

************************************************/   
u32 Alarm_Clock_Val = 0 ;
u8 Medicine_Flag = 0; //��ҩ��־λ

void Periph_All_Init(void);
void Set_Alarm_Clock(void);


_bool OledDisplayTimeFlag = FALSE;
_bool OledDisplayParamFlag = FALSE;
_bool OledDisplayGasFlag = FALSE;
_bool Task500MsEventFlag = TRUE;
_bool MedicineEventFlag = TRUE;
_bool HeartRateEventFlag = TRUE;
#define PHVAL_MAX  1000 //PHֵ����
/* �û�����ǰ�豸״̬�ṹ��*/
dataPoint_t currentDataPoint;

extern gizwitsProtocol_t gizwitsProtocol;
extern u8 display_status;
extern u8 Water_Temp_status;
//WIFI����״̬
//wifi_sta 0: �Ͽ�
//         1: ������
u8 wifi_sta=0;

//Э���ʼ��
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MSϵͳ��ʱ
    usart3_init(9600);//WIFI��ʼ��
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
}
 int main(void)
 {	 
	u8 able[7] = {0x50,0x34,0x20,0x19,0x04,0x05,0x19};
	u8 temp = 0;
	u8 wifi_con=0;//��¼wifi����״̬ 1:���� 0:�Ͽ�
	u8 temperature;  	    
	u8 humidity;    
	Periph_All_Init();
	//gizwitsSetMode(WIFI_RESET_MODE);
	while(1)
	{
		if(Medicine_Flag == 0)
		{
			switch(display_status)
			{
				case 0:
				if(OledDisplayTimeFlag == TRUE)
				{	
					DS1302_GetTime(able);
					OledDisplayTimeFlag = FALSE;
					OLED_Clear_static(1, 2, 7);
					FuncReg(&OledDisplayTimeFlag, 10000, TRUE);
					OLED_Show_32_32(16*2-8,2,able[2] / 10 + '0');
					OLED_Show_32_32(16*3-8,2,able[2] % 10 + '0');
					OLED_Show_32_32(16*4-8,2,':');
					OLED_Show_32_32(16*5-8,2,able[1] / 10 + '0');
					OLED_Show_32_32(16*6-8,2,able[1] % 10 + '0');
					OLED_ShowChar(32,	  6, able[6] / 10 + '0',16);
					OLED_ShowChar(32+8*1, 6, able[6] % 10 + '0',16);
					OLED_ShowChar(32+8*2, 6, '-',16);
					OLED_ShowChar(32+8*3, 6, able[4] / 10 + '0',16);
					OLED_ShowChar(32+8*4, 6, able[4] % 10 + '0',16);
					OLED_ShowChar(32+8*5, 6, '-',16);
					OLED_ShowChar(32+8*6, 6, able[3] / 10 + '0',16);
					OLED_ShowChar(32+8*7, 6, able[3] % 10 + '0',16);
					
				}
				OledDisplayGasFlag = TRUE;
				OledDisplayParamFlag = TRUE;
				break;
				case 1:
				{
					if(OledDisplayParamFlag == TRUE)
					{
						OledDisplayParamFlag = FALSE;
						OLED_Clear_static(2, 2, 7);
						FuncReg(&OledDisplayParamFlag, 200, TRUE);
						OLED_ShowCHinese(16 * 0, 2, 12);
						OLED_ShowCHinese(16 * 1, 2, 13);
						OLED_ShowChar(16 * 2, 2, ':',16);
						OLED_ShowCHinese(16 * 6, 2, 16);
						OLED_ShowNum(16 * 3, 2, temperature, 3, 16);
						
						OLED_ShowCHinese(16 * 0, 4, 14);
						OLED_ShowCHinese(16 * 1, 4, 15);
						OLED_ShowChar(16 * 2, 4, ':',16);
						OLED_ShowChar(16 * 6, 4, '%',16);
						OLED_ShowNum(16 * 3, 4, humidity, 3, 16);
						
						OLED_ShowChar(8 * 0, 6, 'M',16);
						OLED_ShowChar(8 * 1, 6, 'Q',16);
						OLED_ShowChar(8 * 2, 6, '_',16);
						OLED_ShowChar(8 * 3, 6, '1',16);
						OLED_ShowChar(8 * 4, 6, ':',16);
						OLED_ShowNum(16 * 3, 6, ADC_Val[0], 4, 16);
						
					}
					OledDisplayTimeFlag = TRUE;
					OledDisplayGasFlag = TRUE;
				}
					break;	
				
				case 2:
					if(OledDisplayGasFlag == TRUE)
					{
						OledDisplayGasFlag = FALSE;
						FuncReg(&OledDisplayGasFlag, 5000, TRUE);
						OLED_Clear_static(3, 2, 7);
						OLED_ShowCHinese(16 * 0, 2, 30);
						OLED_ShowCHinese(16 * 1, 2, 31);
						OLED_ShowCHinese(16 * 2, 2, 32);
						OLED_ShowCHinese(16 * 3, 2, 33);
						OLED_ShowCHinese(16 * 4, 2, 34);
						OLED_ShowCHinese(16 * 5, 2, 35);
						OLED_ShowCHinese(16 * 6, 2, 36);
						OLED_ShowCHinese(16 * 0, 4, 37);
						OLED_ShowCHinese(16 * 1, 4, 38);
						OLED_ShowCHinese(16 * 2, 4, 39);
						OLED_ShowCHinese(16 * 3, 4, 40);
						OLED_ShowCHinese(16 * 4, 4, 41);
						OLED_ShowCHinese(16 * 5, 4, 42);
						OLED_ShowCHinese(16 * 6, 4, 43);
						OLED_ShowString(0, 6, (u8 *)"  Any Key Exit..",16);
						Jq_8400_Music_Num(MUSIC_GAS_WARNING);
					}
					break;
				
				case 3:
				{
					OLED_Clear_static(5, 2, 7);
					OLED_ShowCHinese(0 ,2 ,27);
					OLED_ShowCHinese(16 ,2 ,28);
					OLED_ShowCHinese(16 * 2 ,2 ,29);
					OLED_ShowChar(16 * 3, 2, ':',16);
					temp = Heart_Rate();	
					display_status = 4;	
				}
				break;	
				case 4:
				{
					if((temp/100) != 0)
						OLED_Show_32_32(16 * 4 - 8,3,temp/100 + '0');
					OLED_Show_32_32(16 * 4 +16- 8, 3,(temp % 100)/10 + '0');
					OLED_Show_32_32(16 * 4 +32- 8, 3,temp % 10 + '0');
					OLED_ShowString(32-8, 6, (uint8_t *)"          bpm", 16);
					display_status = 5;
					if(temp <= 60)
						Jq_8400_Music_Num(MUSIC_HEART_REAT_LOW);
					else if(temp < 110)
						Jq_8400_Music_Num(MUSIC_HEART_REAT_NORMAL);
					else
						Jq_8400_Music_Num(MUSIC_HEART_REAT_HIGH);
				}
				break;
				default: break;
			}
		}
		else if(MedicineEventFlag)
		{
			MedicineEventFlag = FALSE;
			FuncReg(&MedicineEventFlag, 5000, TRUE);
			
			OLED_Clear_static(4, 2, 7);
			OLED_ShowCHinese(18 * 0, 2, 17);
			OLED_ShowCHinese(18 * 1, 2, 18);
			OLED_ShowCHinese(18 * 2, 2, 19);
			OLED_ShowCHinese(18 * 3, 2, 20);
			OLED_ShowCHinese(18 * 4, 2, 21);
			OLED_ShowCHinese(18 * 2, 4, 22);
			OLED_ShowCHinese(18 * 3, 4, 23);
			OLED_ShowCHinese(18 * 4, 4, 24);
			OLED_ShowCHinese(18 * 5, 4, 25);
			OLED_ShowCHinese(18 * 6, 4, 26);
			OLED_ShowString(0, 6, (u8 *)"  Any Key Exit..",16);
			Jq_8400_Music_Num(MUSIC_MEDICINE);
		}
		
			
		if(Task500MsEventFlag == TRUE)
		{
			Task500MsEventFlag = FALSE;
			FuncReg(&Task500MsEventFlag, 500, TRUE);
			currentDataPoint.valueHum = humidity;
			currentDataPoint.valueGas = ADC_Val[0];
			currentDataPoint.valueHeart_Rate = temp;
			if(ADC_Val[0] > PHVAL_MAX){
				BUZEER_ENABLE;
				display_status = 2;
			}
			else{
				BUZEER_DISABLE;
			}
			currentDataPoint.valueTemp = temperature;
			DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ
			currentDataPoint.valuerssi = gizwitsProtocol.wifiStatusData.rssi;
		}
							    

	
		if(wifi_con!=wifi_sta)
		 {
			 wifi_con=wifi_sta;
			 //wifi_con?printf("connect"):printf("close");
		 }
		

		gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��
		
		
		
		
		
		
//		key = KEY_Scan(0);
//		if(key==KEY1_PRES)//KEY1����
//		{
//			printf("WIFI����AirLink����ģʽ\r\n");
//			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
//		}			
//		if(key == KEY1_PRES)//KEY_UP����
//		{  
//			printf("WIFI��λ����������������\r\n");
//			gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ 
//			wifi_sta=0;//��־wifi�ѶϿ�
//		}

	}
}
 




void Periph_All_Init(void)
{
	delay_init();	    	 //��ʱ������ʼ��	
	//KEY_Init();	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	Periph_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ� 
	 Key_Io_Init();
	BUZEER_DISABLE;
	uart_init(115200);
	usart2_init(9600);
	Gizwits_Init();         //Э���ʼ��
	OLED_Init();			//��ʼ��OLED  
	DHT11_Init();
	Adc_Init();
	OLED_Clear(); 
	Ds1302_GpioInit();
	Jq_8400_Set_Volume(20);
	delay_ms(10);
	Jq_8400_Music_Num(MUSIC_INIT);
	//DS1302_Initial();
	//DS1302_SetTime(able);
	//DS1302_GetTime(able);
	OLED_DrawBMP(0,0,128,1,BMP1);
	OLED_ShowCHinese(18 * 1, 2, 0);
	OLED_ShowCHinese(18 * 2, 2, 1);
	OLED_ShowCHinese(18 * 3, 2, 2);
	OLED_ShowChar(18 * 4, 2, 'N',16);
	OLED_ShowChar(18 * 5 - 8, 2, 'I',16);
	OLED_ShowChar(18 * 5, 2, 'I',16);
	OLED_ShowChar(18 * 6 - 8, 2, 'T',16);
	OLED_ShowCHinese(18 * 1, 4, 3);
	OLED_ShowCHinese(18 * 2, 4, 4);
	OLED_ShowCHinese(18 * 3, 4, 5);
	OLED_ShowCHinese(18 * 4, 4, 6);
	OLED_ShowCHinese(18 * 5, 4, 7);
	OLED_ShowCHinese(18 * 6, 4, 8);
	OLED_ShowChar(18 * 3, 6, '-', 16);
	OLED_ShowCHinese(18 * 4, 6, 9);
	OLED_ShowCHinese(18 * 5, 6, 10);
	OLED_ShowCHinese(18 * 6, 6, 11);
	FuncReg(&OledDisplayTimeFlag, 5000, TRUE);
}

void Set_Alarm_Clock(void)
{
	static u8 previousvalueTime = 0;
	if(previousvalueTime != currentDataPoint.valueTime)
	{
		switch(currentDataPoint.valueTime)
		{
		  case Time_VALUE0:		//δ����
			Alarm_Clock_Val = 0;
			//user handle
			break;
		  case Time_VALUE1:		//10S
			Alarm_Clock_Val = 10;
			//user handle
			break;
		  case Time_VALUE2:		//100S
			Alarm_Clock_Val = 100;
			//user handle
			break;
		  case Time_VALUE3:		//1000S
			Alarm_Clock_Val = 1000;
			//user handle
			break;
		  case Time_VALUE4:		//1����
			//user handle
			Alarm_Clock_Val = 60;
			break;
		  case Time_VALUE5:		//10����
			//user handle
			Alarm_Clock_Val = 60 * 10;
			break;
		  case Time_VALUE6:		//100����
			//user handle
			Alarm_Clock_Val = 60 * 100;
			break;
		  case Time_VALUE7:		//1Сʱ
			//user handle
			Alarm_Clock_Val = 60 * 60;
			break;
		  case Time_VALUE8:		//2Сʱ
			//user handle
			Alarm_Clock_Val = 60 * 60 * 2;
			break;
		  case Time_VALUE9:		//4Сʱ
			//user handle
			Alarm_Clock_Val = 60 * 60 * 4;
			break;
		  case Time_VALUE10:	//12Сʱ
			//user handle
			Alarm_Clock_Val = 60 * 60 * 12;
			break;
		  case Time_VALUE11:	//24Сʱ
			//user handle
			Alarm_Clock_Val = 60 * 60 * 24;
			break;
		  default:
			break;
		}
	}
	previousvalueTime = currentDataPoint.valueTime;
	
}
