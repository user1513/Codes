#include "main.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usart2.h"
#include "led.h"
#include "key.h"
#include "gps.h"
#include "dht11.h"
#include "hc595.h"
#include "mp3play.h"
#include "24cxx.h"
#include "ds3231.h"
#include "myiic.h"
#include "timer.h"
#include "myeeprom.h"

//0,不支持本地(支持GPS时间)校时
//1,支持本地校时
#define SYSTEM_SUPPORT_LOCAL		1		//定义系统文件夹是否支持本地时间校时

#if	SYSTEM_SUPPORT_LOCAL	//如SYSTEM_SUPPORT_LOCAL定义了,说明要支持本地校时
#else	//不支持本地时间校时，支持GPS时间校时
	
	
	
#endif

struct _System_Data_dev sys_clock_dev=
{
	17,		//年
	6,//月
	12,//日
	12,//时
	0,//分
	0,//秒
	1,//周
	17,//教学周
};


struct _gps_data_dev gps_clock_dev=
{
	0,		//GPS年
	0,		//GPS月
	0,		//GPS日
	0,		//GPS时
	0,		//GPS分
	0,		//GPS秒
};

u8 temperature;  	    
u8 humidity;   

u8 GPSReception;
#define ERROR 0X00
#define OK 0x01

u8 aoto_hour = 0x12;
u8 aoto_min = 0x01;
u8 aoto_sec = 0x30;

#define max_alarmnumber 50//最大闹铃个数
u8 Alarm_Data[max_alarmnumber][8];	//类别(0x01)，时，分，秒，铃声，重复 
									//类别(0x02)，时，分，秒，铃声，年，月，日
u8 nowalarmnumber = 0;				//当前闹钟个数 存储到EEPROM

u8 Usart1Data[15];
u8 Usart1ReceiveCounter = 0;
u8 receive_flag = 0;
u8 TIM3_flag = 0;

u16 sys_password;			//系统密码

u8 sys_addr = 0x03;			//系统地址区
u8 School_flag;				//校时标志位

void GPSSignalAnalysis()
{
	if(Data_legitimate==0)//无GPS信号时
	{
		GPSReception = ERROR ;
	}else
	{
		if(Data_legitimate|0x01)	//GPGGA处理
		{			
			if(GPS_lock==0)					//未定位
			{
				GPSReception = ERROR ;
			}
			else				//已定位
			{
				//处理时间
				gps_clock_dev.hour = (GPS_time[0] - 0x30) * 10 + (GPS_time[1] - 0x30);
				gps_clock_dev.min =  (GPS_time[2] - 0x30) * 10 + (GPS_time[3] - 0x30);
				gps_clock_dev.sec =  (GPS_time[4] - 0x30) * 10 + (GPS_time[5] - 0x30);
				
				//处理日期
				gps_clock_dev.year = (GPS_date[4] - 0x30) * 10 + (GPS_date[5] - 0x30);
				gps_clock_dev.month = (GPS_date[2] - 0x30) * 10 + (GPS_date[3] - 0x30);
				gps_clock_dev.day = (GPS_date[0] - 0x30) * 10 + (GPS_date[1] - 0x30);
				
				gps_clock_dev.hour += 8;//8个时区
				if(gps_clock_dev.hour >= 24) //一般校时时间要考虑时区，一般校时的时候不会执行这条语句
				{
					gps_clock_dev.day = gps_clock_dev.day + 1;
				}
				gps_clock_dev.hour %= 24;
				
				//---------------------------------------------------------------------
				if((gps_clock_dev.hour < 24)&&(gps_clock_dev.min < 60)&&(gps_clock_dev.sec < 60)&&(gps_clock_dev.month<13)&&(gps_clock_dev.day<32)) //容错处理//防止在校时那一瞬间出错
				{	
					GPSReception = OK ;//接收成功
				}else
				{
					GPSReception = ERROR ;//接收失败
				}
				//---------------------------------------------------------------------
			}			
			Data_legitimate &= ~0x01;
		}			
	}	
}
//有效性判断
#define Automatic	0
#define Manual		1
#define IS_AOTOTIME_MODE(MODE) 		(((Automatic) == 0) || ((Manual) == 1))
                                    
