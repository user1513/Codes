#ifndef _SEAT_H
#define _SEAT_H

#include "app.h"

#define LIBRARYSEATADDR P0//P0口设置当前座位地址 
#define LIBRARYSEATNUM  P1//设置当前座位可以有多少人坐
#define LIBRARYPRESENGTSEATNUM P2//获取有多少人坐了位子


u8 Check0_add8(u8 *_data,u8 _checklength);

int Seat_Addr();  //座位地址
int Seat_Num();//座位总数
int Present_Seat_Num();//目前座位有多少位子





#endif