/**
 * ������������غ���
 * ���ߣ�hhelib
 * �����£�2016-03-13
 */

#include "Sound.h"
#include "timer.h"
#include "config.h"
#include "app.h"
#include "PinDef.h"
#include "GPIO.h"
#include "delay.h"

// ˽�к�������
void Delay200us(void);

/**
 * void soundInit(void)
 * ��飺�������ų�ʼ��
 * ���룺��
 * �������
 */
void soundInit(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;	
	//TIM_InitTypeDef		TIM_InitStructure;
	
	// ���������� IO ������Ϊ�������ģʽ����������
	GPIO_InitStructure.Pin  = GPIO_Pin_0;	
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		
	GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);	
	BUZ = 0;
	
	// ��ʼ����ʱ�� 1�����ڲ���һ��Ƶ�ʵ�����
	/*
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;
	TIM_InitStructure.TIM_Polity    = PolityLow;
	TIM_InitStructure.TIM_Interrupt = DISABLE;
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;
	TIM_InitStructure.TIM_ClkOut    = DISABLE;
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 100000UL);
	TIM_InitStructure.TIM_Run       = ENABLE;
	Timer_Inilize(Timer1, &TIM_InitStructure);	
	*/
}

/**
 * void tone(uint16_t freq)
 * ��飺�����ض�Ƶ�ʵ�����
 * ���룺freq - ����Ƶ�ʣ�20~20000
 * �������
 */
void tone(uint16_t freq)
{
	uint16_t timerValue = 0;
	timerValue = 65536ul - (MAIN_Fosc / (freq * 24));
	TH1 = (uint8_t)(timerValue >> 8);
	TL1 = (uint8_t)timerValue;
	ET1 = 1;
}

/**
 * void noTone(void)
 * ��飺ֹͣ��������
 * ���룺freq - ����Ƶ�ʣ�20~20000
 * �������
 */
void noTone(void)
{
	ET1 = 0;
	BUZ = 0;
}

/**
 * void playSound(uint8_t var)
 * ��飺����ָ��������
 * ���룺var - ��������
 * �������
 */
void playSound(uint8_t var)
{
	uint16_t i = 0;
	// ������Դ�� NodeWee (http://nodewee.me) �ṩ
  switch(var) 
	{
  case 1://sa jiao
    for(i=200;i<990;i++)
		{
      tone(i);
			Delay200us();
			noTone();
    }
    for(i=990;i>200;i--)
		{
      tone(i);
			Delay200us();
			noTone();
    }
    for(i=200;i<350;i++)
		{
      tone(i);
			Delay200us();
			noTone();
    }
    break;
  case 2: // di di
    tone(900);
    Delay_ms(400);
	  noTone();
    Delay_ms(200);
    tone(900);
	  Delay_ms(400);
	  noTone();
    Delay_ms(200);
    break;
  case 3: // de li
    tone(300);
	  Delay_ms(400);
	  noTone();
    Delay_ms(50);
    tone(400);
	  Delay_ms(250);
	  noTone();
    Delay_ms(50);
    break;
  case 4: // du da
    tone(300);
	  Delay_ms(500);
	  noTone();
    Delay_ms(100);
    for(i=300;i<500;i++)
		{
      tone(i);
			Delay_ms(1);
			noTone();
    }
    Delay_ms(50);
    break;
  case 5: // du~ do~~
    for(i=450;i<650;i++)
		{
      tone(i);
      Delay_ms(1);
    }
    noTone();
    Delay_ms(500);
    //
    for(i=450;i< 750;i++)
		{
      tone(i);
      Delay_ms(1);
    }
    noTone();
    Delay_ms(500);
    break;
  case 6: // du_-`~
    for(i=1000;i<1400;i++)
		{
      tone(i);
      Delay_ms(1);
    }
    noTone();
    Delay_ms(100);
    break;
  case 7: // di- dang-_
    tone(1200);
    Delay_ms(100);
    noTone();
    Delay_ms(100);
    for(i=900;i>300;i--)
		{
      tone(i);
      Delay_ms(1);
    }
    noTone();
    Delay_ms(100);
    break;
  case 8: // dang~ dang-
    tone(1100);
    Delay_ms(120);
    noTone();
    Delay_ms(100);
    tone(600);
    Delay_ms(350);
    noTone();
    Delay_ms(100);
    break;
  case 9: //u- u~
    tone(150);
    Delay_ms(400);
    noTone();
    Delay_ms(250);
    tone(200);
    Delay_ms(400);
    noTone();
    Delay_ms(100);
    break;
  case 12: // bee boo
    for(i=300;i<400;i++)
		{
      tone(i);
      Delay_ms(1);
    }
    tone(400);
    Delay_ms(200);
    for(i=400;i>350;i--)
		{
      tone(i);
      Delay_ms(1);
    }
    tone(350);
    Delay_ms(200);
    noTone();
    break;
  default: //bee bee
    tone(500);
	  Delay_ms(300);
	  noTone();
    Delay_ms(400);
    tone(500);
	  Delay_ms(300);
	  noTone();
    Delay_ms(400);
  }
}

void alarmDispatcher(void)
{
	if((alarmEnable == 1) &&
		 (hour == hour_alarm) &&
	   ((minute - minute_alarm) < 3))
	{
		if(second % 3 == 0)
		{
			// ÿ�� 3 ����һ��
		  playSound(2);
		}
	}
}

void Delay200us()		//@24.000MHz
{
	unsigned char i, j;

	i = 5;
	j = 168;
	do
	{
		while (--j);
	} while (--i);
}

/**
 * void timer1_int (void)
 * ��飺Timer 1 �� ISR����ת������ IO ��
 * ���룺��
 * �������
 */
/*
void timer1_int (void) interrupt TIMER1_VECTOR
{
  BUZ = ~BUZ;
}
*/