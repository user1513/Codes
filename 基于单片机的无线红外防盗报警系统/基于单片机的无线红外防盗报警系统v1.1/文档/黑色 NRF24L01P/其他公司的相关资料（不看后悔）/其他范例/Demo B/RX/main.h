/******************************************************************************************************/
/*
	FileaName:
			main.c
	Date:
			Mar 20th, 09
	Author:
			ShawnZhang
	Descrp:
			
*/


#ifndef MAIN_H
#define MAIN_H

#ifdef MAIN_C

	#include "io.h"	
	#include "interrupt.h"
	#include "typedef.h"
	#include "uart.h"
	#include "rf_driver.h"
	#include "rf_user_app.h"
	#include "nrf24l01.h"
	#include "delay.h"
	
	//global variables declaration
	volatile INT16U G_T0_Int_Cnt = 0;
	volatile INT8U G_Ext_Int0_Flag = FALSE;
	volatile INT8U G_System_Mode = LINK_MODE;
	
	//functions prototype declaration
	void Port_Init ( void );
	void Timer0_Init ( INT8U Flag );
	void ExtInt_Init( INT8U Flag );
	void SystemInit ( void );
	void Fill_Memory( INT8U *Addr, INT8U Length,INT8U Fill_Value );
#else


	/***************************************************/
	/*
		Function Name:
			Clr_Memory
		Input:
			*Addr:  Address of Memory
			Length: Length of Memory
			Fill_Value
		Output:
			None
		Desciption:
			clear memory
	*/
	extern void Fill_Memory( INT8U *Addr, INT8U Length,INT8U Fill_Value );
	
	/***************************************************/
	/*
		Function Name:
			Rf_T_Data
		Intpu:
			*Data: Address of data to transmit
			Length:	 length of data
		Output:
			none
		Description:
			transmit data to rf
	*/
	extern void Rf_T_Data( INT8U *Data, INT8U Length );
#endif


#endif




