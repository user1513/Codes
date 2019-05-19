#ifndef __task_H__
#define __task_H__

//59tiaoba.taobao.com 
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
sbit D9=P7^6;
sbit D10=P7^5;  
sbit D11=P7^4;
sbit D12=P2^7;	
sbit D13=P2^6;
sbit D14=P2^5;  
sbit D15=P2^4;
sbit D16=P2^3;
	
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
/*********************/
/************task.c****************************/
extern bit BIT_timeout;
extern void Show_R(uchar r1,uchar r2);
extern void Show_L(uchar l1,uchar l2,uchar l3,uchar l4);//送显示
extern void delay_PIC(unsigned char n);	//每列显示的时间
/************zimo.c*************************/
extern uchar code zimo[];
extern uchar code dazimo[];
extern uchar code picture[];
/***********IR_Receive.c**************************/
extern bit new_code;
extern uchar xdata key_code;
extern void Ircordpro(void);//红外码值处理函数
/*************ShowTime.c**************************************/
extern uchar id;
extern uchar code Lzimo[];
extern void Close(void); //IO口高电平关闭LED显示
//extern const uchar code shuzi[21][36];
extern void Send_uart(unsigned char d);	  /*从串口发送数据*/
/******************text.c**********************/

#endif
/***********************59tiaoba.taobao.com *************************/