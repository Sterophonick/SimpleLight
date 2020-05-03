#ifndef __SAVESTATE_H__
#define __SAVESTATE_H__

#include "savestate2.h"

#if 0

extern const char saveversion[];
u8* dumpdata(u8* dest, u32 tag, int size, const void* data);
u8* dumpdata2(u8* dest, int tagid);
//typedef struct
//{
//	u8* src; u32 tag; int size;
//} loaddata_return_T;
//loaddata_return_T loaddata(u8* src);
int tag_search(u32 lookfor, const u32 array[], int arrsize);
u8* loadblock(u8* src, u32 tag, int size);
void loadstate(int romnumber, u8* src, int statesize);
int savestate(u8 *dest);

void prepare_variables_for_savestate(void);
void restore_variables_for_loadstate(void);
void restore_more_variables_for_loadstate(void);

#endif

#endif
