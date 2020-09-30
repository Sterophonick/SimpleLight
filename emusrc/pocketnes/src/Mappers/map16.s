 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	global_func mapper16init
	global_func map16_handler

 counter = mapperdata+0  @uses two MSBytes
@ latch = mapperdata+4		@there is no latch!
 irqen = mapperdata+8
 counter_last_timestamp = mapperdata+12

 IRQ_ENABLE = 0x01
@IRQ counter ticks downwards, wraps at 0000->FFFF, and the IRQ triggers when the value reaches zero

@----------------------------------------------------------------------------
mapper16init:@		Bandai
@----------------------------------------------------------------------------
	.word write0,write0,write0,write0

	ldrb_ r1,cartflags		@get cartflags
	bic r1,r1,#SRAM			@don't use SRAM on this mapper
	strb_ r1,cartflags		@set cartflags
	ldr r1,mapper16init
	str_ r1,writemem_6
	.if PRG_BANK_SIZE == 4
	str_ r1,writemem_7
	.endif

@	ldr r0,=mapper_16_hook
@	str_ r0,scanlinehook
	mov pc,lr


@IRQ counter code
@FIXME still buggy

map16_handler:
	ldr_ r1,mapper_timestamp
	bl run_2
	b_long _GO

run_counter:
	@get timestamp
	ldr_ r2,cycles_to_run
0:
	sub r2,r2,cycles,asr#CYC_SHIFT
	ldr_ r1,timestamp
	add r1,r1,r2
run_2:
	add r1,r1,#12
	ldr_ r2,counter_last_timestamp
	str_ r1,counter_last_timestamp
	sub r2,r1,r2
	@r2 = elapsed cycles
	
	ldrb_ r0,irqen
	tst r0,#IRQ_ENABLE
	bxeq lr
	
	@divide time difference by 3
	ldr r0,=0x55555556 @1/3
	umull addy,r2,r0,r2
	
	ldr_ r0,counter
	subs r0,r0,r2,lsl#16
	str_ r0,counter
	
	@triggers if counter<=dt
	@jump ahead otherwise
	bhi 0f
	@verify that dt!=0
	movs r2,r2
	beq 0f
	
	stmfd sp!,{r0,r1,lr}
	ldr r0,=mapper_irq_handler
	adrl_ addy,mapper_irq_timeout
	bl_long replace_timeout_2
	ldmfd sp!,{r0,r1,lr}
	b 0f

find_next_irq:
	ldrb_ r0,irqen
find_next_irq2:
	tst r0,#IRQ_ENABLE
	bxeq lr

	ldr_ r2,cycles_to_run
	sub r2,r2,cycles,asr#CYC_SHIFT
	ldr_ r1,timestamp
	add r1,r1,r2
	add r1,r1,#12
	str_ r1,counter_last_timestamp
	
	ldr_ r0,counter
0:
	@get countdown until zero
	mov r0,r0,lsr#16
	adds r0,r0,r0,lsl#1
	moveq r0,#0x30000
	
	add r1,r1,r0
	sub r1,r1,#12
	adr r0,map16_handler
	adrl_ r12,mapper_timeout
	b_long replace_timeout_2

@@----------------------------------------------------------------------------
@ .align
@ .pool
@ .section .iwram, "ax", %progbits
@ .subsection 7
@ .align
@ .pool
@@----------------------------------------------------------------------------

@-------------------------------------------------------
write0:
@-------------------------------------------------------
	and addy,addy,#0x0f
	tst addy,#0x08
	ldreq r1,=writeCHRTBL
	adrne r1,tbl-8*4
	ldr pc,[r1,addy,lsl#2]
wA: @---------------------------
	@IRQ counter
	@.......x = enable
	@all writes acknowledge IRQs

	ldrb_ r1,wantirq
	bic r1,r1,#IRQ_MAPPER
	strb_ r1,wantirq
	
	@if going from enabled to disabled, run the counter
	tst r0,#1 @skip if want to enable
	bne 0f
	ldrb_ r1,irqen
	tst r1,#IRQ_ENABLE
	beq 0f   @skip if was disabled
	
	stmfd sp!,{r0,lr}
	bl run_counter
	ldmfd sp!,{r0,lr}
0:
	strb_ r0,irqen
	b find_next_irq2
	
wB: @---------------------------
	ldrb_ r1,irqen
	tst r1,#IRQ_ENABLE
	@if IRQ counter is not running, just set the counter
	bne 0f
	strb_ r0,counter+2
	bx lr
wC:
	ldrb_ r1,irqen
	tst r1,#IRQ_ENABLE
	bne 1f
	strb_ r0,counter+3
	bx lr
0:
	@setting low byte with counter running  (games don't do this)
	stmfd sp!,{r0,lr}
	bl run_counter
	ldmfd sp!,{r0,lr}
	strb_ r0,counter+2
	b find_next_irq
1:
	@setting high byte with counter running  (games don't do this)
	stmfd sp!,{r0,lr}
	bl run_counter
	ldmfd sp!,{r0,lr}
	strb_ r0,counter+3
	b find_next_irq

tbl: .word map89AB_,mirrorKonami_,wA,wB,wC,void,void,void

	@.end
