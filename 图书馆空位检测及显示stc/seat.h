#ifndef _SEAT_H
#define _SEAT_H

#include "app.h"

#define LIBRARYSEATADDR P0//P0�����õ�ǰ��λ��ַ 
#define LIBRARYSEATNUM  P1//���õ�ǰ��λ�����ж�������
#define LIBRARYPRESENGTSEATNUM P2//��ȡ�ж���������λ��


u8 Check0_add8(u8 *_data,u8 _checklength);

int Seat_Addr();  //��λ��ַ
int Seat_Num();//��λ����
int Present_Seat_Num();//Ŀǰ��λ�ж���λ��





#endif