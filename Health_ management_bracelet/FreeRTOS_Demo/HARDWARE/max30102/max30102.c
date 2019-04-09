#include "max30102.h"
#include "delay.h"

#define max30102_WR_address 0xAE
#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */
/*
*********************************************************************************************************
*	函 数 名: bsp_InitI2C
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitI2C(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟 
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	   
	GPIO_InitStructure.Pin = GPIO_PIN_7 | GPIO_PIN_8;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH ;   //高速输出
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_OD;  //推挽输出;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	
	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	max30102_i2c_Stop();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void max30102_i2c_Delay(void)
{
	uint8_t i;

	/*　
		CPU主频168MHz时，在内部Flash运行, MDK工程不优化。用台式示波器观测波形。
		循环次数为5时，SCL频率 = 1.78MHz (读耗时: 92ms, 读写正常，但是用示波器探头碰上就读写失败。时序接近临界)
		循环次数为10时，SCL频率 = 1.1MHz (读耗时: 138ms, 读速度: 118724B/s)
		循环次数为30时，SCL频率 = 440KHz， SCL高电平时间1.0us，SCL低电平时间1.2us

		上拉电阻选择2.2K欧时，SCL上升沿时间约0.5us，如果选4.7K欧，则上升沿约1us

		实际应用选择400KHz左右的速率即可
	*/
	for (i = 0; i < 30; i++);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void max30102_i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
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
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void max30102_i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	max30102_I2C_SDA_0();
	max30102_I2C_SCL_1();
	max30102_i2c_Delay();
	max30102_I2C_SDA_1();
	max30102_i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参:  _ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void max30102_i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
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
			 max30102_I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		max30102_i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参:  无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t max30102_i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
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
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参:  无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t max30102_i2c_WaitAck(void)
{
	uint8_t re;

	max30102_I2C_SDA_1();	/* CPU释放SDA总线 */
	max30102_i2c_Delay();
	max30102_I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	max30102_i2c_Delay();
	max30102_SDA_IN()
	if (max30102_I2C_SDA_READ())	/* CPU读取SDA口线状态 */
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
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void max30102_i2c_Ack(void)
{
	max30102_I2C_SDA_0();	/* CPU驱动SDA = 0 */
	max30102_i2c_Delay();
	max30102_I2C_SCL_1();	/* CPU产生1个时钟 */
	max30102_i2c_Delay();
	max30102_I2C_SCL_0();
	max30102_i2c_Delay();
	max30102_I2C_SDA_1();	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void max30102_i2c_NAck(void)
{
	max30102_I2C_SDA_1();	/* CPU驱动SDA = 1 */
	max30102_i2c_Delay();
	max30102_I2C_SCL_1();	/* CPU产生1个时钟 */
	max30102_i2c_Delay();
	max30102_I2C_SCL_0();
	max30102_i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参:  _Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
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
		max30102_i2c_Start();		/* 发送启动信号 */

		/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
		max30102_i2c_SendByte(_Address | I2C_WR);
		ucAck = max30102_i2c_WaitAck();	/* 检测设备的ACK应答 */

		max30102_i2c_Stop();			/* 发送停止信号 */

		return ucAck;
	}
	return 1;	/* I2C总线异常 */
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
    /* 第1步：发起I2C总线启动信号 */
    max30102_i2c_Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    max30102_i2c_SendByte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

    /* 第3步：发送ACK */
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第4步：发送字节地址 */
    max30102_i2c_SendByte(uch_addr);
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第5步：开始写入数据 */
    max30102_i2c_SendByte(uch_data);

    /* 第6步：发送ACK */
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 发送I2C总线停止信号 */
    max30102_i2c_Stop();
    return true;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
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
    /* 第1步：发起I2C总线启动信号 */
    max30102_i2c_Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    max30102_i2c_SendByte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

    /* 第3步：发送ACK */
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第4步：发送字节地址， */
    max30102_i2c_SendByte((uint8_t)uch_addr);
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }


    /* 第6步：重新启动I2C总线。下面开始读取数据 */
    max30102_i2c_Start();

    /* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    max30102_i2c_SendByte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */

    /* 第8步：发送ACK */
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第9步：读取数据 */
    {
        *puch_data = max30102_i2c_ReadByte();	/* 读1个字节 */

        max30102_i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
    }
    /* 发送I2C总线停止信号 */
    max30102_i2c_Stop();
    return true;	/* 执行成功 返回data值 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
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
	//RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟 
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	   
	GPIO_InitStructure.Pin = GPIO_PIN_9;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Mode=GPIO_MODE_INPUT;  //推挽输出;
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



    /* 第1步：发起I2C总线启动信号 */
    max30102_i2c_Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    max30102_i2c_SendByte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

    /* 第3步：发送ACK */
    if (max30102_i2c_WaitAck() != 0)
    {
        //printf("read fifo failed");
        goto cmd_fail;	/* EEPROM器件无应答 */
    }

    /* 第4步：发送字节地址， */
    max30102_i2c_SendByte((uint8_t)REG_FIFO_DATA);
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
    }


    /* 第6步：重新启动I2C总线。下面开始读取数据 */
    max30102_i2c_Start();

    /* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    max30102_i2c_SendByte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */

    /* 第8步：发送ACK */
    if (max30102_i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM器件无应答 */
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

    /* 发送I2C总线停止信号 */
    max30102_i2c_Stop();
    return true;
cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
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



















////初始化IIC
//void max30102_IIC_Init(void)
//{					     
//	GPIO_InitTypeDef GPIO_InitStructure;
//	//RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟 
//	__HAL_RCC_GPIOB_CLK_ENABLE();	
//	   
//	GPIO_InitStructure.Pin = GPIO_PIN_7 | GPIO_PIN_8;
//	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH ;   //高速输出
//	GPIO_InitStructure.Pull = GPIO_PULLUP;
//	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
// 
//	max30102_IIC_SCL=1;
//	max30102_IIC_SDA=1;

//}
////产生IIC起始信号
//void max30102_IIC_Start(void)
//{
//	max30102_SDA_OUT();     //sda线输出
//	max30102_IIC_SDA=1;	  	  
//	max30102_IIC_SCL=1;
//	delay_us(4);
// 	max30102_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//	delay_us(4);
//	max30102_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
//}	  
////产生IIC停止信号
//void max30102_IIC_Stop(void)
//{
//	max30102_SDA_OUT();//sda线输出
//	max30102_IIC_SCL=0;
//	max30102_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
// 	delay_us(4);
//	max30102_IIC_SCL=1; 
//	max30102_IIC_SDA=1;//发送I2C总线结束信号
//	delay_us(4);							   	
//}
////等待应答信号到来
////返回值：1，接收应答失败
////        0，接收应答成功
//u8 max30102_IIC_Wait_Ack(void)
//{
//	u8 ucErrTime=0;
//	max30102_SDA_IN();      //SDA设置为输入  
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
//	max30102_IIC_SCL=0;//时钟输出0 	   
//	return 0;  
//} 
////产生ACK应答
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
////不产生ACK应答		    
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
////IIC发送一个字节
////返回从机有无应答
////1，有应答
////0，无应答			  
//void max30102_IIC_Send_Byte(u8 txd)
//{                        
//    u8 t;   
//	max30102_SDA_OUT(); 	    
//    max30102_IIC_SCL=0;//拉低时钟开始数据传输
//    for(t=0;t<8;t++)
//    {              
//        max30102_IIC_SDA=(txd&0x80)>>7;
//        txd<<=1; 	  
//		delay_us(2);   //对TEA5767这三个延时都是必须的
//		max30102_IIC_SCL=1;
//		delay_us(2); 
//		max30102_IIC_SCL=0;	
//		delay_us(2);
//    }	 
//} 	    
////读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
//u8 max30102_IIC_Read_Byte(unsigned char ack)
//{
//	unsigned char i,receive=0;
//	max30102_SDA_IN();//SDA设置为输入
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
//        max30102_IIC_NAck();//发送nACK
//    else
//        max30102_IIC_Ack(); //发送ACK   
//    return receive;
//}


//void max30102_IIC_WriteBytes(u8 WriteAddr,u8* data,u8 dataLength)
//{		
//	u8 i;	
//    max30102_IIC_Start();  

//	max30102_IIC_Send_Byte(WriteAddr);	    //发送写命令
//	max30102_IIC_Wait_Ack();
//	
//	for(i=0;i<dataLength;i++)
//	{
//		max30102_IIC_Send_Byte(data[i]);
//		max30102_IIC_Wait_Ack();
//	}				    	   
//    max30102_IIC_Stop();//产生一个停止条件 
//	//delay_ms(10);	 
//}

//void max30102_IIC_ReadBytes(u8 deviceAddr, u8 writeAddr,u8* data,u8 dataLength)
//{		
//	u8 i;	
//    max30102_IIC_Start();  

//	max30102_IIC_Send_Byte(deviceAddr);	    //发送写命令
//	max30102_IIC_Wait_Ack();
//	max30102_IIC_Send_Byte(writeAddr);
//	max30102_IIC_Wait_Ack();
//	max30102_IIC_Send_Byte(deviceAddr|0X01);//进入接收模式			   
//	max30102_IIC_Wait_Ack();
//	
//	for(i=0;i<dataLength-1;i++)
//	{
//		data[i] = max30102_IIC_Read_Byte(1);
//	}		
//	data[dataLength-1] = max30102_IIC_Read_Byte(0);	
//    max30102_IIC_Stop();//产生一个停止条件 
//	//delay_ms(10);	 
//}

//void max30102_IIC_Read_One_Byte(u8 daddr,u8 addr,u8* data)
//{				  	  	    																 
//    max30102_IIC_Start();  
//	
//	max30102_IIC_Send_Byte(daddr);	   //发送写命令
//	max30102_IIC_Wait_Ack();
//	max30102_IIC_Send_Byte(addr);//发送地址
//	max30102_IIC_Wait_Ack();		 
//	max30102_IIC_Start();  	 	   
//	max30102_IIC_Send_Byte(daddr|0X01);//进入接收模式			   
//	max30102_IIC_Wait_Ack();	 
//    *data = max30102_IIC_Read_Byte(0);		   
//    max30102_IIC_Stop();//产生一个停止条件	    
//}

//void max30102_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data)
//{				   	  	    																 
//    max30102_IIC_Start();  
//	
//	max30102_IIC_Send_Byte(daddr);	    //发送写命令
//	max30102_IIC_Wait_Ack();
//	max30102_IIC_Send_Byte(addr);//发送地址
//	max30102_IIC_Wait_Ack();	   	 										  		   
//	max30102_IIC_Send_Byte(data);     //发送字节							   
//	max30102_IIC_Wait_Ack();  		    	   
//    max30102_IIC_Stop();//产生一个停止条件 
//	//delay_ms(10);	 
//}

//u8 max30102_Bus_Write(u8 Register_Address, u8 Word_Data)
//{

//	/* 采用串行EEPROM随即读取指令序列，连续读取若干字节 */

//	/* 第1步：发起I2C总线启动信号 */
//	max30102_IIC_Start();

//	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
//	max30102_IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

//	/* 第3步：发送ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}

//	/* 第4步：发送字节地址 */
//	max30102_IIC_Send_Byte(Register_Address);
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}
//	
//	/* 第5步：开始写入数据 */
//	max30102_IIC_Send_Byte(Word_Data);

