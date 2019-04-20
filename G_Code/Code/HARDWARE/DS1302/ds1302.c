#include "ds1302.h"




void Ds1302_GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitTypeState;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeState.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitTypeState.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_13;
	GPIO_InitTypeState.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitTypeState);
	
	GPIO_InitTypeState.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitTypeState.GPIO_Pin = GPIO_Pin_14;	
	GPIO_Init(GPIOB, &GPIO_InitTypeState);
}


u8 DS1302_ReadByte()
{
    u8 i;
    u8 dat = 0;
	IO_IN();
    for (i=0; i<8; i++)             //8λ������
    {
        SCLK = 0;                   //ʱ��������
        delay_us(2);
        dat >>= 1;                  //��������һλ
        if (IOIN) dat |= 0x80;        //��ȡ����
        SCLK = 1;                   //ʱ��������
		delay_us(2);
    }

    return dat;
}

/**************************************
��DS1302д1�ֽ�����
**************************************/
void DS1302_WriteByte(u8 dat)
{
    char i;
	IO_OUT();
    for (i=0; i<8; i++)             //8λ������
    {
        SCLK = 0;                   //ʱ��������
		delay_us(2);
        IOOUT = (dat & 0X01);                    //�ͳ����˿�
        dat >>= 1;                  //�Ƴ�����
		SCLK = 1;                   //ʱ��������
		delay_us(2);
    }
}

/**************************************
��DS1302ĳ��ַ�ĵ�����
**************************************/
u8 DS1302_ReadData(u8 addr)
{
    u8 dat;

    RST = 0;
	delay_us(2);
    SCLK = 0;
    delay_us(2);
    RST = 1;
    delay_us(2);
    DS1302_WriteByte(addr);         //д��ַ
    dat = DS1302_ReadByte();        //������
    SCLK = 1;
    RST = 0;

    return dat;
}

/**************************************
��DS1302��ĳ����ַд������
**************************************/
void DS1302_WriteData(u8 addr, u8 dat)
{
    RST = 0;
    delay_us(2);
    SCLK = 0;
    delay_us(2);
    RST = 1;
    delay_us(2);
    DS1302_WriteByte(addr);         //д��ַ
    DS1302_WriteByte(dat);          //д����
    SCLK = 1;
    RST = 0;
}

/**************************************
д���ʼʱ��
**************************************/
void DS1302_SetTime(u8 *p)
{
    u8 addr = 0x80;
    u8 n = 7;

    DS1302_WriteData(0x8e, 0x00);   //����д����
    while (n--)
    {
        DS1302_WriteData(addr, *p++);
        addr += 2;
    }
    DS1302_WriteData(0x8e, 0x80);   //д����
}

/**************************************
��ȡ��ǰʱ��
**************************************/
void DS1302_GetTime(u8 *p)
{
    u8 addr = 0x81;
    u8 n = 7;
	u8 temp = 0;

    while (n--)
    {
        temp = DS1302_ReadData(addr);
		*p = ((temp)>>4)*10 + ((temp)&0x0F);
		p++;
        addr += 2;
    }
}

/**************************************
��ʼ��DS1302
**************************************/
void DS1302_Initial()
{
    RST = 0;
    SCLK = 0;
    DS1302_WriteData(0x8e, 0x00);   //����д����
    DS1302_WriteData(0x80, 0x00);   //ʱ������
    DS1302_WriteData(0x90, 0xa6);   //һ�������ܣ�4K������
    DS1302_WriteData(0x8e, 0x80);   //д����
}

