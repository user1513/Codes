#include "main.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usart2.h"
#include "led.h"
#include "key.h"
#include "gps.h"
#include "dht11.h"
#include "hc595.h"
#include "mp3play.h"
#include "24cxx.h"
#include "ds3231.h"
#include "myiic.h"
#include "timer.h"
#include "myeeprom.h"

//0,��֧�ֱ���(֧��GPSʱ��)Уʱ
//1,֧�ֱ���Уʱ
#define SYSTEM_SUPPORT_LOCAL		1		//����ϵͳ�ļ����Ƿ�֧�ֱ���ʱ��Уʱ

#if	SYSTEM_SUPPORT_LOCAL	//��SYSTEM_SUPPORT_LOCAL������,˵��Ҫ֧�ֱ���Уʱ
#else	//��֧�ֱ���ʱ��Уʱ��֧��GPSʱ��Уʱ
	
	
	
#endif

struct _System_Data_dev sys_clock_dev=
{
	17,		//��
	6,//��
	12,//��
	12,//ʱ
	0,//��
	0,//��
	1,//��
	17,//��ѧ��
};


struct _gps_data_dev gps_clock_dev=
{
	0,		//GPS��
	0,		//GPS��
	0,		//GPS��
	0,		//GPSʱ
	0,		//GPS��
	0,		//GPS��
};

u8 temperature;  	    
u8 humidity;   

u8 GPSReception;
#define ERROR 0X00
#define OK 0x01

u8 aoto_hour = 0x12;
u8 aoto_min = 0x01;
u8 aoto_sec = 0x30;

#define max_alarmnumber 50//����������
u8 Alarm_Data[max_alarmnumber][8];	//���(0x01)��ʱ���֣��룬�������ظ� 
									//���(0x02)��ʱ���֣��룬�������꣬�£���
u8 nowalarmnumber = 0;				//��ǰ���Ӹ��� �洢��EEPROM

u8 Usart1Data[15];
u8 Usart1ReceiveCounter = 0;
u8 receive_flag = 0;
u8 TIM3_flag = 0;

u16 sys_password;			//ϵͳ����

u8 sys_addr = 0x03;			//ϵͳ��ַ��
u8 School_flag;				//Уʱ��־λ

void GPSSignalAnalysis()
{
	if(Data_legitimate==0)//��GPS�ź�ʱ
	{
		GPSReception = ERROR ;
	}else
	{
		if(Data_legitimate|0x01)	//GPGGA����
		{			
			if(GPS_lock==0)					//δ��λ
			{
				GPSReception = ERROR ;
			}
			else				//�Ѷ�λ
			{
				//����ʱ��
				gps_clock_dev.hour = (GPS_time[0] - 0x30) * 10 + (GPS_time[1] - 0x30);
				gps_clock_dev.min =  (GPS_time[2] - 0x30) * 10 + (GPS_time[3] - 0x30);
				gps_clock_dev.sec =  (GPS_time[4] - 0x30) * 10 + (GPS_time[5] - 0x30);
				
				//��������
				gps_clock_dev.year = (GPS_date[4] - 0x30) * 10 + (GPS_date[5] - 0x30);
				gps_clock_dev.month = (GPS_date[2] - 0x30) * 10 + (GPS_date[3] - 0x30);
				gps_clock_dev.day = (GPS_date[0] - 0x30) * 10 + (GPS_date[1] - 0x30);
				
				gps_clock_dev.hour += 8;//8��ʱ��
				if(gps_clock_dev.hour >= 24) //һ��Уʱʱ��Ҫ����ʱ����һ��Уʱ��ʱ�򲻻�ִ���������
				{
					gps_clock_dev.day = gps_clock_dev.day + 1;
				}
				gps_clock_dev.hour %= 24;
				
				//---------------------------------------------------------------------
				if((gps_clock_dev.hour < 24)&&(gps_clock_dev.min < 60)&&(gps_clock_dev.sec < 60)&&(gps_clock_dev.month<13)&&(gps_clock_dev.day<32)) //�ݴ���//��ֹ��Уʱ��һ˲�����
				{	
					GPSReception = OK ;//���ճɹ�
				}else
				{
					GPSReception = ERROR ;//����ʧ��
				}
				//---------------------------------------------------------------------
			}			
			Data_legitimate &= ~0x01;
		}			
	}	
}
//��Ч���ж�
#define Automatic	0
#define Manual		1
#define IS_AOTOTIME_MODE(MODE) 		(((Automatic) == 0) || ((Manual) == 1))
                                    
