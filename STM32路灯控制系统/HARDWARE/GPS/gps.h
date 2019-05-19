#ifndef __GPS_H
#define __GPS_H	   
//RTrobot.org
//禁止用于商业行为

/******************************************************************************************** 
卫星信息
********************************************************************************************/
__packed typedef struct  
{										    
 	unsigned char number;			//卫星编号
	unsigned char elevation;		//卫星仰角
	unsigned long int azimuth;	//卫星方位角
	unsigned char sig;					//信噪比		   
}gps_satellite;  

/******************************************************************************************** 
UTC时间信息
********************************************************************************************/
__packed typedef struct  
{										    
 	unsigned long int year;	//年份
	unsigned char month;	//月份
	unsigned char date;	//日期
	unsigned char hour; 	//小时
	unsigned char min; 	//分钟
	unsigned char sec; 	//秒钟
}gps_utc_time;   	   

/******************************************************************************************** 
解析后数据存放结构体
********************************************************************************************/
__packed typedef struct  
{										    

	gps_satellite satellite[12];			//最多12颗卫星
	gps_utc_time utc;									//UTC时间
	unsigned char svnum;							//可见卫星数
	unsigned long int latitude;			//纬度 分扩大100000倍,实际要除以100000
	unsigned char nshemi;						//北纬/南纬,N:北纬;S:南纬				  
	unsigned long int longitude;		  //经度 分扩大100000倍,实际要除以100000
	unsigned char ewhemi;						//东经/西经,E:东经;W:西经
	unsigned char gpssta;						//GPS状态:0,未定位;1,非差分定位;2,差分定位;6,正在估算.				  
 	unsigned char posslnum;					//用于定位的卫星数,0~12.
	unsigned char fixmode;					//定位类型:1,没有定位;2,2D定位;3,3D定位
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
