#include <REGX51.H>
#include <MAX6921.h>
#include <Ds12c887.h>
#include <GPS.h>
#include <UART.h>
#include <intrins.h>
#ifndef uchar
#define uchar unsigned char 
#endif

#ifndef uint
#define uint unsigned int 
#endif

extern unsigned char TIME[7];

/*******************************************************************************
* ������         : IrInit()
* ��������		   : ��ʼ�������߽���
* ����           : ��
* ���         	 : ��
*******************************************************************************/
//sbit IRIN = P3^3;
//void IrInit()
//{
//	IT0=1;//�½��ش���
//	EX0=1;//���ж�0����
//	EA=1;	//�����ж�

//	IRIN=1;//��ʼ���˿�
//}

void init()
{
	//��ʼ���ж�Ƶ��
/*
	EA=1;
	EX0=1;
	IT0=1;
*/
	Uart_Init();//���ڳ�ʼ������
	
	write_ds12c887(0x0B,0x2e);//SET PIE AIE UIE SWQE DM 24/12 DSE
	read_ds12c887(0x0c);
	
//	SetTime(13,35,50);				//��ʼ��ʱ��
//	SetDate(16,10,9);				//��ʼ������
//	SetDay(7);							//��ʼ������
//	set_alarm(21,26,00);			//��ʼ������

	write_ds12c887(0x0a,0x20);  
	
}

#define CorrectHour 12
#define CorrectMin  01

sbit KEY1 = P2^1;
sbit KEY2 = P2^2;


