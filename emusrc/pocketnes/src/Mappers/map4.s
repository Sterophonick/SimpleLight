 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	global_func mapper4init
	global_func mapper74init
	global_func mapper118init
	global_func mapper119init
	global_func mapper206init
	global_func mapper245init
	global_func mapper249init
@	global_func MMC3_IRQ_Hook
	global_func mmc3_ntsc_pal_reset
	global_func run_mmc3
	global_func mmc3_timeout_handler
	global_func mmc3_set_next_timeout
	global_func mapper_irq_handler
	global_func mmc3_timeout_handler
	global_func mmc3_screen_on
	
	@IMPORT empty_io_w_hook

 countdown = mapperdata+0
 irq_counter = countdown
 latch = mapperdata+1
 irq_latch = latch
 irqen = mapperdata+2
 irq_enable = irqen
 irq_time_add = mapperdata+3
 cmd = mapperdata+4
 bank0 = mapperdata+5
 bank1 = mapperdata+6
 bankadd = mapperdata+7
 usescramble = mapperdata+8
 lastchr = mapperdata+9

 mmc3_spr_subtract = mapperdata + 16
 mmc3_bg_subtract = mapperdata + 20
 last_mmc3_timestamp = mapperdata + 24
 mmc3_scanline_divide = mapperdata + 28



@----------------------------------------------------------------------------
mapper4init:
mapper74init:
mapper118init:
mapper119init:
mapper245init:
mapper249init:
	.word write0,write1,write2,write3
	@note: this code modifies jump tables
	mov r0,#12
	strb_ r0,irq_time_add
	
	ldr r0,=run_mmc3
	str_ r0,screen_off_hook1
	ldr r0,=mmc3_screen_on
	str_ r0,screen_on_hook1
	ldr r0,=mmc3_set_next_timeout
	str_ r0,screen_off_hook2
	str_ r0,screen_on_hook2
	
	ldr r0,=mmc3_ntsc_pal_reset
	str_ r0,ntsc_pal_reset_hook
	
	ldrb_ r0,mapper_number
	ldr r1,=commandlist
	ldr_ r2,writemem_8
	
	cmp r0,#118
	ldreq r2,=write0_118
	
	cmp r0,#245
	@Use mapper 245 for no vrom, otherwise use mapper 4
	ldreqb_ r3,vrompages
	cmpeq r3,#0
	ldreq r2,=write0_245
	
	.if LESSMAPPERS
	.else
	cmp r0,#249
	ldreq r3,=write4
	ldreq r0,=empty_io_w_hook
	streq r3,[r0]
	
	ldreq r2,=write0_249
	.endif
	
	str_ r2,writemem_8
	.if PRG_BANK_SIZE == 4
	str_ r2,writemem_9
	.endif
	mov pc,lr

mapper206init:
	.word write0_206, void, void, void
	@mapper 206 is a subset of MMC3 that lacks WRAM, can't change bank modes, and has no IRQs or mirroring control.
	ldr r0,=empty_R
	str_ r0,readmem_6
	.if PRG_BANK_SIZE == 4
	str_ r0,readmem_7
	.endif
	ldr r0,=void
	str_ r0,writemem_6
	.if PRG_BANK_SIZE == 4
	str_ r0,writemem_7
	.endif
	mov r0,#0xFF
	strb_ r0,bank6
	bx lr

write0_245:
	tst addy,#1
	ldrb_ r1,cmd
	bne w8001_245
write0_245_even:
	strb_ r0,cmd
	eor addy,r0,r1
	tst addy,#0x40
	bne romswitch_245
	mov pc,lr
