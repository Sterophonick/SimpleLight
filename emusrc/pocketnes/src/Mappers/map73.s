 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	global_func mapper73init
@	global_func mapper_73_hook

 counter = mapperdata
 irqen = mapperdata+4
@----------------------------------------------------------------------------
mapper73init:	@Konami Salamander (J)...
@----------------------------------------------------------------------------
	.word write8000,writeA000,writeC000,writeE000

@	adr r0,mapper_73_hook
@	str_ r0,scanlinehook

	mov pc,lr
@-------------------------------------------------------
write8000:
@-------------------------------------------------------
	ldr_ r2,counter
	and r0,r0,#0xF
	tst addy,#0x1000
	bne write9000
	bic r2,r2,#0xF0000
	orr r0,r2,r0,lsl#16
	str_ r0,counter
	mov pc,lr
write9000:
	bic r2,r2,#0xF00000
	orr r0,r2,r0,lsl#20
	str_ r0,counter
	mov pc,lr

@-------------------------------------------------------
writeA000:
@-------------------------------------------------------
	ldr_ r2,counter
	and r0,r0,#0xF
	tst addy,#0x1000
	bne writeB000
	bic r2,r2,#0xF000000
	orr r0,r2,r0,lsl#24
	str_ r0,counter
	mov pc,lr
writeB000:
	bic r2,r2,#0xF0000000
	orr r0,r2,r0,lsl#28
	str_ r0,counter
	mov pc,lr

@-------------------------------------------------------
writeC000:
@-------------------------------------------------------
	tst addy,#0x1000
	andeq r0,r0,#2
	streqb_ r0,irqen
	mov pc,lr

@-------------------------------------------------------
writeE000:
@-------------------------------------------------------
	tst addy,#0x1000
	bne_long map89AB_
	mov pc,lr
@@-------------------------------------------------------
@mapper_73_hook:
@@------------------------------------------------------
@	ldrb_ r0,irqen
@	cmp r0,#0	@timer active?
@	beq h1
@
@	ldr_ r0,counter
@@	adds r0,r0,#0x71aaab		@113.66667
@	adds r0,r0,#0x720000
@	bcc h0
@	mov r0,#0
@	strb_ r0,irqen
@	sub r0,r0,#0x10000
@	str_ r0,counter
@@	b irq6502
@	b_long CheckI
@h0:
@	str_ r0,counter
@h1:
@	fetch 0

@-------------------------------------------------------
	@.end
