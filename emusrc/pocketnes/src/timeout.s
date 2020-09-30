#include "equates.h"

 .align
 .pool
 .text
 .align
 .pool

@	.include "equates.h"
@	.include "mem.h"
@	.include "6502mac.h"
@	.include "cart.h"
@	.include "memory.h"
@	.include "io.h"
@	.include "ppu.h"
@	.include "sound.h"
@	.include "6502.h"
@	.include "speedhack_asm.h"

	@IMPORT ui	@ui.c
	.if SAVESTATES
	@IMPORT quickload	@sram.c
	@IMPORT quicksave	@sram.c
	.endif
	@IMPORT _pcmctrl

	@IMPORT quickhackfinder
	@IMPORT setjmp0hack

	.if CHEATFINDER
	@IMPORT do_cheats
	.endif

	global_func addcycles
@	global_func add0cycles
	global_func get_scanline_2

	global_func ntsc_pal_reset
	global_func run
	.global sleeptime
@	.global novblankwait
	.global dontstop

	.global _cycles_to_run
	.global _timestamp
	
	global_func timeout_reset
	global_func timeout_handler
	global_func timeout_handler_branch
	
@	global_func install_timeout
	global_func install_timeout_2
@	global_func install_timeout_quick

	global_func replace_timeout_2
	global_func remove_timeout
	
	global_func nmi_handler
	.global TimeoutState
	.global _cycles_to_run
	.global _crash_disabled
	
	.global _mapper_timeout
	.global _mapper_irq_timeout
	global_func timeout_handler_branch_taken
	.global _dmc_timeout


@Called from C code, from the rom menu.  Can either return after frame, or run forever.
@----------------------------------------------------------------------------
run:	@r0=0 to return after frame
@----------------------------------------------------------------------------
	tst r0,#1
	
	stmeqfd sp!,{m6502_nz-m6502_pc,globalptr,cpu_zpage,lr}

	ldr globalptr,=GLOBAL_PTR_BASE
	ldr cpu_zpage,=NES_RAM

	strb_ r0,_dontstop
	
	@If we are running a compressed game, we may have loaded a compressed SRAM file.  If we did, reload the compressed ROM.
	@IMPORT ewram_owner_is_sram
	@IMPORT redecompress
	ldr r2,=ewram_owner_is_sram
	ldrb r2,[r2]
	movs r2,r2
	ldrne r1,=redecompress
	movne lr,pc
	bxne r1
@@	ldr r1,=redecompress
@@	mov lr,pc
@@	bx r1

	mov r1,#0
	strb_ r1,novblankwait_

	b line0x
@----------------------------------------------------------------------------
@cycles ran out
@----------------------------------------------------------------------------

prerender_line_handler:
line0:
	adr_ r2,cpuregs
	stmia r2,{m6502_nz-m6502_pc}	@save 6502 state
	
	mov r0,#1
	strb_ r0,crash_disabled
	mov r0,#0
	strb_ r0,crash_framecounter

@multiplayer stuff (probably needs redoing later)
waitformulti:
	ldr r1,=REG_P1				@refresh input every frame
	ldrh r0,[r1]
		eor r0,r0,#0xff
		eor r0,r0,#0x300		@r0=button state (raw)
	ldr_ r1,AGBjoypad
	eor r1,r1,r0
	and r1,r1,r0				@r1=button state (0->1)
	str_ r0,AGBjoypad

	@Unscaled mode Up and Down on L and R
	ldrb_ r3,emuflags+1
	cmp r3,#SCALED
	bhs 0f						@if unscaled
	ldrb_ r3,windowtop
	tst r0,#0x100				@R=scroll down
	addne r3,r3,#2
	cmp r3,#(240-SCREEN_HEIGHT)
	movgt r3,#(240-SCREEN_HEIGHT)
	tst r0,#0x200				@L=scroll up
	subnes r3,r3,#2
	movmi r3,#0
	strb_ r3,windowtop
