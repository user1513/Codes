#include "app.h"
#include "usart.h"
#include "timer.h"
#include "lcd12864.h"
#include "stdio.h"
#include "temp.h"
#include "key.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define INITSETTEMPVAL 200          							//设置初始设定温度值（200/10）

#define INITSETWINSPEEDVAL Wind_speed_3 		      //设置初始风速值（默认三档）

#define INITMODE	Air_supply_mode									//设置初始模式（默认送风）	

//////////////////////////////////////////////////////////////////////////////////////////////////////////
xdata uint8_t able[16] = {0};											//声明数组

sHistory_Param  	History_Param;									//声明结构体变量		

sSystem_Param 		System_Param;										//声明结构体变量

sHistory_Param  	*pHistory_Param = &History_Param;									//声明结构体指针变量		

sSystem_Param 		*pSystem_Param = &System_Param;										//声明结构体指针变量

uint8_t ukey_val = 0;															//按键值获取

uint32_t STATE_FLAG = 0;													//状态标志位
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void Init_Config(void);														//函数声明

void param_Init(sSystem_Param * _System_Param,sHistory_Param * _History_Param);//函数声明

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void param_Init(sSystem_Param * _System_Param,sHistory_Param * _History_Param)		//结构体参数初始化
{
	_System_Param->Set_tempVal = INITSETTEMPVAL ;																	//设置初始设置温度
	
	_System_Param->Wind_speed = INITSETWINSPEEDVAL;																//设置初始风速
	
	_System_Param->Current_Mode = INITMODE;																				//设置初始模式
	
	_System_Param->History_Param = _History_Param;																//传入历史参数结构体指针
	
	_System_Param->History_Param -> Air_supply_mode_Wind_speed = Wind_speed_3;		//写入默认风速值
	
	_System_Param->History_Param -> Refrigeration_mode_Wind_speed = Wind_speed_3;	//写入默认风速值
	
	_System_Param->History_Param -> Heating_mode_Wind_speed = Wind_speed_3;				//写入默认风速值
}

void Init_Config(void)
{
	Lcm_Init();																																		//lcd12864初始化
	
	Timer0Init();																																	//定时器0初始化
	
	param_Init(pSystem_Param, pHistory_Param);																		//结构体参数初始化
	
	Show_demo(0x80, pSystem_Param->Current_Mode);																	//显示

	Show_demo(0x98, pSystem_Param->Wind_speed);																		//显示
}



void main()
{
	Init_Config();
	while(1)
	{
		if(STATE_FLAG & (1 << 0 ))
		{
			STATE_FLAG &= ~(1 << 0);
			
			sprintf(able, "设置温度：%02d.%d度",(uint16_t)pSystem_Param->Set_tempVal/10, (uint16_t)pSystem_Param->Set_tempVal%10);
	
			Wr_Data_String(0x90, able);
	
			pSystem_Param->Current_temp = LcdDisplay(Ds18b20ReadTemp());
	
			sprintf(able, "实际温度：%02d.%d度",(uint16_t)pSystem_Param->Current_temp/10, (uint16_t)pSystem_Param->Current_temp%10);

			Wr_Data_String(0x88, able);
		}	
		motor_run();		
	}
}