/*******************************************************************************
* ������          : Delay10ms
* ��������		     : ��ʱ��������ʱ10ms
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void Delay10ms(void)		
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 108;
	j = 144;
	do
	{
		while (--j);
	} while (--i);
}


unsigned char KeyScan(void)
{
	unsigned char keyValue = 0 ; //�����ֵ
	//--��ⰴ��1--//
	if (KEY1==0)		//��ⰴ��K1�Ƿ���
	{
		Delay10ms();	//��������
		if (KEY1==0)	//�ٴμ�ⰴ���Ƿ���
		{
			keyValue = 1;
			while(!KEY1);
		}
	}
	//--��ⰴ��2--//
	if (KEY2==0)		//��ⰴ��K2�Ƿ���
	{
		Delay10ms();	//��������
		if (KEY2==0)	//�ٴμ�ⰴ���Ƿ���
		{
			keyValue = 2;
			while(!KEY2);
		}
	}
	return keyValue;
}


void main()
{
	uchar hour,min,sec;
	
	uchar xdata gps_hour,gps_min,gps_sec;
	
	uchar nolocknumber = 0;//δ�����ʶ��

	
	uchar jieshouok = 0;
	
	init();										//��ʼ������
	while(1)
	{
		sec=read_ds12c887(0);	
		min=read_ds12c887(2);
		hour=read_ds12c887(4);
		display_num(8,hour/10);
		display_num(7,hour%10);
		display_num(5,min/10);
		display_num(4,min%10);
		display_num(2,sec/10);
		display_num(1,sec%10);
		
		//------------------------��ʱУʱ����-----------
		//�ж��Ƿ�Уʱ

		if((hour==CorrectHour)&&(min==CorrectMin))
		{
			if(jieshouok == 1)
			{
				SetTime(gps_hour,gps_min,gps_sec);//д��--Уʱ
			}
		}
	
		//------------------------Уʱ����-------------
		if(Data_legitimate==0)				//��GPS�ź�ʱ
		{
			jieshouok = 0;
		}
		else	//��GPS�ź�
		{		
			if(Data_legitimate|0x01)	//GPGGA����
			{			
				if(GPS_lock==0)					//δ��λ
				{
//					//ÿ�λ����ĳ���δ��λ	
					if(++nolocknumber == 250)
					{
						jieshouok = 0;
					}
				}
				else							//�Ѷ�λ
				{
					nolocknumber = 0;//�Ѷ�λ�����δ��λ����
					//����ʱ��
					gps_hour = (GPS_time[0] - 0x30) * 10 + (GPS_time[1] - 0x30);
					gps_min =  (GPS_time[2] - 0x30) * 10 + (GPS_time[3] - 0x30);
					gps_sec =  (GPS_time[4] - 0x30) * 10 + (GPS_time[5] - 0x30);
					
					gps_hour += 8;
					gps_hour %= 24;
					
					//---------------------------------------------------------------------
					if((gps_hour<24)&&(gps_min<60)&&(gps_sec<60)) //�ݴ���//��ֹ��Уʱ��һ˲�����
					{	
						jieshouok = 1;
					}else
					{
						jieshouok = 0;
					}
					//---------------------------------------------------------------------
	
				}			
				Data_legitimate&=~0x01;
			}	
		}
		//------------------------����ɨ�躯��----------
		switch(KeyScan())
		{
			case 1:if(jieshouok==1)SetTime(gps_hour,gps_min,gps_sec);break;//Уʱ
			case 2:SetTime(12,00,50);break;
			default:break;
		}
	}
}


/*****************************************************************************
�����ж�
/****************************************************************************/
void uart(void) interrupt 4
{
	uchar UART_data;
	if(RI)
	{
		UART_data=SBUF;
		
		switch(UART_data)
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
					GPSDATA_cmd[GPSDATA_count]=UART_data;			//�����ַ��������ͻ���
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
						
						/****************
						....
						���������հ�
						****************/
						
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
										GPS_latitude[GPSDATA_count]=UART_data;
									break;
								case 3:										//γ�ȷ�����
									if(GPSDATA_count<1)
										GPS_latitude_dir=UART_data;
									break;
								case 4:										//���ȴ���
									if(GPSDATA_count<10)
										GPS_longitude[GPSDATA_count]=UART_data;
									break;
								case 5:										//���ȷ�����
									if(GPSDATA_count<1)
										GPS_longitude_dir=UART_data;
									break;
								case 6:										//��λ�ж�
									if(GPSDATA_count<1)
										GPS_lock=UART_data;
									break;
								case 7:										//GPSʹ�����Ǹ���
									if(GPSDATA_count<2)
										GPSUSE_satellite[GPSDATA_count]=UART_data;
									break;
								case 9:										//���θ߶ȴ���
									if(GPSDATA_count<6)
										GPS_altitude[GPSDATA_count]=UART_data;
									break;
							}
							break;
							
						case 2:												//����2���ݽ��ա�GPGSV
							switch(GPSDOT_count)
							{
								case 3:										//GPS�ɼ����Ǹ���
									if(GPSDATA_count<2)
										GPSVISIBLE_satellite[GPSDATA_count]=UART_data;
									break;
							}
						break;
							
						case 3:												//����3���ݽ��ա�GPRMC
							switch(GPSDOT_count)
							{
								case 1:										//ʱ�䴦��
									if(GPSDATA_count<6)		
										GPS_time[GPSDATA_count]=UART_data;	
									break;
								case 2:										//��λ�ж�						
									if(GPSDATA_count<1)
									{
									  if(UART_data=='A')
											GPS_lock=1;
									  if(UART_data=='V')
									    GPS_lock=0;
									}
									break;
								case 3:										//γ�ȴ���						
									if(GPSDATA_count<9)
									{
										GPS_latitude[GPSDATA_count]=UART_data;
									}
									break;
								case 4:										//γ�ȷ�����						
									if(GPSDATA_count<1)
									{
										GPS_latitude_dir=UART_data;
									}
									break;
								case 5:										//���ȴ���						
									if(GPSDATA_count<10)
									{
										GPS_longitude[GPSDATA_count]=UART_data;
									}
									break;
								case 6:										//���ȷ�����						
									if(GPSDATA_count<1)
									{
										GPS_longitude_dir=UART_data;
									}
									break;
								case 7:										//�ٶȴ���						
									if(GPSDATA_count<5)
									{
										GPS_speed[GPSDATA_count]=UART_data;
									}
									break;
								case 8:										//��λ�Ǵ���						
									if(GPSDATA_count<5)
									{
										GPS_Angle[GPSDATA_count]=UART_data;
									}
									break;
								case 9:										//����				
									if(GPSDATA_count<6)
									{
										GPS_date[GPSDATA_count]=UART_data;
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
	RI=0;
}