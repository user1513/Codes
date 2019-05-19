#include "bh1750fvi.h"

void bh1750fvi_Init(void) //bh1750fvi≥ı ºªØ
{
	bh1750fvi_Write(0X46,0X01);
	delay_ms(2);
	bh1750fvi_Write(0x46,0x10);
	delay_ms(2);
}

void bh1750fvi_Write(u8 Salve_addr,u8 Mode_addr)//0x46----0x10
{
	IIC_Start();
	IIC_Send_Byte(Salve_addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(Mode_addr);
	IIC_Wait_Ack();
	IIC_Stop();
}

u16 bh1750fvi_Read(u8 Salve_addr)//0X47
{
	u16 bh1750fvi_val=0;
	IIC_Start();
	IIC_Send_Byte(Salve_addr);
	IIC_Wait_Ack();
	bh1750fvi_val=IIC_Read_Byte(1);
	bh1750fvi_val<<=8;
	bh1750fvi_val|=IIC_Read_Byte(0);
	IIC_Stop();
	return bh1750fvi_val/1.2;
}





