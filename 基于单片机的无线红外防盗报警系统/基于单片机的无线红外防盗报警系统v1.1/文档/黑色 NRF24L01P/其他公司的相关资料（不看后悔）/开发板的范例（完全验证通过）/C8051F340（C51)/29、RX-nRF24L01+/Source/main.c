#include "C8051F340.H"
#include "nRF24L01.h"

/*
================================================================================
Function name : Clock_Init( )
Description   : Initialize the system and USB clock
Input         : None
Output        : None
================================================================================
*/
void Clock_Init( void )
{
	INT16U iTmp;

    //Enable internal high speed oscillator
    OSCICN |= 0x83;
    //Enable internal 4X MULL
    if( !( CLKMUL & ( 1<<5 ) ) )
    {
        CLKMUL = 0;
        CLKMUL = 0x00;
        CLKMUL |= ( 1<<7 );
        for( iTmp = 0; iTmp < 200; iTmp ++ );
        CLKMUL |= ( 1<<6 ) | ( 1<<7 );
        for( iTmp = 0; iTmp < 2000; iTmp ++ );
        while( !( CLKMUL & ( 1<<5 ) ) );
    }
    //USB clock: 48MHz, system clock : 48MHz
    CLKSEL = 0x03;
}
/*
================================================================================
Function name : SPI_Init( )
Description   : Initialize the SPI bus
Input         : None
Output        : None
================================================================================
*/
void SPI_Init( void )
{
	SPI0CN = 0x00;
	SPI0CFG |= ( 1<<6 );
	SPI0CKR = 10;
	SPI0CN = 0x01;        //Enable SPI
	XBR0 |= ( 1<<1 );
//strong output
	P0MDOUT |= ( 1<<0 ) | ( 1<<1 ) | ( 1<<2 ) | ( 1<<3 );
	P0 |= ( 1<<0 ) | ( 1<<1 ) | ( 1<<2 ) | ( 1<<3 );
	P0SKIP |= ( 1<<0 );
}
/*
================================================================================
Function name : SPI_ExchangeByte( )
Description   : Exchange a byte via the SPI bus
Input         : -Byte, The byte sent
Output        : The byte returned
================================================================================
*/
INT8U SPI_ExchangeByte( INT8U Byte )
{
	SPI0CN &= ~( 1<<7 );
	while( !( SPI0CN & ( 1<<1 ) ) );
	SPI0DAT = Byte;
	while( !( SPI0CN & ( 1<<7 ) ) );
	SPI0CN &= ~( 1<<7 );
	return SPI0DAT;

	/*INT8U RetValue = 0;
	INT8S btmp;
	for( btmp = 7; btmp >= 0; btmp -- )
	{
		SCK_LOW( );
		MISO_HIGH( );
		if( Byte & ( 1<<btmp ) )
		{
			MOSI_HIGH( );
		}
		else
		{
			MOSI_LOW( );
		}
		SCK_HIGH( );
		if( GET_MISO( ) ) RetValue |= ( 1<<btmp );
	}
	SCK_LOW( );
	return RetValue;*/
}
/*
================================================================================
* Name        : Timer1_Init( )
* Description : Initialize the timer1 for baudrate
* Input       : None
* Output      : None
* Note        : None
================================================================================
*///9600
void Timer1_Init( void )
{
    TH1 = 0x30;
    TL1 = 0x30;
    TCON &= 0x3f;
    TMOD &= 0x0F;
    TMOD |= ( 2<<4 );
    CKCON |= 0x00 | ( 0<<3 );
	TCON |= ( 1<<6 );
}
/*
================================================================================
* Name        : UART0_Init( )
* Description : Initialize the UART0, baudrate
* Input       : None
* Output      : None
* Note        : None
================================================================================
*/
static void UART0_Init( void )
{
    SCON0 = ( 1<<4 );

    IE |= ( 1<<4 );
	P0MDOUT |= ( 1<<4 );
    Timer1_Init( );
}
volatile unsigned char *Ptr_Send = 0;
volatile unsigned int SendCount = 0;
volatile unsigned char SendBusy = 0;
static UART0_ISR( void ) interrupt 4
{
    if( SCON0 & ( 1<<1 ) )
    {
        //TX ok
        SCON0 &= ~( 1<<1 );
		if( SendCount != 0 )
		{
			SendBusy = 1;
		 	SBUF0 = *Ptr_Send++;
			SendCount--;
		}
		else
		{
		 	SendBusy = 0;
		}

    }
    else if( SCON0 & ( 1<<0 ) )
    {
        //RX ok
        SCON0 &= ~( 1<<0 );
    }
}
void _delay_ms( INT16U x )
{
    INT16U m;
    while( x -- )
    {
        for( m = 0; m < 5000; m ++ );
    }
}
void UART0_Send( unsigned char *buf, unsigned int size )
{
	while( SendBusy );
	Ptr_Send = buf;
	SendCount = size;
	SCON0 |= ( 1<<1 );
	while( SendBusy );
}
void UART0_SendStr( char *str )
{
    UART0_Send( (INT8U*)str, strlen( str ) );
}

int main( void )
 {
    INT8U tmp;
    INT8U testbuffer[32];
	char *ReplayData = "Fine, thanks!\r\n";
	XBR1 = 0x40;
	XBR0 |= ( 1<<0 );
	Clock_Init( );
    SPI_Init( );
    UART0_Init( );
    EA = 1;
    L01_Init( );
    L01_SetRXMode( );
    L01_WriteHoppingPoint( 0 );
	UART0_SendStr( "nRF24L01P Received start!\r\n" );
    while( 1 )
    {
		L01_FlushRX( );
		L01_FlushTX( );
		L01_WriteRXPayload_InAck( ReplayData, strlen( ReplayData ) );
		while( 1 )
		{
			tmp = L01_ReadStatusReg( );
			tmp &= ( 1<<TX_DS ) | ( 1<<MAX_RT ) | ( 1<<RX_DR );
			_delay_ms( 1 );
			if( tmp != 0 )
			{
				break;
			}
		}

        if( tmp & ( 1<<TX_DS ) )
        {
         //   USART_SendStr( "发送成功\r\n" );
        }
        else if( tmp & ( 1<<MAX_RT ) )
        {
         //   USART_SendStr( "发送失败\r\n" );
        }
        else if( tmp & ( 1<<RX_DR )  )
        {
			L01_WriteRXPayload_InAck( ReplayData, strlen( ReplayData ) );
            UART0_SendStr( "Received data:" );
			for( tmp = 0; tmp < 32; tmp ++ )
	        {
	            testbuffer[tmp] = 0;
	        }
			tmp = L01_ReadRXPayload( testbuffer );
			UART0_SendStr( testbuffer);
			UART0_SendStr( "Send reply:" );
			UART0_SendStr( ( char*) ReplayData );
        }
        L01_ClearIRQ( IRQ_ALL );
    }

	return 0;
 }