/*************************************************************
                     多种效果显示程序
程序编写：Fucp   2015-5-23
DIY套件  http://59tiaoba.taobao.com

单片机 IAP15W4K61S4  或者其他51	 晶振频率 18.432M  
字模软件用  PC2002 + 圆形图片取模软件
逐列取模方式

电机旋转方向 顺时针
/***********************************************************
说明：本程序支持配套遥控器调整时间
播放按键  切换调整 时、分、秒、月、日
加 按键：增加
减 按键：减少
menu按键： 时钟复位

遥控器按键1 ：平面显示文字+侧面显示时钟	 可调时
遥控器按键2 ：平面指针时钟+侧面显示文字
遥控器按键3 ：平面数字时钟+侧面显示文字
遥控器按键4 ：平面图片+侧面显示文字
*************************************************************/
#include "NEW_8051.H"
#include "intrins.h"
#include "task.h"
#define FOSC 18432000L          //系统频率
#define BAUD 115200             //串口波特率
bit Scan_bit=0;	  
uint Offset=0;
extern void du1302P();
extern void Show_plane_time(void);
bit BIT_timeout=0;
uint OffsetAdder=0;
uchar Row=0;
uchar ucMode=1;	 //模式
bit Time0_flag=0;	//定时器T0中断标志位
uchar DISP_LINE=0;
uint Pt=50,S0,T,D;		//计数用的变量
uchar ucAdder=0;
 uchar code TAB_12369[]=
 {

	0xFF,0x03,0xFF,0xB3,0x6B,0x6B,0x9B,0xFF,		
	0x8F,0x77,0x7F,0x8F,0x7F,0x77,0x8F,0xFF,//3
	0xFF,0xFF,0x9F,0x6D,0x6D,0x6B,0x87,0xFF,//6	
	0x9F,0xEF,0xF7,0x87,0x77,0x77,0x8F,0xFF,///9


 };
