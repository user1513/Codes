#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define PWMA1   TIM8->CCR2  
#define PWMA2   TIM8->CCR1 

#define PWMB1   TIM1->CCR1 
#define PWMB2   TIM1->CCR2 

#define PWMC1   TIM1->CCR4
#define PWMC2   TIM1->CCR3

#define PWMD2   TIM8->CCR4
#define PWMD1   TIM8->CCR3 
void MiniBalance_PWM_Init(u16 arr,u16 psc);
void MiniBalance_Motor_Init(void);
#endif
