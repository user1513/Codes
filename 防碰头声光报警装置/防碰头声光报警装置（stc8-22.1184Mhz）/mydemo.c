#include "mydemo.h"


#define VARMAX 5

typedef xdata struct MYFUNC
{
	u8 Reg;//注冊
	_bool *FlagName;//標志名稱
	u32 DelayTime;//延時時間
	_bool  FlagState;//標準狀態
	
}MyFunc;


MyFunc FlagStateFunc[VARMAX];//定義可以同時進行10位

_bool FuncReg(_bool *_FlagName,u32 _DelayTime,_bool _FlagState)//變量指針，延時時間，標準狀態
{
	_bool flag=FALSE;
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

void CleanCallbackFunc(_bool *_FlagName,_bool _FlagState)
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