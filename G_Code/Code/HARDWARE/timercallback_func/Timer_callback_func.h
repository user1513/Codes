#ifndef __TIMER_CALL_BACK_FUNC_H
#define __TIMER_CALL_BACK_FUNC_H

#include "main.h"


_bool FuncReg(_bool *_FlagName,u32 _DelayTime,_bool _FlagState); //���ö�ʱ�ص�����

void FuncTimeTravers_ISR(void);								  //��ʱ�ص������жϴ���	

void CleanCallbackFunc(_bool *_FlagName,_bool _FlagState);      //ɾ��ָ���ص�����


#endif
