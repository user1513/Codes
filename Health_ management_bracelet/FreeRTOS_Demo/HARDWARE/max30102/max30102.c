#include "max30102.h"
#include "delay.h"

#define max30102_WR_address 0xAE
#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitI2C
*	����˵��: ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitI2C(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//RCC->APB2ENR|=1<<4;//��ʹ������IO PORTCʱ�� 
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	   
	GPIO_InitStructure.Pin = GPIO_PIN_7 | GPIO_PIN_8;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH ;   //�������
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_OD;  //�������;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	
	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	max30102_i2c_Stop();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void max30102_i2c_Delay(void)
{
	uint8_t i;

	/*��
		CPU��Ƶ168MHzʱ�����ڲ�Flash����, MDK���̲��Ż�����̨ʽʾ�����۲Ⲩ�Ρ�
		ѭ������Ϊ5ʱ��SCLƵ�� = 1.78MHz (����ʱ: 92ms, ��д������������ʾ����̽ͷ���ϾͶ�дʧ�ܡ�ʱ��ӽ��ٽ�)
		ѭ������Ϊ10ʱ��SCLƵ�� = 1.1MHz (����ʱ: 138ms, ���ٶ�: 118724B/s)
		ѭ������Ϊ30ʱ��SCLƵ�� = 440KHz�� SCL�ߵ�ƽʱ��1.0us��SCL�͵�ƽʱ��1.2us

		��������ѡ��2.2Kŷʱ��SCL������ʱ��Լ0.5us�����ѡ4.7Kŷ����������Լ1us

		ʵ��Ӧ��ѡ��400KHz���ҵ����ʼ���
	*/
	for (i = 0; i < 30; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C���������ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void max30102_i2c_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
	max30102_I2C_SDA_1();
	max30102_I2C_SCL_1();
	max30102_i2c_Delay();
	max30102_I2C_SDA_0();
	max30102_i2c_Delay();
	
	max30102_I2C_SCL_0();
	max30102_i2c_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void max30102_i2c_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	max30102_I2C_SDA_0();
	max30102_I2C_SCL_1();
	max30102_i2c_Delay();
	max30102_I2C_SDA_1();
	max30102_i2c_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    ��:  _ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void max30102_i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			max30102_I2C_SDA_1();
		}
		else
		{
			max30102_I2C_SDA_0();
		}
		max30102_i2c_Delay();
		max30102_I2C_SCL_1();
		max30102_i2c_Delay();
		max30102_I2C_SCL_0();
		if (i == 7)
		{
			 max30102_I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		max30102_i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    ��:  ��
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t max30102_i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		max30102_I2C_SCL_1();
		max30102_i2c_Delay();
		max30102_SDA_IN()
		if (max30102_I2C_SDA_READ())
		{
			value++;
		}
		max30102_SDA_OUT()
		max30102_I2C_SCL_0();
		max30102_i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    ��:  ��
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t max30102_i2c_WaitAck(void)
{
	uint8_t re;

	max30102_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	max30102_i2c_Delay();
	max30102_I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	max30102_i2c_Delay();
	max30102_SDA_IN()
	if (max30102_I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	max30102_SDA_OUT()
	max30102_I2C_SCL_0();
	max30102_i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void max30102_i2c_Ack(void)
{
	max30102_I2C_SDA_0();	/* CPU����SDA = 0 */
	max30102_i2c_Delay();
	max30102_I2C_SCL_1();	/* CPU����1��ʱ�� */
	max30102_i2c_Delay();
	max30102_I2C_SCL_0();
	max30102_i2c_Delay();
	max30102_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void max30102_i2c_NAck(void)
{
	max30102_I2C_SDA_1();	/* CPU����SDA = 1 */
	max30102_i2c_Delay();
	max30102_I2C_SCL_1();	/* CPU����1��ʱ�� */
	max30102_i2c_Delay();
	max30102_I2C_SCL_0();
	max30102_i2c_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    ��:  _Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
uint8_t max30102_i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;
	max30102_SDA_IN()
	max30102_SCL_IN()
	if (max30102_I2C_SDA_READ() && max30102_I2C_SCL_READ())
	{
		max30102_SDA_OUT()
		max30102_SCL_OUT()
		max30102_i2c_Start();		/* ���������ź� */

		/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
		max30102_i2c_SendByte(_Address | I2C_WR);
		ucAck = max30102_i2c_WaitAck();	/* ����豸��ACKӦ�� */

		max30102_i2c_Stop();			/* ����ֹͣ�ź� */

		return ucAck;
	}
	return 1;	/* I2C�����쳣 */
}




bool maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data)
/**
* \brief        Write a value to a MAX30102 register
* \par          Details
*               This function writes a value to a MAX30102 register
*
* \param[in]    uch_addr    - register address
* \param[in]    uch_data    - register data
*
* \retval       true on success
*/
{
    /* ��1��������I2C���������ź� */
    max30102_i2c_Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    max30102_i2c_SendByte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */

    /* ��3��������ACK */
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }

    /* ��4���������ֽڵ�ַ */
    max30102_i2c_SendByte(uch_addr);
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }

    /* ��5������ʼд������ */
    max30102_i2c_SendByte(uch_data);

    /* ��6��������ACK */
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }

    /* ����I2C����ֹͣ�ź� */
    max30102_i2c_Stop();
    return true;	/* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    max30102_i2c_Stop();
    return false;
}

bool maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data)
/**
* \brief        Read a MAX30102 register
* \par          Details
*               This function reads a MAX30102 register
*
* \param[in]    uch_addr    - register address
* \param[out]   puch_data    - pointer that stores the register data
*
* \retval       true on success
*/
{
    /* ��1��������I2C���������ź� */
    max30102_i2c_Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    max30102_i2c_SendByte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */

    /* ��3��������ACK */
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }

    /* ��4���������ֽڵ�ַ�� */
    max30102_i2c_SendByte((uint8_t)uch_addr);
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }


    /* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
    max30102_i2c_Start();

    /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    max30102_i2c_SendByte(max30102_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */

    /* ��8��������ACK */
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }

    /* ��9������ȡ���� */
    {
        *puch_data = max30102_i2c_ReadByte();	/* ��1���ֽ� */

        max30102_i2c_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
    }
    /* ����I2C����ֹͣ�ź� */
    max30102_i2c_Stop();
    return true;	/* ִ�гɹ� ����dataֵ */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    max30102_i2c_Stop();
    return false;
}

