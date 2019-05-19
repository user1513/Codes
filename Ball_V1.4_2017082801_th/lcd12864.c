#include <STC12C5A60S2.H>
#include "lcd12864.h"


void Display_String(uchar line,uchar *string)//��ʾ�ַ��� 
{   
		uchar addr,i;  
		if(line==1)addr=0x80;  
		else if(line==2)addr=0x90;  
		else if(line==3)addr=0x88;   
		else if(line==4)addr=0x98;      
		Wr_Command(addr,1);  
		for(i=0;i<16;i++)  
		{ 
				Wr_Data(*string++);  
		}  
}  

void Wr_Data(uchar wrdata) //д���� 
{  
		RDbf();  
		RS=1;  
		RW=0;  
		E=1;  
		LCMdata=wrdata;  
		E=0;  
}  

void Wr_Command(uchar wrcommand,busyc) //дָ�� 
{  
		if(busyc)RDbf();  
		RS=0;  
		RW=0;  
		E=1;  
		LCMdata=wrcommand;  
		E=0; 
}  

void RDbf(void) //��æ״̬ 
{ 
		while(1)
		{   
				RS=0;    
				RW=1; 
				E=0;   
				LCMdata=0xFF;    
				E=1;   
				if((LCMdata&Busy)==0)   break;  
		} 
} 

void Lcm_Init(void)//12864��ʼ��
{  
		Delay5Ms();     
		REST=1;     
		REST=0;     
		REST=1;   
		Wr_Command(0x30,0);  
		Delay5Ms();  
		Wr_Command(0x30,0); //2 ����ʾģʽ���ã�������æ  
		Delay5Ms();  
		Wr_Command(0x0C,1); //����ʾ���������  
		Delay5Ms();  
		Wr_Command(0x01,1); //��ʾ����  
		Delay5Ms();  
		Delay5Ms();  
		Wr_Command(0x06,1); //��ʾ����ƶ�����  
		Delay5Ms(); 
}

void Delay5Ms(void) //��ʱ 5ms
{  
		unsigned int TempCyc=5552;  
		while(TempCyc--); 
}

void LCD12864_Clear(void)
{
		Wr_Command(0x01,1); //��ʾ����  
}
