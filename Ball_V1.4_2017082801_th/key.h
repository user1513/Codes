#ifndef __KEY_H__
#define __KEY_H__

#include <STC12C5A60S2.H>

#define uchar unsigned char 
#define uint unsigned int 

sbit KEY_A = P3^6;
sbit KEY_B = P3^7;

uchar KeyScan(void);

#endif