#ifndef __LED_H
#define __LED_H	 

#include "main.h"

#define led PCout(13)		// PC13
#define PM2_5_Led PAout(1)	// PA1

void LED_Init(void);//��ʼ��
void PM2_5_Led_Io_init(void);//PM2.5������led�Ƴ�ʼ��
#endif
