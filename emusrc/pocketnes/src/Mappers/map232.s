 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	global_func mapper232init

 mapbyte1 = mapperdata+0
 mapbyte2 = mapperdata+1
@----------------------------------------------------------------------------
mapper232init:
@----------------------------------------------------------------------------
	.word w9000,write0,write0,write0

	mov r0,#0x18
	strb_ r0,mapbyte1

@	b_long mirror1_
	mov pc,lr
@-------------------------------------------------------
w9000:
@-------------------------------------------------------
	and r0,r0,#0x18
	strb_ r0,mapbyte1
	b prgmap
@-------------------------------------------------------
write0:
@-------------------------------------------------------
	and r0,r0,#0x03
	strb_ r0,mapbyte2
	ldrb_ r0,mapbyte1
prgmap:
	mov addy,lr
	mov r1,#3
	orr r0,r1,r0,lsr#1
	bl_long mapCDEF_

	ldrb_ r0,mapbyte1
	ldrb_ r1,mapbyte2
	orr r0,r1,r0,lsr#1
	mov lr,addy
	b_long map89AB_
@-------------------------------------------------------
	@.end
