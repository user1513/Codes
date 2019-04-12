#include "app.h"
#include "usart.h"
#include "timer.h"
#include "stdio.h"
#include "led.h"
#include "Bluetooth.h"
#include "string.h"
uint8_t xdata Uart2_men_able[UART2DATALENGTHMAX] = {0};

extern uint8_t GetUart2Data_Length;
extern uint8_t TimeOut_status;
extern uint8_t xdata StrPack[3][30];
void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 1;
	k = 243;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main(void)
{
	uint8_t temp = 0;
	Timer0Init();
	Uart1Init();
	Uart2Init();

	while(1)
	{
		GetUart2DataTimeout(Uart2_men_able);							//����2���ճ�ʱ����
		if(TimeOut_status & 0x04)
		{
			TimeOut_status &= ~0x04;
			strupr(Uart2_men_able) ;									//���ݴ�д���루��⵽Сдͳһת���ɴ�д���к���������
			Data_Parsing_Func(Uart2_men_able);
			Peripheral_Handler();
			memset(Uart2_men_able, 0, UART2DATALENGTHMAX);
		}
	}
}
