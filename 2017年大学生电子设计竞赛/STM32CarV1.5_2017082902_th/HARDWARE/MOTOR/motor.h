#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 

#define PWMA	TIM4->CCR1  //PB6
#define PWMB	TIM4->CCR2	//PB7

#define AIN1	PBout(12)
#define AIN2	PBout(13)
#define BIN1	PBout(14)
#define BIN2	PBout(15)

#define MotorAFront();		AIN1 = 1;AIN2 = 0;//Left
#define MotorAAfter();		AIN1 = 0;AIN2 = 1;		
#define MotorAStop();		AIN1 = 0;AIN2 = 0;

#define MotorBFront();		BIN1 = 1;BIN2 = 0;//Right
#define MotorBAfter();		BIN1 = 0;BIN2 = 1;
#define MotorBStop();		BIN1 = 0;BIN2 = 0;

#define Car_DIR_Front();	MotorAFront();MotorBAfter();	//反转
#define Car_DIR_Back();		MotorAAfter();MotorBFront();	//正转
#define Car_DIR_TurnLeft()
#define Car_DIR_TurnRight()
#define Car_DIR_Stop();		MotorAStop();MotorBStop();

#define SERVO   TIM1->CCR4  //舵机引脚	PA11

void Motor_IO_Init(void);
void Motor_PWM_Init(u16 arr,u16 psc);
void Servo_PWM_Init(u16 arr,u16 psc);

#endif
