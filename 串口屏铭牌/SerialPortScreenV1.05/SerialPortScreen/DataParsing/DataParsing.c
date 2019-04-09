#include "DataParsing.h"

#define COMMA_COUNT 3 																																//���ŵĸ���

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
	uint8_t _CheckIsOk = 0;																															//���÷���ֵ��Ĭ�ϼ���ʧ��
	uint8_t _CommaNum = 0;																															//��ȡ','�ĸ���
	uint8_t * _pStrCurrentAdder = str, * _pStrPreviousAdder = NULL;											//������ǰָ���������ǰָ�����
	while(1)
	{
		_pStrCurrentAdder = (uint8_t *)strchr((const char *)_pStrCurrentAdder, ',');			//��ȡ��һ��','����_pStrCurrentAdder�еĵ�ַ,û���ҵ���ʾNULL
		if(_pStrCurrentAdder != _pStrPreviousAdder && _pStrCurrentAdder != NULL)					//�жϵ�ǰ��֮ǰ��ͬ
		{
			_CommaNum ++;																																		//','����++
			_pStrPreviousAdder = _pStrCurrentAdder;																					//����ǰָ��������ݸ�ֵ��֮ǰ��ָ�����
			_pStrCurrentAdder++;																														//��ǰָ�������ַ++
		}
		else if(_CommaNum < COMMA_COUNT)																									//��֮ǰָ������ĵ�ַ���ڵ�ǰָ�������ַ && ','�ĸ���С��У������˳�ѭ��
		{
			break;
		}
		
		if(_CommaNum == COMMA_COUNT && _pStrCurrentAdder == NULL)													//�жϵ����Ÿ����Ƿ���ں궨���趨ֵ && ��ǰָ���������ΪNULL
		{
			_CheckIsOk = 1;																																	//���ü���ɹ�
			break;
		}
		else if(_CommaNum > COMMA_COUNT)																									//������Ÿ���>�趨ֵ�˳�
		{
			break;
		}
	}
		if((_CheckIsOk != 0) && (NULL == (uint8_t *)strchr((const char *)str, ';')))			//��_CheckIsOkΪ����ʱ && ����Str������';' == NULL
		{
			_CheckIsOk = 0;																																	//����У��ʧ��		
		}
		return _CheckIsOk;																																//����_CheckIsOk��״̬
}