0:
	ldrb_ r2,_dontstop
	cmp r2,#0
	ldmeqfd sp!,{m6502_nz-m6502_pc,globalptr,cpu_zpage,lr}	@exit here if doing single frame:
	bxeq lr						@return to rommenu()

	@----anything from here til line0x won't get executed while rom menu is active---

	mov r2,#REG_BASE
	mov r3,#0x0110
	strh r3,[r2,#REG_BLDCNT]	@stop darkened screen,OBJ blend to BG0
	mov r3,#0x1000				@BG0=16, OBJ=0
	strh r3,[r2,#REG_BLDALPHA]	@Alpha values

	adr lr,line0x				@return here after doing L/R + SEL/START

	tst r1,#0x300				@if L or R was pressed
	tstne r0,#0x100
	tstne r0,#0x200				@and both L+R are held..
	ldrne r1,=ui
	bxne r1						@do menu

	ands r3,r0,#0x300			@if either L or R is pressed (not both)
	eornes r3,r3,#0x300
	bicne r0,r0,#0x0c			@hide sel,start from EMU
	str_ r0,NESjoypad
	beq line0x					@skip ahead if neither or both are pressed

	tst r0,#0x200
	tstne r1,#4					@L+SEL for BG adjust
	ldrneb_ r2,adjustblend
	addne r2,r2,#1
	strneb_ r2,adjustblend

	tst r0,#0x200				@L?
	tstne r1,#8					@START?
	ldrb_ r2,novblankwait_		@0=Normal, 1=No wait, 2=Slomo
	addne r2,r2,#1
	cmp r2,#3
	moveq r2,#0
	strb_ r2,novblankwait_
	
	.if SAVESTATES
	tst r0,#0x100				@R?
	tstne r1,#8					@START:
	ldrne r1,=quickload
	bxne r1

	tst r0,#0x100				@R?
	tstne r1,#4					@SELECT:
	ldrne r1,=quicksave
	bxne r1
	.endif
line0x:
	ldr r2,=ewram_owner_is_sram
	ldrb r2,[r2]
	movs r2,r2
	ldrne r1,=redecompress
	movne lr,pc
	bxne r1

	bl_long refreshNESjoypads	@Z=1 if communication ok
	bne waitformulti		@waiting on other GBA..

	ldr_ r0,AGBjoypad
	ldr r1,=fiveminutes
	ldr r2,[r1] @fiveminutes		;sleep after 5/10/30 minutes of inactivity
	cmp r0,#0				@(left out of the loop so waiting on multi-link
	ldrne r2,[r1,#4] @sleeptime		;doesn't accelerate time)
	subs r2,r2,#1
	str r2,[r1] @fiveminutes
	bleq suspend

	mov r0,#0
	strb_ r0,crash_disabled
	
	ldr_ r0,frame
	add r0,r0,#1
	str_ r0,frame
@End of Line 0 emulator UI tasks

@Now for the stuff that concerns the NES

	@Set palette as dirty, so the palette gets copied when the screen turns on
	mov r0,#1
	strb_ r0,palette_dirty

	@turn off inside_vblank
	mov r0,#0
	strb_ r0,inside_vblank
	
	@clear VBL flag, and use the optimized ppustat reader
	ldr r1,=ppustat_
	mov r0,#0
	strb r0,[r1]
	ldr_ r0,stat_r_simple_func
	ldr r1,=PPU_read_tbl+8
	str r0,[r1]
	
@	mov r1,#0
@	strb r1,ppustat_		@vbl clear, sprite0 clear
@	mov r1,#0
@	str_ r1,scanline			@reset scanline count

@	bl_long newframe				@display update (this function withered away to nothing, so it's removed)
@	bl updatesound

	.if CHEATFINDER
	@IMPORT num_cheats
	ldr r1,=num_cheats
	ldr r1,[r1]
	movs r1,r1
	beq 0f
	ldr r0,=do_cheats
	mov lr,pc
	bx r0
0:
	.endif

	ldr_ r0,fpsvalue
	add r0,r0,#1
	str_ r0,fpsvalue

	adr_ r0,cpuregs
	ldmia r0,{m6502_nz-m6502_pc}	@restore 6502 state

@	mov r2,#1
@	mov r0,#2
@	bl call_quickhackfinder
@@	bl set_cpu_hack


@	@even odd frame  (NTSC only)
@	ldr_ r1,emuflags
@	and r2,r1,#PALTIMING
@	eors r2,r2,#PALTIMING
@	ldrneb_ r2,ppuctrl1
@	tstne r2,#0x18
@	ldrneb r2,next_visible_frame_skip_cycle
@	eorne r0,r2,#1
@	strneb r0,next_visible_frame_skip_cycle

	@even odd frame  (NTSC only)
	ldr_ r1,emuflags
	and r2,r1,#PALTIMING
	eors r2,r2,#PALTIMING
	ldrb_ r2,ppuctrl1
	tstne r2,#0x18
	ldrneb_ r2,frame
	andne r2,r2,#1	@r2 = 1 if running one less cycle, 0 otherwise
	
	ldr_ r0,next_frame_timestamp
	sub r0,r0,r2
	str_ r0,frame_timestamp

	tst r1,#PALTIMING
	subeq r2,r0,#96
	subne r2,r0,#91
	str_ r2,frame_timestamp_minus_96
	addeq r2,r0,#245
	addne r2,r0,#228
	str_ r2,frame_timestamp_plus_245
	addeq r2,r0,#128
	addne r2,r0,#120
	str_ r2,frame_timestamp_plus_128	  @not used now, but can make Scanline Number checking faster

	ldr_ r1,cyclesperframe @89342/106392
	add r1,r0,r1
	str_ r1,next_frame_timestamp
	
	sub r1,r1,#12
	ldr r0,=prerender_line_handler
	adrl_ r12,prerender_line_timeout
	bl_long install_timeout_2

	ldr_ r0,frame_timestamp
	ldr_ r2,vblank_time	@242*341 for NTSC, 242*319.6875 for PAL
	add r1,r0,r2
	ldr r0,=vblank_handler
	adrl_ r12,vblank_timeout
	bl_long install_timeout_2
	
	ldr_ r0,frame_timestamp
	ldr_ r2,render_end_time	@241*341 for NTSC, 241*319.6875 for PAL
	add r1,r0,r2
	ldr r0,=render_end_handler
	adrl_ r12,render_end_timeout
	bl_long install_timeout_2
	
	ldr_ r0,frame_timestamp
	ldr_ r2,half_screen_time	@120*341 for NTSC, 120*319.6875 for PAL
	add r1,r0,r2
	ldr r0,=half_screen_handler
	adrl_ r12,half_screen_timeout
	bl_long install_timeout_2
	
	ldr_ r0,frame_timestamp
	ldr_ r2,line_zero_start_time		@304 for NTSC, 285 for PAL
	add r1,r0,r2
	ldr r0,=line_zero_handler
	adrl_ r12,line_zero_timeout
	bl_long install_timeout_2

	@turn screen on if ppuctrl1 says it's ok
	ldrb_ r0,ppuctrl1
	tst r0,#0x18
	blne_long turn_screen_on
	
	b_long _GO

@prerender_line_handler2




queue_dummy_handler:
	adr r0,queue_dummy_handler
	ldr_ r1,queue_dummy_timestamp
	add r1,r1,#0x00700000
	adrl_ r12,queue_dummy_timeout
	bl_long install_timeout_2
	b_long _GO

line_zero_handler:
	bl_long newframe_set0	@should this go here?
	
	ldrb_ r0,screen_off
	movs r0,r0
	bne 0f
	ldr_ r1,loopy_t
	str_ r1,vramaddr
	bl_long UpdateXYScroll_V_equals_T
0:
	b_long _GO
	
half_screen_handler:
	.if DIRTYTILES
	@After half of the screen is rendered, the next time the screen is turned off, gather dirty VRAM tiles.
	mov r0,#1
	strb_ r0,okay_to_run_nes_chr_update_this_frame
	.endif
	
	ldrb_ r0,ppuctrl0
	strb_ r0,ppuctrl0frame		@variable is only used to select 8x8 or 8x16 sprites
	
	b_long _GO

render_end_handler:
	@turn off screen if necessary
	mov r0,#1
	strb_ r0,inside_vblank
	ldrb_ r0,ppuctrl1
	tst r0,#0x18
	beq 0f
	bl turn_screen_off
	b_long _GO
0:
	bl catchups
	b_long _GO


stat_R_suppressvbl:
	strb_ m6502_a,toggle
	mov r1,#1
	strb_ r1,suppress_vbl
	ldr r1,=ppustat_
	ldrb r0,[r1]
	bic r2,r0,#0x80
	strb r2,[r1]
	bx lr

vblank_handler:
	bic cycles,cycles,#BRANCH
	strb_ m6502_a,oam_addr
	
@	ldr_ r1,timestamp
	sub r2,r1,#1
	ldr_ r1,vblank_timestamp
	cmp r1,r2
	add r1,r1,#1
	str_ r1,vblank_timestamp
	bne vblank_handler_0

	adr r0,stat_R_suppressvbl
	ldr r12,=PPU_read_tbl+8
	str r0,[r12]
	adr r0,vblank_handler_0
	adrl_ r12,vblank_timeout
	bl_long install_timeout_2
	b_long _GO
vblank_handler_0:
	stmfd sp!,{r0-r12}
	
	mov r0,#0
	bl call_quickhackfinder
	
	bl_long newframe_nes_vblank
	ldmfd sp!,{r0-r12}
	
	ldr r0,=stat_R_clearvbl
	ldr r2,=PPU_read_tbl+8
	str r0,[r2]

	ldrb_ r0,suppress_vbl
	movs r0,r0
	mov r0,#0
	strneb_ r0,suppress_vbl
	bne 0f
	
	@set VBL flag
	ldr r2,=ppustat_
	ldrb r0,[r2]
	orr r0,r0,#0x80	@set vbl, keep sprite0
	strb r0,[r2]
@0	
	adr r0,vblank_handler_2
	ldr_ r1,vblank_timestamp
	add r1,r1,#2
	adrl_ r12,vblank_timeout
	bl_long install_timeout_2
	b_long _GO
vblank_handler_2:
	@check if NMI is enabled, and if VBL flag is set
	ldrb_ r0,ppuctrl0
	tst r0,#0x80
	ldrne r1,=ppustat_
	ldrneb r0,[r1]
	tstne r0,#0x80
	beq 0f
	adr r0,nmi_handler
	ldr_ r1,vblank_timestamp
	add r1,r1,#12
	adrl_ r12,nmi_timeout
	bl_long replace_timeout_2
0:
	b_long _GO

nmi_handler:
	bic cycles,cycles,#BRANCH
	ldr r12,=NMI_VECTOR
	bl_long Vec6502
	sub cycles,cycles,#3*7*CYCLE
	b_long _GO
	
	
@pcm_scanlinehook
@	ldr addy,=_pcmctrl
@	ldr r2,[addy]
@	tst r2,#0x1000			@Is PCM on?
@	beq_long _GO
@
@	ldr_ r0,pcmirqcount
@@	ldr_ r1,cyclesperscanline
@@	subs r0,r0,r1,lsr#4
@	subs r0,r0,#121			@Fire Hawk=122
@	str_ r0,pcmirqcount
@	bpl_long _GO
@
@	tst r2,#0x40			@Is PCM loop on?
@	ldrne_ r0,pcmirqbakup
@	strne_ r0,pcmirqcount
@	bne_long _GO
@	tst r2,#0x80			@Is PCM IRQ on?
@	orrne r2,r2,#0x8000		@set pcm IRQ bit in R4015
@	bic r2,r2,#0x1000		@clear channel 5
@	str r2,[addy]
@	bne_long CheckI
@	b_long _GO




 .align
 .pool
 .section .iwram, "ax", %progbits
 .subsection 0
 .align
 .pool

@---------------------------------------------------

@linked list:
@	+0 = data (a code pointer)
@	+4 = timestamp
@	+8 = next  (null if no next item)

@install_timeout_b
@	@in: r12=address of node
@	ldr r1,[r12,#4]
@	b install_timeout


@install_timeout_quick
@	@assumes timestamp is correct, and that timeout will happen within r2 cycles
@	@r0 = timeout handler
@	@r1 = timeout timestamp
@	@r12 = timeout node pointer
@	@r2 = timeout relative timestamp
@	stmia r12,{r0,r1}
@	cmp r2,cycles,asr#CYC_SHIFT
@	bgt install_timeout_2
@	ldr_ r0,firsttimeout
@	str_ r12,firsttimeout
@	str r0,[r12,#8]
@	ldr_ r0,timestamp
@	sub r1,r1,r0
@	str_ r1,cycles_to_run
@	and cycles,cycles,#CYC_MASK
@	add cycles,cycles,r1,lsl#CYC_SHIFT
@	bx lr
timeout_in_queue:
	ldr r2,[r12,#4]
	cmp r1,r2
	streq r0,[r12,#0]
	bxeq lr
	stmfd sp!,{r0,r1,lr}
	bl remove_timeout_2
	ldmfd sp!,{r0,r1,lr}
	b install_timeout_2
replace_timeout_2:
	@in: r12=address of node, r0=code pointer, r1=timestamp
	ldr r2,[r12,#8]
	tst r2,#1
	beq timeout_in_queue  @if EQ, timeout is in the queue
install_timeout_2:
	@in: r12=address of node, r0=code pointer, r1=timestamp
	stmia r12,{r0,r1}
install_timeout:
	@psuedocode version:
	@
	@NODE TO INSERT MUST NOT ALREADY BE IN THE LIST
	@
	@left_for_me    = now-me.timestamp
	@left_for_first = now-first.timestamp 
	@
	@if left_for_me<left_for_first then replace the first one
	@else
	@	next=first
	@loop:
	@	if (next.next==null) insert before next.next
	@	left_for_next=now-next.next.timestamp
	@	if (left_for_me<left_for_next) insert before next.next
	@	next=next.next
	@	goto loop
	@
	@insert before next.next:
	@	me.next=next.next
	@	next.next=me
	@replace first:
	@	me.next=first
	@	first=me
	@	@vars: cycles to run, timestamp, "cycles"
	@	  If "cycles" was correctly adjusted before entering here, "cycles" should be equal to "cycles to run"
	@	cycles to run = (left_for_me)
	@	cycles = cycles_to_run << CYC_SHIFT
	@
	
	@in: r12=address of node
	@     r1=timestamp of event
	@destroys r0,r1,r2
	
	@get old timestamp
	ldr_ r2,timestamp
	
	sub r1,r1,r2

	ldr_ r0,cycles_to_run  @old_cycles_to_run = t_first - old_timestamp

	subs r1,r1,r0
	bge 1f

i_t_replace_first:
	@r0 = t_first - old_timestamp
	@r1 = t_me - old_timestamp
	@r2 = old_timestamp
	@t_first = old_timestamp+cycles_to_run
	@want to update cycles_to_run and cycles
	@vars:
	@old_timestamp, old_cycles_to_run, cycles, t_first, t_me
	@
	@now = old_timestamp+old_cycles_to_run-cycles>>8
	@old_cycles_to_run = t_first - old_timestamp
	@
	@new cycles_to_run = t_me-now
	@new cycles_to_run = t_me-old_timestamp-old_cycles_to_run+cycles>>8
	@
	@now = old_timestamp+old_cycles_to_run-cycles>>8
	
	subs r0,r0,cycles,asr#CYC_SHIFT
	addne r2,r2,r0
	strne_ r2,timestamp
	
	add r0,r1,cycles,asr#CYC_SHIFT
	str_ r0,cycles_to_run
	add cycles,cycles,r1,lsl#CYC_SHIFT
	
	ldr_ r0,firsttimeout
	str r0,[r12,#8]
	str_ r12,firsttimeout
	
	bx lr
1:
	add r1,r1,r0
	ldr_ r0,firsttimeout
	stmfd sp!,{r3,r4}
0:
	ldr r4,[r0,#8]
	cmp r4,#0
	beq i_t_insert_before_next_next
	ldr r3,[r4,#4]
	sub r3,r3,r2
	cmp r1,r3
	blt i_t_insert_before_next_next
	mov r0,r4
	b 0b
i_t_insert_before_next_next:
	str r4,[r12,#8]
	str r12,[r0,#8]
	ldmfd sp!,{r3,r4}
	bx lr

remove_timeout:
	ldr r1,[r12,#8]
	tst r1,#1
	bxne lr
remove_timeout_2:
	@r12 = timeout to remove
	@psuedocode:
	@if head==toremove, head=head.next, return
	@next=head
	@loop:
	@if next==null, return
	@if next.next==toremove, next.next=toremove.next, return
	@next=next.next
	@goto loop
	ldr_ r1,firsttimeout
	cmp r1,r12
	beq r_t_remove_first_timeout
0:	
	mov r0,r1
	cmp r0,#0
	bxeq lr
	ldr r1,[r0,#8]
	cmp r1,r12
	bne 0b
r_t_remove_timeout:
	ldr r2,[r12,#8]
	str r2,[r0,#8]
	@mark as empty
	mov r2,#1
	str r2,[r12,#8]
	bx lr
@remove_first_timeout
@	ldr_ r1,firsttimeout
r_t_remove_first_timeout:
	@get real timestamp into r2
	ldr_ r0,cycles_to_run
	sub r0,r0,cycles,asr#CYC_SHIFT
	ldr_ r2,timestamp
	add r2,r2,r0
	str_ r2,timestamp
	
	ldr r0,[r1,#8]
	str_ r0,firsttimeout
	@mark as empty
	add r0,r0,#1
	str r0,[r1,#8]
	
	
	@get timestamp of new first event
	ldr r1,[r0,#3]
	@subtract now
	sub r1,r1,r2
	@set cycles to run
	str_ r1,cycles_to_run
	and cycles,cycles,#CYC_MASK
	add cycles,cycles,r1,lsl#CYC_SHIFT
	bx lr
	

@get_timestamp
@	ldr_ r2,cycles_to_run
@	sub r2,r2,cycles,asr#CYC_SHIFT
@	ldr_ r1,timestamp
@	add r1,r1,r2
@	bx lr
	

get_scanline_2:
	@out: r2 = subscanline fraction
	@     r1 = timestamp
	@     r12 = scanline number, minus 1 if early in the scanline  (0 is the first visible line, not the prerender line)
	@does not touch r0
	ldr_ r2,cycles_to_run
	sub r2,r2,cycles,asr#CYC_SHIFT
	ldr_ r1,timestamp
	add r1,r1,r2
	ldr_ addy,frame_timestamp
	sub r1,r1,addy
	ldr_ addy,timestamp_div
	smull r2,addy,r1,addy		@changed to signed multiplication, valid between -12744874 and 12744874
	cmp r2,#0x60000000	@subtract 1 if r2 less than 60000000
	sbcs addy,addy,#0
	movmi addy,#0		@set to zero if this would make it negative
	bx lr


timeout_handler_branch_taken:
	@The 6502 has a quirk: IRQs that happen on the last cycle of a non-page-crossing brach
	@are ignored until the next instruction finishes.  This code checks for that condition.
	
	@did the branch cross a page?  No interrupt suppression
	cmp r1,r2
	bne timeout_handler
	@did the timeout occur on the last cycle of a non-page-crossing branch?
	@r8 would equal -1, -2, or -3  (* CYCLE).
	cmp r8,#-3*CYCLE
	blt timeout_handler
	@Is the timeout an interrupt?  (one of these four:)
	@nmi_timeout, mapper_irq_timeout, dmc_irq_timeout, frame_irq_timeout
	ldr_ r12,firsttimeout
	ldr r0,=_nmi_timeout
	cmp r0,r12
	ldrne r0,=_mapper_irq_timeout
	cmpne r0,r12
@	ldrne r0,=_dmc_irq_timeout
@	cmpne r0,r12
	ldrne r0,=_frame_irq_timeout
	cmpne r0,r12
	bne timeout_handler_entry
	@force fetching the next instruction
	ldrb r0,[m6502_pc],#1
	ldr pc,[m6502_optbl,r0,lsl#2]
	
timeout_handler:
	@timeout queue must have at least 2 elements in it at all times!
	ldr_ r12,firsttimeout  	@read address of current timeout handler
timeout_handler_entry:
	ldr r1,[r12,#8]         @read the next timeout handler
	str_ r1,firsttimeout     @set that as the next timeout handler
	mov r2,#1
	str r2,[r12,#8]  @mark current timeout handler as empty (by using an odd number, like 1)
	ldr r2,[r1,#4]   @timestamp of next handler
	ldr r1,[r12,#4]  @timestamp of current handler
	sub r1,r2,r1     @r1 = number of cycles which will happen before next timeout
	ldr lr,[r12]     @address to jump to to handle the timeout
@	b addcycles
addcycles:
	@in: r1=cycles to add
	@out: affects cycles,cycles_to_run,timestamp, r1=output timestamp
	@destroys r2
	ldr_ r2,cycles_to_run
	sub r2,r2,cycles,asr#CYC_SHIFT
	add cycles,cycles,r1,lsl#CYC_SHIFT
	mov r1,cycles,asr#CYC_SHIFT
	str_ r1,cycles_to_run
	ldr_ r1,timestamp
	add r1,r1,r2
	str_ r1,timestamp
	@jump to the timeout handler
	@r1 = current timestamp, r2 = cycles until next timeout, r8 and cycles_to_run = current cycles to run before next timeout (r8 is *256 as always)
	bx lr
	

@----------------------------------------------------------------------------

@MOVE THIS CRAP SOMEWHERE ELSE
fiveminutes: .word 5*60*60
sleeptime: .word 5*60*60
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	
 .align
 .pool
 .text
 .align
 .pool

ntsc_pal_reset:
@---NTSC/PAL
	stmfd sp!,{globalptr,lr}
	ldr globalptr,=GLOBAL_PTR_BASE
	
	ldr_ r0,emuflags
	tst r0,#PALTIMING
	moveq r2,#1
	movne r2,#0
	tst r0,#DENDY
	movne r2,#2
	cmp r2,#1
	
	ldreq r1,=89342  @ntsc = 262 * 341
	ldrgt r1,=106392 @dendy = 312 * 341
	ldrlt r1,=99742  @pal = 312 * 319.6875, should be 99742.5
	str_ r1,cyclesperframe
	
@	ldreq r1,=256			@NTSC		(113+2/3)*3
@	ldrne r1,=240			@PAL		(106+9/16)*3  = 319.6875
@	str_ r1,cyclesperscanline1
@	ldreq r1,=85			@NTSC		(113+2/3)*3
@	ldrne r1,=80			@PAL		(106+9/16)*3
@	str_ r1,cyclesperscanline2
@	ldreq r1,=261			@NTSC
@	ldrne r1,=311			@PAL
@	str_ r1,lastscanline

	ldrge r1,=0xb36cdb36	@NTSC Y-scroll change time =239/341
	ldrlt r1,=0xb2d31b2b	@PAL Y-scroll change time =238.2/341
	str_ r1,scroll_threshhold_value
	
	ldrge r1,=0x00C0300D	@NTSC (divide by 341)
	ldrlt r1,=0x00CD000D	@PAL  (divide by 319.6875)
	str_ r1,timestamp_div
	
	ldrge r1,=304-12
	ldrlt r1,=285-12
	str_ r1,line_zero_start_time
	
	ldrge r1,=40920
	ldrlt r1,=38362
	str_ r1,half_screen_time
	
	ldrge r1,=82181  @(1+240) * 341
	ldrlt r1,=77045  @(1+240) * 319.6875
	str_ r1,render_end_time
	
	ldreq r1,=82522-13  @(1+240+1) * 341
	ldrgt r1,=99572-13  @(1+240+51) * 341
	ldrlt r1,=77364-13  @(1+240+1) * 319.6875
	str_ r1,vblank_time
	
	ldrge r1,=16*341
	ldrlt r1,=15*341
	str_ r1,timestamp_mult
	
	ldrge r1,=7458*3
	ldrlt r1,=8314*3
	str_ r1,frame_sequencer_interval
	
	ldrge r1,=0xFFED408C
	ldrlt r1,=0xFFEBD175
	str_ r1,freq_mult

	movge r1,#16
	movlt r1,#15
	strb_ r1,timestamp_mult_2

	mov r1,#0
	strb_ r1,PAL60
	
	ldr_ r1,ntsc_pal_reset_hook
	movs r1,r1
	beq 0f
	adr lr,0f
	cmp r2,#1
	@call ntsc_pal_reset_hook: eq = NTSC, lt = PAL, gt = DENDY
	bx r1
0:
	ldmfd sp!,{globalptr,lr}
	bx lr

@----------------------------------------------------------
timeout_reset:
	stmfd sp!,{lr}
	ldr r2,=_TIMEOUT_END
	ldr r0,=_queue_dummy_timeout
	sub r2,r2,r0
	mov r1,#0xFFFFFFFF
	bl memset32_

	ldr r0,=_queue_dummy_timeout
	str_ r0,firsttimeout
	ldr r0,=queue_dummy_handler
	str_ r0,queue_dummy_timeout
	ldr r0,=0x00700000
	str_ r0,queue_dummy_timestamp
	
	str_ r0,cycles_to_run
	mov cycles,r0,lsl#CYC_SHIFT   @D=0, C=0, V=0, I=1 disable IRQ.
	
	ldr r0,=timeout_handler
	str_ r0,nexttimeout

	mov r0,#0
	str_ r0,timestamp
	str_ r0,queue_dummy_next
	str_ r0,frame_timestamp
	str_ r0,next_frame_timestamp
	
	@First frame begins at scanline 0, dot 8
	ldrb_ r0,emuflags
	tst r0,#PALTIMING
	ldreq r0,=-349   @-(341 + 8)
	ldrne r0,=-327   @-(341 + 8) * (15 / 16)
	str_ r0,frame_timestamp
	str_ r0,next_frame_timestamp
	
	ldmfd sp!,{pc}
	
	@don't need this because the entry point covers this
	
@	adr_ r12,prerender_line_timeout
@	ldr r0,=prerender_line_handler
@	mov r1,#0
@	b_long install_timeout_2

	.if SAVESTATES

	global_func encode_timeouts
encode_timeouts:
	@this probably should have been written in C instead of ASM, oh well...
	stmfd sp!,{r3-r5,lr}
	
	@called from C
	
	@first change timeouts to index numbers using HandlersTable
	ldr r4,=_queue_dummy_timeout
	ldr r5,=_TIMEOUT_END
0:
	ldr r0,[r4]
	bl lookup_timeout
	str r0,[r4],#12
	cmp r4,r5
	blt 0b
	
	@Change next pointers (if least significant bit is not 1)
	ldr r4,=_firsttimeout
	mov r1,r4
	ldr r5,=_TIMEOUT_END
0:
	ldr r0,[r4]
	movs r0,r0
	beq 1f
	tst r0,#0x01
	subeq r0,r0,r1
1:
	str r0,[r4],#12
	cmp r4,r5
	blt 0b
	
	@change the 5 hooks
	ldr r4,=_screen_off_hook1
	mov r5,#5
0:
	ldr r0,[r4]
	bl lookup_timeout
	str r0,[r4],#4
	subs r5,r5,#1
	bne 0b
	
	ldmfd sp!,{r3-r5,lr}
	bx lr

	global_func decode_timeouts
decode_timeouts:
	stmfd sp!,{r3-r5,lr}
	
	@change timeouts back from index numbers to handlers
	ldr r4,=_queue_dummy_timeout
	ldr r5,=_TIMEOUT_END
	ldr r2,=HandlersTable
0:
	ldr r0,[r4]
	cmp r0,#HandlersTableSize
	blge find_forgotten_timeout
	cmp r0,#HandlersTableSize
	ldrlt r0,[r2,r0,lsl#2]
	blt 2f
	mov r11,r11  @not found
2:	
	str r0,[r4],#12
	cmp r4,r5
	blt 0b

	@change next pointers from index numbers to pointers  (if LSB is not 1)
	ldr r4,=_firsttimeout
	ldr r5,=_TIMEOUT_END
	mov r1,r4
0:
	ldr r0,[r4]
	movs r0,r0
	beq 2f
	tst r0,#0x01
	addeq r0,r0,r1
	bne 2f
	@if it's in NOT in a valid address range, set to zero
	cmp r0,r1
	movlo r0,#0
	cmp r0,r5
	movhi r0,#0
2:
	str r0,[r4],#12
	cmp r4,r5
	blt 0b
	
	@change the 5 hooks
	ldr r4,=_screen_off_hook1
	mov r5,#5
	ldr r2,=HandlersTable
0:
	ldr r0,[r4]
	cmp r0,#HandlersTableSize
	blge find_forgotten_timeout
	cmp r0,#HandlersTableSize
	ldrlt r0,[r2,r0,lsl#2]
	blt 2f
	mov r11,r11	@not found
2:
	str r0,[r4],#4
	subs r5,r5,#1
	bne 0b

	ldmfd sp!,{r3-r5,lr}
	bx lr

lookup_timeout:
	@find R0 in table, return index in R0, returns original if not found
	ldr r12,=HandlersTable
	mov r1,r0
	mov r0,#0
0:
	ldr r2,[r12,r0,lsl#2]
	cmp r1,r2
	bxeq lr
	add r0,r0,#1
	cmp r0,#HandlersTableSize
	blt 0b
	
	mov r11,r11  @not found
	mov r0,r1
1:
	bx lr

find_forgotten_timeout:
	@match r0
	stmfd sp!,{r1,r2,r12,lr}
	ldr r12,=ForgottenHandlers
0:
	ldr r1,[r12],#4
	cmp r1,#0xFFFFFFFF
	beq 1f
	cmp r0,r1
	ldr r1,[r12],#4
	moveq r0,r1
	bne 0b
	bl lookup_timeout
1:
	ldmfd sp!,{r1,r2,r12,lr}
	bx lr


@	@IMPORT queue_dummy_handler
@	@IMPORT prerender_line_handler
@	@IMPORT line_zero_handler
@	@IMPORT render_end_handler
@	@IMPORT vblank_handler
@	@IMPORT vblank_handler_0
@	@IMPORT vblank_handler_2
	@IMPORT sprite_zero_handler_2
	@IMPORT sprite_zero_handler_3
	@IMPORT fine_x_handler
	@IMPORT frame_irq_handler
@	@IMPORT half_screen_handler
@	@IMPORT nmi_handler
	@IMPORT frame_sequencer_handler
	@IMPORT konami_handler
	@IMPORT map16_handler
	@IMPORT map19_handler
	@IMPORT mmc3_timeout_handler
	@IMPORT mmc5_handler_2
	@IMPORT mmc5_handler
	@IMPORT map65_handler
	@IMPORT map67_handler
	@IMPORT mapper_irq_handler
	@IMPORT map19_irq_handler
	@IMPORT run_mmc3
	@IMPORT mmc3_set_next_timeout
	@IMPORT mmc3_ntsc_pal_reset
	@IMPORT map18_handler
	@IMPORT map69_handler
	
	.global HandlersTable
HandlersTable:
	.word 0
	.word 0xFFFFFFFF
	.word queue_dummy_handler
	.word prerender_line_handler
	.word line_zero_handler
	.word render_end_handler
	.word vblank_handler
	.word vblank_handler_0
	.word vblank_handler_2
	.word sprite_zero_handler_2
	.word sprite_zero_handler_3
	.word fine_x_handler
	.word dmc_handler
	.word 0  @reserved for DMC
	.word frame_irq_handler
	.word half_screen_handler
	.word nmi_handler
	.word frame_sequencer_handler
	.word konami_handler
	.word map16_handler
	.word map19_handler
	.word mmc3_timeout_handler
	.word mmc5_handler_2
	.word mmc5_handler
	.word map65_handler
	.word map67_handler
	.word mapper_irq_handler
	.word map19_irq_handler
	.word run_mmc3
	.word mmc3_set_next_timeout
	.word mmc3_ntsc_pal_reset
	.word map18_handler
	.word map69_handler
	.word mmc3_screen_on
	.word Mapper163HalfScreenHandler
HandlersTableEnd:
 HandlersTableSize = (HandlersTableEnd  - HandlersTable) / 4
@ HandlersTableSize = 31

ForgottenHandlers:
@Forgetten handlers from old versions, change these back to the correct addresses
	.word 0x08009B64, dmc_handler
	.word 0x0800A0BC, dmc_handler
	.word 0x06003D80, mmc3_screen_on
	.word 0x06003D60, mmc3_screen_on
	.word 0xFFFFFFFF

	.endif

 .section .data.106, "w", %progbits
TimeoutState:

_cycles_to_run: .word 0
_timestamp: .word 0
_frame_timestamp: .word 0
_next_frame_timestamp: .word 0

_firsttimeout: .word 0
_queue_dummy_timeout: .word 0
_queue_dummy_timestamp: .word 0
_queue_dummy_next: .word 0
_prerender_line_timeout: .word 0
_prerender_line_timestamp: .word 0
_prerender_line_next: .word 0
_line_zero_timeout: .word 0
_line_zero_timestamp: .word 0
_line_zero_next: .word 0
_render_end_timeout: .word 0
_render_end_timestamp: .word 0
_render_end_next: .word 0
_vblank_timeout: .word 0
_vblank_timestamp: .word 0
_vblank_next: .word 0
_sprite_zero_timeout: .word 0
_sprite_zero_timestamp: .word 0
_sprite_zero_next: .word 0
_fine_x_timeout: .word 0
_fine_x_timestamp: .word 0
_fine_x_next: .word 0
_mapper_timeout: .word 0
_mapper_timestamp: .word 0
_mapper_next: .word 0
_mapper_irq_timeout: .word 0
_mapper_irq_timestamp: .word 0
_mapper_irq_next: .word 0
_dmc_timeout: .word 0
_dmc_timestamp: .word 0
_dmc_next: .word 0
_frame_irq_timeout: .word 0
_frame_irq_timestamp: .word 0
_frame_irq_next: .word 0
_half_screen_timeout: .word 0
_half_screen_timestamp: .word 0
_half_screen_next: .word 0
_nmi_timeout: .word 0
_nmi_timestamp: .word 0
_nmi_next: .word 0
_frame_sqeuencer_timeout: .word 0
_frame_sequencer_timestamp: .word 0
_frame_sequencer_next: .word 0

_TIMEOUT_END:

_screen_off_hook1: .word 0
_screen_off_hook2: .word 0
_screen_on_hook1: .word 0
_screen_on_hook2: .word 0
_ntsc_pal_reset_hook: .word 0

TimeoutStateEnd:

@these aren't used yet
_pcmirqbakup: .word 0
_pcmirqcount: .word 0

@These are part of NTSC/PAL differences, so they are not saved.
_cyclesperframe: .word 0
_cyclesperscanline: .word 0
_timestamp_div: .word 0
_scroll_threshhold_value: .word 0
_vblank_time: .word 0
_render_end_time: .word 0
_half_screen_time: .word 0
_line_zero_start_time: .word 0

_frame_timestamp_minus_96: .word 0
_timestamp_mult: .word 0
_frame_timestamp_plus_245: .word 0
_frame_timestamp_plus_128: .word 0

dontstop:	.byte 0 @_dontstop
_crash_framecounter:	.byte 0
_crash_disabled:		.byte 0
	.byte 0
	
	.if DEBUG
	.global _fetch_function
_fetch_function: .word normal_fetch
	.endif

	@.end
