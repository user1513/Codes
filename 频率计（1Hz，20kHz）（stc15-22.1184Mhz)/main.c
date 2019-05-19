#include "main.h"

unsigned int delay_time = 1000;
extern u8 able[];
unsigned int eepdelay_time = 0;

void Delay100us()		//@11.0592MHz
{
	unsigned char i, j;

	i = 2;
	j = 15;
	do
	{
		while (--j);
	} while (--i);
}
void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 43;
	j = 6;
	k = 203;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main()
{
	int num = 0x55;
	Usart_Init();
	
	Timer2Init();
	if(IapReadByte(IAP_ADDRESSRST) != 0x01)
	{
		IapEraseSector(IAP_ADDRESSRST);
		IapProgramByte(IAP_ADDRESSRST, 0x01);
		IapProgramByte(IAP_ADDRESSH, delay_time>>8);
		IapProgramByte(IAP_ADDRESSL, delay_time);
	}
	else
	{
		delay_time = (IapReadByte(IAP_ADDRESSH) << 8) + IapReadByte(IAP_ADDRESSL);
	}
	while(1)
	{
		Get_Usart_array();
		if(0xFF == able[5])
		{
			able[5] = 0x00;
			delay_time =  (able[2]<<8) + able[3];
			IapProgramByte(IAP_ADDRESSH,0xFF);
			Delay100us();
			IapProgramByte(IAP_ADDRESSL,0xFF);
			Delay100us();
			IapProgramByte(IAP_ADDRESSH,able[2]);
			Delay100us();
			IapProgramByte(IAP_ADDRESSL,able[3]);
			Delay100us();
			UserSendChar(IapReadByte(IAP_ADDRESSRST));
			Delay100us();
			UserSendChar(IapReadByte(IAP_ADDRESSH));
			Delay100us();
			UserSendChar(IapReadByte(IAP_ADDRESSL));
			Delay100us();
			UserSendString(able, 5);
		}
	}
}