#ifndef __TIMER_CALL_BACK_FUNC_H
#define __TIMER_CALL_BACK_FUNC_H

#include "main.h"


_bool FuncReg(_bool *_FlagName,u32 _DelayTime,_bool _FlagState); //设置定时回调任务

void FuncTimeTravers_ISR(void);								  //定时回调函数中断处理	

void CleanCallbackFunc(_bool *_FlagName,_bool _FlagState);      //删除指定回调函数


#endif
