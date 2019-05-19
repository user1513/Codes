#include "Data_Check.h"



//modbus_CRC16校验
u16 ModBus_CRC(u8 *arg,u8 length)
{
    u16 Reg_CRC=0xffff;
    u8 Temp_Reg=0x00;
    u8 i,j;
    for(i=0;i<length;i++)
    {
        Reg_CRC ^= *arg++;
        for(j=0;j<8;j++)
        {
            if(Reg_CRC&0x0001)
            {
                Reg_CRC=Reg_CRC>>1^0xA001;
            }
            else
            {
                Reg_CRC >>=1;
            }
        }
    }
    Temp_Reg=Reg_CRC;
    Reg_CRC=(Reg_CRC&0xff00)|Temp_Reg;
    return Reg_CRC;
}

//0-ADD8 校验
u8 SUMCheck(u8 *Msg, u16 DatLen)
{
    u8 i=0;
    u8 sum=0;
    for(i=0;i<DatLen;i++)
    {
        sum=Msg[i]+sum;            
    }
    sum = 0x00-sum;
    return sum;
}

//ADD8 校验
u8 SUMCheck_Add8(u8 *Msg, u16 DatLen)
{
    u8 i=0;
    u8 sum=0;
    for(i=0;i<DatLen;i++)
    {
        sum=Msg[i]+sum;            
    }
    return sum;
}


//ADD16 校验
u16 SUMCheck_Add16(u8 *Msg, u16 DatLen)
{
    u8 i=0;
    u16 sum=0;
    for(i=0;i<DatLen;i++)
    {
        sum=Msg[i]+sum;            
    }
    return sum;
}



