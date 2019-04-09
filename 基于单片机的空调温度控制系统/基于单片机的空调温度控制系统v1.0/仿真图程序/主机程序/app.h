#ifndef _APP_H
#define _APP_H

#define uint8_t unsigned char		//重定义unsigned char
#define uint16_t unsigned int		//重定义unsigned int
#define uint32_t unsigned long		//重定义unsigned long
	
#include "stdio.h"	
#include "reg52.h"
#include "temp.h"
#include <intrins.h>
#include "key.h"
#include "timer.h"
#include "AMPIRE12864.h"		   

sbit MOTOR_Bit = P1^7; 			   //电机引脚配置

#endif


