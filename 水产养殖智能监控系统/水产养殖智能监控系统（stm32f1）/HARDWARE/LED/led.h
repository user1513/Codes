#ifndef __LED_H
#define __LED_H	 

#include "main.h"

#define led PCout(13)// PD.2

#define feed PAout(0)// PD.2

#define keyS1 PCin(14)

#define keyS2 PAin(2)

#define keyS3 PAin(3)

#define keyS4 PAin(6)

#define keyS5 PAin(7)

#define jdq1  PBout(0)

#define jdq2  PBout(1)

#define jdq1Absorption()  	{GPIO_SetBits(GPIOB,GPIO_Pin_0);}//闭合
#define jdq1ToBreakOff()  	{GPIO_ResetBits(GPIOB,GPIO_Pin_0);}//断开
#define jdq2Absorption()  	{GPIO_SetBits(GPIOB,GPIO_Pin_1);}
#define jdq2ToBreakOff()  	{GPIO_ResetBits(GPIOB,GPIO_Pin_1);}

void jdqIoInit(void);
void LED_Init(void);//初始化
void FeedIoInit(void);
void KeyIoInit(void);
void IoInit(void);
u8 GetKeySelflocking(void);
void KeyHandle(void);
void GetKeycontinuity(void);//连续获取按键值
void KeyInside(u8 mode);//按键内部判断函数
#endif
