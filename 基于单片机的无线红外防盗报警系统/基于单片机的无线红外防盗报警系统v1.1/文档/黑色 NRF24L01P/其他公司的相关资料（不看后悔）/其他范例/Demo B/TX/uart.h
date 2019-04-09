/****************************************************/
/*
		FileName:	
					uart.h
		Author:		
					ShawnZhang
		Date:		
					Dec 19th, 08
		Description:
					None
*/


#ifndef UART_H
#define UART_H


#ifdef UART_C
		
	#include "io.h"
	
	#define ON 1
	#define OFF 0
	
	#define BAUDRATE 9600
	
	#define ATMEGA48 1
	#define ATMEGA88 1
	#define ATMEGA168 1
	
	#define ATMEGA164P 2
	#define ATMEGA324P 2
	#define ATMEGA644P 2	
	
	#define MCUTYPE ATMEGA324P
	
	//functions protype declaration
	void Uart_Init( unsigned char Flag );
	void Uart_SendChar( unsigned char Data );
	void Uart_SendStr( signed char *Data );
	void Uart_SendBuff( unsigned char *Data, unsigned char Length );
	


#else

	/****************************************************/
	/*
		FunctionName:
				Usart_Init
		Input:
				None
		Output:
				None
	*/
	extern void Uart_Init( unsigned char Flag );
	
	/****************************************************/
	
	/*FunctionName:
				Usart_SendChar
		Input:
				Data
		Output:
				None
	*/
	extern void Uart_SendChar( unsigned char Data );
	
	/****************************************************/
	
	/*FunctionName:
				Usart_SendChar
		Input:
				*Data
		Output:
				None
	*/
	extern void Uart_SendStr( signed char *Data );
	
	
	/****************************************************/
	
	/*FunctionName:
				Uart_SendBuff
		Input:
				*Data
				Length
		Output:
				None
	*/
	extern void Uart_SendBuff( unsigned char *Data, unsigned char Length );

#endif


#endif


