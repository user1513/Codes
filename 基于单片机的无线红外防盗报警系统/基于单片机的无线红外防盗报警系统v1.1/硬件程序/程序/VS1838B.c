#include "VS1838B.h"

xdata unsigned char IRCOM[7];


//     SendByte(IRCOM[6]);        //��һλ����ʾ          
//     SendByte(IRCOM[5]);        //�ڶ�λ����ʾ

void vs1838b_Init(void)
{
		IE |= 0x81;                 //�������ж��ж�,ʹ�� INT0 �ⲿ�ж�
	TCON |= 0x01;               //������ʽΪ���帺���ش���
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
                           //ȷ��IR�źų���
  while (!IRIN)            //��IR��Ϊ�ߵ�ƽ������9ms��ǰ���͵�ƽ�źš�
    {delay_014ms(1);}

 for (j=0;j<4;j++)         //�ռ���������
 { 
  for (k=0;k<8;k++)        //ÿ��������8λ
  {
   while (IRIN)            //�� IR ��Ϊ�͵�ƽ������4.5ms��ǰ���ߵ�ƽ�źš�
     {delay_014ms(1);}
    while (!IRIN)          //�� IR ��Ϊ�ߵ�ƽ
     {delay_014ms(1);}
     while (IRIN)           //����IR�ߵ�ƽʱ��
      {
    delay_014ms(1);
    N++;           
    if (N>=30)
	 { EX0=1;
		 	EA = 1;
	 return;}                  //0.14ms���������Զ��뿪��
      }                        //�ߵ�ƽ�������                
     IRCOM[j]=IRCOM[j] >> 1;                  //�������λ����0��
     if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //�������λ����1��
     N=0;
  }//end for k
 }//end for j
   
   if (IRCOM[2]!=~IRCOM[3])
   { EX0=1;
		 	EA = 1;
     return; }

   IRCOM[5]=IRCOM[2] & 0x0F;     //ȡ����ĵ���λ
   IRCOM[6]=IRCOM[2] >> 4;       //����4�Σ�����λ��Ϊ����λ

   if(IRCOM[5]>9)
    { IRCOM[5]=IRCOM[5]+0x37;}
   else
	  IRCOM[5]=IRCOM[5]+0x30;

   if(IRCOM[6]>9)
    { IRCOM[6]=IRCOM[6]+0x37;}
   else
	  IRCOM[6]=IRCOM[6]+0x30;
	 
     SendByte(IRCOM[6]);        //��һλ����ʾ          
     SendByte(IRCOM[5]);        //�ڶ�λ����ʾ
     EX0 = 1; 
	 EA = 1;
} 


/**********************************************************/
void delay_014ms(unsigned char x)    //x*0.14MS,��ͬ��Ƭ������ͬ����Ƶ�ʣ���Ҫ���ò�ͬ��delay
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