//waiting_ms：自动校时时间
//mode：模式选择，关系到错误代码
//delay_ms_dis：显示错误代码的时间
void Aototime_function(u16 waiting_ms ,u8 mode,u16 delay_ms_dis)
{
	u16 t;
	assert_param(IS_AOTOTIME_MODE(mode));//有效性判断
	t = waiting_ms * 100;//t*delay_us（n）的时间
	while(t--)//按下校时按钮后，一直接收GPS数据，成功则设置DS1302时钟数据，失败则继续循环， //等待500ms
	{
		GPSSignalAnalysis();//GPS时钟数据解析函数
		if(GPSReception == OK)//解析成功
		{
			//赋值时间
			sys_clock_dev.hour = gps_clock_dev.hour;
			sys_clock_dev.min = gps_clock_dev.min;
			sys_clock_dev.second = gps_clock_dev.sec;
			//赋值日期
			sys_clock_dev.year = gps_clock_dev.year;
			sys_clock_dev.month = gps_clock_dev.month;
			sys_clock_dev.day = gps_clock_dev.day;
			
			My_DS3231_SetTIME();//根据结构体初始化数据
			break;
		}//end if
		delay_us(10);
	}
	if(t == 0)
	{
		//显示错误代码,可以直接赋值min sec
		switch(mode)
		{
			case Automatic:
									{
										sys_clock_dev.hour = 0x99;
										sys_clock_dev.min = 0x01;
										HC595TransmitData();
										break;
									}
			case Manual:
									{
										sys_clock_dev.hour = 0x99;
										sys_clock_dev.min = 0x00;
										HC595TransmitData();
										break;
									}
			default:break;
		}
		//延迟显示错误代码时间 ----- 5S
		t = delay_ms_dis;
		while(t--)
		{
			delay_ms(1);
		}
	}		
}
void ReadAllAlarmData(void)
{
	u8 i,j;
	for(i=0; i<nowalarmnumber; i++)
	{
		for(j=0; j<8; j++)
		{
			Alarm_Data[i][j] = ReadAlarmData(j,i);
		}
	}
}

void MainStoreAllAlarm(void)
{
	u8 i,j;
	for(i=0; i<nowalarmnumber; i++)
	{
		for(j=0; j<8; j++)
		{
			StoreAlarmData(j,i,Alarm_Data[i][j]);
		}
	}
}

void MainStoreAlarm_Delete(u8 AlarmNo)
{
	u8 i,j;
	for(i=AlarmNo-1; i<nowalarmnumber; i++)
	{
		for(j=0; j<8; j++)
		{
			StoreAlarmData(j,i,Alarm_Data[i][j]);
		}
	}
}

void MainStoreAlarmData(u8 _alarmnumber)
{
	u8 i;
	for(i=0; i<8; i++)
	{
		Alarm_Data[_alarmnumber - 1][i] = ReadAlarmData(i,_alarmnumber - 1);
	}
}
u8 sysAlarmTimer = 0;

void DeleteAlarm(u8 AlarmNo)//AlarmNo：闹钟编号 从1开始
{
	int i,j;
	for(i=AlarmNo-1; i<nowalarmnumber; i++)
	{
		for(j=0; j<8; j++)
		{
			Alarm_Data[i][j] = Alarm_Data[i+1][j];	
		}
	}
	nowalarmnumber = nowalarmnumber - 1;
	MainStoreAlarm_Delete(AlarmNo);
	StorAlarmNumber(nowalarmnumber);
}

