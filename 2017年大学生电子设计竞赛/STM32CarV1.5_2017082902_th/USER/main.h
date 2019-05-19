#ifndef __MAIN_H
#define __MAIN_H	 

#include "sys.h"

#define SERVO_INIT 750

extern int Encoder_Left,Encoder_Right;				//左右编码器的脉冲计数
extern float Velocity_KP,Velocity_KI;				//速度控制PID参数
extern int Motor_A,Motor_B,Servo,Target_A,Target_B;	//电机舵机控制相关   
extern float Velocity,Angle;						//用户输入的速度,前轮转角
extern u16 Laser1_Distance;							//1号激光测距-距离值
extern u8 Laser1_Mode;								//1号激光测距-模式
extern u8 Car_Garage;								//运行的车库
extern int MotorA_Speed,MotorB_Speed;				//A和B的速度

#endif


