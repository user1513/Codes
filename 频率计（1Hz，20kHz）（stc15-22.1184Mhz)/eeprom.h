#ifndef _EEPROM_H
#define _EEPROM_H


#include "main.h"


//≤‚ ‘µÿ÷∑
#define IAP_ADDRESSRST 0x0000
#define IAP_ADDRESSH 0x0008
#define IAP_ADDRESSL 0x00016
void IapIdle();
u8 IapReadByte(u16 addr);
void IapProgramByte(u16 addr, u8 dat);
void IapEraseSector(u16 addr);








#endif



