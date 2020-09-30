 .align
 .pool
 .text
 .align
 .pool

	@IMPORT mapper_irq_handler
#include "../equates.h"
#include "../6502mac.h"

	global_func mapper65init
@	global_func mapper_65_hook
	global_func map65_handler

 latch = mapperdata+0
 counter = mapperdata+4
 irqen = mapperdata+8
 mswitch = mapperdata+9
 counter_last_timestamp = mapperdata+12

 IRQ_ENABLE = 0x80

@IRQ counts down
@When it reaches 0, it stops, and triggers an IRQ

@----------------------------------------------------------------------------
mapper65init:	@Irem, Spartan X 2...
@----------------------------------------------------------------------------
	.word write8000,writeA000,writeC000,void

@	adr r0,mapper_65_hook
@	str_ r0,scanlinehook

	mov pc,lr

@irq stuff
@--------
map65_handler:
	ldr_ r2,mapper_timestamp
	bl run_2
	b_long _GO

run_counter:
	@get timestamp
	ldr_ r1,cycles_to_run
0:
	sub r1,r1,cycles,asr#CYC_SHIFT
	ldr_ r2,timestamp
	add r2,r2,r1
run_2:
	add r2,r2,#12
	ldr_ r1,counter_last_timestamp
	str_ r2,counter_last_timestamp
	sub r1,r2,r1
	@r1 = elapsed cycles
	
	ldrb_ addy,irqen
	tst addy,#IRQ_ENABLE
	bxeq lr
	
	@divide time difference by 3
	ldr r0,=0x55555556 @1/3
	umull addy,r1,r0,r1
	
	movs r1,r1
	bxeq lr
	
	ldr_ r0,counter
	
	mov r0,r0,lsl#16
	subs r0,r0,r1,lsl#16
	mov r0,r0,lsr#16
	str_ r0,counter
	
	@IRQ triggers if counter<=to_run
	@skip if counter>to_run
	
	bhi 0f
	
	mov r0,#0
	strb_ r0,irqen  @disable IRQs after it ticks
	str_ r0,counter @keep counter at zero
	
	mov r1,r2
	ldr r0,=mapper_irq_handler
	adrl_ r12,mapper_irq_timeout
	b_long replace_timeout_2

find_next_irq:
	ldrb_ addy,irqen
	tst addy,#IRQ_ENABLE
	bxeq lr

	ldr_ r1,cycles_to_run
	sub r1,r1,cycles,asr#CYC_SHIFT
	ldr_ r2,timestamp
	add r2,r2,r1
	add r2,r2,#12
	str_ r2,counter_last_timestamp
	
	ldr_ r0,counter
0:
	@get countdown until wrap
	adds r0,r0,r0,lsl#1
	moveq r0,#0x30000
	
	add r1,r2,r0
	sub r1,r1,#12
	adr r0,map65_handler
	adrl_ r12,mapper_timeout
	b_long replace_timeout_2

@-------------------------------------------------------
write8000:
@-------------------------------------------------------
	tst addy,#0x1000
	beq_long map89_

write9000:
	and addy,addy,#7
	adr r1,write9tbl
	ldr pc,[r1,addy,lsl#2]

w90:
	ldrb_ r1,mswitch
	cmp r1,#0
	movne pc,lr
	tst r0,#0x40
	b_long mirror2H_
w91:
	mov r1,#1
	strb_ r1,mswitch
	tst r0,#0x80
	b_long mirror2V_
w93:
	ldrb_ r1,wantirq
	bic r1,r1,#IRQ_MAPPER
	strb_ r1,wantirq
	
	ldrb_ r1,irqen
	tst r1,#IRQ_ENABLE
	beq 0f
	stmfd sp!,{r0,lr}
	bl run_counter
	ldmfd sp!,{r0,lr}
0:
	and r0,r0,#0x80
	strb_ r0,irqen
	b find_next_irq
w94:
	ldrb_ r1,wantirq
	bic r1,r1,#IRQ_MAPPER
	strb_ r1,wantirq

	ldr_ r2,latch
	str_ r2,counter
	b find_next_irq
w95:
	strb_ r0,latch+1
	mov pc,lr
w96:
	strb_ r0,latch
	mov pc,lr

write9tbl: .word w90,w91,void,w93,w94,w95,w96,void
@-------------------------------------------------------
writeA000:
@-------------------------------------------------------
	tst addy,#0x1000
	beq_long mapAB_
writeB000:
	and addy,addy,#7
	ldr r1,=writeCHRTBL
	ldr pc,[r1,addy,lsl#2]

@-------------------------------------------------------
writeC000:
@-------------------------------------------------------
	cmp addy,#0xC000
	beq_long mapCD_
	mov pc,lr
@@-------------------------------------------------------
@mapper_65_hook:
@@------------------------------------------------------
@	ldrb_ r0,irqen
@	cmp r0,#0	@timer active?
@	beq h1
@
@	ldr_ r0,counter
@	subs r0,r0,#113		@counter-A
@	bhi h0
@
@	mov r0,#0
@	strb_ r0,irqen
@	str_ r0,counter	@clear counter and IRQenable.
@@	b irq6502
@	b_long CheckI
@h0:
@	str_ r0,counter
@h1:
@	fetch 0
@-------------------------------------------------------
	@.end
