#ifndef _TIMERFUNC_H
#define _TIMERFUNC_H
#include "main.h"


bool FuncReg(bool *_FlagName,u32 _DelayTime,bool _FlagState);
void FuncTimeTravers(void);
void FuncDataGet(u8 *data);
void CleanCallbackFunc(bool *_FlagName,bool _FlagState);



#endif
