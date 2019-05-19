#include <STC12C5A60S2.H>
#include <intrins.h> 
#include <math.h>
#include "lcd12864.h"
#include "key.h"
#include "IAP_EEPROM.h"
#include "string.h"


/*
	2017.08.28
	创建者：Tang娃
	
	注：需要添加参考角(jddisplay)时，需要修改conut()中的算法，同时修改参考角(jddisplay)、buff即可。
*/

//Start address for STC12C5A60S2 EEPROM
#define IAP_ADDRESS 0x0400

#define uchar unsigned char 
#define uint unsigned int


#define display 1	//1打开测量角度时的时间显示  0关闭测量角度时的时间显示

//接近开关端口
sbit ProbeA = P3^2;
sbit ProbeB = P3^3;


uchar Y=0;				 //角度
uchar ball_dir=0;	   	 //方向
uchar ball_number=0;	 //计球数
uchar Key_value = 0;	 //正常测试的按键值

uint xdata buff[]=
{ 
	0x9a9f,0x5dca,0x11ac,
	0xe340,0xcbf0,0xb7f1,
	0xa700,0x986f,0x8e94,
	0x8445,0x7c86,0x7bc4,
	
	0x7752,0x722c,0x6e3f,
	0x6d00,0x6c00 
	
};//table


#define TableNumberOfData  sizeof(buff) / 2	//计算buff中的数据个数


uchar code jddisplay[TableNumberOfData] = 
{
	8,10,15,
	20,25,30,
	35,40,45,
	50,55,60,
	
	65,70,75,
	80,82
};

uchar jiaodu[16]  = {"                "};

	
uchar xdata bufchar[3];	
uchar xdata string1[] = {"钢珠量：  个    "};
uchar xdata string2[]	= {"钢珠方向：      "};
uchar xdata string3[] = {"角度：          "};
uchar xdata string4[] = {"周期：          "};
uchar xdata string5[] = {"请在00度进行学习"};

uchar system_mode = 0;		//0为正常模式-钢珠测试  1为学习模式 2为用学习模式测试的钢珠
char angle_number = -1;		//角度编号 在学习模式时，按一下A开始学习模式，同时这个变量会加1

unsigned long int buff_average = 0; //时间平均值

void init_time0(void)
{
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = 0;
	TL0 = 0;
	ET0 = 1;
}

void init0() interrupt 0
{
 if((Key_value==1)|(Key_value==4)){++ball_number; ball_dir=0;}	//  计小球个数  ball_dir dir
 if(Key_value==2)
 {	
	 EX1=0;
	 ball_dir=1;	 
 } 
}	
void tim0() interrupt 1
{
	TH0 = 0;
	TL0 = 0;	
}


void Int1() interrupt 2
{
	if(Key_value == 2)
	{
	 	ball_dir=2;
		EX0=0;
	}
}


void EEPROM_WriteAll()
{
	uchar i;
	
	IapEraseSector(IAP_ADDRESS);
	
	for(i=0;i<TableNumberOfData;i++)
	{
		IapProgramByte(IAP_ADDRESS+i*2+0,buff[i]>>8);	
		IapProgramByte(IAP_ADDRESS+i*2+1,buff[i]);	
	}
}


