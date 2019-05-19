#include "_usart_send.h"


//��MCU�з���ҳ���л�ͨ��ModbusCRC16����
void Send_Page_Switch(u8 addr ,u16 Pageaddr)
{
    u8 Data_able[9];
    u16 Check_val=0;
	 Data_able[0]=0xa5;
	 Data_able[1]=0x5a;     
	 Data_able[2]=0x06;  
	 Data_able[3]=0x80;  
	 Data_able[4]=addr;  
	 Data_able[5]=Pageaddr>>8;  
	 Data_able[6]=Pageaddr;  
	    
     Check_val=ModBus_CRC(Data_able+3,4);     
	 Data_able[8]=Check_val>>8;  
	 Data_able[7]=Check_val;   
     Usart2_String_Send1(Data_able,9);
}


//��MCU�з������ݣ�ascll����������ͨ��ModbusCRC16����
void Send_Number_Data(u16 addr,u16 num)
{
    u8 Data_able[10];
    u16 Check_val=0;
	 Data_able[0]=0xa5;
	 Data_able[1]=0x5a;     
	 Data_able[2]=0x07;  
	 Data_able[3]=0x82;  
	 Data_able[4]=addr>>8;  
	 Data_able[5]=addr;  
	 Data_able[6]=num>>8;  
	 Data_able[7]=num;   
     Check_val=ModBus_CRC(Data_able+3,5);     
	 Data_able[9]=Check_val>>8;  
	 Data_able[8]=Check_val;   
     Usart2_String_Send1(Data_able,10);
}

void Send_Number_Data_u32 (u16 addr,u32 num)
{
    u8 Data_able[12];
    u16 Check_val=0;
	 Data_able[0]=0xa5;
	 Data_able[1]=0x5a;     
	 Data_able[2]=0x09;  
	 Data_able[3]=0x82;  
	 Data_able[4]=addr>>8;  
	 Data_able[5]=addr; 
	 Data_able[6]=num>>24;	
	 Data_able[7]=num>>16;
	 Data_able[8]=num>>8;  
	 Data_able[9]=num;   
     Check_val=ModBus_CRC(Data_able+3,7);     
	 Data_able[11]=Check_val>>8;  
	 Data_able[10]=Check_val;   
     Usart2_String_Send1(Data_able,12);
}


//��MCU�з������ݣ����֣���������ͨ��ModbusCRC16����
void Send_China_Data(u16 addr,u8 *data)
{
    u8 num=0,i=0;
    u8 Data_able[25];
    u8 Data_able1[12];//��෢��6������
    u16 Check_val=0;
    while(*data!='\0')
    {
       
       Data_able1[num]=*data;//0x02;//*(data++);
       num++; 
       data++;
    }

	 Data_able[0]=0xa5;
	 Data_able[1]=0x5a;     
	 Data_able[2]=0x07+num;  
	 Data_able[3]=0x82;  
	 Data_able[4]=addr>>8;  
	 Data_able[5]=addr;  
     while(num>=i)
     {        
        Data_able[6+i]=Data_able1[i];   
        i++;
     }
	 Data_able[num+6]=0xFF;
	 Data_able[num+7]=0xFF;
     Check_val=ModBus_CRC(Data_able+3,0x05+num);     
	 Data_able[num+9]=Check_val>>8;  
	 Data_able[num+8]=Check_val;   
     Usart2_String_Send1(Data_able,10+num);
    
}


////��MCU�з������ݣ����֣���������ͨ��ModbusCRC16����
//void Send_China_Data(u16 addr,u8 *data)
//{
//    u8 num=0,i=0;
//    u8 Data_able[20];
//    u8 Data_able1[12];//��෢��6������
//    u16 Check_val=0;
//    while(*data!='\0')
//    {
//       
//       Data_able1[num]=*data;//0x02;//*(data++);
//       num++; 
//       data++;
//    }

//	 Data_able[0]=0xa5;
//	 Data_able[1]=0x5a;     
//	 Data_able[2]=0x05+num;  
//	 Data_able[3]=0x82;  
//	 Data_able[4]=addr>>8;  
//	 Data_able[5]=addr;  
//     while(num>=i)
//     {        
//        Data_able[6+i]=Data_able1[i];   
//        i++;
//     }
//     Check_val=ModBus_CRC(Data_able+3,0x03+num);     
//	 Data_able[num+7]=Check_val>>8;  
//	 Data_able[num+6]=Check_val;   
//     Usart2_String_Send1(Data_able,8+num);
//    
//}

void SendListInit(u8 _num,u16 _firstaddr1,u16 _firstaddr2,u16 _firstaddr3)//����l�Ͷ��ٽM����
{
	while(_num--)
	{
		Send_China_Data(_firstaddr1,"��");
		_firstaddr1 +=2;
		delay_ms(10);
		Send_China_Data(_firstaddr2,"��");
		_firstaddr2 +=2;
		delay_ms(10);
		Send_China_Data(_firstaddr3,"��");
		_firstaddr3 +=2;
		delay_ms(10);
	}
}


void SendList(u8 _seataddr,u8 * _seatmaxpeople,u8 * _seatpeople,u8 *_Surplus)//�l���б픵��
{
	delay_ms(5);
	Send_China_Data(0x0100+_seataddr*2,_seatmaxpeople);
	delay_ms(5);
	Send_China_Data(0x0200+_seataddr*2,_seatpeople);
	delay_ms(5);
	Send_China_Data(0x0300+_seataddr*2,_Surplus);
	delay_ms(5);
	free(_seatmaxpeople);
	free(_seatpeople);
	free(_Surplus);
}

void SendListT(u8 _seataddr,u8 * _seatmaxpeople,u8 * _seatpeople,u8 *_Surplus)//�l���б픵��
{
	delay_ms(5);
	Send_China_Data(0x0100+_seataddr*2,_seatmaxpeople);
	delay_ms(5);
	Send_China_Data(0x0200+_seataddr*2,_seatpeople);
	delay_ms(5);
	Send_China_Data(0x0300+_seataddr*2,_Surplus);
	delay_ms(5);
}


void LightSet(u8 _lightval)//���ƴ���������
{
	 u8 Data_able[8];
	 u16 Check_val=0;
	 Data_able[0]=0xa5;
	 Data_able[1]=0x5a;     
	 Data_able[2]=0x05;  
	 Data_able[3]=0x80;  
	 Data_able[4]=0x01;  
	 Data_able[5]=_lightval; 
	 Check_val=ModBus_CRC(Data_able+3,3);     
	 Data_able[7]=Check_val>>8;  
	 Data_able[6]=Check_val;   
     Usart2_String_Send1(Data_able,8);
}











