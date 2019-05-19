#ifndef __GPS_H
#define __GPS_H	   
//RTrobot.org
//��ֹ������ҵ��Ϊ

/******************************************************************************************** 
������Ϣ
********************************************************************************************/
__packed typedef struct  
{										    
 	unsigned char number;			//���Ǳ��
	unsigned char elevation;		//��������
	unsigned long int azimuth;	//���Ƿ�λ��
	unsigned char sig;					//�����		   
}gps_satellite;  

/******************************************************************************************** 
UTCʱ����Ϣ
********************************************************************************************/
__packed typedef struct  
{										    
 	unsigned long int year;	//���
	unsigned char month;	//�·�
	unsigned char date;	//����
	unsigned char hour; 	//Сʱ
	unsigned char min; 	//����
	unsigned char sec; 	//����
}gps_utc_time;   	   

/******************************************************************************************** 
���������ݴ�Žṹ��
********************************************************************************************/
__packed typedef struct  
{										    

	gps_satellite satellite[12];			//���12������
	gps_utc_time utc;									//UTCʱ��
	unsigned char svnum;							//�ɼ�������
	unsigned long int latitude;			//γ�� ������100000��,ʵ��Ҫ����100000
	unsigned char nshemi;						//��γ/��γ,N:��γ;S:��γ				  
	unsigned long int longitude;		  //���� ������100000��,ʵ��Ҫ����100000
	unsigned char ewhemi;						//����/����,E:����;W:����
	unsigned char gpssta;						//GPS״̬:0,δ��λ;1,�ǲ�ֶ�λ;2,��ֶ�λ;6,���ڹ���.				  
 	unsigned char posslnum;					//���ڶ�λ��������,0~12.
	unsigned char fixmode;					//��λ����:1,û�ж�λ;2,2D��λ;3,3D��λ
}gps_process_data; 
				 
unsigned char Data_Removal(unsigned char *buf,unsigned char cx);
int Data_Extraction(unsigned char *buf,unsigned char*dx);
void GPS_Analysis(gps_process_data *gps_data,unsigned char *buf);
unsigned long int MN_Process(unsigned char m,unsigned char n);
void GPGSV_Analysis(gps_process_data *gps_data,unsigned char *buf);
void GPGGA_Analysis(gps_process_data *gps_data,unsigned char *buf);
void GPRMC_Analysis(gps_process_data *gps_data,unsigned char *buf);
void GPGSA_Analysis(gps_process_data *gps_data,unsigned char *buf);
#endif  