void conut0(void)
{
	bufchar[2] = TL0;
	bufchar[1] = TH0;
	TH0 = 0;
	TL0 = 0;
	bufchar[0] = TF0;
	TF0 = 0;
}	
#define ERROR 1
void conut(void)
{		
	uint x;
	conut0(); 
  	x=(bufchar[1]*256)|bufchar[2];

	if((bufchar[0]==0x01)) 
	{
		if ((x<=buff[0])&&(x>=buff[1]))  	  	{ Y=10-(((x-buff[1])*1.0)/(buff[0]-buff[1]))*2; }  	//8-10
		else if ((x<=buff[1])&&(x>=buff[2]))  	{ Y=15-(((x-buff[2])*1.0)/(buff[1]-buff[2]))*5; }  	//10-15
		else if ((x<=buff[2])&&(x>=0))        	{ Y=16-((x*1.0)/(buff[2])); }                     	//15-16     
	}
	else if ((bufchar[0]==0x00))
	{ 
		if ((x<=0xffff)&&(x>=buff[3]))     		{ Y=20-(((x-buff[3])*1.0)/(0xffff-buff[3]))*3; }  	//17-20 
		else if ((x<=buff[3])&&(x>=buff[4]))    { Y=25-(((x-buff[4])*1.0)/(buff[3]-buff[4]))*5; } 	//20-25
		else if ((x<=buff[4])&&(x>=buff[5]))    { Y=30-(((x-buff[5])*1.0)/(buff[4]-buff[5]))*5; } 	//25-30
		else if ((x<=buff[5])&&(x>=buff[6]))    { Y=35-(((x-buff[6])*1.0)/(buff[5]-buff[6]))*5; }   //30-35
		else if ((x<=buff[6])&&(x>=buff[7]))    { Y=40-(((x-buff[7])*1.0)/(buff[6]-buff[7]))*5; }   //35-40 
		else if ((x<=buff[7])&&(x>=buff[8]))    { Y=45-(((x-buff[8])*1.0)/(buff[7]-buff[8]))*5; }   //40-45
		else if ((x<=buff[8])&&(x>=buff[9]))    { Y=50-(((x-buff[9])*1.0)/(buff[8]-buff[9]))*5; }   //45-50
		else if ((x<=buff[9])&&(x>=buff[10]))   { Y=55-(((x-buff[10])*1.0)/(buff[9]-buff[10]))*5; } //50-55 
		else if ((x<=buff[10])&&(x>=buff[11]))  { Y=60-(((x-buff[11])*1.0)/(buff[10]-buff[11]))*5; }//55-60  
		else if ((x<=buff[11])&&(x>=buff[12]))  { Y=65-(((x-buff[12])*1.0)/(buff[11]-buff[12]))*5; }//60-65  
		else if ((x<=buff[12])&&(x>=buff[13]))  { Y=70-(((x-buff[13])*1.0)/(buff[12]-buff[13]))*5; }//65-70   
		else if ((x<=buff[13])&&(x>=buff[14]))  { Y=75-(((x-buff[14])*1.0)/(buff[13]-buff[14]))*5; }//70-75  
		else if ((x<=buff[14])&&(x>=buff[15]))  { Y=80-(((x-buff[15])*1.0)/(buff[14]-buff[15]))*5; }//75-80  
		else if ((x<=buff[15])&&(x>=buff[16]))  { Y=82-(((x-buff[16])*1.0)/(buff[15]-buff[16]))*2; }//80-82  
	}
	else Y=ERROR;
}

void Task1_Init(void)
{
	Display_String(1,"  测量钢珠量    ");
	
	ball_number = 0;
	string1[8] = ball_number / 10 + 0x30;
	string1[9] = ball_number % 10 + 0x30;
	Display_String(3,string1);
}

void Task2_Init(void)
{
	Display_String(1,"  测量钢珠方向  "); 
	string3[8]=0+'0';
	string3[9]=0+'0';	
	string2[10]=' '; 
	string2[11]=' '; 
	Display_String(3,string2);
	ball_dir = 0;
}

void Task3_Init(void)
{
	Display_String(1,"  测量钢珠角度  "); 
	string3[10]=' '; 
	string3[11]=' '; 
	Display_String(3,string3);
}

