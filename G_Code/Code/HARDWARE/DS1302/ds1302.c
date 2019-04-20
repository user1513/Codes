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
    for (i=0; i<8; i++)             //8位计数器
    {
        SCLK = 0;                   //时钟线拉低
        delay_us(2);
        dat >>= 1;                  //数据右移一位
        if (IOIN) dat |= 0x80;        //读取数据
        SCLK = 1;                   //时钟线拉高
		delay_us(2);
    }

    return dat;
}

/**************************************
向DS1302写1字节数据
**************************************/
void DS1302_WriteByte(u8 dat)
{
    char i;
	IO_OUT();
    for (i=0; i<8; i++)             //8位计数器
    {
        SCLK = 0;                   //时钟线拉低
		delay_us(2);
        IOOUT = (dat & 0X01);                    //送出到端口
        dat >>= 1;                  //移出数据
		SCLK = 1;                   //时钟线拉高
		delay_us(2);
    }
}

/**************************************
读DS1302某地址的的数据
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
    DS1302_WriteByte(addr);         //写地址
    dat = DS1302_ReadByte();        //读数据
    SCLK = 1;
    RST = 0;

    return dat;
}

/**************************************
往DS1302的某个地址写入数据
**************************************/
void DS1302_WriteData(u8 addr, u8 dat)
{
    RST = 0;
    delay_us(2);
    SCLK = 0;
    delay_us(2);
    RST = 1;
    delay_us(2);
    DS1302_WriteByte(addr);         //写地址
    DS1302_WriteByte(dat);          //写数据
    SCLK = 1;
    RST = 0;
}

/**************************************
写入初始时间
**************************************/
void DS1302_SetTime(u8 *p)
{
    u8 addr = 0x80;
    u8 n = 7;

    DS1302_WriteData(0x8e, 0x00);   //允许写操作
    while (n--)
    {
        DS1302_WriteData(addr, *p++);
        addr += 2;
    }
    DS1302_WriteData(0x8e, 0x80);   //写保护
}

/**************************************
读取当前时间
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
初始化DS1302
**************************************/
void DS1302_Initial()
{
    RST = 0;
    SCLK = 0;
    DS1302_WriteData(0x8e, 0x00);   //允许写操作
    DS1302_WriteData(0x80, 0x00);   //时钟启动
    DS1302_WriteData(0x90, 0xa6);   //一个二极管＋4K电阻充电
    DS1302_WriteData(0x8e, 0x80);   //写保护
}

