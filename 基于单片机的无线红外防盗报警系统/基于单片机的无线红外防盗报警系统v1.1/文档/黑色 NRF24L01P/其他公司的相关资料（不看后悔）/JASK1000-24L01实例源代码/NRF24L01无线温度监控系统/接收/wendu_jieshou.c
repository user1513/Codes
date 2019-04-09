#include <reg52.h>
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned char uint;
//****************************************NRF24L01¶Ë¿Ú¶¨Òå***************************************
sbit 	MISO	=P1^3;
sbit 	MOSI	=P1^4;
sbit	SCK	    =P1^2;
sbit	CE	    =P1^1;
sbit	CSN		=P3^2;
sbit	IRQ		=P3^3;
//************************************°´¼ü***************************************************
sbit	KEY1=P3^6;
sbit	KEY2=P3^7;
//************************************ÊıÂë¹ÜÎ»Ñ¡*********************************************
sbit	led3=P2^0;
sbit	led2=P2^1;
sbit	led1=P2^2;
sbit	led0=P2^3;
//************************************·äÃ÷Æ÷***************************************************
sbit 	BELL=P3^4;
//***********************************ÊıÂë¹Ü0-9±àÂë*******************************************
uchar seg[10]={0xC0,0xCF,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};         //0~~9¶ÎÂë
uchar seg1[10]={0x40,0x4F,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};         //0~~9¶ÎÂë
char temp[6];
uchar RxBuf[20]={0};
//*********************************************NRF24L01*************************************
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  20  	// 20 uints TX payload
#define RX_PLOAD_WIDTH  20  	// 20 uints TX payload
uint const TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//±¾µØµØÖ·
uint const RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//½ÓÊÕµØÖ·
//***************************************NRF24L01¼Ä´æÆ÷Ö¸Áî*******************************************************
#define READ_REG        0x00  	// ¶Á¼Ä´æÆ÷Ö¸Áî
#define WRITE_REG       0x20 	// Ğ´¼Ä´æÆ÷Ö¸Áî
#define RD_RX_PLOAD     0x61  	// ¶ÁÈ¡½ÓÊÕÊı¾İÖ¸Áî
#define WR_TX_PLOAD     0xA0  	// Ğ´´ı·¢Êı¾İÖ¸Áî
#define FLUSH_TX        0xE1 	// ³åÏ´·¢ËÍ FIFOÖ¸Áî
#define FLUSH_RX        0xE2  	// ³åÏ´½ÓÊÕ FIFOÖ¸Áî
#define REUSE_TX_PL     0xE3  	// ¶¨ÒåÖØ¸´×°ÔØÊı¾İÖ¸Áî
#define NOP             0xFF  	// ±£Áô
//*************************************SPI(nRF24L01)¼Ä´æÆ÷µØÖ·****************************************************
#define CONFIG          0x00  // ÅäÖÃÊÕ·¢×´Ì¬£¬CRCĞ£ÑéÄ£Ê½ÒÔ¼°ÊÕ·¢×´Ì¬ÏìÓ¦·½Ê½
#define EN_AA           0x01  // ×Ô¶¯Ó¦´ğ¹¦ÄÜÉèÖÃ
#define EN_RXADDR       0x02  // ¿ÉÓÃĞÅµÀÉèÖÃ
#define SETUP_AW        0x03  // ÊÕ·¢µØÖ·¿í¶ÈÉèÖÃ
#define SETUP_RETR      0x04  // ×Ô¶¯ÖØ·¢¹¦ÄÜÉèÖÃ
#define RF_CH           0x05  // ¹¤×÷ÆµÂÊÉèÖÃ
#define RF_SETUP        0x06  // ·¢ÉäËÙÂÊ¡¢¹¦ºÄ¹¦ÄÜÉèÖÃ
#define STATUS          0x07  // ×´Ì¬¼Ä´æÆ÷
#define OBSERVE_TX      0x08  // ·¢ËÍ¼à²â¹¦ÄÜ
#define CD              0x09  // µØÖ·¼ì²â           
#define RX_ADDR_P0      0x0A  // ÆµµÀ0½ÓÊÕÊı¾İµØÖ·
#define RX_ADDR_P1      0x0B  // ÆµµÀ1½ÓÊÕÊı¾İµØÖ·
#define RX_ADDR_P2      0x0C  // ÆµµÀ2½ÓÊÕÊı¾İµØÖ·
#define RX_ADDR_P3      0x0D  // ÆµµÀ3½ÓÊÕÊı¾İµØÖ·
#define RX_ADDR_P4      0x0E  // ÆµµÀ4½ÓÊÕÊı¾İµØÖ·
#define RX_ADDR_P5      0x0F  // ÆµµÀ5½ÓÊÕÊı¾İµØÖ·
#define TX_ADDR         0x10  // ·¢ËÍµØÖ·¼Ä´æÆ÷
#define RX_PW_P0        0x11  // ½ÓÊÕÆµµÀ0½ÓÊÕÊı¾İ³¤¶È
#define RX_PW_P1        0x12  // ½ÓÊÕÆµµÀ0½ÓÊÕÊı¾İ³¤¶È
#define RX_PW_P2        0x13  // ½ÓÊÕÆµµÀ0½ÓÊÕÊı¾İ³¤¶È
#define RX_PW_P3        0x14  // ½ÓÊÕÆµµÀ0½ÓÊÕÊı¾İ³¤¶È
#define RX_PW_P4        0x15  // ½ÓÊÕÆµµÀ0½ÓÊÕÊı¾İ³¤¶È
#define RX_PW_P5        0x16  // ½ÓÊÕÆµµÀ0½ÓÊÕÊı¾İ³¤¶È
#define FIFO_STATUS     0x17  // FIFOÕ»ÈëÕ»³ö×´Ì¬¼Ä´æÆ÷ÉèÖÃ
//**************************************************************************************
void Delay(unsigned int s);
void inerDelay_us(unsigned char n);
void init_NRF24L01(void);
uint SPI_RW(uint uchar);
uchar SPI_Read(uchar reg);
void SetRX_Mode(void);
uint SPI_RW_Reg(uchar reg, uchar value);
uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars);
uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars);
unsigned char nRF24L01_RxPacket(unsigned char* rx_buf);
void nRF24L01_TxPacket(unsigned char * tx_buf);
//*****************************************³¤ÑÓÊ±*****************************************
void Delay(unsigned int s)
{
	unsigned int i;
	for(i=0; i<s; i++);
	for(i=0; i<s; i++);
}
//******************************************************************************************
uint 	bdata sta;   //×´Ì¬±êÖ¾
sbit	RX_DR	=sta^6;
sbit	TX_DS	=sta^5;
sbit	MAX_RT	=sta^4;
/******************************************************************************************
/*ÑÓÊ±º¯Êı
/******************************************************************************************/
void inerDelay_us(unsigned char n)
{
	for(;n>0;n--)
		_nop_();
}
//****************************************************************************************
/*NRF24L01³õÊ¼»¯
//***************************************************************************************/
void init_NRF24L01(void)
{
    inerDelay_us(100);
 	CE=0;    // chip enable
 	CSN=1;   // Spi disable 
 	SCK=0;   // Spi clock line init high
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // Ğ´±¾µØµØÖ·	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // Ğ´½ÓÊÕ¶ËµØÖ·
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      						 //  ÆµµÀ0×Ô¶¯	ACKÓ¦´ğÔÊĞí	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  ÔÊĞí½ÓÊÕµØÖ·Ö»ÓĞÆµµÀ0£¬Èç¹ûĞèÒª¶àÆµµÀ£¬¿ÉÒÔ²Î¿¼Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //   ÉèÖÃĞÅµÀ¹¤×÷Îª2.4GHZ£¬ÊÕ·¢±ØĞëÒ»ÖÂ
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //ÉèÖÃ½ÓÊÕÊı¾İ³¤¶È£¬±¾´ÎÉèÖÃÎª32Ö½Ú
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   		//ÉèÖÃ·¢ÉäËÙÂÊÎª1MHZ£¬·¢Éä¹¦ÂÊÎª×î´óÖµ0dB
}
/****************************************************************************************************
/*º¯Êı£ºuint SPI_RW(uint uchar)
/*¹¦ÄÜ£ºNRF24L01µÄSPIĞ´Ê±Ğò
/*ÏêÏ¸Çë²Î¿¼Page19 
/****************************************************************************************************/
uint SPI_RW(uint uchar)
{
	uint bit_ctr;
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
   	{
		MOSI = (uchar & 0x80);         // output 'uchar', MSB to MOSI
		uchar = (uchar << 1);           // shift next bit into MSB..
		SCK = 1;                      // Set SCK high..
		uchar |= MISO;       		  // capture current MISO bit
		SCK = 0;            		  // ..then set SCK low again
   	}
    return(uchar);           		  // return read uchar
}
/****************************************************************************************************
/*º¯Êı£ºuchar SPI_Read(uchar reg)
/*¹¦ÄÜ£ºNRF24L01µÄSPIÊ±Ğò
/*ÏêÏ¸Çë²Î¿¼Page19 
/****************************************************************************************************/
uchar SPI_Read(uchar reg)
{
	uchar reg_val;
	
	CSN = 0;                // CSN low, initialize SPI communication...
	SPI_RW(reg);            // Select register to read from..
	reg_val = SPI_RW(0);    // ..then read registervalue
	CSN = 1;                // CSN high, terminate SPI communication
	
	return(reg_val);        // return register value
}
/****************************************************************************************************/
/*¹¦ÄÜ£ºNRF24L01¶ÁĞ´¼Ä´æÆ÷º¯Êı
/****************************************************************************************************/
uint SPI_RW_Reg(uchar reg, uchar value)
{
	uint status;
	
	CSN = 0;                   // CSN low, init SPI transaction
	status = SPI_RW(reg);      // select register
	SPI_RW(value);             // ..and write value to it..
	CSN = 1;                   // CSN high again
	
	return(status);            // return nRF24L01 status uchar
}
/****************************************************************************************************/
/*º¯Êı£ºuint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
/*¹¦ÄÜ: ÓÃÓÚ¶ÁÊı¾İ£¬reg£ºÎª¼Ä´æÆ÷µØÖ·£¬pBuf£ºÎª´ı¶Á³öÊı¾İµØÖ·£¬uchars£º¶Á³öÊı¾İµÄ¸öÊı
/****************************************************************************************************/
uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uint status,uchar_ctr;
	
	CSN = 0;                    		// Set CSN low, init SPI tranaction
	status = SPI_RW(reg);       		// Select register to write to and read status uchar
	
	for(uchar_ctr=0;uchar_ctr<uchars;uchar_ctr++)
		pBuf[uchar_ctr] = SPI_RW(0);    // 
	
	CSN = 1;                           
	
	return(status);                    // return nRF24L01 status uchar
}
/*********************************************************************************************************
/*º¯Êı£ºuint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
/*¹¦ÄÜ: ÓÃÓÚĞ´Êı¾İ£ºÎª¼Ä´æÆ÷µØÖ·£¬pBuf£ºÎª´ıĞ´ÈëÊı¾İµØÖ·£¬uchars£ºĞ´ÈëÊı¾İµÄ¸öÊı
/*********************************************************************************************************/
uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uint status,uchar_ctr;
	
	CSN = 0;            //SPIÊ¹ÄÜ       
	status = SPI_RW(reg);   
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) //
		SPI_RW(*pBuf++);
	CSN = 1;           //¹Ø±ÕSPI
	return(status);    // 
}
/****************************************************************************************************/
/*º¯Êı£ºvoid SetRX_Mode(void)
/*¹¦ÄÜ£ºÊı¾İ½ÓÊÕÅäÖÃ 
/****************************************************************************************************/
void SetRX_Mode(void)
{
	CE=0;
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);   		// IRQÊÕ·¢Íê³ÉÖĞ¶ÏÏìÓ¦£¬16Î»CRC	£¬Ö÷½ÓÊÕ
	CE = 1; 
	inerDelay_us(130);
}
/******************************************************************************************************/
/*º¯Êı£ºunsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
/*¹¦ÄÜ£ºÊı¾İ¶ÁÈ¡ºó·ÅÈçrx_buf½ÓÊÕ»º³åÇøÖĞ
/******************************************************************************************************/
unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
{
    unsigned char revale=0;
	sta=SPI_Read(STATUS);	// ¶ÁÈ¡×´Ì¬¼Ä´æÆäÀ´ÅĞ¶ÏÊı¾İ½ÓÊÕ×´¿ö
	if(RX_DR)				// ÅĞ¶ÏÊÇ·ñ½ÓÊÕµ½Êı¾İ
	{
	    CE = 0; 			
	SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		revale =1;			//¶ÁÈ¡Êı¾İÍê³É±êÖ¾
	}
	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //½ÓÊÕµ½Êı¾İºóRX_DR,TX_DS,MAX_PT¶¼ÖÃ¸ßÎª1£¬Í¨¹ıĞ´1À´ÆäÇå³ıÖĞ¶Ï±êÖ¾
	return revale;
}
/***********************************************************************************************************
/*º¯Êı£ºvoid nRF24L01_TxPacket(unsigned char * tx_buf)
/*¹¦ÄÜ£º·¢ËÍ tx_bufÖĞÊı¾İ
/**********************************************************************************************************/
void nRF24L01_TxPacket(unsigned char * tx_buf)
{
	CE=0;			//StandBy IÄ£Ê½	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // ×°ÔØ½ÓÊÕ¶ËµØÖ·
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // ×°ÔØÊı¾İ	
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQÊÕ·¢Íê³ÉÖĞ¶ÏÏìÓ¦£¬16Î»CRC£¬Ö÷·¢ËÍ
	CE=1;		 //ÖÃ¸ßCE£¬¼¤·¢Êı¾İ·¢ËÍ
	inerDelay_us(10);
}
//***********************************************´®¿Ú³õÊ¼»¯***********************************************
void StartUART( void )	
{  						
     SCON = 0x50;
     TMOD = 0x20;
     TH1 = 0xFA;
     TL1 = 0xFA;			//²¨ÌØÂÊ4800
     PCON = 0x00;
     TR1 = 1;
}
//***********************************************Í¨¹ı´®¿ÚÏòPC·¢ËÍ***********************************************
void R_S_Byte(uchar R_Byte)
{	
	 SBUF = R_Byte;  
     while( TI == 0 );				//²éÑ¯·¨
  	 TI = 0;    
}
//***********************************************ÊıÂë¶¯Ì¬É¨ÃèÏÔÊ¾***********************************************
void disdignit()
{
	P0=0xC6;			  //C
	led0=0;
	Delay(50);
	led0=1;
	P0=seg[temp[1]];	  //Ğ¡ÊıÎ»
	led1=0;
	Delay(50);
	led1=1;
	P0=seg1[temp[5]];    //Ê®Î»
	led2=0;
	Delay(50);
	led2=1;
	P0=seg[temp[4]];  	 //¸öÎ»
	led3=0;
	Delay(50);
	led3=1;
}
//************************************Ö÷º¯Êı************************************************************
void main(void)
{
	uchar i=0;	
    init_NRF24L01() ;			//NRF24L01³õÊ¼»¯
	StartUART();
	Delay(6000);
	BELL=1;
	while(1)
	{
//***********************************************************************************************
		SetRX_Mode();
   		if(nRF24L01_RxPacket(RxBuf))
		{
			temp[0]=RxBuf[3];     					 	//·ûºÅÎ»
			temp[2]=((RxBuf[2]<<4)|RxBuf[1]);			//ÕûÊıÎ»
			temp[1]=RxBuf[0];							//Ğ¡ÊıÎ»
		    temp[4]=RxBuf[2];							//Ê®Î»
	 	    temp[5]=RxBuf[1];							//¸öÎ»
			disdignit();		//ÊıÂë¹ÜÏÔÊ¾ÎÂ¶È
			R_S_Byte('t');     //´®¿ÚÏÔÊ¾ÎÂ¶È
			disdignit();		//ÊıÂë¹ÜÏÔÊ¾ÎÂ¶È
			disdignit();		//ÊıÂë¹ÜÏÔÊ¾ÎÂ¶È
			R_S_Byte(0x30+temp[4]);     //´®¿ÚÏÔÊ¾ÎÂ¶È
			R_S_Byte(0x30+temp[5]);     //´®¿ÚÏÔÊ¾ÎÂ¶È
			R_S_Byte('.');     //´®¿ÚÏÔÊ¾ÎÂ¶È
			R_S_Byte(0x30+temp[1]);     //´®¿ÚÏÔÊ¾ÎÂ¶È
			disdignit();		//ÊıÂë¹ÜÏÔÊ¾ÎÂ¶È
			disdignit();		//ÊıÂë¹ÜÏÔÊ¾ÎÂ¶È
			disdignit();		//ÊıÂë¹ÜÏÔÊ¾ÎÂ¶È
		/*	for(i=0;i<3;i++)
			{
			R_S_Byte(temp[2-i]);     //´®¿ÚÏÔÊ¾ÎÂ¶È
			Delay(60);
			} */
		}
			disdignit();		//ÊıÂë¹ÜÏÔÊ¾ÎÂ¶È
			
		if(temp[2]>=0x30)			//´óÓÚ30¶ÈÊ±±¨¾¯£¬0x30×ª»»³É10½øÖÆÎª48
		{
			BELL=0;   			//´ò¿ª·äÃ÷Æ÷
		//	Delay(1);
		}
		else
		{
			BELL=1;  		//¹Ø±Õ·äÃ÷Æ÷
	}	

		
		
		}
}
