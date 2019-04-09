#ifndef __LED_H
#define __LED_H	 

#include "sys.h"

void LED_Init(void);

#define LED0 PBout(9)
#define LED0_H() {LED0=1;}
#define LED0_L() {LED0=0;}

#endif
