 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"

	global_func mapper184init

@----------------------------------------------------------------------------
mapper184init:
@----------------------------------------------------------------------------
	.word write184,write184,write184,write184

	adr r0,write184
	str_ r0,writemem_6
	.if PRG_BANK_SIZE == 4
	str_ r0,writemem_7
	.endif
	mov pc,lr
@-------------------------------------------------------
write184:
@-------------------------------------------------------
	mov addy,r0,lsr#4
	stmfd sp!,{addy,lr}
	bl_long chr0123_
	ldmfd sp!,{r0,lr}
	b_long chr4567_
@----------------------------------------------------------------------------
	@.end
