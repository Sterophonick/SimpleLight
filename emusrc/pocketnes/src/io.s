#include "equates.h"

@	.include "equates.h"
@	.include "memory.h"
@	.include "ppu.h"
@	.include "sound.h"
@	.include "cart.h"
@	.include "6502.h"
@	.include "link.h"

	global_func IO_reset
	global_func IO_R
	global_func IO_W
	.global joypad_write_ptr
	global_func joy0_W
	.global _joycfg
	global_func spriteinit
	global_func suspend
	global_func refreshNESjoypads
	global_func thumbcall_r1
	.if RTCSUPPORT
	global_func gettime
	.endif
	global_func vbaprint
	global_func waitframe
@	global_func LZ77UnCompVram
	global_func CheckGBAVersion
	.global empty_io_w_hook
	.global empty_io_r_hook
	global_func breakpoint

 .align
 .pool
 .text
 .align
 .pool

breakpoint:
	mov r11,r11
	bx lr


vbaprint:
	swi 0xFF0000		@!!!!!!! Doesn't work on hardware !!!!!!!
	bx lr
@LZ77UnCompVram:
@	swi 0x120000
@	bx lr
waitframe:
VblWait:
	mov r0,#0				@don't wait if not necessary
	mov r1,#1				@VBL wait
	swi 0x040000			@ Turn of CPU until VBLIRQ if not too late allready.
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

scaleparms:@	   NH     FH     NV     FV
	.word 0x0000,0x0100,0xff00,0x0150,0xfeb6,AGB_OAM+6,AGB_OAM+518
@----------------------------------------------------------------------------
IO_reset:
@----------------------------------------------------------------------------
	adr r6,scaleparms		@set sprite scaling params
	ldmia r6,{r0-r6}

@	mov r7,#3
scaleloop:
	strh r1,[r5],#8				@buffer1, buffer2, buffer3
	strh r0,[r5],#8
	strh r0,[r5],#8
	strh r3,[r5],#232
		strh r2,[r5],#8
		strh r0,[r5],#8
		strh r0,[r5],#8
		strh r3,[r5],#232
@	subs r7,r7,#1
	bne scaleloop

	strh r1,[r6],#8				@7000200
	strh r0,[r6],#8
	strh r0,[r6],#8
	strh r4,[r6],#232
		strh r2,[r6],#8
		strh r0,[r6],#8
		strh r0,[r6],#8
		strh r4,[r6]

	@..to spriteinit
@----------------------------------------------------------------------------
spriteinit:	@build yscale_lookup tbl (called by ui.c)
@called by ui.c:  void spriteinit()
@----------------------------------------------------------------------------
	stmfd sp!,{r10,lr}
	ldr r10,=GLOBAL_PTR_BASE
	
	@add the canary
	ldr r0,=0xDEAFBEEF
	ldr r1,=IWRAM_CANARY_2
	str r0,[r1]
	
	ldrb_ r0,emuflags+1
	
	ldr r3,=YSCALE_LOOKUP
	cmp r0,#SCALED
	bhs si1

	sub r3,r3,#80
	mov r0,#-79
si3:	strb r0,[r3],#1
	add r0,r0,#1
	cmp r0,#256
	bne si3
	ldmfd sp!,{r10,lr}
	bx lr
si1:
	mov   r0,#0x00c00000		@0.75
	mov   r1,#0xf3000000		@-16*0.75
	movhi r1,#0xef000000		@-16*0.75 was 0xf5000000
@	ldrhi r1,=0xeec00000		@-16*0.75 was 0xf5000000  ;FIXME: find good value for this
si4:	mov r2,r1,lsr#24
	strb r2,[r3],#1
	add r1,r1,r0
	cmp r2,#0xb4
	bne si4
	
	ldmfd sp!,{r10,lr}
	bx lr
