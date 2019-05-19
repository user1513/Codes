#include "myeeprom.h"
#include "24cxx.h"

#define MaxAlarmNumber 50//最大闹铃个数

//一
#define AlarmData1Addr(i)	0 + 8 * i
#define AlarmData2Addr(i)	1 + 8 * i
#define AlarmData3Addr(i)	2 + 8 * i
#define AlarmData4Addr(i)	3 + 8 * i
#define AlarmData5Addr(i)	4 + 8 * i
#define AlarmData6Addr(i)	5 + 8 * i;
#define AlarmData7Addr(i)	6 + 8 * i
#define AlarmData8Addr(i)	7 + 8 * i

//二
#define AlarmDataAddr(_No,_an)	_No + 8 * _an	//_No：闹钟数据号，闹钟编号


#define PasswordHighAddr	7 + 8 * 49 + 1
#define PasswordLowAddr		7 + 8 * 49 + 2

#define AlarmnumberAddr		7 + 8 * 49 + 3

void StorePassword(u16 password)
{
	AT24CXX_WriteOneByte(PasswordHighAddr,password >> 8);
	AT24CXX_WriteOneByte(PasswordLowAddr,password & 0x00FF);
}
u16 ReadPassword(void)
{
	u16 password = AT24CXX_ReadOneByte(PasswordHighAddr);
	password <<= 8;
	password |= AT24CXX_ReadOneByte(PasswordLowAddr);
	return password;
}

void StorAlarmNumber(u8 nowAlarmnumber)
{
	AT24CXX_WriteOneByte(AlarmnumberAddr,nowAlarmnumber);
}

u8 ReadAlarmNumber(void)
{
	return AT24CXX_ReadOneByte(AlarmnumberAddr);
}

void StoreAlarmData(u8 ArarmDataNo,u8 AlarmNumber,u8 data)
{
	int addr = AlarmDataAddr(ArarmDataNo,AlarmNumber);
	AT24CXX_WriteOneByte(addr,data);
}

u8 ReadAlarmData(u8 ArarmDataNo,u8 AlarmNumber)
{
	return AT24CXX_ReadOneByte(AlarmDataAddr(ArarmDataNo,AlarmNumber));
}
