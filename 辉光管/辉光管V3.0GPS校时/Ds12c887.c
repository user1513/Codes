#include <Ds12c887.h>

///************************************************************* 
//函数功能：该函数用来启动时钟芯片工作 
//应用范围：仅在时钟芯片首次使用时用到一次 【若时钟不走调用一次】
//入口参数： 
//出口参数： 
//*************************************************************/ 

//void StartDs12c887(void)
//{
//		write_ds12c887(0x0A,0x20);
////  chRegA = CMD_START_DS12C887; 
//}

///************************************************************* 
//函数功能：该函数用来关闭时钟芯片 
//应用范围：一般用不到 
//入口参数： 
//出口参数： 
//*************************************************************/ 
//void CloseDs12c887(void) 
//{ 
//			write_ds12c887(0x0A,0x30);
////		chRegA = CMD_CLOSE_DS12C887; 
//} 


/************************************************************* 
函数功能：该函数用来写数据
应用范围： 
入口参数： 
出口参数： 
*************************************************************/ 

void write_ds12c887(uchar add,uchar date)
{
		dscs=0;
		dsas=1;
		dsds=1;
		dsrw=1;
		P0=add;
		dsas=0;
		dsrw=0;
		P0=date;
		dsrw=1;
		dsas=1;
		dscs=1;				
}
/************************************************************* 
函数功能：该函数用来读数据
应用范围： 
入口参数： 
出口参数： 
*************************************************************/ 
uchar read_ds12c887(uchar add)
{
		unsigned char ds_date;
		dsas=1;
		dsds=1;
		dsrw=1;
		dscs=0;
		P0=add;
		dsas=0;
		dsds=0;
		P0=0xff;
		ds_date=P0;
		dsds=1;
		dsas=1;
		dscs=1;
		return ds_date;	
}
/************************************************************* 
函数功能：该函数用来设定DS12C887闹钟
应用范围： 
入口参数： 
出口参数： 
*************************************************************/ 
void set_alarm(uchar ashi,uchar afen,uchar amiao)
{
		write_ds12c887(1,amiao);
		write_ds12c887(3,afen);
		write_ds12c887(5,ashi);
}
/************************************************************* 
函数功能：该函数用来设定DS12C887时间
应用范围： 
入口参数： 
出口参数： 
*************************************************************/ 
void SetTime(uchar shi,uchar fen,uchar miao)
{
		write_ds12c887(4,shi);
		write_ds12c887(2,fen);
		write_ds12c887(0,miao);
}
/************************************************************* 
函数功能：该函数用来设定DS12C887日期
应用范围： 
入口参数： 
出口参数： 
*************************************************************/ 
void SetDate(uchar year,uchar month,uchar date)
{
		write_ds12c887(9,year);
		write_ds12c887(8,month);
		write_ds12c887(7,date);
}
/************************************************************* 
函数功能：该函数用来设定DS12C887星期
应用范围： 
入口参数： 
出口参数： 
*************************************************************/ 
void SetDay(uchar day)
{
		write_ds12c887(6,day);
}
