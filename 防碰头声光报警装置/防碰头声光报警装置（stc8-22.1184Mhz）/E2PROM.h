#ifndef _E2PROM_H
#define _E2PROM_H

#include "pbdata.h"
void IapIdle();
u8 IapRead(u16 addr);
void IapProgram(u16 addr, u8 dat);
void IapErase(u16 addr);
myenum IfAddrData(u16 _addr,u8 _comparisonvalue);//��ȡ_addr��������_comparisonvalue�Ƚ�(��ȷ���Enable/�������Disable)
myenum CollectiveWritData(u16 _RestAdrr,u8 *_able,u8 _ablelength,u8 _length);//����д�뺯��(Ĭ�ϵ�һ����)
myenum CollectiveReadData(u16 _RestAdrr,u8 *_able,u8 _ablelength,u8 _length);//�����������(Ĭ�ϵ�һ����)



#endif