#include "UltrasonicDataHandle.h"

//u8 UltrasonicArray[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//modbus_CRC16ะฃั้
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


u8 Add8check(u8 *Data,u8 length)
{
	u8 sum=0,i=0;
	for(i=0;i<length;i++)
	{
		sum +=*Data;
		Data++;
	}
	return sum;
}
//u8 num=0;
//void UltrasonicDataFunc(u8 _data)
//{
//	u8 sumval=0;
//	UltrasonicArray[num]=_data;
//		switch(num)
//		{
//		case 0:if(UltrasonicArray[num]==0x5A) num++; else num=0;break;
//		case 1:if(UltrasonicArray[num]==0x5A) num++; else num=0;break;
//		case 2:
//		case 3:
//		case 4:
//		case 5:
//		case 6:num++;break;
//		case 7:
//			sumval=Add8check(UltrasonicArray,7);
//			if(UltrasonicArray[num]==sumval)
//			{
//				UltrasonicArray[num+1]=0xFF;
//				num=0;
//			}
//			else 
//			{
//				num=0;
//			}
//			break;
//		default :num=0;break;
//		}
//}

