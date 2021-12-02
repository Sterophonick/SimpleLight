@	#include "equates.h"
@	#include "memory.h"
@	#include "lcd.h"
@	#include "sound.h"
@	#include "cart.h"
@	#include "gbz80.h"
@	#include "gbz80mac.h"
@	#include "sgb.h"
	
    @.global _doing_hdma
    
	global_func _E0
	global_func _E2
	global_func _F0
	global_func _F2
	
	global_func IO_reset
	global_func IO_R
	global_func IO_High_R
	global_func IO_W
	global_func IO_High_W
	
	.global io_read_tbl
	.global io_write_tbl
	
	.global joypad_write_ptr
	global_func joy0_W
	.global joycfg
	global_func suspend
	global_func refreshNESjoypads
@	global_func serialinterrupt
@	global_func resetSIO
	global_func thumbcall_r1
	global_func gettime
	global_func vbaprint
	global_func waitframe
	global_func LZ77UnCompVram
	global_func CheckGBAVersion
@	.global gbpadress
	global_func _FF70W
	.global FF41_R_ptr
	.global FF44_R_ptr
	global_func jump_r0
 .if RESIZABLE
	@IMPORT add_exram
 .endif

	global_func doReset

 .align
 .pool
 .text
 .align
 .pool

	global_func breakpoint
breakpoint:
	mov r11,r11
	bx lr



vbaprint:
	swi 0xFF0000		@!!!!!!! Doesn't work on hardware !!!!!!!
	bx lr
LZ77UnCompVram:
	swi 0x120000
	bx lr
waitframe:
VblWait:
	mov r0,#0				@don't wait if not necessary
	mov r1,#1				@VBL wait
	swi 0x040000			@ Turn off CPU until VBLANK IRQ if not too late already.
	bx lr
CheckGBAVersion:
	ldr r0,=0x5AB07A6E		@Fool proofing
	mov r12,#0
	swi 0x0D0000			@GetBIOSChecksum
	ldr r1,=0xABBE687E		@Proto GBA
	cmp r0,r1
	moveq r12,#1
	ldr r1,=0xBAAE187F		@Normal GBA
	cmp r0,r1
	moveq r12,#2
	ldr r1,=0xBAAE1880		@Nintendo DS
	cmp r0,r1
	moveq r12,#4
	mov r0,r12
	bx lr

jump_r0:
	bx r0


