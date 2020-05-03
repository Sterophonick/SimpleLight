#include "../equates.h"

	.if LESSMAPPERS
	.else

 .align
 .pool
 .text
 .align
 .pool

#include "../6502mac.h"

	global_func mapper5init
@	global_func mapper_5_hook
	global_func mmc5_handler
	global_func mmc5_handler_2

 counter = mapperdata+0
 enable = mapperdata+1
 prgsize = mapperdata+2
 chrsize = mapperdata+3
 prgpage0 = mapperdata+4
 prgpage1 = mapperdata+5
 prgpage2 = mapperdata+6
 prgpage3 = mapperdata+7

 chrpage0 = mapperdata+8
 chrpage1 = mapperdata+9
 chrpage2 = mapperdata+10
 chrpage3 = mapperdata+11
 chrpage4 = mapperdata+12
 chrpage5 = mapperdata+13
 chrpage6 = mapperdata+14
 chrpage7 = mapperdata+15

 chrpage8 = mapperdata+16
 chrpage9 = mapperdata+17
 chrpage10 = mapperdata+18
 chrpage11 = mapperdata+19

 chrbank = mapperdata+20
 mmc5irqr = mapperdata+21
 mmc5mul1 = mapperdata+22
 mmc5mul2 = mapperdata+23
 m5mirror = mapperdata+24
@----------------------------------------------------------------------------
mapper5init:
@----------------------------------------------------------------------------
	.word void,void,void,void

	adr r1,write0
	str_ r1,writemem_4
	.if PRG_BANK_SIZE == 4
	str_ r1,writemem_5
	.endif

	adr r1,mmc5_r
	str_ r1,readmem_4
	.if PRG_BANK_SIZE == 4
	str_ r1,readmem_5
	.endif

	mov r0,#3
	strb_ r0,prgsize
	strb_ r0,chrsize

	mov r0,#0x7f
	strb_ r0,prgpage0
	strb_ r0,prgpage1
	strb_ r0,prgpage2
	strb_ r0,prgpage3

@	adr r0,mapper_5_hook
@	str_ r0,scanlinehook

	mov pc,lr
@-------------------------------------------------------
write0:
@-------------------------------------------------------
	cmp addy,#0x5000
	blo_long IO_W
	cmp addy,#0x5100
	blo map5Sound
	cmp addy,#0x5200
	bge mmc5_200

	ands r2,addy,#0xff
	beq _00
	cmp r2,#0x01
	beq _01
	cmp r2,#0x05
	beq _05
	cmp r2,#0x14
	movlt pc,lr		@ get out.
	cmp r2,#0x17
	ble _17
	cmp r2,#0x20
	movlt pc,lr		@ get out.
	cmp r2,#0x27
	ble _20
	cmp r2,#0x2b
	ble _28
	mov pc,lr		@ get out.

_00:
	and r0,r0,#0x03
	strb_ r0,prgsize
	b mmc5prg
_01:
	and r0,r0,#0x03
	strb_ r0,chrsize
	b mmc5chrb		@ both A and B?
_05:
	strb_ r0,m5mirror
	cmp r0,#0x55
	beq_long mirror5_1
	cmp r0,#0
	beq_long mirror5_1
	cmp r0,#0xE4
	beq_long mirror4_
	eor r1,r0,r0,lsr#4
	ands r1,r1,#0x0C
	b_long mirror2V_
@	b_long mirrorKonami_


mirror5_1:
	cmp r0,#0
	b_long mirror1_

_14:
_15:
_16:
_17:
	sub r2,r2,#0x14
	adrl_ r1,prgpage0
	strb r0,[r1,r2]
mmc5prg:
	ldrb_ r1,prgsize
	cmp r1,#0x00
	bne not0
	ldrb_ r0,prgpage1
	mov r0,r0,lsr#2
	b_long map89ABCDEF_
