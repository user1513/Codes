#include "main.h"

#define YEARDISPLAYTIME 2000 //年月日显示时间
#define DATEDISPLAYTIME 5000 //时分秒显示时间
#define SETFLICKERTIME  500  //设置闪烁频率
#define KEYVALTIME       10  //键值获取时间
#define TicktockTIME     1000  //滴答时间

int SetTime[7]={40,50,16,7,23,11,18};//秒-分-时/日期/日-月-年
int GetTime[7];//秒-分-时/日期/日-月-年

bool FIRMWAREDISPLAYFLAG = TRUE; //辉光管显示标志位
bool DS3231GETTIMEFLAG = TRUE;   //DS3231获取时间标志位
bool YEARDISFLAG = TRUE;         //日期与时间显示比例标志位
bool YEARDISTEMPFLAG = TRUE;     //辅助日期与时间显示比例标志位
bool SETFLICKERFLAG = TRUE;      //设置闪烁标志位
bool KEYGETFLAG = TRUE;     	 //键值标志位
bool TICKTOCKFLAG = TRUE;		 //滴答标志位
int main(void)
{
	gps_process_data gps;     //GPS结构体
	unsigned char GPS_Data_Temp[256];
	u8 GpsKeyFlag = 0, tempFlag = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	gps_io_cfg();
	key_io_config();
	delay_init();
	LED_Init();
	TIM4_Ctrl_Init(72-1,1000-1);
	IIC_Init();
	Usart2_Init(38400);
	//Ds3231_Init();
	//DS3231_SetTime(SetTime);
	FirmwarePeriphCfg();
	while(1)
	{
		if(KEYGETFLAG == TRUE)
		{
			Get_Key_Val(GetTime);
			KEYGETFLAG = FALSE;
			FuncReg(&KEYGETFLAG, KEYVALTIME, TRUE);
		}
		if(YEARDISFLAG == TRUE && YEARDISTEMPFLAG == TRUE) // 控制辉光管显示日期和时间的比例，可以通过修改main.c上部宏定义
		{
			YEARDISTEMPFLAG = FALSE;
			FuncReg(&YEARDISFLAG, YEARDISPLAYTIME, FALSE);
		}
		else if(YEARDISFLAG == FALSE && YEARDISTEMPFLAG == FALSE)
		{
			YEARDISTEMPFLAG = TRUE;
			FuncReg(&YEARDISFLAG, DATEDISPLAYTIME, TRUE);
		}
		if(FIRMWAREDISPLAYFLAG == TRUE && keystate[0] == 0)                   // 每隔1ms调用一下辉光显示函数
		{
			FIRMWAREDISPLAYFLAG = FALSE;
			FuncReg(&FIRMWAREDISPLAYFLAG, 1, TRUE);
			if(YEARDISFLAG == TRUE)						  // 控制显示日期与时间显示比例
				displayTime(GetTime + 4, 4);
			else
				displayTime(GetTime, 4);
		}
		if(DS3231GETTIMEFLAG == TRUE && keystate[0] == 0)                    //每隔500ms调用一下DS3231获取时间的函数
		{
			DS3231GETTIMEFLAG = FALSE;
			FuncReg(&DS3231GETTIMEFLAG, 500, TRUE);
			DS3231_GetTime(GetTime);
		}
		if(SETFLICKERFLAG == TRUE)
		{
			SETFLICKERFLAG = FALSE;
			tempFlag = ~tempFlag;
			FuncReg(&SETFLICKERFLAG , SETFLICKERTIME , TRUE);
		}
		if(keystate[0] == 0x01 )
		{	
			if(tempFlag == 0x00)
			{
				FirmwareShift(keystate[1], GetTime, TRUE);
			}
			else
			{
				FirmwareShift(keystate[1], GetTime, FALSE);
			}
		}
		if(TICKTOCKFLAG == TRUE && keystate[0] == 0)
		{
			TICKTOCKFLAG = FALSE;
			DIDT = ~DIDT;
			FuncReg(&TICKTOCKFLAG , TicktockTIME , TRUE);
		}
		if(GPSKEY == 0 && GpsKeyFlag != 0 )
		{
			GPSENABLE();
			if(Gps_Data_writ(&gps) == TRUE)
			{
				GpsKeyFlag = 0;
			}
			if(GPS_Stop_flag==1)                    //获取GPS数据
			{
				GPS_Stop_flag=0;
				strcpy((char *) GPS_Data_Temp,(const char *)GPS_Uart2_Data);
				memset(GPS_Uart2_Data,0,sizeof(unsigned char) *256);
				GPS_Analysis(&gps,(u8*)GPS_Data_Temp);//分析字符串			
			}
		}
		else if (GPSKEY != 0) {GpsKeyFlag = 0xff; GPSDISENABLE();memset((uint8_t *)&gps.utc, 0,sizeof(gps_utc_time));}
	}
}
//定时器5中断服务程序

u16 Time500Ms = 0;
void TIM4_IRQHandler(void)   //TIM5中断
{
	if(TIM4->SR&0X0001)//20ms定时中断
 {
	 if((Time500Ms++)==500)
	 {
		 Time500Ms=0;
		 led=~led; 
	 }
	 FuncTimeTravers();
 }
 TIM4->SR&=~(1<<0);  //中断标志位清0，等待下一次中断
}
