/**
 * �����ʱ����
 * ��Դ��STC �ٷ�������
 * �����£�2014-07-18
 */

#include	"delay.h"


/**
 * void  Delay_ms(uint8_t ms)
 * ��飺1ms ��ʱ�������Զ���Ӧ��ʱ��
 * ���룺ms: Ҫ��ʱ�� ms ��
 * �������
 */
void Delay_ms(uint8_t ms)
{
	uint16_t i;
	do
	{
		i = MAIN_Fosc / 13000;
		while(--i)	;   //14T per loop
	}while(--ms);
}

void Delay5us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 9;
	while (--i);
}