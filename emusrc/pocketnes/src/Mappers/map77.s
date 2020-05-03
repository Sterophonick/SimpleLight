 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"

	global_func mapper77init

@----------------------------------------------------------------------------
mapper77init:@	Irem mapper something...
@----------------------------------------------------------------------------
	.word write0,write0,write0,write0

	mov r0,#0
	mov addy,lr
	bl_long chr01234567_
	mov lr,addy
	mov r0,#0
	b_long map89ABCDEF_
@----------------------------------------------------------------------------
write0:
@----------------------------------------------------------------------------

	stmfd sp!,{r0,lr}
	mov r0,r0,lsr#4
	bl_long chr01_
	ldmfd sp!,{r0,lr}
	b_long map89ABCDEF_

@----------------------------------------------------------------------------
	@.end
