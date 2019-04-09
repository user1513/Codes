#include "reg52.h"

sbit key = P1^7;
void sendData(unsigned char Data);
void uart_init();
void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 180;
	j = 73;
	do
	{
		while (--j);
	} while (--i);
}


void main(void)
{
	uart_init();
	while(1)
	{
		if(key == 0)
		{
			sendData('O');
			sendData('K');
		}
		else
		{
			sendData('N');
			sendData('O');
		}
		Delay100ms();
	}
}

void sendData(unsigned char Data)
{
		SBUF = Data; //将收到的数据发送至缓冲区 

		while(!TI); //等待数据发送结束

		TI=0; // 将发送结束标志清零
}

void uart_init()
{

    TMOD |= 0x20; // 设置定时器1 为方式2

    TH1=0xfd;  //装入初值 

    TL1=0xfd; 

    TR1=1; //启动定时器1 

    SM0=0; 

    SM1=1; //设置串口为方式1

    REN=1;  // 接受使能

    EA=1; // 打开总中断开关

    ES=0; // 打开串口中断开关 

}




void serial() interrupt 4   // 串口中断服务函数
{ 
//		unsigned char Data_usart = 0;
//		static num = 0; 
//    Data_usart = SBUF; // 读出接收到的数据
//		switch(num)
//		{
//			case 0:if(Data_usart == 'O'||Data_usart == 'N') {able[num] =Data_usart; num++; }
//						 else num = 0;
//						 break;
//			case 1:able[num] =Data_usart;flag=1; num = 0; //标志位
//			break;
//			default:num = 0; break;
//		}
    RI=0; // 清零接收标志位 
} 