//waiting_ms���Զ�Уʱʱ��
//mode��ģʽѡ�񣬹�ϵ���������
//delay_ms_dis����ʾ��������ʱ��
void Aototime_function(u16 waiting_ms ,u8 mode,u16 delay_ms_dis)
{
	u16 t;
	assert_param(IS_AOTOTIME_MODE(mode));//��Ч���ж�
	t = waiting_ms * 100;//t*delay_us��n����ʱ��
	while(t--)//����Уʱ��ť��һֱ����GPS���ݣ��ɹ�������DS1302ʱ�����ݣ�ʧ�������ѭ���� //�ȴ�500ms
	{
		GPSSignalAnalysis();//GPSʱ�����ݽ�������
		if(GPSReception == OK)//�����ɹ�
		{
			//��ֵʱ��
			sys_clock_dev.hour = gps_clock_dev.hour;
			sys_clock_dev.min = gps_clock_dev.min;
			sys_clock_dev.second = gps_clock_dev.sec;
			//��ֵ����
			sys_clock_dev.year = gps_clock_dev.year;
			sys_clock_dev.month = gps_clock_dev.month;
			sys_clock_dev.day = gps_clock_dev.day;
			
			My_DS3231_SetTIME();//���ݽṹ���ʼ������
			break;
		}//end if
		delay_us(10);
	}
	if(t == 0)
	{
		//��ʾ�������,����ֱ�Ӹ�ֵmin sec
		switch(mode)
		{
			case Automatic:
									{
										sys_clock_dev.hour = 0x99;
										sys_clock_dev.min = 0x01;
										HC595TransmitData();
										break;
									}
			case Manual:
									{
										sys_clock_dev.hour = 0x99;
										sys_clock_dev.min = 0x00;
										HC595TransmitData();
										break;
									}
			default:break;
		}
		//�ӳ���ʾ�������ʱ�� ----- 5S
		t = delay_ms_dis;
		while(t--)
		{
			delay_ms(1);
		}
	}		
}
void ReadAllAlarmData(void)
{
	u8 i,j;
	for(i=0; i<nowalarmnumber; i++)
	{
		for(j=0; j<8; j++)
		{
			Alarm_Data[i][j] = ReadAlarmData(j,i);
		}
	}
}

void MainStoreAllAlarm(void)
{
	u8 i,j;
	for(i=0; i<nowalarmnumber; i++)
	{
		for(j=0; j<8; j++)
		{
			StoreAlarmData(j,i,Alarm_Data[i][j]);
		}
	}
}

void MainStoreAlarm_Delete(u8 AlarmNo)
{
	u8 i,j;
	for(i=AlarmNo-1; i<nowalarmnumber; i++)
	{
		for(j=0; j<8; j++)
		{
			StoreAlarmData(j,i,Alarm_Data[i][j]);
		}
	}
}

void MainStoreAlarmData(u8 _alarmnumber)
{
	u8 i;
	for(i=0; i<8; i++)
	{
		Alarm_Data[_alarmnumber - 1][i] = ReadAlarmData(i,_alarmnumber - 1);
	}
}
u8 sysAlarmTimer = 0;

void DeleteAlarm(u8 AlarmNo)//AlarmNo�����ӱ�� ��1��ʼ
{
	int i,j;
	for(i=AlarmNo-1; i<nowalarmnumber; i++)
	{
		for(j=0; j<8; j++)
		{
			Alarm_Data[i][j] = Alarm_Data[i+1][j];	
		}
	}
	nowalarmnumber = nowalarmnumber - 1;
	MainStoreAlarm_Delete(AlarmNo);
	StorAlarmNumber(nowalarmnumber);
}

