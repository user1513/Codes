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
//////////////////////////////CONFIGURE(0x00) REGISTER
/****************************************************/
/*
	Function Name:
		L01_Power
	Input:
		Flag: ON, Power up; OFF, Power down
	Output:
		None
	Desc:
		control Power of NRF24L01
*/
void L01_Power( INT8U Flag )
{
	if ( Flag == ON )
	{
		L01_Wr_Reg( CMD_NRF24L01_WR_REG + 0x00, ( L01_Rd_Reg(CMD_NRF24L01_RD_REG+0x00) | (1<<1) ) );
	}
	else if ( Flag == OFF )
	{
		L01_Wr_Reg( CMD_NRF24L01_WR_REG + 0x00, ( L01_Rd_Reg(CMD_NRF24L01_RD_REG+0x00) & (~(1<<1)) ) );
	}
}

/****************************************************/
/*
	Function Name:
		L01_Mode
	Input:
		Mode: 0, PTX; 1, PRX
	Output:
		None
	Desc:
		configure 24L01 as PTX or PRX
*/
void L01_Mode( INT8U Mode )
{
	if ( Mode == 0 )
	{
		L01_Wr_Reg ( (CMD_NRF24L01_WR_REG + 0X00), (L01_Rd_Reg(CMD_NRF24L01_RD_REG + 0x00) & (~(1<<0))) );
	}
	else if ( Mode == 1 )
	{
		L01_Wr_Reg ( (CMD_NRF24L01_WR_REG + 0X00), (L01_Rd_Reg(CMD_NRF24L01_RD_REG + 0x00) | (1<<0)) );
	}
}



////////////////////////////// SETUP_AW(0x03) REGISTER
/****************************************************/
/*
	Function Name:
		En_RxAddr
	Input:
		Width: from 3 to 5; other value invalid		
	Output:
		None
	Desc:
		setup address width
*/
void L01_Setup_Addr_Width( INT8U Width )
{
	L01_Wr_Reg( (CMD_NRF24L01_WR_REG + 0x03), (Width - 2) );
}



/////////////////////////////SETUP_RETR(0x04) REGISTER
/****************************************************/
/*
	Function Name:
		L01_Setup_ReTransmit
	Input:
		ART: from 0 to 15. 0, disable retransmit; 
			1-15: means count of retransmit
		ARD: from 0 to 15. means 250us, 500us, 750us, 
				..., 4000us		
	Output:
		None
	Desc:
		setup retranmist parameters
*/
void L01_Setup_ReTransmit( INT8U ART, INT8U ARD )
{
	L01_Wr_Reg( (CMD_NRF24L01_WR_REG + 0x04), ((ARD << 4) | ART) );
}




//////////////////////////////////RF_CH(0x05) REGISTER
/****************************************************/
/*
	Function Name:
		L01_Setup_Rf_Channel
	Input:
		Channel: 
	Output:
		None
	Desc:
		Frequency = 2.4G hz + Channel Mhz
*/
void L01_Setup_Rf_Channel( INT8U Channel )
{
	L01_Wr_Reg( (CMD_NRF24L01_WR_REG + 0x05), Channel );
}




///////////////////////////////RF_SETUP(0x06) REGISTER
/****************************************************/
/*
	Function Name:
		L01_Setup_Rf_Parameter
	Input:
		Air_Rate: 0, 1Mbps; 1, 2Mbps
		Power: 0, -18dBm; 1, -12dBm; 2, -6dBm; 3, 0dBm
		LNA: 1, enable low noise amplifier; 0, disable
	Output:
		None
	Desc:
		Frequency = 2.4G hz + Channel Mhz
*/
void L01_Setup_Rf_Parameter( INT8U Air_Rate, INT8U Power, INT8U LNA )
{
	L01_Wr_Reg( (CMD_NRF24L01_WR_REG + 0X06), (( Air_Rate << 3 ) | ( Power << 1 ) | LNA));
}




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




////////////////////RX_ADDR_PX(0x0a+Pipe_No) REGISTER
/****************************************************/
/*
	Function Name:
		L01_Wr_Rx_Addr
	Input:
		Pipe_No: indicate the pipe
		*Addr: Address of rx address
		Length: Length of rx address
	Output:
		None
	Desc:
		write RX address of the pipe indicated by Pipe_No
*/
void L01_Wr_Rx_Addr( INT8U Pipe_No, INT8U *Addr, INT8U Length )
{
	if ( Pipe_No < 6 )
	{
		L01_Wr_Buff( (CMD_NRF24L01_WR_REG + 0X0A + Pipe_No), Addr, Length );
	}
}



////////////////////////////////TX_ADDR REGISTER(0x10)
/****************************************************/
/*
	Function Name:
		L01_Wr_Tx_Addr
	Input:		
		*Addr: Address of tx address
		Length: Length of tx address
	Output:
		None
	Desc:
		write TX address
*/
void L01_Wr_Tx_Addr( INT8U *Addr, INT8U Length )
{
	L01_Wr_Buff( (CMD_NRF24L01_WR_REG + 0X10), Addr, Length );
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
void L01_Wr_Rx_Pw_Pipe_X( INT8U Pipe_No, INT8U Payload_Width )
{
	if ( Pipe_No < 6 )
	{
		L01_Wr_Reg(CMD_NRF24L01_WR_REG + 0X11 + Pipe_No, Payload_Width);
	}
}


////////////////////////////FIFO_STATUS(0X17) REGISTER
/****************************************************/
/*
	Function Name:
		L01_Rd_Fifo_Statu
	Input:		
		None		
	Output:
		Status of FIFO
	Desc:		
*/
INT8U L01_Rd_Fifo_Statu( void )
{
	return ( L01_Rd_Reg( CMD_NRF24L01_RD_REG + 0x17 ) );
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////code below is command to access NRF24L01/////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************/
/*
	Function name:
		L01_Activate
	Input:
		None
	Output:
		None
	Descr:
		Before use the 3 commands:R_RX_PL_WID, W_ACK_PAYLOAD, 
		W_TX_PAYLOAD_NOACK, you must Activate first, else a 
		write has no effect, a read results in zero on MISO
*/
void L01_Activate( void )
{
	if ( (L01_Rd_Reg(0x1c) == 0x00) && (L01_Rd_Reg(0x1d) == 0x00) )
	{
		L01_Wr_Reg(0x50, 0x73);
	}
}

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
	Function name:
		L01_Wr_Pl_In_Ack
	Input:
		*Buff: Address of Payload
		Length: Length of Payload
	Output:
		None
	Descr:
		Write payload user defined to ACK packet in RX mode
*/
void L01_Wr_Pl_In_Ack ( INT8U *Buff, INT8U Length )
{
	L01_Wr_Buff ( 0xa8, Buff, Length );	
}

/****************************************************/
/*
	Function name:
		L01_Wr_Pl_NoAck
	Input:
		*Buff
		Length
	Output:
		None
	Descr:
		Write payload in tx mode, need't ack on this packet
*/
void L01_Wr_Pl_NoAck ( INT8U *Buff, INT8U Length )
{
	L01_Wr_Buff ( 0xb0, Buff, Length );
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



