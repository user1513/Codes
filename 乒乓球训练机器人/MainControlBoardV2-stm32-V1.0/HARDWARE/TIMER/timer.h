#ifndef __TIMER_H
#define __TIMER_H
#include <sys.h>	 
void TIM6_Init(u16 arr,u16 psc)	;
void Read_Distane(void);
void TIM3_IRQHandler(void);
#endif