void USART1_SENDEnevt(u8 com)
{
	u8 i;
	u8 checknumber;
	u8 USARTData[6] = {0x55,0x00,0x00,0x00,0x00,0xAA};
	USARTData[1] = sys_addr;//��ַ
	USARTData[2] = com;//����
	switch(com)
	{
		case 0x01:
			USARTData[3] = sys_addr;
			break;
		case 0x02:
			USARTData[3] = 0x01;
			break;
		case 0x03:
			USARTData[3] = 0x01;
			break;
		case 0x04://��������һ
			USARTData[3] = 0x01;
			break;
		case 0x05://�������ö�
			USARTData[3] = 0x01;
			break;
		case 0x06://�޸�����
			USARTData[3] = 0x01;
			break;
		case 0x07://��������
			USARTData[3] = 0x01;
			break;
		case 0x08://ȡ������
			USARTData[3] = 0x01;	
			break;
		case 0x09://��ȡ��������
			USARTData[3] = nowalarmnumber;
			break;
		case 0x0A://ɾ����ǰ����
			USARTData[3] = 0x01;
			break;
		case 0x0B://ɾ����������
			USARTData[3] = 0x01;
			break;
		case 0x0C://�����޸�ʧ��
			USARTData[3] = 0x01;
			break;
		case 0x0D://�޸�����OK
			USARTData[3] = 0x01;
			break;
		default:
			break;
	}
	checknumber = 0x00;
	for(i=1;i<4;i++)
	{
		checknumber += USARTData[i];
	}
	USARTData[4] = checknumber;
	USARTData[5] = 0xAA;
	for(i=0;i<6;i++)
	{
		USART_SendData(USART1,USARTData[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}

void USART1_SENDAlarmEvent(void)
{
	u8 i,j;
	u8 checknumber;
	u8 UsartData[13];
	UsartData[0] = 0x66;
	UsartData[1] = sys_addr;
	
	for(i=0;i<nowalarmnumber;i++)
	{
		UsartData[2] = i + 1;//��1��ʼ
		for(j=0;j<8;j++)
		{
			UsartData[j + 3] = Alarm_Data[i][j];
		}
		checknumber = 0x00;
		for(j=1;j<11;j++)
		{
			checknumber += UsartData[j];
		}
		UsartData[11] = checknumber;
		UsartData[12] = 0x99;
		
		for(j=0;j<13;j++)
		{
			USART_SendData(USART1,UsartData[j]);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		}
		delay_ms(sys_addr*25);//�ӳ�һ��ʱ�䣬��������Ȼ���ջ��������
	}
}

void USART1_Analysis(void)
{

	switch(Usart1Data[4])
	{
		case 0x01://��ȡ�豸��ַ
			USART1_SENDEnevt(0x01);
			break;
		case 0x02://�ֶ�����ʱ��
			sys_clock_dev.year = Usart1Data[5];
			sys_clock_dev.month = Usart1Data[6];
			sys_clock_dev.day = Usart1Data[7];
			sys_clock_dev.hour = Usart1Data[8];
			sys_clock_dev.min = Usart1Data[9];
			sys_clock_dev.second = Usart1Data[10];
			sys_clock_dev.week = Usart1Data[11];
			sys_clock_dev.tweek = Usart1Data[12];
			My_DS3231_SetRTC();
			USART1_SENDEnevt(0x02);
			//��ѧ��
			break;
		case 0x03://�Զ�����ʱ��
			sys_clock_dev.year = Usart1Data[5];
			sys_clock_dev.month = Usart1Data[6];
			sys_clock_dev.day = Usart1Data[7];
			sys_clock_dev.hour = Usart1Data[8];
			sys_clock_dev.min = Usart1Data[9];
			sys_clock_dev.second = Usart1Data[10];
			sys_clock_dev.week = Usart1Data[11];
			My_DS3231_SetRTC();
			USART1_SENDEnevt(0x03);
			break;
		case 0x04://��������һ
			if(nowalarmnumber < Usart1Data[5])//0<1
			{
				nowalarmnumber = nowalarmnumber + 1;

				Alarm_Data[nowalarmnumber - 1][0] = 0x01;
				Alarm_Data[nowalarmnumber - 1][1] = Usart1Data[6] / 10 * 16 + Usart1Data[6] % 10; 
				Alarm_Data[nowalarmnumber - 1][2] = Usart1Data[7] / 10 * 16 + Usart1Data[7] % 10;
				Alarm_Data[nowalarmnumber - 1][3] = Usart1Data[8] / 10 * 16 + Usart1Data[8] % 10;
				Alarm_Data[nowalarmnumber - 1][4] = Usart1Data[9]; 
				Alarm_Data[nowalarmnumber - 1][5] = Usart1Data[10]; 
			}else
			{
				Alarm_Data[Usart1Data[5] - 1][0] = 0x01;
				Alarm_Data[Usart1Data[5] - 1][1] = Usart1Data[6] / 10 * 16 + Usart1Data[6] % 10; 
				Alarm_Data[Usart1Data[5] - 1][2] = Usart1Data[7] / 10 * 16 + Usart1Data[7] % 10;
				Alarm_Data[Usart1Data[5] - 1][3] = Usart1Data[8] / 10 * 16 + Usart1Data[8] % 10;
				Alarm_Data[Usart1Data[5] - 1][4] = Usart1Data[9]; 
				Alarm_Data[Usart1Data[5] - 1][5] = Usart1Data[10]; 
			}
			MainStoreAllAlarm();
			USART1_SENDEnevt(0x04);
			break;
		case 0x05://�������ö�
			if(nowalarmnumber < Usart1Data[5])//0<1
			{
				nowalarmnumber = nowalarmnumber + 1;
				Alarm_Data[nowalarmnumber - 1][0] = 0x02;
				Alarm_Data[nowalarmnumber - 1][1] = Usart1Data[6] / 10 * 16 + Usart1Data[6] % 10;
				Alarm_Data[nowalarmnumber - 1][2] = Usart1Data[7] / 10 * 16 + Usart1Data[7] % 10;
				Alarm_Data[nowalarmnumber - 1][3] = Usart1Data[8] / 10 * 16 + Usart1Data[8] % 10;
				Alarm_Data[nowalarmnumber - 1][4] = Usart1Data[9]; 
				Alarm_Data[nowalarmnumber - 1][5] = Usart1Data[10] / 10 * 16 + Usart1Data[10] % 10;
				Alarm_Data[nowalarmnumber - 1][6] = Usart1Data[11] / 10 * 16 + Usart1Data[11] % 10;
				Alarm_Data[nowalarmnumber - 1][7] = Usart1Data[12] / 10 * 16 + Usart1Data[12] % 10;
				
			}else
			{
				Alarm_Data[Usart1Data[5] - 1][0] = 0x02;
				Alarm_Data[Usart1Data[5] - 1][1] = Usart1Data[6] / 10 * 16 + Usart1Data[6] % 10;
				Alarm_Data[Usart1Data[5] - 1][2] = Usart1Data[7] / 10 * 16 + Usart1Data[7] % 10;
				Alarm_Data[Usart1Data[5] - 1][3] = Usart1Data[8] / 10 * 16 + Usart1Data[8] % 10;
				Alarm_Data[Usart1Data[5] - 1][4] = Usart1Data[9]; 
				Alarm_Data[Usart1Data[5] - 1][5] = Usart1Data[10] / 10 * 16 + Usart1Data[10] % 10;
				Alarm_Data[Usart1Data[5] - 1][6] = Usart1Data[11] / 10 * 16 + Usart1Data[11] % 10;
				Alarm_Data[Usart1Data[5] - 1][7] = Usart1Data[12] / 10 * 16 + Usart1Data[12] % 10;
			}
			MainStoreAllAlarm();
			USART1_SENDEnevt(0x05);
			break;
		case 0x06://�޸�����
			sys_password = Usart1Data[5]<<8;
			sys_password |= Usart1Data[6];
			StorePassword(sys_password);//�洢����
			USART1_SENDEnevt(0x06);
			break;
		case 0x07://��������
			MP3_Play(Usart1Data[5]);
			USART1_SENDEnevt(0x07);
			break;
		case 0x08://ȡ������
			MP3_Stop();
			USART1_SENDEnevt(0x08);
			break;
		case 0x09://��ȡ��������
			USART1_SENDEnevt(0x09);
			USART1_SENDAlarmEvent();
			break;
		case 0x0A://ɾ����ǰ����
			DeleteAlarm(Usart1Data[5]);
			USART1_SENDEnevt(0x0A);
			break;
		case 0x0B://ɾ����������
			nowalarmnumber = 0;
			StorAlarmNumber(nowalarmnumber);
			USART1_SENDEnevt(0x0B);
			break;
		case 0x0C://��������
			MP3_SetVolume(Usart1Data[5]);
			USART1_SENDEnevt(0x0C+0x01);			
			break;
	}
}	


void AlarmClockEvent(void)
{
	u8 i;
	for(i=0; i<nowalarmnumber; i++)
	{
		if(Alarm_Data[i][0] == 0x01)//ʱ���������ظ�
		{
			if(((Alarm_Data[i][5] >> (sys_clock_dev.week - 1)) & 0x01) == 1)//����ȷ
			{
				if(sys_clock_dev.hour == Alarm_Data[i][1])
				{
					if(sys_clock_dev.min == Alarm_Data[i][2])
					{
						if(sys_clock_dev.second == Alarm_Data[i][3])
						{
							//������ȷ
							MP3_Play(Alarm_Data[i][4]);
							sysAlarmTimer = 30;
						}else
						{
							continue;
						}
					}else
					{
						continue;
					}
				}else
				{
					continue;
				}
			}else if(Alarm_Data[i][5] == 0x80)//����һ�Σ�����ܲ���ȷ���ж��Ƿ�û���ܣ�������ֻ����һ��
			{
				if(sys_clock_dev.hour == Alarm_Data[i][1])
				{
					if(sys_clock_dev.min == Alarm_Data[i][2])
					{
						if(sys_clock_dev.second == Alarm_Data[i][3])
						{
							//������ȷ
							MP3_Play(Alarm_Data[i][4]);
							DeleteAlarm(i + 1);//ɾ������
							sysAlarmTimer = 30;
						}else
						{
							continue;
						}
					}else
					{
						continue;
					}
				}else
				{
					continue;
				}
			}else
			{
				continue;
			}
		}else if(Alarm_Data[i][0] == 0x02)
		{
			if((sys_clock_dev.year == Alarm_Data[i][5])&&(sys_clock_dev.month == Alarm_Data[i][6])&&(sys_clock_dev.day == Alarm_Data[i][7]))
			{
				if(sys_clock_dev.hour == Alarm_Data[i][1])
				{
					if(sys_clock_dev.min == Alarm_Data[i][2])
					{
						if(sys_clock_dev.second == Alarm_Data[i][3])
						{
							//������ȷ
							MP3_Play(Alarm_Data[i][4]);
							DeleteAlarm(i + 1);//ɾ����ǰ����
							sysAlarmTimer = 30;
						}else
						{
							continue;//������ǰѭ��
						}
					}else
					{
						continue;
					}
				}else
				{
					continue;
				}
			}else
			{
				continue;
			}
		}
	}
}



void TIM3_Event(void)
{
	
	if(sysAlarmTimer != 0)
	{
		sysAlarmTimer--;
		if(sysAlarmTimer == 0)
		{
			MP3_Stop();
		}
	}
}

void SetSSchoolTimer(u8 hour,u8 min,u8 sec)
{
	#if	SYSTEM_SUPPORT_LOCAL	//��SYSTEM_SUPPORT_LOCAL������,˵��Ҫ֧�ֱ���Уʱ
		aoto_hour = hour;
		aoto_min = min;
		aoto_sec = sec;
	#else	//��֧�ֱ���ʱ��Уʱ��֧��GPSʱ��Уʱ
		aoto_hour = hour / 16 * 10 + hour % 16;
		aoto_min = min / 16  * 10 + min % 16;
		aoto_sec = sec / 16 * 10 + sec % 16;	
	#endif

}

int main(void)
{	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�JTAG��ʹ��SWD��������SWDģʽ����,��Լ��Դ
	delay_init();
	NVIC_Configuration();		// �����ж����ȼ�����
	
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	USART2_Init(9600);	 	//����2��ʼ��
	MP3_Init(9600);
	
	LED_Init();				//LED��ʼ�� 
	DHT11_Init();	//DHT11��ʼ��	
	HC595_Init();
	
	DS3231_Init();
	AT24CXX_Init();
	
	if(ReadPassword() > 9999)//����������������豸
	{
		StorePassword(0);
		My_DS3231_SetRTC();//���ݽṹ���ʼ������
	}
//	My_DS3231_SetRTC();//���ݽṹ���ʼ������	
	nowalarmnumber = ReadAlarmNumber();
	sys_password = ReadPassword();
	
	ReadAllAlarmData();
	
	SetSSchoolTimer(aoto_hour,aoto_min,aoto_sec);//���ݺ궨��ѡ���Ƿ�ת��

//	MP3_SetVolume(4);
	
	TIM3_Int_Init(9999,7199);//10Khz�ļ���Ƶ�ʣ�������1000Ϊ100ms     

	while(1)
	{
		HC595TransmitData();
		My_DS3231_ReadRTC();
		
		LED = sys_clock_dev.second % 2;//����ָʾ��
		DP = sys_clock_dev.second % 2;//����ָʾ��
		
		DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ
		
		#if	SYSTEM_SUPPORT_LOCAL	//��SYSTEM_SUPPORT_LOCAL������,˵��Ҫ֧�ֱ���Уʱ
		//�����Զ�Уʱ��ʱ�䡣����5s��Уʱ
		if((sys_clock_dev.hour == aoto_hour)&&(sys_clock_dev.min == aoto_min)&&(sys_clock_dev.second == aoto_sec))
		{
			School_flag = 1;
		}
		#endif
		if(School_flag == 1)
		{
			School_flag = 0;
			Aototime_function(500,Automatic,5000);//Уʱ500ms��
		}
		//��ʱ���¼�
		if(TIM3_flag == 1)
		{
			TIM3_flag = 0;
			TIM3_Event();
		}
		//�����¼�
		AlarmClockEvent();
		//����1������
		if(receive_flag == 1)
		{
			receive_flag = 0;
			delay_ms(sys_addr*25);//�ӳ�һ��ʱ�䣬��������Ȼ���ջ��������
			USART1_Analysis();//����1�ж�
		}
		
		//------------------------����ɨ�躯��----------
		switch(KEY_Scan(0))
		{
			case S1_PRES:
						{
							Aototime_function(500,Manual,5000);
							break;//Уʱ
						}
			case S2_PRES:
				
				break;
			default:break;
		}
	}	
}


void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	u8 i;//�����ۼӾֲ�����
	u8 checknumber;//�ۼ�У��ֲ�����
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //������
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		Usart1Data[Usart1ReceiveCounter] = Res;
		
		switch(Usart1ReceiveCounter)
		{
			case 0:
				if(Usart1Data[Usart1ReceiveCounter] == 0x55)//��ͷ
				{
					Usart1ReceiveCounter ++;
				}else 
				{
					Usart1ReceiveCounter = 0;
				}
			break;
			case 1:
				if((Usart1Data[Usart1ReceiveCounter] == 0x00) || (Usart1Data[Usart1ReceiveCounter] == sys_addr))//��ַ
				{
					
					Usart1ReceiveCounter ++;
				}else 
				{
					Usart1ReceiveCounter = 0;
				}
				break;
			case 2:
				if(Usart1Data[Usart1ReceiveCounter] == (sys_password >> 8))// �����λ
				{
					
					Usart1ReceiveCounter ++;
					
				}else 
				{
					Usart1ReceiveCounter = 0;
					USART1_SENDEnevt(0x0C);
				}
				break;
				
			case 3:
				if(Usart1Data[Usart1ReceiveCounter] == (sys_password & 0x00FF))// �����λ
				{
					Usart1ReceiveCounter ++;
					
				}else 
				{
					Usart1ReceiveCounter = 0;
					USART1_SENDEnevt(0x0C);
				}
				break;
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
				Usart1ReceiveCounter ++;
				break;
			case 13:
				checknumber = 0x00;
				 for (i = 1; i < 13; i++)
				{
					checknumber += Usart1Data[i];
				}
				if(Usart1Data[Usart1ReceiveCounter] == checknumber)
				{
					Usart1ReceiveCounter ++;
					
				}else 
				{
					Usart1ReceiveCounter = 0;
				}
				break;
			case 14:
			if(Usart1Data[Usart1ReceiveCounter] == 0xAA)
			{
				receive_flag = 1;//ִ�д����ж�
			}
			Usart1ReceiveCounter = 0;
			break;	
		}
		

	} 
}




void USART2_IRQHandler(void)                	//����2�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Res = USART_ReceiveData(USART2);//(USART2->DR);	//��ȡ���յ�������
		switch(Res)
		{
			case '$':
				GPSDATA_number=0;		//GPS�����������
				GPSDATA_mode=1;			//��������ģʽ
				GPSDATA_count=0;		//����λ�����
				break;
			
			case ',':
				GPSDOT_count++;		//���ż�����1
				GPSDATA_count=0;
				break;
			
			case '*':
				switch(GPSDATA_number)
				{
					case 1:
						Data_legitimate|=0x81;//GPGGA
						break;
					case 2:
						Data_legitimate|=0x82;//GPGSV
						break;
					case 3:
						Data_legitimate|=0x84;//GPRMC
						break;
				}
				GPSDATA_mode=0;
				break;
				
			default:
				if(GPSDATA_mode==1)													//���������ж�
				{
					GPSDATA_cmd[GPSDATA_count]=Res;			//�����ַ��������ͻ���
					if(GPSDATA_count==4)											//����������ݽ�����ϣ��ж�����
					{											
						if(GPSDATA_cmd[0]=='G'&&GPSDATA_cmd[1]=='P'&&GPSDATA_cmd[2]=='G'&&GPSDATA_cmd[3]=='G'&&GPSDATA_cmd[4]=='A')//�ж�GPGGA
						{
							GPSDATA_number=1;
							GPSDATA_mode=2;
							GPSDOT_count=0;
							GPSDATA_count=0;
						}
						
						if(GPSDATA_cmd[0]=='G'&&GPSDATA_cmd[1]=='P'&&GPSDATA_cmd[2]=='G'&&GPSDATA_cmd[3]=='S'&&GPSDATA_cmd[4]=='V')//�ж�GPGSV
						{
							GPSDATA_number=2;
							GPSDATA_mode=2;
							GPSDOT_count=0;
							GPSDATA_count=0;
						}
						
						if(GPSDATA_cmd[0]=='G'&&GPSDATA_cmd[1]=='P'&&GPSDATA_cmd[2]=='R'&&GPSDATA_cmd[3]=='M'&&GPSDATA_cmd[4]=='C')//�ж�GPRMC
						{
							GPSDATA_number=3;
							GPSDATA_mode=2;
							GPSDOT_count=0;
							GPSDATA_count=0;
						}
					}
				}
				else if(GPSDATA_mode==2)					//�������ݴ���
				{
					switch (GPSDATA_number)
					{
						case 1:												//����1���ݽ��ա�GPGGA
							switch(GPSDOT_count)
							{
								case 2:										//γ�ȴ���
									if(GPSDATA_count<9)
										GPS_latitude[GPSDATA_count]=Res;
									break;
								case 3:										//γ�ȷ�����
									if(GPSDATA_count<1)
										GPS_latitude_dir=Res;
									break;
								case 4:										//���ȴ���
									if(GPSDATA_count<10)
										GPS_longitude[GPSDATA_count]=Res;
									break;
								case 5:										//���ȷ�����
									if(GPSDATA_count<1)
										GPS_longitude_dir=Res;
									break;
								case 6:										//��λ�ж�
									if(GPSDATA_count<1)
										GPS_lock=Res;
									break;
								case 7:										//GPSʹ�����Ǹ���
									if(GPSDATA_count<2)
										GPSUSE_satellite[GPSDATA_count]=Res;
									break;
								case 9:										//���θ߶ȴ���
									if(GPSDATA_count<6)
										GPS_altitude[GPSDATA_count]=Res;
									break;
							}
							break;
							
						case 2:												//����2���ݽ��ա�GPGSV
							switch(GPSDOT_count)
							{
								case 3:										//GPS�ɼ����Ǹ���
									if(GPSDATA_count<2)
										GPSVISIBLE_satellite[GPSDATA_count]=Res;
									break;
							}
						break;
							
						case 3:												//����3���ݽ��ա�GPRMC
							switch(GPSDOT_count)
							{
								case 1:										//ʱ�䴦��
									if(GPSDATA_count<6)		
										GPS_time[GPSDATA_count]=Res;	
									break;
								case 2:										//��λ�ж�						
									if(GPSDATA_count<1)
									{
									  if(Res=='A')
											GPS_lock=1;
									  if(Res=='V')
									    GPS_lock=0;
									}
									break;
								case 3:										//γ�ȴ���						
									if(GPSDATA_count<9)
									{
										GPS_latitude[GPSDATA_count]=Res;
									}
									break;
								case 4:										//γ�ȷ�����						
									if(GPSDATA_count<1)
									{
										GPS_latitude_dir=Res;
									}
									break;
								case 5:										//���ȴ���						
									if(GPSDATA_count<10)
									{
										GPS_longitude[GPSDATA_count]=Res;
									}
									break;
								case 6:										//���ȷ�����						
									if(GPSDATA_count<1)
									{
										GPS_longitude_dir=Res;
									}
									break;
								case 7:										//�ٶȴ���						
									if(GPSDATA_count<5)
									{
										GPS_speed[GPSDATA_count]=Res;
									}
									break;
								case 8:										//��λ�Ǵ���						
									if(GPSDATA_count<5)
									{
										GPS_Angle[GPSDATA_count]=Res;
									}
									break;
								case 9:										//����				
									if(GPSDATA_count<6)
									{
										GPS_date[GPSDATA_count]=Res;
										#if	SYSTEM_SUPPORT_LOCAL	//��SYSTEM_SUPPORT_LOCAL������,˵��Ҫ֧�ֱ���Уʱ
										#else	//��֧�ֱ���ʱ��Уʱ��֧��GPSʱ��Уʱ
										gps_clock_dev.hour = (GPS_time[0] - 0x30) * 10 + (GPS_time[1] - 0x30);
										gps_clock_dev.min =  (GPS_time[2] - 0x30) * 10 + (GPS_time[3] - 0x30);
										gps_clock_dev.sec =  (GPS_time[4] - 0x30) * 10 + (GPS_time[5] - 0x30);
										gps_clock_dev.hour += 8;//8��ʱ��
										gps_clock_dev.hour %= 24;										
										if(gps_clock_dev.hour == aoto_hour)
										{
											if(gps_clock_dev.min == aoto_min)
											{
												if(gps_clock_dev.sec == aoto_sec)
												{
													School_flag = 1;
												}
											}
										}
										#endif
									}
									break;
							}
							break;
					}
				}
				GPSDATA_count++;		//������λ��1
				break;
		}
	}
}


//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM3_flag = 1;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
	}
}

