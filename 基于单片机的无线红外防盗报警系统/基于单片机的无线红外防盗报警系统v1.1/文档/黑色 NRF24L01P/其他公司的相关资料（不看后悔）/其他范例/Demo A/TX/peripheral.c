//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************/
/*
		FileName:	
					peripheral.c
		Author:		
					ShawnZhang
		Date:		
					Mar 21th, 09
		Description:
					provide functions about peripheral, 
					Here include adc and keypad
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////




#define PERIPHERAL_C
#include "peripheral.h"








//////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////functions definition/////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////



/****************************************************/
/*
	Functions name:
		Key_Scan
	Input:
		None
	Ouput:
		Key value current
	Descrp:
		Key_value: bit0-bit4: SW1 - SW5; bit5: remote button
*/
INT8U Key_Scan( void )
{
	INT8U Key_Value = 0;
	RELEASE_SW1;
	RELEASE_SW2;
	RELEASE_SW3;
	RELEASE_SW4;
	RELEASE_SW5;
	RELEASE_REMOTE;
	if ( SW1_PRESS_STATU || SW2_PRESS_STATU || SW3_PRESS_STATU || SW4_PRESS_STATU 
			|| SW5_PRESS_STATU || REMOTE_PRESS_STATU )
	{
		_delay_ms(5);
		if ( SW1_PRESS_STATU ) 
		{
			Key_Value |= (1<<0);
		}
		if ( SW2_PRESS_STATU ) 
		{
			Key_Value |= (1<<1);
		}
		if ( SW3_PRESS_STATU ) 
		{
			Key_Value |= (1<<2);
		}
		if ( SW4_PRESS_STATU ) 
		{
			Key_Value |= (1<<3);			
		}
		if ( SW5_PRESS_STATU ) 
		{
			Key_Value |= (1<<4);			
		}
		if ( REMOTE_PRESS_STATU ) 
		{
			Key_Value |= (1<<5);			
		}
	}	
	return Key_Value;
}






/****************************************************/
/*
	Functions name:
		Perpheral_Task
	Input:
		None
	Ouput:
		1: data send; 0: no data send
	Descrp:
		send data out
*/
INT8U Peripheral_Task( void )
{
	INT8U Key_Value = 0;

	Fill_Memory( &(Rf_Buff[0]), PACKET_LENGTH, 0 );
	
	if ( (Key_Value = Key_Scan()) != My_Key.Last_KeyValue )
	{
		My_Key.Last_KeyValue = Key_Value;
		Rf_Buff[ADDR_PACKET_KEY] = Key_Value;
		return 1;
	}
	else 
	{
		return 0;
	}
	
}


