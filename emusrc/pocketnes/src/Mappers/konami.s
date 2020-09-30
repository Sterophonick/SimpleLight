 .align
 .pool
 .section .iwram, "ax", %progbits
 .subsection 0
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	.global KoLatch
	.global KoLatchLo
	.global KoLatchHi
	.global KoCounter
	.global KoIRQen   @actually IRQ acknowlege register
	global_func konami_handler
	
 latch = mapperdata+0   @IRQ counter reset value
 irqen = mapperdata+1   @IRQ enabled
 k4irq = mapperdata+2
 counter = mapperdata+3 @IRQ counter

 counter_last_timestamp = mapperdata+24
 prescale_fraction = mapperdata+28

konami_handler:
	ldr_ r2,mapper_timestamp
	bl run_counter_2
	b_long _GO
run_counter:
	@get timestamp
	ldr_ r1,cycles_to_run
	sub r1,r1,cycles,asr#CYC_SHIFT
	ldr_ r2,timestamp
	add r2,r2,r1
run_counter_2:
	add r2,r2,#12
	ldr_ r1,counter_last_timestamp
	str_ r2,counter_last_timestamp
	sub r1,r2,r1
	@r1 = elapsed cycles
	
	ldr_ r0,latch
	@are IRQs enabled?
	tst r0,#0x200
	bxeq lr
	tst r0,#0x040000
	bne 0f
	@scanline mode
	ldr r0,=0xC0300D  @1/341
	umull addy,r1,r0,r1
	ldr_ r0,prescale_fraction
	adds r0,r0,addy
	str_ r0,prescale_fraction
	adc r1,r1,#0
	@r1 = number of times to clock the counter
1:	
	ldr_ r0,latch	@32 bit number, MSByte is counter, LSByte is latch
	adds r0,r0,r1,lsl#24
	addcs r0,r0,r0,lsl#24
	str_ r0,latch
	bxcc lr
	@counter has overflowed if it reaches here, latch has already been assigned to counter
	@r2 = timestamp
	
	stmfd sp!,{r2,lr}
	mov r1,r2
	ldr r0,=mapper_irq_handler
	adrl_ r12,mapper_irq_timeout
	bl_long replace_timeout_2
	ldmfd sp!,{r1,lr}
	b find_next_irq_2
0:
	@cycle mode
	ldr r0,=0x55555556 @1/3
	umull addy,r1,r0,r1
	b 1b
find_next_irq:
	@get timestamp
	ldr_ r2,cycles_to_run
	sub r2,r2,cycles,asr#CYC_SHIFT
	ldr_ r1,timestamp
	add r1,r1,r2
	add r1,r1,#12
	str_ r1,counter_last_timestamp
find_next_irq_2:
	@r1 = timestamp now
	ldr_ r0,latch
	tst r0,#0x200
	bxeq lr
	tst r0,#0x040000
	mov r0,r0,lsr#24
	rsb r0,r0,#0x100
	bne 2f
	@we're in scanline mode
	@eliminate rounding errors
	ldr_ r2,prescale_fraction
	cmp r2,#0x00C00000
	bhi 0f
	mov r2,#0
	str_ r2,prescale_fraction
1:
	@we have no fraction
	@r0 = irq counter
	@r1 may be in the future now, that's what we want
	
	@multiply r0 by 341, that's what we add to r1.
	ldr r2,=341
	mla r1,r2,r0,r1
	@we have a timestamp now, r1 = time to trigger the timeout
3:
	sub r1,r1,#12
	adr r0,konami_handler
	adrl_ addy,mapper_timeout
	b_long replace_timeout_2
0:
	@we have a fraction
	ldr addy,=341
	rsb r2,r2,#0
	umull r2,r0,addy,r2
	add r1,r1,r0
	ldrb_ r0,counter
	rsb r0,r0,#0xFF
	b 1b
2:
	@we're in CPU cycle mode
	@r0 = number of CPU cycles until the IRQ hits
	@multiply by 3 to get PPU cycles
	add r0,r0,r0,lsl#1
	add r1,r1,r0
	b 3b

@-------------------------------------------------------
KoLatch: @- - - - - - - - - - - - - - -
	strb_ r0,latch
	mov pc,lr
KoLatchLo: @- - - - - - - - - - - - - - -
	and r2,r2,#0xf0
	and r0,r0,#0x0f
	orr r0,r0,r2
	strb_ r0,latch
	mov pc,lr
KoLatchHi: @- - - - - - - - - - - - - - -
	and r2,r2,#0x0f
	orr r0,r2,r0,lsl#4
	strb_ r0,latch
	mov pc,lr
KoCounter: @- - - - - - - - - - - - - - -
	@if IRQ was disabled, don't run IRQ counter
	ldr_ r1,latch
	tst r1,#0x0200
	beq 0f
	@Otherwise, run the IRQ counter if Enable/Disable changes, or Mode changes
	ldrb_ r2,k4irq
	eor r2,r2,r0
	tst r2,#6
	beq 0f
	stmfd sp!,{r0,r1,lr}
	bl run_counter
	ldmfd sp!,{r0,r1,lr}
0:
	@Acknowledge IRQ
	ldrb_ r2,wantirq
	bic r2,r2,#IRQ_MAPPER
	strb_ r2,wantirq
	
	bic r1,r1,#0x00FF0000
	orr r1,r1,r0,lsl#16
	tst r0,#2
	orrne r1,r1,#0x00000200
	streq_ r1,latch
	bxeq lr
	@copy latch to counter
	bic r1,r1,#0xFF000000
	orr r1,r1,r1,lsl#24
	str_ r1,latch
	mov r1,#0
	str_ r1,prescale_fraction  @reset prescaler
	
	b find_next_irq

KoIRQen: @- - - - - - - - - - - - - - -
	ldrb_ r0,wantirq
	bic r0,r0,#IRQ_MAPPER
	strb_ r0,wantirq
	
	ldr_ r0,latch
	@if IRQ is enabled, and this operation disables IRQ, run IRQ counter
	tst r0,#0x00000200
	beq 0f
	tst r0,#0x00010000
	bxne lr  @irq is still enabled, so we can just return now
	stmfd sp!,{lr}
	bl run_counter
	ldmfd sp!,{lr}
	mov r0,#0
	strb_ r0,irqen
	bx lr
0:
	tst r0,#0x00010000
	@if IRQ should be enabled, enable it.
	mov r0,#0x02
	strb_ r0,irqen
	b find_next_irq
	bx lr

	@.end
