	.section .vram1, "ax", %progbits
@----------------------------------------------------------------------------
@cycles ran out
@----------------------------------------------------------------------------
line0:
	adr_ r2,cpuregs
	stmia r2,{gb_flg-gb_pc,gb_sp}	@save gbz80 state
	
	ldrb_ r0,autoborderstate
	cmp r0,#1
	bne 0f
	ldr_ r0,frame
	ldr_ r1,auto_border_reboot_frame
	cmp r0,r1
	blt 0f
	bl_long loadcart_after_sgb_border
0:
	


@waitformulti
	ldr r1,=REG_P1		@refresh input every frame
	ldrh r0,[r1]
		eor r0,r0,#0xff
		eor r0,r0,#0x300	@r0=button state (raw)
	ldr_ r1,AGBjoypad
	eor r1,r1,r0
	and r1,r1,r0		@r1=button state (0->1)
	str_ r0,AGBjoypad

	ldrb_ r2,dontstop_
	cmp r2,#0
	ldmeqfd sp!,{gb_flg-gb_pc,globalptr,r11,lr}	@exit here if doing single frame:
	bxeq lr							@return to rommenu()

	@----anything from here til line0x won't get executed while rom menu is active---

@	mov r2,#REG_BASE
@	mov r3,#0x0110				;was 0x0310
@	strh r3,[r2,#REG_BLDMOD]	;stop darkened screen,OBJ blend to BG0/1
@	mov r3,#0x1000				;BG0/1=16, OBJ=0
@	strh r3,[r2,#REG_BLDALPHA]	;Alpha values

	adr lr,line0x		@return here after doing L/R + SEL/START

	tst r1,#0x300		@if L or R was pressed
	tstne r0,#0x100
	tstne r0,#0x200		@and both L+R are held..
	ldrne r1,=ui
	bxne r1			@do menu


	ands r3,r0,#0x300		@if either L or R is pressed (not both)
	eornes r3,r3,#0x300
	bicne r0,r0,#0x0c		@	hide sel,start from NES
	str_ r0,XGBjoypad
	beq line0x		@skip ahead if neither or both are pressed

@	tst r0,#0x200
@	tstne r1,#4		;L+SEL for BG adjust
@	ldrne r2,adjustblend
@	addne r2,r2,#1
@	strne r2,adjustblend

	tst r0,#0x200		@L?
	tstne r1,#8		@START?
	ldrb_ r2,novblankwait_	@0=Normal, 1=No wait, 2=Slomo
	addne r2,r2,#1
	cmp r2,#3
	moveq r2,#0
	strb_ r2,novblankwait_

	tst r0,#0x100		@R?
	tstne r1,#8		@START:
	ldrne r1,=quickload
	bxne r1

	tst r0,#0x100		@R?
	tstne r1,#4		@SELECT:
	ldrne r1,=quicksave
	bxne r1
line0x:
	bl_long refreshNESjoypads	@Z=1 if communication ok

#if JOYSTICK_READ_HACKS
	@joystick speed hacks
	mov r1,#-64
	strb_ r1,joy_read_count
#endif

@	bne waitformulti	;waiting on other GBA..

	ldr_ r0,AGBjoypad
	ldr_ r2,fiveminutes_		@sleep after 5/10/30 minutes of inactivity
	cmp r0,#0				@(left out of the loop so waiting on multi-link
	ldrne_ r2,sleeptime_		@doesn't accelerate time)
	subs r2,r2,#1
	str_ r2,fiveminutes_
	bleq_long suspend

	mov r1,#0
	strb_ r1,scanline		@reset scanline count
@	bl newframe		@display update

	@now do double speed vblank stuff:
	ldr_ r0,doubletimer_
	tst r0,#0x01
	blne_long updatespeed

	adr_ r0,cpuregs
	ldmia r0,{gb_flg-gb_pc,gb_sp}	@restore GB-Z80 state

@	ldrb r1,lcdctrl		;not liked by SML.
@	tst r1,#0x80
	ldr r1,=lcdstat
	ldrb r0,[r1]		@
	and r2,r0,#0x7C		@reset lcd mode flags (vblank/hblank/oam/lcd)
	cmp r0,r2
	strneb r2,[r1]		@
	strneb r2,[r1,#-12] @FIXME

	ldr_ r0,cyclesperscanline
	add cycles,cycles,r0

	ldr r0,=line1_to_71
	str_ r0,nexttimeout

	adrl_ r1,FF41_R_function
	ldr r0,[r1]
@	ldr r0,=FF41_R
	ldr r1,=FF41_R_ptr
	str r0,[r1]

	ldr_ pc,scanlinehook

 .section .iwram.3, "ax", %progbits

line1_to_71: @------------------------
	ldr_ r0,cyclesperscanline
	add cycles,cycles,r0

	ldrb_ r1,scanline
	add r1,r1,#1
	strb_ r1,scanline
	cmp r1,#75		@was 71
	ldrmi_ pc,scanlinehook
@--------------------------------------------- between 71 and 72

	ldrb_ r0,lcdctrl
	strb_ r0,lcdctrl0midframe		@Chase HQ likes this
	
	ldrb_ r0,sgb_mask
	movs r0,r0
	bleq_long copy_gbc_palette
	
	@@@
	@@@bl gbc_chr_update
	@@@

	adr addy,line72_to_143
	str_ addy,nexttimeout
	ldr_ pc,scanlinehook
line72_to_143: @------------------------
	ldr_ r0,cyclesperscanline
	add cycles,cycles,r0

	ldrb_ r1,scanline
	add r1,r1,#1
	strb_ r1,scanline
	cmp r1,#143
	ldrmi_ pc,scanlinehook

	adr addy,line144
	str_ addy,nexttimeout
	ldr_ pc,scanlinehook

	.pushsection .iwram.3
line144: @------------------------
#if SPEEDHACKS_NEW
	@quick hack finder code
	
	@quick hack finder used?
	ldrb_ r0,quickhackused
	movs r0,r0
	bne 0f
	@increment counter
	ldrb_ r0,quickhackcounter
	adds r0,r0,#1
	strb_ r0,quickhackcounter
	cmp r0,#8
	@under 8, don't search
	blt 1f
	stmfd sp!,{r3}

	adr_ r0,cpuregs
	stmia r0,{gb_flg-gb_pc,gb_sp}
	
	mov r0,gb_pc
	ldr_ r1,lastbank
	blx_long quickhackfinder
	ldmfd sp!,{r3}
0:	
	@if quick hack finder was called, or hack was used, reset counter to 0
	mov r0,#0
	strb_ r0,quickhackcounter
	strb_ r0,quickhackused
1:
#endif	
	ldrb_ r0,doubletimer_
	tst r0,#1
	blne_long updatespeed
	bl newframe_vblank
@	stmfd sp!,{r0-addy,lr}

@ [ BUILD <> "DEBUG"
@	ldrb r2,novblankwait
@	teq r2,#1
@	beq l03
@l01
@	mov r0,#0				;don't wait if not necessary
@	mov r1,#1				;VBL wait
@	swi 0x040000			; Turn of CPU until IRQ if not too late allready.
@	teq r2,#2				;Check for slomo
@	moveq r2,#0
@	beq l01
@l03
@ ]
@	ldmfd sp!,{r0-addy,lr}


	ldr_ r0,fpsvalue
	add r0,r0,#1
	str_ r0,fpsvalue



@ [ DEBUG
@	mov r1,#REG_BASE			;darken screen during GB vblank
@	mov r0,#0x00f1
@	strh r0,[r1,#REG_BLDMOD]
@	ldrh r0,[r1,#REG_VCOUNT]
@	mov r1,#19
@	bl debug_
@ ]
	tst cycles,#CYC_LCD_ENABLED
	@ldrb_ r0,lcdctrl		@LCD turned on?
	@tst r0,#0x80
	beq novbirq

	adrl_ r1,FF41_R_vblank_function
	ldr r0,[r1]
@	ldr r0,=FF41_R_vblank
	ldr r1,=FF41_R_ptr
	str r0,[r1]

	adrl r1,lcdstat
	ldrb r0,[r1]		@vbl flag
	and r0,r0,#0x7C
	orr r0,r0,#0x01
	strb r0,[r1]		@vbl flag
	strb r0,[r1,#-12] @FIXME


	ldrb_ r0,gb_if
	orr r0,r0,#0x01		@1=VBL
	strb_ r0,gb_if
novbirq:
	mov r0,#24*CYCLE
	add cycles,cycles,r0

	mov r1,#144
	strb_ r1,scanline

	adr addy,VBL_Hook
	str_ addy,nexttimeout
	b _GO
VBL_Hook:
	ldr_ r0,cyclesperscanline
	sub r0,r0,#24*CYCLE
	add cycles,cycles,r0

	adr addy,line145_to_end
	str_ addy,nexttimeout
	ldr_ pc,scanlinehook
line145_to_end: @------------------------
	ldr_ r0,cyclesperscanline
	add cycles,cycles,r0

	ldrb_ r1,scanline
	add r1,r1,#1
	strb_ r1,scanline
	cmp r1,#153				@last scanline
	ldrmi_ pc,scanlinehook
	
#if !EARLY_LINE_0
	ldr addy,=line0
	str_ addy,nexttimeout
	ldr_ pc,scanlinehook
#else
	sub cycles,cycles,r0
toLineZero_modify1:
	adds cycles,cycles,#16*CYCLE  @8 for non-double speed mode
	
	@bmi toLineZero
	adr addy,toLineZero
	str_ addy,nexttimeout
	ldr_ pc,scanlinehook
	
toLineZero:
	@line 0, but still in Vblank state
	mov r0,#0
	strb_ r0,scanline
@	bl_long newframe	@display update
	
	ldr_ r0,cyclesperscanline
toLineZero_modify2:
	sub r0,r0,#16*CYCLE  @8 for non-double speed mode
	add cycles,cycles,r0
	ldr addy,=line0
	str_ addy,nexttimeout
	ldr_ r0,frame
	add r0,r0,#1
	str_ r0,frame
	ldr_ pc,scanlinehook
#endif

immediate_check_irq:
	ldrb_ r0,gb_ie		@0xFFFF=Interrupt Enable.
	ldrb_ r1,gb_if
	ands r1,r1,r0
	tstne cycles,#CYC_IE
	bxeq lr
	b_long immediate_check_irq_2
@	ldrb_ r0,gb_ime
@	movs r0,r0
@	bxeq lr
	@different ugly hack which doesn't mess up timing,
	@this is necessary because goomba must finish executing its instruction before checking for GB interrupts
	.pushsection .text
immediate_check_irq_2:
	ldr_ r0,nexttimeout
	@don't override checkMasterIRQ_minus12
	ldr r1,=checkMasterIRQ_minus12
	cmp r0,r1
	bxeq lr
	sub cycles,cycles,#1024*CYCLE  @this just makes it go somewhere else instead of the next instruction
	
	str_ r0,nexttimeout_alt
	ldr r1,=no_more_irq_hack
	str_ r1,nexttimeout
	bx lr

no_more_irq_hack:
	add cycles,cycles,#1024*CYCLE
	ldr_ r0,nexttimeout_alt
	str_ r0,nexttimeout
	b_long checkIRQ
	.popsection


@----------------------------------------------------------
default_scanlinehook:
checkScanlineIRQ:
default_scanlinehook_nohblank:
    ldrb_ r1,dma_blocks_total
    cmp r1,#0
    beq _checkScanlineIRQ  @ If not mid-hdma, continue normal execution.
    @ Else, fall through to tick_hdma
tick_hdma:
    @ Decrement _dma_blocks_remaining
    ldr r1,=_dma_blocks_remaining
    ldrb r2,[r1]
    sub r2,r2,#1
    strb r2,[r1]
    
    @ If _dma_blocks_remaining == 0, call DoDma
    cmp r2,#0
    bne _checkScanlineIRQ
    @ Fall through to call_dodma
call_dodma:
    @ Call DoDma
    stmfd sp!,{r3,r8-r12,r14,lr}
    ldrb_ r0,dma_blocks_total
    lsl r0,r0,#4
    blxeq_long DoDma  @ Call DoDma if we're doing HDMA
	ldmfd sp!,{r3,r8-r12,r14,lr}
    
    @ Set _dma_blocks_remaining and _dma_blocks_total to 0
    mov r0,#0
    ldr r1,=_dma_blocks_remaining
    str r0,[r1]
    ldr r1,=_dma_blocks_total
    str r0,[r1]
    @ Finally, fall through and continue execution
_checkScanlineIRQ:
	tst cycles,#CYC_LCD_ENABLED
	beq noScanlineIRQ
	
	@do LC==LYC test
	ldrb_ r1,scanline
	ldrb_ r0,lcdyc
	cmp r0,r1
	adrl r1,lcdstat
	ldrb r0,[r1]
	@LY==LYC bit must change from 0 to 1 to trigger an LY==LYC interrupt
	orreq r2,r0,#4
	bicne r2,r0,#4
	cmp r2,r0
	@ne if LYC bit has changed
	strneb r2,[r1]
	strneb r2,[r1,#-12] @FIXME
@	beq 0f
	@has it turned on, and interrupts are enabled?
	tstne r2,#4
	tstne r2,#0x40
	bne ScanlineIRQ
@	ldrb r2,lcdstat
@0:
	@in vblank?  no Hblank or Mode 2 IRQ
	tst r2,#0x01
	bne noScanlineIRQ
	@Hblank IRQ or Mode 2 IRQ enabled?
	@TODO: real Hblank IRQ
	tst r2,#0x28
	beq noScanlineIRQ
ScanlineIRQ:
	ldrb_ r0,gb_if
	orr r0,r0,#0x02		@2=LCD STAT
	strb_ r0,gb_if
noScanlineIRQ:
@------------------

@------------------
checkTimerIRQ:
	ldr_ r2,timercyclesperscanline
	
	ldr_ r0,dividereg
	add r0,r0,r2,lsl#12		@256th cycles.
	str_ r0,dividereg

	ldrb_ r1,timerctrl
	tst r1,#0x4
	beq noTimer
	ands r1,r1,#3
	moveq r1,#4
	mov r0,#18
	sub r1,r0,r1,lsl#1
	ldr_ r0,timercounter
	adds r0,r0,r2,lsl r1
	bcc noTimerIRQ
	ldrb_ r0,gb_if
	orr r0,r0,#0x04		@4=Timer
	strb_ r0,gb_if
	ldrb_ r0,timermodulo
	mov r0,r0,lsl#24
noTimerIRQ:
	str_ r0,timercounter
noTimer:
	ldrb_ r1,gb_if
	tst r1,#0x08			@Serial
	ldrneb_ r0,stctrl
	andne r0,r0,#0x7F		@Clear Serial Transfer flag.
	strneb_ r0,stctrl
checkMasterIRQDelayed:
	tst cycles,#CYC_IE
	beq _GO
checkIRQDelayed:
	ldrb_ r0,gb_ie
	ldrb_ r1,gb_if
	ands r0,r0,r1
	beq _GO
	
	ldrb r2,[gb_pc]
	cmp r2,#0x76			@Check if we're doing Halt.
	beq irqGBZ80_ifhalt

	ldr_ r12,cyclesperscanline
	sub r12,r12,#8*CYCLE
	subs r12,cycles,r12
	bmi irqGBZ80_nothalt
	mov cycles,r12
	
	ldrb r0,lcdstat
	orr r0,r0,#2
	strb r0,lcdstat
	
	ldr_ r0,nexttimeout
	str_ r0,nexttimeout_alt
	adr r0,checkMasterIRQ_minus12
	str_ r0,nexttimeout
	b _GO

checkMasterIRQ_minus12:
	ldrb r0,lcdstat
	bic r0,r0,#2
	strb r0,lcdstat

	ldr_ r0,cyclesperscanline
	add r0,#8*CYCLE
	add cycles,cycles,r0
	ldr_ r0,nexttimeout_alt
	str_ r0,nexttimeout
	@proceed to checkMasterIRQ
	
@----------------------------------------------------------
checkMasterIRQ:
@----------------------------------------------------------
	tst cycles,#CYC_IE
	@ldrb_ r2,gb_ime
	@tst r2,#1
	beq _GO
@----------------------------------------------------------
checkIRQ:
@----------------------------------------------------------
	ldrb_ r0,gb_ie
	ldrb_ r1,gb_if
	ands r0,r0,r1
	beq _GO
@----------------------------------------------------------
irqGBZ80:
@----------------------------------------------------------
	ldrb r2,[gb_pc]
	cmp r2,#0x76			@Check if we're doing Halt.
irqGBZ80_ifhalt:
@	cmpne r2,#0x10                  ;or STOP
	addeq gb_pc,gb_pc,#1	@get out of HALT
irqGBZ80_nothalt:
	bic cycles,cycles,#CYC_IE
@	mov r2,#0				@disable IRQ
@	strb_ r2,gb_ime

	tst r0,#0x01			@VBlank
	movne r2,#0x40
	bicne r1,r1,#0x01		@clear the IRQ flag
	bne doIRQ

	tst r0,#0x02			@LCD Stat
	movne r2,#0x48
	bicne r1,r1,#0x02		@clear the  IRQ flag
	bne doIRQ

	tst r0,#0x04			@Timer
	movne r2,#0x50
	bicne r1,r1,#0x04		@clear the  IRQ flag
@	bne doIRQ
	beq_long _irqGBZ80_
	@10 instructions moved to .text section
@	tst r0,#0x08			@Serial
@	movne r2,#0x58
@	bicne r1,r1,#0x08		@clear the  IRQ flag
@	bne doIRQ
@
@	tst r0,#0x10			@Joypad
@	movne r2,#0x60
@	bicne r1,r1,#0x10		@clear the  IRQ flag
@	bne doIRQ
@
@	and r1,r1,#0x1f			@unknown IRQ?
@	mov r2,#0x40

doIRQ:
	strb_ r1,gb_if
	ldr_ r0,lastbank
	sub r0,gb_pc,r0
	mov gb_pc,r2			@get IRQ vector

	push16_novram					@save PC
	encodePC_afterpush16

	fetch 24

.pushsection .text
_irqGBZ80_:
	tst r0,#0x08			@Serial
	movne r2,#0x58
	bicne r1,r1,#0x08		@clear the  IRQ flag
	bne_long doIRQ

	tst r0,#0x10			@Joypad
	movne r2,#0x60
	bicne r1,r1,#0x10		@clear the  IRQ flag
	bne_long doIRQ

	and r1,r1,#0x1f			@unknown IRQ?
	mov r2,#0x40
	b_long doIRQ
.popsection


 .section .iwram.end.105, "ax", %progbits
@----------------------------------------------------------------------------
fiveminutes: .word 5*60*60 @fiveminutes_
sleeptime: .word 5*60*60 @sleeptime_
dontstop: .byte 0 @dontstop_
g_hackflags: .byte 0 @hackflags
g_hackflags2: .byte 0 @hackflags2
 .byte 0
@----------------------------------------------------------------------------
