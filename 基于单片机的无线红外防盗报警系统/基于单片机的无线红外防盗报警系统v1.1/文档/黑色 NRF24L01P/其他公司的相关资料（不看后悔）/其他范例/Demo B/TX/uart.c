/****************************************************/
/*
		FileName:	
					uart.c
		Author:		
					ShawnZhang
		Date:		
					Apr 1th, 09
		Description:
					
*/




#define UART_C
#include "uart.h"





/****************************************************/
/* 		         functions declaration              */
/****************************************************/
/****************************************************/
/*
	FunctionName:
			Uart_Init
	Input:
			Flag, 1, 
	Output:
			None
*/
void Uart_Init( unsigned char Flag )
{	
	if ( Flag == ON )
	{
	#if MCUTYPE == ATMEGA88 || MCUTYPE == ATMEGA48 || MCUTYPE == ATMEGA168
		UCSR0A = 0;
		UCSR0B = 0;
		UCSR0C =  (1<<2) | (1<<1);//8 bit, 1 stop bit, no chk bit, asynchronous
		UBRR0L = 51;			//9600
		UBRR0H = 0;
		UCSR0B = ( 1<<3 ); //enable transmit		
	#elif MCUTYPE == ATMEGA164P || MCUTYPE == ATMEGA324P || MCUTYPE == ATMEGA644P
		UCSR0A = 0;
		UCSR0B = 0;
		//Set baud rate
		UBRR0H = 0;
		UBRR0L = 13; //56000 bps
		//Asynchronous USART, Set frame format: 8data, NO verification, 1stop bit
		UCSR0C = (0X03<<UCSZ00);
		// Enable receiver and transmitter
		UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
	#endif
	}
	else if ( Flag == OFF )
	{
	#if MCUTYPE == ATMEGA88 || MCUTYPE == ATMEGA48 || MCUTYPE == ATMEGA168
		UCSR0B = 0;
	#endif
	}
}




/****************************************************/

/*FunctionName:
			Uart_SendChar
	Input:
			Data
	Output:
			None
*/
void Uart_SendChar( unsigned char Data )
{
	#if MCUTYPE == ATMEGA88 || MCUTYPE == ATMEGA48 || MCUTYPE == ATMEGA168
		UDR0 = Data;
		while( !GET( UCSR0A, 6 ) );	//Wait transmit complete
		UCSR0A |= (1<<5);		//Clear complete flag
	
	#elif MCUTYPE == ATMEGA164P || MCUTYPE == ATMEGA324P || MCUTYPE == ATMEGA644P
		//Wait for empty transmit buffer
		while ( !( UCSR0A & (1<<5)) );
		//Put data into buffer, sends the data */
		UDR0 = Data;
		while ( !( UCSR0A & (1<<6)) ); //wait transmit complete
		UCSR0A |= (1<<6);
	#else
		Data = Data;
	#endif
}




/****************************************************/

/*FunctionName:
			Uart_SendChar
	Input:
			*Data
	Output:
			None
*/
void Uart_SendStr( signed char *Data )
{	
	while( *Data )
	{
		Uart_SendChar( *(Data++) );			
	}
}



/****************************************************/

/*FunctionName:
			Uart_SendBuff
	Input:
			*Data
			Length
	Output:
			None
*/
void Uart_SendBuff( unsigned char *Data, unsigned char Length )
{
	while( Length--)
	{
		Uart_SendChar( *(Data++) );
	}
} 

