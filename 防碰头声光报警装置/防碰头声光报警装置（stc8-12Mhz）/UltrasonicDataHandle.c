#include "UltrasonicDataHandle.h"

u8 UltrasonicArray[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

u8 Add8check(u8 *Data,u8 length)
{
	u8 sum=0,i=0;
	for(i=0;i<length;i++)
	{
		sum +=*Data;
		Data++;
	}
	// 
	return sum;
}

u8 num=0;
void UltrasonicDataFunc(u8 GetData)
{

	u8 sumval=0;

	switch(num)
	{
		case 0:if(GetData==0x5A) {UltrasonicArray[num]=GetData;num++;} else num=0;break;
		case 1:if(GetData==0x5A) {UltrasonicArray[num]=GetData;num++;} else num=0;break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:UltrasonicArray[num]=GetData; num++;break;
		case 7:UltrasonicArray[8]=0xFF;
//			sumval=Add8check(UltrasonicArray,7);
//		if(GetData==sumval)
//		{
//			UltrasonicArray[num++]=GetData;
//			UltrasonicArray[num]=0xFF;
//			Uart1_Send(UltrasonicArray[7]);
//			num=0;
//		}
//		else 
//		{
//		//Uart1_Send((u8)Add8check(UltrasonicArray,7));
//			Uart1_Send(num);
//			num=0;
//		}
	break;
		default :num=0;break;
	}
}