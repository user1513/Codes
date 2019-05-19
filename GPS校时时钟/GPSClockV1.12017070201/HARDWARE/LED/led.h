#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED PAout(0)	// PA12
#define DP  PAout(1)

void LED_Init(void);
	

#endif
