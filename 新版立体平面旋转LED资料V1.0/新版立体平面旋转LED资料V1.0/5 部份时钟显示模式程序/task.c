/*************************************************************
                     ʱ����ʾ����
�����д��Fucp   2015-5-17
DIY�׼�  http://59tiaoba.taobao.com

��Ƭ�� IAP15W4K61S4  ��������51	 ����Ƶ�� 18.432M  
��ģ�����  PC2002
����ȡģ��ʽ

�����ת���� ˳ʱ��
/***********************************************************
˵����������֧������ң��������ʱ��
���Ű���  �л����� ʱ���֡��롢�¡���
�� ����������
�� ����������
menu������ ʱ�Ӹ�λ

ң��������1 ��ƽ����ʾ����+������ʾʱ��	 �ɵ�ʱ
ң��������2 ��ƽ��ָ��ʱ��+������ʾ����
*************************************************************/
#include "NEW_8051.H"
#include "intrins.h"
#include "task.h"
#define FOSC 18432000L          //ϵͳƵ��
#define BAUD 115200             //���ڲ�����
bit Scan_bit=0;	  
uint Offset=0;
bit BIT_timeout=0;
uchar ucMode=1;	 //ģʽ
bit Time0_flag=0;	//��ʱ��T0�жϱ�־λ
uchar DISP_LINE=0;
uint Pt=50,S0,T,D;		//�����õı���
uchar ucAdder=0;
 uchar code TAB_12369[]=
 {

	0xFF,0x03,0xFF,0xB3,0x6B,0x6B,0x9B,0xFF,		
	0x8F,0x77,0x7F,0x8F,0x7F,0x77,0x8F,0xFF,//3
	0xFF,0xFF,0x9F,0x6D,0x6D,0x6B,0x87,0xFF,//6	
	0x9F,0xEF,0xF7,0x87,0x77,0x77,0x8F,0xFF,///9


 };
