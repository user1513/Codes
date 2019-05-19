#include "gps.h" 
#include "delay.h" 								   								   
#include "string.h"	 
//RTrobot.org
//��ֹ������ҵ��Ϊ

/********************************************************************************************
GPS���ݽ���
********************************************************************************************/
void GPS_Analysis(gps_process_data *gps_data,unsigned char *buf)
{
	if((strstr((const char *)buf,"$GPGSV")))//GPGSV����
		GPGSV_Analysis(gps_data,buf);

	if((strstr((const char *)buf,"$GNGGA")))//GPGGA����
		GPGGA_Analysis(gps_data,buf);

	if((strstr((const char *)buf,"$GPRMC")))//GPRMC����
		GPRMC_Analysis(gps_data,buf);
	if((strstr((const char *)buf,"$GPGSA")))//GPGSA����	
		GPGSA_Analysis(gps_data,buf);
}

/********************************************************************************************
����GPGSV����
********************************************************************************************/
void GPGSV_Analysis(gps_process_data *gps_data,unsigned char *buf)
{
	unsigned char *temp_buf,*address_buf,end_buf,decimal_places;  	 
	temp_buf=buf;
	address_buf=(unsigned char*)strstr((const char *)temp_buf,"$GPGSV");
	end_buf=Data_Removal(address_buf,3);									 										//����ȥ��
	if(end_buf!=0XFF)
		gps_data->svnum=Data_Extraction(address_buf+end_buf,&decimal_places);		//�õ���֪��������
}

/********************************************************************************************
����GPGGA����
********************************************************************************************/
void GPGGA_Analysis(gps_process_data *gps_data,unsigned char *buf)
{
	unsigned char *address_buf,end_buf,decimal_places;					 
	unsigned long int temp;	   
	float rs;  
	
	address_buf =(unsigned char*)strstr((const char *)buf,"$GNGGA");
	end_buf=Data_Removal(address_buf,1);																			//����ȥ��								
  if(end_buf!=0XFF)
	temp	=(Data_Extraction(address_buf+end_buf,&decimal_places))/100;	//�õ�GPS״̬
	
	gps_data->utc.hour=(temp/10000+8)%24;
	gps_data->utc.min=temp%10000/100;
	gps_data->utc.sec=temp%100;

	end_buf=Data_Removal(address_buf,2);																			//����ȥ��								
	if(end_buf!=0XFF)
	{
		temp=Data_Extraction(address_buf+end_buf,&decimal_places);		 	 
		gps_data->latitude=temp/MN_Process(10,decimal_places+2);
		rs=temp%MN_Process(10,decimal_places+2);
		gps_data->latitude=gps_data->latitude*MN_Process(10,6)+(rs*MN_Process(10,5-decimal_places))/6;//�õ�γ��
	}
	
	
	end_buf=Data_Removal(address_buf,3);																			//����ȥ��								
	if(end_buf!=0XFF)
		gps_data->nshemi=*(address_buf+end_buf);																//��γ���Ǳ�γ 			

	
 	end_buf=Data_Removal(address_buf,4);																			//����ȥ��									
	if(end_buf!=0XFF)
	{												  
		temp=Data_Extraction(address_buf+end_buf,&decimal_places);		 	 
		gps_data->longitude=temp/MN_Process(10,decimal_places+2);
		rs=temp%MN_Process(10,decimal_places+2); 
		gps_data->longitude=gps_data->longitude*MN_Process(10,6)+(rs*MN_Process(10,5-decimal_places))/6;//�õ�����
	}
	
	end_buf=Data_Removal(address_buf,5);																			//����ȥ��
	if(end_buf!=0XFF)
		gps_data->ewhemi=*(address_buf+end_buf);																//������������	
	end_buf=Data_Removal(address_buf,6);																			//����ȥ��				
  if(end_buf!=0XFF)
		gps_data->gpssta=Data_Extraction(address_buf+end_buf,&decimal_places);	//�õ�GPS״̬
	
	end_buf=Data_Removal(address_buf,7);																			//����ȥ��		
	if(end_buf!=0XFF)
		gps_data->posslnum=Data_Extraction(address_buf+end_buf,&decimal_places);//�õ����ڶ�λ��������
  end_buf=Data_Removal(address_buf,6);																			//����ȥ��				
  if(end_buf!=0XFF)
		gps_data->gpssta=Data_Extraction(address_buf+end_buf,&decimal_places);	//�õ�GPS״̬
	
	end_buf=Data_Removal(address_buf,7);																			//����ȥ��		
	if(end_buf!=0XFF)
		gps_data->posslnum=Data_Extraction(address_buf+end_buf,&decimal_places);//�õ����ڶ�λ��������
}

