 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"

	global_func mapper93init

@----------------------------------------------------------------------------
mapper93init:	@Fantazy Zone (J)
@----------------------------------------------------------------------------
	.word write93,write93,write93,write93

	mov pc,lr
@-------------------------------------------------------
write93:
@-------------------------------------------------------
	stmfd sp!,{r0,lr}
	tst r0,#1
	bl_long mirror2V_
	ldmfd sp!,{r0,lr}
	mov r0,r0,lsr#4
	b_long map89AB_
@-------------------------------------------------------
	@.end
