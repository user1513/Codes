/**
 * ������������غ���
 * ���ߣ�hhelib
 * �����£�2016-03-13
 */
 
#ifndef __SOUND_H
#define __SOUND_H

#include "config.h"

void soundInit(void);
void tone(uint16_t);
void noTone(void);
void playSound(uint8_t var);
void alarmDispatcher(void);
void timer1_int(void);

#endif