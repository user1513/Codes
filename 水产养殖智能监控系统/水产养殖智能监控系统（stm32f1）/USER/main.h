#ifndef _MAIN_H
#define _MAIN_H

#include "sys.h"

typedef unsigned char * String;

enum 
{
	Disable,
	Enable
};

typedef enum
{
	FALSE,
	TRUE
}bool;


#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "McuDataSend.h"
#include "usart2.h"
#include "usart3.h"
#include "sortfunc.h"
#include "math.h"
#include "adc.h"
#include "lcd12864.h"
#include "stdio.h"
extern int DataSave[];


#endif


