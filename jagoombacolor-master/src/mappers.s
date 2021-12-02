 .section .iwram.2, "ax", %progbits

@	#include "equates.h"
@	#include "memory.h"
@	#include "lcd.h"
@	#include "cart.h"
@	#include "io.h"

 .if RUMBLE
	@IMPORT DoRumble
 .endif
	global_func mbc0init
	global_func mbc1init
	global_func mbc2init
	global_func mbc3init
	global_func mbc4init
	global_func mbc5init
	global_func mbc6init
	global_func mbc7init
	global_func mmm01init
	global_func huc1init
	global_func huc3init
	global_func RamSelect
@----------------------------------------------------------------------------
RamSelect:
@----------------------------------------------------------------------------
	ldrb_ r0,mapperdata+2	@ram enable
@----------------------------------------------------------------------------
RamEnable:
@----------------------------------------------------------------------------
	strb_ r0,mapperdata+2
	and r0,r0,#0x0F
	cmp r0,#0x0A
	adrnel r1,empty_W
	ldreq_ r1,sramwptr
	str_ r1,writemem_tbl+40
	str_ r1,writemem_tbl+44
	adrnel r1,empty_R
	adreql r1,mem_RA0
	str_ r1,readmem_tbl_-40
	str_ r1,readmem_tbl_-44
	ldrb_ r0,mapperdata+4		@rambank
	b mapAB_

	.pushsection .text
@----------------------------------------------------------------------------
mbc0init:
@----------------------------------------------------------------------------
	.word void,void,void,void
	mov pc,lr

@mapperdata for mbc1:
@0	low 5 bits of rom bank number (00-1F), 00 becomes 01.
@1	high 2 bits of rom bank number
@2	sram enabled (0A if enabled)
@3	rom/ram bankswitch mode (0 for rom, 1 for ram)
@4	sram bank
@5	2 bits for either rom bank or sram bank

@----------------------------------------------------------------------------
mbc1init:
@----------------------------------------------------------------------------
	.word RamEnable,MBC1map0,MBC1map1,MBC1mode
	mov pc,lr
	
	.popsection
@----------------------------------------------------------------------------
MBC1map0:
@----------------------------------------------------------------------------
	ands r0,r0,#0x1f
	moveq r0,#1
	strb_ r0,mapperdata
	ldrb_ r1,mapperdata+1
	orr r0,r0,r1,lsl#5
	b map4567_
	
	.pushsection .text
@----------------------------------------------------------------------------
MBC1map1:
@----------------------------------------------------------------------------
	and r0,r0,#0x03
	strb_ r0,mapperdata+5		@Ram/Rom bank select.
	ldrb_ r0,mapperdata+3
