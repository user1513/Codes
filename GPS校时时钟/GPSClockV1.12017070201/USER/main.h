#ifndef __MAIN_H
#define __MAIN_H
#include "sys.h"





struct _System_Data_dev
{
	u8 year;	//年
	u8 month;	//月
	u8 day;		//日
	u8 hour;	//时
	u8 min;		//分
	u8 second;	//秒
	u8 week;	//周
	u8 tweek;	//教学周
};

struct _gps_data_dev
{
	u8 year;	//GPS年
	u8 month;	//GPS月
	u8 day;		//GPS日
	u8 hour;	//GPS时
	u8 min;		//GPS分
	u8 sec;		//GPS秒
};

extern struct _System_Data_dev sys_clock_dev;
extern struct _gps_data_dev gps_clock_dev;
 
#endif