w8001_245:
	and r1,r1,#7
	ldr pc,[pc,r1,lsl#2]
	.word 0
@----------------------------------------------------------------------------
commandlist_245:
	.word cmd0_245,void,void,void,void,void,cmd6_245,cmd7_245
@----------------------------------------------------------------------------

	.if LESSMAPPERS
	.else
write0_249:
	tst addy,#1
	beq_long write0_even @@
	ldrb_ r1,usescramble
	tst r1,#0xFF
	mov addy,lr
	mov r2,r0
	blne unscramble1
	mov lr,addy

	ldrb_ r1,cmd
	tst r1,#0x80	@reverse CHR?
	and r1,r1,#7
	orrne r1,r1,#8
	ldr pc,[pc,r1,lsl#2]
	.word 0
@----------------------------------------------------------------------------
commandlist_249:
	.word chr01_rshift_,chr23_rshift_,chr4_,chr5_,chr6_,chr7_,cmd6_249,cmd7_249
	.word chr45_rshift_,chr67_rshift_,chr0_,chr1_,chr2_,chr3_,cmd6_249,cmd7_249

	.endif

@----------------------------------------------------------------------------
 .align
 .pool
 .section .vram1, "ax", %progbits
 .subsection 3
 .align
 .pool

write0_206:
	@mapper 206 is a subset of MMC3 that lacks WRAM, can't change bank modes, and has no IRQs or mirroring control.
	and r0,r0,#0x3F
@----------------------------------------------------------------------------
write0:		@$8000-8001
@----------------------------------------------------------------------------
	tst addy,#1
	bne w8001
write0_even:
	ldrb_ r1,cmd
	eors addy,r0,r1
	strneb_ r0,cmd
	tst addy,#0xC0
	bxeq lr
	tst addy,#0x80
	beq romswitch
	b_long write0_chr_base_switch
	.pushsection .text, "ax", %progbits
write0_chr_base_switch:

			@CHR base switch (0000/1000)
	ldr_ r1,nes_chr_map
	ldr_ r2,nes_chr_map+4
	str_ r2,nes_chr_map
	str_ r1,nes_chr_map+4
	stmfd sp!,{r3-r7,lr}
	adr_ lr,vram_map
	ldmia lr,{r0-r7}
	stmia lr!,{r4-r7}
	stmia lr,{r0-r3}
	bl_long updateBGCHR_  @@
	ldmfd sp!,{r3-r7,lr}
	bx lr
	.popsection
w8001:
	ldrb_ r1,cmd
	tst r1,#0x80	@reverse CHR?
	and r1,r1,#7
	orrne r1,r1,#8
	ldr pc,[pc,r1,lsl#2]
	.word 0
@----------------------------------------------------------------------------
commandlist:	.word chr01_rshift_,chr23_rshift_,chr4_,chr5_,chr6_,chr7_,cmd6,mapAB_
		.word chr45_rshift_,chr67_rshift_,chr0_,chr1_,chr2_,chr3_,cmd6,mapAB_
@----------------------------------------------------------------------------


write0_118:		@$8000-8001
@----------------------------------------------------------------------------
	tst addy,#1
	beq write0_even
w8001_118:
	ldrb_ r1,cmd
	and r2,r1,#7
	cmp r2,#6
	bge w8001
	@if r1 & 0x80 && r1<2
	tst r1,#0x80
	beq w8001_118_alttest
	cmp r2,#2
	blt w8001
	b w8001_118_noalttest
w8001_118_alttest:
	cmp r2,#2
	bge w8001
w8001_118_noalttest:
	ldrb_ r1,lastchr
	strb_ r0,lastchr
	eor r1,r0,r1
	tst r1,#0x80
	beq w8001
	
	tst r0,#0x80
	stmfd sp!,{r0,lr}
	bl_long mirror1_  @@
	ldmfd sp!,{r0,lr}
	b w8001

	.if LESSMAPPERS
	.else
	.pushsection .text, "ax", %progbits
cmd7_249:
	mov r0,r2
	mov addy,lr
	bl_long unscramble3  @@
	mov lr,addy
	b_long mapAB_
cmd6_249:
	mov r0,r2
	mov addy,lr
	bl_long unscramble3  @@
	mov lr,addy
	.popsection
	.endif
cmd6:			@$8000/$C000 select
	strb_ r0,bank0
romswitch:
	mov r0,#-2
	ldrb_ r1,cmd
	tst r1,#0x40
	mov addy,lr
	bne_long rs0

	bl_long mapCD_  @@
	ldrb_ r0,bank0
	mov lr,addy
	b_long map89_  @@
	.pushsection .text, "ax", %progbits
rs0:
	bl_long map89_  @@
	ldrb_ r0,bank0
	mov lr,addy
	b_long mapCD_
	.popsection


@----------------------------------------------------------------------------
write1:		@$A000-A001
@----------------------------------------------------------------------------
	tst addy,#1
	movne pc,lr
	tst r0,#1
	b_long mirror2V_
@----------------------------------------------------------------------------
write2:		@C000-C001
@----------------------------------------------------------------------------
	tst addy,#1
	streqb_ r0,irq_latch
	bxeq lr
	movne r0,#0
	strneb_ r0,irq_counter
	
	@???
	ldr_ r1,cycles_to_run
	sub r1,r1,cycles,asr#CYC_SHIFT
	ldr_ r2,timestamp
	add r2,r2,r1
	str_ r2,last_mmc3_timestamp
	
	b_long mmc3_set_next_timeout
@----------------------------------------------------------------------------
write3:		@E000-E001
@----------------------------------------------------------------------------
	ands r0,addy,#1
	strb_ r0,irq_enable
	bxne lr
	ldrb_ r0,wantirq
	bic r0,r0,#IRQ_MAPPER
	strb_ r0,wantirq
	bx lr


	.pushsection .text, "ax", %progbits
mmc3_ntsc_pal_reset:
	@eq = NTSC
	@lt = PAL
	@gt = DENDY
	ldrge r1,=0x510+12*16	@NTSC: 81 of 341
	ldrlt r1,=0x4BF+12*16	@PAL: 75+15/16  of 319+11/16
	str_ r1,mmc3_spr_subtract
	ldrge r1,=0x110+12*16 @NTSC: 17 of 341
	ldrlt r1,=0x0FF+12*16 @PAL: 15+15/16  of 319+11/16
	str_ r1,mmc3_bg_subtract
	ldrge r1,=0xc0301	@0x10000000 / 341
	ldrlt r1,=0xcd001	@0x10000000 / (319+11/16)
	str_ r1,mmc3_scanline_divide
	bx lr
	.popsection
	
	.pushsection .iwram, "ax", %progbits
mmc3_timeout_handler:
	bl run_mmc3
	bl mmc3_set_next_timeout
	b _GO  @@

	.pushsection .vram1, "ax", %progbits
mmc3_screen_on:
	ldr_ r2,cycles_to_run
	sub r2,r2,cycles,asr#CYC_SHIFT
	ldr_ r1,timestamp
	add r1,r1,r2
	str_ r1,last_mmc3_timestamp
	@r1 = timestamp

	@check if PPU address is low
	ldr_ r0,vramaddr
	tst r0,#0x1000
	bxne lr
	@ppu address is low, check if we are turning screen on to a high A12

	@check which pattern table sprites fetch from
	@if BG and SPR sources match, bye
	ldrb_ r0,ppuctrl0
	ands r2,r0,#0x10
	movne r2,#1
	ands r0,r0,#0x28
	movne r0,#1
	cmp r2,r0
	bxeq lr
	
	ldr_ addy,frame_timestamp
	sub r1,r1,addy
	ldr_ addy,timestamp_div
	smull r2,addy,r1,addy
	@addy = scanline count, r2 = fraction
	
	mov addy,#0
	ldr r1,=0xBA2E8BA2  @(260 - 12) / 341
	cmp r2,r1
	@if r2 >= r1, we are inside sprites
	movcs addy,#1 @cs = unsigned higher or same
	ldrcs r1,=0xEA3A8EA3   @(324 - 12) / 341
	cmpcs r2,r1
	movcs addy,#0
	@r0 = sprites use right table (1), or bg uses right table (0)
	@addy = whether we are rendering sprites (1) or bg (0)
	@if r0 == addy, we count one line, otherwise do nothing.
	cmp r0,addy
	bxne lr
	mov r0,#1
	b_long mmc3_clock_irq_entry
	.popsection

run_mmc3:
	@assumes timestamp is correct
	ldr_ r12,last_mmc3_timestamp
	
	ldr_ r1,cycles_to_run
	sub r1,r1,cycles,asr#CYC_SHIFT
	ldr_ r2,timestamp
	add r2,r2,r1
run_mmc3_2:
	str_ r2,last_mmc3_timestamp
	@if screen is off, bye  (MUST UPDATE last_mmc3_timestamp when turning on screen)
	ldrb_ r0,screen_off
	movs r0,r0
	bxne lr
	@if BG and SPR sources match, bye
	ldrb_ r0,ppuctrl0
	ands r1,r0,#0x10
	movne r1,#1
	ands r0,r0,#0x28
	movne r0,#1
	cmp r1,r0
	bxeq lr
	@r0==1 indicates sprites use right table, otherwise backgrounds use right table.  No other case is possible to reach this code.
	@r12 = last timestamp, r2=current timestamp

	@timestamp
	@frame_timestamp
	@time_subtract*16
	@last_mmc3_timestamp
	@
	@a = time_subtract*16 - frame_timestamp*16
	@b = a + last_mmc3_timestamp*16
	@c = a + timestamp*16
	@
	@b_div = b/341_16
	@c_div = c/341_16
	@to_count = c_div-b_div
	
	@we read the timestamp, and skew the clock to align division to a lo-hi edge boundary.
	tst r0,#1
	ldr_ r1,frame_timestamp  @this could be replaced by a number multiplied by 16, with an offset subtracted from it
	ldrne_ r0,mmc3_spr_subtract	@NTSC=81*16  PAL=NTSC*(3/3.2)
	ldreq_ r0,mmc3_bg_subtract	@NTSC=17*16
	sub r0,r0,r1,lsl#4
	add r12,r0,r12,lsl#4
	add r2,r0,r2,lsl#4
	
	ldr_ r0,mmc3_scanline_divide	@NTSC=0xc0301  PAL=0xcd001
	umull r1,r12,r0,r12
	umull r1,r2,r0,r2
	subs r0,r2,r12
	bxeq lr

mmc3_clock_irq_entry:
	@if you call this from outside, set last_mmc3_timestamp first
	@r0 = number of times to clock the MMC3 counter

	ldrb_ r1,irq_enable
	
	ldrb_ r2,irq_counter
	cmp r2,#0
	beq 1f
	subs addy,r2,r0
	strb_ addy,irq_counter
	bne 2f
@reached zero
3:@	ldrb r0,irq_enable
	tst r1,#1
	beq 2f
	
	adrl_ r12,mapper_irq_timeout
	ldr_ r1,last_mmc3_timestamp
	ldrb_ r0,irq_time_add
	add r1,r1,r0
	ldr r0,=mapper_irq_handler
	stmfd sp!,{lr}
	bl replace_timeout_2  @@
	ldmfd sp!,{lr}
	b 2f
1:	@was zero
	
	tst r1,#4	@if this bit is set, we use the "add one extra" feature for tengen rambo
	bic r1,r1,#4
	strb_ r1,irqen
	
	ldrb_ r2,irq_latch
	addne r2,r2,#1		@for tengen RAMBO
	strb_ r2,irq_counter
	movs r2,r2
	beq 3b
	
@	@TEST, if breakpoint hits, something is wrong
@	subs r0,r0,#1
@	beq 2f
@	mov r11,r11
2:
	bx lr

mmc3_set_next_timeout:
	@if screen is off, bye  (MUST UPDATE last_mmc3_timestamp when turning on screen)
	ldrb_ r0,screen_off
	movs r0,r0
	bne mmc3_cancel_timeout
	@if BG and SPR sources match, bye
	ldrb_ r0,ppuctrl0
	ands r1,r0,#0x10
	movne r1,#1
	ands r0,r0,#0x28
	movne r0,#1
	cmp r1,r0
	beq mmc3_cancel_timeout
	
	@if counter = 0 and latch = 0, and IRQ disabled
	ldr_ r1,countdown
	bics r1,r1,#0xFF000000
	beq mmc3_cancel_timeout
	@cyc = (timestamp-frame_timestamp)
	@next_mmc3_irq = (((cyc+81)/341) + counter) *341-81

	ldr_ r2,last_mmc3_timestamp	@r2 = last MMC3 timestamp (ppu cycles)
	ldr_ r1,frame_timestamp		@r1 = timestamp of frame start (ppu cycles)
	sub r2,r2,r1				@r2 = MMC3 timestamp relative to frame start
	tst r0,#1					@test if we are using BG or sprites
	ldrne_ r1,mmc3_spr_subtract	@r1 = 81 cycles times 16
	ldreq_ r1,mmc3_bg_subtract	@r1 = 17 cycles times 16
	add r2,r1,r2,lsl#4
	ldr_ r0,mmc3_scanline_divide
	umull r12,r2,r0,r2
	ldrb_ r12,irq_counter
	movs r12,r12
	moveq r12,#1
	add r2,r2,r12
	ldr_ r12,timestamp_mult
	mul r2,r12,r2
	sub r2,r2,r1
	ldr_ r0,frame_timestamp
	add r1,r0,r2,lsr#4			@adding units that were multipled by 16, so we divide here
	adrl_ r12,mapper_timeout
	ldr r0,=mmc3_timeout_handler
	
	b replace_timeout_2
mmc3_cancel_timeout:
	adrl_ r12,mapper_timeout
	b remove_timeout

mapper_irq_handler:
	ldrb_ r0,wantirq
	orr r0,r0,#IRQ_MAPPER
	strb_ r0,wantirq
	b CheckI	@@
	
	.popsection
	
 .align
 .pool
 .text
 .align
 .pool

cmd0_245:
	tst r0,#2
	moveq r1,#0
	movne r1,#1
	strb_ r1,bankadd
	b romswitch_245
cmd7_245:
	strb_ r0,bank1
	b romswitch_245
cmd6_245:			@$8000/$C000 select
	strb_ r0,bank0
romswitch_245:
	mov addy,lr
	mov r0,#63
	ldrb_ r1,bankadd
	orr r0,r0,r1,lsl#6
	bl_long mapEF_
	
	ldrb_ r0,bank1
	ldrb_ r1,bankadd
	orr r0,r0,r1,lsl#6
	bl_long mapAB_
	
	mov r0,#62
	ldrb_ r1,bankadd
	orr r0,r0,r1,lsl#6

	ldrb_ r1,cmd
	tst r1,#0x40
	bne rs0_245
	
	bl_long mapCD_
	ldrb_ r0,bank0
	ldrb_ r1,bankadd
	orr r0,r0,r1,lsl#6
	
	mov lr,addy
	b_long map89_
rs0_245:
	bl_long map89_
	ldrb_ r0,bank0
	ldrb_ r1,bankadd
	orr r0,r0,r1,lsl#6
	
	mov lr,addy
	b_long mapCD_
	
	.if LESSMAPPERS
	.else
@this stuff is for mapper 249, which scrambles the bank numbers
@----------------------------------------------------------------------------
write4:		@5000
@----------------------------------------------------------------------------
	cmp addy,#0x5000
	movne pc,lr
	and r0,r0,#2
	strb_ r0,usescramble
	mov pc,lr
unscramble1:
	and r1,r0,#0x03
	tst r0,#0x04
	orrne r1,r1,#0x20
	tst r0,#0x08
	orrne r1,r1,#0x04
	tst r0,#0x10
	orrne r1,r1,#0x40
	tst r0,#0x20
	orrne r1,r1,#0x80
	tst r0,#0x40
	orrne r1,r1,#0x10
	tst r0,#0x80
	orrne r1,r1,#0x08
	mov r0,r1
	bx lr
unscramble2:
	and r1,r0,#0x01
	tst r0,#0x02
	orrne r1,r1,#0x08
	tst r0,#0x04
	orrne r1,r1,#0x10
	tst r0,#0x08
	orrne r1,r1,#0x04
	tst r0,#0x10
	orrne r1,r1,#0x02
	mov r0,r1
	bx lr
unscramble3:
	ldrb_ r1,usescramble
	tst r1,#0xFF
	bxeq lr
	cmp r0,#0x20
	blt unscramble2
	sub r0,r0,#0x20
	b unscramble1
	.endif
	@.end
