/*************************************************************
                     Ê±ÖÓÏÔÊ¾³ÌÐò
³ÌÐò±àÐ´£ºFucp   2015-5-17
DIYÌ×¼þ  http://59tiaoba.taobao.com

µ¥Æ¬»ú IAP15W4K61S4  »òÕßÆäËû51	 ¾§ÕñÆµÂÊ 18.432M  
×ÖÄ£Èí¼þÓÃ  PC2002
ÖðÁÐÈ¡Ä£·½Ê½

µç»úÐý×ª·½Ïò Ë³Ê±Õë
/***********************************************************
ËµÃ÷£º±¾³ÌÐòÖ§³ÖÅäÌ×Ò£¿ØÆ÷µ÷ÕûÊ±¼ä
²¥·Å°´¼ü  ÇÐ»»µ÷Õû Ê±¡¢·Ö¡¢Ãë¡¢ÔÂ¡¢ÈÕ
¼Ó °´¼ü£ºÔö¼Ó
¼õ °´¼ü£º¼õÉÙ
menu°´¼ü£º Ê±ÖÓ¸´Î»

Ò£¿ØÆ÷°´¼ü1 £ºÆ½ÃæÏÔÊ¾ÎÄ×Ö+²àÃæÏÔÊ¾Ê±ÖÓ	 ¿Éµ÷Ê±
Ò£¿ØÆ÷°´¼ü2 £ºÆ½ÃæÖ¸ÕëÊ±ÖÓ+²àÃæÏÔÊ¾ÎÄ×Ö
*************************************************************/
#include "NEW_8051.H"
#include "intrins.h"
#include "task.h"
#define FOSC 18432000L          //ÏµÍ³ÆµÂÊ
#define BAUD 115200             //´®¿Ú²¨ÌØÂÊ
bit Scan_bit=0;	  
uint Offset=0;
bit BIT_timeout=0;
uchar ucMode=1;	 //Ä£Ê½
bit Time0_flag=0;	//¶¨Ê±Æ÷T0ÖÐ¶Ï±êÖ¾Î»
uchar DISP_LINE=0;
uint Pt=50,S0,T,D;		//¼ÆÊýÓÃµÄ±äÁ¿
uchar ucAdder=0;
 uchar code TAB_12369[]=
 {

	0xFF,0x03,0xFF,0xB3,0x6B,0x6B,0x9B,0xFF,		
	0x8F,0x77,0x7F,0x8F,0x7F,0x77,0x8F,0xFF,//3
	0xFF,0xFF,0x9F,0x6D,0x6D,0x6B,0x87,0xFF,//6	
	0x9F,0xEF,0xF7,0x87,0x77,0x77,0x8F,0xFF,///9


 };
