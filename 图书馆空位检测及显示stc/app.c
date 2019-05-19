#include "app.h"

#define DELAYTIME 100//5ms��ʱ����
#define RESTDELAYTIME 50//5ms��ʱ����
bit uart_get_flag=0;
_bool uartdelaysendflag=FLASE; 
u8 uart_cmd_data[2];
void uart_pack_send(void);

void main()
{
	UartInit();
	Timer0Init();
	while(1)
	{
		if(uart_get_flag==1)
		{
			uart_get_flag=0;
			FuncReg(&uartdelaysendflag,RESTDELAYTIME+DELAYTIME*(~(u8)Seat_Addr()),TRUE);
			
		}
		if(uartdelaysendflag==TRUE)
		{
			uartdelaysendflag=FLASE;
			uart_pack_send();	
		}
	}
}


//���ڽ�������
//֡ͷ 55
//֡ͷ AA
//addr xx
//seatdata xx
//seatdata xx
//���� 0-add8
//��β 0xFF

void uart_pack_send(void)
{
	u8 uart_pack_data[7] = {0};
	uart_pack_data[0] = 0X55;
	uart_pack_data[1] = 0XAA;
	uart_pack_data[2] = ~(u8)Seat_Addr();
	uart_pack_data[3] = ~(u8)Seat_Num();
	uart_pack_data[4] = (u8)Present_Seat_Num();
	uart_pack_data[5] = Check0_add8(uart_pack_data+2,3);
	uart_pack_data[6] = 0XFF;
	Send_String_Length(uart_pack_data,7);
}














