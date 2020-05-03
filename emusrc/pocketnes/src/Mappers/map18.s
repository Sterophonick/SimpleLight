 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	global_func mapper18init
	global_func map18_handler

 prg_xx = mapperdata+0 @4 bytes
 chr_xx = mapperdata+4 @8 bytes
 latch = mapperdata+12
 counter = mapperdata+16  @number is NOT shifted 16 to the left
 irqen = mapperdata+20

 counter_last_timestamp = mapperdata+24

 IRQ_ENABLE = 0x01

@IRQ counts down, and wraps at 0000->FFFF.  IRQ triggered when it WRAPS.


@IRQs seem to work OK, but the non-16 width stuff is untested


@----------------------------------------------------------------------------
mapper18init:	@Jaleco SS8806..
@----------------------------------------------------------------------------
	.word write8000,writeA000,writeC000,writeE000

@	adr r0,mapper_18_hook
@	str_ r0,scanlinehook

	mov pc,lr

irq_shift_table:
	.byte 16, 20, 24, 24, 28, 28, 28, 28




@when to "run counter" on mapper 18:
@on writes to F001
@when timeout happens
@
@when to "find next irq" on mapper 18:
@on writes to F000 (if IRQs are enabled)
@on writes to F001 (if IRQs are enabled afterwards)

map18_handler:
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
	
	@divide time difference by 3
	ldr r0,=0x55555556 @1/3
	umull addy,r2,r0,r2
	
	ldrb_ addy,irqen
	tst addy,#IRQ_ENABLE
	bxeq lr
	
	ldr_ r0,counter
	
	@find width of IRQ counter (normally is zero for full width)
	tst addy,#0x0E
	bne not_16_wide
	
	mov r0,r0,lsl#16
	subs r0,r0,r2,lsl#16
	mov r0,r0,lsr#16
	str_ r0,counter
	
	@triggers if counter<to_run
	@skip ahead if counter >= to run
	bcs 0f
1:
	stmfd sp!,{r0,r1,lr}
	ldr r0,=mapper_irq_handler
	adrl_ addy,mapper_irq_timeout
	bl_long replace_timeout_2
	ldmfd sp!,{r0,r1,lr}
	b 0f
not_16_wide:
	adr r1,irq_shift_table
	ldrb addy,[r1,addy,lsr#1]
	mov r1,r0,lsl addy
	subs r1,r1,r2,lsl addy
	@4 wide:
	@0000 0000 0000 0000 xxxx xxxx xxxx xxxx  counter
	@1111 1111 1111 1111 .... .... .... ....  bit clear mask
	@xxxx .... .... .... .... .... .... ....  counter shifted left 28
	@.... .... .... .... .... .... .... 1111  bit clear mask shifted right 28
	mov r2,#0xFFFFFFFF
	bic r0,r0,r2,lsr addy
	orr r0,r0,r1,lsr addy
	str_ r0,counter
	
	ldr_ r1,counter_last_timestamp
	bcc 1b
	and r0,r0,r2,lsr addy
	b 0f

find_next_irq:
	ldrb_ addy,irqen
	tst addy,#IRQ_ENABLE
	bxeq lr

	ldr_ r2,cycles_to_run
	sub r2,r2,cycles,asr#CYC_SHIFT
	ldr_ r1,timestamp
	add r1,r1,r2
	add r1,r1,#12
	str_ r1,counter_last_timestamp
	
	ldr_ r0,counter
	
	tst addy,#0x0E
	bne findnext_not16
0:
	@get countdown until wrap
	add r0,r0,#1  @add 1 so it can exceed zero instead of reaching zero
	adds r0,r0,r0,lsl#1
	
	add r1,r1,r0
	sub r1,r1,#12
	adr r0,map18_handler
	adrl_ addy,mapper_timeout
	b_long replace_timeout_2
findnext_not16:
	adr r2,irq_shift_table
	ldrb addy,[r2,addy,lsr#1]
	mov r2,#0xFFFFFFFF
	and r0,r0,r2,lsr addy
	b 0b
	

@-------------------------------------------------------
write8000:
@-------------------------------------------------------
writeA000:
@-------------------------------------------------------
writeC000:	@addy=A/B/C/Dxxx
@-------------------------------------------------------
	and r1,addy,#3
	and addy,addy,#0x7000
	orr r1,r1,addy,lsr#10
	movs r1,r1,lsr#1

	adrl_ addy,prg_xx
	and r0,r0,#0xF
	ldrb r2,[addy,r1]

	andcc r2,r2,#0xF0
	orrcc r0,r2,r0
	andcs r2,r2,#0xF
	orrcs r0,r2,r0,lsl#4
	strb r0,[addy,r1]

	cmp r1,#4
	ldrge addy,=writeCHRTBL-4*4
	adrlo addy,write8tbl
	ldr pc,[addy,r1,lsl#2]


write8tbl: .word map89_,mapAB_,mapCD_,void
@-------------------------------------------------------
writeE000:
@-------------------------------------------------------
	and r1,addy,#3
	tst addy,#0x1000
	orrne r1,r1,#4

	and r0,r0,#0xF
	ldr_ r2,latch
	adr addy,writeFtbl
	ldr pc,[addy,r1,lsl#2]

wE0: @- - - - - - - - - - - - - - -
	bic r2,r2,#0xF
	orr r0,r2,r0
	str_ r0,latch
	mov pc,lr
wE1: @- - - - - - - - - - - - - - -
	bic r2,r2,#0xF0
	orr r0,r2,r0,lsl#4
	str_ r0,latch
	mov pc,lr
wE2: @- - - - - - - - - - - - - - -
	bic r2,r2,#0xF00
	orr r0,r2,r0,lsl#8
	str_ r0,latch
	mov pc,lr
wE3: @- - - - - - - - - - - - - - -
	bic r2,r2,#0xF000
	orr r0,r2,r0,lsl#12
	str_ r0,latch
	mov pc,lr
wF0: @- - - - - - - - - - - - - - -
	str_ r2,counter
0:
	ldrb_ r1,wantirq
	bic r1,r1,#IRQ_MAPPER
	strb_ r1,wantirq
	b find_next_irq
wF1: @- - - - - - - - - - - - - - -
	ldrb_ r1,irqen
	tst r1,#IRQ_ENABLE
	beq 1f
	stmfd sp!,{r0,lr}
	bl run_counter
	ldmfd sp!,{r0,lr}
1:	
	strb_ r0,irqen
	b 0b
wF2: @- - - - - - - - - - - - - - -
	movs r1,r0,lsr#2
	tst r0,#1
	bcc_long mirror2H_
	bcs_long mirror1_

writeFtbl: .word wE0,wE1,wE2,wE3,wF0,wF1,wF2,void
@@-------------------------------------------------------
@mapper_18_hook:
@@------------------------------------------------------
@	ldrb_ r0,irqen
@	cmp r0,#0	@timer active?
@	beq h1
@
@	ldr_ r0,counter
@	cmp r0,#0	@timer active?
@	beq h1
@	subs r0,r0,#113		@counter-A
@	bhi h0
@
@	mov r0,#0
@	str_ r0,counter	@clear counter and IRQenable.
@	strb_ r0,irqen
@@	b irq6502
@	b_long CheckI
@h0:
@	str_ r0,counter
@h1:
	fetch 0
@-------------------------------------------------------
	@.end
