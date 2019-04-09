#ifndef _APP_H
#define _APP_H


#include <STC12C5A60S2.H>
#include "intrins.h"



#define uint8_t 	unsigned char 
#define uint16_t 	unsigned int 
#define uint32_t 	unsigned long  
	

enum SYSYEMSTATE
{
	Air_supply_mode,								//送风模式
	Refrigeration_mode,							//制冷模式
	Heating_mode,										//制热模式	
	Wind_speed_1,										//风速1
	Wind_speed_2,										//风速2
	Wind_speed_3,										//风速3
	Wind_stop,											//停转
};

typedef struct _history_param
{
	uint8_t Air_supply_mode_Wind_speed; 		//送风模式下风速
	uint8_t Refrigeration_mode_Wind_speed; 	//制冷模式下风速
	uint8_t Heating_mode_Wind_speed; 				//制热模式下风速
}sHistory_Param;


typedef struct _system_param
{
	uint16_t Set_tempVal;   				//设定温度
	uint16_t Current_temp;   				//当前温度
	uint8_t	 Wind_speed;						//当前风速
	uint8_t  Current_Mode;					//当前模式
	sHistory_Param * History_Param;	//历史参数
}sSystem_Param;

#endif 