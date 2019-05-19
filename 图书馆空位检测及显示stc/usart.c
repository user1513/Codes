#include "usart.h"

void UartInit(void)		//9600bps@11.0592MHz
{
	SCON |= 0x50;            //8-bit variable UART
    TMOD |= 0x20;            //Set Timer1 as 8-bit auto reload mode
    TH1 = TL1 = -(11059200L/12/32/9600); //Set auto-reload vaule
    TR1 = 1;                //Timer1 start run
    ES = 1;                 //Enable UART interrupt
    EA = 1;                 //Open master interrupt switch
}


void SendByte(unsigned char byte)
{
	SBUF = byte;   //��Ҫ���͵����ݷ��뵽���ͼĴ���
	while(!TI);		  //�ȴ������������
	TI=0;			  //���������ɱ�־λ
}

void Send_String_Length(unsigned char *str,u8 length)	
{
	while(length--)
	{
		SendByte(*str);			
		str++;	
	}
}
/*****************************************************************************
�ַ������ͺ���
/****************************************************************************/
void Send_String(unsigned char *str)		
{
	while(*str != '\0')		
	{
		SendByte(*str);			
		str++;		
	}				  
}
//���ڽ�������
//֡ͷ 55
//֡ͷ AA
//cmd  01
//data xx
//���� 0-add8
//��β 0xFF

u8 uart_data[6];
u8 uart_num=0;
u8 uartval=0;
void Uart_Isr() interrupt 4 using 1
{
    if (RI)
    {
        RI = 0;             //Clear receive interrupt flag
		if(uart_get_flag==0)
		{
			uart_data[uart_num]=SBUF;
			switch(uart_num)
			{
				case 0:if(uart_data[uart_num]==0X55)uart_num++;else uart_num=0; break;
				case 1:if(uart_data[uart_num]==0XAA)uart_num++;else uart_num=0; break;
				case 2:
				case 3:
				case 4:uart_num++;break;//if(Check0_add8(uart_data,4)==uart_data[uart_num]){uart_num++;}else uart_num=0;break;
				case 5:if(uart_data[uart_num]==0XFF){uart_num=0;uart_get_flag=1;}else uart_num=0;break;
				default:uart_num=0;break;				
			}		
		}
    }
}