@----------------------------------------------------------------------------
MBC1mode:
@----------------------------------------------------------------------------
	strb_ r0,mapperdata+3
	tst r0,#1
	ldrb_ r0,mapperdata+5
	mov r1,#0
	streqb_ r0,mapperdata+1		@16Mbit Rom
	strneb_ r1,mapperdata+1		@4Mbit Rom
	streqb_ r1,mapperdata+4		@8kByte Ram
	strneb_ r0,mapperdata+4		@32kbyte Ram

	ldrb_ r0,mapperdata
	ldrb_ r1,mapperdata+1
	orr r0,r0,r1,lsl#5
	str lr,[sp,#-4]!
	bl map4567_
	ldr lr,[sp],#4
	b RamSelect

@----------------------------------------------------------------------------
mbc2init:
@----------------------------------------------------------------------------
	.word MBC2RamEnable,MBC2map,void,void
	mov pc,lr
	.popsection
@----------------------------------------------------------------------------
MBC2map:
@----------------------------------------------------------------------------
	tst addy,#0x0100
	moveq pc,lr
	ands r0,r0,#0xf
	moveq r0,#1
	b map4567_
	.pushsection .text
@----------------------------------------------------------------------------
MBC2RamEnable:
	tst addy,#0x0100
	beq_long RamEnable
	mov pc,lr

@----------------------------------------------------------------------------
mbc3init:
@----------------------------------------------------------------------------
	.word RamEnable,map4567_,mbc3bank,mbc3latchtime
	mov pc,lr
@----------------------------------------------------------------------------
mbc3latchtime:
@----------------------------------------------------------------------------
	ldrb_ r1,mapperdata+3
	strb_ r0,mapperdata+3
	eor r1,r1,r0
	and r1,r1,r0
	cmp r1,#1
	movne pc,lr
	stmfd sp!,{r3,lr}
	bl_long gettime
	ldmfd sp!,{r3,lr}
	mov pc,lr
@----------------------------------------------------------------------------
mbc3bank:
@----------------------------------------------------------------------------
	strb_ r0,mapperdata+4
	tst r0,#8
	beq RamSelect
	ldr r1,=empty_W
	str_ r1,writemem_tbl+40
	str_ r1,writemem_tbl+44
	ldr r1,=empty_R
	cmp r0,#0x8
	adreq r1,clk_sec
	cmp r0,#0x9
	adreq r1,clk_min
	cmp r0,#0xA
	adreq r1,clk_hrs
	cmp r0,#0xB
	adreq r1,clk_dayL
	cmp r0,#0xC
	adreq r1,clk_dayH
	str_ r1,readmem_tbl_-40
	str_ r1,readmem_tbl_-44
	mov pc,lr

@------------------------------
clk_sec:
	ldrb_ r0,mapperdata+30
	b calctime
@------------------------------
clk_min:
	ldrb_ r0,mapperdata+29
	b calctime
@------------------------------
clk_hrs:
	ldrb_ r0,mapperdata+28
	and r0,r0,#0x3F
	b calctime
@------------------------------
clk_dayL:
	ldrb_ r0,mapperdata+26
	b calctime
clk_dayH:
	mov r0,#0
calctime:
	and r1,r0,#0xf
	mov r0,r0,lsr#4
	add r0,r0,r0,lsl#2
	add r0,r1,r0,lsl#1 
	mov pc,lr
@------------------------------
	

@----------------------------------------------------------------------------
mbc5init:
@----------------------------------------------------------------------------
	.word RamEnable,MBC5map0,MBC5RAMB,void
	mov pc,lr
	.popsection
@----------------------------------------------------------------------------
MBC5map0:
@----------------------------------------------------------------------------
	tst addy,#0x1000
	andne r0,r0,#0x01
	strneb_ r0,mapperdata+1
	streqb_ r0,mapperdata
	ldr_ r0,mapperdata
	b map4567_
@----------------------------------------------------------------------------
MBC5RAMB:
@----------------------------------------------------------------------------
	strb_ r0,mapperdata+4
@	tst r0,#0x08		;rumble motor.
	and r0,r0,#0x8
 .if RUMBLE
	ldr r1,=DoRumble
	str r0,[r1]
 .endif
	b RamSelect

	.pushsection .text
@----------------------------------------------------------------------------
mbc7init:
@----------------------------------------------------------------------------
	.word void,MBC7map,MBC7RAMB,void
	mov pc,lr
	.popsection
@----------------------------------------------------------------------------
MBC7map:
@----------------------------------------------------------------------------
	ands r0,r0,#0x7f
	moveq r0,#1
	strb_ r0,mapperdata
	b map4567_
	.pushsection .text
@----------------------------------------------------------------------------
MBC7RAMB:
@----------------------------------------------------------------------------
	strb_ r0,mapperdata+4
	cmp r0,#9
	movmi r0,#0xA
	movpl r0,#0
	strb_ r0,mapperdata+2
	b RamSelect

@----------------------------------------------------------------------------
huc1init:
@----------------------------------------------------------------------------
	.word RamEnable,HUC1map0,MBC1map1,MBC1mode
@	DCD RamEnable,HUC1map0,MBC5RAMB,void
	mov pc,lr
	.popsection
@----------------------------------------------------------------------------
HUC1map0:
@----------------------------------------------------------------------------
	ands r0,r0,#0x3f
	moveq r0,#1
	strb_ r0,mapperdata
@	ldrb r1,mapperdata+1
@	orr r0,r0,r1,lsl#5
	b map4567_

	.pushsection .text
@----------------------------------------------------------------------------
huc3init:
@----------------------------------------------------------------------------
	.word RamEnable,map4567_,MBC5RAMB,void
	mov pc,lr

@----------------------------------------------------------------------------
mmm01init:
mbc4init:
mbc6init:
@----------------------------------------------------------------------------
	.word RamEnable,map4567_,void,void
	mov pc,lr
	.popsection

@----------------------------------------------------------------------------
@----------------------------------------------------------------------------
	@.end