/*********中断子程序**********/
void int0() interrupt 0	//使用外部中断0
{
	BIT_timeout=1; //起点标志位


		D+=(DISP_LINE-180)*2;   //修正值
   		Pt=205+D; 
	    //得到定时器T0的初设值
		TH0=-Pt/256;
		TL0=-Pt%256;   //设置定时器T0的初设值
		DISP_LINE=0;   //计数器清零，将重新计数	
	 if(ucMode==2)
	 {	
		du1302();//获取时间
		Sec=Sec/16*10+Sec%16 ;
	    Cent=Cent/16*10+Cent%16 ;
		Hour=Hour/16*10+Hour%16 ;
		if(Hour>11)
		{
			Hour=Hour-12;	
		}
		Sec=Sec*3;//秒针定位="秒"*3
		Hour=(Hour*15)+(Cent/4);//时针定位="时"*15+"分"/4
		Cent=Cent*3;//分针定位="分"*3
		Offset++;
		if(Offset>480)//侧面有多少文字 这个值就对应改成  多少字*16 
		Offset=0;			
	}
	else if(ucMode==4)
	{
		if(++Row>=40)
		{
			Row=0;
			OffsetAdder=OffsetAdder+720;//一个图片720个字节
	        if(OffsetAdder>2880) //有几张图片 就需要720 * (几张-1)
		     OffsetAdder=0;
		}
		Offset++;
		if(Offset>480)//侧面有多少文字 这个值就对应改成  多少字*16 
		Offset=0;	
	}	
}
/******************************************/
void Send_uart(unsigned char d)	  /*从串口发送数据*/
{
	  ES=0;
      SBUF=d;
	  while(TI==0);       //等待发送完毕
      TI=0;
	  ES=1;
}
/*************************************/
//图片切换以及模拟表盘显示函数
void display_pic()
{
	uchar date1,date2,date3,date4;
	if(DISP_LINE<180)
	{
		if(Time0_flag)
		{
		 	Time0_flag=0;
			if(ucMode==2)
			{
				Close();
				  //DISP_LINE=DISP_LINE-1;
				      //标整点,以及显示数字12,3,6,9
			      if ( (DISP_LINE >= 175) || (DISP_LINE <= 3))  //12点
			      {
			       ucAdder=DISP_LINE+4 ;
			       if ( ucAdder>=180) ucAdder=ucAdder-180 ;
			      }
			      else if ( (DISP_LINE >= 41) && (DISP_LINE <= 48)) //3点
			      {
			       ucAdder=DISP_LINE-41+8*1 ;
			      }
			      else if ( (DISP_LINE >= 86) && (DISP_LINE <= 93)) //6点
			      {
			       ucAdder=DISP_LINE-86+8*2 ;
			      }
			      else if ( (DISP_LINE >= 131) && (DISP_LINE <= 138)) //9点
			      {
			       ucAdder=DISP_LINE-131+8*3 ;
			      }
				  else
				  {
					 ucAdder=250;
					/*************形成表盘*******************/
				   switch (DISP_LINE)    
			       {
				        case 180*1/12 :    //1点
				        case 180*2/12 :    //2点
				        case 180*4/12 :    //4点
				        case 180*5/12 :    //5点
				        case 180*7/12 :    //6点
				        case 180*8/12 :    //7点
				        case 180*10/12 :   //10点
				        case 180*11/12 :   //11点
				          //LED=0;
						  PortA(0X0F);
				         break;
			       }
				   }
				   if ( ucAdder < 32 )
				    {
						PortA(TAB_12369[ucAdder]) ; //查表显示12,3,6,9
						
					}
			   /****************************************/   
			   	  //显示指针"时"
			//	   i = Hour ;
			//       j = Hour ;
			//      if ( ++i > 179 ) i=0 ;    //i+1,结果>179,则清零
			//      if ( --j > 179 ) i=179 ;   //j-1,结果为负,则重新赋值179
				if(( (Hour-2)<=DISP_LINE)&&((Hour+2)>=DISP_LINE) )
				{
					if(((Hour-1)==DISP_LINE)||((Hour+1)==DISP_LINE))
					{
						PortD(0XFF);
						PortC(0XBF);
					
					}
					else if(((Hour-2)==DISP_LINE)||((Hour+2)==DISP_LINE))
					{
						PortD(0XFF);
						PortC(0XDF);
						
					}	
				}
			   	if(Hour==DISP_LINE)//||(DISP_LINE==i)||(DISP_LINE==j
			    {
			        
			        PortD(0X80) ;
					PortC(0X00);
					
			
			
			    }    
			    //显示指针"分"
				if(( (Cent-2)<=DISP_LINE)&&((Cent+2)>=DISP_LINE) )
				{
					if(((Cent-1)==DISP_LINE)||((Cent+1)==DISP_LINE))
					{
						PortC(0XFF);
						PortB(0XFB);
						PortD(0XFF);
					
					}	
					else if(((Cent-2)==DISP_LINE)||((Cent+2)==DISP_LINE))
					{
						PortC(0XFF);
						PortB(0XFD);
						PortD(0XFF);
						
					}
					
				}
				if(Cent==DISP_LINE)
			    {
			        //PortB=(0X00);
					PortC(0X00);
					PortB(0XF0);
					PortD(0X80);
					
			       	
			    }
				//显示指针"秒"
				if(( (Sec-2)<=DISP_LINE)&&((Sec+2)>=DISP_LINE) )
				{
					if(((Sec-1)==DISP_LINE)||((Sec+1)==DISP_LINE))
					{
						PortB(0XBF);
						PortC(0XFF);
						PortD(0XFF);
					
					}
					else if(((Sec-2)==DISP_LINE)||((Sec+2)==DISP_LINE))
					{
						PortB(0XDF);
						PortC(0XFF);
						PortD(0XFF);
					}
						
				}
			    if(Sec==DISP_LINE)
			    {
			        PortB(0X00);
					PortC(0X00);
					PortD(0X80);
				
			       
			    }
			  Show_R(Lzimo[(DISP_LINE+Offset)*2],Lzimo[(DISP_LINE+Offset)*2+1]);//送16*16文字显示	
			}
			else if(ucMode==4) //平面图片+侧面文字
			{
			 	Close();		
				date1=picture[DISP_LINE*4+0+OffsetAdder];
				date2=picture[DISP_LINE*4+1+OffsetAdder];
				date3=picture[DISP_LINE*4+2+OffsetAdder];
				date4=picture[DISP_LINE*4+3+OffsetAdder];
				Show_L(date1,date2,date3,date4);
			    Show_R(Lzimo[(DISP_LINE+Offset)*2],Lzimo[(DISP_LINE+Offset)*2+1]);//送16*16文字显示	
			}
		}
      }
}
/*********************************************/
void Init(void)
{
   	P0M0=0X00;//初始化IO口 为准双向口
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

	Show_R(0x55,0x55);//测试
	Show_L(0x55,0x55,0x55,0x55);//测试

AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xFE;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;

	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0x12;		//设置定时初值 155US
	TH1 = 0x12;		//设置定时重载值
	TF1 = 0;		//清除TF1标志
	ET1=1;
	TR1 = 0;		//定时器1开始计时

	SCON = 0x50; 
 	T2L = (65536 - (FOSC/4/BAUD));   //设置波特率重装值
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2为1T模式, 并启动定时器2
    AUXR |= 0x01;               //选择定时器2为串口1的波特率发生器
   // ES = 1;                     //使能串口1中断

     IT0 = 1;    //外部中断为边沿触发
                   //设置INT0的中断类型 (1:仅下降沿 0:上升沿和下降沿)
     EX0 = 1;  //使能INT0中断

	 IT1=1;
	 EX1=1;//外部中断1 下降沿 中断
	 EA = 1;                 
}
/************************************/
void Timer0Interrupt(void) interrupt 1
{			 
    Time0_flag=1;	           //定时中断标志位置1;
	TH0=-Pt/256;TL0=-Pt%256;   //设置定时器T0的初设值
	DISP_LINE++;                    //计数
}
/*****主函数*****/
void main(void)
{
	Init();
	Auto_Set1302(starts_time);//自动初始化时钟
	Send_uart(0x55);
	
    //主程序
   	while(1)
    {      	
    	
		if((ucMode==2)||(ucMode==4))
		{
			display_pic();
		}
		 //BIT_timeout=1;
		if(BIT_timeout==1)	//起点判断
	 	{			
			BIT_timeout=0; //清零
			
			Scan_bit=1;// 起点检测到后 LED不再流水测试
			if( new_code ) //有红外按键
			{
				new_code=0;
				switch( key_code ) //根据不同的按键值执行不同的动作
				{	
				   case 0x0C:
					ucMode=0x01;//平面显示文字模式+侧面时钟
				//	TR0 = 0;	
				break;

				case 0x18:
					ucMode=0x02;//指针式时钟模式
					//TR0 = 1;
				break;
				case 0x5E:
					ucMode=0x03;//平面时钟模式+侧面文字
				//	TR0=0;			 	
				break;
				case 0x08:
					ucMode=0x04;//平面显示图片+侧面文字
					//TR0 = 1;
				break;
				/**********************************************/
					case 0x40:	 //调时加
						Set(id,1);

					break;
	
					case 0x19: //调时减				
							Set(id,0); 
					
					break;
					
					case 0x15://调时切换
								id++;						
		   						if(id>5)
								{
		    						id=0;
								}
					break;
					case 0x47://时钟初始化
							Set1302(starts_time);    //初始化 
							W1302(0x8e,0x00); //控制命令,WP=0,写操作                
							W1302(0x90,0xa5); //打涞二级�  一个二级管串联一个2K玷
							//write_1302Data(DISP_TIME_adder,160);
					break;
	
					default: break;
				}
				key_code=0;
			}
		
			if(ucMode==1)
			{
				du1302();//读取时钟芯片 的时钟数据
				Show_line_time();//侧面显示时间+平面文字
			}
    		else if(ucMode==3)
			{
				du1302P();//读取时钟芯片 的时钟数据
				Show_plane_time();//平面显示时间+侧面文字
			}
    
            
    		}   
    
		}
}
/*********************************************************************************/
/*************************************************************************************
************************************************************************************
***********************                                     ***************************
***********************    http://59tiaoba.taobao.com       ***************************
***********************                                     ***************************
************************************************************************************
**********************************************************************************/