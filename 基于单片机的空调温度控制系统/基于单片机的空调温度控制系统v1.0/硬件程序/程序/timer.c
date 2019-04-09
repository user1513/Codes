#include "timer.h"
#include "key.h"
extern uint32_t STATE_FLAG;
extern uint8_t ukey_val ;
extern uint8_t motor_speed_pwm;
void Timer0Init(void)		//10毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0xF0;		//设置定时初值
	TH0 = 0xD8;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	IE |=0X82;
}

void tim0() interrupt 1
{
    static uint8_t _TimeOut_100ms = 0;
		static uint8_t motor_temp = 0;
		uint8_t _ukey_val = 0;
		TL0 = 0xF0;		//设置定时初值
		TH0 = 0xD8;		//设置定时初值
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

