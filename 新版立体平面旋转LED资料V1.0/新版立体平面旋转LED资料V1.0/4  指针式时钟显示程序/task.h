#ifndef __task_H__
#define __task_H__

//59tiaoba.taobao.com 
#define uchar unsigned char
#define uint unsigned int    //�궨��


sbit D1=P0^4; //����ÿ��IO���Ƶ�LED
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
#define LINE_ADJ_adder 0XC0     //��ʾģ�����ʱ���м�����У��ֵ
#define DISP_TIME_adder 0XC4	//��ʾģ�����ʱ�ӵ���ʱ��У��ֵ
#define ucNumRows_adder 0XC6
#define ucNumRows_p_adder 0XC8	
#define DEC2BCD(X) ((X/10)<<4 | (X%10))               //���ڽ�ʮ����ת��BCD��ĺ�
#define BCD2DEC(X) (((X&0xF0)>>4)*10 + (X&0x0F))      //���ڽ�BCD��ת��ʮ���Ƶĺ�
/************task.c****************************/
extern void delay(unsigned char n);
extern void delay2(unsigned char n);
extern bit BIT_timeout;
extern void Show_R(uchar r1,uchar r2);
extern void Show_L(uchar l1,uchar l2,uchar l3,uchar l4);//����ʾ
extern void delay_PIC(unsigned char n);	//ÿ����ʾ��ʱ��
/************zimo.c*************************/
extern uchar code zimo[];
extern uchar code dazimo[];
extern uchar code picture[];
/***********IR_Receive.c**************************/
extern bit new_code;
extern uchar xdata key_code;
extern void Ircordpro(void);//������ֵ������
/*************ShowTime.c**************************************/
extern uchar id;

extern uchar code Lzimo[];

extern void PortA(uchar temp);//����ʾ
extern void PortB(uchar temp);//����ʾ
extern void PortC(uchar temp);//����ʾ
extern void PortD(uchar temp);//����ʾ

extern void Show_line_time(void);
extern void Close(void); //IO�ڸߵ�ƽ�ر�LED��ʾ
//extern const uchar code shuzi[21][36];
extern void Send_uart(unsigned char d);	  /*�Ӵ��ڷ�������*/

/***************DS1302.c****************************************/
extern void   RTInputByte(uchar);         //* ���� 1Byte */
extern uchar  RTOutputByte(void);         //* ��� 1Byte */
extern void   W1302(uchar, uchar);        // ��DS1302д��һ���ֽ�
extern uchar  R1302(uchar);               // ��DS1302����һ���ֽ�
extern void   Set1302(uchar *pClock);
extern uchar xdata TimeBuff[20]; 
extern void   DelayMs(void);              // ��ʱ
extern void   Set(uchar,uchar );          // ��������
extern void  write_1302Data (uchar  ucAddr,uchar  Value);
extern uchar  Sec;
extern uchar  Cent;
extern uchar  Hour;
extern uchar  Year;
extern uchar  Day;
extern uchar  Week;
extern uchar  Month;
extern void du1302();
extern void Auto_Set1302(uchar *pClock);
extern uchar code starts_time[7];
extern void Set1302R(uchar *pClock);
#endif

/***********************59tiaoba.taobao.com *************************/