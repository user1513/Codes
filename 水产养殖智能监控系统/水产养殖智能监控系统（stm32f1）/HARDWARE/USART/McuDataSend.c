#include "McuDataSend.h"


#define VARMAX 10

typedef struct MYFUNC
{
	u8 Reg;//注冊
	bool *FlagName;//標志名稱
	u32 DelayTime;//延時時間
	bool  FlagState;//標準狀態
	
}MyFunc;


MyFunc FlagStateFunc[VARMAX];//定義可以同時進行10位

bool FuncReg(bool *_FlagName,u32 _DelayTime,bool _FlagState)//變量指針，延時時間，標準狀態
{
	bool flag=FALSE;
	int i=0;
	for(i=0;i<VARMAX;i++)
	{
		if(FlagStateFunc[i].FlagName==_FlagName&&FlagStateFunc[i].Reg==0x01)
		{
			FlagStateFunc[i].DelayTime=_DelayTime;
			FlagStateFunc[i].FlagState=_FlagState;
			return flag=TRUE;
		}
	}
	for(i = 0;i < VARMAX;i++)
	{
		if(FlagStateFunc[i].Reg!=0x01)
		{
			FlagStateFunc[i].Reg=0x01;
			FlagStateFunc[i].DelayTime=_DelayTime;
			FlagStateFunc[i].FlagState=_FlagState;
			FlagStateFunc[i].FlagName=_FlagName;
			flag=TRUE;
			break;
		}
	}
	return flag;
}


void FuncTimeTravers(void)//定时器回调函数
{
	int i = 0;
	for(i = 0; i < VARMAX; i++)
	{
		if(FlagStateFunc[i].Reg==0x01)
		{
			FlagStateFunc[i].DelayTime--;
			if(FlagStateFunc[i].DelayTime==0)
			{
				FlagStateFunc[i].Reg=0x00;
				FlagStateFunc[i].DelayTime=0;
				*FlagStateFunc[i].FlagName=FlagStateFunc[i].FlagState;
			}
		}
	}
}

void CleanCallbackFunc(bool *_FlagName,bool _FlagState)
{
	int i=0;
	for(i=0;i<VARMAX;i++)
	{
		if(FlagStateFunc[i].FlagName==_FlagName)
		{
			FlagStateFunc[i].Reg=0x00;
			*FlagStateFunc[i].FlagName=_FlagState;
		}
	}
}


String IntTostring(u32 _data)//将int类型转换成string
{
	u8 *ptemp=NULL;
	u8 length = 0,length_temp;
	u8 i=0;
	length = GetIntegerlength(_data);
	ptemp=(u8 *)malloc(sizeof(u8)*length + 2);
	length_temp = length - 1;
	for(i = 0;i < length; i++ , length_temp--)
	{
		*(ptemp + i) =(u8)(_data / pow(10,length_temp)+'0');
		_data %=(u32) pow(10,length_temp);
	}
	*(ptemp + i + 1)='\0';	
	return ptemp;
}

u8 GetIntegerlength(u32 _data)//获取int类型数据的长度
{
	u8 temp = 0;
	u8 i = 0;
	for(i = 0;;i ++)
	{
		_data/=10;
		if(_data==0)
			break;
	}
	temp=i + 1;
	return temp;
}

int GetDecimalLength(double _data)//获取浮点数长度，默认保留二位
{
	int num = (int)(_data * 100);
	u8 length = 0;
	length = GetIntegerlength((u32)num);
	return length;
}

void InversionFunc(String string, u8 _length)//函数倒置
{
	int i = 0,j=_length;
	int temp = 0;
	while (i<j)
	{
		temp = *(string + i);
		*(string + i) = *(string + j);
		*(string + j) = temp;
		i++;
		j--;

	}
}

String DecimalToStringSymbol(double _data)//不反回正负符号->string
{
	int num = 0, i = 0,temp=0;
	String Newstring = NULL;
	String string = NULL;
	if(_data !=0)
	{
		string = (String)malloc(sizeof(u8)*(GetDecimalLength(_data) + 2));
		Newstring = string;
		if (_data < 0)
		{
			_data = -_data;
		}
		num = (int)_data;
		for (i = 0; num != 0; i++)
		{
			temp = num % 10;
			*(string + i) = temp+'0';
			num /= 10;
		}
		InversionFunc(Newstring, i - 1);//减2  because：前面for结束循环前i++，并且我们得到的实际长度要减去一（从0开始）
		num = (int)_data;
		_data -= num;
		num = _data * 100;
		*(string + i) ='.';
		*(string + i+1) = num / 10+'0';
		*(string + i+2) = num % 10+'0';
		*(string + i+3) = '\0';
	}
	else
	{
		string = (String)malloc(sizeof(u8)*5);
		Newstring = string;
		*(string+0) = '0';
		*(string+1) = '.';
		*(string+2) = '0';
		*(string+3) = '0';
		*(string+4) = '\0';
	}
	return Newstring;
}

//默认保存小数点后两位
String DecimalToString(double _data)//浮点数->string
{
	int num = 0, i = 0,temp=0;
	String Newstring = NULL;
	String string = NULL;
	if(_data !=0)
	{
		string = (String)malloc(sizeof(u8)*(GetDecimalLength(_data) + 3));
		Newstring = string;
		if (_data < 0)
		{
			*string = '-';
			_data = -_data;
		}
		else
		{
			*string = '+';
		}
		num = (int)_data;
		for (i = 1; num != 0; i++)
		{
			temp = num % 10;
			*(string + i) = temp+'0';
			num /= 10;
		}
		InversionFunc(Newstring+1, i - 2);//减2  because：前面for结束循环前i++，并且我们得到的实际长度要减去一（从0开始）
		num = (int)_data;
		_data -= num;
		num = _data * 100;
		*(string + i) ='.';
		*(string + i+1) = num / 10+'0';
		*(string + i+2) = num % 10+'0';
		*(string + i+3) = '\0';
	}
	else
	{
		string = (String)malloc(sizeof(u8)*6);
		Newstring = string;
		*(string) = ' ';
		*(string+1) = '0';
		*(string+2) = '.';
		*(string+3) = '0';
		*(string+4) = '0';
		*(string+5) = '\0';
	}
	return Newstring;
}

