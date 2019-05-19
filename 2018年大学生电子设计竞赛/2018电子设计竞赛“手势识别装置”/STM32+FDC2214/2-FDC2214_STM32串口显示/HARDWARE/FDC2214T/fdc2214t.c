#include "fdc2214t.h"
#include "delay.h"
#include "led.h"

u32 Data_FDCt;
//FDC_I2C����д����**************************************************

//FDC IIC ��ʱ����
void FDCt_IIC_Delay(void)
{
	delay_us(2);
}

void FDCt_GPIO_Init(void)
{
	
}

//��ʼ��IIC I/O��
void FDCt_IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14); 	//PB6,PB7 �����
}

//����IIC��ʼ�ź�
void FDCt_IIC_Start(void)
{
	FDCt_SDA_OUT();     //sda�����
	FDCt_IIC_SDA=1;	  	  
	FDCt_IIC_SCL=1;
	FDCt_IIC_Delay();
 	FDCt_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	FDCt_IIC_Delay();
	FDCt_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void FDCt_IIC_Stop(void)
{
	FDCt_SDA_OUT();//sda�����
	FDCt_IIC_SCL=0;
	FDCt_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	FDCt_IIC_Delay();
	FDCt_IIC_SCL=1; 
	FDCt_IIC_SDA=1;//����I2C���߽����ź�
	FDCt_IIC_Delay();							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 FDCt_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	FDCt_SDA_IN();      //SDA����Ϊ����  
	FDCt_IIC_SDA=1;FDCt_IIC_Delay();	   
	FDCt_IIC_SCL=1;FDCt_IIC_Delay();	 
	while(FDCt_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			FDCt_IIC_Stop();
			return 1;
		}
	}
	FDCt_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
}
//����ACKӦ��
void FDCt_IIC_Ack(void)
{
	FDCt_IIC_SCL=0;
	FDCt_SDA_OUT();
	FDCt_IIC_SDA=0;
	FDCt_IIC_Delay();
	FDCt_IIC_SCL=1;
	FDCt_IIC_Delay();
	FDCt_IIC_SCL=0;
}
//������ACKӦ��		    
void FDCt_IIC_NAck(void)
{
	FDCt_IIC_SCL=0;
	FDCt_SDA_OUT();
	FDCt_IIC_SDA=1;
	FDCt_IIC_Delay();
	FDCt_IIC_SCL=1;
	FDCt_IIC_Delay();
	FDCt_IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void FDCt_IIC_Send_Byte(u8 txd)
{                        
  u8 t;   
	FDCt_SDA_OUT(); 	    
    FDCt_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        FDCt_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		    FDCt_IIC_SCL=1;
		    FDCt_IIC_Delay(); 
		    FDCt_IIC_SCL=0;	
		    FDCt_IIC_Delay();
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 FDCt_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	FDCt_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        FDCt_IIC_SCL=0; 
        FDCt_IIC_Delay();
		FDCt_IIC_SCL=1;
        receive<<=1;
        if(FDCt_READ_SDA)receive++;   
		FDCt_IIC_Delay(); 
    }				 
    if (!ack)
        FDCt_IIC_NAck();//����nACK
    else
        FDCt_IIC_Ack(); //����ACK   
    return receive;
}

//FDC�Ĵ�������***********************************************************************

/*FDC2214���ú���
 *IICд2���ֽ� 
 *reg:�Ĵ�����ַ
 *data1:����1
 *data2:����2
 *����ֵ:0    ����
 *     ����  �������
*/
u8 Sett_FDC2214(u8 reg,u8 MSB,u8 LSB) 				 
{ 
    FDCt_IIC_Start(); 
	FDCt_IIC_Send_Byte((FDC2214_ADDR<<1)|0);//����������ַ+д����	
	if(FDCt_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		FDCt_IIC_Stop();		 
		return 1;		
	}
    FDCt_IIC_Send_Byte(reg);//д�Ĵ�����ַ
    FDCt_IIC_Wait_Ack();		 //�ȴ�Ӧ�� 
	FDCt_IIC_Send_Byte(MSB);  //��������1
	if(FDCt_IIC_Wait_Ack())	 //�ȴ�ACK
	{
		FDCt_IIC_Stop();	 
		return 1;		 
	}		 
	FDCt_IIC_Send_Byte(LSB);  //��������2
	if(FDCt_IIC_Wait_Ack())	 //�ȴ�ACK
	{
		FDCt_IIC_Stop();	 
		return 1;		 
	}	
    FDCt_IIC_Stop();	 
	return 0;
}

/*��ȡFDC2214�Ĵ�������
 *IIC��2���ֽ� 
 *reg:�Ĵ�����ַ 
 *����ֵ:����������
 */
u16 FDCt_Read(u8 reg)
{
	u16 res;
   FDCt_IIC_Start(); 
	FDCt_IIC_Send_Byte((FDC2214_ADDR<<1)|0);//����������ַ+д����	
	FDCt_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
    FDCt_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    FDCt_IIC_Wait_Ack();		//�ȴ�Ӧ��
    FDCt_IIC_Start();
	FDCt_IIC_Send_Byte((FDC2214_ADDR<<1)|1);//����������ַ+������	
    FDCt_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	res=FDCt_IIC_Read_Byte(1)<<8;//��ȡ����,����ACK
	  
	res|=FDCt_IIC_Read_Byte(0);//��ȡ����,����nACK
    FDCt_IIC_Stop();			//����һ��ֹͣ����
	return res;		
}

