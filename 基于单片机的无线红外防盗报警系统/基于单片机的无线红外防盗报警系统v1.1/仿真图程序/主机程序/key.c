#include "key.h"


void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 9;
	j = 244;
	do
	{
		while (--j);
	} while (--i);
}


uint8_t kscanf(void)
{		
  static uint8_t key_mode = 1; 
	static last_keyval = 1, keyflag = 1;

	if(key1 == 0 && last_keyval == 0 && keyflag)
		{
			keyflag = 0;
			key_mode ^= 0X01;
		}
	if(key1 != 0)
		keyflag = 1;
	 last_keyval = key1;
	 
	return key_mode;

}	