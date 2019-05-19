/*************************************************************
                     时钟显示程序
程序编写：Fucp   2015-5-16
DIY套件  http://59tiaoba.taobao.com

单片机 IAP15W4K61S4  或者其他51	 晶振频率 18.432M  
字模软件用  PC2002
逐列取模方式

电机旋转方向 顺时针
/***********************************************************
说明：本程序支持配套遥控器调整时间
播放按键  切换调整 时、分、秒、月、日
加 按键：增加
减 按键：减少
menu按键： 时钟复位

平面显示文字+侧面显示时钟

*************************************************************/
#include "NEW_8051.H"
#include "intrins.h"
#include "task.h"
bit Scan_bit=0;
bit BIT_timeout=0;



/*********中断子程序**********/
void int0() interrupt 0	//使用外部中断0
{
	BIT_timeout=1; //起点标志位	
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
	TL0 = 0x12;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	ET0=1;
	//TR0 = 1;		//定时器0开始计时

	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xD8;		//设定定时初值
	TH1 = 0xFF;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1

     IT0 = 1;    //外部中断为边沿触发
                   //设置INT0的中断类型 (1:仅下降沿 0:上升沿和下降沿)
     EX0 = 1;  //使能INT0中断

	 IT1=1;
	 EX1=1;//外部中断1 下降沿 中断
	 EA = 1;                 
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
			du1302();//读取时钟芯片 的时钟数据
			Show_line_time();//显示时间
    
            
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