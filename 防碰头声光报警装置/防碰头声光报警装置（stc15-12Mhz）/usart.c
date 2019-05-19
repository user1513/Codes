#include "usart.h"

BYTE TBUF,RBUF;
BYTE TDAT,RDAT;
BYTE TCNT,RCNT;
BYTE TBIT,RBIT;
BOOL TING,RING;
BOOL TEND,REND;

void Usart_Init(void)
{
    TMOD = 0x00;                        //timer0 in 16-bit auto reload mode
    AUXR = 0x80;                        //timer0 working at 1T mode
    TL0 = BAUD;
    TH0 = BAUD>>8;                      //initial timer0 and set reload value
    TR0 = 1;                            //tiemr0 start running
    ET0 = 1;                            //enable timer0 interrupt
    PT0 = 1;                            //improve timer0 interrupt priority
    EA = 1;                             //open global interrupt switch

    UART_INIT();
/*
    while (1)
    {                                   //user's function
        if (REND)
        {
            REND = 0;
            buf[r++ & 0x0f] = RBUF;
        }
        if (TEND)
        {
            if (t != r)
            {
                TEND = 0;
                TBUF = buf[t++ & 0x0f];
                TING = 1;
            }
        }
    }
*/
}

void UserSendString(BYTE *buf,BYTE Length)
{
	int i=0;
		while (i != Length)
		{
			UserSendChar(*(buf++));
			i++;
			/*
			if (TEND)
			{
				TEND = 0;
				TBUF = *(buf++);
				TING = 1;
				i++;
			}
			*/
		}
}

void UserSendChar(char Data)
{
	while(!TEND);
	TEND = 0;
	TBUF = Data;
	TING = 1;
}


void UserSendString1(char *str )
{
	while(*str !='\0')
	{
		UserSendChar(*(str++));
	}
}

//-----------------------------------------
//Timer interrupt routine for UART

void tm0() interrupt 1 using 1
{
    if (RING)
    {
        if (--RCNT == 0)
        {
            RCNT = 3;                   //reset send baudrate counter
            if (--RBIT == 0)
            {
                RBUF = RDAT;            //save the data to RBUF
                RING = 0;               //stop receive
                REND = 1;               //set receive completed flag
            }
            else
            {
                RDAT >>= 1;
                if (RXB) RDAT |= 0x80;  //shift RX data to RX buffer
            }
        }
    }
    else if (!RXB)
    {
        RING = 1;                       //set start receive flag
        RCNT = 4;                       //initial receive baudrate counter
        RBIT = 9;                       //initial receive bit number (8 data bits + 1 stop bit)
    }

    if (--TCNT == 0)
    {
        TCNT = 3;                       //reset send baudrate counter
        if (TING)                       //judge whether sending
        {
            if (TBIT == 0)
            {
                TXB = 0;                //send start bit
                TDAT = TBUF;            //load data from TBUF to TDAT
                TBIT = 9;               //initial send bit number (8 data bits + 1 stop bit)
            }
            else
            {
                TDAT >>= 1;             //shift data to CY
                if (--TBIT == 0)
                {
                    TXB = 1;
                    TING = 0;           //stop send
                    TEND = 1;           //set send completed flag
                }
                else
                {
                    TXB = CY;           //write CY to TX port
                }
            }
        }
    }
}

//-----------------------------------------
//initial UART module variable

void UART_INIT(void)
{
    TING = 0;
    RING = 0;
    TEND = 1;
    REND = 0;
    TCNT = 0;
    RCNT = 0;
}

