#include "key.h"
u8 keystate[2]= {0,0} ;

void key_io_config(void)
{
	GPIO_InitTypeDef GPIO_InitTypeState;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	
	GPIO_InitTypeState.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitTypeState.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_Init(GPIOB, &GPIO_InitTypeState);
}

void Get_Key_Val(int * time)
{
	int static keyflag = 1;
	int static keytime = 0;
	int static keylock = 1;
	if(key1 == 0 || key2 == 0 || key3 == 0)
	{
		if( keyflag == 1)
		{
			keytime = 0;
			keyflag = 0;
			return;
		}
		if(keylock == 1)
		{
			if(key1 == 0)
			{
				key_val_switch(keystate[1], time, 1);
				keylock = 0;
			}
			if(key2 == 0)
			{
				keystate[1] ++;
				if(keystate[1] == 7)
				{
					DS3231_SetTime(time);
					keystate[1] = 0;
				}
				keylock = 0;
			}
			if(key3 == 0)
			{
				key_val_switch(keystate[1], time, 0);
				keylock = 0;
			}
		}
		if(key1 == 0)
		{
			keytime ++;
			if(keytime == 25)
			{
				keytime = 0;
				key_val_switch(keystate[1], time, 1);
			}
		}
		if(key2 == 0)
		{
			keytime ++;
			if(keytime == 100)
			{
				keytime = 0;
				keystate[1] ++;
				if(keystate[1] == 7)
				{
					keystate[1] = 0;
				}
			}
		}
		if(key3 == 0)
		{
			keytime ++;
			if(keytime == 25)
			{
				keytime = 0;
				key_val_switch(keystate[1], time, 0);
			}
		}
		paracorrect(time);
		
	}
	else if(key1 != 0 && key2 != 0 && key3 != 0) {keyflag = 1;keylock = 1;}
	
	if(keystate[1] != 0) keystate[0] = 0x01;
	else keystate[0] = 0x00;
}



void key_val_switch(u8 num, int * time, u8 mode)
{
	int val = 0;
	if(mode == 0)
	{
		val = -1;
	}
	else
	{
		val = 1;
	}
	switch(num)
	{
	case 1: time[6] += val; break; 
	case 2: time[5] += val; break;
	case 3: time[4] += val; break;
	case 4: time[2] += val; break;
	case 5: time[1] += val; break;
	case 6: time[0] += val; break;
	default :break;
	}
}

void paracorrect(int * time)
{
	if(time[6] >= 100) time[6] = 0;
	if(time[6] <= -1)  time[6] = 99;
	if(time[5] >= 13)  time[5] = 1;
	if(time[5] <= 0 )  time[5] = 12;
	if(time[4] >= 32)  time[4] = 1;
	if(time[4] <= 0 )  time[4] = 31;
	if(time[2] >=24)   time[2] = 0;
	if(time[2] <=-1)   time[2] = 23;
	if(time[1] >= 60)  time[1] = 0;
	if(time[1] <=-1)   time[1] = 59;
	if(time[0] >=60 )  time[0] = 0;
	if(time[0] <=-1)   time[0] = 59;

	
}









