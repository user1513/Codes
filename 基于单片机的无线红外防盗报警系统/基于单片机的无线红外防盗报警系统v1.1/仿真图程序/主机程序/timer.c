#include "timer.h"

extern const uint8_t code HZ_tab[][16];
/*******************************************************************************
* 函 数 名         : Timer0Init
* 函数功能		   : 定时器0初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Timer0Init(void)
{
	TMOD|=0X01; //选择为定时器0模式，工作方式1，仅用TR0打开启动。
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	ET0=1;      //打开定时器0中断允许
	EA=1;       //打开总中断
	TR0=1;      //打开定时器			
}

	uint8_t * _pDs18b20 = (uint8_t *)0;
	uint8_t * _pDs1302 = (uint8_t *)0;
	uint8_t key_val = 0;
void Timer0() interrupt 1
{
	static uint8_t delay_10ms = 0;
	int i = 0;
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	delay_10ms ++;
	key_val = kscanf();
		if(key_val && delay_10ms == 50)
		{
			LCD_ShowChinaLanguage(6 ,3 , &HZ_tab[76][0], 2);
		}
		else if(delay_10ms == 50)
		{
			LCD_ShowChinaLanguage(6 ,3 , &HZ_tab[80][0], 2);
		}
	if(delay_10ms == 50)
	{	
		delay_10ms = 0;
		_pDs18b20 = Ds18b20_datapros(ReadTemperature());	 //数据处理函数

		LED_ShowString_length(4, 7, _pDs18b20 ,4);
		
		_pDs1302 = Ds1302_datapros();	 //数据处理函数
		LCD_ShowNumber(0, 7, _pDs1302[0], 2);
		LCD_ShowChar(0, 9, '-');
		LCD_ShowNumber(0, 10, _pDs1302[2], 2);
		LCD_ShowChar(0, 12, '-');
		LCD_ShowNumber(0, 13, _pDs1302[3], 2);
		LCD_ShowNumber(2, 7, _pDs1302[4], 2);
		LCD_ShowChar(2, 9, '-');
		LCD_ShowNumber(2, 10, _pDs1302[5], 2);
		LCD_ShowChar(2, 12, '-');
		LCD_ShowNumber(2, 13, _pDs1302[6], 2);
	}
	
	
	
}