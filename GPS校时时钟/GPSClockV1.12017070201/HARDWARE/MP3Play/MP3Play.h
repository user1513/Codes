#ifndef __MP3PLAY_H
#define __MP3PLAY_H

#include "sys.h"

void MP3_Init(u32 Bound);
void MP3_Play(u8 Song);
void MP3_SetVolume(u8 Volume);
void MP3_Stop(void);
 
#endif