bool maxim_max30102_init(void)
/**
* \brief        Initialize the MAX30102
* \par          Details
*               This function initializes the MAX30102
*
* \param        None
*
* \retval       true on success
*/
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//RCC->APB2ENR|=1<<4;//��ʹ������IO PORTCʱ�� 
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	   
	GPIO_InitStructure.Pin = GPIO_PIN_9;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Mode=GPIO_MODE_INPUT;  //�������;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    if(!maxim_max30102_write_reg(REG_INTR_ENABLE_1, 0xc0)) // INTR setting
        return false;
    if(!maxim_max30102_write_reg(REG_INTR_ENABLE_2, 0x00))
        return false;
    if(!maxim_max30102_write_reg(REG_FIFO_WR_PTR, 0x00)) //FIFO_WR_PTR[4:0]
        return false;
    if(!maxim_max30102_write_reg(REG_OVF_COUNTER, 0x00)) //OVF_COUNTER[4:0]
        return false;
    if(!maxim_max30102_write_reg(REG_FIFO_RD_PTR, 0x00)) //FIFO_RD_PTR[4:0]
        return false;
    if(!maxim_max30102_write_reg(REG_FIFO_CONFIG, 0x6f)) //sample avg = 8, fifo rollover=false, fifo almost full = 17
        return false;
    if(!maxim_max30102_write_reg(REG_MODE_CONFIG, 0x03))  //0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
        return false;
    if(!maxim_max30102_write_reg(REG_SPO2_CONFIG, 0x2F)) // SPO2_ADC range = 4096nA, SPO2 sample rate (400 Hz), LED pulseWidth (411uS)
        return false;

    if(!maxim_max30102_write_reg(REG_LED1_PA, 0x17))  //Choose value for ~ 4.5mA for LED1
        return false;
    if(!maxim_max30102_write_reg(REG_LED2_PA, 0x17))  // Choose value for ~ 4.5mA for LED2
        return false;
    if(!maxim_max30102_write_reg(REG_PILOT_PA, 0x7f))  // Choose value for ~ 25mA for Pilot LED
        return false;
    return true;
}

bool maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led)

