#include "key.h"
#include "lcd12864.h"
#include "stdio.h"

uint8_t motor_speed_pwm = 0;
uint8_t Refrigeration_mode_flag = 0;
uint8_t Heating_mode_flag = 0;
extern sSystem_Param 		*pSystem_Param;			//�����ṹ��ָ�����
extern void Show_demo(uint8_t addr, uint8_t _param);	

xdata uint8_t able1[16] = {0};											//��������
uint8_t keyval_Get(uint8_t _current_key)						//����ֵ��ȡ����
{
	static uint8_t _previous_key = 0xff , _key_flag = 1;//��������
	static uint8_t timeout_500ms = 0;										//��������			
	uint8_t keyval = 0; 																//��������

	if(_current_key != 0x0f && _previous_key == _current_key && _key_flag)		//if(��ǰ����ֵ ��= 0x0f ���� ��ǰ����ֵ == ֮ǰ����ֵ ������������û�йر�)
	{
	   _key_flag = 0;																													//�������ش�
			timeout_500ms = 0;																										//500ms��ʱ��������
		if((_current_key & 1) == 0)																							//�жϾ��尴��
			keyval |= 1;
		
		if((_current_key & (1 << 1)) == 0)																			//�жϾ��尴��
			keyval |= 1 << 1;
		
		if((_current_key & (1 << 2))  == 0)																			//�жϾ��尴��
			keyval |= 1 << 2;
		
		if((_current_key & (1 << 3))  == 0)																			//�жϾ��尴��
			keyval |= 1 << 3;

	}
	else if(_current_key == 0x0f)																							//��û�а����������������أ��ر�
	{
		_key_flag = 1;
	}
	
	if((_current_key & (1 << 1)) == 0 || (_current_key & (1 << 2))  == 0)			//�ж������������Լ�����������ô������ʱ����
	{
		timeout_500ms ++;
		if(timeout_500ms > 50)
			_key_flag = 1;
			
	}
	_previous_key = _current_key;																							//����ǰ�İ���ֵ��ֵ��֮ǰ����ֵ�ı���
	
	return keyval;
}


void Event_Handler(uint8_t _key_val)																				//�¼�������
{
	switch(_key_val)																													//�жϰ���ֵ
	{
		case 1:if(++(pSystem_Param->Current_Mode) == Wind_speed_1)							//����ģʽ�л�
				{
					pSystem_Param->Current_Mode = Air_supply_mode;
				}
				Refrigeration_mode_flag = 0;
				Heating_mode_flag = 0;
				Show_demo(0x80, pSystem_Param->Current_Mode);												//��ʾ
				switch(pSystem_Param->Current_Mode)
				{
					case Air_supply_mode:
						
						pSystem_Param->Wind_speed = pSystem_Param->History_Param->Air_supply_mode_Wind_speed;													//��ʾ
						break;
					case Refrigeration_mode: 
						pSystem_Param->Wind_speed = pSystem_Param->History_Param->Refrigeration_mode_Wind_speed;													//��ʾ
						break;
					case Heating_mode:
						pSystem_Param->Wind_speed = pSystem_Param->History_Param->Heating_mode_Wind_speed;													//��ʾ
						break;
					default:break;
				}
				Show_demo(0x98, pSystem_Param->Wind_speed);													//��ʾ
			break;
		case 2:pSystem_Param->Set_tempVal += 5;break;														//�趨�¶�������û����������		
		case 4:pSystem_Param->Set_tempVal -= 5;break;														//�趨�¶��Լ���û����������
		case 8:
			switch(pSystem_Param->Current_Mode)
			{
				case Air_supply_mode:																																									//��¼�ͷ�ģʽ�·���
							if(--(pSystem_Param->History_Param->Air_supply_mode_Wind_speed) == Heating_mode)								//�����л�
							{
								pSystem_Param->History_Param->Air_supply_mode_Wind_speed = Wind_speed_3;
							}
							pSystem_Param->Wind_speed = pSystem_Param->History_Param->Air_supply_mode_Wind_speed;
							break;
						case Refrigeration_mode:																																					//��¼����ģʽ�·���	
							if(--(pSystem_Param->History_Param->Refrigeration_mode_Wind_speed) == Heating_mode)							//�����л�
							{
								pSystem_Param->History_Param->Refrigeration_mode_Wind_speed = Wind_speed_3;
							}	
							pSystem_Param->Wind_speed	= pSystem_Param->History_Param->Refrigeration_mode_Wind_speed;						
							break;
						case Heating_mode:																																								//��¼����ģʽ�·���
							if(--(pSystem_Param->History_Param->Heating_mode_Wind_speed) == Heating_mode)										//�����л�
							{
								pSystem_Param->History_Param->Heating_mode_Wind_speed = Wind_speed_3;
							}		
							pSystem_Param->Wind_speed = pSystem_Param->History_Param->Heating_mode_Wind_speed;
							break;
						default:break;
			}
			
				Show_demo(0x98, pSystem_Param->Wind_speed);																														//��ʾ
				break;
		default: break;
	}
}


