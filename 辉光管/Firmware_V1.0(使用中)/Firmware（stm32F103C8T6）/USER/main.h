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

extern uint8_t hour;
extern uint8_t minute;
extern uint8_t second;

#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "MyFunc.h"
#include "usart2.h"
#include "usart3.h"
#include "math.h"
#include "adc.h"
#include "stdio.h"
#include "FirmwareDisplay.h"
#include "ds3231.h"
#include "myiic.h"
#include "gps.h" 
#include "string.h"
#include "key.h"
#endif


