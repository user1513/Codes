#include "uart1.h"

#define FOSC            22118400UL
#define BRT             (65536 - FOSC / 9600 / 4)

xdata u8 UltrasonicArray[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
xdata u8 UartCmdArray[12]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
u8 num=0,Cmdnum=0;
u16 crcval=0;

void UartIsr() interrupt 4 
{
	u8 sumval=0,temp=0;
    if (RI)
    {
		RI = 0;	
		temp=SBUF;
		UltrasonicArray[num]=temp;
		switch(num)
		{
		case 0:if(UltrasonicArray[num]==0x5A) num++; else num=0;break;
		case 1:if(UltrasonicArray[num]==0xA5) num++; else num=0;break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:num++;break;
		case 7:
			sumval=Add8check(UltrasonicArray,7);
			if(UltrasonicArray[num]==sumval)
			{
				UltrasonicArray[num+1]=0xFF;
				num=0;
			}
			else 
			{
				num=0;
			}
			break;
		default :num=0;break;
		}
		
		UartCmdArray[Cmdnum]=temp;
		switch(Cmdnum)
		{
		case 0:if(UartCmdArray[num]==0xA5) Cmdnum++; else Cmdnum=0;break;
		case 1:if(UartCmdArray[num]==0x5A) Cmdnum++; else Cmdnum=0;break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:Cmdnum++;break;
		case 8:if(UartCmdArray[Cmdnum]==0xFF) Cmdnum++;else Cmdnum=0;break;
		case 9:crcval=ModBus_CRC(UartCmdArray,9);
			if((u8)(crcval)==UartCmdArray[Cmdnum])Cmdnum++; else Cmdnum=0;break;
		case 10:
			if((u8)(crcval>>8)==UartCmdArray[Cmdnum])
			{
				UartCmdArray[Cmdnum+1]=0xFF;
				Cmdnum=0;
				crcval=0;
			}
			else 
			{
				crcval=0;
				Cmdnum=0;
			}
			break;
		default :Cmdnum=0;break;
		}
    }
}

void Uart1_Init(void)		//9600bps@11.0592MHz
{
	SCON |= 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = BRT;
    TH1 = BRT >> 8;
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	ES=1;
	EA=1;
}

void Uart1_Send(u8 dat)
{
    SBUF = dat;
	while (!TI);
    TI=0;
    
}

void Uart1_SendStr(u8 *p)
{
    while (*p!='\0')
    {
        Uart1_Send(*p);
		p++;
    }
}

void Uart1SendStrLen(u8 *p,u8 length)
{
    while (length--)
    {
        Uart1_Send(*p);
		p++;
    }
}