void USART1_SENDEnevt(u8 com)
{
	u8 i;
	u8 checknumber;
	u8 USARTData[6] = {0x55,0x00,0x00,0x00,0x00,0xAA};
	USARTData[1] = sys_addr;//地址
	USARTData[2] = com;//命令
	switch(com)
	{
		case 0x01:
			USARTData[3] = sys_addr;
			break;
		case 0x02:
			USARTData[3] = 0x01;
			break;
		case 0x03:
			USARTData[3] = 0x01;
			break;
		case 0x04://闹钟设置一
			USARTData[3] = 0x01;
			break;
		case 0x05://闹钟设置二
			USARTData[3] = 0x01;
			break;
		case 0x06://修改密码
			USARTData[3] = 0x01;
			break;
		case 0x07://试听闹铃
			USARTData[3] = 0x01;
			break;
		case 0x08://取消试听
			USARTData[3] = 0x01;	
			break;
		case 0x09://读取所有闹钟
			USARTData[3] = nowalarmnumber;
			break;
		case 0x0A://删除当前闹铃
			USARTData[3] = 0x01;
			break;
		case 0x0B://删除所有闹铃
			USARTData[3] = 0x01;
			break;
		case 0x0C://密码修改失败
			USARTData[3] = 0x01;
			break;
		case 0x0D://修改音量OK
			USARTData[3] = 0x01;
			break;
		default:
			break;
	}
	checknumber = 0x00;
	for(i=1;i<4;i++)
	{
		checknumber += USARTData[i];
	}
	USARTData[4] = checknumber;
	USARTData[5] = 0xAA;
	for(i=0;i<6;i++)
	{
		USART_SendData(USART1,USARTData[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	}
}

void USART1_SENDAlarmEvent(void)
{
	u8 i,j;
	u8 checknumber;
	u8 UsartData[13];
	UsartData[0] = 0x66;
	UsartData[1] = sys_addr;
	
	for(i=0;i<nowalarmnumber;i++)
	{
		UsartData[2] = i + 1;//从1开始
		for(j=0;j<8;j++)
		{
			UsartData[j + 3] = Alarm_Data[i][j];
		}
		checknumber = 0x00;
		for(j=1;j<11;j++)
		{
			checknumber += UsartData[j];
		}
		UsartData[11] = checknumber;
		UsartData[12] = 0x99;
		
		for(j=0;j<13;j++)
		{
			USART_SendData(USART1,UsartData[j]);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
		}
		delay_ms(sys_addr*25);//延迟一段时间，，，，不然接收会出现问题
	}
}

void USART1_Analysis(void)
{

	switch(Usart1Data[4])
	{
		case 0x01://读取设备地址
			USART1_SENDEnevt(0x01);
			break;
		case 0x02://手动设置时间
			sys_clock_dev.year = Usart1Data[5];
			sys_clock_dev.month = Usart1Data[6];
			sys_clock_dev.day = Usart1Data[7];
			sys_clock_dev.hour = Usart1Data[8];
			sys_clock_dev.min = Usart1Data[9];
			sys_clock_dev.second = Usart1Data[10];
			sys_clock_dev.week = Usart1Data[11];
			sys_clock_dev.tweek = Usart1Data[12];
			My_DS3231_SetRTC();
			USART1_SENDEnevt(0x02);
			//教学周
			break;
		case 0x03://自动设置时间
			sys_clock_dev.year = Usart1Data[5];
			sys_clock_dev.month = Usart1Data[6];
			sys_clock_dev.day = Usart1Data[7];
			sys_clock_dev.hour = Usart1Data[8];
			sys_clock_dev.min = Usart1Data[9];
			sys_clock_dev.second = Usart1Data[10];
			sys_clock_dev.week = Usart1Data[11];
			My_DS3231_SetRTC();
			USART1_SENDEnevt(0x03);
			break;
		case 0x04://闹钟设置一
			if(nowalarmnumber < Usart1Data[5])//0<1
			{
				nowalarmnumber = nowalarmnumber + 1;

				Alarm_Data[nowalarmnumber - 1][0] = 0x01;
				Alarm_Data[nowalarmnumber - 1][1] = Usart1Data[6] / 10 * 16 + Usart1Data[6] % 10; 
				Alarm_Data[nowalarmnumber - 1][2] = Usart1Data[7] / 10 * 16 + Usart1Data[7] % 10;
				Alarm_Data[nowalarmnumber - 1][3] = Usart1Data[8] / 10 * 16 + Usart1Data[8] % 10;
				Alarm_Data[nowalarmnumber - 1][4] = Usart1Data[9]; 
				Alarm_Data[nowalarmnumber - 1][5] = Usart1Data[10]; 
			}else
			{
				Alarm_Data[Usart1Data[5] - 1][0] = 0x01;
				Alarm_Data[Usart1Data[5] - 1][1] = Usart1Data[6] / 10 * 16 + Usart1Data[6] % 10; 
				Alarm_Data[Usart1Data[5] - 1][2] = Usart1Data[7] / 10 * 16 + Usart1Data[7] % 10;
				Alarm_Data[Usart1Data[5] - 1][3] = Usart1Data[8] / 10 * 16 + Usart1Data[8] % 10;
				Alarm_Data[Usart1Data[5] - 1][4] = Usart1Data[9]; 
				Alarm_Data[Usart1Data[5] - 1][5] = Usart1Data[10]; 
			}
			MainStoreAllAlarm();
			USART1_SENDEnevt(0x04);
			break;
		case 0x05://闹钟设置二
			if(nowalarmnumber < Usart1Data[5])//0<1
			{
				nowalarmnumber = nowalarmnumber + 1;
				Alarm_Data[nowalarmnumber - 1][0] = 0x02;
				Alarm_Data[nowalarmnumber - 1][1] = Usart1Data[6] / 10 * 16 + Usart1Data[6] % 10;
				Alarm_Data[nowalarmnumber - 1][2] = Usart1Data[7] / 10 * 16 + Usart1Data[7] % 10;
				Alarm_Data[nowalarmnumber - 1][3] = Usart1Data[8] / 10 * 16 + Usart1Data[8] % 10;
				Alarm_Data[nowalarmnumber - 1][4] = Usart1Data[9]; 
				Alarm_Data[nowalarmnumber - 1][5] = Usart1Data[10] / 10 * 16 + Usart1Data[10] % 10;
				Alarm_Data[nowalarmnumber - 1][6] = Usart1Data[11] / 10 * 16 + Usart1Data[11] % 10;
				Alarm_Data[nowalarmnumber - 1][7] = Usart1Data[12] / 10 * 16 + Usart1Data[12] % 10;
				
			}else
			{
				Alarm_Data[Usart1Data[5] - 1][0] = 0x02;
				Alarm_Data[Usart1Data[5] - 1][1] = Usart1Data[6] / 10 * 16 + Usart1Data[6] % 10;
				Alarm_Data[Usart1Data[5] - 1][2] = Usart1Data[7] / 10 * 16 + Usart1Data[7] % 10;
				Alarm_Data[Usart1Data[5] - 1][3] = Usart1Data[8] / 10 * 16 + Usart1Data[8] % 10;
				Alarm_Data[Usart1Data[5] - 1][4] = Usart1Data[9]; 
				Alarm_Data[Usart1Data[5] - 1][5] = Usart1Data[10] / 10 * 16 + Usart1Data[10] % 10;
				Alarm_Data[Usart1Data[5] - 1][6] = Usart1Data[11] / 10 * 16 + Usart1Data[11] % 10;
				Alarm_Data[Usart1Data[5] - 1][7] = Usart1Data[12] / 10 * 16 + Usart1Data[12] % 10;
			}
			MainStoreAllAlarm();
			USART1_SENDEnevt(0x05);
			break;
		case 0x06://修改密码
			sys_password = Usart1Data[5]<<8;
			sys_password |= Usart1Data[6];
			StorePassword(sys_password);//存储密码
			USART1_SENDEnevt(0x06);
			break;
		case 0x07://试听闹铃
			MP3_Play(Usart1Data[5]);
			USART1_SENDEnevt(0x07);
			break;
		case 0x08://取消试听
			MP3_Stop();
			USART1_SENDEnevt(0x08);
			break;
		case 0x09://读取所有闹铃
			USART1_SENDEnevt(0x09);
			USART1_SENDAlarmEvent();
			break;
		case 0x0A://删除当前闹铃
			DeleteAlarm(Usart1Data[5]);
			USART1_SENDEnevt(0x0A);
			break;
		case 0x0B://删除所有闹铃
			nowalarmnumber = 0;
			StorAlarmNumber(nowalarmnumber);
			USART1_SENDEnevt(0x0B);
			break;
		case 0x0C://调节音量
			MP3_SetVolume(Usart1Data[5]);
			USART1_SENDEnevt(0x0C+0x01);			
			break;
	}
}	


void AlarmClockEvent(void)
{
	u8 i;
	for(i=0; i<nowalarmnumber; i++)
	{
		if(Alarm_Data[i][0] == 0x01)//时分秒铃声重复
		{
			if(((Alarm_Data[i][5] >> (sys_clock_dev.week - 1)) & 0x01) == 1)//周正确
			{
				if(sys_clock_dev.hour == Alarm_Data[i][1])
				{
					if(sys_clock_dev.min == Alarm_Data[i][2])
					{
						if(sys_clock_dev.second == Alarm_Data[i][3])
						{
							//闹铃正确
							MP3_Play(Alarm_Data[i][4]);
							sysAlarmTimer = 30;
						}else
						{
							continue;
						}
					}else
					{
						continue;
					}
				}else
				{
					continue;
				}
			}else if(Alarm_Data[i][5] == 0x80)//代表一次，如果周不正确，判断是否没有周，这样就只响铃一次
			{
				if(sys_clock_dev.hour == Alarm_Data[i][1])
				{
					if(sys_clock_dev.min == Alarm_Data[i][2])
					{
						if(sys_clock_dev.second == Alarm_Data[i][3])
						{
							//闹铃正确
							MP3_Play(Alarm_Data[i][4]);
							DeleteAlarm(i + 1);//删除闹钟
							sysAlarmTimer = 30;
						}else
						{
							continue;
						}
					}else
					{
						continue;
					}
				}else
				{
					continue;
				}
			}else
			{
				continue;
			}
		}else if(Alarm_Data[i][0] == 0x02)
		{
			if((sys_clock_dev.year == Alarm_Data[i][5])&&(sys_clock_dev.month == Alarm_Data[i][6])&&(sys_clock_dev.day == Alarm_Data[i][7]))
			{
				if(sys_clock_dev.hour == Alarm_Data[i][1])
				{
					if(sys_clock_dev.min == Alarm_Data[i][2])
					{
						if(sys_clock_dev.second == Alarm_Data[i][3])
						{
							//闹铃正确
							MP3_Play(Alarm_Data[i][4]);
							DeleteAlarm(i + 1);//删除当前闹钟
							sysAlarmTimer = 30;
						}else
						{
							continue;//跳出当前循环
						}
					}else
					{
						continue;
					}
				}else
				{
					continue;
				}
			}else
			{
				continue;
			}
		}
	}
}



void TIM3_Event(void)
{
	
	if(sysAlarmTimer != 0)
	{
		sysAlarmTimer--;
		if(sysAlarmTimer == 0)
		{
			MP3_Stop();
		}
	}
}

void SetSSchoolTimer(u8 hour,u8 min,u8 sec)
{
	#if	SYSTEM_SUPPORT_LOCAL	//如SYSTEM_SUPPORT_LOCAL定义了,说明要支持本地校时
		aoto_hour = hour;
		aoto_min = min;
		aoto_sec = sec;
	#else	//不支持本地时间校时，支持GPS时间校时
		aoto_hour = hour / 16 * 10 + hour % 16;
		aoto_min = min / 16  * 10 + min % 16;
		aoto_sec = sec / 16 * 10 + sec % 16;	
	#endif

}

int main(void)
{	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭JTAG，使能SWD，可以用SWD模式调试,节约资源
	delay_init();
	NVIC_Configuration();		// 设置中断优先级分组
	
	uart_init(9600);	 	//串口初始化为9600
	USART2_Init(9600);	 	//串口2初始化
	MP3_Init(9600);
	
	LED_Init();				//LED初始化 
	DHT11_Init();	//DHT11初始化	
	HC595_Init();
	
	DS3231_Init();
	AT24CXX_Init();
	
	if(ReadPassword() > 9999)//这个条件代表是新设备
	{
		StorePassword(0);
		My_DS3231_SetRTC();//根据结构体初始化数据
	}
//	My_DS3231_SetRTC();//根据结构体初始化数据	
	nowalarmnumber = ReadAlarmNumber();
	sys_password = ReadPassword();
	
	ReadAllAlarmData();
	
	SetSSchoolTimer(aoto_hour,aoto_min,aoto_sec);//根据宏定义选择是否转化

//	MP3_SetVolume(4);
	
	TIM3_Int_Init(9999,7199);//10Khz的计数频率，计数到1000为100ms     

	while(1)
	{
		HC595TransmitData();
		My_DS3231_ReadRTC();
		
		LED = sys_clock_dev.second % 2;//运行指示灯
		DP = sys_clock_dev.second % 2;//运行指示灯
		
		DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值
		
		#if	SYSTEM_SUPPORT_LOCAL	//如SYSTEM_SUPPORT_LOCAL定义了,说明要支持本地校时
		//到达自动校时的时间。进行5s自校时
		if((sys_clock_dev.hour == aoto_hour)&&(sys_clock_dev.min == aoto_min)&&(sys_clock_dev.second == aoto_sec))
		{
			School_flag = 1;
		}
		#endif
		if(School_flag == 1)
		{
			School_flag = 0;
			Aototime_function(500,Automatic,5000);//校时500ms，
		}
		//定时器事件
		if(TIM3_flag == 1)
		{
			TIM3_flag = 0;
			TIM3_Event();
		}
		//闹钟事件
		AlarmClockEvent();
		//串口1服务函数
		if(receive_flag == 1)
		{
			receive_flag = 0;
			delay_ms(sys_addr*25);//延迟一段时间，，，，不然接收会出现问题
			USART1_Analysis();//串口1判断
		}
		
		//------------------------按键扫描函数----------
		switch(KEY_Scan(0))
		{
			case S1_PRES:
						{
							Aototime_function(500,Manual,5000);
							break;//校时
						}
			case S2_PRES:
				
				break;
			default:break;
		}
	}	
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	u8 i;//计算累加局部变量
	u8 checknumber;//累加校验局部变量
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		Usart1Data[Usart1ReceiveCounter] = Res;
		
		switch(Usart1ReceiveCounter)
		{
			case 0:
				if(Usart1Data[Usart1ReceiveCounter] == 0x55)//包头
				{
					Usart1ReceiveCounter ++;
				}else 
				{
					Usart1ReceiveCounter = 0;
				}
			break;
			case 1:
				if((Usart1Data[Usart1ReceiveCounter] == 0x00) || (Usart1Data[Usart1ReceiveCounter] == sys_addr))//地址
				{
					
					Usart1ReceiveCounter ++;
				}else 
				{
					Usart1ReceiveCounter = 0;
				}
				break;
			case 2:
				if(Usart1Data[Usart1ReceiveCounter] == (sys_password >> 8))// 密码高位
				{
					
					Usart1ReceiveCounter ++;
					
				}else 
				{
					Usart1ReceiveCounter = 0;
					USART1_SENDEnevt(0x0C);
				}
				break;
				
			case 3:
				if(Usart1Data[Usart1ReceiveCounter] == (sys_password & 0x00FF))// 密码低位
				{
					Usart1ReceiveCounter ++;
					
				}else 
				{
					Usart1ReceiveCounter = 0;
					USART1_SENDEnevt(0x0C);
				}
				break;
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
				Usart1ReceiveCounter ++;
				break;
			case 13:
				checknumber = 0x00;
				 for (i = 1; i < 13; i++)
				{
					checknumber += Usart1Data[i];
				}
				if(Usart1Data[Usart1ReceiveCounter] == checknumber)
				{
					Usart1ReceiveCounter ++;
					
				}else 
				{
					Usart1ReceiveCounter = 0;
				}
				break;
			case 14:
			if(Usart1Data[Usart1ReceiveCounter] == 0xAA)
			{
				receive_flag = 1;//执行串口判断
			}
			Usart1ReceiveCounter = 0;
			break;	
		}
		

	} 
}




void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res = USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据
		switch(Res)
		{
			case '$':
				GPSDATA_number=0;		//GPS数据类型清空
				GPSDATA_mode=1;			//接收命令模式
				GPSDATA_count=0;		//接收位数清空
				break;
			
			case ',':
				GPSDOT_count++;		//逗号计数加1
				GPSDATA_count=0;
				break;
			
			case '*':
				switch(GPSDATA_number)
				{
					case 1:
						Data_legitimate|=0x81;//GPGGA
						break;
					case 2:
						Data_legitimate|=0x82;//GPGSV
						break;
					case 3:
						Data_legitimate|=0x84;//GPRMC
						break;
				}
				GPSDATA_mode=0;
				break;
				
			default:
				if(GPSDATA_mode==1)													//类型种类判断
				{
					GPSDATA_cmd[GPSDATA_count]=Res;			//接收字符放入类型缓存
					if(GPSDATA_count==4)											//如果类型数据接收完毕，判断类型
					{											
						if(GPSDATA_cmd[0]=='G'&&GPSDATA_cmd[1]=='P'&&GPSDATA_cmd[2]=='G'&&GPSDATA_cmd[3]=='G'&&GPSDATA_cmd[4]=='A')//判断GPGGA
						{
							GPSDATA_number=1;
							GPSDATA_mode=2;
							GPSDOT_count=0;
							GPSDATA_count=0;
						}
						
						if(GPSDATA_cmd[0]=='G'&&GPSDATA_cmd[1]=='P'&&GPSDATA_cmd[2]=='G'&&GPSDATA_cmd[3]=='S'&&GPSDATA_cmd[4]=='V')//判断GPGSV
						{
							GPSDATA_number=2;
							GPSDATA_mode=2;
							GPSDOT_count=0;
							GPSDATA_count=0;
						}
						
						if(GPSDATA_cmd[0]=='G'&&GPSDATA_cmd[1]=='P'&&GPSDATA_cmd[2]=='R'&&GPSDATA_cmd[3]=='M'&&GPSDATA_cmd[4]=='C')//判断GPRMC
						{
							GPSDATA_number=3;
							GPSDATA_mode=2;
							GPSDOT_count=0;
							GPSDATA_count=0;
						}
					}
				}
				else if(GPSDATA_mode==2)					//接收数据处理
				{
					switch (GPSDATA_number)
					{
						case 1:												//类型1数据接收。GPGGA
							switch(GPSDOT_count)
							{
								case 2:										//纬度处理
									if(GPSDATA_count<9)
										GPS_latitude[GPSDATA_count]=Res;
									break;
								case 3:										//纬度方向处理
									if(GPSDATA_count<1)
										GPS_latitude_dir=Res;
									break;
								case 4:										//经度处理
									if(GPSDATA_count<10)
										GPS_longitude[GPSDATA_count]=Res;
									break;
								case 5:										//经度方向处理
									if(GPSDATA_count<1)
										GPS_longitude_dir=Res;
									break;
								case 6:										//定位判断
									if(GPSDATA_count<1)
										GPS_lock=Res;
									break;
								case 7:										//GPS使用卫星个数
									if(GPSDATA_count<2)
										GPSUSE_satellite[GPSDATA_count]=Res;
									break;
								case 9:										//海拔高度处理
									if(GPSDATA_count<6)
										GPS_altitude[GPSDATA_count]=Res;
									break;
							}
							break;
							
						case 2:												//类型2数据接收。GPGSV
							switch(GPSDOT_count)
							{
								case 3:										//GPS可见卫星个数
									if(GPSDATA_count<2)
										GPSVISIBLE_satellite[GPSDATA_count]=Res;
									break;
							}
						break;
							
						case 3:												//类型3数据接收。GPRMC
							switch(GPSDOT_count)
							{
								case 1:										//时间处理
									if(GPSDATA_count<6)		
										GPS_time[GPSDATA_count]=Res;	
									break;
								case 2:										//定位判断						
									if(GPSDATA_count<1)
									{
									  if(Res=='A')
											GPS_lock=1;
									  if(Res=='V')
									    GPS_lock=0;
									}
									break;
								case 3:										//纬度处理						
									if(GPSDATA_count<9)
									{
										GPS_latitude[GPSDATA_count]=Res;
									}
									break;
								case 4:										//纬度方向处理						
									if(GPSDATA_count<1)
									{
										GPS_latitude_dir=Res;
									}
									break;
								case 5:										//经度处理						
									if(GPSDATA_count<10)
									{
										GPS_longitude[GPSDATA_count]=Res;
									}
									break;
								case 6:										//经度方向处理						
									if(GPSDATA_count<1)
									{
										GPS_longitude_dir=Res;
									}
									break;
								case 7:										//速度处理						
									if(GPSDATA_count<5)
									{
										GPS_speed[GPSDATA_count]=Res;
									}
									break;
								case 8:										//方位角处理						
									if(GPSDATA_count<5)
									{
										GPS_Angle[GPSDATA_count]=Res;
									}
									break;
								case 9:										//日期				
									if(GPSDATA_count<6)
									{
										GPS_date[GPSDATA_count]=Res;
										#if	SYSTEM_SUPPORT_LOCAL	//如SYSTEM_SUPPORT_LOCAL定义了,说明要支持本地校时
										#else	//不支持本地时间校时，支持GPS时间校时
										gps_clock_dev.hour = (GPS_time[0] - 0x30) * 10 + (GPS_time[1] - 0x30);
										gps_clock_dev.min =  (GPS_time[2] - 0x30) * 10 + (GPS_time[3] - 0x30);
										gps_clock_dev.sec =  (GPS_time[4] - 0x30) * 10 + (GPS_time[5] - 0x30);
										gps_clock_dev.hour += 8;//8个时区
										gps_clock_dev.hour %= 24;										
										if(gps_clock_dev.hour == aoto_hour)
										{
											if(gps_clock_dev.min == aoto_min)
											{
												if(gps_clock_dev.sec == aoto_sec)
												{
													School_flag = 1;
												}
											}
										}
										#endif
									}
									break;
							}
							break;
					}
				}
				GPSDATA_count++;		//接收数位加1
				break;
		}
	}
}


//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM3_flag = 1;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
	}
}

