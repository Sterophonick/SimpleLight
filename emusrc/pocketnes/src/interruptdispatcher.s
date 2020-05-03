#include "equates.h"

@based on interruptdispatcher.s from libgba, but this has been rewritten
@uses log-2 to find out which IRQ handler to execute instead of the key-value list
@---------------------------------------------------------------------------------

	.text
	.extern	IntrTable
	.code 32

jumpToSerialInterrupt:
	add r12,r0,#REG_IE
	strh r2,[r12,#2]
	b serialinterrupt

	.section	.iwram,"ax",%progbits
	global_func IntrMain

@---------------------------------------------------------------------------------
IntrMain:
@---------------------------------------------------------------------------------
@	mov	r0, #0x4000000		@ REG_BASE
	
	@r0 = REG_BASE from BIOS irq handler
	ldr r1, [r0,#REG_IE]
	ands r3,r1,r1,lsr#16
	@r3 = IE & IF
	bxeq lr	@return if there's no interrupts to handle
IntrMain2:
	@mix with BIOS irq flags so vblank waiting works
	ldrh r2,[r0,#-8]
	orr r2,r2,r3
	strh r2,[r0,#-8]
	
	@check for serial interrupt (highest priority)
	ands r2,r3,#0x80
	bne_long jumpToSerialInterrupt
	
	@get bit number of interrupt - this takes log base 2 of r3 into r2. (destroying r3)
	@mov r2,#0
	tst r3,#0xFF00
	movne r3,r3,lsr#8
	addne r2,r2,#8
	tst r3,#0xF0
	movne r3,r3,lsr#4
	addne r2,r2,#4
	tst r3,#0x0C
	movne r3,r3,lsr#2
	addne r2,r2,#2
	tst r3,#0x02
	addne r2,r2,#1
	@r2 = bit number of the IRQ handler
	
	mov r3,#0x1
	mov r3,r3,lsl r2
	@clear IF bit for this interrupt
	add r12,r0,#REG_IE
	strh r3,[r12,#2]

TriggerInterruptHandler:
	ldr r12,=IntrTable
	ldr r12,[r12,r2,lsl#2]
	@r12 = address to jump to
	movs r12,r12
	@look for a different interrupt if there's no handler
	beq IntrMain
	
	tst r1,#0xE600  @is it high priority?  (hblank, vcount, timer2, timer3, serial)
	bxne r12        @Just jump to the handler without allowing nesting  
	                @(Handler runs in IRQ mode with interrupts disabled, not system mode, so it uses the IRQ stack for any pushes/pops)
	
	ldr r1,[r0,#REG_IME]
	str r0,[r0,#REG_IME]
	mrs r3,spsr
	
	stmfd sp!,{r0,r1,r3,lr}
	mrs r1,cpsr		@enter System mode
	bic r1,r1,#0xDF
	orr r1,r1,#0x1F
	msr cpsr,r1
	
	@push user link register
	stmfd sp!,{lr}
	adr lr, IntrRet	@return address
	bx r12			@jump to IRQ handler
IntrRet:
	ldmfd sp!,{lr}
	
	mrs r1,cpsr		@back to IRQ mode
	bic r1,r1,#0xDF
	orr r1,r1,#0x92
	msr cpsr,r1
	ldmfd sp!,{r0,r1,r3,lr}
	
	str r1,[r0,#REG_IME]
	msr spsr,r3
	
	@more interrupts?  Go again
	ldr r1,[r0,#REG_IE]
	ands r3,r1,r1,lsr#16
	bne IntrMain2
	
	@did we miss a vcount interrupt? (never seen this happen since adding the priority system)
	ldr r1,[r0,#REG_DISPSTAT]
	tst r1,#0x20
	bxeq lr
	
	mov r2,r1,lsr#16
	mov r1,r1,lsr#8
	and r1,r1,#0xFF
	cmp r1,#160
	bxge lr
	cmp r2,#160
	bxge lr
	cmp r2,r1 @current scanline > compare value?
	bxlt lr
	mov r11,r11
	mov r2,#2
	b TriggerInterruptHandler

	.pool
	.end
