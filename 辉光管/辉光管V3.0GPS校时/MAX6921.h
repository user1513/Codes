#ifndef _MAX6921_H
#define _MAX6921_H

#include <REGX51.H>


sbit MAX6921_DIN = P1^2;
sbit MAX6921_CLK  = P1^3;
sbit MAX6921_LOAD = P1^4;
sbit MAX6921_BLANK = P1^5;



void display_num(int num,int seg);


#endif