/********************************************************************************************
����GPRMC����
********************************************************************************************/
void GPRMC_Analysis(gps_process_data *gps_data,unsigned char *buf)
{
	unsigned char *address_buf,end_buf,decimal_places;		 
	unsigned long int temp;	   
	float rs;  
	address_buf =(unsigned char*)strstr((const char *)buf,"GPRMC");

	end_buf=Data_Removal(address_buf,3);																			//����ȥ��								
	if(end_buf!=0XFF)
	{
		temp=Data_Extraction(address_buf+end_buf,&decimal_places);		 	 
		gps_data->latitude=temp/MN_Process(10,decimal_places+2);
		rs=temp%MN_Process(10,decimal_places+2);
		gps_data->latitude=gps_data->latitude*MN_Process(10,5)+(rs*MN_Process(10,5-decimal_places))/60;//�õ�γ��
	}
	
	
	end_buf=Data_Removal(address_buf,4);																			//����ȥ��								
	if(end_buf!=0XFF)
		gps_data->nshemi=*(address_buf+end_buf);																//��γ���Ǳ�γ 			

	
 	end_buf=Data_Removal(address_buf,5);																			//����ȥ��									
	if(end_buf!=0XFF)
	{												  
		temp=Data_Extraction(address_buf+end_buf,&decimal_places);		 	 
		gps_data->longitude=temp/MN_Process(10,decimal_places+2);
		rs=temp%MN_Process(10,decimal_places+2); 
		gps_data->longitude=gps_data->longitude*MN_Process(10,5)+(rs*MN_Process(10,5-decimal_places))/60;//�õ�����
	}
	
	end_buf=Data_Removal(address_buf,6);																			//����ȥ��
	if(end_buf!=0XFF)
		gps_data->ewhemi=*(address_buf+end_buf);																//������������		 
}  

/********************************************************************************************
����GPGSA����
********************************************************************************************/
void GPGSA_Analysis(gps_process_data *gps_data,unsigned char *buf)
{
	unsigned char *address_buf,end_buf,decimal_places;			   
	address_buf=(unsigned char*)strstr((const char *)buf,"$GPGSA");
	end_buf=Data_Removal(address_buf,2);
	
	if(end_buf!=0XFF)
		gps_data->fixmode=Data_Extraction(address_buf+end_buf,&decimal_places);	//�õ���λ����
}

/********************************************************************************************
����ȥ��
********************************************************************************************/
unsigned char Data_Removal(unsigned char *buf,unsigned char address_buf)
{	 		    
	unsigned char *temp_buf=buf;
	while(address_buf)
	{		 
		if(*buf=='*'||*buf<' '||*buf>'z')
			return 0XFF;
		if(*buf==',')address_buf--;
			buf++;
	}
	return buf-temp_buf;	 
}

/********************************************************************************************
m^n����
********************************************************************************************/
unsigned long int MN_Process(unsigned char m,unsigned char n)
{
	unsigned long int result=1;	 
	while(n--)
		result*=m;    
	return result;
}

/********************************************************************************************
��ȡ��Ҫ������
********************************************************************************************/
int Data_Extraction(unsigned char *buf,unsigned char*decimal_places)
{
	unsigned char *temp_buf=buf;
	unsigned long int intbuf=0,fres=0;
	unsigned char address_int=0,address_decimal_places=0,i,mask=0;
	int res;
	while(1)
	{
		if(*temp_buf=='-')
		{
			mask|=0X02;
			temp_buf++;
		}
		
		if(*temp_buf==','||(*temp_buf=='*'))
			break;
		
		if(*temp_buf=='.')
		{
			mask|=0X01;
			temp_buf++;
		}
		
		else if(*temp_buf>'9'||(*temp_buf<'0'))
		{	
			address_int=0;
			address_decimal_places=0;
			break;
		}
		
		if(mask&0X01)
			address_decimal_places++;
		else 
			address_int++;
		
		temp_buf++;
	}
	
	if(mask&0X02)
		buf++;
	for(i=0;i<address_int;i++)
	{  
		intbuf+=MN_Process(10,address_int-1-i)*(buf[i]-'0');
	}
	
	if(address_decimal_places>6)
		address_decimal_places=6;
	
	*decimal_places=address_decimal_places;	 		
	
	for(i=0;i<address_decimal_places;i++)
	{  
		fres+=MN_Process(10,address_decimal_places-1-i)*(buf[address_int+1+i]-'0');
	} 
	
	res=intbuf*MN_Process(10,address_decimal_places)+fres;
	
	if(mask&0X02)
		res=-res;		   
	
	return res;
}	 

