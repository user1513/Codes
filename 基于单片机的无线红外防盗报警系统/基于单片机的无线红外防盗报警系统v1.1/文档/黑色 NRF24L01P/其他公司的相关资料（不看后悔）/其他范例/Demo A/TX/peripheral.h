//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************/
/*
		FileName:	
					peripheral.h
		Author:		
					ShawnZhang
		Date:		
					Mar 21th, 09
		Description:
					
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef PERIPHERAL_H
#define PERIPHERAL_H






#ifdef PERIPHERAL_C
	
	#include "io.h"
	#include "delay.h"
	#include "typedef.h"
	#include "main.h"
	
	//////////////////////////////Remote Stick
	
	#define REMOTE_PRESS_STATU ( (PINA & (1<<2)) != (1<<2) )
	#define RELEASE_REMOTE (PINA |= (1<<2) )
	
	//////////////////////////////key port
	#define SW1_PRESS_STATU ( (PIND & (1<<3)) != (1<<3) )
	#define SW2_PRESS_STATU ( (PIND & (1<<4)) != (1<<4) )
	#define SW3_PRESS_STATU ( (PIND & (1<<5)) != (1<<5) )
	#define SW4_PRESS_STATU ( (PIND & (1<<6)) != (1<<6) )
	#define SW5_PRESS_STATU ( (PIND & (1<<7)) != (1<<7) )
	#define RELEASE_SW1 ( PORTD |= (1<<3) )	
	#define RELEASE_SW2 ( PORTD |= (1<<4) )
	#define RELEASE_SW3 ( PORTD |= (1<<5) )
	#define RELEASE_SW4 ( PORTD |= (1<<6) )
	#define RELEASE_SW5 ( PORTD |= (1<<7) )
	
	
	//////////////////////////////data packet definition
	#define PACKET_LENGTH 5
	#define ADDR_PACKET_MARK 0
	#define PACKET_MARK 0X27
	#define ADDR_PACKET_KEY 1
	#define ADDR_PACKET_ADCX 2
	#define ADDR_PACKET_ADCY 3
	#define ADDR_PACKET_CHKSUM 4
	#define MIN_DIFF_ADC 3
	
	/////////////////////////////viriables definition
	volatile MY_KEYPAD My_Key = {0};
	INT8U Rf_Buff[5];
	
	
	/////////////////////////////functions prototype

	INT8U Key_Scan( void );
	INT8U Peripheral_Task( void );
	
#else


	extern INT8U Rf_Buff[5];

	
		
	/****************************************************/
	/*
		Functions name:
			Key_Scan
		Input:
			None
		Ouput:
			Key value current
		Descrp:
			Key_value: bit0-bit4: SW1 - SW5; bit5: remote button
	*/
	extern INT8U Key_Scan( void );
	
	/****************************************************/
	/*
		Functions name:
			Peripheral_Task
		Input:
			None
		Ouput:
			1: data send; 0: no data send
		Descrp:
			send data out
	*/
	extern INT8U Peripheral_Task( void );

#endif

	



#endif


