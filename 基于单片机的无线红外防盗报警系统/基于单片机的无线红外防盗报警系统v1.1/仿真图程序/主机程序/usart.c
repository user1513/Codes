#include "usart.h"

extern uint8_t flag;
extern uint8_t able[];
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

    ES=1; // 打开串口中断开关 

}




void serial() interrupt 4   // 串口中断服务函数
{ 
		uint8_t Data_usart = 0;
		static uint8_t num = 0; 
    Data_usart = SBUF; // 读出接收到的数据
		switch(num)
		{
			case 0:if(Data_usart == 'O'||Data_usart == 'N') {able[num] =Data_usart; num++; }
						 else num = 0;
						 break;
			case 1:able[num] =Data_usart;flag=1; num = 0; //标志位
			break;
			default:num = 0; break;
		}
    RI=0; // 清零接收标志位 
} 

//#define FOSC 11059200L      //System frequency
//#define BAUD 9600           //UART baudrate

///*Define UART parity mode*/
//#define NONE_PARITY     0   //None parity
//#define ODD_PARITY      1   //Odd parity
//#define EVEN_PARITY     2   //Even parity
//#define MARK_PARITY     3   //Mark parity
//#define SPACE_PARITY    4   //Space parity

//#define PARITYBIT NONE_PARITY   //Testing even parity



//void Usart_Init(void)
//{
//#if (PARITYBIT == NONE_PARITY)
//    SCON |= 0x50;            //8-bit variable UART
//#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
//    SCON = 0xda;            //9-bit variable UART, parity bit initial to 1
//#elif (PARITYBIT == SPACE_PARITY)
//    SCON = 0xd2;            //9-bit variable UART, parity bit initial to 0
//#endif

//    TMOD |= 0x20;            //Set Timer1 as 8-bit auto reload mode
//    TH1 = TL1 = -(FOSC/12/32/BAUD); //Set auto-reload vaule
//    TR1 = 1;                //Timer1 start run
//    ES = 1;                 //Enable UART interrupt
//    EA = 1;                 //Open master interrupt switch

//    
//}

///*----------------------------
//UART interrupt service routine
//----------------------------*/
//void Uart_Isr() interrupt 4 using 1
//{
//	uint8_t ReadData = 0;
//    if (RI)
//    {
//        RI = 0;             //Clear receive interrupt flag
//        ReadData = SBUF;          //P0 show UART data
//				SBUF = ReadData;
//    }
//    if (TI)
//    {
//        TI = 0;             //Clear transmit interrupt flag
//    }
//}

///*----------------------------
//Send a byte data to UART
//Input: dat (data to be sent)
//Output:None
//----------------------------*/
//void SendData(uint8_t dat)
//{
//    ACC = dat;              //Calculate the even parity bit P (PSW.0)
//    SBUF = ACC;             //Send data to UART buffer
//}

///*----------------------------
//Send a string to UART
//Input: s (address of string)
//Output:None
//----------------------------*/
//void SendString(uint8_t *s)
//{
//    while (*s)              //Check the end of the string
//    {
//        SendData(*s++);     //Send current char and increment string ptr
//    }
//}