@----------------------------------------------------------------------------
suspend:	@called from ui.c and 6502.s
@-------------------------------------------------
	mov r3,#REG_BASE
	@stop HDMA so we don't get the crawling scanlines (and possible GBC MODE reboot) anymore
	strh r3,[r3,#REG_DM0CNT_H]		@DMA0 stop
	strh r3,[r3,#REG_DM1CNT_H]		@DMA1 stop
	strh r3,[r3,#REG_DM3CNT_H]		@DMA3 stop



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
	.if RTCSUPPORT
@----------------------------------------------------------------------------
gettime:	@called from ui.c
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

	bx lr
	.endif
@--------------------------------------------------

	global_func doReset

doReset:
	mov r1,#REG_BASE
	mov r0,#0
	strh r0,[r1,#REG_DM0CNT_H]	@stop all DMA
	strh r0,[r1,#REG_DM1CNT_H]
	strh r0,[r1,#REG_DM2CNT_H]
	strh r0,[r1,#REG_DM3CNT_H]
	add r1,r1,#0x200
	str r0,[r1,#8]		@interrupts off

	.if VISOLY

@__VISOLY_SIZE = (VISOLY_END-VISOLY_START)	@can't do this anymore
	@copy code to EWRAM and execute it	
	ldr r0,=NES_VRAM2		@temporary buffer for reset code
	ldr r1,=VISOLY_START	@source address
	ldr r2,=VISOLY_END	@end
	sub r2,r2,r1		@subtract to get size
	mov lr,r0
	b memcpy32_  @and jump to code too

@	#include "visoly.s"

	.else

	mov		r0, #0
	ldr		r1,=0x3007ffa	@must be 0 before swi 0x00 is run, otherwise it tries to start from 0x02000000.
	strh		r0,[r1]
	mov		r0, #8		@VRAM clear
	swi		0x010000
	swi		0x000000
	.endif
	
	
 .align
 .pool
 .section .iwram, "ax", %progbits
 .subsection 1
 .align
 .pool

thumbcall_r1: bx r1
@----------------------------------------------------------------------------
IO_R:		@I/O read
@----------------------------------------------------------------------------
@DO NOT TOUCH R12 (addy), it needs to be preserved for Read-Modify-Write instructions
	sub r2,addy,#0x4000
	subs r2,r2,#0x15
	bmi empty_R
	cmp r2,#3
	ldrmi pc,[pc,r2,lsl#2]
	ldr pc,empty_io_r_hook
io_read_tbl:
	.word _4015r	@4015 (sound)
	.word joy0_R	@4016: controller 1
	.word joy1_R	@4017: controller 2
empty_io_r_hook:
	.word empty_R
@----------------------------------------------------------------------------
IO_W:		@I/O write
@----------------------------------------------------------------------------
	sub r2,addy,#0x4000
	cmp r2,#0x18
	ldrmi pc,[pc,r2,lsl#2]
	ldr pc,empty_io_w_hook
io_write_tbl:
	.word _4000w
	.word _4001w
	.word _4002w
	.word _4003w
	.word _4004w
	.word _4005w
	.word _4006w
	.word _4007w
	.word _4008w
	.word void
	.word _400Aw
	.word _400Bw
	.word _400Cw
	.word void
	.word _400Ew
	.word _400Fw
	.word _4010w
	.word _4011w
	.word _4012w
	.word _4013w
	.word dma_W	@$4014: Sprite DMA transfer
	.word _4015w
joypad_write_ptr:
	.word joy0_W	@$4016: Joypad 0 write
	.word _4017w	@$4017: frame irq
empty_io_w_hook:
	.word empty_W


.pushsection .vram1, "ax", %progbits

dulr2rldu: .byte 0x00,0x80,0x40,0xc0, 0x10,0x90,0x50,0xd0, 0x20,0xa0,0x60,0xe0, 0x30,0xb0,0x70,0xf0
ssba2ssab: .byte 0x00,0x02,0x01,0x03, 0x04,0x06,0x05,0x07, 0x08,0x0a,0x09,0x0b, 0x0c,0xe0,0xd0,0x0f

@----------------------------------------------------------------------------
refreshNESjoypads:	@call every frame
@exits with Z flag clear if update incomplete (waiting for other player)
@is my multiplayer code butt-ugly?  yes, I thought so.
@i'm not trying to win any contests here.
@----------------------------------------------------------------------------
	mov r6,lr		@return with this..

@	ldr_ r0,received0
@	mov r1,#4
@	bl debug_
@	ldr_ r0,received1
@	mov r1,#5
@	bl debug_
@	ldr_ r0,received2
@	mov r1,#7
@	bl debug_
@	ldr_ r0,received3
@	mov r1,#8
@	bl debug_
@	ldr_ r0,sending
@	mov r1,#10
@	bl debug_
@	ldr_ r0,lastsent
@	mov r1,#11
@	bl debug_

		ldr_ r4,frame
		movs r0,r4,lsr#2 @C=frame&2 (autofire alternates every other frame)
	ldr_ r1,NESjoypad
	and r0,r1,#0xf0
		ldr_ r2,joycfg
		andcs r1,r1,r2
		movcss addy,r1,lsr#9	@R?
		andcs r1,r1,r2,lsr#16
	adr addy,dulr2rldu
	ldrb r0,[addy,r0,lsr#4]	@downupleftright
	and r1,r1,#0x0f			@startselectBA
	tst r2,#0x400			@Swap A/B?
	adrne addy,ssba2ssab
	ldrneb r1,[addy,r1]	@startselectBA
	orr r0,r1,r0		@r0=joypad state

	tst r2,#0x80000000
	.if LINK
	bne_long link_multi
	.endif

@	tst r2,#0x40000000	@ P3/P4
@	beq no4scr
@	tst r2,#0x20000000	@ P3/P4
@	streqb_ r0,joy2state
@	strneb_ r0,joy3state
@	ands r0,r0,#0		@Z=1
@	mov pc,r6
	
no4scr:
	tst r2,#0x20000000
	strneb_ r0,joy0state
	tst r2,#0x40000000
	strneb_ r0,joy1state
	ands r0,r0,#0		@Z=1
	mov pc,r6

@----------------------------------------------------------------------------
joy0_W:		@4016
@----------------------------------------------------------------------------
	ands r0,r0,#1
	strb_ r0,joystrobe
	bxeq lr
	ldrb_ r2,nrplayers
	cmp r2,#3
	mov r2,#-1

	ldrb_ r0,joy0state
	ldrb_ r1,joy2state
	orr r0,r0,r1,lsl#8
	orrmi r0,r0,r2,lsl#8	@for normal joypads.
	orrpl r0,r0,#0x00080000	@4player adapter
	str_ r0,joy0serial

	ldrb_ r0,joy1state
	ldrb_ r1,joy3state
	orr r0,r0,r1,lsl#8
	orrmi r0,r0,r2,lsl#8	@for normal joypads.
	orrpl r0,r0,#0x00040000	@4player adapter
	str_ r0,joy1serial
	mov pc,lr
@----------------------------------------------------------------------------
joy0_R:		@4016
@----------------------------------------------------------------------------
@DO NOT TOUCH R12 (addy), it needs to be preserved for Read-Modify-Write instructions
	ldr_ r0,joy0serial
	mov r1,r0,asr#1
	and r0,r0,#1

	ldrb_ r2,joystrobe
	movs r2,r2
	streq_ r1,joy0serial

	ldrb_ r1,cartflags
	tst r1,#VS
	orreq r0,r0,#0x40
	moveq pc,lr

	ldrb_ r1,joy0state
	tst r1,#8		@start=coin (VS)
	orrne r0,r0,#0x40

	mov pc,lr
@----------------------------------------------------------------------------
joy1_R:		@4017
@----------------------------------------------------------------------------
@DO NOT TOUCH R12 (addy), it needs to be preserved for Read-Modify-Write instructions
	ldr_ r0,joy1serial
	mov r1,r0,asr#1
	and r0,r0,#1
	
	ldrb_ r2,joystrobe
	movs r2,r2
	streq_ r1,joy1serial

	ldrb_ r1,cartflags
	tst r1,#VS
	orrne r0,r0,#0xf8	@VS dip switches
	mov pc,lr
@----------------------------
 .popsection


 .section .data.104, "w", %progbits

_sending: .word 0
_lastsent: .word 0
_received0: .word 0
_received1: .word 0
_received2: .word 0
_received3: .word 0

_joycfg: .word 0x20ff01ff @byte0=auto mask, byte1=(saves R)bit2=SwapAB, byte2=R auto mask
@bit 31=single/multi, 30,29=1P/2P, 27=(multi) link active, 24=reset signal received
_joy0state: .byte 0
_joy1state: .byte 0
_joy2state: .byte 0
_joy3state: .byte 0
_joy0serial: .word 0
_joy1serial: .word 0
_nrplayers: .byte 0		@Number of players in multilink.
_joystrobe: .byte 0
	.byte 0
	.byte 0

	@.end
