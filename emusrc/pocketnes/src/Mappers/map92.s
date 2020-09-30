 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"

	global_func mapper92init

@----------------------------------------------------------------------------
mapper92init:	@Jaleco - Moero!! Pro Soccer, Moero!! Pro Yakyuu '88 Kettei Ban...
@----------------------------------------------------------------------------
	.word write0,write1,write1,write1

	mov pc,lr
@-------------------------------------------------------
write0:		@ Moero!! Pro Yakyuu '88
@-------------------------------------------------------
	tst addy,#0x1000
	bne write1
	and r0,addy,#0xff
	mov r1,r0,lsr#4
	cmp r1,#0xB
	beq_long mapCDEF_
	cmp r1,#0x7
	beq_long chr01234567_
	mov pc,lr
@-------------------------------------------------------
write1:		@ Moero!! Pro Soccer
@-------------------------------------------------------
	and r0,addy,#0xff
	mov r1,r0,lsr#4
	cmp r1,#0xD
	beq_long mapCDEF_
	cmp r1,#0xE
	beq_long chr01234567_
	mov pc,lr
@-------------------------------------------------------
	@.end