//	/* 第6步：发送ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}

//	/* 发送I2C总线停止信号 */
//	max30102_IIC_Stop();
//	return 1;	/* 执行成功 */

//cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
//	/* 发送I2C总线停止信号 */
//	max30102_IIC_Stop();
//	return 0;
//}



//u8 max30102_Bus_Read(u8 Register_Address)
//{
//	u8  data;


//	/* 第1步：发起I2C总线启动信号 */
//	max30102_IIC_Start();

//	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
//	max30102_IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

//	/* 第3步：发送ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}

//	/* 第4步：发送字节地址， */
//	max30102_IIC_Send_Byte((uint8_t)Register_Address);
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}
//	

//	/* 第6步：重新启动I2C总线。下面开始读取数据 */
//	max30102_IIC_Start();

//	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
//	max30102_IIC_Send_Byte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */

//	/* 第8步：发送ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}

//	/* 第9步：读取数据 */
//	{
//		data = max30102_IIC_Read_Byte(0);	/* 读1个字节 */

//		max30102_IIC_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
//	}
//	/* 发送I2C总线停止信号 */
//	max30102_IIC_Stop();
//	return data;	/* 执行成功 返回data值 */

//cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
//	/* 发送I2C总线停止信号 */
//	max30102_IIC_Stop();
//	return 0;
//}


