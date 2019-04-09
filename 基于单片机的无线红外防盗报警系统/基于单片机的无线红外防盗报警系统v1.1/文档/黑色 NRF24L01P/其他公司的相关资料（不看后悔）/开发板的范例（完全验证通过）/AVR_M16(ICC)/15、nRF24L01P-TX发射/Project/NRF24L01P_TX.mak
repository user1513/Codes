CC = iccavr
LIB = ilibw
CFLAGS =  -e -D__ICC_VERSION=722 -DATMega16  -l -g -MLongJump -MHasMul -MEnhanced 
ASFLAGS = $(CFLAGS) 
LFLAGS =  -O -g -e:0x4000 -ucrtatmega.o -bfunc_lit:0x54.0x4000 -dram_end:0x45f -bdata:0x60.0x45f -dhwstk_size:30 -beeprom:0.512 -fihx_coff -S2
FILES = nRF24L01.o main.o 

NRF24L01P_TX:	$(FILES)
	$(CC) -o NRF24L01P_TX $(LFLAGS) @NRF24L01P_TX.lk   -lcatmega
nRF24L01.o: .\..\..\..\..\..\YHTPRO~1\YHT02-M32EVB\Demo_V2\15、nRF24L01P-TX发射\Source\nRF24L01.h .\..\..\..\..\..\YHTPRO~1\YHT02-M32EVB\Demo_V2\15、nRF24L01P-TX发射\Source\MyTypedef.h C:\iccv7avr\include\stdio.h C:\iccv7avr\include\stdarg.h C:\iccv7avr\include\_const.h C:\iccv7avr\include\stdlib.h C:\iccv7avr\include\limits.h C:\iccv7avr\include\string.h C:\iccv7avr\include\math.h .\..\..\..\..\..\YHTPRO~1\YHT02-M32EVB\Demo_V2\15、nRF24L01P-TX发射\Source\nRF24L01_Reg.h C:\iccv7avr\include\iom16v.h .\..\..\..\..\..\YHTPRO~1\YHT02-M32EVB\Demo_V2\15、nRF24L01P-TX发射\Source\main.h .\..\..\..\..\..\YHTPRO~1\YHT02-M32EVB\Demo_V2\15、nRF24L01P-TX发射\Source\mytypedef.h
nRF24L01.o:	..\..\..\..\..\YHTPRO~1\YHT02-M32EVB\Demo_V2\15、nRF24L01P-TX发射\Source\nRF24L01.c
	$(CC) -c $(CFLAGS) ..\..\..\..\..\YHTPRO~1\YHT02-M32EVB\Demo_V2\15、nRF24L01P-TX发射\Source\nRF24L01.c
main.o: .\..\..\..\..\..\YHTPRO~1\YHT02-M32EVB\Demo_V2\15、nRF24L01P-TX发射\Source\main.h C:\iccv7avr\include\iom16v.h .\..\..\..\..\..\YHTPRO~1\YHT02-M32EVB\Demo_V2\15、nRF24L01P-TX发射\Source\mytypedef.h C:\iccv7avr\include\stdio.h C:\iccv7avr\include\stdarg.h C:\iccv7avr\include\_const.h C:\iccv7avr\include\stdlib.h C:\iccv7avr\include\limits.h C:\iccv7avr\include\string.h C:\iccv7avr\include\math.h .\..\..\..\..\..\YHTPRO~1\YHT02-M32EVB\Demo_V2\15、nRF24L01P-TX发射\Source\nRF24L01.h .\..\..\..\..\..\YHTPRO~1\YHT02-M32EVB\Demo_V2\15、nRF24L01P-TX发射\Source\MyTypedef.h .\..\..\..\..\..\YHTPRO~1\YHT02-M32EVB\Demo_V2\15、nRF24L01P-TX发射\Source\nRF24L01_Reg.h
main.o:	..\..\..\..\..\YHTPRO~1\YHT02-M32EVB\Demo_V2\15、nRF24L01P-TX发射\Source\main.c
	$(CC) -c $(CFLAGS) ..\..\..\..\..\YHTPRO~1\YHT02-M32EVB\Demo_V2\15、nRF24L01P-TX发射\Source\main.c
