NOTE: AVR Code addresses are word addresses
All other addresses (including ones in FLASH) are byte addresses

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                           idata   0054   005A =     90. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0054  __idata_start
       00AE  __idata_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   00AE   0548 =   1352. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0057  __start
       0057  __text_start
       0078  _exit
       0079  _L01_ReadSingleReg
       0086  _L01_ReadMultiReg
       009E  _L01_WriteSingleReg
       00AC  _L01_WriteMultiReg
       00BE  _L01_FlushTX
       00C5  _L01_FlushRX
       00CC  _L01_ReuseTXPayload
       00D3  _L01_Nop
       00DA  _L01_ReadStatusReg
       00E4  _L01_ClearIRQ
       00F7  _L01_ReadIRQSource
       00FA  _L01_ReadTopFIFOWidth
       0106  _L01_ReadRXPayload
       0125  _L01_WriteTXPayload_Ack
       0138  _L01_WriteTXPayload_NoAck
       0152  _L01_WriteRXPayload_InAck
       0164  _L01_SetTXAddr
       0170  _L01_SetRXAddr
       0186  _L01_WriteHoppingPoint
       018E  _L01_SetTXMode
       0198  _L01_SetRXMode
       01A2  _L01_Init
       01DA  _USART_SendChar
       01DF  _USART_SendArray
       01EB  _USART_SendStr
       01FA  _USART_Init
       0205  _SPI_Init
       020F  _SPI_ExchangeByte
       0214  _main
       028A  pop_xgset00FC
       028D  pop_xgset30FC
       0292  push_xgsetF0FC
       0294  push_xgset30FC
       0296  push_xgset00FC
       0298  push_xgset003C
       029D  push_xgset300C
       02A2  pop_xgset300C
       02A7  push_xgset303C
       02AE  pop_xgset303C
       02B5  asgnblk
       02CD  <created procedures>
       02FB  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            data   0060   005A =     90. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0060  __data_start
       00BA  __data_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             bss   00BA   0020 =     32. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       00BA  __bss_start
       00BA  _testbuffer
       00DA  __bss_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          vector   0000   0004 =      4. bytes (abs,ovr,rom)

Files Linked      [ module(s) ]

C:\iccv7avr\lib\crtatmega.o	[ crtatmega.s ]
nRF24L01.o	[ nRF24L01.c ]
main.o	[ main.c ]
<library>	[ gpop3.s, gpop4.s, gpush.s, gpushx.s, gpusha.s, struct.s ]

User Global Definitions

ram_end = 0x45f
hwstk_size = 0x1e

User Base Address Definitions

func_lit = 0x54
data = 0x60
eeprom:0.512

