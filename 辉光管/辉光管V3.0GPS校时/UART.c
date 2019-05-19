#include <UART.h>

/*****************************************************************************
UART初始化
*****************************************************************************/
void Uart_Init(void)		
{
	SCON=0X50;			//设置为工作方式1
    TMOD = 0x20;            //Set Timer1 as 8-bit auto reload mode
    TH1 = TL1 = -(FOSC/12/32/BAUD); //Set auto-reload vaule
    TR1 = 1;                //Timer1 start run
    ES = 1;                 //Enable UART interrupt
    EA = 1;                 //Open master interrupt switch
}



/*----------------------------
发送串口数据
----------------------------*/
void SendData(uchar dat)
{
	SBUF = dat;                     //发送当前数据
    while (!TI);                    //等待前一个数据发送完成
    TI = 0;                         //清除发送标志

}