/*********�ж��ӳ���**********/
void int0() interrupt 0	//ʹ���ⲿ�ж�0
{
	BIT_timeout=1; //����־λ
	if(ucMode==2)
	{	

		D+=(DISP_LINE-180)*2;   //����ֵ
   		Pt=205+D; 
	    //�õ���ʱ��T0�ĳ���ֵ
		TH0=-Pt/256;
		TL0=-Pt%256;   //���ö�ʱ��T0�ĳ���ֵ
		DISP_LINE=0;   //���������㣬�����¼���	
		du1302();//��ȡʱ��
		Sec=Sec/16*10+Sec%16 ;
	    Cent=Cent/16*10+Cent%16 ;
		Hour=Hour/16*10+Hour%16 ;
		if(Hour>11)
		{
			Hour=Hour-12;	
		}
		Sec=Sec*3;//���붨λ="��"*3
		Hour=(Hour*15)+(Cent/4);//ʱ�붨λ="ʱ"*15+"��"/4
		Cent=Cent*3;//���붨λ="��"*3
		Offset++;
		if(Offset>480)//�����ж������� ���ֵ�Ͷ�Ӧ�ĳ�  ������*16 
		Offset=0;	
		
			
			
	}
}
/******************************************/
void Send_uart(unsigned char d)	  /*�Ӵ��ڷ�������*/
{
	  ES=0;
      SBUF=d;
	  while(TI==0);       //�ȴ��������
      TI=0;
	  ES=1;
}
/*************************************/
//ͼƬ�л��Լ�ģ�������ʾ����
void display_pic()
{
	//uchar date1,date2,date3,date4;
	if(DISP_LINE<180)
	{
		if(Time0_flag)
		{
		 	Time0_flag=0;
			Close();
				  //DISP_LINE=DISP_LINE-1;
				      //������,�Լ���ʾ����12,3,6,9
			      if ( (DISP_LINE >= 175) || (DISP_LINE <= 3))  //12��
			      {
			       ucAdder=DISP_LINE+4 ;
			       if ( ucAdder>=180) ucAdder=ucAdder-180 ;
			      }
			      else if ( (DISP_LINE >= 41) && (DISP_LINE <= 48)) //3��
			      {
			       ucAdder=DISP_LINE-41+8*1 ;
			      }
			      else if ( (DISP_LINE >= 86) && (DISP_LINE <= 93)) //6��
			      {
			       ucAdder=DISP_LINE-86+8*2 ;
			      }
			      else if ( (DISP_LINE >= 131) && (DISP_LINE <= 138)) //9��
			      {
			       ucAdder=DISP_LINE-131+8*3 ;
			      }
				  else
				  {
					 ucAdder=250;
					/*************�γɱ���*******************/
				   switch (DISP_LINE)    
			       {
				        case 180*1/12 :    //1��
				        case 180*2/12 :    //2��
				        case 180*4/12 :    //4��
				        case 180*5/12 :    //5��
				        case 180*7/12 :    //6��
				        case 180*8/12 :    //7��
				        case 180*10/12 :   //10��
				        case 180*11/12 :   //11��
				          //LED=0;
						  PortA(0X0F);
				         break;
			       }
				   }
				   if ( ucAdder < 32 )
				    {
						PortA(TAB_12369[ucAdder]) ; //�����ʾ12,3,6,9
						
					}
			   /****************************************/   
			   	  //��ʾָ��"ʱ"
			//	   i = Hour ;
			//       j = Hour ;
			//      if ( ++i > 179 ) i=0 ;    //i+1,���>179,������
			//      if ( --j > 179 ) i=179 ;   //j-1,���Ϊ��,�����¸�ֵ179
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
			    //��ʾָ��"��"
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
				//��ʾָ��"��"
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
			  Show_R(Lzimo[(DISP_LINE+Offset)*2],Lzimo[(DISP_LINE+Offset)*2+1]);//��16*16������ʾ	

		}
      }
}
/*********************************************/
void Init(void)
{
   	P0M0=0X00;//��ʼ��IO�� Ϊ׼˫���
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

	Show_R(0x55,0x55);//����
	Show_L(0x55,0x55,0x55,0x55);//����

AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xFE;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0=1;

	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TMOD |= 0x20;		//���ö�ʱ��ģʽ
	TL1 = 0x12;		//���ö�ʱ��ֵ 155US
	TH1 = 0x12;		//���ö�ʱ����ֵ
	TF1 = 0;		//���TF1��־
	ET1=1;
	TR1 = 0;		//��ʱ��1��ʼ��ʱ

	SCON = 0x50; 
 	T2L = (65536 - (FOSC/4/BAUD));   //���ò�������װֵ
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2Ϊ1Tģʽ, ��������ʱ��2
    AUXR |= 0x01;               //ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
   // ES = 1;                     //ʹ�ܴ���1�ж�

     IT0 = 1;    //�ⲿ�ж�Ϊ���ش���
                   //����INT0���ж����� (1:���½��� 0:�����غ��½���)
     EX0 = 1;  //ʹ��INT0�ж�

	 IT1=1;
	 EX1=1;//�ⲿ�ж�1 �½��� �ж�
	 EA = 1;                 
}
/************************************/
void Timer0Interrupt(void) interrupt 1
{			 
    Time0_flag=1;	           //��ʱ�жϱ�־λ��1;
	TH0=-Pt/256;TL0=-Pt%256;   //���ö�ʱ��T0�ĳ���ֵ
	DISP_LINE++;                    //����
}
/*****������*****/
void main(void)
{
	Init();
	Auto_Set1302(starts_time);//�Զ���ʼ��ʱ��
	Send_uart(0x55);
	
    //������
   	while(1)
    {      	
    	
		if((ucMode==2)||(ucMode==3))
		{
			display_pic();
		}
		 //BIT_timeout=1;
		if(BIT_timeout==1)	//����ж�
	 	{			
			BIT_timeout=0; //����
			
			Scan_bit=1;// ����⵽�� LED������ˮ����
			if( new_code ) //�к��ⰴ��
			{
				new_code=0;
				switch( key_code ) //���ݲ�ͬ�İ���ִֵ�в�ͬ�Ķ���
				{	
				   case 0x0C:
					ucMode=0x01;//ƽ����ʾ����ģʽ+����ʱ��
					TR0 = 0;	
				break;

				case 0x18:
					ucMode=0x02;//ָ��ʽʱ��ģʽ
					TR0 = 1;
				break;
				case 0x5E:
					ucMode=0x03;//ƽ��ʱ��ģʽ+��������
					TR0=0;			 	
				break;
				/**********************************************/
					case 0x40:	 //��ʱ��
						Set(id,1);

					break;
	
					case 0x19: //��ʱ��				
							Set(id,0); 
					
					break;
					
					case 0x15://��ʱ�л�
								id++;						
		   						if(id>5)
								{
		    						id=0;
								}
					break;
					case 0x47://ʱ�ӳ�ʼ��
							Set1302(starts_time);    //��ʼ�� 
							W1302(0x8e,0x00); //��������,WP=0,д����                
							W1302(0x90,0xa5); //��䵶����  һ�������ܴ���һ��2K��
							//write_1302Data(DISP_TIME_adder,160);
					break;
	
					default: break;
				}
				key_code=0;
			}
		
			if(ucMode==1)
			{
				du1302();//��ȡʱ��оƬ ��ʱ������
				Show_line_time();//������ʾʱ��+ƽ������
			}
    		else if(ucMode==3)
			{
				du1302P();//��ȡʱ��оƬ ��ʱ������
				Show_plane_time();//ƽ����ʾʱ��+��������
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