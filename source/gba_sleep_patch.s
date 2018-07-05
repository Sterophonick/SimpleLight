@;********************************************************************
@;********************************************************************
@;--------------------------------------------------------------------
@;-                      Reset                                    -
@;--------------------------------------------------------------------
	.section   	.iwram,"ax",%progbits
		
	.global  Sleep_ReplaceIRQ_start
	.global  Sleep_ReplaceIRQ_end
	.global  Return_address_L
	.global  Sleep_key
	.global  Reset_key
	.global  Wakeup_key

REG_BASE		= 0x4000000
REG_DISPCNT		= 0x00
REG_DISPSTAT	= 0x04
REG_VCOUNT		= 0x06
REG_BG0CNT		= 0x08
REG_BG1CNT		= 0x0A
REG_BG2CNT		= 0x0C
REG_BG3CNT		= 0x0E
REG_BG0HOFS		= 0x10
REG_BG0VOFS		= 0x12
REG_BG1HOFS		= 0x14
REG_BG1VOFS		= 0x16
REG_BG2HOFS		= 0x18
REG_BG2VOFS		= 0x1A
REG_BG3HOFS		= 0x1C
REG_BG3VOFS		= 0x1E
REG_WIN0H		= 0x40
REG_WIN1H		= 0x42
REG_WIN0V		= 0x44
REG_WIN1V		= 0x46
REG_WININ		= 0x48
REG_WINOUT		= 0x4A
REG_BLDCNT		= 0x50
REG_BLDALPHA	= 0x52
REG_BLDY		= 0x54
REG_SOUND1CNT_L	= 0x60
REG_SOUND1CNT_H	= 0x62
REG_SOUND1CNT_X	= 0x64
REG_SOUND2CNT_L	= 0x68
REG_SOUND2CNT_H	= 0x6C
REG_SOUND3CNT_L	= 0x70
REG_SOUND3CNT_H	= 0x72
REG_SOUND3CNT_X	= 0x74
REG_SOUND4CNT_L	= 0x78
REG_SOUND4CNT_H	= 0x7c
REG_SOUNDCNT_L		= 0x80
REG_SOUND2CNT_H		= 0x82
REG_SOUNDCNT_X		= 0x84
REG_SOUNDBIAS		= 0x88
REG_WAVE_RAM0_L		= 0x90
REG_FIFO_A_L	= 0xA0
REG_FIFO_A_H	= 0xA2
REG_FIFO_B_L	= 0xA4
REG_FIFO_B_H	= 0xA6
REG_DM0SAD		= 0xB0
REG_DM0DAD		= 0xB4
REG_DM0CNT_L	= 0xB8
REG_DM0CNT_H	= 0xBA
REG_DM1SAD		= 0xBC
REG_DM1DAD		= 0xC0
REG_DM1CNT_L	= 0xC4
REG_DM1CNT_H	= 0xC6
REG_DM2SAD		= 0xC8
REG_DM2DAD		= 0xCC
REG_DM2CNT_L	= 0xD0
REG_DM2CNT_H	= 0xD2
REG_DM3SAD		= 0xD4
REG_DM3DAD		= 0xD8
REG_DM3CNT_L	= 0xDC
REG_DM3CNT_H	= 0xDE
REG_TM0D		= 0x100
REG_TM0CNT		= 0x102
REG_IE			= 0x200
REG_IF			= 0x202
REG_P1			= 0x130
REG_P1CNT		= 0x132
REG_WAITCNT		= 0x204

	
	.arm
