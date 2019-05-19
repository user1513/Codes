/**
 * ���Ŷ���
 * ���ߣ�    Blanboom
 * �����£�2014-07-17
 * http://blanboom.org
 */
#ifndef __PINDEF_H
#define __PINDEF_H

#include "config.h"

/**
 * MAX6921 ���Ŷ���
*/
sbit MAX6921_DIN = P2^3;
sbit MAX6921_CLK  = P2^2;
sbit MAX6921_LOAD = P2^1;
sbit MAX6921_BLANK = P2^0;
 
/**
 * GPS ���Ŷ���
*/
sbit GPS_TXD = P3^7;
sbit GPS_RXD = P3^6;
sbit GPS_PPS = P3^5;
	
/**
 * RTC ���Ŷ���
*/
sbit RTC_INT = P3^3;
sbit I2C_SCL = P1^4;
sbit I2C_SDA = P1^3;

/**
 * ���������Ŷ���
 */
sbit BUZ = P1^0;

sbit LED = P0^7;

#endif