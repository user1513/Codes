#include "mydemo.h"


#define VARMAX 5

typedef xdata struct MYFUNC
{
	u8 Reg;//ע��
	_bool *FlagName;//��־���Q
	u32 DelayTime;//�ӕr�r�g
	_bool  FlagState;//�˜ʠ�B
	
}MyFunc;


MyFunc FlagStateFunc[VARMAX];//���x����ͬ�r�M��10λ

_bool FuncReg(_bool *_FlagName,u32 _DelayTime,_bool _FlagState)//׃��ָᘣ��ӕr�r�g���˜ʠ�B
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


void FuncTimeTravers(void)//��ʱ���ص�����
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