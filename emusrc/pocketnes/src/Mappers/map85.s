 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	global_func mapper85init

 latch = mapperdata+0
 irqen = mapperdata+1
 k4irq = mapperdata+2
 counter = mapperdata+3
@----------------------------------------------------------------------------
mapper85init:	@Konami - Tiny Toon Adventure 2 (J)...
		@ Lagrange Point, requires CHRRAM swappability  =)
@----------------------------------------------------------------------------
	.word write85,write85,write85,write85
VRC7:
	bx lr
@-------------------------------------------------------
write85:
@-------------------------------------------------------
	mov r1,addy,lsr#11
	and r1,r1,#0xE
	tst addy,#0x8
	orrne r1,r1,#1
	tst addy,#0x10
	orrne r1,r1,#1

	adr addy,tbl85
	ldr pc,[addy,r1,lsl#2]
	
tbl85:	.word map89_,mapAB_,mapCD_,VRC7,chr0_,chr1_,chr2_,chr3_,chr4_,chr5_,chr6_,chr7_,mirrorKonami_,KoLatch,KoCounter,KoIRQen
@-------------------------------------------------------
	@.end
