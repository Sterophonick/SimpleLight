 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"

	global_func mapper68init
@----------------------------------------------
mapper68init:	@Sunsoft, After Burner...
@----------------------------------------------
	.word write0,write1,write2,write3

	mov pc,lr
@----------------------------------------------
write0:
@----------------------------------------------
	tst addy,#0x1000
	bne_long chr23_
	b_long chr01_
@----------------------------------------------
write1:
@----------------------------------------------
	tst addy,#0x1000
	bne_long chr67_
	b_long chr45_
@----------------------------------------------
write2:
@----------------------------------------------
	b_long empty_W
@	tst addy,#0x1000
@	bne_long chrAB_
@	b_long chr89_
@----------------------------------------------
write3:
@----------------------------------------------
	tst addy,#0x1000
	bne_long map89AB_

	tst r0,#0x10
	bne setNTmanualy
	b_long mirrorKonami_
setNTmanualy:
	b_long empty_W		@fix this some day
@----------------------------------------------
	@.end
