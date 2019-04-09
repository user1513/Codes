#ifndef _APP_H
#define _APP_H


#include <STC12C5A60S2.H>
#include "intrins.h"



#define uint8_t 	unsigned char 
#define uint16_t 	unsigned int 
#define uint32_t 	unsigned long  
	

enum SYSYEMSTATE
{
	Air_supply_mode,								//�ͷ�ģʽ
	Refrigeration_mode,							//����ģʽ
	Heating_mode,										//����ģʽ	
	Wind_speed_1,										//����1
	Wind_speed_2,										//����2
	Wind_speed_3,										//����3
	Wind_stop,											//ͣת
};

typedef struct _history_param
{
	uint8_t Air_supply_mode_Wind_speed; 		//�ͷ�ģʽ�·���
	uint8_t Refrigeration_mode_Wind_speed; 	//����ģʽ�·���
	uint8_t Heating_mode_Wind_speed; 				//����ģʽ�·���
}sHistory_Param;


typedef struct _system_param
{
	uint16_t Set_tempVal;   				//�趨�¶�
	uint16_t Current_temp;   				//��ǰ�¶�
	uint8_t	 Wind_speed;						//��ǰ����
	uint8_t  Current_Mode;					//��ǰģʽ
	sHistory_Param * History_Param;	//��ʷ����
}sSystem_Param;

#endif 