/*�õ�ͨ��xת���������
 *index:0 -> CH0
 *      1 -> CH1
 *      2 -> CH2
 *      3 -> CH3
*����ֵ:����������
 */
//u16 FCD2214_ReadCH(u8 index) 
//{
//	u16 result;
//	switch(index)
//	{
//		case 0:
//		  result = FDC_Read(DATA_CH0)&0x0FFF;
//		  result = (result<<4)|(FDC_Read(DATA_LSB_CH0)>>12);
//			break;
//		case 1:
//			result = FDC_Read(DATA_CH1)&0x0FFF;
//		  result = (result<<4)|(FDC_Read(DATA_LSB_CH1)>>12);
//			break;
//		case 2:
//			result = FDC_Read(DATA_CH2)&0x0FFF;
//		  result = (result<<4)|(FDC_Read(DATA_LSB_CH2)>>12);
//			break;
//		case 3:
//			result = FDC_Read(DATA_CH3)&0x0FFF;
//		  result = (result<<4)|(FDC_Read(DATA_LSB_CH3)>>12);
//			break;
//		default:break;
//	}
//	return result;
//}

u32 FCD2214tt_ReadCH(u8 index) 
{
	u32 result;
	switch(index)
	{
		case 0:
		  result = FDCt_Read(DATA_CH0)&0x0FFF;
		  result = (result<<16)|(FDCt_Read(DATA_LSB_CH0));
			break;
		case 1:
			result = FDCt_Read(DATA_CH1)&0x0FFF;
		  result = (result<<16)|(FDCt_Read(DATA_LSB_CH1));
			break;
		case 2:
			result = FDCt_Read(DATA_CH2)&0x0FFF;
		  result = (result<<16)|(FDCt_Read(DATA_LSB_CH2));
			break;
		case 3:
			result = FDCt_Read(DATA_CH3)&0x0FFF;
		  result = (result<<16)|(FDCt_Read(DATA_LSB_CH3));
			break;
		default:break;
	}
	result =result&0x0FFFFFFF;
	return result;
}

/*FDC2214��ʼ������
 *����ֵ:0����ʼ������
 *       1��������
 */
u8 FDC2214t_Init(void)
{
	u16 res;


	
	//���ID�Ƿ���ȷ���ٳ�ʼ���Ĵ���
	res=FDCt_Read(MANUFACTURER_ID);

	if(res==0x5449)
	{
		//����Set_FDC2214�Ĵ���
		Sett_FDC2214(RCOUNT_CH0,0x34,0xFB);//�ο�����ת�����ʱ�䣨T=(RCOUNT_CH0*16)/Frefx��
		Sett_FDC2214(RCOUNT_CH1,0x34,0xFB);
		Sett_FDC2214(RCOUNT_CH2,0x34,0xFB);
		
		Sett_FDC2214(SETTLECOUNT_CH0,0x00,0x1B);//ת��֮ǰ���ȶ�ʱ�䣨T=(SETTLECOUNT_CHx*16)/Frefx��
		Sett_FDC2214(SETTLECOUNT_CH1,0x00,0x1B);
		Sett_FDC2214(SETTLECOUNT_CH2,0x00,0x1B);
		
		Sett_FDC2214(CLOCK_DIVIDERS_C_CH0,0x20,0x02);//ѡ����0.01MHz ~ 10MHz�Ĵ�����Ƶ��
		Sett_FDC2214(CLOCK_DIVIDERS_C_CH1,0x20,0x02);//Frefx = Fclk = 43.4MHz/2(2��Ƶ)
		Sett_FDC2214(CLOCK_DIVIDERS_C_CH2,0x20,0x02);//CHx_REF_DIVIDER=2;CHx_FIN_SEL=2
		
		Sett_FDC2214(DRIVE_CURRENT_CH0,0x78,0x00);//0.146ma��������ʱ�ӽ���+ת��ʱ�������������
		Sett_FDC2214(DRIVE_CURRENT_CH1,0x78,0x00);
		Sett_FDC2214(DRIVE_CURRENT_CH2,0x78,0x00);
		
		Sett_FDC2214(ERROR_CONFIG,0x00,0x00);//ȫ����ֹ����㱨
		
		Sett_FDC2214(MUX_CONFIG,0xC2,0x0D);//ͨ��0��1��2 ��3��ѡ��10MhzΪ�����񵴲���Ƶ�ʵ�������ã���ͨ������ͨ��
		
		Sett_FDC2214(CONFIG,0x14,0x01);//����ģʽ��ʹ���ڲ��������ο�Ƶ�ʣ�INTB���Ż���״̬�Ĵ������±���λ
	}
	else return 1;
	
	return 0;
}

/*������·����
 *index:0����·0
 *      1����·1
 *      2����·2
 *      3����·3
 *����ֵ����·�ܵ���C
 */
float Cap1_Calculate(u8 index)
{
	float Cap;
	Data_FDCt = FCD2214tt_ReadCH(index);
//	Cap = 56645.763f/((float)Data_FDC);
//	return ((Cap*Cap)-33);
	Cap = 232021045.248/(Data_FDCt);
	return (Cap*Cap);
}