void Task4_Init(void)
{
	Display_String(1,"  测量钢珠周期  ");
	string4[10]=' '; 
	string4[11]=' '; 
	ball_number = 0;
	Display_String(3,string4);
}
void Task5_Init(void)
{
	buff_average = 0;
	
	string5[4] = jddisplay[angle_number]/10 + '0';
	string5[5] = jddisplay[angle_number]%10 + '0';
	Display_String(1,string5);
	
	Display_String(2,"                ");
	Display_String(3,"                ");
	Display_String(4,"                ");
}
//测量钢珠个数
void Task_1(void)
{
	string1[8] = ball_number / 10 + 0x30;
	string1[9] = ball_number % 10 + 0x30;
	Display_String(3,string1);
}
//测量钢珠方向
void Task_2(void)
{
	if(ball_dir==1)
	{
		string2[10]='1'; 
		string2[11]='0'; 	
		Display_String(3,string2);
		EX0=1;
		EX1=1;
	} 
	if(ball_dir==2)
	{
		string2[10]='0'; 
		string2[11]='1';	
		Display_String(3,string2);
		EX0=1;
		EX1=1;
	} 
}






//测量钢珠角度
void Task_3(void)
{
	uint val = 0;

	EA=0;
	TH0=0;
	TL0=0;
	TF0=0;
	EX0=0;
	while(ProbeA)
	{
		if(KEY_A == 0)
		{
			goto stop;
		} 
		if(KEY_B == 0)
		{
			goto stop;
		} 
	}
	TR0 = 1;
	while(ProbeB)
	{
		if(KEY_A == 0)
		{
			goto stop;
		}
		if(KEY_B == 0)
		{
			goto stop;
		} 
	}
	TR0 = 0;
	#if	display
	val = TF0;
	switch(val/16)
	{
		case 10:jiaodu[1]='A';break;
		case 11:jiaodu[1]='B';break;
		case 12:jiaodu[1]='C';break;
		case 13:jiaodu[1]='D';break;
		case 14:jiaodu[1]='E';break;
		case 15:jiaodu[1]='F';break;
		default:jiaodu[1] = val/16+0x30;break;
	}
	switch(val%16)
	{
		case 10:jiaodu[2]='A';break;
		case 11:jiaodu[2]='B';break;
		case 12:jiaodu[2]='C';break;
		case 13:jiaodu[2]='D';break;
		case 14:jiaodu[2]='E';break;
		case 15:jiaodu[2]='F';break;
		default:jiaodu[2] = val%16+0x30;break;
	}

	val = TH0;
	switch(val/16)
	{
		case 10:jiaodu[4]='A';break;
		case 11:jiaodu[4]='B';break;
		case 12:jiaodu[4]='C';break;
		case 13:jiaodu[4]='D';break;
		case 14:jiaodu[4]='E';break;
		case 15:jiaodu[4]='F';break;
		default:jiaodu[4] = val/16+0x30;break;
	}
	switch(val%16)
	{
		case 10:jiaodu[5]='A';break;
		case 11:jiaodu[5]='B';break;
		case 12:jiaodu[5]='C';break;
		case 13:jiaodu[5]='D';break;
		case 14:jiaodu[5]='E';break;
		case 15:jiaodu[5]='F';break;
		default:jiaodu[5] = val%16+0x30;break;
	}

 	val = TL0;
	switch(val/16)
	{
		case 10:jiaodu[7]='A';break;
		case 11:jiaodu[7]='B';break;
		case 12:jiaodu[7]='C';break;
		case 13:jiaodu[7]='D';break;
		case 14:jiaodu[7]='E';break;
		case 15:jiaodu[7]='F';break;
		default:jiaodu[7] = val/16+0x30;break;
	}
	switch(val%16)
	{
		case 10:jiaodu[8]='A';break;
		case 11:jiaodu[8]='B';break;
		case 12:jiaodu[8]='C';break;
		case 13:jiaodu[8]='D';break;
		case 14:jiaodu[8]='E';break;
		case 15:jiaodu[8]='F';break;
		default:jiaodu[8] = val%16+0x30;break;
	}
	Display_String(4,jiaodu);
	#endif

	conut();
	if(Y != ERROR)
	{
		string3[8] = Y / 10 + 0x30;
		string3[9] = Y % 10 + 0x30;
		string3[10] = ' ';
		string3[11] = ' ';
		string3[12] = ' ';
	}else
	{
		string3[8] = 'E';
		string3[9] = 'R';
		string3[10] = 'R';
		string3[11] = 'O';
		string3[12] = 'R';
	}
	Display_String(3,string3);	
	stop:;
} 



