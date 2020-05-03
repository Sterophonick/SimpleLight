 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"

	global_func mapper88init

 cmd = mapperdata
@----------------------------------------------------------------------------
mapper88init:
@----------------------------------------------------------------------------
	.word write0,void,write1,void

	mov pc,lr
@----------------------------------------------------------------------------
write0:		@$8000-8001
@----------------------------------------------------------------------------
	tst addy,#1

	streqb_ r0,cmd
	moveq pc,lr
w8001:
	ldrb_ r1,cmd
	and r1,r1,#7
	adr r2,commandlist
	ldr pc,[r2,r1,lsl#2]!
commandlist:	.word cmd0,cmd1,cmd2,cmd3,cmd4,cmd5,map89_,mapAB_
commandlist2: .word chr01_rshift_,chr23_rshift_,chr4_,chr5_,chr6_,chr7_
cmd0:
cmd1:
	and r0,r0,#0x3F
	ldr pc,[r2,#32]
cmd2:
cmd3:
cmd4:
cmd5:
cmd6:
cmd7:
	orr r0,r0,#0x40
	ldr pc,[r2,#32]


@----------------------------------------------------------------------------
write1:		@$C000
@----------------------------------------------------------------------------
	tst r0,#0
	b_long mirror1_

@----------------------------------------------------------------------------
	@.end
