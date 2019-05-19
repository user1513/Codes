#include "main.h"

#include "delay.h"
#include "sys.h"
 
#include "led.h"
#include "key.h"
#include "motor.h"
#include "encoder.h"
#include "timer.h"
#include "usart1.h"
#include "exti.h"
#include "control.h"
#include "trajectory.h"


int Encoder_Left,Encoder_Right;					//左右编码器的脉冲计数
float Velocity_KP=12,Velocity_KI=12;			//速度控制PID参数
int Motor_A,Motor_B,Servo,Target_A,Target_B;	//电机舵机控制相关       
float Velocity,Angle;							//用户输入的速度,前轮转角
u16 Laser1_Distance;							//1号激光测距-距离值
u8 Laser1_Mode;									//1号激光测距-模式
u8 Car_Garage = 0;								//运行的车库
int MotorA_Speed,MotorB_Speed;					//A和B的速度

int main(void)
{
	Stm32_Clock_Init(9);            //系统时钟设置
	delay_init(72);                 //延时初始化
//	JTAG_Set(JTAG_SWD_DISABLE);     //关闭JTAG接口
//	JTAG_Set(SWD_ENABLE);           //打开SWD接口 可以利用主板的SWD接口调试
	Motor_IO_Init();				//初始化电机IO口
	Car_DIR_Stop();
	uart1_init(72,9600);	 		//串口初始化为9600
	LED_Init();		  				//初始化与LED连接的硬件接口
	KEY_Init();                     //按键初始化
	Encoder_Init_TIM2();            //编码器接口
	Encoder_Init_TIM3();            //始化编码器 
//	Servo_PWM_Init(9999,144);   	//始化PWM50HZ驱动 舵机
	EXTIX_Init();
	TIM5_Int_Init(49,7199);         //定时器2中断初始化 5ms
	
	while(1)
	{
		
		break;
	}
	Motor_PWM_Init(7199,0);  				//初始化PWM 10KHZ，用于驱动电机

	while(1)
	{
		if(Car_Garage !=0)
		{
			CarRun(Car_Garage);
			Car_Garage = 0;
		}
	}
}

