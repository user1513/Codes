/**
 * ��� I2C ������
 * ��Դ���ı��ԡ�ʱ�䴥��Ƕ��ʽϵͳ���ģʽ��������½�
 * �����£�2014-07-17
 * http://blanboom.org
 *********************************************************
 * ע�⣺
 *       1. I2C_Delay() ��Ҫ����ʵ��Ӳ��������ʱʱ��
 *       2. I2C_Sync_The_Clock() ���ܻ��� I2C ���ߴ����ռ�ýϳ�ʱ��
 */
 
#include "Soft_I2C.h"
#include "PinDef.h"   // ��Ҫ�ڴ��ļ��ж��� I2C ���ţ�I2C_SDA, I2C_SCL

// ˽�к�������
static uint8_t I2C_Get_Ack_From_Slave(void); 
static bit     I2C_Sync_The_Clock(void);   
static void    I2C_Delay(void);  

// �������Ҫ ACK �� NOT ACK�����Խ�����һ��ע�͵�
#define I2C_ACK_NACK

/**
 * void I2C_Send_Start(void)
 * ��飺���� START ״̬
 * ���룺��
 * �������
 */
void I2C_Send_Start(void)
{
	I2C_Delay(); 
	// ��ʼ������
	I2C_SCL = 1;
	I2C_SDA = 1;
	I2C_Delay(); 

	// ���� START ״̬
	I2C_SDA = 0;
	I2C_Delay();
	I2C_SCL = 0;
	I2C_Delay();
}


/**
 * void I2C_Send_Stop(void)
 * ��飺���� STOP ״̬
 * ���룺��
 * �������
 */
void I2C_Send_Stop(void)
{
	I2C_Delay(); 
	I2C_SDA = 0; 
	I2C_Delay();   
	I2C_SCL = 1; 
	I2C_Delay();   
	I2C_SDA = 1;   
	I2C_Delay(); 
}

/**
 * uint8_t I2C_Get_Ack_From_Slave(void)
 * ��飺�ȴ��ӻ�Ӧ��
 * ���룺��
 * �����
 *       I2C_OK       - �ɹ�
 *       I2C_ERROR    - ʧ��
 */
uint8_t I2C_Get_Ack_From_Slave(void)
{
	// ��ʼ������
	I2C_SDA = 1; 
	I2C_SCL = 1;

	if(I2C_Sync_The_Clock())
	{
		return I2C_ERROR;  // ���� - ͬ��ʧ��
	}

	// ���ʱ��ͬ��
	I2C_Delay();       

	if (I2C_SDA)       
	{
		// ����ʱ������
		I2C_SCL = 0;  

		return I2C_ERROR;  // ���� -  �ӻ���Ӧ��
	}

	I2C_SCL = 0; // ����ʱ������

	return I2C_OK;     // �ɹ� - �ӻ���Ӧ��
}


/**
 * uint8_t I2C_Write_Byte(uint8_t Data)
 * ��飺����һ���ֽڵ��ӻ�
 * ���ܣ�
 *     ����һ���ֽڵ��ӻ���ͨ������ʱ�����족��֧�ֵ��ٴӻ�
 *     ����ʱ�� ~100 ΢�루���Ƿ������ߴ���
 * ���룺Data: ��Ҫ���͵��ֽ�
 * �����
 *     I2C_OK - �ɹ�
 *     I2C_ERROR    - ʧ��   
 */
uint8_t I2C_Write_Byte(uint8_t Data)
{
	uint8_t Bit = 0;  

	// ÿ�η���һλ���ݣ���λ�ȷ��ͣ�
	for (Bit = 0; Bit < 8; Bit++ )
	{
		I2C_Delay();
		I2C_SDA = (bit)((Data & 0x80) >> 7); 
		I2C_SCL = 1;  

		if (I2C_Sync_The_Clock() == I2C_ERROR)
		{
			return I2C_ERROR; // ���� - ͬ��ʧ��
		}

		I2C_Delay(); 

		// ����ʱ������
		I2C_SCL = 0;                        

		// ׼��������һλ
		Data <<= 1;                    
	}

	// ȷ���ӻ���Ӧ��
	return(I2C_Get_Ack_From_Slave());
}


/**
 * uint8_t I2C_Read_Byte(void)
 * ��飺�Ӵӻ���ȡһ���ֽ�
 * ���ܣ�
 *       �Ӵӻ���ȡһ���ֽڵ����ݡ�ͨ������ʱ�����족��֧�ֵ��ٴӻ�
 * ���룺��
 * �����
 *       ��ȡ�� I2C �ֽڣ���ʧ���򷵻� I2C_ERROR
 */
uint8_t I2C_Read_Byte(void)
{
	uint8_t result  = 0;  // ���ض�ȡ�� I2C �ֽ�
	uint8_t Bit = 0;      // λ������

	for(Bit = 0; Bit < 8; Bit++)
	{
		I2C_SDA = 1;           // �ͷ� SDA
		I2C_SCL = 1;           // �ͷ� SCL

		if (I2C_Sync_The_Clock() == I2C_ERROR)
		{
			return I2C_ERROR;     // ���� - ͬ��ʧ��
		}

		I2C_Delay(); 

		result <<= 1;      // �������

		if (I2C_SDA)
		{
			result |= 0x01;  // ���λ����Ϊʵ�ʵ� SDA ״̬
		}

		I2C_SCL = 0;       // ǿ��ʱ������
		I2C_Delay();       
	}
	
	return result;
}

/**
 * bit I2C_Sync_The_Clock(void)
 * ��飺I2C ʱ��ͬ��
 * ���ܣ�
 *       ͬ�� I2C ʱ�ӣ�I2C ���ݴ���ʹ�õĵײ㺯��
 * ���룺��
 * �����
 *       I2C_ERROR - ����ʱ��δͬ����
 *       I2C_OK    - �ɹ���ʱ��ͬ����
 */
bit I2C_Sync_The_Clock(void)
{
	uint16_t i;
	i = MAIN_Fosc / 13000;
	
	// ��ͼͬ��ʱ��
	while((I2C_SCL == 0) && (i != 0))
	{
		i--;
	}

	if (i == 0)
	{
		return I2C_ERROR;  // ���� - ��ʱ����������
	}

	return I2C_OK;       // �ɹ� - ʱ��ͬ��
}

/**
 * void I2C_Delay(void)
 * ��飺I2C ��ʱ
 * ���ܣ��϶̵�����ӳ٣�Ӧ����Ϊ 5.425 ΢��
 * ���룺��
 * �������
 */
void I2C_Delay(void)
{
	//@24MHz
	uint8_t i;
	_nop_();
	_nop_();
	_nop_();
	i = 50;
	while (--i);
}

#ifdef I2C_ACK_NACK

/**
 * void I2C_Send_Master_Ack(void)
 * ��飺���� ACK ״̬
 * ���룺��
 * �������
 */
void I2C_Send_Master_Ack(void)
{
  I2C_SDA = 0; 
  I2C_SCL = 1;

  I2C_Sync_The_Clock();

  I2C_Delay();  
  I2C_SCL = 0;  
}


/**
 * void I2C_Send_Master_Ack(void)
 * ��飺���� NOT ACK ״̬
 * ���룺��
 * �������
 */
void I2C_Send_Master_NAck(void)
{
	I2C_SDA = 1; 
	I2C_SCL = 1; 

	I2C_Sync_The_Clock();

	I2C_Delay();
	I2C_SCL = 0;
} 
#endif