/**
* \brief        Read a set of samples from the MAX30102 FIFO register
* \par          Details
*               This function reads a set of samples from the MAX30102 FIFO register
*
* \param[out]   *pun_red_led   - pointer that stores the red LED reading data
* \param[out]   *pun_ir_led    - pointer that stores the IR LED reading data
*
* \retval       true on success
*/
{
    uint32_t un_temp;
    uint8_t uch_temp;
    *pun_ir_led = 0;
    *pun_red_led = 0;
    maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
    maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);



    /* ��1��������I2C���������ź� */
    max30102_i2c_Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    max30102_i2c_SendByte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */

    /* ��3��������ACK */
    if (max30102_i2c_WaitAck() != 0)
    {
        //printf("read fifo failed");
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }

    /* ��4���������ֽڵ�ַ�� */
    max30102_i2c_SendByte((uint8_t)REG_FIFO_DATA);
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }


    /* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
    max30102_i2c_Start();

    /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    max30102_i2c_SendByte(max30102_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */

    /* ��8��������ACK */
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM������Ӧ�� */
    }

    un_temp = max30102_i2c_ReadByte();
    max30102_i2c_Ack();
    un_temp <<= 16;
    *pun_red_led += un_temp;
    un_temp = max30102_i2c_ReadByte();
    max30102_i2c_Ack();
    un_temp <<= 8;
    *pun_red_led += un_temp;
    un_temp = max30102_i2c_ReadByte();
    max30102_i2c_Ack();
    *pun_red_led += un_temp;

    un_temp = max30102_i2c_ReadByte();
    max30102_i2c_Ack();
    un_temp <<= 16;
    *pun_ir_led += un_temp;
    un_temp = max30102_i2c_ReadByte();
    max30102_i2c_Ack();
    un_temp <<= 8;
    *pun_ir_led += un_temp;
    un_temp = max30102_i2c_ReadByte();
    max30102_i2c_Ack();
    *pun_ir_led += un_temp;
    *pun_red_led &= 0x03FFFF; //Mask MSB [23:18]
    *pun_ir_led &= 0x03FFFF; //Mask MSB [23:18]

    /* ����I2C����ֹͣ�ź� */
    max30102_i2c_Stop();
    return true;
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    max30102_i2c_Stop();
    return false;
}

bool maxim_max30102_reset()
/**
* \brief        Reset the MAX30102
* \par          Details
*               This function resets the MAX30102
*
* \param        None
*
* \retval       true on success
*/
{
    if(!maxim_max30102_write_reg(REG_MODE_CONFIG, 0x40))
        return false;
    else
        return true;
}



















////��ʼ��IIC
//void max30102_IIC_Init(void)
//{					     
//	GPIO_InitTypeDef GPIO_InitStructure;
//	//RCC->APB2ENR|=1<<4;//��ʹ������IO PORTCʱ�� 
//	__HAL_RCC_GPIOB_CLK_ENABLE();	
//	   
//	GPIO_InitStructure.Pin = GPIO_PIN_7 | GPIO_PIN_8;
//	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH ;   //�������
//	GPIO_InitStructure.Pull = GPIO_PULLUP;
//	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;  //�������;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
// 
//	max30102_IIC_SCL=1;
//	max30102_IIC_SDA=1;

