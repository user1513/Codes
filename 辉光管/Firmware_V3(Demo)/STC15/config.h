/**
 * ��Ƭ������
 * ���ߣ�    Blanboom
 * �����£�2014-07-17
 * http://blanboom.org
 */

#ifndef		__CONFIG_H
#define		__CONFIG_H


/*********************************************************/

//#define MAIN_Fosc		22118400L	//������ʱ��
//#define MAIN_Fosc		12000000L	//������ʱ��
//#define MAIN_Fosc		11059200L	//������ʱ��
//#define MAIN_Fosc		 5529600L	//������ʱ��
#define MAIN_Fosc		24000000L	//������ʱ��


/*********************************************************/

#include	"STC15Fxxxx.H"


/**************************************************************************/

#define Main_Fosc_KHZ	(MAIN_Fosc / 1000)

/***********************************************************/
typedef 	unsigned char	uint8_t;
typedef 	unsigned int	uint16_t;
typedef 	unsigned long	uint32_t;
typedef   unsigned char uchar;
typedef   unsigned int  uint;
typedef   unsigned long	 ulong; 

#endif
