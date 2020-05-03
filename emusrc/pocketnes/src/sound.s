#include "equates.h"

@	.include "equates.h"
@	.include "ppu.h"
@	.include "timeout.h"
@	.include "6502.h"
	
	@IMPORT GLOBAL_PTR_BASE

@	.global _freqtbl
	.global _pcmstart
	.global _pcmcurrentaddr 
	
	.global sound_state
	
	global_func timer1interrupt
	
	global_func timer_2_interrupt
	global_func timer_3_interrupt
	global_func Sound_hardware_reset
@	global_func updatesound
@	global_func make_freq_table
	global_func _4000w
	global_func _4001w
	global_func _4002w
	global_func _4003w
	global_func _4004w
	global_func _4005w
	global_func _4006w
	global_func _4007w
	global_func _4008w
	global_func _400Aw
	global_func _400Bw
	global_func _400Cw
	global_func _400Ew
	global_func _400Fw
	global_func _4010w
	global_func _4011w
	global_func _4012w
	global_func _4013w
	global_func _4015w
	global_func _4015r
	global_func _4017w
	.global _pcmctrl
		
	.global PCMWAV
	
	global_func reset_sound_after_loadstate
	.global _frame_mode
	.global _channel_enables
	global_func frame_irq_handler
	global_func frame_sequencer_handler
	global_func dmc_handler
	global_func sound_reset

 PCMSTEP		= 3<<24
 PCMLIMIT	= 96<<24

 .align
 .pool
 .section .iwram, "ax", %progbits
 .subsection 0
 .align
 .pool
@----------------------------------------------------------------------------
pcm_mix:
@----------------------------------------------------------------------------
	movs r2,r2,lsr#1
	addcs r0,r0,#PCMSTEP
	subcc r0,r0,#PCMSTEP
	cmp r0,#PCMLIMIT			@range check volume level
	movgt r0,#PCMLIMIT
	cmp r0,#-PCMLIMIT
	movlt r0,#-PCMLIMIT
	mov r5,r0

	movs r2,r2,lsr#1
	addcs r0,r0,#PCMSTEP
	subcc r0,r0,#PCMSTEP
	cmp r0,#PCMLIMIT			@range check volume level
	movgt r0,#PCMLIMIT
	cmp r0,#-PCMLIMIT
	movlt r0,#-PCMLIMIT
	orr r5,r0,r5,lsr#8

	movs r2,r2,lsr#1
	addcs r0,r0,#PCMSTEP
	subcc r0,r0,#PCMSTEP
	cmp r0,#PCMLIMIT			@range check volume level
	movgt r0,#PCMLIMIT
	cmp r0,#-PCMLIMIT
	movlt r0,#-PCMLIMIT
	orr r5,r0,r5,lsr#8

	movs r2,r2,lsr#1
	addcs r0,r0,#PCMSTEP
	subcc r0,r0,#PCMSTEP
	cmp r0,#PCMLIMIT			@range check volume level
	movgt r0,#PCMLIMIT
	cmp r0,#-PCMLIMIT
	movlt r0,#-PCMLIMIT
	orr r5,r0,r5,lsr#8
	str r5,[r12],#4

	movs r2,r2,lsr#1
	addcs r0,r0,#PCMSTEP
	subcc r0,r0,#PCMSTEP
	cmp r0,#PCMLIMIT			@range check volume level
	movgt r0,#PCMLIMIT
	cmp r0,#-PCMLIMIT
	movlt r0,#-PCMLIMIT
	mov r5,r0

	movs r2,r2,lsr#1
	addcs r0,r0,#PCMSTEP
	subcc r0,r0,#PCMSTEP
	cmp r0,#PCMLIMIT			@range check volume level
	movgt r0,#PCMLIMIT
	cmp r0,#-PCMLIMIT
	movlt r0,#-PCMLIMIT
	orr r5,r0,r5,lsr#8

	movs r2,r2,lsr#1
	addcs r0,r0,#PCMSTEP
	subcc r0,r0,#PCMSTEP
	cmp r0,#PCMLIMIT			@range check volume level
	movgt r0,#PCMLIMIT
	cmp r0,#-PCMLIMIT
	movlt r0,#-PCMLIMIT
	orr r5,r0,r5,lsr#8

	movs r2,r2,lsr#1
	addcs r0,r0,#PCMSTEP
	subcc r0,r0,#PCMSTEP
	cmp r0,#PCMLIMIT			@range check volume level
	movgt r0,#PCMLIMIT
	cmp r0,#-PCMLIMIT
	movlt r0,#-PCMLIMIT
	orr r5,r0,r5,lsr#8
	str r5,[r12],#4

	b_long endmix
@----------------------------------------------------------------------------

 .align
 .pool
 .section .vram1, "ax", %progbits
 .align
 .pool