not0:
	str lr,[sp,#-4]!
	cmp r1,#0x01
	bne not1
	ldrb_ r0,prgpage1
	bl_long map89AB_rshift_
	ldrb_ r0,prgpage3
	ldr lr,[sp],#4
	b_long mapCDEF_rshift_
not1:
	cmp r1,#0x02
	bne not2
	ldrb_ r0,prgpage1
	bl_long map89AB_rshift_
	ldrb_ r0,prgpage2
	bl_long mapCD_
	ldrb_ r0,prgpage3
	ldr lr,[sp],#4
	b_long mapEF_
not2:
	ldrb_ r0,prgpage0
	bl_long map89_
	ldrb_ r0,prgpage1
	bl_long mapAB_
	ldrb_ r0,prgpage2
	bl_long mapCD_
	ldrb_ r0,prgpage3
	ldr lr,[sp],#4
	b_long mapEF_

_20:				@ For sprites.
_21:
_22:
_23:
_24:
_25:
_26:
_27:
	mov r1,#0
	strb_ r1,chrbank
	adrl_ r1,chrpage0
	sub r2,r2,#0x20
	strb r0,[r1,r2]
mmc5chra:
@	mov pc,lr		@ get out?
	ldrb_ r1,chrsize
	cmp r1,#0x00
	bne notch0
	ldrb_ r0,chrpage7
@	mov r0,r0,lsr#3
	b_long chr01234567_
notch0:
	str lr,[sp,#-4]!
	cmp r1,#0x01
	bne notch1
	ldrb_ r0,chrpage3
@	mov r0,r0,lsr#2
	bl_long chr0123_
	ldr lr,[sp],#4
	ldrb_ r0,chrpage7
@	mov r0,r0,lsr#2
@	b_long chr4567_
	mov pc,lr		@ get out?
notch1:
	cmp r1,#0x02
	bne notch2
	ldrb_ r0,chrpage1
@	mov r0,r0,lsr#1
	bl_long chr01_
	ldrb_ r0,chrpage3
@	mov r0,r0,lsr#1
	bl_long chr23_
	ldrb_ r0,chrpage5
@	mov r0,r0,lsr#1
@	bl_long chr45_
	ldr lr,[sp],#4
	ldrb_ r0,chrpage7
@	mov r0,r0,lsr#1
@	b_long chr67_
	mov pc,lr		@ get out?
notch2:
	ldrb_ r0,chrpage0
	bl_long chr0_
	ldrb_ r0,chrpage1
	bl_long chr1_
	ldrb_ r0,chrpage2
	bl_long chr2_
	ldrb_ r0,chrpage3
	bl_long chr3_
	ldrb_ r0,chrpage4
@	bl_long chr4_
	ldrb_ r0,chrpage5
@	bl_long chr5_
	ldrb_ r0,chrpage6
@	bl_long chr6_
	ldr lr,[sp],#4
	ldrb_ r0,chrpage7
@	b_long chr7_
	mov pc,lr		@ get out?

_28:				@ For background.
_29:
_2a:
_2b:
	mov r1,#1
	strb_ r1,chrbank
	adrl_ r1,chrpage0
	sub r2,r2,#0x20
	strb r0,[r1,r2]
mmc5chrb:
@	mov pc,lr		@ get out?
	ldrb_ r1,chrsize
	cmp r1,#0x00
	bne notchb0
	ldrb_ r0,chrpage11
	mov r0,r0,lsr#3
	b_long chr01234567_
notchb0:
	str lr,[sp,#-4]!
	cmp r1,#0x01
	bne notchb1
	ldrb_ r0,chrpage11
@	mov r0,r0,lsr#2
@	bl_long chr0123_
	ldr lr,[sp],#4
	ldrb_ r0,chrpage11
	mov r0,r0,lsr#2
	b_long chr4567_
	mov pc,lr
notchb1:
	cmp r1,#0x02
	bne notchb2
@@	ldrb_ r0,chrpage9
@	mov r0,r0,lsr#1
@	bl_long chr01_
@@	ldrb_ r0,chrpage11
@	mov r0,r0,lsr#1
@	bl_long chr23_
	ldrb_ r0,chrpage9
	bl_long chr45_rshift_
	ldr lr,[sp],#4
	ldrb_ r0,chrpage11
	b_long chr67_rshift_
	mov pc,lr
notchb2:
	ldrb_ r0,chrpage8
@	bl_long chr0_
	ldrb_ r0,chrpage9
@	bl_long chr1_
	ldrb_ r0,chrpage10
@	bl_long chr2_
	ldrb_ r0,chrpage11
@	bl_long chr3_
	ldrb_ r0,chrpage8
	bl_long chr4_
	ldrb_ r0,chrpage9
	bl_long chr5_
	ldrb_ r0,chrpage10
	bl_long chr6_
	ldr lr,[sp],#4
	ldrb_ r0,chrpage11
	b_long chr7_

map5Sound:
	mov pc,lr
@-------------------------------------------------------
mmc5_200:
	and r2,addy,#0xff
	cmp r2,#0x03
	beq setCounter

	cmp r2,#0x04
	beq setEnIrq

	cmp r2,#0x05
	streqb_ r0,mmc5mul1
	moveq pc,lr

	cmp r2,#0x06
	streqb_ r0,mmc5mul2
	mov pc,lr

setEnIrq:
	ands r0,r0,#0x80
	strb_ r0,enable
	
	ldreqb_ r0,wantirq
	biceq r0,r0,#IRQ_MAPPER
	streqb_ r0,wantirq
	
	b find_mmc5_irq
setCounter:
	strb_ r0,counter
	b find_mmc5_irq

@-------------------------------------------------------
mmc5_r:		@5204,5205,5206
	cmp addy,#0x5200
	blo_long IO_R
	and r2,addy,#0xff
	cmp r2,#0x04
	beq MMC5IRQR
	cmp r2,#0x05
	beq MMC5MulA
	cmp r2,#0x06
	beq MMC5MulB

	mov r0,#0xff
	mov pc,lr

MMC5IRQR:
	@Acknowledge IRQ
	ldrb_ r0,wantirq
	tst r0,#IRQ_MAPPER
	bic r0,r0,#IRQ_MAPPER
	strb_ r0,wantirq
	
	mov r0,#0
	orrne r0,r0,#0x80
	@return 0x80 if we acknowledged an IRQ
	
	@Return 0x40 if PPU is rendering
	ldr_ r1,timestamp
	ldr_ r2,cycles_to_run
	sub r2,r2,cycles,asr#CYC_SHIFT
	add r1,r1,r2
	ldr_ r2,frame_timestamp
	sub r1,r1,r2
	
	ldr_ addy,timestamp_div
	umull r2,addy,r1,addy
	cmp r2,#0xC0000000
	orrlo r0,r0,#0x40
	cmp addy,#241
	bichi r0,r0,#0x40

	mov pc,lr

MMC5MulA:
	ldrb_ r1,mmc5mul1
	ldrb_ r2,mmc5mul1
	mul r0,r1,r2
	and r0,r0,#0xff
	mov pc,lr
MMC5MulB:
	ldrb_ r1,mmc5mul1
	ldrb_ r2,mmc5mul1
	mul r0,r1,r2
	mov r0,r0,lsr#8
	mov pc,lr

mmc5_handler_2: @disable IRQ automatically if it reaches the next scanline after an IRQ
	ldrb_ r0,wantirq
	bic r0,r0,#IRQ_MAPPER
	strb_ r0,wantirq
0:	
	bl find_mmc5_irq
	b_long _GO
	
mmc5_handler:
	ldrb_ r0,enable
	tst r0,#0x80
	beq_long _GO
	
	ldrb_ r0,screen_off
	movs r0,r0
	bne 0b
	
	ldr_ r1,mapper_timestamp
	add r1,r1,#12
	ldr r0,=mapper_irq_handler
	adrl_ r12,mapper_irq_timeout
	bl_long replace_timeout_2
	
	ldr_ r1,mapper_timestamp
	ldr_ r0,timestamp_mult
	add r1,r1,r0,lsr#4
	adr r0,mmc5_handler_2
	adrl_ r12,mapper_timeout
	bl_long replace_timeout_2
	b_long _GO

	
find_mmc5_irq:
	ldrb_ r0,enable
	tst r0,#0x80
	bxeq lr
	ldrb_ r0,counter
	movs r0,r0
	bxeq lr
	cmp r0,#0xF0
	bxge lr
	
	add r0,r0,#1
	
	ldr_ r1,timestamp_mult
	mul r1,r0,r1
	ldr_ r0,frame_timestamp
	add r1,r0,r1,lsr#4
	sub r1,r1,#12
	
	@if target timestamp is BEFORE now, add xxxxx to the target timestamp
	ldr_ r0,cycles_to_run
	sub r0,r0,cycles,asr#CYC_SHIFT
	ldr_ r2,timestamp
	add r2,r2,r0
	
	@target timestamp BEFORE now?
	cmp r1,r2
	ldrmi_ r0,cyclesperframe
	addmi r1,r1,r0
	
	adr r0,mmc5_handler
	adrl_ r12,mapper_timeout
	b_long replace_timeout_2

@@-------------------------------------------------------
@mapper_5_hook:
@@------------------------------------------------------
@	ldrb_ r0,counter
@	ldr_ r1,scanline
@	ldrb_ r2,mmc5irqr
@	cmp r1,#239
@	blt h2
@	orr r2,r2,#0x40
@h2:
@	cmp r1,#245
@	bge h1
@
@	cmp r1,r0
@	ble h1
@
@	orr r2,r2,#0x80
@	strb_ r2,mmc5irqr
@
@	ldrb_ r0,enable
@	cmp r0,#0
@@	bne irq6502
@	bne_long CheckI
@h1:
@	strb_ r2,mmc5irqr
@	fetch 0

@-------------------------------------------------------

	.endif
	
	@.end
