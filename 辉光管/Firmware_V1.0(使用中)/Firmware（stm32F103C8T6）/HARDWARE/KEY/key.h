#ifndef _KEY_H
#define _KEY_H

#include "main.h"


#define key1 PBin(5)
#define key2 PBin(6)
#define key3 PBin(7)

void key_io_config(void);
void Get_Key_Val(int * time);
void key_val_switch(u8 num, int * time, u8 mode);
void paracorrect(int * time);
extern u8 keystate[];



#endif

