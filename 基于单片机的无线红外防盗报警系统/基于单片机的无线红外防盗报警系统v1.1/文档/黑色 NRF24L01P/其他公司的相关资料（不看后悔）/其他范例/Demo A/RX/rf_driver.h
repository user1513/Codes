//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************/
/*
		FileName:	
					rf_driver.h
		Author:		
					ShawnZhang
		Date:		
					Dec 26h, 08
		Description:
					
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef RF_DRIVER_H
#define RF_DRIVER_H


#ifdef RF_DRIVER_C
	

	#include "typedef.h"
	#include "io.h"
	#include "delay.h"
	

	
	/////////////////////////chip macro definition
	#define SW_SPI 0
	#define ATMEGA48 1
	#define ATMEGA88 1
	#define ATMEGA168 1
	#define ATMEGA164P 2
	#define ATMEGA324P 2
	#define ATMEGA644P 2	
	#define MCUTYPE ATMEGA324P
	
	
	/////////////////////////functions prototype declaration
	void Spi_Init( INT8U Flag );
	INT8U Spi_RW_Byte( INT8U Data );
	INT8U Spi_MT_Str ( INT8U *Str );
	INT8U Spi_MT_Buff ( INT8U *Buff, INT8U Length );
	INT8U Spi_MR_Byte ( void );
	void Spi_MR_Str ( INT8U *DataBuff, INT8U Length );

	

#else

	////////////////////////IO definition for NRF24L01
	#define CSN_HIGH (PORTB |= (1<<4))
	#define CSN_LOW (PORTB &= (~(1<<4)))
	#define CE_HIGH (PORTB |= (1<<3))
	#define CE_LOW (PORTB &= (~(1<<3)))
	
	
	///////////////////////functions declaration
	/************************************************/
	/*
		functions Name:
			Spi_Init()
		Input
			None
		Output:
			None
		Desc:
			None
	*/
	extern void Spi_Init ( INT8U Flag );
	
	/************************************************/
	/*
		functions Name:
			Spi_M_T()
		Input
			statu
		Output:
			None
		Desc:
			None
	*/
	extern INT8U Spi_RW_Byte( INT8U Data );
	
	
	/************************************************/
	/*
		functions Name:
			Spi_MR_Byte
		Input
			AscData
		Output:
			None
		Desc:
			None
	*/
	extern INT8U Spi_MR_Byte ( void );
	/************************************************/
	/*
		functions Name:
			Spi_M_T()
		Input
			Str
		Output:
			statu
		Desc:
			None
	*/
	extern INT8U Spi_MT_Str ( INT8U *Str );
	
	
	/************************************************/
	/*
		functions Name:
			Spi_MR_Buff
		Input
			Reg
			*DataBuff
			Length
		Output:
			None
		Desc:
			None
	*/
	extern void Spi_MR_Buff ( INT8U *DataBuff, INT8U Length );
	
	
	/************************************************/
	/*
		functions Name:
			Spi_MT_Str
		Input
			*Str:  Address of Str
		Output:
			return statu of SPI BUS
		Desc:
			None
	*/
	extern INT8U Spi_MT_Buff ( INT8U *Buff, INT8U Length );

#endif

#endif





