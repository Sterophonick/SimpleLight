 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"

	global_func mapper32init

 pswitch = mapperdata+0
@----------------------------------------------------------------------------
mapper32init:	@Irem... Image Fight (J)
@----------------------------------------------------------------------------
	.word write8000,writeA000,void,void

	mov pc,lr
@-------------------------------------------------------
write8000:
@-------------------------------------------------------
	tst addy,#0x1000
	bne write9000
	ldr_ r1,pswitch
	tst r1,#0x02
	beq_long map89_
	bne_long mapCD_
write9000:
	str_ r0,pswitch
	tst r0,#0x1
	b_long mirror2V_

@-------------------------------------------------------
writeA000:
@-------------------------------------------------------
	tst addy,#0x1000
	beq_long mapAB_
	and addy,addy,#7
	ldr r1,=writeCHRTBL
	ldr pc,[r1,addy,lsl#2]

@-------------------------------------------------------
	@.end
