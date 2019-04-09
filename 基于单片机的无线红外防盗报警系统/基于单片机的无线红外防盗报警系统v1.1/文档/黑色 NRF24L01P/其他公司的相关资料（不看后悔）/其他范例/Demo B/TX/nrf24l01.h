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

	void L01_Power( INT8U Flag );
	void L01_Mode( INT8U Mode );
	void L01_Setup_Addr_Width( INT8U Width );
	void L01_Setup_ReTransmit( INT8U Retran_Cnt, INT8U Delay );
	void L01_Setup_Rf_Channel( INT8U Channel );
	void L01_Setup_Rf_Parameter( INT8U Air_Rate, INT8U Power, INT8U LNA );
	INT8U L01_Rd_Statu_Reg( void );
	void L01_Clear_Irq( INT8U Irq_No );
	void L01_Wr_Rx_Addr( INT8U Pipe_No, INT8U *Addr, INT8U Length );
	void L01_Wr_Tx_Addr( INT8U *Addr, INT8U Length );
	void L01_Wr_Rx_Pw_Pipe_X( INT8U Pipe_No, INT8U Payload_Width );
	INT8U L01_Rd_Rx_Pw_Pipe_X( INT8U Pipe_No );
	
	void L01_Activate( void );
	void L01_Wr_Pl(INT8U *Buff, INT8U Payload_Width);
	void L01_Wr_Pl_In_Ack ( INT8U *Buff, INT8U Length );
	void L01_Wr_Pl_NoAck ( INT8U *Buff, INT8U Length );
	INT8U L01_Rd_RX_Pload(INT8U *Buff);
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
			L01_Power
		Input:
			Flag: ON, Power up; OFF, Power down
		Output:
			None
		Desc:
			control Power of NRF24L01
	*/
	extern void L01_Power( INT8U Flag );

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
	extern	void L01_Mode( INT8U Mode );
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
	extern void L01_Setup_Addr_Width( INT8U Width );
	
	/****************************************************/
	/*
		Function Name:
			L01_Setup_ReTransmit
		Input:
			Retran_Cnt: from 0 to 15. 0, disable retransmit; 
				1-15: means count of retransmit
			Delay: from 0 to 15. means 250us, 500us, 750us, 
					..., 4000us		
		Output:
			None
		Desc:
			setup retranmist parameters
	*/
	extern void L01_Setup_ReTransmit( INT8U Retran_Cnt, INT8U Delay );
	
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
	extern void L01_Setup_Rf_Channel( INT8U Channel );
	
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
	extern void L01_Setup_Rf_Parameter( INT8U Air_Rate, INT8U Power, INT8U LNA );
	
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
			L01_Rd_Rx_Pw_Pipe_X
		Input:		
			Pipe_No: indicate pipe		
		Output:
			None
		Desc:
			read payload width of the pipe indicated
	*/
	extern INT8U L01_Rd_Rx_Pw_Pipe_X( INT8U Pipe_No );
	
	
	/////////////////////////////command access to NRF24L01
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
	extern INT8U L01_Rd_Fifo_Statu( void );
		
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
	extern void L01_Activate( void );
	
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
	extern void L01_Wr_Pl_In_Ack ( INT8U *Buff, INT8U Length );
	
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
	extern void L01_Wr_Pl_NoAck ( INT8U *Buff, INT8U Length );
	
	
	/****************************************************/
	/*
		Function Name:
			L01_Read_RX_Pload
		Input:
			Buff:	Address to store data read
		Output:
			Payload Width
		Description:
			read current pipe#'s RX payload
	*/
	extern INT8U L01_Rd_RX_Pload(INT8U *Buff);
	
	
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

