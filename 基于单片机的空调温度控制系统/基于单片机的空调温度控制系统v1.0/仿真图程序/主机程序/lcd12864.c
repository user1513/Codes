#include "lcd12864.h"



void delay(unsigned int m)            //��ʱ����
{
	unsigned int i,j;
	for(i=0;i<m;i++)
		 for(j=0;j<10;j++);
}

									 
/******************************************************************/
/*                   Һ����ʼ��-�ֿ�                              */
/******************************************************************/
void   initinal(void)           //LCD�ֿ��ʼ������
{
	 delay(40);             //����40MS����ʱ����
	 RES=0;                 //��λ
	 delay(1);              //��ʱ
	 RES=1;                 //��λ�ø�
	 delay(10);
	 TransferData(0x30,0);  //Extended Function Set :8BIT����,RE=0: basic instruction set, G=0 :graphic display OFF
	 delay(100);            //����100uS����ʱ����
	 TransferData(0x30,0);  //Function Set
	 delay(37);             ////����37uS����ʱ����
	 TransferData(0x08,0);  //Display on Control
	 delay(100);            //����100uS����ʱ����
	 TransferData(0x10,0);  //Cursor Display Control�������
	 delay(100);            //����100uS����ʱ����
	 TransferData(0x0C,0);  //Display Control,D=1,��ʾ��
	 delay(100);            //����100uS����ʱ����
	 TransferData(0x01,0);  //Display Clear
	 delay(10);             //����10mS����ʱ����
	 TransferData(0x06,0);  //Enry Mode Set,�����������1λ�ƶ�
	 delay(100);            //����100uS����ʱ����
}

void lcd_write(uint8_t adder, uint8_t Data)
{
	TransferData(adder, 0);
	delay(100);
	TransferData(Data, 1);
	delay(100);
}

void lcd_mesg(unsigned char code *adder1)
{
	unsigned char i;
    TransferData(0x80,0);  //Set Graphic Display RAM Address
    delay(100);
	for(i=0;i<16;i++)
	{
	  	TransferData(*adder1,1);
	  	adder1++;
	}

     TransferData(0x90,0);  //Set Graphic Display RAM Address
      delay(100);
	for(i=0;i<16;i++)
		{
	  		TransferData(*adder1,1);
	   		adder1++;
		}
	  TransferData(0x88,0);  //Set Graphic Display RAM Address
    delay(100);
	for(i=0;i<16;i++)
	{
	  	TransferData(*adder1,1);
	  	adder1++;
	}

     TransferData(0x98,0);  //Set Graphic Display RAM Address
      delay(100);
	for(i=0;i<16;i++)
		{
	  		TransferData(*adder1,1);
	   		adder1++;
		}
	 

}



/******************************************************************/
/*                    ��������                                    */
/******************************************************************/
void   TransferData(char data1,bit DI)  //�������ݻ�������,��DI=0��,��������,��DI=1,��������.
{
		RW=0;
		RS=DI;
		delay(1);
		P0=data1;
		E=1;
		delay(1);
		E=0;
}
