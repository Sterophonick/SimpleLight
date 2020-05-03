 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"

	global_func mapper87init

@----------------------------------------------------------------------------
mapper87init:	@Konami - City Connection, Goonies...
@----------------------------------------------------------------------------
	.word chr01234567_rshift_,chr01234567_rshift_,chr01234567_rshift_,chr01234567_rshift_

	ldr r1,=chr01234567_rshift_
	str_ r1,writemem_6
	.if PRG_BANK_SIZE == 4
	str_ r1,writemem_7
	.endif

	mov pc,lr
	@.end
