 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	global_func mapper70init
	global_func mapper152init

@----------------------------------------------------------------------------
mapper70init:	@Saint Seiya ..
mapper152init:	@Saint Seiya ..
@----------------------------------------------------------------------------
	.word write152,write152,write152,write152

	movs r0,#1
	b_long mirror1_

@-------------------------------------------------------
write152:
@-------------------------------------------------------
	mov addy,r0,lsr#4
	stmfd sp!,{addy,lr}
	bl_long chr01234567_
	tst addy,#0x8
	bl_long mirror1_
	ldmfd sp!,{r0,lr}
	b_long map89AB_

@-------------------------------------------------------
	@.end
