#ifndef _DS3231_H
#define _DS3231_H

#include "sys.h"
#include "myiic.h"

#define  DS3231WriteAddress 0xd0  
#define  DS3231ReadAddress  0xd1
#define  DS3231_Second      0x00  
#define  DS3231_TimeFirst   0x00 
#define  DS3231_Minute      0x01 
#define  DS3231_Hour        0x02 
#define  DS3231_Week        0x03 
#define  DS3231_Day         0x04 
#define  DS3231_Month       0x05 
#define  DS3231_Year        0x06 
#define  DS3231_Interrupt   0x0e 
#define  DS3231_Status      0x0f 

void Ds3231_Init(void);
u8 DS3231_ReadOneByteFromE2PROM(void);
void Ds3231_Write_Cmd(u8 slave_addr,u8 word_addr);
void Ds3231_Read_Data(u8 slave_addr);
void DS3231_Initialization(void);
void DS3231_SetTime(int *time_data);
void DS3231_GetTime(int *GetTime);
extern int SetTime[];//秒-分-时/日期/日-月-年
extern int GetTime[];//秒-分-时/日期/日-月-年
#endif






