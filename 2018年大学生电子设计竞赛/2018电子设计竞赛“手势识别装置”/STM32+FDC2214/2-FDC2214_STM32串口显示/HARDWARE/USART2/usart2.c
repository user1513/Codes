#include "usart2.h"
#include "main.h"

u8 UsartKey=0x00;
u8 UsartData[12];
/************************************************
 ����Zigbee��·�ƿ���ϵͳ
 ����ģ��
 ע�⣬���Ǵ���Usart2���� 
 //�޸�����:2018/4/6
 //�汾��V1.0
 ���ߣ�Ԭ�г�
************************************************/

void Usart2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitTypeState;
	USART_InitTypeDef USART_InitTypeState;
	NVIC_InitTypeDef NVIC_InitTypeState;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeState.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitTypeState.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitTypeState.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitTypeState);
	
	GPIO_InitTypeState.GPIO_Mode=GPIO_Mode_IN_FLOATING;;
	GPIO_InitTypeState.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitTypeState);
	
	USART_InitTypeState.USART_BaudRate=bound;
	USART_InitTypeState.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitTypeState.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitTypeState.USART_Parity=USART_Parity_No;
	USART_InitTypeState.USART_StopBits=USART_StopBits_1;
	USART_InitTypeState.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2,&USART_InitTypeState);
	
	NVIC_InitTypeState.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitTypeState.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitTypeState.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitTypeState.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitTypeState);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);
	
}

void Usart2_Send_Byte(u8 data)
{
	USART2->SR;
	USART2->DR=data;
	while((USART2->SR&0x40)==0);
	USART2->SR &=~0X40;
}

void Usart2_String_Send(u8 *data)
{
    while(*data!='\0')
        {
            Usart2_Send_Byte(*data);
            data++;
        }
}

void Usart2_String_Send1(u8 *data,u8 length)
{
    while(length--)
        {
            Usart2_Send_Byte(*data);
            data++;
        }
}


