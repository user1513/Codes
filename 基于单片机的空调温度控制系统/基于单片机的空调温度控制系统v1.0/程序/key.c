#include "key.h"
#include "lcd12864.h"
#include "stdio.h"

uint8_t motor_speed_pwm = 0;
uint8_t Refrigeration_mode_flag = 0;
uint8_t Heating_mode_flag = 0;
extern sSystem_Param 		*pSystem_Param;			//声明结构体指针变量
extern void Show_demo(uint8_t addr, uint8_t _param);	

xdata uint8_t able1[16] = {0};											//声明数组
uint8_t keyval_Get(uint8_t _current_key)						//按键值获取函数
{
	static uint8_t _previous_key = 0xff , _key_flag = 1;//变量声明
	static uint8_t timeout_500ms = 0;										//变量声明			
	uint8_t keyval = 0; 																//变量声明

	if(_current_key != 0x0f && _previous_key == _current_key && _key_flag)		//if(当前按键值 ！= 0x0f 并且 当前按键值 == 之前按键值 并且自锁开关没有关闭)
	{
	   _key_flag = 0;																													//自锁开关打开
			timeout_500ms = 0;																										//500ms超时变量清零
		if((_current_key & 1) == 0)																							//判断具体按键
			keyval |= 1;
		
		if((_current_key & (1 << 1)) == 0)																			//判断具体按键
			keyval |= 1 << 1;
		
		if((_current_key & (1 << 2))  == 0)																			//判断具体按键
			keyval |= 1 << 2;
		
		if((_current_key & (1 << 3))  == 0)																			//判断具体按键
			keyval |= 1 << 3;

	}
	else if(_current_key == 0x0f)																							//当没有按键按下是自锁开关，关闭
	{
		_key_flag = 1;
	}
	
	if((_current_key & (1 << 1)) == 0 || (_current_key & (1 << 2))  == 0)			//判断是自增或者自减按键按下怎么触发超时任务
	{
		timeout_500ms ++;
		if(timeout_500ms > 50)
			_key_flag = 1;
			
	}
	_previous_key = _current_key;																							//将当前的按键值赋值给之前按键值的变量
	
	return keyval;
}


void Event_Handler(uint8_t _key_val)																				//事件处理函数
{
	switch(_key_val)																													//判断按键值
	{
		case 1:if(++(pSystem_Param->Current_Mode) == Wind_speed_1)							//运行模式切换
				{
					pSystem_Param->Current_Mode = Air_supply_mode;
				}
				Refrigeration_mode_flag = 0;
				Heating_mode_flag = 0;
				Show_demo(0x80, pSystem_Param->Current_Mode);												//显示
				switch(pSystem_Param->Current_Mode)
				{
					case Air_supply_mode:
						
						pSystem_Param->Wind_speed = pSystem_Param->History_Param->Air_supply_mode_Wind_speed;													//显示
						break;
					case Refrigeration_mode: 
						pSystem_Param->Wind_speed = pSystem_Param->History_Param->Refrigeration_mode_Wind_speed;													//显示
						break;
					case Heating_mode:
						pSystem_Param->Wind_speed = pSystem_Param->History_Param->Heating_mode_Wind_speed;													//显示
						break;
					default:break;
				}
				Show_demo(0x98, pSystem_Param->Wind_speed);													//显示
			break;
		case 2:pSystem_Param->Set_tempVal += 5;break;														//设定温度自增（没有做保护）		
		case 4:pSystem_Param->Set_tempVal -= 5;break;														//设定温度自减（没有做保护）
		case 8:
			switch(pSystem_Param->Current_Mode)
			{
				case Air_supply_mode:																																									//记录送风模式下风速
							if(--(pSystem_Param->History_Param->Air_supply_mode_Wind_speed) == Heating_mode)								//风速切换
							{
								pSystem_Param->History_Param->Air_supply_mode_Wind_speed = Wind_speed_3;
							}
							pSystem_Param->Wind_speed = pSystem_Param->History_Param->Air_supply_mode_Wind_speed;
							break;
						case Refrigeration_mode:																																					//记录制冷模式下风速	
							if(--(pSystem_Param->History_Param->Refrigeration_mode_Wind_speed) == Heating_mode)							//风速切换
							{
								pSystem_Param->History_Param->Refrigeration_mode_Wind_speed = Wind_speed_3;
							}	
							pSystem_Param->Wind_speed	= pSystem_Param->History_Param->Refrigeration_mode_Wind_speed;						
							break;
						case Heating_mode:																																								//记录制热模式下风速
							if(--(pSystem_Param->History_Param->Heating_mode_Wind_speed) == Heating_mode)										//风速切换
							{
								pSystem_Param->History_Param->Heating_mode_Wind_speed = Wind_speed_3;
							}		
							pSystem_Param->Wind_speed = pSystem_Param->History_Param->Heating_mode_Wind_speed;
							break;
						default:break;
			}
			
				Show_demo(0x98, pSystem_Param->Wind_speed);																														//显示
				break;
		default: break;
	}
}


