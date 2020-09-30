#ifndef __SPEEDHACK_H__
#define __SPEEDHACK_H__

#if !OLDSPEEDHACKS
#include "new_speed_hack.h"
#else

#if EDITBRANCHHACKS
void changehackmode(void);
void changebranchlength(void);
#endif

#if BRANCHHACKDETAIL
int getbranchhacknumber(void);
#endif

//u32*const speedhack_buffers[];
//const u8 hacktypes[];
//const int num_speedhack_buffers=9;
void autodetect_speedhack(void);
void find_best_speedhack(void);
void drawui4(void);

#endif

#endif
