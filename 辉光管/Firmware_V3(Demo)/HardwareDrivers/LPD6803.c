/**
 * LPD6803 ����
 * оƬ��飺
 *     3 · LED ����оƬ
 * ��Դ���������� LPD6803 �����ֲ�
 * �����£�2014-07-18
 * http://blanboom.org
 *********************************************
 * ʹ�ñ�����ʱ����Ҫ�� PinDef.h ��
 * ���� HV57708 �ĸ������ţ�������ο� LPD6803.h
 *********************************************
 * ������ֻ�ʺϵ�Ƭ LPD6803�����ʺ϶�ƬоƬ���������
 */
 
#include "LPD6803.h"

#include "PinDef.h"

/**
 * void LPD6803_LEDLightUp(uint8_t dr, uint8_t dg, uint8_t db)
 * ��飺��ָ���Ĳ������� LED
 * ���ܣ��� LPD6803 ������· LED ���������ݣ������� LED
 * ���룺
 *       dr, dg, db �ֱ�Ϊ��· LED ���������ݣ���Χ�� 0 ~ 31
 * �������
 */
void LPD6803_LEDLightUp(uint8_t dg, uint8_t dr, uint8_t db)
{
	uint8_t j;
	uint16_t mask;
	
	// first output 32 start frame
	LPD6803_DCLK = 0;
	LPD6803_DIN = 0;
	
	for(j = 0; j < 32; j++)
	{
		LPD6803_DCLK = 1;
		LPD6803_DCLK = 0;
	}

	LPD6803_DIN  = 1;
	LPD6803_DCLK = 1;
	LPD6803_DCLK = 0;

	// output 5 bits red data 
	mask = 0x10;
	for(j = 0; j < 5; j++)
	{
		if(mask & dr)
		{
			LPD6803_DIN = 1;
		}
		else
		{
			LPD6803_DIN = 0;
		}
		LPD6803_DCLK = 1;
		LPD6803_DCLK = 0;
		mask >>= 1;
	}

	// output 5 bits green data 
	mask=0x10;
	for(j = 0; j < 5; j++)
	{
		if(mask & dg)
		{
			LPD6803_DIN = 1;
		}
		else
		{		
			LPD6803_DIN = 0;
		}
		LPD6803_DCLK = 1;
		LPD6803_DCLK = 0;
		mask >>= 1;
	}


	// output 5bits blue data
	mask = 0x10;
	for(j = 0; j < 5; j++)
	{
		if(mask & db)
		{
			LPD6803_DIN = 1;
		}
		else
		{
			LPD6803_DIN = 0;
		}
		LPD6803_DCLK = 1;
		LPD6803_DCLK = 0;
		mask >>= 1;
	}

	LPD6803_DIN = 0;
	LPD6803_DCLK = 1;
	LPD6803_DCLK = 0;
	// transport data finish
	_nop_();
	_nop_();
	_nop_();
}