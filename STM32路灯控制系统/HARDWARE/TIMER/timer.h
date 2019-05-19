#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#include "led.h"
#include "usart1.h"	
#include "dht11.h"
#include "_usart_send.h"
#include "Data_Check.h"
#include "Bh1750fvi.h"
#include "gps.h"
#include "adc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   


void TIM3_Int_Init(u16 arr,u16 psc);
void TIM2_Int_Init(u16 arr,u16 psc);
void street_lamp_Rgb(u8 rgb_flag,u8 R_Val,u8 G_Val,u8 B_Val);
#endif
