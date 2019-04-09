#include "timer.h"

extern const uint8_t code HZ_tab[][16];

extern uint8_t save_data;

extern uint8_t key_able[];

uint8_t motor_count = 0;
/*******************************************************************************
* 函 数 名         : Timer0Init
* 函数功能		   : 定时器0初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Timer0Init(void)
{
	TMOD|=0X01; //	选择为定时器0模式，工作方式1，仅用TR0打开启动。
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	ET0=1;      //打开定时器0中断允许
	EA=1;       //打开总中断
	TR0=1;      //打开定时器			
}

	uint8_t * _pDs18b20 = (uint8_t *)0;
	uint8_t key_val = 0;

	uint8_t pwm_val = 0;

float temp_val = 0;
void Timer0() interrupt 1
{
	static uint8_t delay_10ms = 0;
	int i = 0;
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	delay_10ms ++;
	kscanf();
	if(delay_10ms == 50)
	{	
		delay_10ms = 0;
		
		temp_val = ReadTemperature();
		
		_pDs18b20 = Ds18b20_datapros(temp_val);	 //数据处理函数
		
		LCD_ShowString_length(4, 9, _pDs18b20 ,4);
		
		LCD_ShowNumber(2, 9, key_able[1], 2);
		
		switch(key_able[0])
		{
			case 0: 	LCD_ShowChinaLanguage(0 ,5 , &HZ_tab[48][0], 2);
			P1 &= ~0x10;P1 |= 0x20;P1 |= 0x40;
				break;
			case 1: 	LCD_ShowChinaLanguage(0 ,5 , &HZ_tab[40][0], 2);
			if(key_able[1] > temp_val)
				key_able[2] = 3;
			else
				key_able[2] = save_data;
			P1 &= ~0x20;P1 |= 0x10;P1 |= 0x40;			
				break;
			case 2: 	LCD_ShowChinaLanguage(0 ,5 , &HZ_tab[44][0], 2);
			if(key_able[1] < temp_val)
				key_able[2] = 3;
			else
				key_able[2] = save_data;
			P1 &= ~0x40;P1 |= 0x10;P1 |= 0x20;			
			break;
			default:key_able[0] = 0; break;
		}
		switch(key_able[2])
		{
			case 0: 	LCD_ShowChinaLanguage(6 ,5 , &HZ_tab[60][0], 2);
			motor_count = 2;
				break;
			case 1: 	LCD_ShowChinaLanguage(6 ,5 , &HZ_tab[56][0], 2);
			motor_count = 5;			
			break;
			case 2: 	LCD_ShowChinaLanguage(6 ,5 , &HZ_tab[52][0], 2);
			motor_count = 45;
			break;
			case 3: 	LCD_ShowChinaLanguage(6 ,5 , &HZ_tab[64][0], 2);
			motor_count = 0;
			break;
			default:key_able[3] = 0; break;
		}
		
	}
	if(pwm_val++ < motor_count)
	{
		MOTOR_Bit = 0;
	}
	else if(pwm_val > 50)
	{
		pwm_val = 0;
	}
	else
	{
		MOTOR_Bit = 1;
	}
	
	
	
}