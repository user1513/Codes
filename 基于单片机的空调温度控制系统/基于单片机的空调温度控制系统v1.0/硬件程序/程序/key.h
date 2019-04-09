#ifndef _KEY_H
#define _KEY_H

#include "app.h"

sbit KEY1 = P2^1;
sbit KEY2 = P2^2;
sbit KEY3 = P2^3;
sbit KEY4 = P2^4;

#define KEY_STATE (((uint8_t)KEY4 << 3) + ((uint8_t)KEY3 << 2) +  ((uint8_t)KEY2 << 1) + ((uint8_t)KEY1)) //整合成一个变量

uint8_t keyval_Get(uint8_t);
void Event_Handler(uint8_t _key_val);
void Show_demo(uint8_t addr, uint8_t _param);
void motor_speed(uint8_t _speed_val);
void motor_run(void);
#endif