/*********ÖÐ¶Ï×Ó³ÌÐò**********/
void int0() interrupt 0	//Ê¹ÓÃÍâ²¿ÖÐ¶Ï0
{
	BIT_timeout=1; //Æðµã±êÖ¾Î»
	if(ucMode!=1)
	{	

			D+=(DISP_LINE-180)*2;   //ÐÞÕýÖµ
	   		Pt=205+D; 
	    //µÃµ½¶¨Ê±Æ÷T0µÄ³õÉèÖµ
		TH0=-Pt/256;
		TL0=-Pt%256;   //ÉèÖÃ¶¨Ê±Æ÷T0µÄ³õÉèÖµ
		DISP_LINE=0;   //¼ÆÊýÆ÷ÇåÁã£¬½«ÖØÐÂ¼ÆÊý	
		switch (ucMode)
		{
			case 0x02:
				du1302();//»ñÈ¡Ê±¼ä
				Sec=Sec/16*10+Sec%16 ;
        	    Cent=Cent/16*10+Cent%16 ;
        		Hour=Hour/16*10+Hour%16 ;
				if(Hour>11)
				{
					Hour=Hour-12;	
				}
				Sec=Sec*3;//ÃëÕë¶¨Î»="Ãë"*3
				Hour=(Hour*15)+(Cent/4);//Ê±Õë¶¨Î»="Ê±"*15+"·Ö"/4
				Cent=Cent*3;//·ÖÕë¶¨Î»="·Ö"*3
				Offset++;
				if(Offset>480)//²àÃæÓÐ¶àÉÙÎÄ×Ö Õâ¸öÖµ¾Í¶ÔÓ¦¸Ä³É  ¶àÉÙ×Ö*16 
				Offset=0;	
				break;
			case 0x03:
				du1302();//»ñÈ¡Ê±¼ä
				Sec=Sec/16*10+Sec%16 ;
        	    Cent=Cent/16*10+Cent%16 ;
        		Hour=Hour/16*10+Hour%16 ;
				if(Hour>12)
				{
					Hour=Hour-12;	
				}
				break;
			default:
				break;
		}
	}
}
/******************************************/
void Send_uart(unsigned char d)	  /*´Ó´®¿Ú·¢ËÍÊý¾Ý*/
{
	  ES=0;
      SBUF=d;
	  while(TI==0);       //µÈ´ý·¢ËÍÍê±Ï
      TI=0;
	  ES=1;
}
/*************************************/
//Í¼Æ¬ÇÐ»»ÒÔ¼°Ä£Äâ±íÅÌÏÔÊ¾º¯Êý
void display_pic()
{
	//uchar date1,date2,date3,date4;
	if(DISP_LINE<180)
	{
		if(Time0_flag)
		{
		 	Time0_flag=0;
			Close();
			if(ucMode==2)
			{
				  //DISP_LINE=DISP_LINE-1;
				      //±êÕûµã,ÒÔ¼°ÏÔÊ¾Êý×Ö12,3,6,9
			      if ( (DISP_LINE >= 175) || (DISP_LINE <= 3))  //12µã
			      {
			       ucAdder=DISP_LINE+4 ;
			       if ( ucAdder>=180) ucAdder=ucAdder-180 ;
			      }
			      else if ( (DISP_LINE >= 41) && (DISP_LINE <= 48)) //3µã
			      {
			       ucAdder=DISP_LINE-41+8*1 ;
			      }
			      else if ( (DISP_LINE >= 86) && (DISP_LINE <= 93)) //6µã
			      {
			       ucAdder=DISP_LINE-86+8*2 ;
			      }
			      else if ( (DISP_LINE >= 131) && (DISP_LINE <= 138)) //9µã
			      {
			       ucAdder=DISP_LINE-131+8*3 ;
			      }
				  else
				  {
					 ucAdder=250;
					/*************ÐÎ³É±íÅÌ*******************/
				   switch (DISP_LINE)    
			       {
				        case 180*1/12 :    //1µã
				        case 180*2/12 :    //2µã
				        case 180*4/12 :    //4µã
				        case 180*5/12 :    //5µã
				        case 180*7/12 :    //6µã
				        case 180*8/12 :    //7µã
				        case 180*10/12 :   //10µã
				        case 180*11/12 :   //11µã
				          //LED=0;
						  PortA(0X0F);
				         break;
			       }
				   }
				   if ( ucAdder < 32 )
				    {
						PortA(TAB_12369[ucAdder]) ; //²é±íÏÔÊ¾12,3,6,9
						
					}
			   /****************************************/   
			   	  //ÏÔÊ¾Ö¸Õë"Ê±"
			//	   i = Hour ;
			//       j = Hour ;
			//      if ( ++i > 179 ) i=0 ;    //i+1,½á¹û>179,ÔòÇåÁã
			//      if ( --j > 179 ) i=179 ;   //j-1,½á¹ûÎª¸º,ÔòÖØÐÂ¸³Öµ179
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
			    //ÏÔÊ¾Ö¸Õë"·Ö"
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
				//ÏÔÊ¾Ö¸Õë"Ãë"
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
				//DISP_LINE=DISP_LINE+1;
			  Show_R(Lzimo[(DISP_LINE+Offset)*2],Lzimo[(DISP_LINE+Offset)*2+1]);//ËÍ16*16ÎÄ×ÖÏÔÊ¾	
			}
		}
      }
}
/*********************************************/
void Init(void)
{
   	P0M0=0X00;//³õÊ¼»¯IO¿Ú Îª×¼Ë«Ïò¿Ú
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

	Show_R(0x55,0x55);//²âÊÔ
	Show_L(0x55,0x55,0x55,0x55);//²âÊÔ

AUXR &= 0x7F;		//¶¨Ê±Æ÷Ê±ÖÓ12TÄ£Ê½
	TMOD &= 0xF0;		//ÉèÖÃ¶¨Ê±Æ÷Ä£Ê½
	TMOD |= 0x01;		//ÉèÖÃ¶¨Ê±Æ÷Ä£Ê½
	TL0 = 0xCD;		//ÉèÖÃ¶¨Ê±³õÖµ
	TH0 = 0xFE;		//ÉèÖÃ¶¨Ê±³õÖµ
	TF0 = 0;		//Çå³ýTF0±êÖ¾
	TR0 = 1;		//¶¨Ê±Æ÷0¿ªÊ¼¼ÆÊ±
	ET0=1;

	AUXR &= 0xBF;		//¶¨Ê±Æ÷Ê±ÖÓ12TÄ£Ê½
	TMOD &= 0x0F;		//ÉèÖÃ¶¨Ê±Æ÷Ä£Ê½
	TMOD |= 0x20;		//ÉèÖÃ¶¨Ê±Æ÷Ä£Ê½
	TL1 = 0x12;		//ÉèÖÃ¶¨Ê±³õÖµ 155US
	TH1 = 0x12;		//ÉèÖÃ¶¨Ê±ÖØÔØÖµ
	TF1 = 0;		//Çå³ýTF1±êÖ¾
	ET1=1;
	TR1 = 0;		//¶¨Ê±Æ÷1¿ªÊ¼¼ÆÊ±

	SCON = 0x50; 
 	T2L = (65536 - (FOSC/4/BAUD));   //ÉèÖÃ²¨ÌØÂÊÖØ×°Öµ
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2Îª1TÄ£Ê½, ²¢Æô¶¯¶¨Ê±Æ÷2
    AUXR |= 0x01;               //Ñ¡Ôñ¶¨Ê±Æ÷2Îª´®¿Ú1µÄ²¨ÌØÂÊ·¢ÉúÆ÷
   // ES = 1;                     //Ê¹ÄÜ´®¿Ú1ÖÐ¶Ï

     IT0 = 1;    //Íâ²¿ÖÐ¶ÏÎª±ßÑØ´¥·¢
                   //ÉèÖÃINT0µÄÖÐ¶ÏÀàÐÍ (1:½öÏÂ½µÑØ 0:ÉÏÉýÑØºÍÏÂ½µÑØ)
     EX0 = 1;  //Ê¹ÄÜINT0ÖÐ¶Ï

	 IT1=1;
	 EX1=1;//Íâ²¿ÖÐ¶Ï1 ÏÂ½µÑØ ÖÐ¶Ï
	 EA = 1;                 
}
/************************************/
void Timer0Interrupt(void) interrupt 1
{			 
    Time0_flag=1;	           //¶¨Ê±ÖÐ¶Ï±êÖ¾Î»ÖÃ1;
	TH0=-Pt/256;TL0=-Pt%256;   //ÉèÖÃ¶¨Ê±Æ÷T0µÄ³õÉèÖµ
	DISP_LINE++;                    //¼ÆÊý
}
/*****Ö÷º¯Êý*****/
void main(void)
{
	Init();
	Auto_Set1302(starts_time);//×Ô¶¯³õÊ¼»¯Ê±ÖÓ
	Send_uart(0x55);
	
    //Ö÷³ÌÐò
   	while(1)
    {      	
    	
		if((ucMode==2)||(ucMode==3))
		{
			display_pic();
		}
		 //BIT_timeout=1;
		if(BIT_timeout==1)	//ÆðµãÅÐ¶Ï
	 	{			
			BIT_timeout=0; //ÇåÁã
			
			Scan_bit=1;// Æðµã¼ì²âµ½ºó LED²»ÔÙÁ÷Ë®²âÊÔ
			if( new_code ) //ÓÐºìÍâ°´¼ü
			{
				new_code=0;
				switch( key_code ) //¸ù¾Ý²»Í¬µÄ°´¼üÖµÖ´ÐÐ²»Í¬µÄ¶¯×÷
				{	
				   case 0x0C:
					ucMode=0x01;//Êý×ÖÖÓÄ£Ê½+²àÃæÎÄ×Ö
					TR0 = 0;	
				break;

				case 0x18:
					ucMode=0x02;//Ö¸ÕëÊ½Ê±ÖÓÄ£Ê½
					TR0 = 1;
				break;
				case 0x5E:
				//	ucMode=0x03;//Ö¸ÕëÊ±ÖÓÄ£Ê½
					TR0=1;			 	
				break;
					case 0x40:	 //µ÷Ê±¼Ó
						Set(id,1);

					break;
	
					case 0x19: //µ÷Ê±¼õ				
							Set(id,0); 
					
					break;
					
					case 0x15://µ÷Ê±ÇÐ»»
								id++;						
		   						if(id>5)
								{
		    						id=0;
								}
					break;
					case 0x47://Ê±ÖÓ³õÊ¼»¯
							Set1302(starts_time);    //³õÊ¼»¯ 
							W1302(0x8e,0x00); //¿ØÖÆÃüÁî,WP=0,Ð´²Ù×÷                
							W1302(0x90,0xa5); //´òª³äµ¶þ¼¶Ü  Ò»¸ö¶þ¼¶¹Ü´®ÁªÒ»¸ö2Kçè
							//write_1302Data(DISP_TIME_adder,160);
					break;
	
					default: break;
				}
				key_code=0;
			}
		
			if(ucMode==1)
			{
				du1302();//¶ÁÈ¡Ê±ÖÓÐ¾Æ¬ µÄÊ±ÖÓÊý¾Ý
				Show_line_time();//ÏÔÊ¾Ê±¼ä
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