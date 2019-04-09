#include "key.h"

uint8_t key_able[3]= {0, 27, 0};
uint8_t save_data = 0;
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

	if((P1&0x0f)^0x0f != 0 && last_keyval != (P1&0x0F) && keyflag)
		{
			keyflag = 0;
			switch(P1&0x0f)
			{
				case 0x0E:
					key_able[0]++;
					if(key_able[0] > 2)
						key_able[0] = 0;
					if(key_able[2] == 3)
						key_able[2] = 0;
					break;
				case 0x0D:
					key_able[1]++;
					if(key_able[1] > 30)
						key_able[1] = 16;
					break;	
				case 0x0B:
						key_able[1]--;
					if(key_able[1] < 16)
						key_able[1] = 30;
					break;	
				case 0x07:
					key_able[2]++;
					if(key_able[2] > 2)
						key_able[2] = 0;
					save_data = key_able[2];
					break;	
			}
		}
	if(P1 & 0x0F == 0X0F)
		keyflag = 1;
	 last_keyval = P1&0x0F;
	 
	return key_mode;

}	