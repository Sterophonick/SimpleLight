 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	global_func mapper9init
	global_func mapper10init
	global_func mapper9BGcheck
@	global_func mapper_9_hook

 reg0 = mapperdata+0
 reg1 = mapperdata+1
 reg2 = mapperdata+2
 reg3 = mapperdata+3
@----------------------------------------------------------------------------
mapper9init:	@really bad Punchout hack
@----------------------------------------------------------------------------
	.word empty_W,a000_9,c000,e000
map10start:
	ldrb_ r0,cartflags
	bic r0,r0,#SCREEN4	@(many punchout roms have bad headers)
	strb_ r0,cartflags

@	ldr r0,=mapper_9_hook
@	str_ r0,scanlinehook

	mov r0,#-1
	b_long map89ABCDEF_		@everything to last bank
@----------------------------------------------------------------------------
mapper10init:
@----------------------------------------------------------------------------
	.word empty_W,a000_10,c000,e000
	b_long map10start
@----------------------------------------------------------------------------
@ .align
@ .pool
@ .section .iwram, "ax", %progbits
@ .subsection 7
@ .align
@ .pool
@----------------------------------------------------------------------------
@------------------------------
a000_10:
	tst addy,#0x1000
	beq_long map89AB_
	b b000
@------------------------------
a000_9:
	tst addy,#0x1000
	beq_long map89_
b000: @-------------------------
	strb_ r0,reg0
	mov pc,lr
c000: @-------------------------
	tst addy,#0x1000
	bne d000

	strb_ r0,reg1
	b_long chr0123_
	@mov pc,lr
d000: @-------------------------
	strb_ r0,reg2
	mov pc,lr
e000: @-------------------------
	tst addy,#0x1000
	bne f000

	strb_ r0,reg3
	mov pc,lr
f000: @-------------------------
	tst r0,#1
	b_long mirror2V_
@@------------------------------
@mapper_9_hook:
@@------------------------------
@	ldr_ r0,scanline
@	sub r0,r0,#1
@	tst r0,#7
@	ble h9
@	cmp r0,#239
@	bhi h9
@
@	ldr r2,=latchtbl
@	ldrb r0,[r2,r0,lsr#3]
@
@	cmp r0,#0xfd
@	ldreqb_ r0,reg2
@	ldrneb_ r0,reg3
@	bl_long chr4567_
@h9:
@	fetch 0

@------------------------------
mapper9BGcheck: @called from PPU.s, r0=FD-FF
@------------------------------
	cmp r0,#0xff
	moveq pc,lr

	ldr r1,=latchtbl
	and r2,addy,#0x3f
	cmp r2,#0x10
	strlob r0,[r1,addy,lsr#6]

	mov pc,lr
 .align
 .pool
 .section .iwram, "ax", %progbits
 .subsection 99
 .align
 .pool

latchtbl: .skip 32
@----------------------------------------------------------------------------
	@.end
