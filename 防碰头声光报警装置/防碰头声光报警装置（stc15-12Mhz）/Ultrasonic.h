#ifndef  _ULTRASONIC_H_
#define  _ULTRASONIC_H_

#include "usart.h"

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;


u8 UltrasonicCheck_Add8(u8 *buf,u8 length);
void UltrasonicDataPage(u16 UltrasonicDistance);






#endif
