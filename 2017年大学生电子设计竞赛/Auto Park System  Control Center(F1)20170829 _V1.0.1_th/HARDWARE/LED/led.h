#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED1 PGout(14)// PB5
#define LED2 PGout(15)// PE5	
#define LED3 PBout(5)// PB5
#define LED4 PBout(6)// PE5	
#define LED5 PBout(7)// PB5
#define LED6 PBout(8)// PE5	

void LED_Init(void);//��ʼ��

		 				    
#endif
