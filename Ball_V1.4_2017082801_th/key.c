#include "key.h"

void Delay10ms(void)		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}

uchar KeyScan(void)
{
	uchar keyValue = 0;
	if(KEY_A == 0)
	{
		Delay10ms();
		if(KEY_A == 0)
		{
			keyValue = 1;
			while(!KEY_A);
		}
	}
	if(KEY_B == 0)
	{
		Delay10ms();
		if(KEY_B == 0)
		{
			keyValue = 2;
			while(!KEY_B);
		}
	}
	return keyValue;
}