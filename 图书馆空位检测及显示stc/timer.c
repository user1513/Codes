#include "timer.h"


/* define constants */
#define FOSC 11059200L

#define T1MS (65536-FOSC/12/1000)   //1ms timer calculation method in 12T mode


#define VARMAX 4

typedef xdata struct MYFUNC
{
	u8 Reg;//注冊
	_bool * FlagName;//標志名稱
	u32 DelayTime;//延時時間
	_bool  FlagState;//標準狀態
	
}MyFunc;

MyFunc FlagStateFunc[VARMAX];//定義可以同時進行10位

_bool FuncReg(_bool *_FlagName,u32 _DelayTime,_bool _FlagState)//變量指針，延時時間，標準狀態
{
	_bool flag=FLASE;
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


void FuncTimeTravers(void)
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

void Timer0Init(void)		//1毫秒@11.0592MHz
{
	TMOD |= 0x01;                    //set timer0 as mode1 (16-bit)
    TL0 = T1MS;                     //initial timer0 low byte
    TH0 = T1MS >> 8;                //initial timer0 high byte
    TR0 = 1;                        //timer0 start running
    ET0 = 1;                        //enable timer0 interrupt
    EA = 1;                         //open global interrupt switch
}
u16 count=500;

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1 using 1
{
    TL0 = T1MS;                     //reload timer0 low byte
    TH0 = T1MS >> 8;                //reload timer0 high byte
    FuncTimeTravers();
	if (count-- == 0)               //1ms * 1000 -> 1s
    {
        count = 500;               //reset counter   
		TEST_LED = ! TEST_LED;      //work LED flash
    }
}