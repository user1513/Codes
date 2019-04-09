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
		SBUF = Data; //���յ������ݷ����������� 

		while(!TI); //�ȴ����ݷ��ͽ���

		TI=0; // �����ͽ�����־����
}

void uart_init()
{

    TMOD |= 0x20; // ���ö�ʱ��1 Ϊ��ʽ2

    TH1=0xfd;  //װ���ֵ 

    TL1=0xfd; 

    TR1=1; //������ʱ��1 

    SM0=0; 

    SM1=1; //���ô���Ϊ��ʽ1

    REN=1;  // ����ʹ��

    EA=1; // �����жϿ���

    ES=0; // �򿪴����жϿ��� 

}




void serial() interrupt 4   // �����жϷ�����
{ 
//		unsigned char Data_usart = 0;
//		static num = 0; 
//    Data_usart = SBUF; // �������յ�������
//		switch(num)
//		{
//			case 0:if(Data_usart == 'O'||Data_usart == 'N') {able[num] =Data_usart; num++; }
//						 else num = 0;
//						 break;
//			case 1:able[num] =Data_usart;flag=1; num = 0; //��־λ
//			break;
//			default:num = 0; break;
//		}
    RI=0; // ������ձ�־λ 
} 
