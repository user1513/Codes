#ifndef _APP_H
#define _APP_H

#define uint8_t unsigned char		//�ض���unsigned char
#define uint16_t unsigned int		//�ض���unsigned int
#define uint32_t unsigned long		//�ض���unsigned long
	
#include "stdio.h"	
#include "reg52.h"
#include "temp.h"
#include <intrins.h>
#include "key.h"
#include "timer.h"
#include "AMPIRE12864.h"		   

sbit MOTOR_Bit = P1^7; 			   //�����������

#endif


