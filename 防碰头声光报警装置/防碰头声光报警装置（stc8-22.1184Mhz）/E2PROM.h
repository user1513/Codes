#ifndef _E2PROM_H
#define _E2PROM_H

#include "pbdata.h"
void IapIdle();
u8 IapRead(u16 addr);
void IapProgram(u16 addr, u8 dat);
void IapErase(u16 addr);
myenum IfAddrData(u16 _addr,u8 _comparisonvalue);//读取_addr的数据与_comparisonvalue比较(正确输出Enable/错误输出Disable)
myenum CollectiveWritData(u16 _RestAdrr,u8 *_able,u8 _ablelength,u8 _length);//集体写入函数(默认第一扇区)
myenum CollectiveReadData(u16 _RestAdrr,u8 *_able,u8 _ablelength,u8 _length);//集体读出函数(默认第一扇区)



#endif