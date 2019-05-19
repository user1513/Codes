#include "ds3231.h" 
#include "myiic.h"
#include "delay.h"
#include "main.h"

u8 rtc_address[7]={0x00,0x01,0x02,0x04,0x05,0x03,0x06}; //秒分时日月周年 最低位读写位




void DS3231_Init(void)
{
	IIC_Init();
	DS3231_WriteOneByte(0x0e,0x0c);
}


void DS3231_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{ 	    																 
	IIC_Start();									//产生起始信号
	IIC_Send_Byte(0XD0);					//写入设备地址（写）
	IIC_Wait_Ack();								//等待设备的应答
	IIC_Send_Byte(WriteAddr);			//写入要操作的单元地址
	IIC_Wait_Ack();		 						//等待设备的应答
  IIC_Send_Byte(DataToWrite); 	//写入数据
	IIC_Wait_Ack();	    					//等待设备的应答
  IIC_Stop();										//产生一个停止信号
}


u8 DS3231_ReadOneByte(u8 ReadAddr)
{
	u8 temp=0;		  	    																 
	IIC_Start();									//产生起始信号
	IIC_Send_Byte(0XD0);					//写入设备地址（写）
	IIC_Wait_Ack();								//等待设备的应答
  IIC_Send_Byte(ReadAddr);   		//写入要操作的单元地址
	IIC_Wait_Ack();	    					//等待设备的应答
  IIC_Stop();										//产生一个停止信号				

	IIC_Start();									//产生起始信号
	IIC_Send_Byte(0XD1);					//写入设备地址（读）
	IIC_Wait_Ack();								//等待设备的应答
	temp=IIC_Read_Byte(0);				//写入数据
  IIC_Stop();										//产生一个停止条件	    
	return temp;
}


void DS3231_SetRTC(u8 Rtc_data[]) 
{
	u8 i;
	for(i=0; i<7; i++)
	{
		Rtc_data[i] = Rtc_data[i] / 10 * 16 + Rtc_data[i] % 10 ;//BCD处理
		DS3231_WriteOneByte(rtc_address[i],Rtc_data[i]);
	}
//	DS3231_WriteOneByte(0x0e,0x0c);
}


void DS3231_ReadRTC(u8 Rtc_data[])
{
	u8 i,*p;
	p = rtc_address;         //地址传递
	for(i=0; i<7; i++)            //分7次读取 秒分时日月周年
	{
		Rtc_data[i] = DS3231_ReadOneByte(*p);
		p++;
	}   
}

void DS3231_SetSecond(u8 Second)
{
	Second = Second / 10 * 16 + Second % 10 ;//BCD处理
	DS3231_WriteOneByte(DS3231_SECOND,Second);
}
void DS3231_SetMinute(u8 Minute)
{
	Minute = Minute / 10 * 16 + Minute % 10 ;//BCD处理
	DS3231_WriteOneByte(DS3231_MINUTE,Minute);
}
void DS3231_SetHour(u8 Hour)
{
	Hour = Hour / 10 * 16 + Hour % 10 ;//BCD处理
	DS3231_WriteOneByte(DS3231_HOUR,Hour);
}

void DS3231_SetWeek(u8 Week)
{
	Week = Week / 10 * 16 + Week % 10 ;//BCD处理
	DS3231_WriteOneByte(DS3231_WEEK,Week);
}

void DS3231_SetDate(u8 Date)
{
	Date = Date / 10 * 16 + Date % 10 ;//BCD处理
	DS3231_WriteOneByte(DS3231_DAY,Date);
}
void DS3231_SetMonth(u8 Month)
{
	Month = Month / 10 * 16 + Month % 10 ;//BCD处理
	DS3231_WriteOneByte(DS3231_MONTH,Month);
}
void DS3231_SetYear(u8 Year)
{
	Year = Year / 10 * 16 + Year % 10 ;//BCD处理
	DS3231_WriteOneByte(DS3231_YEAR,Year);
}



u8 DS3231_ReadSecond(void)
{
	return DS3231_ReadOneByte(DS3231_SECOND);
}
u8 DS3231_ReadMinute(void)
{
	return DS3231_ReadOneByte(DS3231_MINUTE);
}
u8 DS3231_ReadHour(void)
{
	return DS3231_ReadOneByte(DS3231_HOUR);
}

u8 DS3231_ReadWeek(void)
{
	return DS3231_ReadOneByte(DS3231_WEEK);
}

u8 DS3231_ReadtDate(void)
{
	return DS3231_ReadOneByte(DS3231_DAY);
}
u8 DS3231_ReadMonth(void)
{
	return DS3231_ReadOneByte(DS3231_MONTH);
}
u8 DS3231_ReadYear(void)
{
	return DS3231_ReadOneByte(DS3231_YEAR);
}


void My_DS3231_ReadRTC(void)
{
	sys_clock_dev.second = DS3231_ReadSecond();
	sys_clock_dev.min = DS3231_ReadMinute();
	sys_clock_dev.hour = DS3231_ReadHour();
	sys_clock_dev.week = DS3231_ReadWeek();
	sys_clock_dev.day = DS3231_ReadtDate();
	sys_clock_dev.month = DS3231_ReadMonth();
	sys_clock_dev.year = DS3231_ReadYear();
}

void My_DS3231_SetRTC(void)
{
	DS3231_SetSecond(sys_clock_dev.second);
	DS3231_SetMinute(sys_clock_dev.min);
	DS3231_SetHour(sys_clock_dev.hour);
	DS3231_SetWeek(sys_clock_dev.week);
	DS3231_SetDate(sys_clock_dev.day);
	DS3231_SetMonth(sys_clock_dev.month);
	DS3231_SetYear(sys_clock_dev.year);
}

void My_DS3231_SetTIME(void)
{
	DS3231_SetSecond(sys_clock_dev.second);
	DS3231_SetMinute(sys_clock_dev.min);
	DS3231_SetHour(sys_clock_dev.hour);
}
