/**
 * MAX6921����
 * оƬ��飺
 * ���ߣ�
 * �����£�
 */

#ifndef __HV57708_H
#define __HV57708_H

#include "config.h"

#define MAX6921_MODE_NORMAL 1
#define MAX6921_MODE_INVERTION 0

void MAX6921_Init(uint8_t MAX6921_Mode);
void MAX6921_SendData(uint32_t datapart1, uint32_t datapart2);
void MAX6921_OutputData(void);

#endif