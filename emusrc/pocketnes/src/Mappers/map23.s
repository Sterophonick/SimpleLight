 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	global_func mapper23init

 latch = mapperdata+0
 irqen = mapperdata+1
 k4irq = mapperdata+2
 counter = mapperdata+3
 k4sel = mapperdata+4
 chr_xx = mapperdata+6 @16 bytes
@----------------------------------------------------------------------------
mapper23init:	@Wai Wai World ..
@----------------------------------------------------------------------------
	.word write8000,writeA000,writeC000,writeE000

	bx lr
@-------------------------------------------------------
write8000:
@-------------------------------------------------------
	cmp addy,#0x9000
	bge write9000
	ldrb_ r1,k4sel
	ands r1,r1,#2
	beq_long map89_
	bne_long mapCD_

write9000:
	orr addy,addy,addy,lsr#4		@0x55=1, 0xAA=2
	orr addy,addy,addy,lsr#2
	ands addy,addy,#3
	beq_long mirrorKonami_
w90_:
	strb_ r0,k4sel
	mov pc,lr

@-------------------------------------------------------
writeA000:
@-------------------------------------------------------
	cmp addy,#0xb000
	bmi_long mapAB_
writeC000:	@addy=B/C/D/Exxx
@-------------------------------------------------------
	and r0,r0,#0x0f

	sub r2,addy,#0xB000
	and r2,r2,#0x3000
	tst addy,#0x55
	orrne r2,r2,#0x400
	tst addy,#0xAA
	orrne r2,r2,#0x800

	adrl_ r1,chr_xx

	strb r0,[r1,r2,lsr#10]
	bic r2,r2,#0x400
	ldrb r0,[r1,r2,lsr#10]!			@ writeback address
	ldrb r1,[r1,#1]
	orr r0,r0,r1,lsl#4

	ldr r1,=writeCHRTBL
	ldr pc,[r1,r2,lsr#9]

@-------------------------------------------------------
writeE000:
@-------------------------------------------------------
	cmp addy,#0xf000
	bmi writeC000

	orr addy,addy,addy,lsr#4		@0x55=1, 0xAA=2
	orr addy,addy,addy,lsr#2
	and addy,addy,#3
	adr r1,writeFtbl
	ldrb_ r2,latch
	ldr pc,[r1,addy,lsl#2]

writeFtbl: .word KoLatchLo,KoLatchHi,KoCounter,KoIRQen
@-------------------------------------------------------
	@.end
