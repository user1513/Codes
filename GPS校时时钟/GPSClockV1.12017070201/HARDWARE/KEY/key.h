#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

#define KEY1 PBin(0)	// PC8
#define KEY2 PBin(1)	// PC9

#define S1_PRES	1		//KEY1
#define S2_PRES	2		//KEY2


void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数	

#endif
