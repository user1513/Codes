/**
  ************************************* Copyright ****************************** 
  *
  *                 (C) Copyright 2019,ZERO,China, NANJING.
  *                            All Rights Reserved
  *                              
  *                     
  *                     https://user1513.github.io/Codes/
  *    
  * FileName   : jq_8400.c   
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

#include "jq_8400.h"

static u8 Sum_Check(u8 * able, u8 num);

void Jq_8400_Music_Num(u16 num)
{
	u8 able[6] = {0};
	
	u8 _num = 0;
	
	able[_num++] = 0xAA;

	able[_num++] = 0x07;

	able[_num++] = 0x02;

	able[_num++] = num >> 8;

	able[_num++] = (u8)num;
	
	able[_num++] = Sum_Check(able, _num);
	
	uart2_Send_Len(able ,_num);
}

void Jq_8400_Set_Volume(u8 Volume)
{
	u8 able[5] = {0};
	
	u8 _num = 0;
	
	able[_num++] = 0xAA;

	able[_num++] = 0x13;

	able[_num++] = 0x01;

	able[_num++] = (u8)Volume;
	
	able[_num++] = Sum_Check(able, _num);
	
	uart2_Send_Len(able ,_num);
}

static u8 Sum_Check(u8 * able, u8 num)
{
	u8 Sum_Check_val = 0;
	while(num--)
	{
		Sum_Check_val += *(able ++);
	}
	return Sum_Check_val;
}
