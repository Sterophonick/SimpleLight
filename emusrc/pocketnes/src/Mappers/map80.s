 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"
#include "../6502mac.h"

	global_func mapper80init

 patch = mapperdata+0
@----------------------------------------------------------------------------
mapper80init:	@Taito
@----------------------------------------------------------------------------
	.word void,void,void,void

	ldrb_ r1,cartflags		@get cartflags
	bic r1,r1,#SRAM			@don't use SRAM on this mapper
	strb_ r1,cartflags		@set cartflags

	adr r0,write80
	str_ r0,writemem_6
	.if PRG_BANK_SIZE == 4
	str_ r0,writemem_7
	.endif
	ldr_ r0,rommask
	tst r0,#0x20000
	movne r0,#1
	moveq r0,#0
	str_ r0,patch

	mov pc,lr
@-------------------------------------------------------
write80:
@-------------------------------------------------------
	mov r1,#0x7F0
	sub r1,r1,#1
	teq r1,addy,lsr#4
	movne pc,lr

	and addy,addy,#0xF
	adr r1,write80tbl
	ldr pc,[r1,addy,lsl#2]

wF0:
	mov addy,r0
	stmfd sp!,{r0,lr}
	bl_long chr0_
	ldr_ r1,patch
	cmp r1,#0
	beq noPatch
	tst addy,#0x80
	bl_long mirror1_
noPatch:
	ldmfd sp!,{r0,lr}
	add r0,r0,#1
	b_long chr1_
wF1:
	stmfd sp!,{r0,lr}
	bl_long chr2_
	ldmfd sp!,{r0,lr}
	add r0,r0,#1
	b_long chr3_
wF6:
	ands r0,r0,#1
	b_long mirror2H_


write80tbl: .word wF0,wF1,chr4_,chr5_,chr6_,chr7_,wF6,void,void,void,map89_,map89_,mapAB_,mapAB_,mapCD_,mapCD_
@-------------------------------------------------------
	@.end
