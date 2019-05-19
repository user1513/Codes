/**
 * SD2405ALPI ����
 * оƬ��飺
 *       ���õ�غ;���� I2C ���ߵ�ʵʱʱ�ӣ������� 8421BCD �����ʽ����
 * ���ߣ�hhelib
 * �����£�2016-03-13
 *********************************************
 * ʹ�ñ�����ʱ����Ҫ�� PinDef.h ��
 * ���� I2C_SDA �� I2C_SCL ���� I2C ����
 */
 
#include "SD2405ALPI.h"
#include "delay.h"
#include "Soft_I2C.h"

/**
 * uint32_t SD2405ALPI_GetTime(void)
 * ��飺�� SD2405ALPI ��ȡ��ǰʱ��
 * ���룺��
 * �������ǰʱ�䣨HHMMSS��
 */
uint32_t SD2405ALPI_GetTime(void)
{
	uint32_t time = 0;
	uint8_t tmp = 0;
	// ������Ҫ��ȡ�ĵ�ַ
  // I2C_Send_Start();
  // I2C_Write_Byte(SD2405ALPI_DEVICE_ADDRESS | I2C_WRITE);
  // I2C_Write_Byte(SD2405ALPI_SECOND_ADDRESS);
	
	// ��ʼ��ȡ����
	I2C_Send_Start();
	I2C_Write_Byte(SD2405ALPI_DEVICE_ADDRESS | I2C_READ);
	
	tmp = I2C_Read_Byte();
	time += (tmp & 0x0f) + ((tmp & 0xf0) >> 4) * 10;
	I2C_Send_Master_Ack();
	
	tmp = I2C_Read_Byte();
	time += ((tmp & 0x0f) + ((tmp & 0xf0) >> 4) * 10) * 100;
	I2C_Send_Master_Ack();
	
	tmp = I2C_Read_Byte();
	time += ((tmp & 0x0f) + ((tmp & 0x70) >> 4) * 10) * 10000ul;
	
	// ֹͣ
	I2C_Send_Master_Ack();
	I2C_Send_Stop();
	
	return time;
}

/**
 * void SD2405ALPI_SetTime(uint32_t time)
 * ��飺���� SD2405ALPI �ڲ�ʱ��
 * ���룺��ǰʱ�䣨HHMMSS��
 * �������
 */
void SD2405ALPI_SetTime(uint32_t time)
{
	// д����
	I2C_Send_Start();
	I2C_Write_Byte(SD2405ALPI_DEVICE_ADDRESS | I2C_WRITE);
	I2C_Write_Byte(SD2405ALPI_CTR2_ADDRESS);
	I2C_Write_Byte(0x80);
	I2C_Send_Stop();
	I2C_Send_Start();
	I2C_Write_Byte(SD2405ALPI_DEVICE_ADDRESS | I2C_WRITE);
	I2C_Write_Byte(SD2405ALPI_CTR1_ADDRESS);
	I2C_Write_Byte(0x84);
	I2C_Send_Stop();
	
	
	// д��ʱ��
	I2C_Send_Start();
	I2C_Write_Byte(SD2405ALPI_DEVICE_ADDRESS | I2C_WRITE);
	I2C_Write_Byte(SD2405ALPI_SECOND_ADDRESS);
	I2C_Write_Byte((time % 100) / 10 | (time % 10) << 4);
	I2C_Write_Byte((time % 1000) / 100 | ((time % 10000) / 1000) << 4);
	I2C_Write_Byte((time % 100000ul) / 10000 | ((time / 100000ul)) << 4 | 0x80);
	I2C_Send_Stop();
	
	
	// д��ֹ
	I2C_Send_Start();
	I2C_Write_Byte(SD2405ALPI_DEVICE_ADDRESS | I2C_WRITE);
	I2C_Write_Byte(SD2405ALPI_CTR1_ADDRESS);
	I2C_Write_Byte(0x00);
	I2C_Send_Stop();
	I2C_Send_Start();
	I2C_Write_Byte(SD2405ALPI_DEVICE_ADDRESS | I2C_WRITE);
	I2C_Write_Byte(SD2405ALPI_CTR2_ADDRESS);
	I2C_Write_Byte(0x00);
	I2C_Send_Stop();
}