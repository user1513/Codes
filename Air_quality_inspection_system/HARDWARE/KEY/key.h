#ifndef _KEY_H
#define _KEY_H

#include "sys.h"

#define MIDDLE_KEY 	PCin(14)   	//�м䰴��
#define ABOVE_KEY	PAin(2)   	//���水��
#define BELOW_KEY 	PCin(15)   	//���水��


/**********************************************************************************************************
** Function name        :   BUTTONPOINTSMOVE
** Descriptions         :   �����㶯����
** parameters           :   key_num ���ð�������ֵ
** parameters           :   current_key ���밴��ֵ
** parameters           :   previous_key ��һ�ΰ���ֵ
** parameters           :   key_flag ������־λ
** Returned value       :   ���·����趨����ֵ����֮����0
***********************************************************************************************************/

#define BUTTONPOINTSMOVE(key_num, current_key, previous_key, key_flag)  Key_Get(0,key_num,current_key, (uint16_t *) 0,previous_key, key_flag) 


/**********************************************************************************************************
** Function name        :   BUTTONLONGPRESS
** Descriptions         :   ����������⺯��
** parameters           :   key_num ���ð�������ֵ
** parameters           :   current_key ���밴��ֵ
** parameters           :   timeout ��ʱʱ��
** parameters           :   previous_key ��һ�ΰ���ֵ
** parameters           :   key_flag ������־λ
** Returned value       :   ���·����趨����ֵ����֮����0
***********************************************************************************************************/

#define BUTTONLONGPRESS(key_num, current_key, timeout, previous_key, key_flag) Key_Get(1,key_num,current_key,timeout,previous_key, key_flag) 

void Key_Io_Init(void);			//����io��ʼ��

uint8_t key_Long_press_test(uint16_t * keyTimeOut);  			//�����������

uint8_t Key_Get(uint8_t mode, uint8_t key_num, uint8_t current_key, uint16_t * keyTimeOut,uint8_t* previous_key,uint8_t *key_flag);//����ֵ��ȡ����

uint8_t Key_Event_Handler(void); 								//�����¼�������

#endif

