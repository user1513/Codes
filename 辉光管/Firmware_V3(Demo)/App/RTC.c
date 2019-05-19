/**
 * ʵʱʱ�Ӷ�д��غ���
 * ���ߣ�hhelib
 * �����£�2016-03-13
 */
 
#include "RTC.h"
#include "SD2405ALPI.h"
#include "app.h"

/**
 * void getTime(void)
 * ��飺��ʵʱʱ�ӵ�ʱ�䷢������Ƭ��
 * ���룺��
 * �������
 */
void getTime(void)
{
	uint32_t time = 0;
	time = SD2405ALPI_GetTime();
	hour = (time / 100000ul) * 10 + (time % 100000ul) / 10000ul;
	minute = ((time % 10000ul) / 1000)* 10 + (time % 1000) / 100;
	second = ((time % 100) / 10)* 10 + (time % 10);
}

/**
 * void syncTime(void)
 * ��飺����Ƭ����ʱ��ͬ����ʵʱʱ��,���ú�ȫ�ֱ���ʱ���֡������ô˺�������
 * ���룺��
 * �������
 */
void syncTime(void)
{
	SD2405ALPI_SetTime(hour * 10000ul + minute * 100 + second);
}