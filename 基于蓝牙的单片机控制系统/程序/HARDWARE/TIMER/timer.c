#include "timer.h"

void Timer0Init(void)		//10����@12.000MHz
{
    AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
    TMOD &= 0xF0;		//���ö�ʱ��ģʽ
    TMOD |= 0x01;		//���ö�ʱ��ģʽ
    TL0 = 0xF0;		//���ö�ʱ��ֵ
    TH0 = 0xD8;		//���ö�ʱ��ֵ
    TF0 = 0;		//���TF0��־
    TR0 = 1;		//��ʱ��0��ʼ��ʱ
    IE |=0X82;
}

void tim0() interrupt 1
{

    TL0 = 0xF0;		//���ö�ʱ��ֵ
    TH0 = 0xD8;		//���ö�ʱ��ֵ

}

