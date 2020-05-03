#ifndef __SAVESTATE2_H__
#define __SAVESTATE2_H__

#ifdef __cplusplus
extern "C" {
#endif

#if !MOVIEPLAYER
void loadstate(int romnumber, u8* src, int statesize);
int savestate(u8 *dest);
#else
bool loadstate(const char *filename);
bool savestate(const char *filename);
#endif

#ifdef __cplusplus
}
#endif


//static __inline void dummy_f(int xxxx) { } 

#endif