u8 Usart2_Num=0;
float datasendval=0;
void USART2_IRQHandler(void)                	//����1�жϷ������
{
	u8 Data=0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)
	{
		Data=USART_ReceiveData(USART2);
		switch(Usart2_Num)
		{
			case 0:if(Data==0xA5) {UsartData[Usart2_Num]=Data;Usart2_Num++;} else Usart2_Num=0;break;
			case 1:if(Data==0x5A) {UsartData[Usart2_Num]=Data;Usart2_Num++;} else Usart2_Num=0;break;
            case 2:if(Data==0x08) {UsartData[Usart2_Num]=Data;Usart2_Num++;} else Usart2_Num=0;break;
            case 3:if(Data==0x83) {UsartData[Usart2_Num]=Data;Usart2_Num++;} else Usart2_Num=0;break;
			case 4:if(Data==0x00||Data==0x02) {UsartData[Usart2_Num]=Data;Usart2_Num++;} else Usart2_Num=0;break;
            case 5:UsartData[Usart2_Num]=Data;
				switch(Data)
				{
					case 0X01:UsartKey=DECISIONMODEL; Usart2_Num++;break;
					case 0X02:UsartKey=LEARNMODEL;Usart2_Num++;break;
					case 0X03:UsartKey=LEARNMODEL;Usart2_Num++;break;
					case 0X04:UsartKey=JINGSAIMOS;Usart2_Num++;break;
					
					case 0x10:UsartKey=CHANNELDATA0;Usart2_Num++;break;
					case 0x12:UsartKey=CHANNELDATA1;Usart2_Num++;break;
					case 0x14:UsartKey=CHANNELDATA2;Usart2_Num++;break;
					case 0x16:UsartKey=CHANNELDATA3;Usart2_Num++;break;
					case 0x18:UsartKey=CHANNELDATA4;Usart2_Num++;break;
					case 0x1A:UsartKey=CHANNELDATA5;Usart2_Num++;break;
					default:Usart2_Num=0;UsartKey=0;break;
				}
				break;            
            case 6:if(Data==0x01) {UsartData[Usart2_Num]=Data;Usart2_Num++;} else Usart2_Num=0;break;
			case 7: {UsartData[Usart2_Num]=Data;Usart2_Num++;} break;
			case 8:UsartData[Usart2_Num]=Data;
				switch(UsartKey)
				{
					case DECISIONMODEL:
					{
						switch(Data)
						{
							case 0x01:TestKeyVal &=0X00;TestKeyVal |=0x01;Usart2_Num++;break;
							case 0x02:TestKeyVal &=0X00;TestKeyVal |=0x02;Usart2_Num++;break;
							default :Usart2_Num=0;break;
						}
					}
						break;
					case LEARNMODEL:
					{
						switch(Data)
						{
							case 0x01:LearnKeyVal |=0x01;Usart2_Num++;break;
							case 0x02:LearnKeyVal |=0x02;Usart2_Num++;break;
							case 0x03:LearnKeyVal |=0x04;Usart2_Num++;break;
							case 0x04:LearnKeyVal |=0x08;Usart2_Num++;break;
							case 0x05:LearnKeyVal |=0x10;Usart2_Num++;break;
							case 0x06:LearnKeyVal |=0x20;Usart2_Num++;break;
							case 0x07:LearnKeyVal |=0x40;Usart2_Num++;break;
							case 0x08:LearnKeyVal |=0x80;Usart2_Num++;break;
							default :Usart2_Num=0;break;
						}
					}
						break;
					case JINGSAIMOS :
					{
						switch(Data)
						{
							case 0x01:CompetitionKeyVal &=0X00;CompetitionKeyVal |=0x01;Usart2_Num++;break;
							case 0x02:CompetitionKeyVal &=0X00;CompetitionKeyVal |=0x02;Usart2_Num++;break;
						}
						break;
					}
					case CHANNELDATA0:
					{
						datasendval=((UsartData[7]<<4)|UsartData[8])/100.0;
						Parameter_Set[0]=datasendval;
						Usart2_Num++;break;
					}
					case CHANNELDATA1:
					{
						datasendval=((UsartData[7]<<4)|UsartData[8])/100.0;
						Parameter_Set[1]=datasendval;
						Usart2_Num++;break;
					}
					case CHANNELDATA2:
					{
						datasendval=((UsartData[7]<<4)|UsartData[8])/100.0;
						Parameter_Set[2]=datasendval;
						Usart2_Num++;break;
					}
					case CHANNELDATA3:
					{
						datasendval=((UsartData[7]<<4)|UsartData[8])/100.0;
						Parameter_Set[3]=datasendval;
						Usart2_Num++;break;
					}
					case CHANNELDATA4:
					{
						datasendval=((UsartData[7]<<4)|UsartData[8])/100.0;
						Parameter_Set[4]=datasendval;
						Usart2_Num++;break;
					}
					case CHANNELDATA5:
					{
						datasendval=((UsartData[7]<<4)|UsartData[8])/100.0;
						Parameter_Set[5]=datasendval;
						Usart2_Num++;break;
					}
					default:break;
				}
				break;

            case 9:if(Data==(u8)ModBus_CRC(UsartData+3,6)) {UsartData[Usart2_Num]=Data;Usart2_Num++;} else Usart2_Num=0;break;            
            case 10:
				if(Data==(u8)(ModBus_CRC(UsartData+3,6)>>8)) 
					{UsartData[Usart2_Num]=Data;
					if(UsartKey==DECISIONMODEL) {UsartData[11]=0xAA; Usart2_Num=0;}
					else if(UsartKey==JINGSAIMOS){UsartData[11]=0xBB; Usart2_Num=0;}
					else Usart2_Num=0;break;
				}
            default: Usart2_Num=0;
		}
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}





