Sleep_ReplaceIRQ_start:
	MOV             R0, #0x4000000
	ADR             R1, my_irq
	STR             R1, [R0,#-0x4] 	 @; 3FFFFFC = my_irq;
	LDR             R0, =0x12345678  @;//0x80000C0
	BX              R0 							 @; loc_80000C0
	.align
Return_address_L:
	.ltorg
@;--------------------------------------------------------------
my_irq:
	@;r0 = reg_base
	@;r1 = REG_IE,REG_IF
	@;LDR             PC, [R0,#-0xC]
	LDR             R1, [R0,#0x200]
	TST             R1, #0x10000
	TSTEQ           R1, #0x10000000
	LDREQ           PC, [R0,#-0xC]		@;old_interrupt_handler
	

	ldr r2,[r0,#REG_P1]
	bic r2,r2,#0xFF000000
	bic r2,r2,#0x00FF0000
	@;tst r2,#0x0300	@L+R?
	@;ldrne pc,[r0,#-(0x04000000-0x03FFFFB4)] @to IRQ routine if not pressed
	
	adr r3,Reset_key @
	ldr r3,[r3]
	cmp r2,r3				
	beq reset_now	

	adr r3,Sleep_key @
	ldr r3,[r3]
	cmp r2,r3 			
	beq sleep_now
	ldr pc,[r0,#-(0x04000000-0x03FFFFF4)] @;to normal IRQ routine									
@;--------------------------------------------------------------
reset_now:
	adr r1,reset_code
	adr r3,reset_end
	mov r2,#0x02000000
copy_loop:
	ldr r0,[r1],#4
	str r0,[r2],#4
	cmp r1,r3
	blt copy_loop
	mov r0,#0x02000000
	add r0,r0,#1
	bx r0
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Sleep_key:
	.word 0xF7 @L+R+Start?
Reset_key:
	.word 0x1BD @;L up B;
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.thumb
reset_code:
	mov r0,#0x20
	lsl r3,r0,#22 @;#0x8000000 r3
	lsl r0,r0,#12 @;#0x0020000
	add r4,r3,r0  @;#0x8020000 r4
	add r5,r4,r0  @;#0x8040000 r5
	lsl r1,r0,#8  @;#0x2000000
	add r2,r3,r1  @;#0xa000000
	lsr r1,r3,#4  @;#0x0800000
	sub r6,r2,r1  @;#0x9800000
	lsr r1,r1,#4  @;#0x0080000
	add r6,r6,r1  @;#0x9880000 r6
	sub r2,r2,r0  @;#0x9fe0000 r2
	sub r7,r2,r0  @;#0x9fc0000 r7

	mov r0,#210
	lsl r0,r0,#8  @;0xd200 r0
	mov r1,#21
	lsl r1,r1,#8  @;0x1500 r1

	strh r0,[r2]
	strh r1,[r3]
	strh r0,[r4]
	strh r1,[r5]

	lsr r0,r3,#12 @;#0x0008000 r0
	add r0,#2

	strh r0,[r6]
	strh r1,[r7]

	lsl r1,r0,#11 @;#0x4000000
	sub r1,r1,#8  @;#0x3FFFFFA
	mov r0,#0xfc  @;#252 r0
	str r0,[r1]   @;#0x3FFFFFA (mirror of #0x3007FFA
	swi 0x01
	swi 0x00
reset_end:
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.arm
sleep_now:
	stmfd sp!,{r4-r11,lr}
	add r1,r0,#REG_SOUND1CNT_L
	@;copy and push 32 bytes
	ldmia r1!,{r2-r9}
	stmfd sp!,{r2-r9}
	@;copy and push 32 bytes
	ldmia r1!,{r2-r9}
	stmfd sp!,{r2-r9}
	@;r3 = contents of REG_SOUND3CNT_X

	@;save old io values
	add r1,r0,#REG_IE
	ldrh r4,[r1]
	ldr r5,[r0,#REG_P1]
	ldrh r6,[r0,#REG_DISPCNT]

	@;enable ints on Keypad, Game Pak
	ldr r1,=0xFFFF1000
	str r1,[r0,#REG_IE]
	mov r1,#0xC0000000		@;interrupt on start+sel
	@;orr r1,r1,#0x000C0000
	adr r2,Wakeup_key
	ldr r2,[r2]
	MVN R2,R2
	lsl	r2,r2,#0x10
	orr r1,r1,r2
	str r1,[r0,#REG_P1]
	strh r0,[r0,#REG_SOUNDCNT_X]	@;sound off
	orr r1,r6,#0x80
	strh r1,[r0,#REG_DISPCNT]			@;LCD off

	swi 0x030000

	@;Loop to wait for letting go of Sel+start
loop:
	mov r0,#REG_BASE
	ldr r1,[r0,#REG_P1]
	adr r7,Wakeup_key
	ldr r7,[r7]
	and r1,r1,r7
	@;cmp r1,#0x000C
	cmp r1,r7
	bne loop

	@;spin until VCOUNT==159
spin2:
	ldrh r1,[r0,#REG_VCOUNT]
	cmp r1,#159
	bne spin2
	@;spin until VCOUNT==160
spin4:
	ldrh r1,[r0,#REG_VCOUNT]
	cmp r1,#160
	bne spin4
	@;spin until VCOUNT==159
spin5:
	ldrh r1,[r0,#REG_VCOUNT]
	cmp r1,#159
	bne spin5
	@;spin until VCOUNT==160
spin6:
	ldrh r1,[r0,#REG_VCOUNT]
	cmp r1,#160
	bne spin6
	@;spin until VCOUNT==159
spin7:
	ldrh r1,[r0,#REG_VCOUNT]
	cmp r1,#159
	bne spin7

	@;restore interrupts
	add r1,r0,#REG_IE
	strh r4,[r1]
	@;restore joystick interrupt
	str r5,[r0,#REG_P1]
	mov r4,#0x1000 @;clear the damn joystick interrupt
	strh r4,[r1,#2]

	@;restore screen
	strh r6,[r0,#REG_DISPCNT]
	ldmfd sp!,{r2-r9}
	@;restore sound state
	str r3,[r0,#REG_SOUNDCNT_X]
	add r1,r0,#0x80
	stmia r1!,{r2-r9}
	add r1,r0,#0x60
	ldmfd sp!,{r2-r9}
	stmia r1!,{r2-r9}
	ldmfd sp!,{r4-r11,lr}
	@;spin until VCOUNT==160, triggers next vblank
spin3:
	ldrh r1,[r0,#REG_VCOUNT]
	cmp r1,#160
	bne spin3  @<insert ytmnd cliche here>
	@;all done!
	ldr pc,[r0,#-(0x04000000-0x03FFFFF4)] @to IRQ routine

	.align
Wakeup_key:
	.word 0x3F3 @;start and select
	.ltorg
Sleep_ReplaceIRQ_end:
   .end
