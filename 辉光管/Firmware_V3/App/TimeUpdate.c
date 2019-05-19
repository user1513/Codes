/**
 * ʱ�������غ���
 * ���ߣ�hhelib
 * �����£�2016-03-13
 **************************
 * ʹ��ʱӦ����ʱ��Ƶ�ʵ�����ʱ����ֵ
 */
 
#include "TimeUpdate.h"
#include "timer.h"
#include "app.h"

/**
 * void timerInit(void)
 * ��飺��ʼ����ʱ�õĶ�ʱ��
 * ���룺��
 * �������
 */
void timerInit(void)
{
	TIM_InitTypeDef		TIM_InitStructure;
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReloadNoMask; // 16 λ�Զ���װ�����������ж�
	TIM_InitStructure.TIM_Polity    = PolityLow;			
	TIM_InitStructure.TIM_Interrupt = ENABLE;
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;  // 12T ģʽ
	TIM_InitStructure.TIM_ClkOut    = DISABLE;
	TIM_InitStructure.TIM_Value     = 0x3cb0;         // 25ms @ 24.000MHz 0x3cb0
	TIM_InitStructure.TIM_Run       = ENABLE;
	Timer_Inilize(Timer2, &TIM_InitStructure);
	
}


/**
 * void timeUpdate_ISR(void)
 * ��飺��ʱ�õĶ�ʱ���� ISR
 * ���룺��
 * �������
 */
void timeUpdate_ISR(void) interrupt TIMER2_VECTOR
{
	per25ms++;
	if(per25ms >= 40)
	{
    per25ms = 0;
		second++;
	}
	if(second >= 60)
	{
		second = 0;
		minute++;
	}
	if(minute >= 60)
	{
		minute = 0;
		hour++;
	}
	if(hour >= 24)
	{
		hour = 0;
	}
}