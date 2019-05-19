#ifndef __MAIN_H
#define __MAIN_H	 

#include "sys.h"

#define SERVO_INIT 750

extern int Encoder_Left,Encoder_Right;				//���ұ��������������
extern float Velocity_KP,Velocity_KI;				//�ٶȿ���PID����
extern int Motor_A,Motor_B,Servo,Target_A,Target_B;	//�������������   
extern float Velocity,Angle;						//�û�������ٶ�,ǰ��ת��
extern u16 Laser1_Distance;							//1�ż�����-����ֵ
extern u8 Laser1_Mode;								//1�ż�����-ģʽ
extern u8 Car_Garage;								//���еĳ���
extern int MotorA_Speed,MotorB_Speed;				//A��B���ٶ�

#endif


