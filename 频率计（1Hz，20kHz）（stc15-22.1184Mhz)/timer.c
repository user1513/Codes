#include "timer.h"
extern unsigned int delay_time;



void Timer2Init(void)		//250΢��@11.0592MHz
{
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xD7;		//���ö�ʱ��ֵ
	T2H = 0xFD;		//���ö�ʱ��ֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	IE2 |= 0x04;                    //����ʱ��2�ж�
    EA = 1;
}


//-----------------------------------------------
//�жϷ������
void t2int() interrupt 12           //�ж����
{
	static int i = 0 , t = 0;
	
	T2L = 0xD7;		//���ö�ʱ��ֵ
	T2H = 0xFD;		//���ö�ʱ��ֵ
	
	t = 20000 / delay_time ;
	if( ++i >= t) 
	{
		i = 0;
		P32 = ~P32;
		P34 = ~P34;
	}
		
	
			
}

