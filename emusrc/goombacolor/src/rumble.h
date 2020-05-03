#ifndef __RUMBLE_H__
#define __RUMBLE_H__

extern u32 SerialIn, DoRumble, RumbleCnt;
extern u16 stage, ind;
extern u16 SerOut0, SerOut1;

void RumbleInterrupt(void);
void StartRumbleComs(void);

#endif
