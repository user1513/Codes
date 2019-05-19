#ifndef __MAIN_H
#define __MAIN_H
#include "sys.h"





struct _System_Data_dev
{
	u8 year;	//��
	u8 month;	//��
	u8 day;		//��
	u8 hour;	//ʱ
	u8 min;		//��
	u8 second;	//��
	u8 week;	//��
	u8 tweek;	//��ѧ��
};

struct _gps_data_dev
{
	u8 year;	//GPS��
	u8 month;	//GPS��
	u8 day;		//GPS��
	u8 hour;	//GPSʱ
	u8 min;		//GPS��
	u8 sec;		//GPS��
};

extern struct _System_Data_dev sys_clock_dev;
extern struct _gps_data_dev gps_clock_dev;
 
#endif

