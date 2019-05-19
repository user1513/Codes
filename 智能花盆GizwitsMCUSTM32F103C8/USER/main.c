#include "main.h"



void Usart2_Init(u32 bound);
void Usart2_Send_Byte(u8 data);
void Usart2_String_Send(u8 *data);
void Usart2_String_Send1(u8 *data,u8 length);
/************************************************  

************************************************/   
/* �û�����ǰ�豸״̬�ṹ��*/
dataPoint_t currentDataPoint;

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

bool GetTimeFlag = FALSE;
bool GetUserFlag = FALSE;

 int main(void)
 {	 
	int key  = 0 ;
//	_calendar_obj time = {2018,11,1,4,12,40,30};
//	u8 timedata[50]; 
//	u8 FixedTimeTemp[3] = {0,0,0};
	u8 wifi_con=0;//��¼wifi����״̬ 1:���� 0:�Ͽ�
	delay_init();	    	 //��ʱ������ʼ��	
	KEY_Init();	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	//RTC_Init(&time);
	Usart2_Init(9600);
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	//Lsens_Init(); 			//��ʼ������������   
	Gizwits_Init();         //Э���ʼ��
//		printf("WIFI��λ����������������\r\n");
//		gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ 
//		wifi_sta=0;//��־wifi�ѶϿ�
	FuncReg(&GetTimeFlag, 500, TRUE);
	FuncReg(&GetUserFlag, 100, TRUE);
	while(1)
	{
		if(array[8] == 0xFF)
		{
			array[8] = 0X00;
			currentDataPoint.valueEnvironmentalScienceHum =  array[5];
			currentDataPoint.valueEnvironmentalScienceTemp =  array[4];
			currentDataPoint.valuelight =  array[2]*256 + array[3];
			currentDataPoint.valuesoilhum =  array[6];
		}
		if( TRUE == GetUserFlag)
		{
			GetUserFlag = FALSE;
			FuncReg(&GetUserFlag, 100, TRUE);	
			if(wifi_con!=wifi_sta)
			 {
				 wifi_con=wifi_sta;
				 wifi_con?printf("connect"):printf("close");
			 }
			currentDataPoint.valuesignal = gizwitsProtocol.wifiStatusData.rssi;
			if(currentDataPoint.valuemotor == 0x01)
			{
					motor = 0;
			}else
			{
					motor = 1;
			}
			gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��
		}
//		if( TRUE == GetTimeFlag)
//		{
//			GetTimeFlag = FALSE;
//			FuncReg(&GetTimeFlag, 1000, TRUE);
//			sprintf((char *)timedata, "%d��%d��%d������:%d %dʱ%d��%d��" , calendar.w_year, calendar.w_month, calendar.w_date, calendar.week, calendar.hour, calendar.min, calendar.sec);
//			Usart2_String_Send(timedata);
//			//			gizwitsGetNTP();
////			sprintf((char *)timedata,
////				"%d��%d��%d��--%dʱ%d��%d��",
////				gizwitsProtocol.TimeNTP.year,
////				gizwitsProtocol.TimeNTP.month,
////				gizwitsProtocol.TimeNTP.day,
////				gizwitsProtocol.TimeNTP.hour,
////				gizwitsProtocol.TimeNTP.minute,
////				gizwitsProtocol.TimeNTP.second);
////			Usart2_String_Send(timedata);
//			
//			//FixedTime(FixedTimeTemp, TRUE);
//		}
		
		
		
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
 



 
