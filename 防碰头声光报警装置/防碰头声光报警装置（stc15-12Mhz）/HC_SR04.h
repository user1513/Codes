#ifndef _HC_SR04_H
#define _HC_SR04_H

#include "intrins.h"
#include <STC15F2K60S2.H>
#include "Ultrasonic.h"
#include "timer.h"
sbit TRIG=P3^3;//Send
sbit ECHO=P3^2;//get

unsigned int  Conut(void);
void StartModule(void);





#endif