//void max30102_FIFO_ReadWords(u8 Register_Address,u16 Word_Data[][2],u8 count)
//{
//	u8 i=0;
//	u8 no = count;
//	u8 data1, data2;
//	/* 第1步：发起I2C总线启动信号 */
//	max30102_IIC_Start();

//	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
//	max30102_IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

//	/* 第3步：发送ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}

//	/* 第4步：发送字节地址， */
//	max30102_IIC_Send_Byte((uint8_t)Register_Address);
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}
//	

//	/* 第6步：重新启动I2C总线。下面开始读取数据 */
//	max30102_IIC_Start();

//	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
//	max30102_IIC_Send_Byte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */

//	/* 第8步：发送ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}

//	/* 第9步：读取数据 */
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
//			max30102_IIC_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
//		else
//			max30102_IIC_Ack();
//		Word_Data[i][1] = (((u16)data1 << 8) | data2); 

//		no--;	
//		i++;
//	}
//	/* 发送I2C总线停止信号 */
//	max30102_IIC_Stop();

//cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
//	/* 发送I2C总线停止信号 */
//	max30102_IIC_Stop();
//}

//void max30102_FIFO_ReadBytes(u8 Register_Address,u8* Data)
//{	
//	max30102_Bus_Read(REG_INTR_STATUS_1);
//	max30102_Bus_Read(REG_INTR_STATUS_2);
//	
//	/* 第1步：发起I2C总线启动信号 */
//	max30102_IIC_Start();