@----------------------------------------------------------------------------
timer1interrupt:
@----------------------------------------------------------------------------
	@r0 = REG_BASE
	
	@copy trailing 8 bytes
	@ldr r3,=PCMWAV+PCMWAVSIZE
	@ldmia r3,{r1,r2}
	@sub r3,r3,#PCMWAVSIZE
	@stmia r3,{r1,r2}
	
	strh r0,[r0,#REG_DMA2CNT_H] @DMA stop
	mov r1,   #0xb600
	orr r1,r1,#0x0040		@noINTR fifo 32bit repeat incsrc fixeddst
	strh r1,[r0,#REG_DMA2CNT_H] @DMA go	
	
	@disable vblank interrupts
	ldrb r1,[r0,#REG_IE]
	bic r1,r1,#1
	strb r1,[r0,#REG_IE]
	mov r1,#1
	strb r1,[r0,#REG_IME]	@enable other interrupts
	
	
	@update DMA buffer for PCM
	stmfd sp!,{r4-r8,globalptr,lr}
	ldr globalptr,=GLOBAL_PTR_BASE
	
	ldr_ r3,pcmcount			@r3=bytes remaining
	ldr r12,=PCMWAV			@r12=dma buffer
	add r4,r12,#PCMWAVSIZE		@r4=end of buffer

	tst r3,#0x80000000		@if pcmcount<0 (pcm finished on previous interrupt)
	movne r3,#0x80000000
	bne pcm1				@finish clearing buffer

	ldr_ r1,pcmcurrentaddr		@r1=pcm data ptr
	ldr_ r0,pcmlevel			@r0=volume level
	mov r0,r0,lsl#24

pcm_loop:
	@convert pcmcurrentaddr (r1) from NES address to real memory address
	tst r1,#0xFF000000  @If this is already a real pointer, do nothing
	adreq_ r2,memmap_tbl
	moveq r5,r1,lsr#PRG_BANK_SHIFT
	ldreq r5,[r2,r5,lsl#2]
	addeq r1,r1,r5
pcm0:
	subs r3,r3,#1			@pcmcount--
	bmi pcm2
	ldrb r2,[r1],#1			@next byte..
	b_long pcm_mix
endmix:
	cmp r12,r4
	blo pcm0
	mov r0,r0,lsr#24
	str_ r0,pcmlevel
	str_ r1,pcmcurrentaddr
	b pcmexit
pcm2:			@PCM data just ran out.  what now?
	ldrb_ r2,pcmctrl
	tst r2,#0x40		@looping enabled?
				@yes?
	ldrne_ r3,pcmlength			@reload pcmcount
	ldrne_ r1,pcmstart			@reload pcmcurrentaddr
	bne pcm_loop
pcm1:				@PCM has stopped.  clear remaining sound buffer.
	mov r1,#0
	mov r0,r12
	subs r2,r12,r4
	blgt_long memset32

	cmp r3,#0x80000000		@if this was the 2nd pass (entire buffer is cleared),
	bne pcmexit
	mov r1,#REG_BASE
	add r1,r1,#REG_TM0CNT_L
	mov r0,#0
	strh r0,[r1,#2]			@stop timer 0 (to reduce interrupt frequency)
	ldrb_ r0,pcmctrl+1
	and r0,r0,#0xef			@only clear channel 5.
	strb_ r0,pcmctrl+1

pcmexit:
	str_ r3,pcmcount
	
	@re-enable vblank interrupts
	mov r0,#REG_BASE
	ldrb r1,[r0,#REG_IE]
	orr r1,r1,#1
	strb r1,[r0,#REG_IE]
	
	ldmfd sp!,{r4-r8,globalptr,pc}

.section .vram1, "ax", %progbits

write_square_1:
	mov r0,#REG_BASE
timer_2_interrupt:
	@There is about 80 clock cycles between the IRQ firing and updating the sound register.  That's good enough.
	@r0 = REG_BASE
	ldr r1,=GLOBAL_PTR_BASE
	ldr r2,[r1,#gba_timer_2]
	str r0,[r0,#REG_TM2CNT_L]  @disable timer
	str r2,[r0,#REG_TM2CNT_L]  @set reload and start value, disable IRQ, start timer
	ldr r2,[r1,#gba_sound1cnt_x]
	str r2,[r0,#REG_SOUND1CNT_X]  @write soundcnt_x
	bx lr

write_square_2:
	mov r0,#REG_BASE
timer_3_interrupt:
	ldr r1,=GLOBAL_PTR_BASE
	ldr r2,[r1,#gba_timer_3]
	str r0,[r0,#REG_TM3CNT_L]	@disable timer
	str r2,[r0,#REG_TM3CNT_L]	@set reload and start value, disable IRQ, start timer
	ldr r2,[r1,#gba_sound2cnt_h]
	str r2,[r0,#REG_SOUND2CNT_H]	@write soundcnt_h
	bx lr
	
	global_func write_square_1
	global_func timer_2_interrupt
	global_func write_square_2
	global_func timer_3_interrupt
	
update_tri_volume:
	ldrb_ r0,tri_length
	ldrb_ r1,tri_linear
	movs r0,r0
	movnes r1,r1
	beq tri_off
tri_on:
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SOUND3CNT_H+1]
	orr r0,r0,#0x20
	strb r0,[r1,#REG_SOUND3CNT_H+1]
	bx lr
tri_off:
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SOUND3CNT_H+1]
	bic r0,r0,#0x20
	strb r0,[r1,#REG_SOUND3CNT_H+1]
	bx lr
	
	global_func tri_on
	global_func tri_off
	
.text

frame_sequencer_handler:
	@envelopes and triangle linear counter are clocked every apu frame

	@sq1 envelope
	ldrb_ r0,reg_4000
	and r2,r0,#0x0F

	ldrb_ r1,reg_4003_written
	movs r1,r1
	beq 0f
	strb_ zero_byte,reg_4003_written
	strb_ r2,sq1_env_delay
	mov r1,#0x0F
	strb_ r1,sq1_env
0:
	ldrb_ r12,sq1_env_delay
	subs r12,r12,#1
	movmi r12,r2
	strb_ r12,sq1_env_delay
	bpl 0f
	
	ldrb_ r1,sq1_env
	and r2,r0,#0x20
	orrs r2,r2,r1
	beq 0f
	sub r1,r1,#1
	and r1,r1,#0x0F
	strb_ r1,sq1_env

	@volume may change
	tst r0,#0x10
	moveq r0,r1,lsl#4
	bleq update_sq1_volume_r0_env
0:
	@sq2 envelope
	ldrb_ r0,reg_4004
	and r2,r0,#0x0F

	ldrb_ r1,reg_4007_written
	movs r1,r1
	beq 0f
	strb_ zero_byte,reg_4007_written
	strb_ r2,sq2_env_delay
	mov r1,#0x0F
	strb_ r1,sq2_env
0:
	ldrb_ r12,sq2_env_delay
	subs r12,r12,#1
	movmi r12,r2
	strb_ r12,sq2_env_delay
	bpl 0f
	
	ldrb_ r1,sq2_env
	and r2,r0,#0x20
	orrs r2,r2,r1
	beq 0f
	sub r1,r1,#1
	and r1,r1,#0x0F
	strb_ r1,sq2_env

	@volume may change
	tst r0,#0x10
	moveq r0,r1,lsl#4
	bleq update_sq2_volume_r0_env
0:
	@noise envelope
	ldrb_ r0,reg_400C
	and r2,r0,#0x0F

	ldrb_ r1,reg_400F_written
	movs r1,r1
	beq 0f
	strb_ zero_byte,reg_400F_written
	strb_ r2,noise_env_delay
	mov r1,#0x0F
	strb_ r1,noise_env
0:
	ldrb_ r12,noise_env_delay
	subs r12,r12,#1
	movmi r12,r2
	strb_ r12,noise_env_delay
	bpl 0f
	
	ldrb_ r1,noise_env
	and r2,r0,#0x20
	orrs r2,r2,r1
	beq 0f
	sub r1,r1,#1
	and r1,r1,#0x0F
	strb_ r1,noise_env

	@volume may change
	tst r0,#0x10
	moveq r0,r1,lsl#4
	bleq update_noise_volume_r0_env
0:
	@linear counter
	ldrb_ r2,reg_4008
	ldrb_ r0,tri_linear
	ldrb_ r1,reg_400B_written
	tst r2,#0x80
	streqb_ zero_byte,reg_400B_written
	movs r1,r1
	andne r0,r2,#0x7F
	addne r0,r0,#1
	movs r0,r0
	subnes r0,r0,#1
	strb_ r0,tri_linear
	bl_long update_tri_volume

	ldrb_ r0,frame_sequencer_step
	add r1,r0,#1
	strb_ r1,frame_sequencer_step
	and r0,r0,#3
	ldr pc,[pc,r0,lsl#2]
	nop
	.word _frame0, _frame1, _frame2, _frame3
_frame3:
	ldrb_ r0,frame_mode
	tst r0,#0x80
	ldr_ r2,frame_sequencer_interval
	beq _frame1_b
	add r2,r2,r2
	sub r2,r2,#6*3
	b 2f
	
_frame0:
	ldrb_ r0,frame_mode
	tst r0,#0xC0
	bleq want_to_do_frame_irq
	@fall through to _frame2
_frame2:
	@square 1 length
	ldrb_ r0,reg_4000
	tst r0,#0x20
	bne 0f
	ldrb_ r0,sq1_length
	subs r0,r0,#1
	bmi 0f
	strb_ r0,sq1_length
	bl update_sq1_volume
0:
	@square 2 length
	ldrb_ r0,reg_4004
	tst r0,#0x20
	bne 0f
	ldrb_ r0,sq2_length
	subs r0,r0,#1
	bmi 0f
	strb_ r0,sq2_length
	bl update_sq2_volume
0:
	@triangle length
	ldrb_ r0,reg_4008
	tst r0,#0x80
	bne 0f
	ldrb_ r0,tri_length
	subs r0,r0,#1
	bmi 0f
	strb_ r0,tri_length
	bl_long update_tri_volume
0:
	@noise length
	ldrb_ r0,reg_400C
	tst r0,#0x20
	bne 0f
	ldrb_ r0,noise_length
	subs r0,r0,#1
	bmi 0f
	strb_ r0,noise_length
	bl update_noise_volume
0:

	@sweep = reg_4001
	@old_sweep_delay = sweep_delay
	@if sweep delay == 0 || reg_4001_written
	@	reg_4001_written = false
	@	sweep_delay = (sweep >> 4) & 7
	@else
	@	sweep_delay--
	@if old_sweep_delay == 0
	@	shift = sweep & 7
	@	if (shift != 0 && sweep_enabled && period >= 8)
	@		offset = period >> shift
	@		if (sweep & 8) @negative flag
	@			offset = -1 - offset  @for sq1
	@			offset =  0 - offset  @for sq2
	@		if (period + offset < 0x800 )
	@			period += offset
	ldrb_ r1,reg_4001
	ldrb_ r0,sq1_sweep_delay
	and r2,r1,#0x87
	orrs r2,r2,r0
	beq 0f
	
	ldrb_ r2,reg_4001_written
	movs r0,r0
	eornes r2,r2,#0xFF
	@eq = find sweep delay
	moveq r2,r1,lsr#4
	andeq r2,r2,#7
	subne r2,r0,#1
	strb_ r2,sq1_sweep_delay
	mov r2,#0
	strb_ r2,reg_4001_written
	
	movs r0,r0
	bne 0f
	ands r0,r1,#7
	tstne r1,#0x80
	beq 0f
	ldr_ r2,sq1_period
	cmp r2,#8
	blt 0f
	tst r1,#0x08
	mov r1,r2,lsr r0
	mvnne r1,r1  @for square 2, rsb r1,r1,#0 instead
	add r2,r2,r1
	cmp r2,#0x800
	bge 0f
@	mov r1,#0xFF
@	orr r1,r1,#0x0700
@	and r2,r2,r1
	str_ r2,sq1_period
	bl update_sq1_period_r2
0:
	@square 2 sweep
	ldrb_ r1,reg_4005
	ldrb_ r0,sq2_sweep_delay
	and r2,r1,#0x87
	orrs r2,r2,r0
	beq 0f
	
	ldrb_ r2,reg_4005_written
	movs r0,r0
	eornes r2,r2,#0xFF
	@eq = find sweep delay
	moveq r2,r1,lsr#4
	andeq r2,r2,#7
	subne r2,r0,#1
	strb_ r2,sq2_sweep_delay
	mov r2,#0
	strb_ r2,reg_4005_written
	
	movs r0,r0
	bne 0f
	ands r0,r1,#7
	tstne r1,#0x80
	beq 0f
	ldr_ r2,sq2_period
	cmp r2,#8
	blt 0f
	tst r1,#0x08
	mov r1,r2,lsr r0
	rsbne r1,r1,#0
	add r2,r2,r1
	cmp r2,#0x800
	bge 0f
@	mov r1,#0xFF
@	orr r1,r1,#0x0700
@	and r2,r2,r1
	str_ r2,sq2_period
	bl update_sq2_period_r2
0:
	ldr_ r2,frame_sequencer_interval
	b 2f

_frame1:
	@frame 1 is 2 CPU cycles shorter
	ldr_ r2,frame_sequencer_interval
	sub r2,r2,#2*3
_frame1_b:
2:
	ldr_ r1,frame_sequencer_timestamp
	adr r0,frame_sequencer_handler
	adrl_ r12,frame_sequencer_timeout
	add r1,r1,r2
	bl_long install_timeout_2
	b_long _GO

	global_func want_to_do_frame_irq
want_to_do_frame_irq:
	adrl_ r12,frame_irq_timeout
	ldr_ r1,frame_sequencer_timestamp
	add r2,r2,#12
	adr r0,frame_irq_handler
	b_long install_timeout_2

frame_irq_handler:
	ldrb_ r0,wantirq
	orr r0,r0,#IRQ_APU
	strb_ r0,wantirq
	b_long CheckI
	






@------------------------------------------------------------------------------
@   Register Writes
@------------------------------------------------------------------------------
@        Square 1
@------------------------------------------------------------------------------
@$4000   ddle nnnn   duty, loop env/disable length, env disable, vol/env period
@------------------------------------------------------------------------------
_4000w:
	strb_ r0,reg_4000
	@apply duty cycle change immediately
	mov r1,#REG_BASE
	and r2,r0,#0xC0
	strb r2,[r1,#REG_SOUND1CNT_H]
update_sq1_volume:
	ldrb_ r0,reg_4000
	tst r0,#0x10		@envelope disable bit
	ldreqb_ r0,sq1_env	@read for envelope if enabled, otherwise use low 4 bits
	mov r0,r0,lsl#4		@shift left for GBA hardware
	and r0,r0,#0xF0
update_sq1_volume_r0_env:
	ldrb_ r1,sq1_length	@set volume to zero if length counter is 0
	movs r1,r1
	moveq r0,#0
update_sq1_volume_r0:
	@does period and sweep silence the channel?
	ldr_ r2,sq1_period
	ldrb_ r1,reg_4001
	tst r1,#0x08
	andeq r1,r1,#7
	addeq r1,r2,r2,lsr r1
	movne r1,r2
	cmp r1,#2048
	movge r0,#0
	cmp r2,#7
	movle r0,#0	
	
	mov r1,#REG_BASE
	ldrb r2,[r1,#REG_SOUND1CNT_H+1]
	strb r0,[r1,#REG_SOUND1CNT_H+1]	@set next volume to trigger at timer IRQ
	cmp r0,r2
	bxeq lr				@no interrupts if volume not changed
	@if either now or previous was zero volume, write it immediately
	movs r0,r0
	movnes r2,r2
	beq write_square_1
	ldr_ r0,gba_timer_2
	orr r0,r0,#0x400000
	str r0,[r1,#REG_TM2CNT_L]	@enable timer IRQ
	bx lr
	
@------------------------------------------------------------------------------
@$4001   eppp nsss   enable sweep, period, negative, shift
@------------------------------------------------------------------------------
_4001w:
	strb_ r0,reg_4001
	mov r1,#0xFF
	strb_ r1,reg_4001_written
	bx lr

@------------------------------------------------------------------------------
@$4002   pppp pppp   period low
@------------------------------------------------------------------------------
_4002w:
	strb_ r0,reg_4002
	strb_ r0,sq1_period
update_sq1_period:
	ldr_ r2,sq1_period
update_sq1_period_r2:
	@period values >= 0x800 after shift or <= 7 will be silenced by the jump to update_volume
	mov r1,   #0x80000000
	add r1,r1,#0x00080000 @r1 = 0x80080000
	ldr_ r0,freq_mult  @0xFFED408C NTSC, 0xFFEBD175 PAL
	mlas r2,r0,r2,r1
	movmis r1,r2,lsl#1
	movmi r2,#0
	mov r2,r2,lsr#20
	cmp r2,#0x0800
	subeq r2,r2,#1  @2048 -> 2047
	add r0,r2,#0x8000
	str_ r0,gba_sound1cnt_x	@set next sound value
	add r0,r2,#0xF000
	add r0,r0,r2
	add r0,r0,#0x810000
	str_ r0,gba_timer_2		@set next timer value
	orr r0,r0,#0x400000
	mov r1,#REG_BASE
	str r0,[r1,#REG_TM2CNT_L]	@enable timer IRQ
	b update_sq1_volume
	
	global_func update_sq1_period
	
@------------------------------------------------------------------------------
@$4003   llll lppp   length index, period high
@------------------------------------------------------------------------------
_4003w:
	strb_ r0,reg_4003
	mov r2,#0xFF
	strb_ r2,reg_4003_written
	
	ldrb_ r1,channel_enables
	ands r2,r1,#0x01
	adrne r2,length_counter_table
	and r0,r0,#0xFF
	ldrneb r2,[r2,r0,lsr#3]
	strb_ r2,sq1_length
	
	and r2,r0,#0x07
	strb_ r2,sq1_period+1
	mov r12,lr
	bl update_sq1_period	@also jumps to update_sq1_volume
	mov lr,r12
	b write_square_1  @reset the square phase

length_counter_table:
	.byte 0x0A, 0xFE, 0x14, 0x02, 0x28, 0x04, 0x50, 0x06
	.byte 0xA0, 0x08, 0x3C, 0x0A, 0x0E, 0x0C, 0x1A, 0x0E 
	.byte 0x0C, 0x10, 0x18, 0x12, 0x30, 0x14, 0x60, 0x16
	.byte 0xC0, 0x18, 0x48, 0x1A, 0x10, 0x1C, 0x20, 0x1E

@------------------------------------------------------------------------------
@        Square 2
@------------------------------------------------------------------------------
@$4004   ddle nnnn   duty, loop env/disable length, env disable, vol/env period
@------------------------------------------------------------------------------
_4004w:
	strb_ r0,reg_4004
	@apply duty cycle change immediately
	mov r1,#REG_BASE
	and r2,r0,#0xC0
	strb r2,[r1,#REG_SOUND2CNT_L]
update_sq2_volume:
	ldrb_ r0,reg_4004
	tst r0,#0x10		@envelope disable bit
	ldreqb_ r0,sq2_env	@read for envelope if enabled, otherwise use low 4 bits
	mov r0,r0,lsl#4		@shift left for GBA hardware
update_sq2_volume_r0_env:
	ldrb_ r1,sq2_length	@set volume to zero if length counter is 0
	movs r1,r1
	moveq r0,#0
update_sq2_volume_r0:
	@does period and sweep silence the channel?
	ldr_ r2,sq2_period
	ldrb_ r1,reg_4005
	tst r1,#0x08
	andeq r1,r1,#7
	addeq r1,r2,r2,lsr r1
	movne r1,r2
	cmp r1,#2048
	movge r0,#0
	cmp r2,#7
	movle r0,#0	
	
	mov r1,#REG_BASE
	ldrb r2,[r1,#REG_SOUND2CNT_L+1]
	strb r0,[r1,#REG_SOUND2CNT_L+1]	@set next volume to trigger at timer IRQ
	and r0,r0,#0xF0
	cmp r0,r2
	bxeq lr
	@if either now or previous was zero volume, write it immediately
	movs r0,r0
	movnes r2,r2
	beq write_square_2
	ldr_ r0,gba_timer_3
	orr r0,r0,#0x400000
	str r0,[r1,#REG_TM3CNT_L]	@enable timer IRQ
	bx lr

@------------------------------------------------------------------------------
@$4005   eppp nsss   enable sweep, period, negative, shift
@------------------------------------------------------------------------------
_4005w:
	strb_ r0,reg_4005
	mov r1,#0xFF
	strb_ r1,reg_4005_written
	bx lr

@------------------------------------------------------------------------------
@$4006   pppp pppp   period low
@------------------------------------------------------------------------------
_4006w:
	strb_ r0,reg_4006
	strb_ r0,sq2_period
update_sq2_period:
	ldr_ r2,sq2_period
update_sq2_period_r2:
	@period values >= 0x800 after shift or <= 7 will be silenced by the jump to update_volume
	mov r1,   #0x80000000
	add r1,r1,#0x00080000 @r1 = 0x80080000
	ldr_ r0,freq_mult  @0xFFED408C NTSC, 0xFFEBD175 PAL
	mlas r2,r0,r2,r1
	movmis r1,r2,lsl#1
	movmi r2,#0
	mov r2,r2,lsr#20
	cmp r2,#0x0800
	subeq r2,r2,#1  @2048 -> 2047
	add r0,r2,#0x8000
	str_ r0,gba_sound2cnt_h	@set next sound value
	add r0,r2,#0xF000
	add r0,r0,r2
	add r0,r0,#0x810000
	str_ r0,gba_timer_3		@set next timer value
	orr r0,r0,#0x400000
	mov r1,#REG_BASE
	str r0,[r1,#REG_TM3CNT_L]	@enable timer IRQ
	b update_sq2_volume
	
	global_func update_sq2_period
	
@------------------------------------------------------------------------------
@$4007   llll lppp   length index, period high
@------------------------------------------------------------------------------
_4007w:
	strb_ r0,reg_4007
	mov r2,#0xFF
	strb_ r2,reg_4007_written
	
	ldrb_ r1,channel_enables
	ands r2,r1,#0x02
	adrne r2,length_counter_table
	and r0,r0,#0xFF
	ldrneb r2,[r2,r0,lsr#3]
	strb_ r2,sq2_length

	and r2,r0,#0x07
	strb_ r2,sq2_period+1
	mov r12,lr
	bl update_sq2_period	@also calls update_sq2_volume
	mov lr,r12
	b write_square_2  @reset the square phase

@------------------------------------------------------------------------------
@        Triangle
@------------------------------------------------------------------------------
@$4008   clll llll   control, linear counter load
@------------------------------------------------------------------------------
_4008w:
	strb_ r0,reg_4008
	bx lr
@------------------------------------------------------------------------------
@$400A   pppp pppp   period low
@------------------------------------------------------------------------------
_400Aw:
	strb_ r0,reg_400A
	strb_ r0,tri_period
update_tri_period:
	ldr_ r2,tri_period
	mov r1,   #0x80000000
	add r1,r1,#0x00080000 @r1 = 0x80080000
	ldr_ r0,freq_mult  @0xFFED408C NTSC, 0xFFEBD175 PAL
	mlas r2,r0,r2,r1
	movmis r1,r2,lsl#1
	movmi r2,#0
	mov r2,r2,lsr#20
	cmp r2,#0x0800
	subeq r2,r2,#1  @2048 -> 2047
	mov r1,#REG_BASE
	strh r2,[r1,#REG_SOUND3CNT_X]
	bx lr
	global_func update_tri_period

@------------------------------------------------------------------------------
@$400B   llll lppp   length index, period high
@------------------------------------------------------------------------------
_400Bw:
	strb_ r0,reg_400B
	
	mov r2,#0xFF
	strb_ r2,reg_400B_written
	
	ldrb_ r1,channel_enables
	ands r2,r1,#0x04
	adrne r2,length_counter_table
	and r0,r0,#0xFF
	ldrneb r2,[r2,r0,lsr#3]
	strb_ r2,tri_length
	
	and r2,r0,#0x07
	strb_ r2,tri_period+1
	
	mov r12,lr
	bl_long update_tri_volume
	mov lr,r12
	b update_tri_period

@------------------------------------------------------------------------------
@        Noise
@------------------------------------------------------------------------------
@$400C   --le nnnn   loop env/disable length, env disable, vol/env period
@------------------------------------------------------------------------------
_400Cw:
	strb_ r0,reg_400C
update_noise_volume:
	ldrb_ r0,reg_400C
	tst r0,#0x10		@envelope disable bit
	ldreqb_ r0,noise_env	@read for envelope if enabled, otherwise use low 4 bits
	mov r0,r0,lsl#4		@shift left for GBA hardware
	and r0,r0,#0xF0
update_noise_volume_r0_env:
	ldrb_ r1,noise_length	@set volume to zero if length counter is 0
	movs r1,r1
	moveq r0,#0
update_noise_volume_r0:
	mov r1,#REG_BASE
	ldrb r2,[r1,#REG_SOUND4CNT_L+1]
	strb r0,[r1,#REG_SOUND4CNT_L+1]	@set next volume to trigger when noise is replayed
	cmp r0,r2
	bxeq lr
	ldr_ r0,gba_sound4cnt_h
	orr r0,r0,#0x8000
	str r0,[r1,#REG_SOUND4CNT_H]	@write noise freq with initial flag
	bx lr
@------------------------------------------------------------------------------
@$400E   s--- pppp   short mode, period index
@------------------------------------------------------------------------------
_400Ew:
	strb_ r0,reg_400E
	and r1,r0,#0x0F
	
	adr r2,noisefreqs
	ldrb r2,[r2,r1]
	
@	@force looped noise mode for low noise frequencies (D,E,F) (breaks Contra?)
@	cmp r1,#0x0D
@	orrge r2,r2,#0x08
	
	tst r0,#0x80
	orrne r2,r2,#8
	
	mov r1,#REG_BASE
	str_ r2,gba_sound4cnt_h
	strh r2,[r1,#REG_SOUND4CNT_H]	@set freq
	
	bx lr

noisefreqs:
	.byte 0x00,0x01,0x05,0x11,0x15,0x17,0x25,0x33,0x34,0x35,0x37,0x45,0x47,0x55,0x65,0x75
@	.byte 2,2,2,3
@	.byte 3,20,22,36
@	.byte 37,39,53,55
@	.byte 69,70,87,103
@0x75,0x65,0x55,0x47,0x45,0x37,0x35,0x34,0x33,0x25,0x17,0x15,0x05,0x11,0x01,0x00

@------------------------------------------------------------------------------
@$400F   llll l---   length index
@------------------------------------------------------------------------------
_400Fw:
	strb_ r0,reg_400F
	mov r2,#0xFF
	strb_ r2,reg_400F_written
	
	ldrb_ r1,channel_enables
	ands r2,r1,#0x08
	adrne r2,length_counter_table
	and r0,r0,#0xFF
	ldrneb r2,[r2,r0,lsr#3]
	strb_ r2,noise_length
	
	b update_noise_volume

@TODO: 4010-4013 (DMC)

@        DMC
@
@$4010   il-- ffff   IRQ enable, loop, frequency index
@$4011   -ddd dddd   DAC
@$4012   aaaa aaaa   sample address
@$4013   llll llll   sample length

@----------------------------------------------------------
_4010w:
@----------------------------------------------------------
	ldrb_ r2,pcmctrl
	strb_ r0,pcmctrl		@bit7=irq enable, bit 6=loop enable, bits 0-3=freq
	tst r0,#0x80
	
	ldreqb_ r1,wantirq
	biceq r1,r1,#IRQ_DMC
	streqb_ r1,wantirq
	
	eor r2,r2,r0
	tst r2,#0x0F
	bxeq lr
	
	and r0,r0,#0x0f
	adr r1,pcmfreq
	add r0,r0,r0
	ldrh r2,[r1,r0]		@lookup PCM freq..
	mov r0,#REG_BASE
	mov r1,#REG_TM0CNT_L
	strh r2,[r0,r1]		@set timer0 rate
	
	stmfd sp!,{lr}
	bl_long run_dmc_plus12
	ldmfd sp!,{lr}

_4010w_entry:
	ldrb_ r0,pcmctrl
	and r0,r0,#0x0F
	adr r1,dmc_period_table_ntsc
	ldrb_ r2,emuflags
	tst r2,#PALTIMING
	addne r0,r0,#16
	add r1,r1,r0,lsl#1
	ldrh r2,[r1]
	str_ r2,dmc_period
	adr r1,dmc_reciprocal_table_ntsc
	ldr r2,[r1,r0,lsl#2]
	str_ r2,dmc_period_reciprocal
	@find next DMC event
	b_long find_next_dmc_event
@----------------------------------------------------------
_4011w:	@Delta Counter load register
@----------------------------------------------------------
	add r0,r0,r0
	and r0,r0,#0xfe
	sub r0,r0,#0x80		@GBA has -128 -> +127
	str_ r0,pcmlevel		@Start level for PCM

@	orr r0,r0,r0,lsl#8
@	orr r0,r0,r0,lsl#16
@	mov r1,#REG_BASE
@	str r0,[r1,#REG_FIFO_B_L]		@Set DA value... doesn't work  :(

	mov pc,lr
@----------------------------------------------------------
_4012w:	@DMC start address, returns pcmstart (_4015w calls this)
@----------------------------------------------------------
	strb_ r0,pcmctrl+2	@pcm start addr

	and r1,r0,#0xff
	mov r0,#0xc000
	orr r0,r0,r1,lsl#6	@pcm start addr=$C000+x*64
@	adr_ r2,memmap_tbl
@	mov r1,r0,lsr#13
@	ldr r1,[r2,r1,lsl#2]	@lookup rom ptr..
@	add r0,r0,r1
	str_ r0,pcmstart

	mov pc,lr
@----------------------------------------------------------
_4013w:	@DMC length, returns pcmlength (_4015w calls this)
@----------------------------------------------------------
	strb_ r0,pcmctrl+3

	and r0,r0,#0xff
	mov r0,r0,lsl#4
	add r0,r0,#1		@pcm length(bytes)=1+x*16
	str_ r0,pcmlength

	mov pc,lr

@  (GBA CPU / NES CPU*8)*cycles
@-(16777216/14318180)*N
pcmfreq: .hword 0xf054 @N=3424
	.hword 0xf216 @3040
	.hword 0xf38d @2720
	.hword 0xf449 @2560
	.hword 0xf588 @2288
	.hword 0xf6b4 @2032
	.hword 0xf7ba @1808
	.hword 0xf82a @1712
	.hword 0xf90b @1520
	.hword 0xfa25 @1280
	.hword 0xfacd @1136
	.hword 0xfb51 @1024
	.hword 0xfc1f @848
	.hword 0xfce4 @680
	.hword 0xfd5e @576
	.hword 0xfe06 @432

@period table * 3 for NTSC
dmc_period_table_ntsc:
	.hword 0x504,0x474,0x3FC,0x3C0,0x35A,0x2FA,0x2A6,0x282
	.hword 0x23A,0x1E0,0x1AA,0x180,0x13E,0x0FC,0x0D8,0x0A2

@period table * 3 for PAL
dmc_period_table_pal:
	.hword 0x4AA,0x426,0x3B4,0x37E,0x33C,0x2C4,0x276,0x252
	.hword 0x210,0x1BC,0x18C,0x162,0x126,0x0EA,0x0C6,0x096

@period * 3 reciprocal table for NTSC
dmc_reciprocal_table_ntsc:
	.word 0x00330A5F,0x00397CDC,0x00404041,0x00444445,0x004C61DE,0x00560159,0x0060A929,0x006614BD
	.word 0x0072F9B7,0x00888889,0x0099D723,0x00AAAAAB,0x00CE168B,0x01041042,0x012F684C,0x01948B10

@period * 3 reciprocal table for PAL
dmc_reciprocal_table_pal:
	.word 0x0036E346,0x003DB5C2,0x0045217D,0x00494E76,0x004F2657,0x005C90A2,0x00680681,0x006E5479
	.word 0x007C1F08,0x00939A86,0x00A57EB6,0x00B92144,0x00DEE95D,0x01181182,0x014AFD6B,0x01B4E81C

@------------------------------------------------------------------------------
@$4015   ---d nt21   length ctr enable: DMC, noise, triangle, pulse 2, 1
@------------------------------------------------------------------------------
_4015w:
	ldrb_ r2,wantirq
	bics r2,r2,#IRQ_DMC
	@do we need a dummy read to clear frame IRQ?  (for abs,X and abs,y store instructions)
	@tst r2,#IRQ_APU
	bne _4015w_check_dummy_read
_4015w_check_dummy_read_done:
	strb_ r2,wantirq
	
	ldrb_ r1,channel_enables
	and r1,r1,#0x0F
	ldr_ r2,dmc_length_counter
	movs r2,r2
	orrne r1,r1,#0x10
	strb_ r0,channel_enables
	eors r12,r1,r0
	bxeq lr
	stmfd sp!,{lr}
	ands r12,r12,r1
	beq 0f
	@turn off squares, triangle, noise if bits of r12 are set
	mov r0,#0
	tst r12,#0x01
	strneb_ zero_byte,sq1_length
	blne update_sq1_volume_r0	@preserves r12, r0 = #0
	mov r0,#0
	tst r12,#0x02
	strneb_ zero_byte,sq2_length
	blne update_sq2_volume_r0
	tst r12,#0x04
	strneb_ zero_byte,tri_length
	blne tri_off
	mov r0,#0
	tst r12,#0x08
	strneb_ zero_byte,noise_length
	blne update_noise_volume_r0
0:
	@DMC stuff
	ldrb_ r0,channel_enables
	ldr_ r1,dmc_length_counter
	movs r1,r1
	ldrb_ r2,pcmctrl+1
	and r2,r2,#0x0F
	orrne r2,r2,#0x10
	eors r2,r2,r0
	strb_ r0,pcmctrl+1
	@DMC enable/disable unchanged?
	tst r2,#0x10
	beq 0f	@return
	@DMC stopping?
	tst r0,#0x10
	bne 1f
	@stopping: stop the GBA code from playing any more
	moveq r1,#-1
	streq_ r1,pcmcount
	@simulate this in the DMC channel too
	bl_long run_dmc_plus12
	mov r0,#0
	str_ r0,dmc_length_counter
	b 0f	@return
1:	
	@turning on DMC - simulate up to now first
	bl_long run_dmc_plus12
	
	ldrb_ r0,pcmctrl+2
	bl _4012w
	str_ r0,pcmcurrentaddr
	
	ldrb_ r0,pcmctrl+3
	bl _4013w
	str_ r0,pcmcount
	
	cmp r0,#50			@if the sample is less then 50 bytes it's not a sound.
	ldrpl r1,=REG_BASE + REG_TM0CNT_L
	movpl r2,#0x80
	strplh r2,[r1,#2]		@timer 0 on
	
	@if no extra byte, buffer is empty, fetch byte now
	ldrb_ r1,dmc_extra_byte
	movs r1,r1
	mov r2,#1
	strb_ r2,dmc_extra_byte
	subeq r0,r0,#1
	str_ r0,dmc_length_counter
@	moveq r2,#1
	bleq_long dmc_steal_cycles
	bl_long find_next_dmc_event
0:
	ldmfd sp!,{pc}

_4015w_check_dummy_read:
	ldrb r1,[m6502_pc,#-3]
	@look for 9D or 99
	cmp r1,#0x9D
	cmpne r1,#0x99
	biceq r2,r2,#IRQ_APU
	b _4015w_check_dummy_read_done


@------------------------------------------------------------------------------
@$4017   fd-- ----   5-frame cycle, disable frame interrupt
@------------------------------------------------------------------------------
_4017w:
	and r0,r0,#0xC0
	strb_ r0,frame_mode
	
	@clear IRQ when disabling it
	tst r0,#0x40
	ldrneb_ r1,wantirq
	bicne r1,r1,#IRQ_APU
	strneb_ r1,wantirq
	
	tst r0,#0x80
	movne r1,#0
	moveq r1,#1
	strb_ r1,frame_sequencer_step
	
	@even-odd apu clock jitter
	@get timestamp
	ldr_ r0,cycles_to_run
	sub r0,r0,cycles,asr#CYC_SHIFT
	ldr_ r1,timestamp
	add r1,r1,r0
	
	@get previous frame sequencer timeout (value in the future)
	ldr_ r2,frame_sequencer_timestamp
	sub r0,r2,r1
	@r0 = cycles until next timeout
	ldr r2,=0x5556
	mul r2,r0,r2
	mov r2,r2,lsr#16
	and r2,r2,#1
	add r2,r2,r2,lsl#1 @multiply by 3
	
	@no subtracting 12 cycles because we got here 12 cycles early already.
@	addne r2,#0
	ldreq_ r0,frame_sequencer_interval
	addeq r2,r2,r0
	
	add r1,r1,r2
	adrl_ r12,frame_sequencer_timeout
	ldr r0,=frame_sequencer_handler
	b_long replace_timeout_2

@------------------------------------------------------------------------------
@$4015   if-d nt21   DMC IRQ, frame IRQ, length counter statuses
@------------------------------------------------------------------------------
@DO NOT TOUCH R12 (addy), it needs to be preserved for Read-Modify-Write instructions
_4015r:
	ldrb_ r0,wantirq
	bic r1,r0,#IRQ_APU  @clear APU frame irq, but not DMC irq
	strb_ r1,wantirq
	and r0,r0,#0xC0
	
	ldrb_ r1,sq1_length
	movs r1,r1
	orrne r0,r0,#0x01
	ldrb_ r1,sq2_length
	movs r1,r1
	orrne r0,r0,#0x02
	ldrb_ r1,tri_length
	movs r1,r1
	orrne r0,r0,#0x04
	ldrb_ r1,noise_length
	movs r1,r1
	orrne r0,r0,#0x08
	
	@DMC status
	ldr_ r1,dmc_length_counter
	movs r1,r1
	orrne r0,r0,#0x10
	
	bx lr

@        Square 1/Square 2
@
@$4000/4 ddle nnnn   duty, loop env/disable length, env disable, vol/env period
@$4001/5 eppp nsss   enable sweep, period, negative, shift
@$4002/6 pppp pppp   period low
@$4003/7 llll lppp   length index, period high
@
@        Triangle
@
@$4008   clll llll   control, linear counter load
@$400A   pppp pppp   period low
@$400B   llll lppp   length index, period high
@
@        Noise
@
@$400C   --le nnnn   loop env/disable length, env disable, vol/env period
@$400E   s--- pppp   short mode, period index
@$400F   llll l---   length index
@
@        DMC
@
@$4010   il-- ffff   IRQ enable, loop, frequency index
@$4011   -ddd dddd   DAC
@$4012   aaaa aaaa   sample address
@$4013   llll llll   sample length
@
@        Common
@
@$4015   ---d nt21   length ctr enable: DMC, noise, triangle, pulse 2, 1
@$4017   fd-- ----   5-frame cycle, disable frame interrupt
@
@        Status (read)
@
@$4015   if-d nt21   DMC IRQ, frame IRQ, length counter statuses






	@running X cycles:
	@if x < counter
	@	counter -= x
	@	done
	@else
	@	x -= counter
	@	;get number of periods run
	@	periods = x / period + 1
	@	counter = period - (x % period)
	@
	@running X periods:
	@if x < remaining_bits
	@	remaining_bits -= x
	@	make some noise
	@	done
	@else
	@	x -= remaining_bits
	@	if lengthcounter > 0
	@		steal 4 cpu cycles
	@		lengthcounter -= 1
	@		if lengthcounter == 0 && dmc_irq_enabled
	@			trigger dmc irq
	@	else
	@		remaining_bits = 8 - (x % 8)

	.pushsection .text, "ax", %progbits
run_dmc_plus12:
	ldr_ r2,cycles_to_run
	add r2,r2,#12
	b 0f
run_dmc:
	ldr_ r2,cycles_to_run
0:
	sub r2,r2,cycles,asr#CYC_SHIFT
	ldr_ r1,timestamp
	add r1,r1,r2
	
	@r1 = timestamp
	ldr_ r2,dmc_last_timestamp
	subs r0,r1,r2
	bxmi lr
	str_ r1,dmc_last_timestamp
	
	@r0 = X cycles to simulate
	@are we simulating more cycles than are remaining on the tick counter?
	ldr_ r1,dmc_tick_counter
	subs r2,r1,r0
	@r2 = tick counter - x
	@if (ticks - x) > 0, the tick counter does not reach zero, so store the new counter value, and we're done.
	strgt_ r2,dmc_tick_counter
	bxgt lr
	
	@x -= counter
	sub r0,r0,r1
	@periods to run = (x / period)
	ldr_ r1,dmc_period_reciprocal
	umull r12,r2,r0,r1
	@r2 = periods to run (add 1)
	@now set counter
	ldr_ r1,dmc_period
	@want r1 - (r0 - r2 * r1)
	mul r12,r2,r1
	subs r0,r0,r12
	sub r1,r1,r0
	str_ r1,dmc_tick_counter
	
	add r2,r2,#1
	@r2 = number of periods to run
	@if remaining_bits > periods, remaining_bits -= periods, done
	ldrb_ r0,dmc_remaining_bits
	subs r1,r0,r2
	strgtb_ r1,dmc_remaining_bits
	bxgt lr
	@else, periods to run -= remaining_bits
	sub r2,r2,r0
	@remaining bits = 8 - r2 & 7
	and r0,r2,#7
	rsb r0,r0,#8
	strb_ r0,dmc_remaining_bits
	
	@number of bytes = r2 >> 3
	mov r2,r2,lsr#3
	add r2,r2,#1
	
	@is length counter 0 and extra byte 0?  quit
	ldr_ r0,dmc_length_counter
	ldrb_ r1,dmc_extra_byte
	adds r1,r0,r1
	bxeq lr
	@length counter 0 and extra byte 1?  Set extra byte to 0 and quit.  No cycle theft.
	movs r0,r0
	streqb_ r0,dmc_extra_byte
	bxeq lr
	
	@length counter (r0) > times to clock it (r2)?  Branch ahead.
	subs r1,r0,r2
	bgt 0f
	@dmc length counter runs out unless we are in loop mode
	ldrb_ r12,pcmctrl
	and r12,r12,#0xC0
	tst r12,#0x40
	@if not looping, times to clock it is limited to length counter value
	bne 1f

	@not looping: force DMC length counter and extra byte to correct values (if we're using the code that skips stuff)
	movs r1,r1
	mov r1,#0
	strmib_ r1,dmc_extra_byte
	ldr_ r2,dmc_length_counter
	
	@check for IRQ enabled
	cmp r12,#0x80
	bne 0f
	@trigger an IRQ
	ldrb_ r12,wantirq
	orr r12,r12,#IRQ_DMC
	strb_ r12,wantirq
	
	b 0f
1:
	@set new length counter value to sample length - ticks
	ldr_ r12,pcmlength
	movs r12,r12  @force no division by zero
	moveq r1,#1
2:
	adds r1,r1,r12
	ble 2b
0:
	str_ r1,dmc_length_counter
dmc_steal_cycles:
	.if FULL_DMC
	@steal 4 CPU cycles for each byte removed from length counter
	@r2 = number of bytes to fetch
	@multiply by 3 for PPU cycles
	add r2,r2,r2,lsl#1
	@4 cycles to steal
	mov r2,r2,lsl#2
	sub cycles,cycles,r2,lsl#CYC_SHIFT
	.endif
	bx lr
	
find_next_dmc_event:
	@called after calling run_dmc
	@if dmc length counter == 0, no events
	@next timestamp = dmc_last_timestamp + dmc_tick_counter + (dmc_remaining_bits - 1) * dmc_period
	@alternative: dmc_last_timestamp + dmc_tick_counter + (dmc_remaining_bits - 1 + dmc_length_counter * 8) * dmc_period

@	@fast version: no interrupts - not interested
@	ldrb_ r0,pcmctrl
@	and r0,r0,#0xC0
@	cmp r0,#0x80
@	bxne lr

	
	ldr_ r2,dmc_length_counter
	subs r2,r2,#1
	bxmi lr
	ldrb_ r0,dmc_remaining_bits
	sub r0,r0,#1
	and r0,r0,#7
	.if !FULL_DMC
	add r0,r0,r2,lsl#3
	.endif
	ldr_ r2,dmc_period
	ldr_ r1,dmc_tick_counter
	mla r12,r0,r2,r1
	ldr_ r1,dmc_last_timestamp
	add r1,r1,r12
	ldr r0,=dmc_handler
	ldr r12,=_dmc_timeout
	@sub r1,r1,#12
	@r0 = handler, r1 = timestamp of next DMC byte fetch, r12 = timeout
	b_long replace_timeout_2

dmc_handler:
	bl run_dmc
	bl find_next_dmc_event
	b_long check_irq
	.popsection

	.text
@----------------------------------------------------------------------------
Sound_hardware_reset:
@----------------------------------------------------------------------------

@	stmfd sp!,{r4-r8,globalptr,lr}
@	ldr globalptr,=GLOBAL_PTR_BASE
	
	mov r1,#REG_BASE
	
	ldrh r0,[r1,#REG_SOUNDBIAS]
	bic r0,r0,#0xc000			@just change bits we know about.
	orr r0,r0,#0x8000			@PWM 7-bit 131.072kHz
	strh r0,[r1,#REG_SOUNDBIAS]
	
	mov r0,#0x80
	strh r0,[r1,#REG_SOUNDCNT_X]	@sound master enable
	
	ldr r0,=0xB00EFF77			@Enable all basic channels, output ratio=full range.  use directsound B, timer 0
	str r0,[r1,#REG_SOUNDCNT_L]
	
	@square0, square1 reset
	mov r0,#0x08
	str r0,[r1,#REG_SOUND1CNT_L]  @sweep off, initial volume 0
	str r0,[r1,#REG_SOUND2CNT_L]
	mov r0,#0x8000
	str r0,[r1,#REG_SOUND1CNT_X]  @play sound (with volume 0)
	str r0,[r1,#REG_SOUND2CNT_H]
	
@@	str r0,sweepctrl
@@	ldr r0,=0x10001010
@@	str r0,soundctrl			@volume=0
@@	mov r0,#0xffffff00
@@	str r0,soundmask
	
	@triangle reset
	mov r0,#0x40
	str r0,[r1,#REG_SOUND3CNT_L]  @disable triangle channel, select playback bank 1 (wave ram bank 0 accessible)
	adr r12,trianglewav
	ldmia r12,{r0,r1,r2,r3}
	ldr r12,=REG_BASE + REG_SOUNDWR0_L
	stmia r12,{r0,r1,r2,r3}
	mov r1,#REG_BASE
	mov r0,#0x80
	str r0,[r1,#REG_SOUND3CNT_L]  @enable triangle channel, set playback bank 0, set 0 volume
	mov r0,#0x8000
	str r0,[r1,#REG_SOUND3CNT_X]  @play sound (with zero volume)
	
	@noise reset
	mov r0,#0
	str r0,[r1,#REG_SOUND4CNT_L]  @initial volume 0
	mov r0,#0x8000
	str r0,[r1,#REG_SOUND4CNT_L]  @play channel (with volume 0)
	mov r0,#0
	str r0,[r1,#REG_SOUND4CNT_L]  @set initial volume 0 again (fix no$gba?)
	

@DMC?
	strh r1,[r1,#REG_DM2CNT_H]	@DMA2 stop
	add r0,r1,#REG_FIFO_B_L		@DMA2 destination..
	str r0,[r1,#REG_DM2DAD]
	ldr r0,=PCMWAV
	str r0,[r1,#REG_DM2SAD]		@dmasrc=..
	ldr r0,=0xB640				@noIRQ fifo 32bit repeat incsrc fixeddst
	strh r0,[r1,#REG_DM2CNT_H]	@DMA start
								@PCM reset:
@	mov r0,#-1
@	str_ r0,pcmcount
@	mov r0,#0x00001000
@	str_ r0,pcmctrl
	
	add r1,r1,#REG_TM0CNT_L		@timer 0 controls sample rate:
	mov r0,#0
	strh r0,[r1],#4
@	str_ r0,pcmlevel

	mov r0,#-PCMWAVSIZE		@timer 1 counts samples played:
	strh r0,[r1],#2
	mov r0,#0					@disable timer 1 before enabling it again.
	strh r0,[r1]
	mov r0,#0xc4				@enable+irq+count up
	strh r0,[r1],#2

@	ldmfd sp!,{r4-r8,globalptr,lr}

	bx lr

sound_reset:
	stmfd sp!,{lr}
	mov r0,#8
	strb_ r0,dmc_remaining_bits
	mov r0,#3  @set to 1 CPU cycle to match Nintendulator
	str_ r0,dmc_tick_counter
	bl _4010w_entry
	
	ldmfd sp!,{pc}
	

trianglewav:				@Remember this is 4-bit
	.byte 0x76,0x54,0x32,0x10,0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0xFE,0xDC,0xBA,0x98

reset_sound_after_loadstate:
	stmfd sp!,{r3-r12,lr}
	ldr r10,=GLOBAL_PTR_BASE
	@update sound
	ldrb_ r0,reg_4000
	bl _4000w @update sq1 duty
	
	bl update_sq1_period
	bl write_square_1
	
	ldrb_ r0,reg_4004
	bl _4004w @update sq2 duty
	
	bl update_sq2_period
	bl write_square_2
	
	bl update_tri_period
	
	bl update_noise_volume
	ldrb_ r0,reg_400E
	bl _400Ew  @update noise frequency
	
	bl _4010w_entry

	ldmfd sp!,{r3-r12,lr}
	bx lr

 .section .data.103, "w", %progbits
sound_state:
_reg_4000: .byte 0
_reg_4001: .byte 0
_reg_4002: .byte 0
_reg_4003: .byte 0
_reg_4004: .byte 0
_reg_4005: .byte 0
_reg_4006: .byte 0
_reg_4007: .byte 0
_reg_4008: .byte 0
 .byte 0
_reg_400A: .byte 0
_reg_400B: .byte 0
_reg_400C: .byte 0
 .byte 0
_reg_400E: .byte 0
_reg_400F: .byte 0

_frame_sequencer_step: .byte 0
_sq1_length: .byte 0
_reg_4001_written: .byte 0
_reg_4003_written: .byte 0

_frame_mode: .byte 0 
_sq2_length: .byte 0
_reg_4005_written: .byte 0
_reg_4007_written: .byte 0

_tri_linear: .byte 0
_tri_length: .byte 0
 .byte 0
_reg_400B_written: .byte 0

_channel_enables: .byte 0
_noise_length: .byte 0
 .byte 0
_reg_400F_written: .byte 0

_sq1_env: .byte 0
_sq1_env_delay: .byte 0
_sq1_sweep_delay: .byte 0
_sq1_neg_offset: .byte 0

_sq2_env: .byte 0
_sq2_env_delay: .byte 0
_sq2_sweep_delay: .byte 0
_sq2_neg_offset: .byte 0

_noise_env: .byte 0
_noise_env_delay: .byte 0
 .byte 0
 .byte 0

_sq1_period: .word 0
_sq2_period: .word 0
_tri_period: .word 0

_pcmctrl: .word 0		@bit7=irqen, bit6=loop.  bit 12=PCM enable (from $4015). bits 8-15=old $4015
_pcmlength: .word 0		@total bytes
_pcmcount: .word 0		@bytes remaining
_pcmlevel: .word 0

@internal pointers not in sound_state

_pcmstart: .word 0		@starting addr
_pcmcurrentaddr: .word 0	@current addr, either NES address or Memory address

_gba_sound1cnt_x: .word 0
_gba_sound2cnt_h: .word 0
_gba_timer_2: .word 0
_gba_timer_3: .word 0
_gba_sound4cnt_h: .word 0

_freq_mult:	.word 0
_frame_sequencer_interval: .word 0

_dmc_last_timestamp: .word 0
_dmc_tick_counter: .word 0
_dmc_period: .word 0
_dmc_period_reciprocal: .word 0

_dmc_length_counter: .word 0

_dmc_remaining_bits: .byte 0
_dmc_extra_byte: .byte 0
	.byte 0,0
	
	.word 0,0


.global _dmc_last_timestamp
.global _dmc_tick_counter
.global _dmc_period
.global _dmc_period_reciprocal
.global _dmc_length_counter
.global _dmc_remaining_bits
.global _pcmctrl

	.global _sq1_period

	@.end
