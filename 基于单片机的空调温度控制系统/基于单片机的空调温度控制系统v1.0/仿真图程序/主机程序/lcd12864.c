#include "lcd12864.h"



void delay(unsigned int m)            //延时程序
{
	unsigned int i,j;
	for(i=0;i<m;i++)
		 for(j=0;j<10;j++);
}

									 
/******************************************************************/
/*                   液晶初始化-字库                              */
/******************************************************************/
void   initinal(void)           //LCD字库初始化程序
{
	 delay(40);             //大于40MS的延时程序
	 RES=0;                 //复位
	 delay(1);              //延时
	 RES=1;                 //复位置高
	 delay(10);
	 TransferData(0x30,0);  //Extended Function Set :8BIT设置,RE=0: basic instruction set, G=0 :graphic display OFF
	 delay(100);            //大于100uS的延时程序
	 TransferData(0x30,0);  //Function Set
	 delay(37);             ////大于37uS的延时程序
	 TransferData(0x08,0);  //Display on Control
	 delay(100);            //大于100uS的延时程序
	 TransferData(0x10,0);  //Cursor Display Control光标设置
	 delay(100);            //大于100uS的延时程序
	 TransferData(0x0C,0);  //Display Control,D=1,显示开
	 delay(100);            //大于100uS的延时程序
	 TransferData(0x01,0);  //Display Clear
	 delay(10);             //大于10mS的延时程序
	 TransferData(0x06,0);  //Enry Mode Set,光标从右向左加1位移动
	 delay(100);            //大于100uS的延时程序
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
/*                    函数声明                                    */
/******************************************************************/
void   TransferData(char data1,bit DI)  //传送数据或者命令,当DI=0是,传送命令,当DI=1,传送数据.
{
		RW=0;
		RS=DI;
		delay(1);
		P0=data1;
		E=1;
		delay(1);
		E=0;
}
