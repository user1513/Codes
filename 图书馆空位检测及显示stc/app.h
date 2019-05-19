#ifndef _APP_H
#define _APP_H

typedef  unsigned char u8;
typedef  unsigned int u16;
typedef  unsigned long u32 ;


typedef enum 
{
	FLASE,
	TRUE
}_bool;

#include <STC12C5A60S2.H>
#include "usart.h"
#include "timer.h"
#include "seat.h"




extern bit uart_get_flag;
extern u8 uart_cmd_data[];
#endif