#ifndef _KEY_H
#define _KEY_H

#include "sys.h"

#define KEY_1	PAin(7)   	//上面按键
#define KEY_2	PBin(9)   	//上面按键

/**********************************************************************************************************
** Function name        :   BUTTONPOINTSMOVE
** Descriptions         :   按键点动函数
** parameters           :   key_num 设置按键反馈值
** parameters           :   current_key 传入按键值
** parameters           :   previous_key 上一次按键值
** parameters           :   key_flag 按键标志位
** Returned value       :   按下返回设定按键值，反之返回0
***********************************************************************************************************/

#define BUTTONPOINTSMOVE(key_num, current_key, previous_key, key_flag)  Key_Get(0,key_num,current_key, (uint16_t *) 0,previous_key, key_flag) 


/**********************************************************************************************************
** Function name        :   BUTTONLONGPRESS
** Descriptions         :   按键常按检测函数
** parameters           :   key_num 设置按键反馈值
** parameters           :   current_key 传入按键值
** parameters           :   timeout 超时时间
** parameters           :   previous_key 上一次按键值
** parameters           :   key_flag 按键标志位
** Returned value       :   按下返回设定按键值，反之返回0
***********************************************************************************************************/

#define BUTTONLONGPRESS(key_num, current_key, timeout, previous_key, key_flag) Key_Get(1,key_num,current_key,timeout,previous_key, key_flag) 

void Key_Io_Init(void);			//按键io初始化

uint8_t key_Long_press_test(uint16_t * keyTimeOut);  			//按键长按检测

uint8_t Key_Get(uint8_t mode, uint8_t key_num, uint8_t current_key, uint16_t * keyTimeOut,uint8_t* previous_key,uint8_t *key_flag);//按键值获取函数

uint8_t Key_Event_Handler(void); 								//按键事件处理函数

#endif