//	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
//	max30102_IIC_Send_Byte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */

//	/* 第3步：发送ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}

//	/* 第4步：发送字节地址， */
//	max30102_IIC_Send_Byte((uint8_t)Register_Address);
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}
//	

//	/* 第6步：重新启动I2C总线。下面开始读取数据 */
//	max30102_IIC_Start();

//	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
//	max30102_IIC_Send_Byte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */

//	/* 第8步：发送ACK */
//	if (max30102_IIC_Wait_Ack() != 0)
//	{
//		goto cmd_fail;	/* EEPROM器件无应答 */
//	}

//	/* 第9步：读取数据 */
//	Data[0] = max30102_IIC_Read_Byte(1);	
//	Data[1] = max30102_IIC_Read_Byte(1);	
//	Data[2] = max30102_IIC_Read_Byte(1);	
//	Data[3] = max30102_IIC_Read_Byte(1);
//	Data[4] = max30102_IIC_Read_Byte(1);	
//	Data[5] = max30102_IIC_Read_Byte(0);
//	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
//	/* 发送I2C总线停止信号 */
//	max30102_IIC_Stop();

//cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
//	/* 发送I2C总线停止信号 */
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
////	//RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟 
////	__HAL_RCC_GPIOB_CLK_ENABLE();	
////	   
////	GPIO_InitStructure.Pin = GPIO_PIN_9;
////	GPIO_InitStructure.Pull = GPIO_PULLUP;
////	GPIO_InitStructure.Mode=GPIO_MODE_INPUT;  //推挽输出;
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
////			| (3<<0) // LED_PW 3 = 411μs, ADC resolution 18 bits
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
