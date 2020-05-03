#include "../equates.h"

	.if LESSMAPPERS
	.else

 .align
 .pool
 .text
 .align
 .pool


	global_func mapper228init

 mapbyte1 = mapperdata
@----------------------------------------------------------------------------
mapper228init:@		Action 52 & Cheetahmen 2. PocketNES only support 256k of CHR, Action52 got 512k.
@----------------------------------------------------------------------------
	.word write0,write0,write0,write0

	ldr_ r0,rommask
	cmp r0,#0x40000
	addhi r0,r0,#0x80000
	str_ r0,rommask		@rommask=romsize-1

	mov r0,#0
	b_long map89ABCDEF_
@-------------------------------------------------------
write0:
@-------------------------------------------------------
	str_ addy,mapbyte1
	and r0,r0,#0x03
	orr r0,r0,addy,lsl#2
	mov addy,lr

	bl_long chr01234567_

	ldr_ r0,mapbyte1
	tst r0,#0x2000
	bl_long mirror2V_

	ldr_ r0,mapbyte1
	tst r0,#0x1000
	bicne r0,r0,#0x800
	
	tst r0,#0x40
	bne swap16k
	mov r0,r0,lsr#7
	mov lr,addy
	b_long map89ABCDEF_

swap16k:
	and r1,r0,r0,lsl#1
	mov r1,r1,lsr#6
	orr r1,r1,#0xFE
	and r0,r1,r0,lsr#6
	str_ r0,mapbyte1
	bl_long mapCDEF_
	ldr_ r0,mapbyte1
	mov lr,addy
	b_long map89AB_
@-------------------------------------------------------
	.endif
	@.end
