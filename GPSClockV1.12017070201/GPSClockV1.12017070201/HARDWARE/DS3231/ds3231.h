#ifndef __DS3231_H
#define __DS3231_H

#include "sys.h"

#define DS3231_WriteAddress 0xD0    //器件写地址 
#define DS3231_ReadAddress  0xD1    //器件读地址

#define DS3231_SECOND       0x00    //秒
#define DS3231_MINUTE       0x01    //分
#define DS3231_HOUR         0x02    //时
#define DS3231_WEEK         0x03    //星期
#define DS3231_DAY          0x04    //日
#define DS3231_MONTH        0x05    //月
#define DS3231_YEAR         0x06    //年
//闹铃1            
 #define DS3231_SALARM1ECOND 0x07    //秒
#define DS3231_ALARM1MINUTE 0x08    //分
#define DS3231_ALARM1HOUR   0x09    //时
#define DS3231_ALARM1WEEK   0x0A    //星期/日
//闹铃2
 #define DS3231_ALARM2MINUTE 0x0b    //分
#define DS3231_ALARM2HOUR   0x0c    //时
#define DS3231_ALARM2WEEK   0x0d    //星期/日
#define DS3231_CONTROL      0x0e    //控制寄存器
#define DS3231_STATUS       0x0f    //状态寄存器
#define BSY                 2       //忙
#define OSF                 7       //振荡器停止标志
#define DS3231_XTAL         0x10    //晶体老化寄存器
#define DS3231_TEMPERATUREH 0x11    //温度寄存器高字节(8位)
#define DS3231_TEMPERATUREL 0x12    //温度寄存器低字节(高2位) 





void DS3231_Init(void);
void DS3231_WriteOneByte(u16 WriteAddr,u8 DataToWrite);
u8 DS3231_ReadOneByte(u8 ReadAddr);
void DS3231_SetRTC(u8 Rtc_data[]);
void DS3231_ReadRTC(u8 Rtc_data[]);

void DS3231_SetSecond(u8 Second);
void DS3231_SetMinute(u8 Minute);
void DS3231_SetHour(u8 Hour);
void DS3231_SetWeek(u8 Week);
void DS3231_SetDate(u8 Date);
void DS3231_SetMonth(u8 Month);
void DS3231_SetYear(u8 Year);

u8 DS3231_ReadSecond(void);
u8 DS3231_ReadMinute(void);
u8 DS3231_ReadHour(void);
u8 DS3231_ReadWeek(void);
u8 DS3231_ReadtDate(void);
u8 DS3231_ReadMonth(void);
u8 DS3231_ReadYear(void);

void My_DS3231_ReadRTC(void);
void My_DS3231_SetRTC(void);
void My_DS3231_SetTIME(void);
#endif

