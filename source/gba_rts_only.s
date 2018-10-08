@;--------------------------------------------------------------------
	.section   	.iwram,"ax",%progbits
		
	.global  RTS_only_ReplaceIRQ_start
	.global  RTS_only_ReplaceIRQ_end
	.global  RTS_only_Return_address_L
	.global  RTS_only_SAVE_key
	.global  RTS_only_LOAD_key


	
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

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
	.arm
RTS_only_ReplaceIRQ_start:
	MOV             R0, #0x4000000
	ADR             R1, RTS_irq
	STR             R1, [R0,#-4] @; 0x3FFFFFC = RTS_irq;
	LDR             R0, =0x12345678
	BX              R0
	.align
RTS_only_Return_address_L:
	.ltorg 			@;return address need modify
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
spend_0x80:
	.word 0x0203FE00   @; default
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RTS_irq:
	LDR			R1, [R0,#0x200]
	TST			R1, #0x10000
	TSTEQ		R1, #0x10000000
	LDREQ		PC, [R0,#-0xC]			@;old_interrupt_handler

	ldr 		r2,[r0,#REG_P1]
	bic 		r2,r2,#0xFF000000
	bic 		r2,r2,#0x00FF0000
	
check_save:
	adr 		r3,RTS_only_SAVE_key 
	ldr 		r3,[r3]
	cmp 		r2,r3		
	beq			call_Save	
check_load:
	adr 		r3,RTS_only_LOAD_key 
	ldr 		r3,[r3]
	cmp 		r2,r3 	
	beq 		call_Load	
	ldr 		pc,[r0,#-(0x04000000-0x03FFFFF4)] @;to normal IRQ routine								
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RTS_only_SAVE_key:
	.word 0xFB @;L+R+select
RTS_only_LOAD_key:
	.word 0xF7 @;L+R+Start
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.arm
@;------------------------------------------------------
SetRampage:
	ldr 	r1,=0xD200
	ldr 	r2,=0x1500
	ldr 	r3,=0x9fe0000
	strh 	r1,[r3]
	mov 	r3,#0x8000000
	strh  r2,[r3]
	ldr 	r3,=0x8020000
	strh 	r1,[r3]
	ldr 	r3,=0x8040000
	strh  r2,[r3]
	ldr 	r3,=0x9C00000
	strh  r0,[r3]
	ldr 	r3,=0x9FC0000
	strh  r2,[r3]
	bx		lr
@;------------------------------------------------------
WriteSram: @;(u32 address, u8 *data, u32 size)
	ADD 		R2,R2,R0
	SUB 		R1,R1,R0
wSram_loop1:
	CMP     R0, R2
	BNE     wSram_loop
	BX      LR
wSram_loop:
	LDR     R3, [R1,R0]
	STRB    R3, [R0],#1
	LSR     R3, R3, #0x8
	STRB    R3, [R0],#1
	LSR     R3, R3, #0x8
	STRB    R3, [R0],#1
	LSR     R3, R3, #0x8
	STRB    R3, [R0],#1	
	B       wSram_loop1
@;------------------------------------------------------
ReadSram: @;(u32 address, u8 *data, u32 size)
	ADDS    R2, R2, R0
	SUBS    R1, R1, R0
rSram_loop1:
	CMP     R0, R2
	BNE     rSram_loop
	BX      LR
rSram_loop:
	LDRB    R3, [R0]
	LSL 		R4,R3,#0
	LDRB    R3, [R0,#1]
	LSL 		R5,R3,#8
	ORR			R4,R5
	LDRB    R3, [R0,#2]
	LSL 		R5,R3,#16
	ORR			R4,R5
	LDRB    R3, [R0,#3]
	LSL 		R5,R3,#24
	ORR			R4,R5
	STR     R4, [R1,R0]
	ADDS    R0, #4
	B       rSram_loop1
@;------------------------------------------------------
@;------------------------------------------------------
restore2_IO:        @; IOaddress, offset
	LDRB    R3, [R1]
	LSL 		R4,R3,#0
	LDRB    R3, [R1,#1]
	LSL 		R5,R3,#8
	ORR			R4,R5
	STRH    R4, [R0]
	bx lr	
	.ltorg	
@;------------------------------------------------------
@;------------------------------------------------------
call_Save:	
	@;adrl	r7,RTS_switch
	@;ldr		r7,[r7]
	@;cmp 	r7,#1
	@;bne 	errorRTS
	adrl		r2, spend_0x80
	ldr			r2,[r2]
	stmia		r2!,{r4-r11,sp,lr} @;0x0
	mrs	 		r3,SPSR
	stmia		r2!,{r3}      			@;0x28
	
	stmfd		sp!,{r0-r10,lr}
	bl			BAK_	

	@;02000000-0203FFFF   WRAM - On-board Work RAM  (256 KBytes)
	mov		r8,#0x40   @; 0x40 0x50 0x60 0x70
	mov   r9,#0x2000000
wram_2000000:
	mov 	r0,r8
	bl 		SetRampage	
	mov 	r0,#0x0E000000
	mov 	r1,r9
	mov		r2,#0x10000
	bl 		WriteSram
	add 	r8,#0x10
	add 	r9,#0x10000
	cmp 	r8,#0x80	
	bne 	wram_2000000
	
	@;03000000-03007FFF   WRAM - On-chip Work RAM   (32 KBytes)
	mov 	r0,#0x80
	bl 		SetRampage	
	mov r0,#0x0E000000
	mov r1,#0x3000000
	mov r2,#0x8000
	bl 	WriteSram
	
	@;05000000-050003FF   BG/OBJ Palette RAM        (1 Kbyte)
	ldr r0,=0x0E008000
	mov r1,#0x5000000
	mov r2,#0x400
	bl 	WriteSram	
		
	mov 	r0,#0x90
	bl 		SetRampage	
	mov r0,#0x0E000000
	mov r1,#0x6000000
	mov r2,#0x10000
	bl 	WriteSram
	
	mov 	r0,#0xA0
	bl 		SetRampage	
	mov r0,#0x0E000000
	ldr r1,=0x6010000
	mov r2,#0x8000
	bl 	WriteSram
	
	@;07000000-070003FF   OAM - OBJ Attributes      (1 Kbyte)
	ldr r0,=0x0E008000
	mov r1,#0x7000000
	mov r2,#0x400
	bl 	WriteSram

	@;R4-R11
	mrs	  r0,CPSR    @;Back up
	adrl	r7, spend_0x80
	ldr		r7,[r7]
	add		r7,#0x50  @;{r4-r11,sp,lr} SPSR  0x28+4
		
	mov		r1, #0xDF		@; Switch to systme Mode
	msr		cpsr_cf, r1
	NOP
	mov		r6,sp
	stmia r7!,{r6,lr}	
	
	msr 	cpsr_cf,r0	;@return IRQ mode	
	NOP
	
	ldr 	r0,=0x0E008400
	adrl 	r1, spend_0x80
	ldr		r1,[r1]
	mov 	r2,#0x80
	bl 		WriteSram	
	
	@;04000000-040003FE   I/O Registers
	ldr r0,=0x0E009000
	mov r1,#0x4000000
	mov r2,#0x210  @;#0x60					@;0x0-0x60
	bl 	WriteSram
	
	@;FLAG	
	ldr r0,=0x0E00FFF0
	adrl r1,S_RTS_FLAG
	mov r2,#0x10
	bl 	WriteSram	
	
save_exit:
	mov 	r0,#0x0
	bl 		SetRampage

	BL 		Restore_	
	ldmfd	sp!,{r0-r10,lr}
	@;mov 	r0,#0x04000000
	ldr 	pc,[r0,#-(0x04000000-0x03FFFFF4)] @;to normal IRQ routine	
@;===================================================	
BAK_:	
	adrl		R2, spend_0x80
	ldr			R2,[R2]
	add 		R2,#0x30
	LDR			R0, =0x4000200
	MOV			R1, #0
	
	LDRH		R3, [R0,#8]
	STRH		R3, [R2,#0]
	STRH		R1, [R0,#8]@;v4000208 = 0;
		
	LDR			R0, =0x4000100
	LDRH		R3, [R0,#2]
	STRH		R3, [R2,#2]
	STRH		R1, [R0,#2]@;v4000102 = 0;
	
	LDRH		R3, [R0,#6]
	STRH		R3, [R2,#4]
	STRH    R1, [R0,#6]@;v4000106 = 0;
	
	LDRH    R3, [R0,#0xA]
	STRH		R3, [R2,#6]
	STRH    R1, [R0,#0xA]@;v400010A = 0;
	
	LDRH    R3, [R0,#0xE]
	STRH		R3, [R2,#8]
	STRH    R1, [R0,#0xE]@;v400010E = 0;
	MOV			PC, LR
@;===================================================			
Restore_:
	adrl		R3, spend_0x80
	ldr			R3,[R3]
	add 		R3,#0x30
	
	LDR			R0, =0x4000100
	
	LDRH		R1, [R3,#2]
	STRH		R1, [R0,#2]@;v4000102
	
	LDRH		R1, [R3,#4]
	STRH		R1, [R0,#6]@;v4000106;
	
	LDRH		R1, [R3,#6]
	STRH		R1, [R0,#0xA]@;v400010A;
	
	LDRH		R1, [R3,#8]
	STRH		R1, [R0,#0xE]@;v400010E;
	
	LDR			R0, =0x4000200
	LDR			R2, =0x0
	STRH		R2, [R0,#0x2]@;v4000202;
	LDRH		R1, [R3,#0]
	STRH		R1, [R0,#0x8]@;v4000208;	 IME
	MOV			PC, LR
@;===================================================	
call_Load:	
	@;check 	;FLAG	
	stmfd		sp!,{r0-r3,lr}	
	mov 	r0,#0xA0
	bl 		SetRampage
	ldr 	r0,=0x0E00FFF0
	adrl 	r1, spend_0x80  @;temp buff
	ldr 	r1,[r1]
	mov 	r2,#0x10
	bl 		ReadSram	
	
	adrl 	r1,S_RTS_FLAG	
	adrl 	r2, spend_0x80 @;temp buff
	ldr 	r2,[r2]
	mov 	r3,#0
loop_check:
	ldr 	r4,[r1],#4
	ldr 	r5,[r2],#4
	cmp 	r4,r5
	bne 	errorRTS
	add 	r3,#1
	cmp 	r3,#4
	bne 	loop_check
	b 		checkOK
errorRTS:
	mov 	r0,#0x0
	bl 		SetRampage		
	ldmfd		sp!,{r0-r3,lr}
	ldr 		pc,[r0,#-(0x04000000-0x03FFFFF4)] @;to normal IRQ routine		
@;===================================================	
@;===================================================	
checkOK:
	ldr r0,=0x4000208   @;0x4000208
	mov r1,#0
	strh r1,[r0]

	@;02000000-0203FFFF   WRAM - On-board Work RAM  (256 KBytes)
	mov		r8,#0x40   @; 0x40 0x50 0x60 0x70
	mov   r9,#0x2000000
wram_2000000_Load:
	mov 	r0,r8
	bl 		SetRampage	
	mov 	r0,#0x0E000000
	mov 	r1,r9
	mov		r2,#0x10000
	bl 		ReadSram
	add 	r8,#0x10
	add 	r9,#0x10000
	cmp 	r8,#0x80	
	bne 	wram_2000000_Load

	@;03000000-03007FFF   WRAM - On-chip Work RAM   (32 KBytes)
	mov 	r0,#0x80
	bl 		SetRampage	
	mov r0,#0x0E000000
	mov r1,#0x3000000
	mov r2,#0x8000
	bl 	ReadSram
	
	@;05000000-050003FF   BG/OBJ Palette RAM        (1 Kbyte)
	ldr r0,=0x0E008000
	mov r1,#0x5000000
	mov r2,#0x400
	bl 	ReadSram

	@;06000000-06017FFF   VRAM - Video RAM          (96 KBytes)
	mov 	r0,#0x90
	bl 		SetRampage	
	mov r0,#0x0E000000
	mov r1,#0x6000000
	mov r2,#0x10000
	bl 	ReadSram
	mov 	r0,#0xA0
	bl 		SetRampage	
	mov r0,#0x0E000000
	ldr r1,=0x6010000
	mov r2,#0x8000
	bl 	ReadSram
	
	@;07000000-070003FF   OAM - OBJ Attributes      (1 Kbyte)
	ldr r0,=0x0E008000
	mov r1,#0x7000000
	mov r2,#0x400
	bl 	ReadSram
	
	@;-------------------------------------
	mov r10,#0x4000000
	LDR r11,=0x0E009000
	
	adr r9,register_list
register_list_loop:
	ldrh r2,[r9],#2
	cmp r2 ,#0xFF00
	beq register_list_end

	add r0,r10,r2   @;0x4000000  0x4000002 0x4000004
	add r1,r11,r2
	bl  restore2_IO
	b 	register_list_loop
register_list_end:

	@;mov r10,#0x4000000
	@;LDR r11,=0x0E008500		
@;	add r0,r10,#0xBA  @;0x40000BA  DMA
@;	add r1,r11,#0x32
@;	bl  restore2_IO	
	@;add r0,r10,#0xC6  @;0x40000C6
	@;add r1,r11,#0x34
	@;bl  restore2_IO	
@;	add r0,r10,#0xD2  @;0x40000D2
@;	add r1,r11,#0x36
@;	bl  restore2_IO	
@;	add r0,r10,#0xDE  @;0x40000DE
@;	add r1,r11,#0x38
@;	bl  restore2_IO	
	
	@;mov   r4,#0x8F
	@;mov   r7,#0x4000000
	@;strh 	r4,[r7,#REG_SOUNDCNT_X]
	
	@;ldr r0,=0x4000202   @;0x4000202
	@;mov r1,#0
	@;strh r1,[r0]	

	ldr 	r0,=0x0E008400
	adrl 	r1, spend_0x80 @;temp buff
	ldr		r1,[r1] 
	mov 	r2,#0x80
	bl 		ReadSram	
	
	mrs	  r0,CPSR
	adrl	r7, spend_0x80
	ldr		r7,[r7] 
	add		r7,#0x28      @;SPSR offset
	
	ldmia	r7!,{r2}			@;r7=0x2C
	msr		SPSR_cxsf,r2	@;restore SPSR_irq  

	mov		r1, #0xDF		  @;Switch to systme Mode
	msr		cpsr_cf, r1
	NOP
	add		r7,#0x24					@;offset 0x50
	ldmia r7!,{r13-r14}

	msr 	cpsr_cf,r0	  @;restore IRQ	
	NOP

	mov 	r0,#0x0
	bl 		SetRampage
		
	BL 		Restore_	
	
	adrl	r12, spend_0x80
	ldr		r12,[r12] 
	ldmia r12!,{r4-r11,sp,lr}
	mov 	r0,#0x04000000
	ldr 	pc,[r0,#-(0x04000000-0x03FFFFF4)] @;to normal IRQ routine		

	@;===================================================		
	.ltorg	
	@;===================================================		
register_list:
	.hword 0x0000,0x0002,0x0004,0x0008,0x000A,0x000C,0x000E
	.hword 0x0048,0x004A,0x0050,0x0052
	.hword 0x0084
	.hword 0x0060,0x0062,0x0068,0x0070,0x0072,0x0078
	.hword 0x0080,0x0082,0x0088                 @;0x008C,0x008E,
	.hword 0x0090,0x0092,0x0094,0x0096,0x0098,0x009A,0x009C,0x009E
	
	.hword 0x00B8,0x00C4,0x00D0,0x00DC  @;DMA
	.hword 0x0120,0x0122,0x0124,0x0126,0x0128,0x012A,0x012C,0x0132,0x0134
	.hword 0x0140,0x0150,0x0154    
	@;,0x0200,0x0204,0x0208
	.hword 0xFF00
	.align
@;------------------------------------------------	

	.align	
S_RTS_FLAG:
 	.byte  'E','Z','-','O','m','e','g','a','R','T','C','F','I','L','E','.'
	.align	
RTS_only_ReplaceIRQ_end:
   .end
