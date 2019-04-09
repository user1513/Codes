#ifndef __LED_H
#define __LED_H	 

#include "main.h"

#define led PCout(13)		// PC13
#define PM2_5_Led PAout(1)	// PA1

void LED_Init(void);//初始化
void PM2_5_Led_Io_init(void);//PM2.5传感器led灯初始化
#endif
