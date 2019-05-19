#include <REGX51.H>
#include <MAX6921.h>
#include <Ds12c887.h>
#include <GPS.h>
#include <UART.h>
#include <intrins.h>
#ifndef uchar
#define uchar unsigned char 
#endif

#ifndef uint
#define uint unsigned int 
#endif

extern unsigned char TIME[7];

/*******************************************************************************
* 函数名         : IrInit()
* 函数功能		   : 初始化红外线接收
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
//sbit IRIN = P3^3;
//void IrInit()
//{
//	IT0=1;//下降沿触发
//	EX0=1;//打开中断0允许
//	EA=1;	//打开总中断

//	IRIN=1;//初始化端口
//}

void init()
{
	//初始化中断频闭
/*
	EA=1;
	EX0=1;
	IT0=1;
*/
	Uart_Init();//串口初始化函數
	
	write_ds12c887(0x0B,0x2e);//SET PIE AIE UIE SWQE DM 24/12 DSE
	read_ds12c887(0x0c);
	
//	SetTime(13,35,50);				//初始化时间
//	SetDate(16,10,9);				//初始化日期
//	SetDay(7);							//初始化星期
//	set_alarm(21,26,00);			//初始化闹钟

	write_ds12c887(0x0a,0x20);  
	
}

#define CorrectHour 12
#define CorrectMin  01

sbit KEY1 = P2^1;
sbit KEY2 = P2^2;


