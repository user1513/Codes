#ifndef __TIMER_CALL_BACK_FUNC_H
#define __TIMER_CALL_BACK_FUNC_H

#include "main.h"


bool FuncReg(bool *_FlagName,u32 _DelayTime,bool _FlagState); //���ö�ʱ�ص�����

void FuncTimeTravers_ISR(void);								  //��ʱ�ص������жϴ���	

void CleanCallbackFunc(bool *_FlagName,bool _FlagState);      //ɾ��ָ���ص�����


#endif
