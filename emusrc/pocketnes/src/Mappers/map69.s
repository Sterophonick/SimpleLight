 .align
 .pool
 .text
 .align
 .pool

	@IMPORT mapper_irq_handler
#include "../equates.h"
#include "../6502mac.h"

	global_func mapper69init
	global_func map69_handler

 counter = mapperdata+0
 irqen = mapperdata+4
 cmd = mapperdata+5
 counter_last_timestamp = mapperdata+8

 IRQ_ENABLE = 0x01
 IRQ_COUNTING = 0x80
@IRQ counts down, when it wraps from 0000->FFFF, trigger an IRQ.

@----------------------------------------------------------------------------
mapper69init:			@ Sunsoft FME-7, Batman ROTJ, Gimmick...
@----------------------------------------------------------------------------
	.word write0,write1,void,void			@There is a music channel also

@	mov r1,#-1
@	str_ r1,counter

@	ldr r0,=mapper_69_hook
@	str_ r0,scanlinehook
	
	mov pc,lr

@irq stuff
@--------
map69_handler:
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
	tst addy,#IRQ_COUNTING
	bxeq lr
	
	ldr_ r0,counter
	
	mov r0,r0,lsl#16
	subs r0,r0,r2,lsl#16
	mov r0,r0,lsr#16
	str_ r0,counter
	bcs 0f
	
	tst addy,#IRQ_ENABLE  @are IRQs enabled?
	beq 0f

	stmfd sp!,{r0,r1,lr}
	ldr r0,=mapper_irq_handler
	adrl_ addy,mapper_irq_timeout
	bl_long replace_timeout_2
	ldmfd sp!,{r0,r1,lr}
	b 0f

find_next_irq:
	ldrb_ addy,irqen
	tst addy,#IRQ_COUNTING
	bxeq lr

	ldr_ r2,cycles_to_run
	sub r2,r2,cycles,asr#CYC_SHIFT
	ldr_ r1,timestamp
	add r1,r1,r2
	add r1,r1,#12
	str_ r1,counter_last_timestamp
	
	ldr_ r0,counter
0:
	@get countdown until wrap
	add r0,r0,#1
	adds r0,r0,r0,lsl#1
	
	add r1,r1,r0
	sub r1,r1,#12
	adr r0,map69_handler
	adrl_ addy,mapper_timeout
	b_long replace_timeout_2

@----------------------------------------------------------------------------
write0:		@$8000
@----------------------------------------------------------------------------
	strb_ r0,cmd
	mov pc,lr

@----------------------------------------------------------------------------
write1:		@$A000
@----------------------------------------------------------------------------
	ldrb_ r1,cmd
	tst r1,#0x08
	and r1,r1,#7
	ldreq r2,=writeCHRTBL
	adrne r2,commandlist
	ldr pc,[r2,r1,lsl#2]

irqen69:
	ldrb_ r1,irqen
	tst r1,#IRQ_COUNTING
	beq 0f
	stmfd sp!,{r0,lr}
	bl run_counter
	ldmfd sp!,{r0,lr}
0:
	strb_ r0,irqen
	tst r0,#1
	bne find_next_irq
	
	ldrb_ r1,wantirq
	bic r1,r1,#IRQ_MAPPER
	strb_ r1,wantirq
	b find_next_irq
irqA69:
	ldrb_ r1,irqen
	tst r1,#IRQ_COUNTING
	bne 0f
	strb_ r0,counter+0
	bx lr
0:	@update while counter is running, shouldn't happen
	stmfd sp!,{r0,lr}
	bl run_counter
	ldmfd sp!,{r0,lr}
	strb_ r0,counter+0
	b find_next_irq
irqB69:
	ldrb_ r1,irqen
	tst r1,#IRQ_COUNTING
	bne 0f
	strb_ r0,counter+1
	bx lr
0:	@update while counter is running, shouldn't happen
	stmfd sp!,{r0,lr}
	bl run_counter
	ldmfd sp!,{r0,lr}
	strb_ r0,counter+1
	b find_next_irq

@----------------------------------------------------------------------------
mapJinx:
@----------------------------------------------------------------------------
	tst r0,#0x40
	ldreq r1,=rom_R60			@Swap in ROM at $6000-$7FFF.
	ldrne r1,=sram_R		@Swap in sram at $6000-$7FFF.
	str_ r1,readmem_6
	.if PRG_BANK_SIZE == 4
	str_ r1,readmem_7
	.endif
	ldreq r1,=empty_W		@ROM.
	ldrne r1,=sram_W		@sram.
	str_ r1,writemem_6
	.if PRG_BANK_SIZE == 4
	str_ r1,writemem_7
	.endif
	beq_long map67_
	ldr r1,=NES_RAM-0x5800		@sram at $6000.
	str_ r1,memmap_6
	.if PRG_BANK_SIZE == 4
	str_ r1,memmap_7
	.endif

	mov r0,#0xFF
	strb_ r0,bank6
	mov pc,lr

@----------------------------------------------------------------------------
commandlist:	.word mapJinx,map89_,mapAB_,mapCD_,mirrorKonami_,irqen69,irqA69,irqB69
@----------------------------------------------------------------------------
@ .align
@ .pool
@ .section .iwram, "ax", %progbits
@ .subsection 7
@ .align
@ .pool
@@----------------------------------------------------------------------------
@mapper_69_hook:
@@----------------------------------------------------------------------------
@	ldrb_ r1,irqen
@	cmp r1,#0
@	beq default_scanlinehook
@
@	ldr r0,countdown
@	ldr r1,video			@ Number of cycles per scanline.
@	subs r0,r0,r1
@	str r0,countdown
@	bhi default_scanlinehook
@
@	mov r1,#-1
@	str r1,countdown
@
@	mov r1,#0
@	strb_ r1,irqen
@@	b irq6502
@	b CheckI


	@.end
