#ifndef __NEW_SPEED_HACK_H__
#define __NEW_SPEED_HACK_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	const u8 *hack_pc;
	u8 num_incs;
	u8 hack_was_used;
	u8 frames_hack_not_used;
	u8 __;
	u32 cycles_per_iteration;
	u32 divider;
} speedhack_T;

void speedhack_manager(const u8* initpc, const u8* lastbank, int hacknum);
//bool quickhackfinder(const u8 *initpc, const u8 *lastbank, int hacknum);

#ifdef __cplusplus
}
#endif

#endif
