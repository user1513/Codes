#ifndef _APP_H
#define _APP_H

#include "sys.h"
#include "led.h"
#include "delay.h"
#include "usart2.h"
#include "usart.h"
#include "ModbusCRC16.h"
#include "stdint.h"
#include <string.h>
#include "timer.h"
#include "tList.h"
#include "tool_test.h"
#include "stdio.h"
#include "SerialPortScreenFunc.h"
#include "DataParsing.h"
//默认ISUSE_MODBUS_CHECK = 1 启用CRC校验
//当ISUSE_MODBUS_CHECK = 0 不使用CRC校验
#define ISUSE_MODBUS_CHECK 1

//默认使用MODBUS_MODE = 0 使用查表法
//当MODBUS_MODE != 0 时使用计算法
#define MODBUS_MODE 0

//串口1接收最大字节数
#define UART1RX_LENMAX 100
//串口1时间比例
#define UART1TIME_PROPORTION 100









//串口2接收最大字节数
#define UART2RX_LENMAX 100

//串口2时间比例
#define UART2TIME_PROPORTION 5

//串口屏数据接收超时
#define RECEIVINGTIMEOUT 50

//串口屏数据接收次数
#define RECEIVINGTIMEOUT_NUM 3

//串口接收数据帧的长度
#define USART2_ACK_DATAFRAME_LENGTH 7

//串口接收数据帧的最大长度
#define USART2_DATAFRAME_LENGTH_MAX 12

//串口接收数据帧的最短长度
#define USART2_DATAFRAME_LENGTH_MIN 7

//数据帧头
#define DATA_FRAME_HEADER 0X0000A55A
#endif
