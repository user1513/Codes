#ifndef _TIMER_H
#define _TIMER_H

#include "app.h" 

/* define SFR */
sbit TEST_LED = P3^5;               //work LED, flash once per second
void Timer0Init(void);		//1ºÁÃë@11.0592MHz

_bool FuncReg(_bool *_FlagName,u32 _DelayTime,_bool _FlagState);//×ƒÁ¿Ö¸á˜£¬ÑÓ•r•rég£¬˜ËœÊ î‘B


#endif