/*******************************************************************************
* 函数名          : Delay10ms
* 函数功能		     : 延时函数，延时10ms
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void Delay10ms(void)		
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 108;
	j = 144;
	do
	{
		while (--j);
	} while (--i);
}


unsigned char KeyScan(void)
{
	unsigned char keyValue = 0 ; //保存键值
	//--检测按键1--//
	if (KEY1==0)		//检测按键K1是否按下
	{
		Delay10ms();	//消除抖动
		if (KEY1==0)	//再次检测按键是否按下
		{
			keyValue = 1;
			while(!KEY1);
		}
	}
	//--检测按键2--//
	if (KEY2==0)		//检测按键K2是否按下
	{
		Delay10ms();	//消除抖动
		if (KEY2==0)	//再次检测按键是否按下
		{
			keyValue = 2;
			while(!KEY2);
		}
	}
	return keyValue;
}


void main()
{
	uchar hour,min,sec;
	
	uchar xdata gps_hour,gps_min,gps_sec;
	
	uchar nolocknumber = 0;//未定义标识符

	
	uchar jieshouok = 0;
	
	init();										//初始化函数
	while(1)
	{
		sec=read_ds12c887(0);	
		min=read_ds12c887(2);
		hour=read_ds12c887(4);
		display_num(8,hour/10);
		display_num(7,hour%10);
		display_num(5,min/10);
		display_num(4,min%10);
		display_num(2,sec/10);
		display_num(1,sec%10);
		
		//------------------------定时校时函数-----------
		//判断是否校时

		if((hour==CorrectHour)&&(min==CorrectMin))
		{
			if(jieshouok == 1)
			{
				SetTime(gps_hour,gps_min,gps_sec);//写入--校时
			}
		}
	
		//------------------------校时函数-------------
		if(Data_legitimate==0)				//无GPS信号时
		{
			jieshouok = 0;
		}
		else	//有GPS信号
		{		
			if(Data_legitimate|0x01)	//GPGGA处理
			{			
				if(GPS_lock==0)					//未定位
				{
//					//每次会错误的出现未定位	
					if(++nolocknumber == 250)
					{
						jieshouok = 0;
					}
				}
				else							//已定位
				{
					nolocknumber = 0;//已定位，清楚未定位次数
					//处理时间
					gps_hour = (GPS_time[0] - 0x30) * 10 + (GPS_time[1] - 0x30);
					gps_min =  (GPS_time[2] - 0x30) * 10 + (GPS_time[3] - 0x30);
					gps_sec =  (GPS_time[4] - 0x30) * 10 + (GPS_time[5] - 0x30);
					
					gps_hour += 8;
					gps_hour %= 24;
					
					//---------------------------------------------------------------------
					if((gps_hour<24)&&(gps_min<60)&&(gps_sec<60)) //容错处理//防止在校时那一瞬间出错
					{	
						jieshouok = 1;
					}else
					{
						jieshouok = 0;
					}
					//---------------------------------------------------------------------
	
				}			
				Data_legitimate&=~0x01;
			}	
		}
		//------------------------按键扫描函数----------
		switch(KeyScan())
		{
			case 1:if(jieshouok==1)SetTime(gps_hour,gps_min,gps_sec);break;//校时
			case 2:SetTime(12,00,50);break;
			default:break;
		}
	}
}


/*****************************************************************************
串口中断
/****************************************************************************/
void uart(void) interrupt 4
{
	uchar UART_data;
	if(RI)
	{
		UART_data=SBUF;
		
		switch(UART_data)
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
					GPSDATA_cmd[GPSDATA_count]=UART_data;			//接收字符放入类型缓存
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
						
						/****************
						....
						其他类型照搬
						****************/
						
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
										GPS_latitude[GPSDATA_count]=UART_data;
									break;
								case 3:										//纬度方向处理
									if(GPSDATA_count<1)
										GPS_latitude_dir=UART_data;
									break;
								case 4:										//经度处理
									if(GPSDATA_count<10)
										GPS_longitude[GPSDATA_count]=UART_data;
									break;
								case 5:										//经度方向处理
									if(GPSDATA_count<1)
										GPS_longitude_dir=UART_data;
									break;
								case 6:										//定位判断
									if(GPSDATA_count<1)
										GPS_lock=UART_data;
									break;
								case 7:										//GPS使用卫星个数
									if(GPSDATA_count<2)
										GPSUSE_satellite[GPSDATA_count]=UART_data;
									break;
								case 9:										//海拔高度处理
									if(GPSDATA_count<6)
										GPS_altitude[GPSDATA_count]=UART_data;
									break;
							}
							break;
							
						case 2:												//类型2数据接收。GPGSV
							switch(GPSDOT_count)
							{
								case 3:										//GPS可见卫星个数
									if(GPSDATA_count<2)
										GPSVISIBLE_satellite[GPSDATA_count]=UART_data;
									break;
							}
						break;
							
						case 3:												//类型3数据接收。GPRMC
							switch(GPSDOT_count)
							{
								case 1:										//时间处理
									if(GPSDATA_count<6)		
										GPS_time[GPSDATA_count]=UART_data;	
									break;
								case 2:										//定位判断						
									if(GPSDATA_count<1)
									{
									  if(UART_data=='A')
											GPS_lock=1;
									  if(UART_data=='V')
									    GPS_lock=0;
									}
									break;
								case 3:										//纬度处理						
									if(GPSDATA_count<9)
									{
										GPS_latitude[GPSDATA_count]=UART_data;
									}
									break;
								case 4:										//纬度方向处理						
									if(GPSDATA_count<1)
									{
										GPS_latitude_dir=UART_data;
									}
									break;
								case 5:										//经度处理						
									if(GPSDATA_count<10)
									{
										GPS_longitude[GPSDATA_count]=UART_data;
									}
									break;
								case 6:										//经度方向处理						
									if(GPSDATA_count<1)
									{
										GPS_longitude_dir=UART_data;
									}
									break;
								case 7:										//速度处理						
									if(GPSDATA_count<5)
									{
										GPS_speed[GPSDATA_count]=UART_data;
									}
									break;
								case 8:										//方位角处理						
									if(GPSDATA_count<5)
									{
										GPS_Angle[GPSDATA_count]=UART_data;
									}
									break;
								case 9:										//日期				
									if(GPSDATA_count<6)
									{
										GPS_date[GPSDATA_count]=UART_data;
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
	RI=0;
}