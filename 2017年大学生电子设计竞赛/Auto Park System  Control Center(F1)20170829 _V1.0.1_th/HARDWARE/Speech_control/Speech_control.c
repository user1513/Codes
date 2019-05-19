#include "Speech_control.h"
#include "usart3.h"	  


void Speech_SwitchGarage(u8 number)
{
	USART3Send(0x44);
	USART3Send(number);
	USART3Send(0x00);
	USART3Send(0x00);
	USART3Send(0x00);
	USART3Send(number);
	USART3Send(0xBB);
}

void Speech_Switch_Go(u8 number)
{
	USART3Send(0x44);
	USART3Send(0x10+number);
	USART3Send(0x00);
	USART3Send(0x00);
	USART3Send(0x00);
	USART3Send(0x10+number);
	USART3Send(0xBB);
}

void Speech_Switch_home(u8 time,u8 money,u8 collision)
{
	USART3Send(0x44);
	USART3Send(0x55);
	USART3Send(time);
	USART3Send(money);
	USART3Send(collision);
	USART3Send(0x55+time+money+collision);
	USART3Send(0xBB);
}

void Speech_null(void)
{
	USART3Send(0x44);
	USART3Send(0x00);
	USART3Send(0x00);
	USART3Send(0x00);
	USART3Send(0x00);
	USART3Send(0x00);
	USART3Send(0xBB);	
}

void Speech_Reset(void)
{
	USART3Send(0x44);
	USART3Send(0x17);
	USART3Send(0x00);
	USART3Send(0x00);
	USART3Send(0x00);
	USART3Send(0x17);
	USART3Send(0xBB);	
}
