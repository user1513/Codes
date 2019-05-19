/*************************************************************
                     流水灯测试程序
程序编写：Fucp   2015-2-4
DIY套件  http://59tiaoba.taobao.com

单片机 IAP15W4K61S4  或者其他stc15w系列 晶振18.432M

/***********************************************************
说明：此程序用于测试LED焊接好坏


*************************************************************/
#include "NEW_8051.H"
#define uchar unsigned char
#define uint unsigned int    //宏定义


sbit D1=P0^4; //定义每个IO控制的LED
sbit D2=P0^3;  
sbit D3=P0^2;
sbit D4=P0^1;
sbit D5=P0^0;
sbit D6=P4^6;  
sbit D7=P4^5;
sbit D8=P7^7;	
sbit D9=P2^3;
sbit D10=P2^4;  
sbit D11=P2^5;
sbit D12=P2^6;	
sbit D13=P2^7;
sbit D14=P7^4;  
sbit D15=P7^5;
sbit D16=P7^6;

	
sbit D17=P2^1;
sbit D18=P2^0;  
sbit D19=P4^4;
sbit D20=P4^3;	
sbit D21=P4^2;
sbit D22=P6^3;  
sbit D23=P1^0;
sbit D24=P1^1;	
sbit D25=P4^7;
sbit D26=P1^2;  
sbit D27=P1^3;
sbit D28=P1^4;
sbit D29=P1^5;  
sbit D30=P1^6;
sbit D31=P1^7;	
sbit D32=P5^4;
sbit D33=P5^5;  
sbit D34=P4^0;
sbit D35=P6^5;	
sbit D36=P6^6;
sbit D37=P6^7;  
sbit D38=P3^4;
sbit D39=P5^0;	
sbit D40=P3^6;
sbit D41=P3^5;  
sbit D42=P5^1;
sbit D43=P3^7;	
sbit D44=P7^0;
sbit D45=P7^1;  
sbit D46=P7^2;
sbit D47=P7^3;	
sbit D48=P4^1;
	
uchar DataLow[4];
unsigned long CeL=0;
/***************************************/
void Show_R(uchar r1,uchar r2)//数据输出IO口  R是右边 16灯
{
 	D1=(bit)(r1&0x01);
	D2=(bit)(r1&0x02);
	D3=(bit)(r1&0x04);
	D4=(bit)(r1&0x08);
	D5=(bit)(r1&0x10);
	D6=(bit)(r1&0x20);
	D7=(bit)(r1&0x40);
	D8=(bit)(r1&0x80);

	D9=(bit)(r2&0x01);
	D10=(bit)(r2&0x02);
	D11=(bit)(r2&0x04);
	D12=(bit)(r2&0x08);
	D13=(bit)(r2&0x10);
	D14=(bit)(r2&0x20);
	D15=(bit)(r2&0x40);
	D16=(bit)(r2&0x80);
			
}
void Show_P(uchar l1,uchar l2,uchar l3,uchar l4)//数据输出IO口  L是左边 32灯
{
 	D17=(bit)(l1&0x01);
	D18=(bit)(l1&0x02);
	D19=(bit)(l1&0x04);
	D20=(bit)(l1&0x08);
	D21=(bit)(l1&0x10);
	D22=(bit)(l1&0x20);
	D23=(bit)(l1&0x40);
	D24=(bit)(l1&0x80);

	D25=(bit)(l2&0x01);
	D26=(bit)(l2&0x02);
	D27=(bit)(l2&0x04);
	D28=(bit)(l2&0x08);
	D29=(bit)(l2&0x10);
	D30=(bit)(l2&0x20);
	D31=(bit)(l2&0x40);
	D32=(bit)(l2&0x80);

	D33=(bit)(l3&0x01);
	D34=(bit)(l3&0x02);
	D35=(bit)(l3&0x04);
	D36=(bit)(l3&0x08);
	D37=(bit)(l3&0x10);
	D38=(bit)(l3&0x20);
	D39=(bit)(l3&0x40);
	D40=(bit)(l3&0x80);

	D41=(bit)(l4&0x01);
	D42=(bit)(l4&0x02);
	D43=(bit)(l4&0x04);
	D44=(bit)(l4&0x08);
	D45=(bit)(l4&0x10);
	D46=(bit)(l4&0x20);
	D47=(bit)(l4&0x40);
	D48=(bit)(l4&0x80);
			
}
void delayMs(void)  //误差 -0.054253472218us 300ms 
{
    unsigned char a,b,c;
    for(c=83;c>0;c--)
        for(b=68;b>0;b--)
            for(a=203;a>0;a--);
}

/*****主函数*****/
void main(void)
{
    uchar l=0;
	P0M0=0X00;	//IO口初始化准双向IO
	P0M1=0X00;

	P1M0=0X00;
	P1M1=0X00;

	P2M0=0X00;
	P2M1=0X00;

	P3M0=0X00;
	P3M1=0X00;

	P4M0=0X00;
	P4M1=0X00;

	P5M0=0X00;
	P5M1=0X00;

	P6M0=0X00;
	P6M1=0X00;

	P7M0=0X00;
	P7M1=0X00;
		
    //主程序中
   	while(1)
    {
          //	CeR=0x0001;
			CeL=0x00010001;
			for(l=0;l<16;l++)
			{
				DataLow[0]=CeL;
				DataLow[1]=CeL>>8;
				DataLow[2]=CeL>>16;
				DataLow[3]=CeL>>24;	
				   	
          		Show_P(~DataLow[0],~DataLow[1],~DataLow[2],~DataLow[3]);
			    Show_R(~DataLow[0],~DataLow[1]);
				delayMs(); 
				CeL=CeL<<1;
				 
               
            }     
      }   
    
}
/*********************************************************************************/
/*************************************************************************************
************************************************************************************
***********************                                     ***************************
*************DIY视界出品   http://59tiaoba.taobao.com       ***************************
***********************                                     ***************************
************************************************************************************
**********************************************************************************/