//测量钢珠周期
void Task_4(void)
{
	EX0=1;
	EA=1;
	string4[10]=ball_number/2/10+'0'; 
	string4[11]=ball_number/2%10+'0';
	Display_String(3,string4);	
}

void Task_5(void)
{
	uint val = 0;
	EA=0;
	TH0=0;
	TL0=0;
	TF0=0;
	EX0=0;
	while(ProbeA)
	{
		if(KEY_A == 0)
		{
			goto stop;
		} 
		if(KEY_B == 0)
		{
			goto stop;
		} 
	}
	TR0 = 1;
	while(ProbeB)
	{
		if(KEY_A == 0)
		{
			goto stop;
		} 
		if(KEY_B == 0)
		{
			goto stop;
		} 
	}
	TR0 = 0;
	
	val = TF0;
	switch(val/16)
	{
		case 10:jiaodu[1]='A';break;
		case 11:jiaodu[1]='B';break;
		case 12:jiaodu[1]='C';break;
		case 13:jiaodu[1]='D';break;
		case 14:jiaodu[1]='E';break;
		case 15:jiaodu[1]='F';break;
		default:jiaodu[1] = val/16+0x30;break;
	}
	switch(val%16)
	{
		case 10:jiaodu[2]='A';break;
		case 11:jiaodu[2]='B';break;
		case 12:jiaodu[2]='C';break;
		case 13:jiaodu[2]='D';break;
		case 14:jiaodu[2]='E';break;
		case 15:jiaodu[2]='F';break;
		default:jiaodu[2] = val%16+0x30;break;
	}

	val = TH0;
	switch(val/16)
	{
		case 10:jiaodu[4]='A';break;
		case 11:jiaodu[4]='B';break;
		case 12:jiaodu[4]='C';break;
		case 13:jiaodu[4]='D';break;
		case 14:jiaodu[4]='E';break;
		case 15:jiaodu[4]='F';break;
		default:jiaodu[4] = val/16+0x30;break;
	}
	switch(val%16)
	{
		case 10:jiaodu[5]='A';break;
		case 11:jiaodu[5]='B';break;
		case 12:jiaodu[5]='C';break;
		case 13:jiaodu[5]='D';break;
		case 14:jiaodu[5]='E';break;
		case 15:jiaodu[5]='F';break;
		default:jiaodu[5] = val%16+0x30;break;
	}
 	val = TL0;
	switch(val/16)
	{
		case 10:jiaodu[7]='A';break;
		case 11:jiaodu[7]='B';break;
		case 12:jiaodu[7]='C';break;
		case 13:jiaodu[7]='D';break;
		case 14:jiaodu[7]='E';break;
		case 15:jiaodu[7]='F';break;
		default:jiaodu[7] = val/16+0x30;break;
	}
	switch(val%16)
	{
		case 10:jiaodu[8]='A';break;
		case 11:jiaodu[8]='B';break;
		case 12:jiaodu[8]='C';break;
		case 13:jiaodu[8]='D';break;
		case 14:jiaodu[8]='E';break;
		case 15:jiaodu[8]='F';break;
		default:jiaodu[8] = val%16+0x30;break;
	}
	
	jiaodu[10] = '-';
	jiaodu[11] = 'N';
	jiaodu[12] = 'e';
	jiaodu[13] = 'w';
	Display_String(4,jiaodu);
	conut0();
	
	if(buff_average == 0)
	{
		buff_average = (bufchar[1]*256)|bufchar[2];
	}else
	{
		buff_average += (bufchar[1]*256)|bufchar[2];
		buff_average = buff_average / 2;
	}
	val = buff_average/256;
	switch(val/16)
	{
		case 10:jiaodu[4]='A';break;
		case 11:jiaodu[4]='B';break;
		case 12:jiaodu[4]='C';break;
		case 13:jiaodu[4]='D';break;
		case 14:jiaodu[4]='E';break;
		case 15:jiaodu[4]='F';break;
		default:jiaodu[4] = val/16+0x30;break;
	}
	switch(val%16)
	{
		case 10:jiaodu[5]='A';break;
		case 11:jiaodu[5]='B';break;
		case 12:jiaodu[5]='C';break;
		case 13:jiaodu[5]='D';break;
		case 14:jiaodu[5]='E';break;
		case 15:jiaodu[5]='F';break;
		default:jiaodu[5] = val%16+0x30;break;
	}
 	val = buff_average%256;
	switch(val/16)
	{
		case 10:jiaodu[7]='A';break;
		case 11:jiaodu[7]='B';break;
		case 12:jiaodu[7]='C';break;
		case 13:jiaodu[7]='D';break;
		case 14:jiaodu[7]='E';break;
		case 15:jiaodu[7]='F';break;
		default:jiaodu[7] = val/16+0x30;break;
	}
	switch(val%16)
	{
		case 10:jiaodu[8]='A';break;
		case 11:jiaodu[8]='B';break;
		case 12:jiaodu[8]='C';break;
		case 13:jiaodu[8]='D';break;
		case 14:jiaodu[8]='E';break;
		case 15:jiaodu[8]='F';break;
		default:jiaodu[8] = val%16+0x30;break;
	}
	jiaodu[10] = '-';
	jiaodu[11] = 'A';
	jiaodu[12] = 'V';
	jiaodu[13] = 'E';
	Display_String(3,jiaodu);	
	buff[angle_number] = buff_average;
	EEPROM_WriteAll();
	stop:;
}