//}
////����IIC��ʼ�ź�
//void max30102_IIC_Start(void)
//{
//	max30102_SDA_OUT();     //sda�����
//	max30102_IIC_SDA=1;	  	  
//	max30102_IIC_SCL=1;
//	delay_us(4);
// 	max30102_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//	delay_us(4);
//	max30102_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
//}	  
////����IICֹͣ�ź�
//void max30102_IIC_Stop(void)
//{
//	max30102_SDA_OUT();//sda�����
//	max30102_IIC_SCL=0;
//	max30102_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
// 	delay_us(4);
//	max30102_IIC_SCL=1; 
//	max30102_IIC_SDA=1;//����I2C���߽����ź�
//	delay_us(4);							   	
//}
////�ȴ�Ӧ���źŵ���
////����ֵ��1������Ӧ��ʧ��
////        0������Ӧ��ɹ�
//u8 max30102_IIC_Wait_Ack(void)
//{
//	u8 ucErrTime=0;
//	max30102_SDA_IN();      //SDA����Ϊ����  
//	max30102_IIC_SDA=1;delay_us(1);	   
//	max30102_IIC_SCL=1;delay_us(1);	 
//	while(max30102_READ_SDA)
//	{
//		ucErrTime++;
//		if(ucErrTime>250)
//		{
//			max30102_IIC_Stop();
//			return 1;
//		}
//	}
//	max30102_IIC_SCL=0;//ʱ�����0 	   
//	return 0;  
//} 
////����ACKӦ��
//void max30102_IIC_Ack(void)
//{
//	max30102_IIC_SCL=0;
//	max30102_SDA_OUT();
//	max30102_IIC_SDA=0;
//	delay_us(2);
//	max30102_IIC_SCL=1;
//	delay_us(2);
//	max30102_IIC_SCL=0;
//}
////������ACKӦ��		    
//void max30102_IIC_NAck(void)
//{
//	max30102_IIC_SCL=0;
//	max30102_SDA_OUT();
//	max30102_IIC_SDA=1;
//	delay_us(2);
//	max30102_IIC_SCL=1;
//	delay_us(2);
//	max30102_IIC_SCL=0;
//}					 				     
////IIC����һ���ֽ�
////���شӻ�����Ӧ��
////1����Ӧ��
////0����Ӧ��			  
//void max30102_IIC_Send_Byte(u8 txd)
//{                        
//    u8 t;   
//	max30102_SDA_OUT(); 	    
//    max30102_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
//    for(t=0;t<8;t++)
//    {              
//        max30102_IIC_SDA=(txd&0x80)>>7;
//        txd<<=1; 	  
//		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
//		max30102_IIC_SCL=1;
//		delay_us(2); 
//		max30102_IIC_SCL=0;	
//		delay_us(2);
//    }	 
//} 	    
////��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
//u8 max30102_IIC_Read_Byte(unsigned char ack)
//{
//	unsigned char i,receive=0;
//	max30102_SDA_IN();//SDA����Ϊ����
//    for(i=0;i<8;i++ )
//	{
//        max30102_IIC_SCL=0; 
//        delay_us(2);
//		max30102_IIC_SCL=1;
//        receive<<=1;
//        if(max30102_READ_SDA)receive++;   
//		delay_us(1); 
//    }					 
//    if (!ack)
//        max30102_IIC_NAck();//����nACK
//    else
//        max30102_IIC_Ack(); //����ACK   
//    return receive;
//}


//void max30102_IIC_WriteBytes(u8 WriteAddr,u8* data,u8 dataLength)
//{		
//	u8 i;	
//    max30102_IIC_Start();  

//	max30102_IIC_Send_Byte(WriteAddr);	    //����д����
//	max30102_IIC_Wait_Ack();
//	
//	for(i=0;i<dataLength;i++)
//	{
//		max30102_IIC_Send_Byte(data[i]);
//		max30102_IIC_Wait_Ack();
//	}				    	   
//    max30102_IIC_Stop();//����һ��ֹͣ���� 
//	//delay_ms(10);	 
//}

//void max30102_IIC_ReadBytes(u8 deviceAddr, u8 writeAddr,u8* data,u8 dataLength)
//{		
//	u8 i;	
//    max30102_IIC_Start();  

//	max30102_IIC_Send_Byte(deviceAddr);	    //����д����
//	max30102_IIC_Wait_Ack();
//	max30102_IIC_Send_Byte(writeAddr);
//	max30102_IIC_Wait_Ack();
//	max30102_IIC_Send_Byte(deviceAddr|0X01);//�������ģʽ			   
//	max30102_IIC_Wait_Ack();
//	
//	for(i=0;i<dataLength-1;i++)
//	{
//		data[i] = max30102_IIC_Read_Byte(1);
//	}		
//	data[dataLength-1] = max30102_IIC_Read_Byte(0);	
//    max30102_IIC_Stop();//����һ��ֹͣ���� 
//	//delay_ms(10);	 
//}

//void max30102_IIC_Read_One_Byte(u8 daddr,u8 addr,u8* data)
//{				  	  	    																 
//    max30102_IIC_Start();  
//	
//	max30102_IIC_Send_Byte(daddr);	   //����д����
//	max30102_IIC_Wait_Ack();
//	max30102_IIC_Send_Byte(addr);//���͵�ַ
//	max30102_IIC_Wait_Ack();		 
//	max30102_IIC_Start();  	 	   
//	max30102_IIC_Send_Byte(daddr|0X01);//�������ģʽ			   
//	max30102_IIC_Wait_Ack();	 
//    *data = max30102_IIC_Read_Byte(0);		   
//    max30102_IIC_Stop();//����һ��ֹͣ����	    
//}

