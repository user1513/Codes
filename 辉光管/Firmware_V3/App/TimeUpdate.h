/**
 * 时间更新相关函数
 * 作者：hhelib
 * 最后更新：2016-03-13
 **************************
 * 使用时应根据时钟频率调整定时器初值
 */
 
#ifndef __TIME_UPDATE_H
#define __TIME_UPDATE_H

#include "config.h"

void timerInit(void);
void timeUpdate_ISR(void);

#endif