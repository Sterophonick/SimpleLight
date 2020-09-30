@ .align
@ .pool
@ .section .iwram, "ax", %progbits
@ .subsection 3
@ .align
@ .pool
 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	global_func mapper21init
	global_func mapper25init

 latch = mapperdata+0
 irqen = mapperdata+1
 k4irq = mapperdata+2
 counter = mapperdata+3
 k4sel = mapperdata+4
 k4map1 = mapperdata+5
 chr_xx = mapperdata+8 @16 bytes
@----------------------------------------------------------------------------
mapper21init:	@gradius 2, wai wai world 2..
mapper25init:
@----------------------------------------------------------------------------
	.word write8000,writeA000,writeC000,writeE000

	bx lr
@-------------------------------------------------------
write8000:
@-------------------------------------------------------
	tst addy,#0x1000
	bne write9000
	strb_ r0,k4map1
	b romswitch

write9000:
	orr addy,addy,addy,lsr#2
	ands addy,addy,#3
	beq_long mirrorKonami_
	cmp addy,#1
	movne pc,lr
w91:
	strb_ r0,k4sel
romswitch:
	mov addy,lr
	ldrb_ r0,k4sel
	tst r0,#2
	mov r0,#-2
	bne reverseMap
	bl_long mapCD_
	mov lr,addy
	ldrb_ r0,k4map1
	b_long map89_
reverseMap:
	bl_long map89_
	mov lr,addy
	ldrb_ r0,k4map1
	b_long mapCD_

@-------------------------------------------------------
writeA000:
@-------------------------------------------------------
	tst addy,#0x1000
	beq_long mapAB_
writeC000:	@addy=B/C/D/Exxx
@-------------------------------------------------------
	sub r2,addy,#0xB000
	and r2,r2,#0x3000
	tst addy,#0x85			@0x01 + 0x04 + 0x80
	orrne r2,r2,#0x800
	tst addy,#0x4A			@0x02 + 0x08 + 0x40
	orrne r2,r2,#0x4000

	adrl_ r1,chr_xx
	and r0,r0,#0x0f

	strb r0,[r1,r2,lsr#11]
	bic r2,r2,#0x4000
	ldrb r0,[r1,r2,lsr#11]!
	ldrb r1,[r1,#8]
	orr r0,r0,r1,lsl#4

	ldr r1,=writeCHRTBL
	ldr pc,[r1,r2,lsr#9]

@-------------------------------------------------------
writeE000:
@-------------------------------------------------------
	cmp addy,#0xf000
	bmi writeC000

	tst addy,#0x85			@0x04 + 0x01 + 0x80
	orrne addy,addy,#0x1
	tst addy,#0x4A			@0x02 + 0x08 + 0x40
	orrne addy,addy,#0x2
	and addy,addy,#3
	adr r1,writeFtbl
	ldrb_ r2,latch
	ldr pc,[r1,addy,lsl#2]

writeFtbl: .word KoLatchLo,KoCounter,KoLatchHi,KoIRQen
@-------------------------------------------------------
	@.end