@----------------------------------------------------------------------------
doReset:
	mov r1,#REG_BASE
	mov r0,#0
	strh r0,[r1,#REG_DM0CNT_H]	@stop all DMA
	strh r0,[r1,#REG_DM1CNT_H]
	strh r0,[r1,#REG_DM2CNT_H]
	strh r0,[r1,#REG_DM3CNT_H]
	add r1,r1,#0x200
	str r0,[r1,#8]		@interrupts off
	
	#if VISOLY

	@copy code to EWRAM and execute it	
	ldr r0,=XGB_VRAM		@temporary buffer for reset code
	ldr r1,=VISOLY_START	@source address
	ldr r2,=VISOLY_END	@end
	sub r2,r2,r1		@subtract to get size
	mov lr,r0
	b memcpy32_  @and jump to code too

	#include "visoly.s"

	#else

	mov		r0, #0
	ldr		r1,=0x3007ffa	@must be 0 before swi 0x00 is run, otherwise it tries to start from 0x02000000.
	strh		r0,[r1]
	mov		r0, #8		@VRAM clear
	swi		0x010000
	swi		0x000000

	#endif

@----------------------------------------------------------------------------
suspend:	@called from ui.c and 6502.s
@----------------------------------------------------------------------------
	mov r3,#REG_BASE

	ldr r1,=REG_P1CNT
	ldr r0,=0xc00c			@interrupt on start+sel
	strh r0,[r3,r1]

	ldrh r1,[r3,#REG_SGCNT_L]
	strh r3,[r3,#REG_SGCNT_L]	@sound off

	ldrh r0,[r3,#REG_DISPCNT]
	orr r0,r0,#0x80
	strh r0,[r3,#REG_DISPCNT]	@LCD off

	swi 0x030000

	ldrh r0,[r3,#REG_DISPCNT]
	bic r0,r0,#0x80
	strh r0,[r3,#REG_DISPCNT]	@LCD on

	strh r1,[r3,#REG_SGCNT_L]	@sound on

	bx lr

@----------------------------------------------------------------------------
thumbcall_r1: bx r1
@----------------------------------------------------------------------------
gettime:	@called from ui.c and mappers.s
@----------------------------------------------------------------------------
	ldr r3,=0x080000c4		@base address for RTC
	mov r1,#1
	strh r1,[r3,#4]			@enable RTC
	mov r1,#7
	strh r1,[r3,#2]			@enable write

	mov r1,#1
	strh r1,[r3]
	mov r1,#5
	strh r1,[r3]			@State=Command

	mov r2,#0x65			@r2=Command, YY:MM:DD 00 hh:mm:ss
	mov addy,#8
RTCLoop1:
	mov r1,#2
	and r1,r1,r2,lsr#6
	orr r1,r1,#4
	strh r1,[r3]
	mov r1,r2,lsr#6
	orr r1,r1,#5
	strh r1,[r3]
	mov r2,r2,lsl#1
	subs addy,addy,#1
	bne RTCLoop1

	mov r1,#5
	strh r1,[r3,#2]			@enable read
	mov r2,#0
	mov addy,#32
RTCLoop2:
	mov r1,#4
	strh r1,[r3]
	mov r1,#5
	strh r1,[r3]
	ldrh r1,[r3]
	and r1,r1,#2
	mov r2,r2,lsr#1
	orr r2,r2,r1,lsl#30
	subs addy,addy,#1
	bne RTCLoop2

	mov r0,#0
	mov addy,#24
RTCLoop3:
	mov r1,#4
	strh r1,[r3]
	mov r1,#5
	strh r1,[r3]
	ldrh r1,[r3]
	and r1,r1,#2
	mov r0,r0,lsr#1
	orr r0,r0,r1,lsl#22
	subs addy,addy,#1
	bne RTCLoop3
	str_ r2,mapperdata+24
	str_ r0,mapperdata+28

	bx lr

 .section .iwram.1, "ax", %progbits

@----------------------------------------------------------------------------
refreshNESjoypads:	@call every frame
@exits with Z flag clear if update incomplete (waiting for other player)
@is my multiplayer code butt-ugly?  yes, I thought so.
@i'm not trying to win any contests here.
@----------------------------------------------------------------------------
	ldr_ r1,frame
	movs r0,r1,lsr#2 @C=frame&2 (autofire alternates every other frame)
	ldr_ r1,XGBjoypad
	and r0,r1,#0xf0
	ldr r2,joycfg
	andcs r1,r1,r2
	movcss addy,r1,lsr#9	@R?
	andcs r1,r1,r2,lsr#16
	and r1,r1,#0x0f		@startselectBA
	tst r2,#0x400			@Swap A/B?
	adrne addy,ssba2ssab
	ldrneb r1,[addy,r1]	@startselectBA
	orr r0,r1,r0		@r0=joypad state
	
	str_ r0,joy0state
	bx lr

joycfg: .word 0x40ff01ff @byte0=auto mask, byte1=(saves R)bit2=SwapAB, byte2=R auto mask
@bit 31=single/multi, 30,29=1P/2P, 27=(multi) link active, 24=reset signal received
ssba2ssab: .byte 0x00,0x02,0x01,0x03, 0x04,0x06,0x05,0x07, 0x08,0x0a,0x09,0x0b, 0x0c,0xe0,0xd0,0x0f


@----------------------------------------------------------------------------
_F0:@	LD A,($FF00+nn)
@----------------------------------------------------------------------------
	ldrb r2,[gb_pc],#1
	adr lr,_after_IO_High_R
@----------------------------------------------------------------------------
_IO_High_R:	@I/O read
@----------------------------------------------------------------------------
	tst r2,#0x80
	adr r1,io_read_tbl
	ldreq pc,[r1,r2,lsl#2]
@----------------------------------------------------------------------------
_HRAM_R:@		(FF80-FFFF)
@----------------------------------------------------------------------------
	adr_ r1,xgb_hram-0x80
	ldrb r0,[r1,r2]
_after_IO_High_R:
	mov gb_a,r0,lsl#24
	fetch 12

@----------------------------------------------------------------------------
_F2:@	LD A,($FF00+C)
@----------------------------------------------------------------------------
	mov r2,gb_bc,lsr#16
	and r2,r2,#0xFF
	adr lr,_after_IO_High_R_2
@----------------------------------------------------------------------------
_IO_High_R_2:	@I/O read
@----------------------------------------------------------------------------
	tst r2,#0x80
	adr r1,io_read_tbl
	ldreq pc,[r1,r2,lsl#2]
@----------------------------------------------------------------------------
_HRAM_R_2:@		(FF80-FFFF)
@----------------------------------------------------------------------------
	adr_ r1,xgb_hram-0x80
	ldrb r0,[r1,r2]
_after_IO_High_R_2:
	mov gb_a,r0,lsl#24
	fetch 8


@----------------------------------------------------------------------------
IO_R:		@I/O read
@----------------------------------------------------------------------------
	and r2,addy,#0xFF
	cmp addy,#0xFF00
	bmi OAM_R	@OAM, C000-DCFF mirror.
@----------------------------------------------------------------------------
IO_High_R:	@I/O read
@----------------------------------------------------------------------------
	tst r2,#0x80
	ldreq pc,[pc,r2,lsl#2]
	b HRAM_R
@----------------------------------------------------------------------------
io_read_tbl:
joypad_read_ptr:
	.word joy0_R	@$FF00: Joypad 0 read
	.word _FF01R	@SB - Serial Transfer Data
	.word _FF02R	@SC - Serial Transfer Ctrl
	.word void
	.word _FF04R	@DIV - Divider Register
	.word _FF05R	@TIMA - Timer counter
	.word _FF06R	@TMA - Timer Modulo
	.word _FF07R	@TAC - Timer Control
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word _FF0FR	@IF Interrupt flags.

	.word _FF10R	@Sound channel 1
	.word _FF11R
	.word _FF12R
	.word _FF13R
	.word _FF14R
	.word void
	.word _FF16R	@Sound channel 2
	.word _FF17R
	.word _FF18R
	.word _FF19R
	.word _FF1AR	@Sound channel 3
	.word _FF1BR
	.word _FF1CR
	.word _FF1DR
	.word _FF1ER
	.word void

	.word _FF20R	@Sound channel 4
	.word _FF21R
	.word _FF22R
	.word _FF23R
	.word _FF24R
	.word _FF25R
	.word _FF26R
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void

	.word _FF30R	@Sound channel 3 wave ram
	.word _FF30R
	.word _FF30R
	.word _FF30R
	.word _FF30R
	.word _FF30R
	.word _FF30R
	.word _FF30R
	.word _FF30R
	.word _FF30R
	.word _FF30R
	.word _FF30R
	.word _FF30R
	.word _FF30R
	.word _FF30R
	.word _FF30R

	.word FF40_R	@LCDC - LCD Control
FF41_R_ptr:
	.word FF41_R	@STAT - LCDC Status
	.word FF42_R	@SCY - Scroll Y
	.word FF43_R	@SCX - Scroll X
FF44_R_ptr:
	.word FF44_R	@LY - LCD Y-Coordinate
	.word FF45_R	@LYC - LCD Y Compare
	.word void	@DMA - DMA Transfer and Start Address (W?)
	.word FF47_R	@BGP - BG Palette Data - Non CGB Mode Only
	.word FF48_R	@OBP0 - Object Palette 0 Data - Non CGB Mode Only
	.word FF49_R	@OBP1 - Object Palette 1 Data - Non CGB Mode Only
	.word FF4A_R	@WY - Window Y Position
	.word FF4B_R	@WX - Window X Position minus 7
	.word void
	.word FF4D_R	@KEY1 - bit 7, Read current speed of CPU - CGB Mode Only
	.word void
	.word FF4F_R	@VBK - VRAM Bank - CGB Mode Only

	.word void
	.word FF51_R	@HDMA1
	.word FF52_R	@HDMA2
	.word FF53_R	@HDMA3
	.word FF54_R	@HDMA4
	.word FF55_R	@HDMA5
	.word _FF56R	@RP - Infrared Port
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void

	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word FF68_R	@BCPS - BG Color Palette Specification  CGB Mode Only
	.word FF69_R	@BCPD - BG Color Palette Data
	.word FF6A_R	@OCPS - OBJ Color Palette Specification
	.word FF6B_R	@OCPD - OBJ Color Palette Data
	.word void
	.word void
	.word void
	.word void

	.word _FF70R	@SVBK - WRAM Bank - CGB Mode Only
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void


@OAM_R
@	cmp addy,#0xFE00
@	ldrpl r1,=GBOAM_BUFFER
@	ldrplb r0,[r1,r2]
@	movpl pc,lr
@	sub addy,addy,#0x2000		;C000-DCFF mirror.
@	b mem_RC0
@OAM_W
@	cmp addy,#0xFE00
@	ldrpl r1,=GBOAM_BUFFER
@	strplb r0,[r1,r2]
@	movpl pc,lr
@	sub addy,addy,#0x2000		;C000-DCFF mirror.
@	b wram_W

@----------------------------------------------------------------------------
_E0:@	LD ($FF00+nn),A
@----------------------------------------------------------------------------
	ldrb r2,[gb_pc],#1
	mov r0,gb_a,lsr#24
	
	adr lr,_after_IO_High_W
@----------------------------------------------------------------------------
_IO_High_W:	@I/O write
@----------------------------------------------------------------------------
	tst r2,#0x80
	adr r1,io_write_tbl
	ldreq pc,[r1,r2,lsl#2]
@----------------------------------------------------------------------------
_HRAM_W:@	(FF80-FFFF)
@----------------------------------------------------------------------------
	adr_ r1,xgb_hram-0x80
	strb r0,[r1,r2]
	cmp r2,#0xFF
	beq HRAM_W_IE_
_after_IO_High_W:
	fetch 12
@----------------------------------------------------------------------------
_E2:@	LD ($FF00+C),A
@----------------------------------------------------------------------------
	mov r2,gb_bc,lsr#16
	and r2,r2,#0xFF
	mov r0,gb_a,lsr#24
	
	adr lr,_after_IO_High_W_2
@----------------------------------------------------------------------------
_IO_High_W_2:	@I/O write
@----------------------------------------------------------------------------
	tst r2,#0x80
	adr r1,io_write_tbl
	ldreq pc,[r1,r2,lsl#2]
@----------------------------------------------------------------------------
_HRAM_W_2:@	(FF80-FFFF)
@----------------------------------------------------------------------------
	adr_ r1,xgb_hram-0x80
	strb r0,[r1,r2]
	cmp r2,#0xFF
	beq HRAM_W_IE_
_after_IO_High_W_2:
	fetch 8


@----------------------------------------------------------------------------
IO_W:		@I/O write
@----------------------------------------------------------------------------
	and r2,addy,#0xFF
	cmp addy,#0xFF00
	bmi OAM_W	@OAM, C000-DCFF mirror.
@----------------------------------------------------------------------------
IO_High_W:	@I/O write
@----------------------------------------------------------------------------
	tst r2,#0x80
	ldreq pc,[pc,r2,lsl#2]
	b HRAM_W
@----------------------------------------------------------------------------
io_write_tbl:
joypad_write_ptr:
	.word joy0_W	@$FF00: Joypad write
	.word _FF01W	@SB - Serial Transfer Data
	.word _FF02W	@SC - Serial Transfer Ctrl
	.word void
	.word _FF04W	@DIV - Divider Register
	.word _FF05W	@TIMA - Timer counter
	.word _FF06W	@TMA - Timer Modulo
	.word _FF07W	@TAC - Timer Control
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word _FF0FW	@IF Interrupt flags.

	.word _FF10W	@Sound channel 1
	.word _FF11W
	.word _FF12W
	.word _FF13W
	.word _FF14W
	.word void
	.word _FF16W	@Sound channel 2
	.word _FF17W
	.word _FF18W
	.word _FF19W
	.word _FF1AW	@Sound channel 3
	.word _FF1BW
	.word _FF1CW
	.word _FF1DW
	.word _FF1EW
	.word void

	.word _FF20W	@Sound channel 4
	.word _FF21W
	.word _FF22W
	.word _FF23W
	.word _FF24W
	.word _FF25W
	.word _FF26W
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void

	.word _FF30W	@Sound channel 3 wave ram
	.word _FF30W
	.word _FF30W
	.word _FF30W
	.word _FF30W
	.word _FF30W
	.word _FF30W
	.word _FF30W
	.word _FF30W
	.word _FF30W
	.word _FF30W
	.word _FF30W
	.word _FF30W
	.word _FF30W
	.word _FF30W
	.word _FF30W

	.word FF40_W	@LCDC - LCD Control
	.word FF41_W	@STAT - LCDC Status
	.word FF42_W	@SCY - Scroll Y
	.word FF43_W	@SCX - Scroll X
	.word void  	@LY - LCD Y-Coordinate
	.word FF45_W	@LYC - LCD Y Compare
	.word FF46_W	@DMA - DMA Transfer and Start Address
	.word FF47_W	@BGP - BG Palette Data - Non CGB Mode Only
	.word FF48_W	@OBP0 - Obj Palette 0 Data - Non CGB Mode Only
	.word FF49_W	@OBP1 - Obj Palette 1 Data - Non CGB Mode Only
	.word FF4A_W	@WY - Window Y Position
	.word FF4B_W	@WX - Window X Position minus 7
	.word void
	.word FF4D_W	@KEY1 - Prepare Speed Switch - CGB Mode Only
	.word void
	.word FF4F_W	@VBK - VRAM Bank - CGB Mode Only

	.word _FF50W	@BIOS bank select
	.word FF51_W	@HDMA1
	.word FF52_W	@HDMA2
	.word FF53_W	@HDMA3
	.word FF54_W	@HDMA4
	.word FF55_W	@HDMA5
	.word _FF56W	@RP - Infrared Port
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void

	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word FF68_W	@BCPS - BG Color Palette Specification
	.word FF69_W	@BCPD - BG Color Palette Data
	.word FF6A_W	@OCPS - OBJ Color Palette Specification
	.word FF6B_W	@OCPD - OBJ Color Palette Data
	.word void
	.word void
	.word void
	.word void

	.word _FF70W	@SVBK - WRAM Bank - CGB Mode Only
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void
	.word void

@----------------------------------------------------------------------------
HRAM_W:@		(FF80-FFFF)
@----------------------------------------------------------------------------
	adr_ r1,xgb_hram-0x80
	strb r0,[r1,r2]
	cmp r2,#0xFF
	bxne lr
HRAM_W_IE_:
	strb_ r0,gb_ie
	b immediate_check_irq

@----------------------------------------------------------------------------
HRAM_R:@		(FF80-FFFF)
@----------------------------------------------------------------------------
	adr_ r1,xgb_hram-0x80
	ldrb r0,[r1,r2]
	bx lr
@----------------------------------------------------------------------------
_FF70W:@		SVBK - CGB Mode Only - WRAM Bank
@----------------------------------------------------------------------------
	strb_ r0,rambank
	and r0,r0,#7
	cmp r0,#2
	bge select_gbc_ram
	ldr_ r1,memmap_tbl+48
	str_ r1,memmap_tbl+52
	ldr r1,=wram_W
	str_ r1,writemem_tbl+52
    
wram_remap_pc:
	ldr_ r1,lastbank
	sub gb_pc,gb_pc,r1
	stmfd sp!,{r0}
	encodePC
	ldmfd sp!,{r0}

	mov pc,lr

select_gbc_ram:
 .if RESIZABLE
	ldr_ r1,gbc_exram
	subs r1,r1,#0xD000+0x2000
	bmi add_exram_
 .else	
	ldr r1,=GBC_EXRAM-0xD000-0x2000
 .endif
	add r1,r1,r0,lsl#12
	@D000
	str_ r1,memmap_tbl+52
	ldr r1,=wram_W_2
	str_ r1,writemem_tbl+52

	@mov pc,lr
    b wram_remap_pc
 .if RESIZABLE
add_exram_:
	stmfd sp!,{r0-addy,lr}
	ldr r1,=add_exram
	mov lr,pc
	bx r1
@	bl thumbcall_r1
	ldmfd sp!,{r0-addy,lr}
	ldr_ r1,lastbank
	sub gb_pc,gb_pc,r1
	stmfd sp!,{r0}
	encodePC
	ldmfd sp!,{r0}
	b select_gbc_ram
 .endif

@----------------------------------------------------------------------------
_FF70R:@		SVBK - CGB Mode Only - WRAM Bank
@----------------------------------------------------------------------------
	ldrb_ r0,rambank
	mov pc,lr

@----------------------------------------------------------------------------
joy0_W:		@FF00
@----------------------------------------------------------------------------
	orr r0,r0,#0xCF
	mov r2,#0x0
	ldrb_ r1,joy0state
	tst r0,#0x10		@Direction
	orreq r2,r2,r1,lsr#4
	tst r0,#0x20		@Buttons
	orreq r2,r2,r1

	and r2,r2,#0x0F
	eor r2,r2,r0
	ldrb_ r1,joy0serial
	strb_ r2,joy0serial
	
	bx lr
@----------------------------------------------------------------------------
joy0_R_SGB: @FF00
@----------------------------------------------------------------------------
	ldrb_ r1,lineslow
	orr r1,r1,#0x04
	strb_ r1,lineslow
	@continue to @joy0_R
@----------------------------------------------------------------------------
joy0_R:		@FF00
@----------------------------------------------------------------------------
	ldrb_ r0,joy0serial

joy0_R_subsequent_check:
	
	#if JOYSTICK_READ_HACKS
	@joystick polling speed hack
	ldrsb r1,[r10,#joy_read_count]
	adds r1,r1,#1
	andpl cycles,cycles,#CYC_MASK
@	subpl r1,r1,#16
	strb_ r1,joy_read_count
	#endif
	
	@check for subsequent F0 00 instructions (very common to see 2, or 6 or 8 in a row)
	ldrb r1,[gb_pc]
	cmp r1,#0xF0
	bxne lr
	mov r2,r9
0:
	ldreqb r1,[gb_pc,#1]
	cmpeq r1,#0x00
	ldreqb r1,[gb_pc,#2]!
	cmp r1,#0xF0
	beq 0b
	subs r2,r9,r2
	@r2 = number of bytes advanced, add half to get number of instructions * 3
	add r2,r2,r2,lsr#1
	sub cycles,cycles,r2,lsl#(CYC_SHIFT + 2)
	mov pc,lr
@----------------------------

FF55_W:	@HDMA5
	tst r0,#0x80
@	bxne lr
	bic r0,r0,#0x80
	
	@immediately steal cycles if it's not HDMA
	bne start_hdma
    @ General DMA code below
    @ Write HDMA cancel block here
    ldrb_ r1,dma_blocks_total
    cmp r1,#0
    beq general_dma
cancel_hdma:
    stmfd sp!,{r0-r4,lr}
    ldrb_ r0,dma_blocks_total
    ldrb_ r1,dma_blocks_remaining
    sub r0,r0,r1
    lsls r0,r0,#4
    blxne_long DoDma
    ldmfd sp!,{r0-r4,lr}
    
    ldr r1,=_dma_blocks_total
    mov r2,#0x00
    strb r2,[r1]
    
    bx lr  @ I'm not sure if this is right or not
general_dma:
    @ Steal cycles
	ldr_ r1,cyclesperscanline
	cmp r1,#DOUBLE_SPEED
	add r1,r0,#1
	moveq r1,r1,lsl#1
	mov r1,r1,lsl#(3 + CYC_SHIFT)
	sub cycles,cycles,r1
    
    @ Immediately call DoDma
    stmfd sp!,{r3,lr}
	add r0,r0,#1
	mov r0,r0,lsl#4
	blx_long DoDma
	ldmfd sp!,{r3,pc}
    
start_hdma:
    ldrb_ r1,dmamode
    cmp r1,#2
    beq general_dma  @ For compatibility with Shantae and its multitude of speed hacks

    @ HDMA code below
    add r0,r0,#1
    ldr r1,=_dma_blocks_remaining
    ldr r2,=_dma_blocks_total
    strb r0,[r1]
    strb r0,[r2]
    
	bx lr
	
@r0 = dest, r1 = src, r2 = byteCount, r3 = dirtyMapBits
	global_func copy_map_and_compare
copy_map_and_compare:
	stmfd sp!,{r4,r5,r6,r7,r8,r9,r10,r11}
cmc_loop1:
	mov r12,#0
	subs r2,r2,#32
	bmi cmc_part2

	ldmia r0!,{r4,r5,r6,r7}
	ldmia r1!,{r8,r9,r10,r11}
	eors r4,r4,r8
	strne r8,[r0,#-16]
	orrne r12,r12,#0x01
	eors r5,r5,r9
	strne r9,[r0,#-12]
	orrne r12,r12,#0x02
	eors r6,r6,r10
	strne r10,[r0,#-8]
	orrne r12,r12,#0x04
	eors r7,r7,r11
	strne r11,[r0,#-4]
	orrne r12,r12,#0x08
	
	ldmia r0!,{r4,r5,r6,r7}
	ldmia r1!,{r8,r9,r10,r11}
	eors r4,r4,r8
	strne r8,[r0,#-16]
	orrne r12,r12,#0x10
	eors r5,r5,r9
	strne r9,[r0,#-12]
	orrne r12,r12,#0x20
	eors r6,r6,r10
	strne r10,[r0,#-8]
	orrne r12,r12,#0x40
	eors r7,r7,r11
	strne r11,[r0,#-4]
	orrne r12,r12,#0x80
	
	ldrb r4,[r3]
	orr r12,r12,r4
	strb r12,[r3],#1
	b cmc_loop1
cmc_part2:
	adds r2,r2,#32
	ldmlefd sp!,{r4,r5,r6,r7,r8,r9,r10,r11}
	bxle lr
	b_long _cmc_part2_
	.pushsection .text
_cmc_part2_:
	ble cmc_done
	mov r6,#1
cmc_loop2:
	ldr r4,[r0],#4
	ldr r5,[r1],#4
	eors r4,r4,r5
	strne r5,[r0,#-4]
	orrne r12,r12,r6
	mov r6,r6,lsl#1
	subs r2,r2,#4
	bgt cmc_loop2
	ldrb r4,[r3]
	orr r12,r12,r4
	strb r12,[r3],#1
cmc_done:
	ldmfd sp!,{r4,r5,r6,r7,r8,r9,r10,r11}
	bx lr
	.popsection

@	global_func UpdateTiles1
@	global_func UpdateTiles2
@	global_func UpdateTiles3
@	
@	gbcptr	.req r0
@	bytecount	.req r1
@	agbptr1	.req r2
@	agbptr2	.req r3
@	agbptr3	.req r4
@	mask	.req r5
@	gbcword	.req r6
@	dest0	.req r7
@	pixels0	.req r8
@	pixels1	.req r9
@	decodeptr .req r11
@UpdateTiles1:
@	@void UpdateTiles1(byte* gbcptr, int byteCount, int gbaVramAddress);
@	@updates bg tiles at 0x06008000
@	stmfd sp!,{r3-r9,r11}
@	ldr decodeptr,=CHR_DECODE
@	mov mask,#0xFF
@render_tiles_1_loop:
@	ldr gbcword,[gbcptr],#4
@	ands pixels0,mask,gbcword
@	ldrne pixels0,[decodeptr,pixels0,lsl#2]
@	ands pixels1,mask,gbcword,lsr#8
@	ldrne pixels1,[decodeptr,pixels1,lsl#2]
@	orr dest0,pixels0,pixels1,lsl#1
@	ands pixels0,mask,gbcword,lsr#16
@	ldrne pixels0,[decodeptr,pixels0,lsl#2]
@	ands pixels1,mask,gbcword,lsr#24
@	ldrne pixels1,[decodeptr,pixels1,lsl#2]
@	orr pixels1,pixels0,pixels1,lsl#1
@	stmia agbptr1!,{dest0,pixels1}
@	subs bytecount,bytecount,#4
@	bgt render_tiles_1_loop
@	ldmfd sp!,{r3-r9,r11}
@	bx lr
@
@UpdateTiles2:
@	@void UpdateTiles2(byte* gbcptr, int byteCount, int gbaVramAddress);
@	@updates sprite tiles at 0x06000000 and 0x06014000
@	stmfd sp!,{r3-r9,r11}
@	add agbptr2,agbptr1,#0x14000
@	ldr decodeptr,=CHR_DECODE
@	mov mask,#0xFF
@render_tiles_2_loop:
@	ldr gbcword,[gbcptr],#4
@	ands pixels0,mask,gbcword
@	ldrne pixels0,[decodeptr,pixels0,lsl#2]
@	ands pixels1,mask,gbcword,lsr#8
@	ldrne pixels1,[decodeptr,pixels1,lsl#2]
@	orr dest0,pixels0,pixels1,lsl#1
@	ands pixels0,mask,gbcword,lsr#16
@	ldrne pixels0,[decodeptr,pixels0,lsl#2]
@	ands pixels1,mask,gbcword,lsr#24
@	ldrne pixels1,[decodeptr,pixels1,lsl#2]
@	orr pixels1,pixels0,pixels1,lsl#1
@	stmia agbptr1!,{dest0,pixels1}
@	stmia agbptr2!,{dest0,pixels1}
@	subs bytecount,bytecount,#4
@	bgt render_tiles_2_loop
@	ldmfd sp!,{r3-r9,r11}
@	bx lr
@
@UpdateTiles3:
@	@void UpdateTiles3(byte* gbcptr, int byteCount, int gbaVramAddress);
@	@updates shared tiles at 0x06001000, 0x06009000, and 0x06015000
@	stmfd sp!,{r3-r9,r11}
@	add agbptr2,agbptr1,#0x8000
@	add agbptr3,agbptr1,#0x14000
@	ldr decodeptr,=CHR_DECODE
@	mov mask,#0xFF
@render_tiles_3_loop:
@	ldr gbcword,[gbcptr],#4
@	ands pixels0,mask,gbcword
@	ldrne pixels0,[decodeptr,pixels0,lsl#2]
@	ands pixels1,mask,gbcword,lsr#8
@	ldrne pixels1,[decodeptr,pixels1,lsl#2]
@	orr dest0,pixels0,pixels1,lsl#1
@	ands pixels0,mask,gbcword,lsr#16
@	ldrne pixels0,[decodeptr,pixels0,lsl#2]
@	ands pixels1,mask,gbcword,lsr#24
@	ldrne pixels1,[decodeptr,pixels1,lsl#2]
@	orr pixels1,pixels0,pixels1,lsl#1
@	stmia agbptr1!,{dest0,pixels1}
@	stmia agbptr2!,{dest0,pixels1}
@	stmia agbptr3!,{dest0,pixels1}
@	subs bytecount,bytecount,#4
@	bgt render_tiles_3_loop
@	ldmfd sp!,{r3-r9,r11}
@	bx lr
@	.unreq gbcptr
@	.unreq bytecount
@	.unreq agbptr1
@	.unreq agbptr2
@	.unreq agbptr3
@	.unreq mask
@	.unreq gbcword
@	.unreq pixels0
@	.unreq pixels1
@	.unreq dest0
@	.unreq decodeptr


	
@	add r2,r0,#1
@	mov r2,r2,lsl#4
@do_dma:
@	stmfd sp!,{r0-addy,lr}
@	@r2 = byte count
@dma_again:
@	ldr_ r3,dma_src
@	mov r4,r3,lsr#16 @r4 = dest address (16-bit word)
@	ldr r1,=0xFFFF
@	and r3,r3,r1	@r3 = source address (16-bit word)
@	
@@source: ROM0 (0000-3FF0), ROM1 (4000-7FF0), SRAM (A000-BFF0), RAM0 (C000-CFF0) or RAM1 (D000-DFF0)
@@dest: VRAM (8000-9FF0)
@
@	@will source overflow into another page?
@	add r0,r3,r2
@	sub r0,r0,#1
@	eor r0,r0,r3
@	tst r0,#0x1000
@	beq dma_source_ok
@	cmp r3,#0x8000
@	tstlt r0,#0x4000
@	beq dma_source_ok
@	and r0,r3,#0xF000
@	add r0,r0,#0x1000
@	sub r1,r0,r3
@	mov r0,r2
@	mov r2,r0
@	bl do_dma
@	sub r2,r0,r2
@	b dma_again
@dma_source_ok:
@	@will destination overflow?
@	add r0,r4,r2
@	rsbs r1,r0,#0xA000
@	mov r0,r2
@	mov r2,r1
@	bl do_dma
@	sub r2,r0,r2
@	b dma_again
@dma_dest_ok:
@	@get true source address
@	and r1,r3,#0xF000
@	adr_ r12,memmap_tbl
@	ldr r1,[r12,r1,lsr#10]
@	add r1,r1,r3
@	@get true dest address
@	and r0,r4,#0xF000
@	ldr r0,[r12,r0,lsr#10]
@	add r0,r0,r4
@	mov r5,r2
@	@copy r2 bytes from r1 to r0
@	bl memcpy32
@	@increment DMA_SRC
@	add r1,r3,r5
@	add r0,r4,r5
@	bic r0,r0,#0xE000
@	orr r0,r0,#0x8000
@	orr r0,r1,r0,lsl#16
@	str_ r0,dma_src
@	
@	@TODO: mark tiles as dirty, or mark tilemap as dirty
@	@vram address > tile number:
@	ldr_ r2,dirty_tiles
@	
@	
@	
@	
@@	@get true address
@@	and r1,r3,#0xF000
@@	adr_ r2,memmap_tbl
@@	ldr r0,[r2,r1,lsr#10]
@@	add r1,r0,r3
@	
@@	adrl_ r2,GBC_DMA_Cache
@
@
@	
@	
@	
@@dma5_loop:
@@	mov addy,r3
@@	readmem
@@	mov addy,r4
@@	writemem
@@	add r3,r3,#1
@@	add r4,r4,#1
@@	bic r4,r4,#0xE000
@@	orr r4,r4,#0x8000
@@	
@@	subs r5,r5,#1
@@	bne dma5_loop
@
@@	orr r3,r3,r4,lsl#16
@@	str_ r3,dma_src
@	
@	ldmfd sp!,{r0-addy,lr}
@	bx lr

 .align
 .pool
 .text
 .align
 .pool
@----------------------------------------------------------------------------
IO_reset:
@----------------------------------------------------------------------------
	mov r0,#0
	str_ r0,joy0state
	str_ r0,joy0serial
	strb_ r0,stctrl
	str_ r0,timercounter
	str_ r0,timermodulo


	ldrb_ r0,sgbmode
	movs r0,r0
	ldreq r0,=joy0_W
	ldrne r0,=joy0_W_SGB
	ldr r1,=joypad_write_ptr
	str r0,[r1]
	ldreq r0,=joy0_R
	ldrne r0,=joy0_R_SGB
	ldr r1,=joypad_read_ptr
	str r0,[r1]
	
	ldr r1,=io_write_tbl

	@lock GBC registers for non GBC mode
	ldrb_ r0,gbcmode
	movs r0,r0
	bne 0f

	ldr r0,=void
	str r0,[r1,#0x4D*4] @speed switch
	str r0,[r1,#0x4F*4] @vram bank
	str r0,[r1,#0x51*4] @HDMA1
	str r0,[r1,#0x52*4] @HDMA2 
	str r0,[r1,#0x53*4] @HDMA3 
	str r0,[r1,#0x54*4] @HDMA4 
	str r0,[r1,#0x55*4] @HDMA5 
	str r0,[r1,#0x56*4] @infrared
	str r0,[r1,#0x68*4] @bg palette index
	str r0,[r1,#0x69*4] @bg palette data
	str r0,[r1,#0x6A*4] @obj palette index
	str r0,[r1,#0x6B*4] @obj palette data
	str r0,[r1,#0x70*4] @wram bank
	b 1f
	
0:	
	@Allow GBC registers in GBC mode
	ldr r0,=FF4D_W
	str r0,[r1,#0x4D*4] @speed switch
	ldr r0,=FF4F_W
	str r0,[r1,#0x4F*4] @vram bank
	ldr r0,=FF51_W
	str r0,[r1,#0x51*4] @HDMA1
	ldr r0,=FF52_W
	str r0,[r1,#0x52*4] @HDMA2 
	ldr r0,=FF53_W
	str r0,[r1,#0x53*4] @HDMA3 
	ldr r0,=FF54_W
	str r0,[r1,#0x54*4] @HDMA4 
	ldr r0,=FF55_W
	str r0,[r1,#0x55*4] @HDMA5 
	ldr r0,=_FF56W
	str r0,[r1,#0x56*4] @infrared
	ldr r0,=FF68_W
	str r0,[r1,#0x68*4] @bg palette index
	ldr r0,=FF69_W
	str r0,[r1,#0x69*4] @bg palette data
	ldr r0,=FF6A_W
	str r0,[r1,#0x6A*4] @obj palette index
	ldr r0,=FF6B_W
	str r0,[r1,#0x6B*4] @obj palette data
	ldr r0,=_FF70W
	str r0,[r1,#0x70*4] @wram bank
1:
	bx lr

@----------------------------------------------------------------------------
_FF01W:@		SB - Serial Transfer Data
@----------------------------------------------------------------------------
	mov pc,lr
@----------------------------------------------------------------------------
_FF02W:@		SC - Serial Transfer Ctrl
@----------------------------------------------------------------------------
	strb_ r0,stctrl
	and r0,r0,#0x81
	cmp r0,#0x81		@Are going to transfer on internal clock?
	ldreqb_ r0,gb_if		@IRQ flags
	orreq r0,r0,#8		@8=Serial
	streqb_ r0,gb_if
	mov pc,lr
@----------------------------------------------------------------------------
_FF56W:@		RP - Infrared Port
@----------------------------------------------------------------------------
	mov pc,lr
@----------------------------------------------------------------------------
_FF04W:@		DIV - Divider Register
@----------------------------------------------------------------------------
	mov r0,#0		@any write resets the reg.
	str_ r0,dividereg
	mov pc,lr
@----------------------------------------------------------------------------
_FF05W:@		TIMA - Timer counter
@----------------------------------------------------------------------------
	strb_ r0,timercounter+3
	mov pc,lr
@----------------------------------------------------------------------------
_FF06W:@		TMA - Timer Modulo
@----------------------------------------------------------------------------
	strb_ r0,timermodulo
	mov pc,lr
@----------------------------------------------------------------------------
_FF07W:@		TAC - Timer Control
@----------------------------------------------------------------------------
	strb_ r0,timerctrl
	mov pc,lr
@----------------------------------------------------------------------------
_FF0FW:
@----------------------------------------------------------------------------
	@store interrupt flags
	strb_ r0,gb_if
	b_long immediate_check_irq
@----------------------------------------------------------------------------
_FF50W:@		Undocumented BIOS banking
@----------------------------------------------------------------------------
	cmp r0,#1
	mov r0,#0
	beq map0123_
	mov pc,lr
@----------------------------------------------------------------------------
_FF01R:@		SB - Serial Transfer Data
@----------------------------------------------------------------------------
	mov r0,#0xff		@When no GB attached
	mov pc,lr
@----------------------------------------------------------------------------
_FF02R:@		SC - Serial Transfer Ctrl
@----------------------------------------------------------------------------
	ldrb_ r0,stctrl
@	mov r0,#0x00		;0x80 when transfering, 0x00 when finnished.
	mov pc,lr		@Rc Pro Am ,wants 0x80, Cosmo Tank wants 0x00.
@----------------------------------------------------------------------------
_FF56R:@		RP - Infrared Port
@----------------------------------------------------------------------------
	mov r0,#0
	mov pc,lr
@----------------------------------------------------------------------------
_FF04R:@		DIV - Divider Register
@----------------------------------------------------------------------------
	ldrb_ r0,dividereg+3
	mov pc,lr
@----------------------------------------------------------------------------
_FF05R:@		TIMA - Timer counter
@----------------------------------------------------------------------------
	ldrb_ r0,timercounter+3
	mov pc,lr
@----------------------------------------------------------------------------
_FF06R:@		TMA - Timer Modulo
@----------------------------------------------------------------------------
	ldrb_ r0,timermodulo
	mov pc,lr
@----------------------------------------------------------------------------
_FF07R:@		TAC - Timer Control
@----------------------------------------------------------------------------
	ldrb_ r0,timerctrl
	mov pc,lr
@----------------------------------------------------------------------------
_FF0FR:
@----------------------------------------------------------------------------
	ldrb_ r0,gb_if
	mov pc,lr

FF51_R:	@HDMA1
	ldrb_ r0,dma_src+1
	mov pc,lr
FF52_R:	@HDMA2
	ldrb_ r0,dma_src
	mov pc,lr
FF53_R:	@HDMA3
	ldrb_ r0,dma_dest+1
	mov pc,lr
FF54_R:	@HDMA4
	ldrb_ r0,dma_dest
	mov pc,lr
FF55_R:	@HDMA5
    ldrb_ r0,dma_blocks_remaining
    sub r0,r0,#1
	mov pc,lr


FF51_W:	@HDMA1  Src High
	strb_ r0,dma_src+1
	mov pc,lr
FF52_W:	@HDMA2  Src Low
	bic r0,r0,#0xF
	strb_ r0,dma_src
	mov pc,lr
FF53_W:	@HDMA3  Dest high
	bic r0,r0,#0xE0
	orr r0,r0,#0x80
	strb_ r0,dma_dest+1
	mov pc,lr
FF54_W:	@HDMA4  Dest Low
	bic r0,r0,#0xF
	strb_ r0,dma_dest
	mov pc,lr

FF4D_R:	@KEY1 - bit 7, Read current speed of CPU - CGB Mode Only
	ldrb_ r0,doublespeed
	bx lr
FF4D_W:	@KEY1 - prepare double speed
	ldrb_ r1,doublespeed
	bic r1,r1,#0x7F
	and r0,r0,#1
	orr r0,r0,r1
	strb_ r0,doublespeed
	bx lr

	

 .section .iwram.end.103, "ax", %progbits
	 .byte 0 @joy0state
	 .byte 0 @joy1state
	 .byte 0 @joy2state
	 .byte 0 @joy3state
	 .byte 0 @joy0serial
	 .byte 0 
	 .byte 0
	 .byte 0


	@.end
