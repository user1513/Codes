#include "usart3.h"
#include "main.h"

/**************************************************************************
函数功能：串口3初始化
入口参数：pclk2:PCLK2 时钟频率(Mhz)    bound:波特率
返回  值：无
**************************************************************************/
void uart3_init(u32 pclk2,u32 bound)
{  	 
	
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<3;   //使能PORTB口时钟  
	RCC->APB1ENR|=1<<18;  //使能串口时钟 
	GPIOB->CRH&=0XFFFF00FF; 
	GPIOB->CRH|=0X00008B00;//IO状态设置
	GPIOB->ODR|=1<<10;	  
	RCC->APB1RSTR|=1<<18;   //复位串口1
	RCC->APB1RSTR&=~(1<<18);//停止复位	      	   
	//波特率设置
 	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断 
	USART3->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,0,USART3_IRQn,2);//组2，最低优先级 
}

u8 Usart3ReceiveCounter;
u8 Usart3Data[8];

/**************************************************************************
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART3_IRQHandler(void)
{
	u8 res;
	u8 checknumber = 0x0;//累加校验局部变量
	u8 i;
	if(USART3->SR&(1<<5))//接收到数据
	{	  
		res = USART3->DR;
		Usart3Data[Usart3ReceiveCounter] = res;
		switch(Usart3ReceiveCounter)
		{
			case 0:
				if(Usart3Data[Usart3ReceiveCounter] == 0x5A)
				{
					Usart3ReceiveCounter++;
				}else
				{
					Usart3ReceiveCounter = 0;
				}
				break;
			case 1:
				if(Usart3Data[Usart3ReceiveCounter] == 0x5A)
				{
					Usart3ReceiveCounter++;
				}else
				{
					Usart3ReceiveCounter = 0;
				}
				break;
			case 2:
				if(Usart3Data[Usart3ReceiveCounter] == 0x15)
				{
					Usart3ReceiveCounter++;
				}else
				{
					Usart3ReceiveCounter = 0;
				}
				break;
			case 3:
				if(Usart3Data[Usart3ReceiveCounter] == 0x03)
				{
					Usart3ReceiveCounter++;
				}else
				{
					Usart3ReceiveCounter = 0;
				}
				break;
			case 4:
				Usart3ReceiveCounter++;
				break;
			case 5:
				Usart3ReceiveCounter++;
				break;
			case 6:
				Usart3ReceiveCounter++;
				break;
			case 7:
				checknumber = 0x00;//清零
				for(i=1; i<Usart3ReceiveCounter; i++)
				{
					checknumber += Usart3Data[i];
				}
				checknumber = checknumber & 0xFF;//控制校验位在0x00-0xff之间
				if(Usart3Data[Usart3ReceiveCounter] == checknumber)
				{
					//成功
					Laser1_Distance = Usart3Data[4]<<8;
					Laser1_Distance |= Usart3Data[5];
					Laser1_Mode = Usart3Data[6];
					
				}else 
				{
					//失败
				}
				Usart3ReceiveCounter = 0;
				break;
			default:
				Usart3ReceiveCounter = 0;
				break;
		}
	}
} 

