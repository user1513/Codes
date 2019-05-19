#include "timer1.h"

bit  UsartFlag=0;

//65536-24/1000

void Timer0Init(void)		//1����@24.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x40;		//���ö�ʱ��ֵ
	TH0 = 0xA2;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
    ET0 = 1;                                    //ʹ�ܶ�ʱ���ж�
    EA = 1;
}




u16 Time1_50ms=0;
u16 Time1_500ms=0;
void Timer1() interrupt 1
{
	TL0 = 0x40;		//���ö�ʱ��ֵ
	TH0 = 0xA2;		//���ö�ʱ��ֵ
	
	if(++Time1_50ms==50)
	{
		Time1_50ms=0;
		UsartFlag=Enable;
	}
	if(++Time1_500ms==500)
	{
		Time1_500ms=0;
		P34=~P34;		
	}
	TF0=0;
}
