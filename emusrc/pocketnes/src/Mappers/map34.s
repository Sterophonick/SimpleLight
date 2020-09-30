 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"

	global_func mapper34init

@----------------------------------------------------------------------------
mapper34init:	@Impossible Mission 2 & Deadly Towers
@----------------------------------------------------------------------------
	.word map89ABCDEF_,map89ABCDEF_,map89ABCDEF_,map89ABCDEF_		@ Deadly Towers

	adr r1,write0
	str_ r1,writemem_6
	.if PRG_BANK_SIZE == 4
	str_ r1,writemem_7
	.endif

	mov r0,#0
	b_long map89ABCDEF_
@-------------------------------------------------------
write0:			@Impossible Mission 2
@-------------------------------------------------------
	ldr r1,=0x7fff
	cmp addy,r1		@7FFF
	beq_long chr4567_
	sub r1,r1,#1
	cmp addy,r1		@7FFE
	beq_long chr0123_
	sub r1,r1,#1
	cmp addy,r1		@7FFD
	beq_long map89ABCDEF_
	b_long sram_W
@-------------------------------------------------------
	@.end
