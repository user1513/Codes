//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************/
/*
		FileName:	
					rf_driver.c
		Author:		
					ShawnZhang
		Date:		
					Mar 20th, 09
		Description:
				the file provide SPI functions	
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////





#define RF_DRIVER_C
#include "rf_driver.h"





//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////functions defint/ion////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************/
/*
	functions Name:
		Spi_Init()
	Input
		Flag:	ON, enable spi; OFF, disable spi
	Output:
		None
	Desc:
*/
void Spi_Init ( unsigned char Flag )	
{
	if ( Flag == ON )
	{
	#if MCUTYPE == ATMEGA48 || MCUTYPE == ATMEGA88 || MCUTYPE == ATMEGA168
		SPSR = (1<<SPI2X);	  //double SPI speed
		// enable spi as master, MSB first, SCK keep low when idle,
		//	set sclk as Fck/16
		SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	#elif MCUTYPE == ATMEGA164P || MCUTYPE == ATMEGA324P || MCUTYPE == ATMEGA644P
		//Fspi = Fosc/4 = 3MHZ, MSB first, SCK keep low when idle
		SPCR = (1<<SPE) | (1<<MSTR);
	#endif
	}
	else if ( Flag == OFF )
	{
	#if MCUTYPE == ATMEGA48 || MCUTYPE == ATMEGA88 || MCUTYPE == ATMEGA168
		SPCR &= ~(1<<SPE);
	#elif MCUTYPE == ATMEGA164P || MCUTYPE == ATMEGA324P || MCUTYPE == ATMEGA644P
		SPCR &= ~(1<<SPE);
	#endif
	}
}




/************************************************/
/*
	functions Name:
		Spi_RW_Byte()
	Input
		Data
	Output:
		SPDR:  statu of SPI BUS
	Desc:
		Master write one byte on SPI BUS
*/
unsigned char Spi_RW_Byte( unsigned char Data )
{
	INT16U Timeout = 0;
	#if MCUTYPE == ATMEGA48 || MCUTYPE == ATMEGA88  || MCUTYPE == ATMEGA168
		SPDR = Data;
		while ( !(SPSR & (1<<SPIF)) )
		{
			if ( ++Timeout == 0 )
			{
				break;
			}
			_delay_us(10);
		}
		Data = SPDR;	
	#elif MCUTYPE == ATMEGA164P || MCUTYPE == ATMEGA324P  || MCUTYPE == ATMEGA644P
		SPDR = Data;
		while ( !(SPSR & (1<<SPIF)) )
		{
			if ( ++Timeout == 0 )
			{
				break;
			}
			_delay_us(10);
		}
		Data = SPDR;
	#endif
	
	return Data; //prevent warning occurs when compile
}



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
unsigned char Spi_MT_Str ( unsigned char *Str )
{
	INT8U Data = 0xff;
	while ( *Str != 0 ) 
	{
		Data = Spi_RW_Byte ( *Str++ );			
	}
	return Data;
}




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
unsigned char Spi_MT_Buff ( unsigned char *Buff, unsigned char Length )
{
	INT8U Data = 0xff;
	while ( Length-- ) 
	{
		Data = Spi_RW_Byte ( *Buff++ );
	}
	return Data;
}



/************************************************/
/*
	functions Name:
		Spi_MR_Byte
	Input
		none
	Output:
		value of the register
	Desc:
		read one byte from SPI BUS
*/
unsigned char Spi_MR_Byte ( void )
{	
	return ( Spi_RW_Byte(0) );		//dummy write once
}



/************************************************/
/*
	functions Name:
		Spi_MR_Buff
	Input
		*DataBuff
		Length
	Output:
		None
	Desc:
		None
*/
void Spi_MR_Buff ( unsigned char *DataBuff, unsigned char Length )
{
	while ( Length--)
	{
		*(DataBuff++) = Spi_RW_Byte(0);		//read by dummy write once
	}
}






