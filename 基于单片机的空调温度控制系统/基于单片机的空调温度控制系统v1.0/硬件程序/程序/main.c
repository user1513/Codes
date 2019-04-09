#include "app.h"
#include "usart.h"
#include "timer.h"
#include "lcd12864.h"
#include "stdio.h"
#include "temp.h"
#include "key.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define INITSETTEMPVAL 200          							//���ó�ʼ�趨�¶�ֵ��200/10��

#define INITSETWINSPEEDVAL Wind_speed_3 		      //���ó�ʼ����ֵ��Ĭ��������

#define INITMODE	Air_supply_mode									//���ó�ʼģʽ��Ĭ���ͷ磩	

//////////////////////////////////////////////////////////////////////////////////////////////////////////
xdata uint8_t able[16] = {0};											//��������

sHistory_Param  	History_Param;									//�����ṹ�����		

sSystem_Param 		System_Param;										//�����ṹ�����

sHistory_Param  	*pHistory_Param = &History_Param;									//�����ṹ��ָ�����		

sSystem_Param 		*pSystem_Param = &System_Param;										//�����ṹ��ָ�����

uint8_t ukey_val = 0;															//����ֵ��ȡ

uint32_t STATE_FLAG = 0;													//״̬��־λ
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void Init_Config(void);														//��������

void param_Init(sSystem_Param * _System_Param,sHistory_Param * _History_Param);//��������

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void param_Init(sSystem_Param * _System_Param,sHistory_Param * _History_Param)		//�ṹ�������ʼ��
{
	_System_Param->Set_tempVal = INITSETTEMPVAL ;																	//���ó�ʼ�����¶�
	
	_System_Param->Wind_speed = INITSETWINSPEEDVAL;																//���ó�ʼ����
	
	_System_Param->Current_Mode = INITMODE;																				//���ó�ʼģʽ
	
	_System_Param->History_Param = _History_Param;																//������ʷ�����ṹ��ָ��
	
	_System_Param->History_Param -> Air_supply_mode_Wind_speed = Wind_speed_3;		//д��Ĭ�Ϸ���ֵ
	
	_System_Param->History_Param -> Refrigeration_mode_Wind_speed = Wind_speed_3;	//д��Ĭ�Ϸ���ֵ
	
	_System_Param->History_Param -> Heating_mode_Wind_speed = Wind_speed_3;				//д��Ĭ�Ϸ���ֵ
}

void Init_Config(void)
{
	Lcm_Init();																																		//lcd12864��ʼ��
	
	Timer0Init();																																	//��ʱ��0��ʼ��
	
	param_Init(pSystem_Param, pHistory_Param);																		//�ṹ�������ʼ��
	
	Show_demo(0x80, pSystem_Param->Current_Mode);																	//��ʾ

	Show_demo(0x98, pSystem_Param->Wind_speed);																		//��ʾ
}



void main()
{
	Init_Config();
	while(1)
	{
		if(STATE_FLAG & (1 << 0 ))
		{
			STATE_FLAG &= ~(1 << 0);
			
			sprintf(able, "�����¶ȣ�%02d.%d��",(uint16_t)pSystem_Param->Set_tempVal/10, (uint16_t)pSystem_Param->Set_tempVal%10);
	
			Wr_Data_String(0x90, able);
	
			pSystem_Param->Current_temp = LcdDisplay(Ds18b20ReadTemp());
	
			sprintf(able, "ʵ���¶ȣ�%02d.%d��",(uint16_t)pSystem_Param->Current_temp/10, (uint16_t)pSystem_Param->Current_temp%10);

			Wr_Data_String(0x88, able);
		}	
		motor_run();		
	}
}