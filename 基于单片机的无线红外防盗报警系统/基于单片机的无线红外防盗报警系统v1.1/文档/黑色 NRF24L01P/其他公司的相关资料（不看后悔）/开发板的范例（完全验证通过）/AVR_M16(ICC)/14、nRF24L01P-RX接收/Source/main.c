
#include "main.h"
#include "nRF24L01.h"


/*Bit field operations*/
#define SetBit( Byte, Bit )  ( Byte ) |= ( 1<<( Bit ) )
#define ClrBit( Byte, Bit )  ( Byte ) &= ~( 1<<( Bit ) )
#define GetBit( Byte, Bit )  ( ( Byte ) & ( 1<<( Bit ) ) )
#define ComBit( Byte, Bit )  ( Bytes ) ^= ( 1<<( Bit ) )
#define SetBits( Byte, Bits ) ( Byte ) |= ( Bits )
#define ClrBits( Byte, Bits ) ( Byte ) &= ~( Bits )
#define GetBits( Byte, Bits ) ( ( Byte ) & ( Bits ) )
#define ComBits( Byte, Bits ) ( Byte ) ^= ( Bits )

#define M32_MOSI   PB5
#define M32_MISO   PB6
#define M32_SCK    PB7
/*
================================================================================
Function   : USART_SendChar( )
Description: Send a single byte via the USART
InPut      : -dt, The value you want to send
Output     : None
================================================================================
*/
void USART_SendChar( unsigned char dt )
{
		UCSRA |= ( 1<<TXC );
		UDR = dt;
		while( !( UCSRA & ( 1<<TXC ) ) );
}
/*
================================================================================
Function   : USART_SendArray( )
Description: Send a array of data via the USART port
InPut      : -pBuff, The buffer to store the data
             -nBytes, How many bytes do you want to send ?
Output     : None
================================================================================
*/
void USART_SendArray( unsigned char* pBuff, unsigned char nBytes )
{
	unsigned char btmp;
	for( btmp = 0; btmp < nBytes; btmp ++ )
	{
		USART_SendChar( *( pBuff + btmp ) );
	}
}
/*
================================================================================
Function   : USART_SendStr( )
Description: Send a string of data via the USART port
InPut      : pstr->the string to be sent
Output     : None
================================================================================
*/
void USART_SendStr( char *pstr )
{
    while( *pstr )
    {
        USART_SendChar( *pstr++ );
    }
}
/*
================================================================================
Function   : USART_Init( ),
Description: Initialize the USART port
InPut      : None
Output     : None
================================================================================
*/
#define Bandrate    9600
#define F_CPU       12000000
void USART_Init( void )
{

	UCSRB = 0x00; //disable while setting baud rate
	UCSRA = 0x00;
	UCSRC = 0x86;
	UBRRL = ( F_CPU / 16 / Bandrate - 1 ) % 256;
	UBRRH = ( F_CPU / 16 / Bandrate - 1 ) >> 8;
	UCSRB = 0x98;

}
/*
================================================================================
Function : SPI_Init( )
Description : Initialize the SPI bus
Input : None
Output: None
================================================================================
*/
/*void SPI_Init( void )
{
    //IO config
    PORTB |= ( 1<<0 ) | ( 1<<5 ) | ( 1<<3 ) | ( 1<<2 ) | ( 1<<4 );
    DDRB |= ( 1<<0 ) | ( 1<<2 ) | ( 1<<3 ) | ( 1<<5 );
    DDRB &= ~( 1<<4 );
    SPCR = (1<<SPE) | (1<<MSTR);
}*/

void SPI_Init( void )
{
    /*IO config*/
    PORTB |= ( 1<<0 ) | ( 1<<5 ) | ( 1<<6 ) | ( 1<<7 ) | ( 1<<4 );
    DDRB |= ( 1<<7 ) | ( 1<<5 ) | ( 1<<0 ) | ( 1<<4 );
    DDRB &= ~( 1<<6 );
    SPCR = (1<<SPE) | (1<<MSTR);
}
/*
================================================================================
Function : SPI_ExchangeByte( )
Description : Exchange a byte via the SPI bus
Input : -value, The byte sent
Output: The byte read.
================================================================================
*/
INT8U SPI_ExchangeByte( INT8U value )
{
    SPDR = value;
	while ( !(SPSR & (1<<SPIF)) );
	return SPDR;
}

INT8U testbuffer[32];
int main( void )
{
    INT8U tmp;
	unsigned char ReplayData[] = {"�Һܺã�\r\n"};


	SPI_Init( );
	USART_Init( );
	L01_Init( );	
		L01_SetRXMode( );
L01_WriteHoppingPoint( 0 );
USART_SendStr( "nRF24L01P���ղ��Կ�ʼ��\r\n" );
        while( 1 )
    {
        for( tmp = 0; tmp < 32; tmp ++ )
        {
            testbuffer[tmp] = tmp+0x20;
        }
		L01_FlushRX( );
		L01_FlushTX( );
		L01_WriteRXPayload_InAck( ReplayData, sizeof( ReplayData ) );
		while( 1 )
		{
			tmp = L01_ReadStatusReg( );
//			USART_SendChar(tmp);  //for test
			tmp &= ( 1<<TX_DS ) | ( 1<<MAX_RT ) | ( 1<<RX_DR );		
			if( tmp != 0 )
			{
				break;
			}
		}
	 
        if( tmp & ( 1<<TX_DS ) )
        {
         //   USART_SendStr( "���ͳɹ�\r\n" );
        }
        else if( tmp & ( 1<<MAX_RT ) )
        {
         //   USART_SendStr( "����ʧ��\r\n" );
        }
        else if( tmp & ( 1<<RX_DR )  )
        {
			L01_WriteRXPayload_InAck( testbuffer, 32 );
            USART_SendStr( "�յ�����:" );
			tmp = L01_ReadRXPayload( testbuffer );
			USART_SendArray( testbuffer, tmp );
			USART_SendStr( "�ظ�����:" );
			USART_SendStr( ( char*) ReplayData );
        }
        L01_ClearIRQ( IRQ_ALL );
    }
	return 0;
}
