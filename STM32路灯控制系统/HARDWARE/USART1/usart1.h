#ifndef __USART2_H
#define	__USART2_H
//RTrobot.org
//禁止用于商业行为

#include "stm32f10x.h"
#include <stdio.h>

extern unsigned int run_time;

void Usart1_Send(unsigned char *str);
void usart1_init(u32 bound);

extern unsigned char GPS_Stop_flag;
extern char GPS_Uart1_Data[256];



#endif /* __USART1_H */
