#ifndef _UITRASIBICDATAHANDLE_H
#define _UITRASIBICDATAHANDLE_H


#include "pbdata.h"


u8 Add8check(u8 *Data,u8 length);


void UltrasonicDataFunc(u8 _data);

u16 ModBus_CRC(u8 *arg,u8 length);


//extern u8 UltrasonicArray[];









#endif