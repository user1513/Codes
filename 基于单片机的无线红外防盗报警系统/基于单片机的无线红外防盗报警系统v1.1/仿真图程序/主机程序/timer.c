#include "timer.h"

extern const uint8_t code HZ_tab[][16];
/*******************************************************************************
* �� �� ��         : Timer0Init
* ��������		   : ��ʱ��0��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Timer0Init(void)
{
	TMOD|=0X01; //ѡ��Ϊ��ʱ��0ģʽ��������ʽ1������TR0��������
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xDC;		//���ö�ʱ��ֵ
	ET0=1;      //�򿪶�ʱ��0�ж�����
	EA=1;       //�����ж�
	TR0=1;      //�򿪶�ʱ��			
}

	uint8_t * _pDs18b20 = (uint8_t *)0;
	uint8_t * _pDs1302 = (uint8_t *)0;
	uint8_t key_val = 0;
void Timer0() interrupt 1
{
	static uint8_t delay_10ms = 0;
	int i = 0;
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xDC;		//���ö�ʱ��ֵ
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
		_pDs18b20 = Ds18b20_datapros(ReadTemperature());	 //���ݴ�����

		LED_ShowString_length(4, 7, _pDs18b20 ,4);
		
		_pDs1302 = Ds1302_datapros();	 //���ݴ�����
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