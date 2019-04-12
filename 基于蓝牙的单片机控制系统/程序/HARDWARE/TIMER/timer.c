#include "timer.h"
#include "usart.h"
extern uint8_t TimeOut_status;

#define UART2TIMEOUTMAX   100 //����2���ճ�ʱ���Ĭ��10ms
void Timer0Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xD4;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	IE |=0X82;
}

void tim0() interrupt 1
{
	static uint8_t UartTimeOutVal = 0; 
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xD4;		//���ö�ʱ��ֵ
	
	if(TimeOut_status & 0x01)
	{
		UartTimeOutVal++;
		if(UartTimeOutVal > UART2TIMEOUTMAX)
		{
			TimeOut_status |= 0x02;
			UartTimeOutVal= 0;
		}
	}
	else
	{
		UartTimeOutVal= 0;
	}
}

