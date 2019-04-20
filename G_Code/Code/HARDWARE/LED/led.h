#ifndef __LED_H
#define __LED_H	 

#include "main.h"

#define led PCout(13)		// PC13

#define BUZEER_ENABLE 	PAout(1) = 0

#define BUZEER_DISABLE 	PAout(1) = 1

void Periph_Init(void);

#endif
