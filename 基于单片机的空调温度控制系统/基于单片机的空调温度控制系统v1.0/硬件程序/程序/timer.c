#include "timer.h"
#include "key.h"
extern uint32_t STATE_FLAG;
extern uint8_t ukey_val ;
extern uint8_t motor_speed_pwm;
void Timer0Init(void)		//10����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0xF0;		//���ö�ʱ��ֵ
	TH0 = 0xD8;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	IE |=0X82;
}

void tim0() interrupt 1
{
    static uint8_t _TimeOut_100ms = 0;
		static uint8_t motor_temp = 0;
		uint8_t _ukey_val = 0;
		TL0 = 0xF0;		//���ö�ʱ��ֵ
		TH0 = 0xD8;		//���ö�ʱ��ֵ
		if(_ukey_val = keyval_Get((uint8_t)KEY_STATE))
		{
			Event_Handler(_ukey_val);
		}
	if(_TimeOut_100ms ++ > 10)
	{
		_TimeOut_100ms = 0;
		STATE_FLAG |= 1 << 0;
	}
	
	if(++motor_temp < motor_speed_pwm )
	{
			P10 = 0;
	}
	else if(motor_temp > 10)
	{
		motor_temp = 0;
	}
	else
	{
		P10 = 1;
	}
	
}