void layout(uchar ball_numberer)
{
	LCD12864_Clear();
	switch(ball_numberer)
	{
		case 1:
			 
			Task1_Init();
			break;
		case 2:
			Task2_Init();

			break;
		case 3:
			Task3_Init();
			
			break;
		case 4:
			Task4_Init();
			
			break;
		default:
			break;
	}
}

void measure(uchar ball_numberer)
{
	switch(ball_numberer)
	{
		case 1:
			Task_1();
			break;
		case 2:
			Task_2();
			break;
		case 3:
			Task_3();
			break;
		case 4:
			Task_4();
			break;
		default:
			break;
	}
}

void init_ext()		//外部中断初始化
{
   EX0=1;
   IT0=1;
   EX1=1;
   IT1=1;
   EA=1;
}
uchar xdata datadisp[16] = "                ";

void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 22;
	j = 3;
	k = 227;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}



void main(void)
{
	uchar i,j,k;		//i：个数  j：行数  k：页数
	uchar k_max;		//k的最大值
	uchar keyval;		//学习模式下按键的键值
	uchar gotoflag = 0;	//跳出循环标志位
	Lcm_Init();			//LCD初始化
	
	system_mode = KeyScan();	//获取模式  什么都不按是正常用钢珠测试的模式  按A表示进入学习模式  按B表示用学习模式的数据测试钢珠角度
	if(system_mode == 0)
	{
		Display_String(1,"2017年全国大学生"); 
		Display_String(2,"  电子设计竞赛  "); 
		Display_String(3,"钢珠运动测量装置");
		Display_String(4,"请按KEYA开始测试"); 		
	}else if(system_mode == 1)
	{
		Display_String(1,"2017年全国大学生"); 
		Display_String(2,"  电子设计竞赛  "); 
		Display_String(3,"    学习模式    ");
		Display_String(4,"请按KEYA开始学习");
		Key_value = 3;//默认在测量角度模式
		EEPROM_WriteAll();

	}else if(system_mode == 2)
	{
		for(i=0;i<TableNumberOfData;i++)
		{
			buff[i] = IapReadByte(IAP_ADDRESS+i*2+0);
			buff[i] = buff[i] << 8;
			buff[i] += IapReadByte(IAP_ADDRESS+i*2+1);	
		}
		k_max = TableNumberOfData/12;
		k_max += (5>0)?1:0;
		for(k=0;k<k_max;k++)			//16一页  如果除不尽余数大于0，则页数加1
		{
			LCD12864_Clear();			//清屏
			for(j=(0+4*k);j<(4+4*k);j++)
			{
				for(i=0;i<3;i++)
				{
					if(TableNumberOfData <= ((j*3) + i)) //跳出内层循环	 注：外层循环可以自动跳出，一般手动退出较为稳妥
					{
						gotoflag = 1; 
						break;
					}	
					switch(buff[i+j*3]/16/16/16)
					{
						case 10:datadisp[0 + i*6]='A';break;
						case 11:datadisp[0 + i*6]='B';break;
						case 12:datadisp[0 + i*6]='C';break;
						case 13:datadisp[0 + i*6]='D';break;
						case 14:datadisp[0 + i*6]='E';break;
						case 15:datadisp[0 + i*6]='F';break;
						default:datadisp[0 + i*6] = buff[i+j*3]/16/16/16+0x30;break;
					}
					switch(buff[i+j*3]/16/16%16)
					{
						case 10:datadisp[1 + i*6]='A';break;
						case 11:datadisp[1 + i*6]='B';break;
						case 12:datadisp[1 + i*6]='C';break;
						case 13:datadisp[1 + i*6]='D';break;
						case 14:datadisp[1 + i*6]='E';break;
						case 15:datadisp[1 + i*6]='F';break;
						default:datadisp[1 + i*6] = buff[i+j*3]/16/16%16+0x30;break;
					}
					switch(buff[i+j*3]/16%16%16)
					{
						case 10:datadisp[2 + i*6]='A';break;
						case 11:datadisp[2 + i*6]='B';break;
						case 12:datadisp[2 + i*6]='C';break;
						case 13:datadisp[2 + i*6]='D';break;
						case 14:datadisp[2 + i*6]='E';break;
						case 15:datadisp[2 + i*6]='F';break;
						default:datadisp[2 + i*6] = buff[i+j*3]/16%16%16+0x30;break;
					}
					switch(buff[i+j*3]%16%16%16)
					{
						case 10:datadisp[3 + i*6]='A';break;
						case 11:datadisp[3 + i*6]='B';break;
						case 12:datadisp[3 + i*6]='C';break;
						case 13:datadisp[3 + i*6]='D';break;
						case 14:datadisp[3 + i*6]='E';break;
						case 15:datadisp[3 + i*6]='F';break;
						default:datadisp[3 + i*6] = buff[i+j*3]%16%16%16+0x30;break;
					}
				}
				Display_String(j+1-4*k,datadisp); //显示
				for(i=0;i<16;i++)
				{
					datadisp[i] = ' ';
				}
				if(gotoflag == 1) //跳出中层循环	 注：外层循环可以自动跳出，一般手动退出较为稳妥
				{
					break;
				}
			}
			for(i=0;i<6;i++)	//延迟6*500ms
			{
				Delay500ms();
			}
			if(gotoflag == 1)	 //跳出中层循环	 注：外层循环可以自动跳出，一般手动退出较为稳妥
			{
				break;
			}
		}
		Display_String(1,"2017年全国大学生"); 
		Display_String(2,"电子设计竞赛--UP"); 
		Display_String(3,"钢珠运动测量装置");
		Display_String(4,"请按KEYA开始测试"); 
	}

	init_ext();		//外部中断初始化
	EA = 1;
	init_time0();	//定时器0初始化
	TR0 = 0;
	while(1)
	{
		if(system_mode != 1)
		{
			if(KeyScan() == 1)
			{
				Key_value++;
				if(Key_value == 5)
				{
					Key_value = 1;
				}
				layout(Key_value);
			}
			measure(Key_value);			
		}else
		{
			keyval = KeyScan();
			if(keyval == 1)
			{
				angle_number++;
				if(angle_number == TableNumberOfData)
				{
					angle_number = 0;
				}
				Task5_Init();
			}else if(keyval == 2)
			{
				if(angle_number == 0)
				{
					angle_number = TableNumberOfData;
				}
				angle_number--;
				Task5_Init();
			}
			Task_5();
		}
	}
}