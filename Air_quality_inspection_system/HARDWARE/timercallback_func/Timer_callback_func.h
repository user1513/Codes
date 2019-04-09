#ifndef __TIMER_CALL_BACK_FUNC_H
#define __TIMER_CALL_BACK_FUNC_H

#include "main.h"


bool FuncReg(bool *_FlagName,u32 _DelayTime,bool _FlagState); //设置定时回调任务

void FuncTimeTravers_ISR(void);								  //定时回调函数中断处理	

void CleanCallbackFunc(bool *_FlagName,bool _FlagState);      //删除指定回调函数


#endif
