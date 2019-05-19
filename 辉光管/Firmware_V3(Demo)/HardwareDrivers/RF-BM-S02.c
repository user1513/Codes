/**
 * RF-BM-S02 ����
 * ģ���飺
 *     ����4.0ģ�飬��ͨ�������� MCU ͨ��
 * ���ߣ�    Blanboom
 * �����£�2014-07-20
 * http://blanboom.org
 ***************************************
 * ģ�� RX �ӵ�Ƭ�� P3.7��ģ�� TX �ӵ�Ƭ�� P3.6
 * ͬʱ��Ҫ�� "PinDef.h" �ж������� EN, BRTS �� BCTS������
 *         RF_BM_S02_EN
 *         RF_BM_S02_BRTS
 *         RF_BM_S02_BCTS
 * ʹ���˵�Ƭ���� UART1 �� ��ʱ�� 2
 * ���յ������ݴ洢�� UART1 �� Buffer �У������ "USART.h"
 */
 
#include "RF-BM-S02.h"

#include "delay.h"
#include "USART.h"
#include "PinDef.h"
#include "stdio.h"

/**
 * uint8_t RF_BM_S02_Init(void)
 * ��飺��ʼ�� RF_BM_S02 ���������Ӳ����Դ
 * ���ܣ���ʼ������ 1�� ������Ϊ 9600�����ö�ʱ�� 2 ��Ϊ�����ʷ�������ͬʱ��ʼ���������
 * ���룺��
 * �����
 *       RF_BM_S02_ERROR - ��ʼ������
 *       RF_BM_S02_OK    - ��ʼ���ɹ�
 */
uint8_t RF_BM_S02_Init(void)
{
	COMx_InitDefine		COMx_InitStructure;					
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;
	COMx_InitStructure.UART_BaudRate  = 115200ul;
	COMx_InitStructure.UART_RxEnable  = ENABLE;
	COMx_InitStructure.BaudRateDouble = DISABLE;
	COMx_InitStructure.UART_Interrupt = ENABLE;
	COMx_InitStructure.UART_Polity    = PolityHigh;
	COMx_InitStructure.UART_P_SW      = UART1_SW_P36_P37;
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;
	if(USART_Configuration(USART1, &COMx_InitStructure) != 0)
	{
		return RF_BM_S02_ERROR;
	}
	
	RF_BM_S02_EN   = 0;
	RF_BM_S02_BCTS = 1;
	RF_BM_S02_BRTS = 1;
	
	return RF_BM_S02_OK;
}