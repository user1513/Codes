NOTE: AVR Code addresses are word addresses
All other addresses (including ones in FLASH) are byte addresses

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                           idata   0054   003E =     62. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0054  __idata_start
       0092  __idata_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   0092   04F6 =   1270. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0049  __start
       0049  __text_start
       006A  _exit
       006B  _USART_SendChar
       0070  _USART_SendArray
       007C  _USART_SendStr
       008B  _USART_Init
       0096  _SPI_Init
       00A0  _SPI_ExchangeByte
       00A5  _main
       00F2  _L01_ReadSingleReg
       00FF  _L01_ReadMultiReg
       0117  _L01_WriteSingleReg
       0125  _L01_WriteMultiReg
       0137  _L01_FlushTX
       013E  _L01_FlushRX
       0145  _L01_ReuseTXPayload
       014C  _L01_Nop
       0153  _L01_ReadStatusReg
       015D  _L01_ClearIRQ
       0170  _L01_ReadIRQSource
       0173  _L01_ReadTopFIFOWidth
       017F  _L01_ReadRXPayload
       019E  _L01_WriteTXPayload_Ack
       01B1  _L01_WriteTXPayload_NoAck
       01CB  _L01_WriteRXPayload_InAck
       01DD  _L01_SetTXAddr
       01E9  _L01_SetRXAddr
       01FF  _L01_WriteHoppingPoint
       0207  _L01_SetTXMode
       0211  _L01_SetRXMode
       021B  _L01_Init
       0253  pop_xgset00FC
       0256  pop_xgset30FC
       025B  push_xgsetF0FC
       025D  push_xgset30FC
       025F  push_xgset00FC
       0261  push_xgset003C
       0266  push_xgset300C
       026B  pop_xgset300C
       0270  push_xgset303C
       0277  pop_xgset303C
       027E  asgnblk
       0296  <created procedures>
       02C4  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            data   0060   003E =     62. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0060  __data_start
       009E  __data_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             bss   009E   0020 =     32. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       009E  __bss_start
       009E  _testbuffer
       00BE  __bss_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          vector   0000   0004 =      4. bytes (abs,ovr,rom)

Files Linked      [ module(s) ]

C:\iccv7avr\lib\crtatmega.o	[ crtatmega.s ]
main.o	[ main.c ]
nRF24L01.o	[ nRF24L01.c ]
<library>	[ gpop3.s, gpop4.s, gpush.s, gpushx.s, gpusha.s, struct.s ]

User Global Definitions

ram_end = 0x45f
hwstk_size = 0x1e

User Base Address Definitions

func_lit = 0x54
data = 0x60
eeprom:0.512

