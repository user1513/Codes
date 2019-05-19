#include "gps.h" 
#include "delay.h" 								   								   
#include "string.h"	 
//RTrobot.org
//禁止用于商业行为

/********************************************************************************************
GPS数据解析
********************************************************************************************/
void GPS_Analysis(gps_process_data *gps_data,unsigned char *buf)
{
	if((strstr((const char *)buf,"$GPGSV")))//GPGSV解析
		GPGSV_Analysis(gps_data,buf);

	if((strstr((const char *)buf,"$GNGGA")))//GPGGA解析
		GPGGA_Analysis(gps_data,buf);

	if((strstr((const char *)buf,"$GPRMC")))//GPRMC解析
		GPRMC_Analysis(gps_data,buf);
	if((strstr((const char *)buf,"$GPGSA")))//GPGSA解析	
		GPGSA_Analysis(gps_data,buf);
}

/********************************************************************************************
解析GPGSV数据
********************************************************************************************/
void GPGSV_Analysis(gps_process_data *gps_data,unsigned char *buf)
{
	unsigned char *temp_buf,*address_buf,end_buf,decimal_places;  	 
	temp_buf=buf;
	address_buf=(unsigned char*)strstr((const char *)temp_buf,"$GPGSV");
	end_buf=Data_Removal(address_buf,3);									 										//数据去除
	if(end_buf!=0XFF)
		gps_data->svnum=Data_Extraction(address_buf+end_buf,&decimal_places);		//得到已知卫星总数
}

/********************************************************************************************
解析GPGGA数据
********************************************************************************************/
void GPGGA_Analysis(gps_process_data *gps_data,unsigned char *buf)
{
	unsigned char *address_buf,end_buf,decimal_places;					 
	unsigned long int temp;	   
	float rs;  
	
	address_buf =(unsigned char*)strstr((const char *)buf,"$GNGGA");
	end_buf=Data_Removal(address_buf,1);																			//数据去除								
  if(end_buf!=0XFF)
	temp	=(Data_Extraction(address_buf+end_buf,&decimal_places))/100;	//得到GPS状态
	
	gps_data->utc.hour=(temp/10000+8)%24;
	gps_data->utc.min=temp%10000/100;
	gps_data->utc.sec=temp%100;

	end_buf=Data_Removal(address_buf,2);																			//数据去除								
	if(end_buf!=0XFF)
	{
		temp=Data_Extraction(address_buf+end_buf,&decimal_places);		 	 
		gps_data->latitude=temp/MN_Process(10,decimal_places+2);
		rs=temp%MN_Process(10,decimal_places+2);
		gps_data->latitude=gps_data->latitude*MN_Process(10,6)+(rs*MN_Process(10,5-decimal_places))/6;//得到纬度
	}
	
	
	end_buf=Data_Removal(address_buf,3);																			//数据去除								
	if(end_buf!=0XFF)
		gps_data->nshemi=*(address_buf+end_buf);																//南纬还是北纬 			

	
 	end_buf=Data_Removal(address_buf,4);																			//数据去除									
	if(end_buf!=0XFF)
	{												  
		temp=Data_Extraction(address_buf+end_buf,&decimal_places);		 	 
		gps_data->longitude=temp/MN_Process(10,decimal_places+2);
		rs=temp%MN_Process(10,decimal_places+2); 
		gps_data->longitude=gps_data->longitude*MN_Process(10,6)+(rs*MN_Process(10,5-decimal_places))/6;//得到经度
	}
	
	end_buf=Data_Removal(address_buf,5);																			//数据去除
	if(end_buf!=0XFF)
		gps_data->ewhemi=*(address_buf+end_buf);																//东经还是西经	
	end_buf=Data_Removal(address_buf,6);																			//数据去除				
  if(end_buf!=0XFF)
		gps_data->gpssta=Data_Extraction(address_buf+end_buf,&decimal_places);	//得到GPS状态
	
	end_buf=Data_Removal(address_buf,7);																			//数据去除		
	if(end_buf!=0XFF)
		gps_data->posslnum=Data_Extraction(address_buf+end_buf,&decimal_places);//得到用于定位的卫星数
  end_buf=Data_Removal(address_buf,6);																			//数据去除				
  if(end_buf!=0XFF)
		gps_data->gpssta=Data_Extraction(address_buf+end_buf,&decimal_places);	//得到GPS状态
	
	end_buf=Data_Removal(address_buf,7);																			//数据去除		
	if(end_buf!=0XFF)
		gps_data->posslnum=Data_Extraction(address_buf+end_buf,&decimal_places);//得到用于定位的卫星数
}

/********************************************************************************************
解析GPRMC数据
********************************************************************************************/
void GPRMC_Analysis(gps_process_data *gps_data,unsigned char *buf)
{
	unsigned char *address_buf,end_buf,decimal_places;		 
	unsigned long int temp;	   
	float rs;  
	address_buf =(unsigned char*)strstr((const char *)buf,"GPRMC");

	end_buf=Data_Removal(address_buf,3);																			//数据去除								
	if(end_buf!=0XFF)
	{
		temp=Data_Extraction(address_buf+end_buf,&decimal_places);		 	 
		gps_data->latitude=temp/MN_Process(10,decimal_places+2);
		rs=temp%MN_Process(10,decimal_places+2);
		gps_data->latitude=gps_data->latitude*MN_Process(10,5)+(rs*MN_Process(10,5-decimal_places))/60;//得到纬度
	}
	
	
	end_buf=Data_Removal(address_buf,4);																			//数据去除								
	if(end_buf!=0XFF)
		gps_data->nshemi=*(address_buf+end_buf);																//南纬还是北纬 			

	
 	end_buf=Data_Removal(address_buf,5);																			//数据去除									
	if(end_buf!=0XFF)
	{												  
		temp=Data_Extraction(address_buf+end_buf,&decimal_places);		 	 
		gps_data->longitude=temp/MN_Process(10,decimal_places+2);
		rs=temp%MN_Process(10,decimal_places+2); 
		gps_data->longitude=gps_data->longitude*MN_Process(10,5)+(rs*MN_Process(10,5-decimal_places))/60;//得到经度
	}
	
	end_buf=Data_Removal(address_buf,6);																			//数据去除
	if(end_buf!=0XFF)
		gps_data->ewhemi=*(address_buf+end_buf);																//东经还是西经		 
}  

/********************************************************************************************
解析GPGSA数据
********************************************************************************************/
void GPGSA_Analysis(gps_process_data *gps_data,unsigned char *buf)
{
	unsigned char *address_buf,end_buf,decimal_places;			   
	address_buf=(unsigned char*)strstr((const char *)buf,"$GPGSA");
	end_buf=Data_Removal(address_buf,2);
	
	if(end_buf!=0XFF)
		gps_data->fixmode=Data_Extraction(address_buf+end_buf,&decimal_places);	//得到定位类型
}

/********************************************************************************************
数据去除
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
m^n函数
********************************************************************************************/
unsigned long int MN_Process(unsigned char m,unsigned char n)
{
	unsigned long int result=1;	 
	while(n--)
		result*=m;    
	return result;
}

/********************************************************************************************
提取需要的数据
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

