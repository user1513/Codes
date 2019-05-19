#include "sys.h"
#include "led.h"
#include "delay.h"
#include "usart2.h"
#include "timer.h"
#include "lcd12864.h"

int main(void)
{
	int i = 0;
	LED_Init();
	delay_init();
	Lcd12864init();
	delay_ms(1000);
	Lcd12864Display(0x80,"STM32");Lcd12864Display(0x83,"12864");Lcd12864Display(0x86,"3.3V"); 
	
	Lcd12864Display(0x90,"陕西");Lcd12864Display(0x93,"西安");
	
	Lcd12864Display(0x88,"2014");Lcd12864Display(0x8A,"0424");
		
	Lcd12864Display(0x98,"测试");Lcd12864Display(0x9A,"程序");	
	while(1)
	{
		LCD12864SendCmd(0x9c);
		i %=10;
		LCD12864SendDate('0'+i);
		LED0_H();
		delay_ms(1000);
		LED0_L();
		delay_ms(1000);
		i++;
	}
}