//void max30102_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data)
//{				   	  	    																 
//    max30102_IIC_Start();  
//	
//	max30102_IIC_Send_Byte(daddr);	    //����д����
//	max30102_IIC_Wait_Ack();
//	max30102_IIC_Send_Byte(addr);//���͵�ַ
//	max30102_IIC_Wait_Ack();	   	 										  		   
//	max30102_IIC_Send_Byte(data);     //�����ֽ�							   
//	max30102_IIC_Wait_Ack();  		    	   
//    max30102_IIC_Stop();//����һ��ֹͣ���� 
//	//delay_ms(10);	 
//}

//u8 max30102_Bus_Write(u8 Register_Address, u8 Word_Data)
//{

//	/* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */

//	/* ��1��������I2C���������ź� */
//	max30102_IIC_Start();

//	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	max30102_IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */

//	/* ��3��������ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}

//	/* ��4���������ֽڵ�ַ */
//	max30102_IIC_Send_Byte(Register_Address);
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}
//	
//	/* ��5������ʼд������ */
//	max30102_IIC_Send_Byte(Word_Data);

//	/* ��6��������ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}

//	/* ����I2C����ֹͣ�ź� */
//	max30102_IIC_Stop();
//	return 1;	/* ִ�гɹ� */

//cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
//	/* ����I2C����ֹͣ�ź� */
//	max30102_IIC_Stop();
//	return 0;
//}



//u8 max30102_Bus_Read(u8 Register_Address)
//{
//	u8  data;


//	/* ��1��������I2C���������ź� */
//	max30102_IIC_Start();

//	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	max30102_IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */

//	/* ��3��������ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}

//	/* ��4���������ֽڵ�ַ�� */
//	max30102_IIC_Send_Byte((uint8_t)Register_Address);
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}
//	

//	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
//	max30102_IIC_Start();

//	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	max30102_IIC_Send_Byte(max30102_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */

//	/* ��8��������ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}

//	/* ��9������ȡ���� */
//	{
//		data = max30102_IIC_Read_Byte(0);	/* ��1���ֽ� */

//		max30102_IIC_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
//	}
//	/* ����I2C����ֹͣ�ź� */
//	max30102_IIC_Stop();
//	return data;	/* ִ�гɹ� ����dataֵ */

//cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
//	/* ����I2C����ֹͣ�ź� */
//	max30102_IIC_Stop();
//	return 0;
//}


//void max30102_FIFO_ReadWords(u8 Register_Address,u16 Word_Data[][2],u8 count)
//{
//	u8 i=0;
//	u8 no = count;
//	u8 data1, data2;
//	/* ��1��������I2C���������ź� */
//	max30102_IIC_Start();

//	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	max30102_IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */

//	/* ��3��������ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}

//	/* ��4���������ֽڵ�ַ�� */
//	max30102_IIC_Send_Byte((uint8_t)Register_Address);
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}
//	

//	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
//	max30102_IIC_Start();

//	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	max30102_IIC_Send_Byte(max30102_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */

//	/* ��8��������ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}

//	/* ��9������ȡ���� */
//	while (no)
//	{
//		data1 = max30102_IIC_Read_Byte(0);	
//		max30102_IIC_Ack();
//		data2 = max30102_IIC_Read_Byte(0);
//		max30102_IIC_Ack();
//		Word_Data[i][0] = (((u16)data1 << 8) | data2);  //

//		
//		data1 = max30102_IIC_Read_Byte(0);	
//		max30102_IIC_Ack();
//		data2 = max30102_IIC_Read_Byte(0);
//		if(1==no)
//			max30102_IIC_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
//		else
//			max30102_IIC_Ack();
//		Word_Data[i][1] = (((u16)data1 << 8) | data2); 

//		no--;	
//		i++;
//	}
//	/* ����I2C����ֹͣ�ź� */
//	max30102_IIC_Stop();

//cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
//	/* ����I2C����ֹͣ�ź� */
//	max30102_IIC_Stop();
//}

