//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/
/*
	FileName:
			nrf24l01.c
	Author:
			ShawnZhang
	Date:	
			Mar 20th, 09
	Descrp:
*/
/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////








#define NRF24L01_C
#include "nrf24l01.h"






//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////code below are basic operation to NRF24L01//////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************/
/*
	Function Name:
		L01_Wr_Reg
	Input:
		RegAddr:   address of register in NRF24L01
		Data:	data to write in to the register
	Output:
		Rx data 
	Desc:
		command word: MSBit to LSBit(one byte)
		Data bytes: LSByte to MSByte, MSBIT to LSBit first
*/
INT8U L01_Wr_Reg( INT8U Cmd, INT8U Data )
{	
	INT8U Rec_Data = 0xff;
	CSN_HIGH;//SET ( CSN_PORT, CSN_BIT );
	CSN_LOW;//CLR ( CSN_PORT, CSN_BIT );
	if ( (Cmd == CMD_NRF24L01_FLUSH_TX) || (Cmd == CMD_NRF24L01_FLUSH_RX) || 
			(Cmd == CMD_NRF24L01_REUSE_TX_PL) || (Cmd == CMD_NRF24L01_NOP) )
	{
		Spi_RW_Byte( Cmd );
	}
	else
	{
		Spi_RW_Byte( Cmd );
		Rec_Data = Spi_RW_Byte( Data );
	}
	CSN_HIGH;//SET ( CSN_PORT, CSN_BIT );
	return ( Rec_Data );	
}



/****************************************************/
/*
	Function Name:
		L01_Rd_Reg
	Input:
		Reg:
	Output:
		Rec_Data:  Data Recieved from MISO		
	Description:
*/
INT8U L01_Rd_Reg ( INT8U Cmd )
{
	INT8U Rec_Data = 0;
	CSN_HIGH;//SET ( CSN_PORT, CSN_BIT );
	CSN_LOW;//CLR ( CSN_PORT, CSN_BIT );	
	Spi_RW_Byte( Cmd );
	Rec_Data = Spi_MR_Byte ( );
	CSN_HIGH;//SET ( CSN_PORT, CSN_BIT );
	return Rec_Data;
}

/****************************************************/
/*
	Function Name:
		L01_Wr_Buff
	Input:
		RegAddr:	command for write the register
		*Data:		address of data to write
		Length:		Length of data to write
	Output:
		Rec_Data:	Data received from MISO
	Desc:
		None
*/
INT8U L01_Wr_Buff( INT8U RegAddr, INT8U *Data, INT8U Length )
{	
	INT8U Rec_Data = 0;
	CSN_HIGH;//SET ( CSN_PORT, CSN_BIT );
	CSN_LOW;//CLR ( CSN_PORT, CSN_BIT );		
	Rec_Data = Spi_RW_Byte ( RegAddr );
	while ( Length-- )	
	{
		Rec_Data = Spi_RW_Byte( *(Data++) );
	}
	CSN_HIGH;//SET ( CSN_PORT, CSN_BIT );	
	return Rec_Data;
}

/****************************************************/
/*
	Function Name:
		L01_Rd_Buff
	Input:
		Reg
		DataBuff
		Length
	Output:
		None
	Description:
*/
void L01_Rd_Buff ( INT8U Reg, INT8U *DataBuff, INT8U Length )
{	
	CSN_HIGH;//SET ( CSN_PORT, CSN_BIT );
	CSN_LOW;//CLR ( CSN_PORT, CSN_BIT );	
	Spi_RW_Byte( Reg );
	Spi_MR_Buff( DataBuff, Length );
	CSN_HIGH;//SET ( CSN_PORT, CSN_BIT );
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////code below to access register of NRF24L01///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////STATU(0x07) REGISTER
/****************************************************/
/*
	Function Name:
		Read_Statu_Reg
	Input:
		void
	Output:
		None
	Desc:
		Frequency = 2.4G hz + Channel Mhz
*/
INT8U L01_Rd_Statu_Reg( void )
{
	return ( L01_Rd_Reg(CMD_NRF24L01_RD_REG + 0x07) );
}
/****************************************************/
/*
	Function Name:
		L01_Clear_Irq
	Input:
		Irq_No: 4, MAX_RT; 5, TX_DS; 6, RX_DR; 7, all
	Output:
		None
	Desc:
		clear IRQ
*/
void L01_Clear_Irq( INT8U Irq_No )
{
	if ( Irq_No > 6 )
	{
		L01_Wr_Reg ( (CMD_NRF24L01_WR_REG + 0x07), (L01_Rd_Statu_Reg() | 0x70) );
	}
	else if ( Irq_No > 3 ) 
	{
		L01_Wr_Reg ( (CMD_NRF24L01_WR_REG + 0x07), (L01_Rd_Statu_Reg() | (1<<Irq_No)) );
	}
}



///////////////////////RX_PW_PX(0x11+Pipe_No) REGISTER
/****************************************************/
/*
	Function Name:
		L01_Rd_Rx_Pw_Pipe_X
	Input:		
		Pipe_No: indicate pipe		
	Output:
		None
	Desc:
		read payload width of the pipe indicated
*/
INT8U L01_Rd_Rx_Pw_Pipe_X( INT8U Pipe_No )
{
	if ( Pipe_No < 6 )
	{
		return ( L01_Rd_Reg(CMD_NRF24L01_RD_REG + 0X11 + Pipe_No) );
	}
	else 
	{
		return 0xff;
	}
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////code below is command to access NRF24L01/////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************/
/*
	Function Name:
		L01_Wr_Pl
	Input:
		*Buff:	Address of playload data
		Payload_Width:	width of payload data
	Output:
		None
	Description:
		PTX Write payload in tx mode, and PRX will send
		ack packet back
*/
void L01_Wr_Pl(INT8U *Buff, INT8U Payload_Width)
{
	L01_Wr_Buff( 0xa0 , Buff, Payload_Width );	
}


/****************************************************/
/*
	Function Name:
		L01_Read_RX_Pload
	Input:
		Buff:	Address to store data read
	Output:
		None
	Description:
		read current pipe#'s RX payload
*/
INT8U L01_Rd_RX_Pload(INT8U *Buff)      
{
	INT8U Payload_Width, Pipe_No;
	Pipe_No = (L01_Rd_Statu_Reg() & 0x0e) >> 1;  //read pipe no current used
	Payload_Width = L01_Rd_Rx_Pw_Pipe_X(Pipe_No);  //read payload width of the pipe no	
	L01_Rd_Buff( 0x61, Buff, Payload_Width);   // Then get RX data
	return Payload_Width;
}

/****************************************************/
/*
	Function Name:
		L01_Flush_Rx
	Input:
		None
	Output:
		None
	Description:
		Flux rx FIFO in rx mode, must be executed after 
		ack has been transmited
*/
void L01_Flush_Rx( void )
{
	L01_Wr_Reg( CMD_NRF24L01_FLUSH_RX, 0x00 );
}

/****************************************************/
/*
	Function Name:
		L01_Flush_Tx
	Input:
		None
	Output:
		None
	Description:
		Flux Tx FIFO in Tx mode
*/
void L01_Flush_Tx( void )
{
	L01_Wr_Reg( CMD_NRF24L01_FLUSH_TX, 0x00 );
}



