/**
  ************************************* Copyright ****************************** 
  *
  *                 (C) Copyright 2019,ZERO,China, NANJING.
  *                            All Rights Reserved
  *                              
  *                     
  *                     https://user1513.github.io/Codes/
  *    
  * FileName   : Bluetooth.c   
  * Version    : v1.0		
  * Author     : ZERO			
  * Date       : 2019-04-12         
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
#include "Bluetooth.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"
#define COMMA_COUNT 2 																					//逗号的个数

#define DATAPACK_LENGTH (COMMA_COUNT + 1)

uint8_t xdata StrPack[DATAPACK_LENGTH][30] = {{0},{0},{0}};
 

static uint8_t HandlerFunc(void) ;
static uint8_t SubpackageAnalysis_Check(uint8_t * str);//分包解析
 
static uint8_t SubpackageAnalysis_Check(uint8_t * str)//分包解析
{
	uint8_t _CheckIsOk = 0;																					//设置返回值，默认检验失败
	uint8_t _CommaNum = 0;																					//获取','的个数
	uint8_t * _pStrCurrentAdder = str, * _pStrPreviousAdder = NULL;											//创建当前指针变量与先前指针变量
	if((strstr((char*) str,(char*)"AT") != NULL) && (strlen(str) == 2))
	{
		_CheckIsOk = 2;
		return _CheckIsOk;	
	}
	while(1)
	{
		_pStrCurrentAdder = (uint8_t *)strchr((const char *)_pStrCurrentAdder, ',');				//获取第一个','所在_pStrCurrentAdder中的地址,没有找到显示NULL
		if(_pStrCurrentAdder != _pStrPreviousAdder && _pStrCurrentAdder != NULL)					//判断当前与之前不同
		{
			_CommaNum ++;																			//','个数++
			_pStrPreviousAdder = _pStrCurrentAdder;													//将当前指针变量内容赋值给之前的指针变量
			_pStrCurrentAdder++;																	//当前指针变量地址++
		}
		else if(_CommaNum < COMMA_COUNT)															//当之前指针变量的地址等于当前指针变量地址 && ','的个数小于校验个数退出循环
		{
			break;
		}
		
		if(_CommaNum == COMMA_COUNT && _pStrCurrentAdder == NULL)									//判断当逗号个数是否等于宏定义设定值 && 当前指针变量内容为NULL
		{
			_CheckIsOk = 1;																		    //设置检验成功
			break;
		}
		else if(_CommaNum > COMMA_COUNT)														 	//如果逗号个数>设定值退出
		{
			break;
		}
	}
		if((_CheckIsOk != 0) && (NULL == (uint8_t *)strchr((const char *)str, ';')))				//当_CheckIsOk为非零时 && 查找Str数组中';' == NULL
		{
			_CheckIsOk = 0;																			//设置校验失败		
		}
		return _CheckIsOk;																			//返回_CheckIsOk的状态
}


uint8_t Data_Parsing_Func(uint8_t * str)
{
	uint8_t i = 0;
	uint8_t _StateVal = 0;
	uint8_t * _pStrAdder = str;
	_StateVal = SubpackageAnalysis_Check(str);
	if(_StateVal == 1)
	{
		for(i = 0; i < COMMA_COUNT; i++)
		{
			_pStrAdder = (uint8_t *)strchr((const char *)str, ',');
			strncpy((char *)StrPack[i], (const char *)str, _pStrAdder - str);
			*(StrPack[i] + (_pStrAdder - str)) = '\0';
			str = _pStrAdder + 1;
		}
		_pStrAdder = (uint8_t *)strchr((const char *)str, ';');
		strncpy((char *)StrPack[DATAPACK_LENGTH - 1], (const char *)str, _pStrAdder - str);
		*(StrPack[i] + (_pStrAdder - str)) = '\0';
		HandlerFunc();
	}
	return _StateVal;
}

uint8_t strupr(uint8_t *s)
{
	uint8_t err = 1;
	if(s == NULL)
	{
		err = 0;
		return err;
	}
	while(*s != '\0')
	{
		if( 'a' <= *s && 'z' >= *s)
		{
			*s -= 'a' - 'A';
		}
		s ++;
	}
	return err;
}


extern uint8_t cmd_able[];

static uint8_t HandlerFunc(void) 
{
	uint8_t err = 1;
	cmd_able[3] = 1;
	if(strcmp(StrPack[0],"LED") == 0)
	{
		cmd_able[0] = 1;
	}
	else if(strcmp(StrPack[0],"BUZZER") == 0)
	{
		cmd_able[0] = 2;
	}
	else if(strcmp(StrPack[0],"RELAY") == 0)
	{
		cmd_able[0] = 3;
	}
	else
	{
		UART_2SendString("ERROR:001");
		cmd_able[3] = 0;
		err = 0; 
	}
	if(err != 0)
	{	
		if(cmd_able[0] == 1)
		{
			cmd_able[1] = 0;
			if(strcmp(StrPack[1],"LED1") == 0)
			{
				cmd_able[1] = 1;
			}
			else if(strcmp(StrPack[1],"LED2") == 0)
			{
				cmd_able[1] = 2;
			}
			else if(strcmp(StrPack[1],"LED3") == 0)
			{
				cmd_able[1] = 3;
			}
			else if(strcmp(StrPack[1],"LED4") == 0)
			{
				cmd_able[1] = 4;
			}
			else if(strcmp(StrPack[1],"LED5") == 0)
			{
				cmd_able[1] = 5;
			}
			else if(strcmp(StrPack[1],"LED6") == 0)
			{
				cmd_able[1] = 6;
			}
			else if(strcmp(StrPack[1],"LED7") == 0)
			{
				cmd_able[1] = 7;
			}
			else if(strcmp(StrPack[1],"LED8") == 0)
			{
				cmd_able[1] = 8;
			}
			else
			{
				
			}
		}
		else if(cmd_able[0] == 3)
		{
			if(strcmp(StrPack[1],"RELAY1") == 0)
			{
				cmd_able[1] = 1;
			}
			else if(strcmp(StrPack[1],"RELAY2") == 0)
			{
				cmd_able[1] = 2;
			}
			else if(strcmp(StrPack[1],"RELAY3") == 0)
			{
				cmd_able[1] = 3;
			}
			else if(strcmp(StrPack[1],"RELAY4") == 0)
			{
				cmd_able[1] = 4;
			}
		}
		else
		{
			if(strcmp(StrPack[1],"BUZZER1") == 0)
			{
				cmd_able[1] = 1;
			}
		}
		if(strcmp(StrPack[1],"ALL") == 0)
		{
			cmd_able[1] = 0xff;
		}
		if(cmd_able[1] == 0)
		{
			UART_2SendString("ERROR:002\n");
			err = 0; 
			cmd_able[3] = 0;
		}
	}
	if(err != 0)
	{
		if(strcmp(StrPack[2],"ON") == 0)
		{
			cmd_able[2] = 1;
		}
		else if(strcmp(StrPack[2],"OFF") == 0)
		{
			cmd_able[2] = 2;
		}
		else
		{
			UART_2SendString("ERROR:003\n");
			err = 0; 
			cmd_able[3] = 0;
		}
	}
	return err;
}




 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
