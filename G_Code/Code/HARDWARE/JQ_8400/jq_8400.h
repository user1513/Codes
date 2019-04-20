/**
  ************************************* Copyright ******************************   
  *                 (C) Copyright 2019,ZERO,China, NANJING.
  *                            All Rights Reserved
  *                              
  *                     
  *                     https://user1513.github.io/Codes/
  *      
  * FileName   : jq_8400.h   
  * Version    : v1.0		
  * Author     : ZERO		
  * Date       : 2019-04-19         
  * Description:    
  * Function List:  
  	1. ....
  	   <version>: 		
  <modify staff>:
  		  <data>:
   <description>:  
  	2. ...
  ******************************************************************************
 */



#ifndef __JQ_8400_H_
#define __JQ_8400_H_

#include "main.h"

#define MUSIC_INIT 7				//健康管理手环正在初始化，请稍后
#define MUSIC_MEDICINE 1			//时间到了，该吃药了
#define MUSIC_HEART_REAT_LOW 4		//心率偏低
#define MUSIC_HEART_REAT_NORMAL 3	//心率正常
#define MUSIC_HEART_REAT_HIGH 2		//心率偏高
#define MUSIC_HEART_REAT_ERR 5		//心率检测错误
#define MUSIC_GAS_WARNING	6		//气体检测报警


void Jq_8400_Music_Num(u16 num);

void Jq_8400_Set_Volume(u8 Volume);
#endif


 

