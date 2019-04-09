//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/
/*
	FileName:
			rf_user_app.h
	Author:
			ShawnZhang
	Date:	
			Feb 20th, 09
	Desc:
		
*/
/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef RF_USER_APP_H
#define RF_USER_APP_H
	
	#ifdef RF_USER_APP_C
		
		#include "typedef.h"
		#include "io.h"
		#include "nrf24l01.h"
		#include "rf_driver.h"
		#include "uart.h"
		#include "delay.h"
		
		/////////////////////////////functions prototype declaration
		void Debug_HexData( INT8U HexData );
		void Rf_Reg_Debug( INT8U Reg );
		void Rf_Init ( void );

	#else
		
		////////////////////////////functions declaration
		/***************************************************/
		/*
			Function Name:
				Debug_HexData
			Input:
				HexData
			Output:
				none
			Desciption:
				print HexData in RS232 serial port
		*/
		extern void Debug_HexData( INT8U HexData );

		/***************************************************/		
		/*
			Function Name:
				Rf_Init
			Intpu:
				None
			Output:
				None
			Description:
				initialize rf module as below:
				pipe0, pipe0 address = 0xb2b3b4b501, payload 
				width = 4, address width = 5, frequency = 2.44Gh, 2byte CRC,
				air rate = 1bmps, power=0dBm, enable low noise amplifier,
				retransmit 10 times, delay 500us
		*/
		extern void Rf_Init ( void );
		
		/***************************************************/
		/*
			Function Name:
				Rf_Reg_Debug
			Input:
				Reg
			Output:
				none
			Desciption:
				print content of the register to debug NRF24L01
		*/
		extern void Rf_Reg_Debug( INT8U Reg );
	
	
	
	#endif
#endif


