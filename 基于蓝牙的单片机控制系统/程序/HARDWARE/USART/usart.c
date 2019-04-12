#include "usart.h"
#include "string.h"

#define S2RI 0x01        //����2�����ж������־λ
#define S2TI 0x02        //����2�����ж������־λ

uint8_t xdata uart2str_able[UART2DATALENGTHMAX] = {0};//�����Ŵ���2������������

uint8_t TimeOut_status = 0;						//���峬ʱ���� 

void Uart1Init(void)		//9600bps@11.0592MHz
{
	SCON |= 0x50;            //8-bit variable UART
    TMOD |= 0x20;            //Set Timer1 as 8-bit auto reload mode
    TH1 = TL1 = -(11059200L/12/32/9600); //Set auto-reload vaule
    TR1 = 1;                //Timer1 start run
    //ES = 1;                 //Enable UART interrupt
    EA = 1;                 //Open master interrupt switch
}

void Uart2Init(void)		//9600bps@11.0592MHz
{
	AUXR &= 0xF7;			//�����ʲ�����
	S2CON = 0x50;			//8λ����,�ɱ䲨����
	AUXR &= 0xFB;			//���������ʷ�����ʱ��ΪFosc/12,��12T
	BRT = 0xFD;				//�趨���������ʷ�������װֵ
	AUXR |= 0x10;			//�������������ʷ�����
    IE2 |= 0x01;             //Enable UART2 interrupt
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

void UART_2SendOneByte(unsigned char _Data)
{
    S2BUF = _Data;
    while( ! (S2CON & S2TI) );  //��S2TI=0���ڴ˵ȴ�
    S2CON &= ~S2TI;          //S2TI=0
}

/*----------------------------
Send a string to UART
Input: s (address of string)
Output:None
----------------------------*/
void UART_2SendString(char *s)
{
    while (*s)              //Check the end of the string
    {
        UART_2SendOneByte(*s++);     //Send current char and increment string ptr
    }
}
/*----------------------------
UART2 interrupt service routine
----------------------------*/

uint8_t GetUart2Data_Length = 0;

void Uart2() interrupt 8
{
	uint8_t _UartData = 0;
    if (S2CON & S2RI)
    {
		_UartData = S2BUF;
        S2CON &= ~S2RI;     //Clear receive interrupt flag
		
		if(GetUart2Data_Length < UART2DATALENGTHMAX)
		{
			uart2str_able[GetUart2Data_Length ++] = _UartData;
		}       
    }
}

void GetUart2DataTimeout(uint8_t * men_able)
{
	static uint8_t previousUartLength = 0;     
	if(GetUart2Data_Length != 0)
	{
		if((previousUartLength ==  GetUart2Data_Length) && ((TimeOut_status & 0x01) == 0))
		{
			TimeOut_status |= 0x01;
		}
		else if(TimeOut_status & 0x02)
		{
			TimeOut_status &= ~0x02;
			TimeOut_status &= ~0x01;
			TimeOut_status |= 0x04;
			strncpy ((char *) men_able, (const char *) uart2str_able, GetUart2Data_Length); 
			GetUart2Data_Length = 0;
			previousUartLength = 0;
		}
		else if((previousUartLength !=  GetUart2Data_Length))
		{
			TimeOut_status &= ~0x01;
		}
	}
	previousUartLength = GetUart2Data_Length;
}



