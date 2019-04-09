//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/
/*
	FileName:
			nrf24l01.h
	Author:
			ShawnZhang
	Date:	
			Mar 20th, 09
	Descrp:

*/
/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NRF24L01_H
#define NRF24L01_H


#ifdef NRF24L01_C
	
	
	#include "typedef.h"
	#include "rf_driver.h"
	#include "io.h"
	
	/////////////////////////////command for nrf24l01
	#define CMD_NRF24L01_WR_REG 0X20
	#define CMD_NRF24L01_RD_REG 0X00
	#define CMD_NRF24L01_FLUSH_TX 0XE1
	#define CMD_NRF24L01_FLUSH_RX 0XE2
	#define CMD_NRF24L01_REUSE_TX_PL 0XE3	
	#define CMD_NRF24L01_NOP 0XFF
	

	/////////////////////////////functions prototype declaration	
	INT8U L01_Wr_Reg( INT8U RegAddr, INT8U Data );
	INT8U L01_Rd_Reg ( INT8U Reg );
	INT8U L01_Wr_Buff( INT8U RegAddr, INT8U *Data, INT8U Length );
	void L01_Rd_Buff ( INT8U Reg, INT8U *DataBuff, INT8U Length );


	INT8U L01_Rd_Statu_Reg( void );
	void L01_Clear_Irq( INT8U Irq_No );

	void L01_Flush_Tx( void );
	void L01_Flush_Rx( void );					



#else		
	//////////////////////////////////////////////////////
	/////////////////////////////////functions declaration
	//////////////////////////////////////////////////////
	
	////////////////////////////access register of NRF24L01
	/****************************************************/
	/*
		Function Name:
			L01_Wr_Reg
		Input:
			RegAddr:   address of register in NRF24L01
			Data:	data to write in to the register
		Output:
			None
		Desc:
			command word: MSBit to LSBit(one byte)
			Data bytes: LSByte to MSByte, MSBIT to LSBit first
	*/
	extern INT8U L01_Wr_Reg( INT8U RegAddr, INT8U Data );

	/****************************************************/
	/*
		Function Name:
			L01_Rd_Reg
		Input:
			Reg:
		Output:
			DataRead:  Data read from register of NRF24L01
		Description:
	*/
	extern INT8U L01_Rd_Reg ( INT8U Reg );
	
	/****************************************************/
	/*
		Function Name:
			L01_Wr_Buff
		Input:
			RegAddr:	command for write the register
			*Data:		address of data to write
			Length:		Length of data to write
		Output:
			Statu:		Statu of SPI operation
		Desc:
			None
	*/
	extern INT8U L01_Wr_Buff( INT8U RegAddr, INT8U *Data, INT8U Length );
	
	/****************************************************/
	/*
		Function Name:
			L01_Rd_Buff
		Input:
			Reg
			DataBuff
			Length
		Output:
			Statu:	statu of SPI read operation
		Description:
	*/
	extern void L01_Rd_Buff ( INT8U Reg, INT8U *DataBuff, INT8U Length );
	
	
	
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
	extern INT8U L01_Rd_Statu_Reg( void );
	
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
	extern void L01_Clear_Irq( INT8U Irq_No );
	
	

	
	/****************************************************/
	/*
		Function Name:
			L01_Write_TX_Pload
		Input:
			*PBuf:	Address of playload data
			Payload_Width:	width of payload data
		Output:
			None
		Description:
			Write TX payload in tx mode 
	*/
	extern void L01_Wr_Pl(INT8U *Buff, INT8U Payload_Width);

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
	extern INT8U L01_Rd_RX_Pload(INT8U *Buff) ;
	
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
	extern void L01_Flush_Rx( void );
	
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
	extern void L01_Flush_Tx( void );


#endif



#endif

