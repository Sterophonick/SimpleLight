 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	global_func mapper19init
	global_func mapper210init
@	global_func mapper_19_hook
	global_func map19_irq_handler
	global_func map19_handler

 counter = mapperdata+0
 irqen = mapperdata+4
 mirror = mapperdata+8
 counter_last_timestamp = mapperdata+16

 IRQ_ENABLE = 0x80

@IRQ increments to 7FFF, then stops there, repeatedly triggering IRQs until someone stops it

@IRQs may be buggy, not sure


@----------------------------------------------------------------------------
mapper210init:
@----------------------------------------------------------------------------
	.word map19_8,map19_A,void,map19_E
	b 0f

@----------------------------------------------------------------------------
mapper19init:
@----------------------------------------------------------------------------
	.word map19_8,map19_A,map19_C,map19_E
0:
	adr r1,write0
	str_ r1,writemem_4
	.if PRG_BANK_SIZE == 4
	str_ r1,writemem_5
	.endif

	adr r1,map19_r
	str_ r1,readmem_4
	.if PRG_BANK_SIZE == 4
	str_ r1,readmem_5
	.endif


@	adr r0,mapper_19_hook
@	str_ r0,scanlinehook

	mov pc,lr
@----------------------------------------------------------------------------

map19_irq_handler:
	ldr_ r0,counter
	ldr r1,=0x7FFF
	cmp r0,r1
	bne_long _GO
	
	ldrb_ r0,wantirq
	orr r0,r0,#IRQ_MAPPER
	strb_ r0,wantirq
	b_long CheckI




map19_handler:
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

	ldrb_ r0,irqen
	tst r0,#IRQ_ENABLE
	bxeq lr

	sub r2,r1,r2
	@r2 = elapsed cycles
	
	@divide time difference by 3
	ldr r0,=0x55555556 @1/3
	umull addy,r2,r0,r2
	
	ldr_ r0,counter
	add r0,r0,r2
	cmp r0,#0x8000
	strlt_ r0,counter
	blt 0f
	ldr r0,=0x7FFF
	str_ r0,counter
	
	adr r0,map19_irq_handler
	adrl_ addy,mapper_irq_timeout
	b_long replace_timeout_2
	
find_next_irq:
	ldrb_ r0,irqen
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
	@get count up until 0x8000
	rsb r0,r0,#0x8000
	adds r0,r0,r0,lsl#1
	
	add r1,r1,r0
	sub r1,r1,#12
	adr r0,map19_handler
	adrl_ addy,mapper_timeout
	b_long replace_timeout_2

@-------------------------------------------------------
write0:
	@4800 is sound registers
	
	cmp addy,#0x5000
	blo_long IO_W

	@run counter
	stmfd sp!,{r0,addy,lr}
	bl run_counter
	ldmfd sp!,{r0,addy,lr}
	
	@all reads and writes clear IRQ flag
	ldrb_ r1,wantirq
	bic r1,r1,#IRQ_MAPPER
	strb_ r1,wantirq
	
	and r1,addy,#0x7800
	cmp r1,#0x5000
	streqb_ r0,counter+0
	moveq pc,lr

	cmp r1,#0x5800
	movne pc,lr
	and r1,r0,#0x7F
	strb_ r1,counter+1
	and r0,r0,#0x80
	strb_ r0,irqen
	b find_next_irq
@-------------------------------------------------------
map19_r:
	@4800 is sound registers

	cmp addy,#0x5000
	blo_long IO_R

	@run counter
	stmfd sp!,{r0,addy,lr}
	bl run_counter
	ldmfd sp!,{r0,addy,lr}
	
	@all reads and writes clear IRQ flag
	ldrb_ r1,wantirq
	bic r1,r1,#IRQ_MAPPER
	strb_ r1,wantirq
	
	and r1,addy,#0x7800

	cmp r1,#0x5000
	ldreqb_ r0,counter+0
	moveq pc,lr

	cmp r1,#0x5800
	bxne lr
	ldrb_ r0,counter+1
	ldrb_ r1,irqen
	orr r0,r0,r1
	bx lr

@-------------------------------------------------------
map19_8:
map19_A:
	and r1,addy,#0x7800
	ldr r2,=writeCHRTBL
	ldr pc,[r2,r1,lsr#9]
map19_C:
	@ Do NameTable RAMROM change, for mirroring.
	
	@not yet emulated, let's just fake it
	and r2,addy,#0x1800
	adrl_ r1,mirror
	strb r0,[r1,r2,lsr#11]
	
	ldr_ r0,mirror
	ldr r1,=0x01010101
	ands r0,r0,r1
	beq_long mirror1_
	tst r0,#0x00000001
	bne_long mirror1_
	tst r0,#0x00000100
	bne_long mirror2H_
	tst r0,#0x00010000
	b_long mirror2V_
@-------------------------------------------------------

@-------------------------------------------------------
map19_E:
@-------------------------------------------------------
	and r1,addy,#0x7800
	cmp r1,#0x6000
	beq_long map89_
	cmp r1,#0x6800
	beq_long mapAB_
	cmp r1,#0x7000
	beq_long mapCD_
	mov pc,lr

@@-------------------------------------------------------
@mapper_19_hook:
@@------------------------------------------------------
@	ldrb_ r0,irqen
@	cmp r0,#0
@	beq h1
@
@	ldr_ r0,counter
@@	adds r0,r0,#0x71aaab		@113.66667
@	adds r0,r0,#0x720000
@	str_ r0,counter
@	bcc h1
@	mov r0,#0
@	strb_ r0,irqen
@	sub r0,r0,#0x10000
@	str_ r0,counter
@@	b irq6502
@	b_long CheckI
@h1:
@	fetch 0

@----------------------------------------------------------------------------
	@.end