//void max30102_FIFO_ReadBytes(u8 Register_Address,u8* Data)
//{	
//	max30102_Bus_Read(REG_INTR_STATUS_1);
//	max30102_Bus_Read(REG_INTR_STATUS_2);
//	
//	/* ��1��������I2C���������ź� */
//	max30102_IIC_Start();

//	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	max30102_IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */

//	/* ��3��������ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}

//	/* ��4���������ֽڵ�ַ�� */
//	max30102_IIC_Send_Byte((uint8_t)Register_Address);
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}
//	

//	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
//	max30102_IIC_Start();

//	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	max30102_IIC_Send_Byte(max30102_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */

//	/* ��8��������ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM������Ӧ�� */
//	}

//	/* ��9������ȡ���� */
//	Data[0] = max30102_IIC_Read_Byte(1);	
//	Data[1] = max30102_IIC_Read_Byte(1);	
//	Data[2] = max30102_IIC_Read_Byte(1);	
//	Data[3] = max30102_IIC_Read_Byte(1);
//	Data[4] = max30102_IIC_Read_Byte(1);	
//	Data[5] = max30102_IIC_Read_Byte(0);
//	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
//	/* ����I2C����ֹͣ�ź� */
//	max30102_IIC_Stop();

//cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
//	/* ����I2C����ֹͣ�ź� */
//	max30102_IIC_Stop();

////	u8 i;
////	u8 fifo_wr_ptr;
////	u8 firo_rd_ptr;
////	u8 number_tp_read;
////	//Get the FIFO_WR_PTR
////	fifo_wr_ptr = max30102_Bus_Read(REG_FIFO_WR_PTR);
////	//Get the FIFO_RD_PTR
////	firo_rd_ptr = max30102_Bus_Read(REG_FIFO_RD_PTR);
////	
////	number_tp_read = fifo_wr_ptr - firo_rd_ptr;
////	
////	//for(i=0;i<number_tp_read;i++){
////	if(number_tp_read>0){
////		IIC_ReadBytes(max30102_WR_address,REG_FIFO_DATA,Data,6);
////	}
//	
//	//max30102_Bus_Write(REG_FIFO_RD_PTR,fifo_wr_ptr);
//}

//void max30102_init(void)
//{
////	GPIO_InitTypeDef GPIO_InitStructure;
////	//RCC->APB2ENR|=1<<4;//��ʹ������IO PORTCʱ�� 
////	__HAL_RCC_GPIOB_CLK_ENABLE();	
////	   
////	GPIO_InitStructure.Pin = GPIO_PIN_9;
////	GPIO_InitStructure.Pull = GPIO_PULLUP;
////	GPIO_InitStructure.Mode=GPIO_MODE_INPUT;  //�������;
////	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
//	max30102_IIC_Init();
//	
//	max30102_reset();
//	
////	max30102_Bus_Write(REG_MODE_CONFIG, 0x0b);  //mode configuration : temp_en[3]      MODE[2:0]=010 HR only enabled    011 SP02 enabled
////	max30102_Bus_Write(REG_INTR_STATUS_2, 0xF0); //open all of interrupt
////	max30102_Bus_Write(REG_INTR_STATUS_1, 0x00); //all interrupt clear
////	max30102_Bus_Write(REG_INTR_ENABLE_2, 0x02); //DIE_TEMP_RDY_EN
////	max30102_Bus_Write(REG_TEMP_CONFIG, 0x01); //SET   TEMP_EN

////	max30102_Bus_Write(REG_SPO2_CONFIG, 0x47); //SPO2_SR[4:2]=001  100 per second    LED_PW[1:0]=11  16BITS

////	max30102_Bus_Write(REG_LED1_PA, 0x47); 
////	max30102_Bus_Write(REG_LED2_PA, 0x47); 
//	
//	
//	
//	max30102_Bus_Write(REG_INTR_ENABLE_1,0xc0);	// INTR setting
//	max30102_Bus_Write(REG_INTR_ENABLE_2,0x00);
//	max30102_Bus_Write(REG_FIFO_WR_PTR,0x00);  	//FIFO_WR_PTR[4:0]
//	max30102_Bus_Write(REG_OVF_COUNTER,0x00);  	//OVF_COUNTER[4:0]
//	max30102_Bus_Write(REG_FIFO_RD_PTR,0x00);  	//FIFO_RD_PTR[4:0]
//	max30102_Bus_Write(REG_FIFO_CONFIG,0x0f);  	//sample avg = 1, fifo rollover=false, fifo almost full = 17
//	max30102_Bus_Write(REG_MODE_CONFIG,0x03);  	//0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
//	max30102_Bus_Write(REG_SPO2_CONFIG,0x27);  	// SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)  
//	max30102_Bus_Write(REG_LED1_PA,0x24);   	//Choose value for ~ 7mA for LED1
//	max30102_Bus_Write(REG_LED2_PA,0x24);   	// Choose value for ~ 7mA for LED2
//	max30102_Bus_Write(REG_PILOT_PA,0x7f);   	// Choose value for ~ 25mA for Pilot LED


