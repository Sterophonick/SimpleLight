 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"

	global_func mapper11init
	global_func mapper66init
@----------------------------------------------------------------------------
mapper11init:
@----------------------------------------------------------------------------
	.word write11,write11,write11,write11
	mov pc,lr
@----------------------------------------------------------------------------
mapper66init:
@----------------------------------------------------------------------------
	.word write66,write66,write66,write66

	ldr r1,mapper66init
	str_ r1,writemem_6
	.if PRG_BANK_SIZE == 4
	str_ r1,writemem_7
	.endif

	ldrb_ r0,cartflags
	orr r0,r0,#MIRROR	@???
	strb_ r0,cartflags

	mov pc,lr
@----------------------------------------------------------------------------
@ .align
@ .pool
@ .section .iwram, "ax", %progbits
@ .subsection 7
@ .align
@ .pool
@----------------------------------------------------------------------------
@------------------------------
write11:
@------------------------------
	stmfd sp!,{r0,lr}
	bl_long map89ABCDEF_
	ldmfd sp!,{r0,lr}
	mov r0,r0,lsr#4
	b_long chr01234567_
@------------------------------
write66:
@------------------------------
	stmfd sp!,{r0,lr}
	bl_long chr01234567_
	ldmfd sp!,{r0,lr}
	mov r0,r0,lsr#4
	b_long map89ABCDEF_
@----------------------------------------------------------------------------
	@.end
