 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	global_func mapper40init
@	global_func mapper_40_hook

 countdown = mapperdata+0
 irqen = mapperdata+4
@----------------------------------------------------------------------------
mapper40init:		@SMB2j
@----------------------------------------------------------------------------
	.word write0,write1,void,mapCD_

	mov addy,lr
@	adr r0,mapper_40_hook
@	str_ r0,scanlinehook

	ldr r0,=rom_R60			@Set ROM at $6000-$7FFF.
	str_ r0,readmem_6
	.if PRG_BANK_SIZE == 4
	str_ r0,readmem_7
	.endif
	
	ldr r0,=empty_W			@ROM.
	str_ r0,writemem_6
	.if PRG_BANK_SIZE == 4
	str_ r0,writemem_7
	.endif

	bl write0

	mov r0,#-1
	bl_long map89ABCDEF_

	mov r0,#6
	mov lr,addy
	b_long map67_
@----------------------------------------------------------------------------
write0:		@$8000-$9FFF
@----------------------------------------------------------------------------
	mov r0,#36
	str_ r0,countdown
	mov r0,#0
	strb_ r0,irqen
	mov pc,lr
@----------------------------------------------------------------------------
write1:		@$A000-$BFFF
@----------------------------------------------------------------------------
	mov r0,#1
	strb_ r0,irqen
	mov pc,lr
@@----------------------------------------------------------------------------
@mapper_40_hook:
@@----------------------------------------------------------------------------
@	ldrb_ r0,irqen
@	cmp r0,#0
@	beq_long default_scanlinehook
@
@	ldr_ r0,countdown
@@	bmi default_scanlinehook
@	subs r0,r0,#1
@	str_ r0,countdown
@	bcs_long default_scanlinehook
@
@	mov r0,#0
@	strb_ r0,irqen
@@	b irq6502
@	b_long CheckI
@----------------------------------------------------------------------------
	@.end