//	
////	// Interrupt Enable 1 Register. Set PPG_RDY_EN (data available in FIFO)
////	max30102_Bus_Write(0x2, 1<<6);

////	// FIFO configuration register
////	// SMP_AVE: 16 samples averaged per FIFO sample
////	// FIFO_ROLLOVER_EN=1
////	//max30102_Bus_Write(0x8,  1<<4);
////	max30102_Bus_Write(0x8, (0<<5) | 1<<4);

////	// Mode Configuration Register
////	// SPO2 mode
////	max30102_Bus_Write(0x9, 3);

////	// SPO2 Configuration Register
////	max30102_Bus_Write(0xa,
////			(3<<5)  // SPO2_ADC_RGE 2 = full scale 8192 nA (LSB size 31.25pA); 3 = 16384nA
////			| (1<<2) // sample rate: 0 = 50sps; 1 = 100sps; 2 = 200sps
////			| (3<<0) // LED_PW 3 = 411��s, ADC resolution 18 bits
////	);

////	// LED1 (red) power (0 = 0mA; 255 = 50mA)
////	max30102_Bus_Write(0xc, 0xb0);

////	// LED (IR) power
////	max30102_Bus_Write(0xd, 0xa0);
//											
//}

//void max30102_reset(void)
//{
//	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
//	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
//}






//void maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data)
//{
////  char ach_i2c_data[2];
////  ach_i2c_data[0]=uch_addr;
////  ach_i2c_data[1]=uch_data;
////	
////  IIC_WriteBytes(I2C_WRITE_ADDR, ach_i2c_data, 2);
//	max30102_IIC_Write_One_Byte(I2C_WRITE_ADDR,uch_addr,uch_data);
//}

//void maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data)
//{
////  char ch_i2c_data;
////  ch_i2c_data=uch_addr;
////  IIC_WriteBytes(I2C_WRITE_ADDR, &ch_i2c_data, 1);
////	
////  i2c.read(I2C_READ_ADDR, &ch_i2c_data, 1);
////  
////   *puch_data=(uint8_t) ch_i2c_data;
//	max30102_IIC_Read_One_Byte(I2C_WRITE_ADDR,uch_addr,puch_data);
//}

//void maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led)
//{
//	uint32_t un_temp;
//	unsigned char uch_temp;
//	char ach_i2c_data[6];
//	*pun_red_led=0;
//	*pun_ir_led=0;

//  
//  //read and clear status register
//  maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
//  maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);
//  
//  max30102_IIC_ReadBytes(I2C_WRITE_ADDR,REG_FIFO_DATA,(u8 *)ach_i2c_data,6);
//  
//  un_temp=(unsigned char) ach_i2c_data[0];
//  un_temp<<=16;
//  *pun_red_led+=un_temp;
//  un_temp=(unsigned char) ach_i2c_data[1];
//  un_temp<<=8;
//  *pun_red_led+=un_temp;
//  un_temp=(unsigned char) ach_i2c_data[2];
//  *pun_red_led+=un_temp;
//  
//  un_temp=(unsigned char) ach_i2c_data[3];
//  un_temp<<=16;
//  *pun_ir_led+=un_temp;
//  un_temp=(unsigned char) ach_i2c_data[4];
//  un_temp<<=8;
//  *pun_ir_led+=un_temp;
//  un_temp=(unsigned char) ach_i2c_data[5];
//  *pun_ir_led+=un_temp;
//  *pun_red_led&=0x03FFFF;  //Mask MSB [23:18]
//  *pun_ir_led&=0x03FFFF;  //Mask MSB [23:18]
//}
