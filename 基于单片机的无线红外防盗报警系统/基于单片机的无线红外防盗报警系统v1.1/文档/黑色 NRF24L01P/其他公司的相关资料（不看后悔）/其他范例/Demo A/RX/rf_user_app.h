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
		
		/////////////////////////////functions prototype declaration
		void Rf_Init ( void );

	#else
		

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
	
	#endif
#endif


