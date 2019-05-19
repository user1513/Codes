#include <UART.h>

/*****************************************************************************
UART��ʼ��
*****************************************************************************/
void Uart_Init(void)		
{
	SCON=0X50;			//����Ϊ������ʽ1
    TMOD = 0x20;            //Set Timer1 as 8-bit auto reload mode
    TH1 = TL1 = -(FOSC/12/32/BAUD); //Set auto-reload vaule
    TR1 = 1;                //Timer1 start run
    ES = 1;                 //Enable UART interrupt
    EA = 1;                 //Open master interrupt switch
}



/*----------------------------
���ʹ�������
----------------------------*/
void SendData(uchar dat)
{
	SBUF = dat;                     //���͵�ǰ����
    while (!TI);                    //�ȴ�ǰһ�����ݷ������
    TI = 0;                         //������ͱ�־

}

