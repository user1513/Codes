#ifndef __FIRMWAREDISPLAY_H
#define __FIRMWAREDISPLAY_H

#include "main.h"

#define MAX6921_DIN  PBout(14)
#define MAX6921_CLK   PBout(12)
#define MAX6921_LOAD  PBout(13)
#define MAX6921_BLANK  PBout(15)

//defination of pin actions
#define DIN_HIGH    MAX6921_DIN = 1
#define DIN_LOW     MAX6921_DIN = 0
#define CLK_HIGH    MAX6921_CLK = 1
#define CLK_LOW     MAX6921_CLK = 0
#define BLANK_HIGH  MAX6921_BLANK = 1
#define BLANK_LOW   MAX6921_BLANK = 0
#define LOAD_HIGH   MAX6921_LOAD = 1
#define LOAD_LOW    MAX6921_LOAD = 0

void FirmwarePeriphCfg(void);
void displayInit(void);
void displayNumber(int,int);
void displayTime(int * time, int num);
void FirmwareShift(u8 digit,int *time, bool state);
#endif


