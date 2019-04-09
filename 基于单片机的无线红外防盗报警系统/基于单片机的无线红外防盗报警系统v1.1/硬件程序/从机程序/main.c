#include "app.h"
#include "NRF24L01.h"
#include "usart.h"
#include "timer.h"


uint8_t flag_hw = 0;
uint32_t STATE_FLAG = 0; //状态标志位
uint8_t delay_hw = 0;

void main()
{
	uint8_t data_ = 0, temp = 0;
	UartInit();
	//NRF24L01初始化
	init_io();		           //无线初始化IO
	RX_Mode();		           //设置为接收模式
	Timer0Init();
	while(1)
	{
		if(STATE_FLAG & (1 << 0 ))
		{
			STATE_FLAG &= ~(1 << 0);
			if(P16 == 1)
			{
				delay_hw = 20;
				flag_hw = 1;
			}
			else if(delay_hw != 0)
			{
				delay_hw --;
			}
			else
			{
				flag_hw = 0;
			}
		}
			
		NRF24L01_Send();	
		
	}
}