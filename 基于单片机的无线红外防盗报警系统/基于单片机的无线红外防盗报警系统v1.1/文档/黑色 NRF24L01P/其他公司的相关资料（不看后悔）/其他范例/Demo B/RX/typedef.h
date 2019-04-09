/****************************************************/
/*
		FileName:	
					typedef.h
		Author:		
					ShawnZhang
		Date:		
					Dec 19th, 08
		Description:
					None
*/



#ifndef TYPEDEF_H
#define TYPEDEF_H

	#define INT8U unsigned char 
	#define INT8S signed char
	#define INT16U unsigned int
	#define INT16S signed int
	#define INT32U unsigned long
	#define INT32S signed long
	
	typedef struct
	{
		INT16U Last_Adc_X;
		INT16U Last_Adc_Y;
	}REMOTE_AD;
	
	typedef struct
	{
//		INT16U Key_Press_Tick;
//		INT16U Key_Release_Tick;
		INT8U Last_KeyValue; //press or release
	}MY_KEYPAD;
	
	
	#define GET( PORT, BIT ) ((PORT) & ( 1 << BIT ))
	#define SET( PORT, BIT ) PORT |= ( 1 << BIT )
	#define CLR( PORT, BIT ) PORT &= (~( 1 << BIT ))
	
	#define SEI asm( "sei" )
	#define CLI asm( "cli" )
	
	#define ON 1
	#define OFF 0
	#define TRUE 1
	#define FALSE 0
	#define RELEASE 0
	#define PRESS 1
	
	
	
	
	#define LED_GREEN_ON { DDRB |= (1<<1); PORTB |= (1<<1); }
	#define LED_GREEN_OFF { DDRB |= (1<<1); PORTB &= (~(1<<1)); }
	#define LED_GREEN_FLASH { DDRB |= (1<<1); PORTB ^= (1<<1); }
	#define LED_YELLOW_ON { DDRB |= (1<<2); PORTB |= (1<<2); }
	#define LED_YELLOW_OFF { DDRB |= (1<<2); PORTB &= (~(1<<2)); }
	#define LED_YELLOW_FLASH { DDRB |= (1<<2); PORTB ^= (1<<2); }
	
	#define LINK_MODE 1
	#define WORK_MODE 2
	


#endif


