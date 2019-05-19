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
 
#ifndef __SD2405ALPI_H
#define __SD2405ALPI_H

#include "config.h"

/**
 * SD2405ALPI �豸��ַ
 * ���һλΪ 0��ʹ��ʱҪ�� I2C_READ �� I2C_WRITE ���а�λ������
 */
#define SD2405ALPI_DEVICE_ADDRESS 0x64

// SD2405ALPI �ڲ��Ĵ�����ַ
// ʵʱʱ�ӼĴ���
#define SD2405ALPI_SECOND_ADDRESS 0x00
#define SD2405ALPI_MINUTE_ADDRESS 0x01
#define SD2405ALPI_HOUR_ADDRESS   0x02
#define SD2405ALPI_WEEK_ADDRESS   0x03
#define SD2405ALPI_DAY_ADDRESS    0x04
#define SD2405ALPI_MONTH_ADDRESS  0x05
#define SD2405ALPI_YEAR_ADDRESS   0x06
// ʱ�䱨���Ĵ���
#define SD2405ALPI_SECOND_ALARM_ADDRESS 0x07
#define SD2405ALPI_MINUTE_ALARM_ADDRESS 0x08
#define SD2405ALPI_HOUR_ALARM_ADDRESS   0x09
#define SD2405ALPI_WEEK_ALARM_ADDRESS   0x0A
#define SD2405ALPI_DAY_ALARM_ADDRESS    0x0B
#define SD2405ALPI_MONTH_ALARM_ADDRESS  0x0C
#define SD2405ALPI_YEAR_ALARM_ADDRESS   0x0D
#define SD2405ALPI_ALARM_ENABLE_ADDRESS 0x0E
// ���ƼĴ���
#define SD2405ALPI_CTR1_ADDRESS            0x0F
#define SD2405ALPI_CTR2_ADDRESS            0x10
#define SD2405ALPI_CTR3_ADDRESS            0x11
#define SD2405ALPI_TIME_SETTING_ADDRESS    0x12
#define SD2405ALPI_COUNTING_DOWN_ADDRESS   0x13


uint32_t SD2405ALPI_GetTime(void);
void  SD2405ALPI_SetTime(uint32_t);

#endif