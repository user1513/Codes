#include "control.h"
#include "main.h"
#include "math.h"
#include "encoder.h"
#include "motor.h"
#include "delay.h"
#include "key.h"
#include "usart1.h"
#include "key.h"


u8 Collision = 0x00;
u8 tim5count_5ms = 0;
u8 pz_up_static = 0;
void TIM5_IRQHandler(void)  
{    
	if(TIM5->SR&0X0001)//定时中断
	{
		TIM5->SR&=~(1<<0);

		Encoder_Left+=Read_Encoder(2);				//读取编码器的值
		Encoder_Right+=Read_Encoder(3);				//读取编码器的值

		if(pz_up_static == 1)
		{
			tim5count_5ms++;
			if(tim5count_5ms == 3)
			{
				tim5count_5ms = 0;
				if(KEY == 1)
				{				 
					Collision = Collision + 1;
					//发送
					USART1->DR=0x55;
					while((USART1->SR&0X40)==0);//等待发送结束
					USART1->DR=0x02;
					while((USART1->SR&0X40)==0);//等待发送结束
					USART1->DR=0xFF;
					while((USART1->SR&0X40)==0);//等待发送结束
					USART1->DR=Collision;
					while((USART1->SR&0X40)==0);//等待发送结束
					USART1->DR=(u8)(Collision+0xFF+0x02);
					while((USART1->SR&0X40)==0);//等待发送结束
					USART1->DR=0xAA;
					while((USART1->SR&0X40)==0);//等待发送结束
					
				}
				pz_up_static = 0;
			}
		}
		
	}
}


void Car_Forward(int encoder_left_max,int left_speed,int encoder_right_max,int rightt_speed)
{
	Encoder_Left = 0;
	Encoder_Right = 0;
	MotorA_Speed = left_speed;
	MotorB_Speed = rightt_speed; 
	PWMA = MotorA_Speed;
	PWMB = MotorB_Speed;
	SERVO = SERVO_INIT;
	Car_DIR_Front();	//方向设置向前
	
	while(1)
	{
		if(Encoder_Left > encoder_left_max)
		{
			MotorAStop();
		}
		if(Encoder_Right > encoder_right_max)
		{
			MotorBStop();
		}
		if((AIN1 == 0)&&(AIN2 ==0)&&(BIN1 ==0)&&(BIN2 ==0))
		{
			break;
		}
	}
}

void Car_Reversing(int encoder_left_max,int left_speed,int encoder_right_max,int rightt_speed)
{
	Encoder_Left = encoder_left_max;
	Encoder_Right = encoder_right_max;
	MotorA_Speed = left_speed;
	MotorB_Speed = rightt_speed; 
	PWMA = MotorA_Speed;
	PWMB = MotorB_Speed;
	SERVO = SERVO_INIT;
	Car_DIR_Back();			//后退
	while(1)
	{
		if(Encoder_Left <= 0)
		{
			MotorAStop();
		}
		if(Encoder_Right <= 0)
		{
			MotorBStop();
		}
		if((AIN1 == 0)&&(AIN2 ==0)&&(BIN1 ==0)&&(BIN2 ==0))
		{
			break;
		}
	}
}

void Car_ForwardTurnCorner(int servo,int encoder_left_max,int left_speed,int encoder_right_max,int rightt_speed)
{
	Encoder_Left = 0;
	Encoder_Right = 0;
	MotorA_Speed = left_speed;
	MotorB_Speed = rightt_speed; 
	PWMA = MotorA_Speed;
	PWMB = MotorB_Speed;
	
	SERVO = servo;
	delay_ms(300);
	
	Car_DIR_Front();	//方向设置向前
	
	while(1)
	{
		if(Encoder_Left > encoder_left_max)
		{
			MotorAStop();
		}
		if(Encoder_Right > encoder_right_max)
		{
			MotorBStop();
		}
		if((AIN1 == 0)&&(AIN2 ==0)&&(BIN1 ==0)&&(BIN2 ==0))
		{
			break;
		}
	}	
}
void Car_ReversingTurnCorner(int servo,int encoder_left_max,int left_speed,int encoder_right_max,int rightt_speed)
{
	Encoder_Left = encoder_left_max;
	Encoder_Right = encoder_right_max;
	MotorA_Speed = left_speed;
	MotorB_Speed = rightt_speed; 
	PWMA = MotorA_Speed;
	PWMB = MotorB_Speed;
	
	SERVO = servo;
	delay_ms(300);
	
	Car_DIR_Back();			//后退
	
	while(1)
	{
		if(Encoder_Left <= 0)
		{
			MotorAStop();
		}
		if(Encoder_Right <= 0)
		{
			MotorBStop();
		}
		if((AIN1 == 0)&&(AIN2 ==0)&&(BIN1 ==0)&&(BIN2 ==0))
		{
			break;
		}
	}	
}

void Car_Traversing(int encoder_left_max,int left_speed,int encoder_right_max,int right_speed)
{
	if(left_speed > 0)
	{
		MotorAFront();
		Encoder_Left = 0;
		MotorA_Speed = left_speed;
	}else
	{
		MotorAAfter();
		Encoder_Left = encoder_left_max;
		MotorA_Speed = -left_speed;
	}
	if(right_speed > 0)
	{
		MotorBAfter();
		Encoder_Right = 0;
		MotorB_Speed = right_speed; 
	}else
	{
		MotorBFront();
		Encoder_Right = encoder_right_max;
		MotorB_Speed = -right_speed; 
	}
	PWMA = MotorA_Speed;
	PWMB = MotorB_Speed;
	while(1)
	{
		if(left_speed > 0)
		{
			if(Encoder_Left > encoder_left_max)
			{
				MotorAStop();
			}			
		}else
		{
			if(Encoder_Left <= 0)
			{
				MotorAStop();
			}			
		}
		
		if(right_speed > 0)
		{
			if(Encoder_Right > encoder_right_max)
			{
				MotorBStop();
			}			
		}else
		{
			if(Encoder_Right <= 0)
			{
				MotorBStop();
			}			
		}
		if((AIN1 == 0)&&(AIN2 ==0)&&(BIN1 ==0)&&(BIN2 ==0))
		{
			break;
		}
	}
}
