#ifndef __MCUDATASEND_H
#define __MCUDATASEND_H
#include "main.h"


bool FuncReg(bool *_FlagName,u32 _DelayTime,bool _FlagState);
void FuncTimeTravers(void);
void FuncDataGet(u8 *data);
void CleanCallbackFunc(bool *_FlagName,bool _FlagState);


//string
u8 GetIntegerlength(u32 _data);//获取int类型数据的长度
String IntTostring(u32 _data);//将int类型转换成string
int GetDecimalLength(double _data);//获取浮点数长度，默认保留二位
void InversionFunc(String string, u8 _length);//函数倒置
String DecimalToString(double _data);//浮点数->string
String DecimalToStringSymbol(double _data);//不反回正负符号->string
#endif
