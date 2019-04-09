#ifndef _SOUND_LIGHT_ALARM_H
#define _SOUND_LIGHT_ALARM_H


#include "sys.h"


#define Buzzer_Enable()  	PBout(0) = 1
#define Buzzer_Disable() 	PBout(0) = 0

#define Led_Enable()		PBout(1) = 1
#define Led_Disable()		PBout(1) = 0

void Sound_Light_Alarm_Io_Init(void);











#endif


