 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	global_func mapper79init

@----------------------------------------------------------------------------
mapper79init:
@----------------------------------------------------------------------------
	.word chr01234567_,chr01234567_,chr01234567_,chr01234567_

	adr r1,write0
	str_ r1,writemem_4
	.if PRG_BANK_SIZE == 4
	str_ r1,writemem_5
	.endif

	mov r0,#0xff
	b_long map89ABCDEF_
@-------------------------------------------------------
write0:
@-------------------------------------------------------
	cmp addy,#0x4100
	blo_long IO_W

	and r1,addy,#0xE100
	cmp r1,#0x4100
	movne pc,lr

	stmfd sp!,{r0,lr}
	mov r0,r0,lsr#3
	bl_long map89ABCDEF_
	ldmfd sp!,{r0,lr}
	b_long chr01234567_

@-------------------------------------------------------
	@.end
