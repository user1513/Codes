#include "usart.h"

xdata unsigned char Data[5];
unsigned char Counter = 0;
bit receive_flag=0;
unsigned char receiveData = 0;

void UartInit(void)		//9600bps@11.0592MHz
{
	SCON |= 0x50;            //8-bit variable UART
    TMOD |= 0x20;            //Set Timer1 as 8-bit auto reload mode
    TH1 = TL1 = -(11059200L/12/32/9600); //Set auto-reload vaule
    TR1 = 1;                //Timer1 start run
    //ES = 1;                 //Enable UART interrupt
    EA = 1;                 //Open master interrupt switch
}


void SendByte(unsigned char byte)
{
	SBUF = byte;   //��Ҫ���͵����ݷ��뵽���ͼĴ���
	while(!TI);		  //�ȴ������������
	TI=0;			  //���������ɱ�־λ
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

/*****************************************************************************
�ַ������ͺ���
/****************************************************************************/
void Send_String_Len(unsigned char *str, unsigned char Len)		
{
	int i = 0;
	for(i = 0; i < Len; i++)
		SendByte(*(str++));							  
}





