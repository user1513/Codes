#include "timer0.h"


void Timer0Init(void)		//1����@24.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x9A;		//���ö�ʱ��ֵ
	TH0 = 0xA9;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
    ET0 = 1;                                    //ʹ�ܶ�ʱ���ж�
    EA = 1;
}



u16 Time0_500ms=0;
void Timer0() interrupt 1
{
	TL0 = 0x9A;		//���ö�ʱ��ֵ
	TH0 = 0xA9;		//���ö�ʱ��ֵ
	HAL_MYU1680U_Callback();////MY1680U�жϻص�����	
	FuncTimeTravers();
	if(++Time0_500ms==500)
	{
		Time0_500ms=0;
		LEDPORT=~LEDPORT;		
	}
	TF0=0;
}

