#include "usart1.h"
#include "main.h"

//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart1_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置 
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断 
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 
}
u8 Usart1ReceiveCounter;
u8 Usart1Data[8];

extern u8 Turn_left_90_value;
extern u8 Turn_right_90_value;

//串口数据包进行判断
void USART1_IRQHandler(void)
{
	u8 res;	
	u8 checknumber = 0x0;//累加校验局部变量
	u8 i;
	if(USART1->SR&(1<<5))	//接收到数据
	{	 
		res = USART1->DR;
		Usart1Data[Usart1ReceiveCounter] = res;
		switch(Usart1ReceiveCounter)
		{
			case 0:
				if(Usart1Data[Usart1ReceiveCounter] == 0x55)
				{
					Usart1ReceiveCounter++;
				}else
				{
					Usart1ReceiveCounter = 0;
				}
				break;
			case 1:
				if(Usart1Data[Usart1ReceiveCounter] == 0x01)
				{
					Usart1ReceiveCounter++;
				}else
				{
					Usart1ReceiveCounter = 0;
				}
				break;
			case 2:
				Usart1ReceiveCounter++;
				break;
			case 3:
				Usart1ReceiveCounter++;
				break;
			case 4:
				checknumber = 0x00;//清零
				for(i=1; i<Usart1ReceiveCounter; i++)
				{
					checknumber += Usart1Data[i];
				}
				checknumber = checknumber & 0xFF;//控制校验位在0x00-0xff之间
				if(Usart1Data[Usart1ReceiveCounter] == checknumber)
				{
					//成功
					Usart1ReceiveCounter++;
					switch(Usart1Data[2])
					{
						case 0x00:
							if((Usart1Data[3]>=1)&&(Usart1Data[3]<=6))
							{
								if(Car_Garage == 0)
								{
									Car_Garage = Usart1Data[3];
								}
							}else if(Usart1Data[3] == 7)
							{
								__set_FAULTMASK(1);
								NVIC_SystemReset();
							}else if((Usart1Data[3] >= 8)&&(Usart1Data[3] <= 9))
							{
								if(Car_Garage == 0)
								{
									Car_Garage = Usart1Data[3];
								}								
							}
							break;
						case 0x01:
								Turn_left_90_value = Usart1Data[3];
							break;
						case 0x02:
								Turn_right_90_value = Usart1Data[3];
							break;
						default:
							break;
					}
				}else 
				{
					//失败
					Usart1ReceiveCounter = 0;
				}
				break;
			case 5://包尾
				if(Usart1Data[Usart1ReceiveCounter] == 0xAA)//包尾
				{
					//OK
					//开始解析
				}
				Usart1ReceiveCounter = 0;
				break;
			default:break;
		}								     
	}
} 
