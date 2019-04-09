#include "app.h"

extern const uint8_t code HZ_tab[][16];

sbit buzzer = P3^7; 
sbit key = P2^7;
sbit LED = P3^6;

uint8_t flag = 0;
uint8_t able[2] = {0};

extern uint8_t key_val;

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


int main(void)
{
	int i = 0;
	Ds1302Init();
	buzzer = 1;
	LED = 1;
	Init_DS18B20();
	InitLcd(); 
	LCD_ShowChinaLanguage(0 ,0 , &HZ_tab[0][0], 8);
	LCD_ShowChinaLanguage(2 ,0 , &HZ_tab[16][0], 8);
	LCD_ShowChinaLanguage(4 ,0 , &HZ_tab[32][0], 6); 
	LCD_ShowChinaLanguage(6 ,0 , &HZ_tab[44][0], 3);
	LED_ShowString(6, 6, "150401211");
	
	while(i++ < DISPLAY_TIME)
		Delay1000ms();
	SetClear();
	
	LCD_ShowChinaLanguage(0 ,0 , &HZ_tab[50][0], 3);
	LCD_ShowChinaLanguage(2 ,0 , &HZ_tab[56][0], 3);
	LCD_ShowChinaLanguage(4 ,0 , &HZ_tab[62][0], 3);
	LCD_ShowChinaLanguage(4 ,7 , &HZ_tab[68][0], 1);
	LCD_ShowChinaLanguage(6 ,0 , &HZ_tab[70][0], 5);
	
	Timer0Init();
	uart_init();
	while(1)
	{
			
			if(flag == 1) // 如果收到数据 
		{ 
				ES = 0; //关闭串口中断

				flag = 0; //标志位清零
				for(i = 0; i < 2; i++)
			{
				SBUF = *(able + i); //将收到的数据发送至缓冲区 

				while(!TI); //等待数据发送结束

				TI=0; // 将发送结束标志清零
			}
			if(able[0] == 'O' && able[1] == 'K'&&key_val == 1) {buzzer = 0; LED = 0;}
			else if(able[0] == 'N' && able[1] == 'O'){buzzer = 1; LED = 1;}
				ES=1; // 使能串口中断
		}
	}
}