void motor_run(void)																						//电机运行函数
{
	switch(pSystem_Param->Current_Mode)														//获取当前状态
	{
		case Air_supply_mode:																				//送风模式
			motor_speed(pSystem_Param->Wind_speed);										//根据风速进行控制电机运行	
			Show_demo(0x98,pSystem_Param->Wind_speed);								//显示风速
			break;
		case Refrigeration_mode: 																		//制冷模式
			if((pSystem_Param->Set_tempVal) - 30 >= pSystem_Param->Current_temp && Refrigeration_mode_flag == 0)  //当设定温度-3度大于等于当前温度
			{
				Refrigeration_mode_flag = 1;																													
				motor_speed(Wind_stop);																																							//电机停转
				Show_demo(0x98,Wind_stop);																																					//显示	
			}
			else if(Refrigeration_mode_flag == 1 && ((pSystem_Param->Set_tempVal) + 30 < pSystem_Param->Current_temp))//当设定温度+3度<当前温度
			{
				Refrigeration_mode_flag = 0;
			}
			if(Refrigeration_mode_flag == 0 )																																				
			{
				motor_speed(pSystem_Param->Wind_speed);									//根据风速进行控制电机运行	
				Show_demo(0x98,pSystem_Param->Wind_speed);							//显示风速
			}
			break;
		case Heating_mode:																					//制热模式
			if((pSystem_Param->Set_tempVal) + 30 <= pSystem_Param->Current_temp && Heating_mode_flag == 0)	//当设定温度-3度小于等于当前温度
			{
				Heating_mode_flag = 1;
				motor_speed(Wind_stop);																																				//电机停转
				Show_demo(0x98,Wind_stop);																																		//显示
			}
			else if(Heating_mode_flag == 1 && ((pSystem_Param->Set_tempVal) - 30 > pSystem_Param->Current_temp))//当设定温度+3度>当前温度
			{
				Heating_mode_flag = 0;
			}
			if(Heating_mode_flag == 0 )
			{
				motor_speed(pSystem_Param->Wind_speed);								//根据风速进行控制电机运行
				Show_demo(0x98,pSystem_Param->Wind_speed);						//显示风速
			}
			break;
		default:break;
	}
}

void motor_speed(uint8_t _speed_val)
{
	switch(_speed_val)
	{
		case Wind_speed_1:motor_speed_pwm = 10; break;										//风速1
		case Wind_speed_2:motor_speed_pwm = 5;break;										  //风速2
		case Wind_speed_3:motor_speed_pwm = 2;break;									   	//风速3
		case Wind_stop:motor_speed_pwm = 0;break;													//停转
		default:motor_speed_pwm = 0;break;
	}
}

void Show_demo(uint8_t addr, uint8_t _param)
{
	switch(_param)
	{
		case Air_supply_mode:		Wr_Data_String(addr, "运行模式：送风");break;
		
		case Refrigeration_mode:Wr_Data_String(addr, "运行模式：制冷");break;
		
		case Heating_mode:			Wr_Data_String(addr, "运行模式：制热");break;
		
		case Wind_speed_1:			Wr_Data_String(addr, "当前风速：①  档");break;
		
		case Wind_speed_2:			Wr_Data_String(addr, "当前风速：②  档");break;
		
		case Wind_speed_3:			Wr_Data_String(addr, "当前风速：③  档");break;
		
		case Wind_stop:					Wr_Data_String(addr, "当前风速：STOP  ");break;
		
		default :break;
	}
}

