#include "app.h"   //����app.h

extern const uint8_t code HZ_tab[][16]; //__ASCLLFONT_H �ֿ����


int main(void)
{
	int i = 0;
	Init_DS18B20();  //��ʼ��DS18B20�¶ȴ�����
	InitLcd();		 //��ʼ��LCD12864Һ����
	
	LCD_ShowChinaLanguage(0 ,0 , &HZ_tab[0][0], 5);  //LCD12864Һ������һ�г���ʾ"����ģʽ:"
	LCD_ShowChinaLanguage(2 ,0 , &HZ_tab[10][0], 5); //LCD12864Һ�����ڶ��г���ʾ"�����¶�:"
	LCD_ShowChinaLanguage(2 ,7 , &HZ_tab[68][0], 1); //LCD12864Һ�����ڶ���ĩ��ʾ"��"
	LCD_ShowChinaLanguage(4 ,0 , &HZ_tab[20][0], 5); //LCD12864Һ������������ʾ"ʵ���¶�:"
	LCD_ShowChinaLanguage(4 ,7 , &HZ_tab[68][0], 1); //LCD12864Һ����������ĩ��ʾ"��"
	LCD_ShowChinaLanguage(6 ,0 , &HZ_tab[30][0], 5); //LCD12864Һ������������ʾ"��ǰ����:"
		
	Timer0Init();	//��ʱ��0��ʼ��������Ϊ1ms�ж�
	while(1)		//��ѭ��
	{
			
		;	
	}
}