/************************************
������MPU6050����������������λ�������ͨѶЭ��
�Լ����ݷ��ͺ���
**************************************/
#ifndef __MCUDATASEND_H
#define __MCUDATASEND_H
#include "sys.h"
#include "stm32f10x.h"

void Data_Send_Status(short pitch,short roll,short yaw);
void Data_Send_Senser(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);

#endif
