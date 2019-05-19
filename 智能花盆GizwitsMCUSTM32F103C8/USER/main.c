#include "main.h"



void Usart2_Init(u32 bound);
void Usart2_Send_Byte(u8 data);
void Usart2_String_Send(u8 *data);
void Usart2_String_Send1(u8 *data,u8 length);
/************************************************  

************************************************/   
/* 用户区当前设备状态结构体*/
dataPoint_t currentDataPoint;

//WIFI连接状态
//wifi_sta 0: 断开
//         1: 已连接
u8 wifi_sta=0;

//协议初始化
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MS系统定时
    usart3_init(9600);//WIFI初始化
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
}

bool GetTimeFlag = FALSE;
bool GetUserFlag = FALSE;

 int main(void)
 {	 
	int key  = 0 ;
//	_calendar_obj time = {2018,11,1,4,12,40,30};
//	u8 timedata[50]; 
//	u8 FixedTimeTemp[3] = {0,0,0};
	u8 wifi_con=0;//记录wifi连接状态 1:连接 0:断开
	delay_init();	    	 //延时函数初始化	
	KEY_Init();	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	//RTC_Init(&time);
	Usart2_Init(9600);
	LED_Init();		  		//初始化与LED连接的硬件接口
	//Lsens_Init(); 			//初始化光敏传感器   
	Gizwits_Init();         //协议初始化
//		printf("WIFI复位，请重新配置连接\r\n");
//		gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位 
//		wifi_sta=0;//标志wifi已断开
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
			gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
		}
//		if( TRUE == GetTimeFlag)
//		{
//			GetTimeFlag = FALSE;
//			FuncReg(&GetTimeFlag, 1000, TRUE);
//			sprintf((char *)timedata, "%d年%d月%d日星期:%d %d时%d分%d秒" , calendar.w_year, calendar.w_month, calendar.w_date, calendar.week, calendar.hour, calendar.min, calendar.sec);
//			Usart2_String_Send(timedata);
//			//			gizwitsGetNTP();
////			sprintf((char *)timedata,
////				"%d年%d月%d日--%d时%d分%d秒",
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
//		if(key==KEY1_PRES)//KEY1按键
//		{
//			printf("WIFI进入AirLink连接模式\r\n");
//			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
//		}			
//		if(key == KEY1_PRES)//KEY_UP按键
//		{  
//			printf("WIFI复位，请重新配置连接\r\n");
//			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位 
//			wifi_sta=0;//标志wifi已断开
//		}

	}
}
 



 
