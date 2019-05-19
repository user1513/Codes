#ifndef __MCUDATASEND_H
#define __MCUDATASEND_H
#include "main.h"


bool FuncReg(bool *_FlagName,u32 _DelayTime,bool _FlagState);
void FuncTimeTravers(void);
void FuncDataGet(u8 *data);
u8 GetIntegerlength(u32 _data);


//关于链表的程序
pDATALIST CreateListHard(void);
bool AddList(pDATALIST pHard, u8 * pArray);
bool DeleteList(pDATALIST pHard, u8 addr, pDATALIST *pLast);
void PrintfList(pDATALIST pHard);
void iswork(pDATALIST pHard, u8 * pArray);
bool isAddrRepeat(pDATALIST pHard, u8 * pArray);//判断是否地址已保存，已保存将数据覆盖原始数据
void isSend(pDATALIST pHard);
void sendseat(pDATALIST  p);
void SendListTV1(pDATALIST  p);//發送列表數據
#endif
