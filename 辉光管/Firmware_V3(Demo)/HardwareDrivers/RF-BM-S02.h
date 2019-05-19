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
 
#ifndef __RF_BM_S02_H
#define __RF_BM_S02_H

#include "config.h"

#define RF_BM_S02_OK      0
#define RF_BM_S02_ERROR   1
#define RF_BM_S02_NO_DATA 2

uint8_t RF_BM_S02_Init(void);


#endif