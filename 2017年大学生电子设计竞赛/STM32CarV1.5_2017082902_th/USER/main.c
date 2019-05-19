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


int Encoder_Left,Encoder_Right;					//���ұ��������������
float Velocity_KP=12,Velocity_KI=12;			//�ٶȿ���PID����
int Motor_A,Motor_B,Servo,Target_A,Target_B;	//�������������       
float Velocity,Angle;							//�û�������ٶ�,ǰ��ת��
u16 Laser1_Distance;							//1�ż�����-����ֵ
u8 Laser1_Mode;									//1�ż�����-ģʽ
u8 Car_Garage = 0;								//���еĳ���
int MotorA_Speed,MotorB_Speed;					//A��B���ٶ�

int main(void)
{
	Stm32_Clock_Init(9);            //ϵͳʱ������
	delay_init(72);                 //��ʱ��ʼ��
//	JTAG_Set(JTAG_SWD_DISABLE);     //�ر�JTAG�ӿ�
//	JTAG_Set(SWD_ENABLE);           //��SWD�ӿ� �������������SWD�ӿڵ���
	Motor_IO_Init();				//��ʼ�����IO��
	Car_DIR_Stop();
	uart1_init(72,9600);	 		//���ڳ�ʼ��Ϊ9600
	LED_Init();		  				//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();                     //������ʼ��
	Encoder_Init_TIM2();            //�������ӿ�
	Encoder_Init_TIM3();            //ʼ�������� 
//	Servo_PWM_Init(9999,144);   	//ʼ��PWM50HZ���� ���
	EXTIX_Init();
	TIM5_Int_Init(49,7199);         //��ʱ��2�жϳ�ʼ�� 5ms
	
	while(1)
	{
		
		break;
	}
	Motor_PWM_Init(7199,0);  				//��ʼ��PWM 10KHZ�������������

	while(1)
	{
		if(Car_Garage !=0)
		{
			CarRun(Car_Garage);
			Car_Garage = 0;
		}
	}
}

