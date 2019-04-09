#include "VS1838B.h"

xdata unsigned char IRCOM[7];


//     SendByte(IRCOM[6]);        //第一位数显示          
//     SendByte(IRCOM[5]);        //第二位数显示

void vs1838b_Init(void)
{
		IE |= 0x81;                 //允许总中断中断,使能 INT0 外部中断
	TCON |= 0x01;               //触发方式为脉冲负边沿触发
}

void IR_IN() interrupt 0 using 0
{
  unsigned char j,k,N=0;
	EA = 0;
     EX0 = 0;   
	 delay_014ms(15);
	 if (IRIN==1) 
     { EX0 =1;
			 	EA = 1;
	   return;
	  } 
                           //确认IR信号出现
  while (!IRIN)            //等IR变为高电平，跳过9ms的前导低电平信号。
    {delay_014ms(1);}

 for (j=0;j<4;j++)         //收集四组数据
 { 
  for (k=0;k<8;k++)        //每组数据有8位
  {
   while (IRIN)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
     {delay_014ms(1);}
    while (!IRIN)          //等 IR 变为高电平
     {delay_014ms(1);}
     while (IRIN)           //计算IR高电平时长
      {
    delay_014ms(1);
    N++;           
    if (N>=30)
	 { EX0=1;
		 	EA = 1;
	 return;}                  //0.14ms计数过长自动离开。
      }                        //高电平计数完毕                
     IRCOM[j]=IRCOM[j] >> 1;                  //数据最高位补“0”
     if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //数据最高位补“1”
     N=0;
  }//end for k
 }//end for j
   
   if (IRCOM[2]!=~IRCOM[3])
   { EX0=1;
		 	EA = 1;
     return; }

   IRCOM[5]=IRCOM[2] & 0x0F;     //取键码的低四位
   IRCOM[6]=IRCOM[2] >> 4;       //右移4次，高四位变为低四位

   if(IRCOM[5]>9)
    { IRCOM[5]=IRCOM[5]+0x37;}
   else
	  IRCOM[5]=IRCOM[5]+0x30;

   if(IRCOM[6]>9)
    { IRCOM[6]=IRCOM[6]+0x37;}
   else
	  IRCOM[6]=IRCOM[6]+0x30;
	 
     SendByte(IRCOM[6]);        //第一位数显示          
     SendByte(IRCOM[5]);        //第二位数显示
     EX0 = 1; 
	 EA = 1;
} 


/**********************************************************/
void delay_014ms(unsigned char x)    //x*0.14MS,不同单片机，不同晶振频率，需要配置不同的delay
{
 unsigned char i, j;
  while(x--)
 {
  _nop_();
	_nop_();
	i = 2;
	j = 125;
	do
	{
		while (--j);
	} while (--i);
 }
}