void motor_run(void)																						//������к���
{
	switch(pSystem_Param->Current_Mode)														//��ȡ��ǰ״̬
	{
		case Air_supply_mode:																				//�ͷ�ģʽ
			motor_speed(pSystem_Param->Wind_speed);										//���ݷ��ٽ��п��Ƶ������	
			Show_demo(0x98,pSystem_Param->Wind_speed);								//��ʾ����
			break;
		case Refrigeration_mode: 																		//����ģʽ
			if((pSystem_Param->Set_tempVal) - 30 >= pSystem_Param->Current_temp && Refrigeration_mode_flag == 0)  //���趨�¶�-3�ȴ��ڵ��ڵ�ǰ�¶�
			{
				Refrigeration_mode_flag = 1;																													
				motor_speed(Wind_stop);																																							//���ͣת
				Show_demo(0x98,Wind_stop);																																					//��ʾ	
			}
			else if(Refrigeration_mode_flag == 1 && ((pSystem_Param->Set_tempVal) + 30 < pSystem_Param->Current_temp))//���趨�¶�+3��<��ǰ�¶�
			{
				Refrigeration_mode_flag = 0;
			}
			if(Refrigeration_mode_flag == 0 )																																				
			{
				motor_speed(pSystem_Param->Wind_speed);									//���ݷ��ٽ��п��Ƶ������	
				Show_demo(0x98,pSystem_Param->Wind_speed);							//��ʾ����
			}
			break;
		case Heating_mode:																					//����ģʽ
			if((pSystem_Param->Set_tempVal) + 30 <= pSystem_Param->Current_temp && Heating_mode_flag == 0)	//���趨�¶�-3��С�ڵ��ڵ�ǰ�¶�
			{
				Heating_mode_flag = 1;
				motor_speed(Wind_stop);																																				//���ͣת
				Show_demo(0x98,Wind_stop);																																		//��ʾ
			}
			else if(Heating_mode_flag == 1 && ((pSystem_Param->Set_tempVal) - 30 > pSystem_Param->Current_temp))//���趨�¶�+3��>��ǰ�¶�
			{
				Heating_mode_flag = 0;
			}
			if(Heating_mode_flag == 0 )
			{
				motor_speed(pSystem_Param->Wind_speed);								//���ݷ��ٽ��п��Ƶ������
				Show_demo(0x98,pSystem_Param->Wind_speed);						//��ʾ����
			}
			break;
		default:break;
	}
}

void motor_speed(uint8_t _speed_val)
{
	switch(_speed_val)
	{
		case Wind_speed_1:motor_speed_pwm = 10; break;										//����1
		case Wind_speed_2:motor_speed_pwm = 5;break;										  //����2
		case Wind_speed_3:motor_speed_pwm = 2;break;									   	//����3
		case Wind_stop:motor_speed_pwm = 0;break;													//ͣת
		default:motor_speed_pwm = 0;break;
	}
}

void Show_demo(uint8_t addr, uint8_t _param)
{
	switch(_param)
	{
		case Air_supply_mode:		Wr_Data_String(addr, "����ģʽ���ͷ�");break;
		
		case Refrigeration_mode:Wr_Data_String(addr, "����ģʽ������");break;
		
		case Heating_mode:			Wr_Data_String(addr, "����ģʽ������");break;
		
		case Wind_speed_1:			Wr_Data_String(addr, "��ǰ���٣���  ��");break;
		
		case Wind_speed_2:			Wr_Data_String(addr, "��ǰ���٣���  ��");break;
		
		case Wind_speed_3:			Wr_Data_String(addr, "��ǰ���٣���  ��");break;
		
		case Wind_stop:					Wr_Data_String(addr, "��ǰ���٣�STOP  ");break;
		
		default :break;
	}
}

