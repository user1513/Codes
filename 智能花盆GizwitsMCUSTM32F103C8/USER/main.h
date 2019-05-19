#ifndef _MAIN_H
#define _MAIN_H

#include "sys.h"


typedef unsigned char * String;

typedef enum
{
	FALSE,
	TRUE
}bool;

typedef struct 
{
	//公历日月年周
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;	
	
	vu8 hour;
	vu8 min;
	vu8 sec;			
	 
}_calendar_obj;	


#include "led.h"
#include "delay.h"
#include "key.h"
#include "usart.h"	 
#include "adc.h"
#include "lsens.h"
#include "timer.h"
#include "usart3.h"
#include "gizwits_product.h" 
#include "timerfunc.h"
#include "rtc.h"
#include "usart2.h"






#endif



