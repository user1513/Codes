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
		Peripheral_Init
	Input:
		None
	Ouput:
		None
	Descrp:
*/
void Adc_Init( INT8U Adc_Channel )
{
	//ADC initialize
	//AVCC reference, ADC result right ajustment
	DIDR0 |= 0x03;  //
	ADCSRA = 0;
	ADCSRB = 0;
	ADMUX = (0X01 << 6) | (1<<5) | ( Adc_Channel );
	ADCSRA = 0X02;  //Fadc = Fosc/4
	//keypad initialize
}



/****************************************************/
/*
	Functions name:
		Get_Adc
	Input:
		Adc_Channel
	Ouput:
		Adc
	Descrp:
*/
INT8U Get_Adc( INT8U Adc_Channel )
{

	INT8U Temp = 0;
	INT8U Adc_Data[50];
	INT8U i = 0, j = 0;
	Adc_Init( Adc_Channel );
	
	for ( i = 0; i < 50; i++ )
	{
		ADCSRA |= (1 << 7) | (1 << 6);  //start ADC
		while ( ADCSRA & (1<<6) );  //wait ADIF set		
		Adc_Data[i] = ADCH;	
	}
	
	//middle value filter, sort by ascending
	for ( i = 0; i < 49; i++ )
	{
		for ( j = i + 1; j < 50; j++ )
		{
			if ( Adc_Data[i] > Adc_Data[j] )
			{
				Temp = Adc_Data[i];
				Adc_Data[i] = Adc_Data[j];
				Adc_Data[j] = Temp;
			}
		}
	}
	return Adc_Data[25];	
	
//	ADCSRA &= ( (~(1 << 7)) & (~(1 << 6)) );  //start ADC
}






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
		Chk_Sum
	Input:
		*Buff:
		Length:
	Ouput:
		Sum of buff
	Descrp:
		get sum of buff
*/
INT8U Chk_Sum( INT8U *Buff, INT8U Length )
{
	INT8U ChkSum = 0;
	while ( Length-- )
	{
		ChkSum += *(Buff++);
	}
	return ChkSum;
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
	INT8U Send_Flag = FALSE;
	INT8U Key_Value = 0;
	INT8U Adc_X = 0, Adc_Y = 0;

	Fill_Memory( &(Rf_Buff[0]), PACKET_LENGTH, 0 );
	
	if ( (Key_Value = Key_Scan()) != My_Key.Last_KeyValue )
	{
		My_Key.Last_KeyValue = Key_Value;
		LED_GREEN_FLASH;
		Send_Flag = TRUE;
	}
	
	
	Adc_X = Get_Adc(HORIZONTAL_CHANNEL);
	Adc_Y = Get_Adc(VERTICAL_CHANNEL);
	
	if ( Adc_X > My_Remote.Last_Adc_X )
	{
		if ( (Adc_X - My_Remote.Last_Adc_X) > MIN_DIFF_ADC )
		{
			Send_Flag = TRUE;
		}
	}
	else if ( (My_Remote.Last_Adc_X - Adc_X) > MIN_DIFF_ADC )
	{
		Send_Flag = TRUE;
	}
	
	if ( Adc_Y > My_Remote.Last_Adc_Y )
	{
		if ( (Adc_Y - My_Remote.Last_Adc_Y) > MIN_DIFF_ADC )
		{
			Send_Flag = TRUE;
		}
	}
	else if ( (My_Remote.Last_Adc_Y - Adc_Y) > MIN_DIFF_ADC )
	{
		Send_Flag = TRUE;
	}
	
	
	My_Remote.Last_Adc_Y = Adc_Y;
	My_Remote.Last_Adc_X = Adc_X;


	

	if ( Send_Flag == TRUE )
	{
		Rf_Buff[ADDR_PACKET_MARK] = PACKET_MARK;
		Rf_Buff[ADDR_PACKET_KEY] = Key_Value;
		Rf_Buff[ADDR_PACKET_ADCX] = Adc_X;
		Rf_Buff[ADDR_PACKET_ADCY] = Adc_Y ;		
		Rf_Buff[ADDR_PACKET_CHKSUM] = Chk_Sum( &(Rf_Buff[0]), PACKET_LENGTH-1 );
		//Uart_SendBuff( &(Rf_Buff[0]), PACKET_LENGTH );
		return 1;
	}
	else
	{
		return 0;
	}
}


