#ifndef __MCUDATASEND_H
#define __MCUDATASEND_H
#include "main.h"


bool FuncReg(bool *_FlagName,u32 _DelayTime,bool _FlagState);
void FuncTimeTravers(void);
void FuncDataGet(u8 *data);
void CleanCallbackFunc(bool *_FlagName,bool _FlagState);


//string
u8 GetIntegerlength(u32 _data);//��ȡint�������ݵĳ���
String IntTostring(u32 _data);//��int����ת����string
int GetDecimalLength(double _data);//��ȡ���������ȣ�Ĭ�ϱ�����λ
void InversionFunc(String string, u8 _length);//��������
String DecimalToString(double _data);//������->string
String DecimalToStringSymbol(double _data);//��������������->string
#endif
