#include "seat.h"

int Seat_Addr()  //��λ��ַ
{
	return LIBRARYSEATADDR;
}

int Seat_Num()//��λ����
{
	return LIBRARYSEATNUM;
}

int Present_Seat_Num()//Ŀǰ��λ�ж���λ��
{
	u8 temp =~LIBRARYPRESENGTSEATNUM;
//	u8 num=0;
//	u8 temp1=0;
//	int i=0;
//	for(i = 0; i < 8; i++)
//	{
//		temp1 = temp << 7;
//		if (temp1)
//		{
//			num++;
//		}
//		temp >>= 1;
//	}
	
	return temp;
}

u8 Check0_add8(u8 *_data,u8 _checklength)
{
	u8 CheckVal=0;
	while(_checklength--)
	{
		CheckVal +=*_data;
		_data++;
	}
	CheckVal =0-CheckVal;
	return CheckVal;
}




