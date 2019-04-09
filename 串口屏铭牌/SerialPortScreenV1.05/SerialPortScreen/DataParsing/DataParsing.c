#include "DataParsing.h"

#define COMMA_COUNT 3 																																//逗号的个数

#define DATAPACK_LENGTH (COMMA_COUNT + 1)

uint8_t StrPack[DATAPACK_LENGTH][40] = {{0},{0},{0}};

uint8_t Data_Parsing_Func(uint8_t * str)
{
	uint8_t i = 0;
	uint8_t _StateVal = 0;
	uint8_t * _pStrAdder = str;
	_StateVal = DataPage_Check(str);
	if(_StateVal != 0)
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
	}
	return _StateVal;
}

uint8_t DataPage_Check(uint8_t * str)
{
	uint8_t _CheckIsOk = 0;																															//设置返回值，默认检验失败
	uint8_t _CommaNum = 0;																															//获取','的个数
	uint8_t * _pStrCurrentAdder = str, * _pStrPreviousAdder = NULL;											//创建当前指针变量与先前指针变量
	while(1)
	{
		_pStrCurrentAdder = (uint8_t *)strchr((const char *)_pStrCurrentAdder, ',');			//获取第一个','所在_pStrCurrentAdder中的地址,没有找到显示NULL
		if(_pStrCurrentAdder != _pStrPreviousAdder && _pStrCurrentAdder != NULL)					//判断当前与之前不同
		{
			_CommaNum ++;																																		//','个数++
			_pStrPreviousAdder = _pStrCurrentAdder;																					//将当前指针变量内容赋值给之前的指针变量
			_pStrCurrentAdder++;																														//当前指针变量地址++
		}
		else if(_CommaNum < COMMA_COUNT)																									//当之前指针变量的地址等于当前指针变量地址 && ','的个数小于校验个数退出循环
		{
			break;
		}
		
		if(_CommaNum == COMMA_COUNT && _pStrCurrentAdder == NULL)													//判断当逗号个数是否等于宏定义设定值 && 当前指针变量内容为NULL
		{
			_CheckIsOk = 1;																																	//设置检验成功
			break;
		}
		else if(_CommaNum > COMMA_COUNT)																									//如果逗号个数>设定值退出
		{
			break;
		}
	}
		if((_CheckIsOk != 0) && (NULL == (uint8_t *)strchr((const char *)str, ';')))			//当_CheckIsOk为非零时 && 查找Str数组中';' == NULL
		{
			_CheckIsOk = 0;																																	//设置校验失败		
		}
		return _CheckIsOk;																																//返回_CheckIsOk的状态
}


