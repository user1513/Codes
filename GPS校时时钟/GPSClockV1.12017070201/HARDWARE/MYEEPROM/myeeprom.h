#ifndef __MYEEPROM_H
#define __MYEEPROM_H
#include "sys.h"

void StorePassword(u16 password);
u16 ReadPassword(void);
void StorAlarmNumber(u8 nowAlarmnumber);
u8 ReadAlarmNumber(void);
void StoreAlarmData(u8 ArarmDataNo,u8 AlarmNumber,u8 data);
u8 ReadAlarmData(u8 ArarmDataNo,u8 AlarmNumber);
#endif
