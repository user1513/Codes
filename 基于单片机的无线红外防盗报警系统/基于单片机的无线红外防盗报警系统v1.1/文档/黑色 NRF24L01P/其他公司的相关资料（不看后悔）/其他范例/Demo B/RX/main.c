//////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************************/
/*
	FileaName:
			main.c
	Date:
			Mar 20th, 09
	Author:
			ShawnZhang
	Descrp:
			compiler is AVR-GCC 4.3.2
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////



#define MAIN_C
#include "main.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////functions definition below///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
/***************************************************/
/*
	Function Name:
		Port_Init
	Intpu:
		None
	Output:
		None
	Description:
		None
*/
void Port_Init ( void )
{
	DDRA = 0XF0;  //PA0, PA1: ADC IN; PA2: KEY in; PA4-PA7: D0-D3
	PORTA = 0XFF; 
	DDRB = 0XBF; //PB7: SCK; PB6: MISO; PB5: MOSI; PB4: CSN; PB3: CE; PB2: LED_Y; PB1: LED_G; PB0: RST_LCD
	PORTB = 0XFF;
	DDRC = 0XFF;//PC4-PC7: D7-D4;  PC3: RD;  PC2: W/R;  PC1: A0;  PC0: CS
	PORTC = 0XFF;
	DDRD = 0X02;//PD0: RXD;  PD1: TXD;  PD2: INT0;  PD3-PD7: SW1-SW5
	PORTD = 0XFF;
	LED_YELLOW_OFF;
	LED_GREEN_OFF;
}



/***************************************************/
/*
	Function Name:
		Timer0_Init
	Input:
		Flag, 0, OFF, 1, ON
	Output:
		None
	Desciption:
		set timer0 as 8 bit timer, interrupt per 10ms//1ms.
		Note 1ms not accuracy, just 1002us speaking
		accuracyly.
*/
void Timer0_Init ( INT8U Flag )
{
	if ( Flag == ON )
	{
		TCCR0B = 0;
		TCCR0A = 0;  //normal T/C	
		TIMSK0 = 0X01; //TOIE0
		TCNT0 = 0x8b;
		TCCR0B = 0x05;  //ClkT = ClkIo/1024
	}
	else if ( Flag == OFF )
	{
		TCCR0B = 0;
	}
}



/***************************************************/
/*
	Function Name:
		ExInt_Init
	Input:
		Flag
	Output:
		none
	Desciption:
		Initialize extern interrupt
*/
void ExtInt_Init( INT8U Flag )
{
	if ( Flag == ON )
	{				
		EIFR = 0; 
		PCICR = 0;
		PCIFR = 0;
		PCMSK3 = 0;
		PCMSK2 = 0;
		PCMSK1 = 0;
		EICRA = 0x02; //INT0 falling edger active
		EIMSK = 0X01; //INT0 enable
	}
	else if ( Flag == OFF )
	{
		EIMSK = 0; //disable all interrupts
	}
}



/***************************************************/
/*
	Function Name:
		SystemInit
	Input:
		None
	Output:
		None
	Desciption:
		None
*/
void SystemInit ( void )
{	
	Port_Init( );	
	ExtInt_Init( ON );
	Uart_Init( ON );
	Timer0_Init( ON );
	Rf_Init();
	SEI;
//	Uart_SendStr( "System Initialize\n" );
}



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
void Fill_Memory( INT8U *Addr, INT8U Length,INT8U Fill_Value )
{
	while ( Length-- )
	{
		*Addr++ = Fill_Value;
	}
}


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
void Rf_T_Data( INT8U *Data, INT8U Length )
{	
	L01_Flush_Tx();	
	CE_LOW;
	L01_Wr_Pl( Data, Length );
	CE_HIGH;
	_delay_us(20); //at least 10us	
	CE_LOW;
}




/****************************************************/
/*
	Function Name:
		main
	Input:
		None
	Output:
		None
	Descrp:
		main program starts here
*/
int main( void )
{

	
	INT8U Data_Buff[5];
	INT8U Rf_Statu = 0xff;
	Fill_Memory( &(Data_Buff[0]), 5, 0);
	INT8U Timeout = 0;
	
	
	SystemInit( );
	
	
	while(1)
	{
		switch ( G_System_Mode )
		{
			case LINK_MODE:
				if ( G_Ext_Int0_Flag == TRUE )
				{
					Rf_Statu = L01_Rd_Statu_Reg( );
					
					if ( Rf_Statu & (1<<6) )	//RX_DR
					{
						L01_Clear_Irq(6);
						L01_Flush_Rx();
						G_System_Mode = WORK_MODE;
						LED_GREEN_ON;
						LED_YELLOW_ON;
					}
					
					G_Ext_Int0_Flag = FALSE;
				}
				break;
			case WORK_MODE:
				if ( G_Ext_Int0_Flag == TRUE )
				{
					Rf_Statu = L01_Rd_Statu_Reg( );
					if ( Rf_Statu & (1<<6) )	//RX_DR
					{
						G_T0_Int_Cnt = 0;
						Timeout = 0;
						while ( !( L01_Rd_Fifo_Statu() & (1<<0)) )  //data in rx fifo
						{
							Fill_Memory( &(Data_Buff[0]), 5, 0 );	
							L01_Rd_RX_Pload( &(Data_Buff[0]) );
							Uart_SendBuff( &(Data_Buff[0]), 5 );
							if ( ++Timeout > 5 )
							{
								L01_Flush_Rx();								
								break;
							}
						}
						L01_Clear_Irq(6);
					}

					G_Ext_Int0_Flag = FALSE;
				}				
				break;
			default:
				break;
		}
	}
	return 1;
}





//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////ISR part below/////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
/***************************************************/
/*
	Function Name:
		SIGNAL(TIMER0_OVF_vect)
	Input:
		None
	Output:
		None
	Desciption:
		ISR for timer0
		interrupt once per 10ms
*/
SIGNAL(TIMER0_OVF_vect)
{
	TCNT0 += 0x8b;
	CLI;
	++G_T0_Int_Cnt;	
	switch ( G_System_Mode )
	{
		case LINK_MODE:
			if ( (G_T0_Int_Cnt % 100) == 0 )
			{
				LED_YELLOW_FLASH;
			}
			break;
		case WORK_MODE:
			if ( G_T0_Int_Cnt > 5000 )  //if 5s timeout, return back to LINK_MODE
			{
				G_System_Mode = LINK_MODE;
				LED_YELLOW_OFF;
				LED_GREEN_OFF;
			}
			break;
		default:
			break;
	}
	
	SEI;
}





/***************************************************/
/*
	Function Name:
		SIGNAL ( SIG_INTERRUPT0 )
	Input:
		None
	Output:
		None
	Desciption:
		ISR external interrupt 0
*/
SIGNAL ( SIG_INTERRUPT0 )
{
//	CLI;		
	G_Ext_Int0_Flag = TRUE;	
//	SEI;
}


