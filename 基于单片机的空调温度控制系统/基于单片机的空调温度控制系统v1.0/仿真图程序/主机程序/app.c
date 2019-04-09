#include "app.h"   //调用app.h

extern const uint8_t code HZ_tab[][16]; //__ASCLLFONT_H 字库调用


int main(void)
{
	int i = 0;
	Init_DS18B20();  //初始化DS18B20温度传感器
	InitLcd();		 //初始化LCD12864液晶屏
	
	LCD_ShowChinaLanguage(0 ,0 , &HZ_tab[0][0], 5);  //LCD12864液晶屏第一行初显示"运行模式:"
	LCD_ShowChinaLanguage(2 ,0 , &HZ_tab[10][0], 5); //LCD12864液晶屏第二行初显示"设置温度:"
	LCD_ShowChinaLanguage(2 ,7 , &HZ_tab[68][0], 1); //LCD12864液晶屏第二行末显示"℃"
	LCD_ShowChinaLanguage(4 ,0 , &HZ_tab[20][0], 5); //LCD12864液晶屏第三行显示"实际温度:"
	LCD_ShowChinaLanguage(4 ,7 , &HZ_tab[68][0], 1); //LCD12864液晶屏第三行末显示"℃"
	LCD_ShowChinaLanguage(6 ,0 , &HZ_tab[30][0], 5); //LCD12864液晶屏第四行显示"当前风速:"
		
	Timer0Init();	//定时器0初始化，配置为1ms中断
	while(1)		//主循环
	{
			
		;	
	}
}