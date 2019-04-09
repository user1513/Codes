//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/
/*
	FileName:
			rf_user_app.c
	Author:
			ShawnZhang
	Date:	
			Feb 20th, 09
	Desc:
			
*/
/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////


#define RF_USER_APP_C
#include "rf_user_app.h"



//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////functions definition///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
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
void Debug_HexData( INT8U HexData )
{
#if 0
	if ( (HexData>>4) > 9 )
	{
		Uart_SendChar( (HexData>>4) -10 + 'A' );
	}
	else 
	{
		Uart_SendChar( (HexData>>4) + '0' );
	}
	if ( (HexData&0x0f) > 9 )
	{
		Uart_SendChar( (HexData&0x0f) -10 + 'A' );
	}
	else 
	{
		Uart_SendChar( (HexData&0x0f) + '0' );
	}
#endif
}

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
void Rf_Reg_Debug( INT8U Reg )
{
#if 0
	Debug_HexData(Reg);
	Uart_SendStr( "H=0x" );	
	Debug_HexData( L01_Rd_Reg( Reg ) );
	Uart_SendChar( '\n' );
#endif
}



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
void Rf_Init ( void )
{
	INT8U Addr[5] = { 0xb2, 0xb3, 0xb4, 0xb5, 0x03 };  //address
	
	
	
	DDRD |= (1<<5) | (1<<6);
	PORTD |= (1<<5) | (1<<6);
	Spi_Init( ON );
	
	CE_LOW;//CLR( CE_PORT, CE_BIT ) ;
	L01_Wr_Reg( 0X20+0, 0X0F );//PRX
	L01_Wr_Reg( 0X20+1, 0X01 );
	L01_Wr_Reg( 0X20+2, 0X01 );
	L01_Wr_Reg( 0X20+3, 0X03 );
	L01_Wr_Reg( 0X20+4, 0X3A );
	
	
	
	L01_Wr_Reg( 0X20+5, 40 );  //channel
	
	
	L01_Wr_Reg( 0X20+6, 0X26 ); //250kbps
	L01_Clear_Irq( 7 );	//clear all IRQ
	L01_Wr_Buff( 0x20+0x0a, &(Addr[0]), 5 );  //rx address
	L01_Wr_Buff( 0x20+0x10, &(Addr[0]), 5 ); //TX adress
	L01_Wr_Reg( 0X20+0x11, 5);


	CE_HIGH;//	SET( CE_PORT, CE_BIT ) ;
	
	
#if 0
	Rf_Reg_Debug( 0 );
	Rf_Reg_Debug( 1 );
	Rf_Reg_Debug( 2 );
	Rf_Reg_Debug( 3 );
	Rf_Reg_Debug( 4 );
	Rf_Reg_Debug( 5 );
	Rf_Reg_Debug( 6 );
	Rf_Reg_Debug( 7 );
	Rf_Reg_Debug( 0x11 );
#endif

}




