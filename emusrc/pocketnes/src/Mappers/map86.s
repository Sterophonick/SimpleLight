 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"

	global_func mapper86init

@----------------------------------------------------------------------------
mapper86init:	@Jaleco - Moero!! Pro Yakyuu, Urusei Yatsura...
@----------------------------------------------------------------------------
	.word void,void,void,void
	adr r1,write86
	str_ r1,writemem_6
	.if PRG_BANK_SIZE == 4
	str_ r1,writemem_7
	.endif
	mov r0,#0
	b_long map89ABCDEF_
@-------------------------------------------------------
write86:
@-------------------------------------------------------
	cmp addy,#0x6000
	movne pc,lr
	stmfd sp!,{r0,lr}
	mov r0,r0,lsr#4
	bl_long map89ABCDEF_
	ldmfd sp!,{r0,lr}
	and r1,r0,#0x40
	and r0,r0,#0x03
	orr r0,r0,r1,lsr#4
	b_long chr01234567_


@-------------------------------------------------------
	@.end
