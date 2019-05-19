#ifndef __HC595_H
#define __HC595_H
#include "sys.h"


void HC595_Init(void);
void HC595TransmitData(void);

#define Date_DS PDout(7)
#define Date_OE PDout(6)
#define Date_STCP PDout(5) 
#define Date_SHCP PDout(4)	


 
#endif

