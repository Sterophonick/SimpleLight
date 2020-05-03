 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"

	global_func mapper78init

@----------------------------------------------------------------------------
mapper78init:	@Holy Diver & Uchuusen - Cosmo Carrier (J)...
@----------------------------------------------------------------------------
	.word write78,write78,write78,write78

	mov pc,lr
@-------------------------------------------------------
write78:
@-------------------------------------------------------
	stmfd sp!,{r0,lr}
	bl_long map89AB_
	ldmfd sp,{r0}
	mov r0,r0,lsr#4
	bl_long chr01234567_
	ldmfd sp!,{r0,lr}
	and addy,addy,#0xFE00
	cmp addy,#0xFE00
	moveq pc,lr
	tst r0,#0x8
	b_long mirror1_


@-------------------------------------------------------
	@.end
