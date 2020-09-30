 .align
 .pool
 .text
 .align
 .pool

	@IMPORT run_mmc3
	@IMPORT mmc3_set_next_timeout
	@IMPORT mmc3_ntsc_pal_reset
	
#include "../equates.h"
#include "../6502mac.h"

	global_func mapper64init

	@does not include CPUcycle/4 IRQ mode, only MMC3 scanline mode

 countdown = mapperdata+0
 latch = mapperdata+1
 irqen = mapperdata+2
 irq_time_add = mapperdata+3
 cmd = mapperdata+4
 prg0 = mapperdata + 5
 prg1 = mapperdata + 6
 prg2 = mapperdata + 7
 chr0 = mapperdata + 8
 chr1 = mapperdata + 9
 chr2 = mapperdata + 10
 chr3 = mapperdata + 11
 chr4 = mapperdata + 12
 chr5 = mapperdata + 13
 chr6 = mapperdata + 14
 chr7 = mapperdata + 15

 mmc3_spr_subtract = mapperdata + 16
 mmc3_bg_subtract = mapperdata + 20
 last_mmc3_timestamp = mapperdata + 24
 mmc3_scanline_divide = mapperdata + 28

enter_mmc3_mode:
	ldr r0,=run_mmc3
	str_ r0,screen_off_hook1
	str_ r0,screen_on_hook1
	ldr r0,=mmc3_set_next_timeout
	str_ r0,screen_off_hook2
	str_ r0,screen_on_hook2
	
	@get timestamp
	ldr_ r1,cycles_to_run
	sub r1,r1,cycles,asr#CYC_SHIFT
	ldr_ r2,timestamp
	add r2,r2,r1
	str_ r2,last_mmc3_timestamp
	
	@maybe do more, like initial clocking for A12?
	
	bx lr
	
exit_mmc3_mode:
	mov r0,#0
	str_ r0,screen_off_hook1
	str_ r0,screen_off_hook2
	str_ r0,screen_on_hook1
	str_ r0,screen_on_hook2
	
	b run_mmc3	@@


@----------------------------------------------------------------------------
mapper64init:
@----------------------------------------------------------------------------
	.word write0,write1,write2,write3

	mov r0,#18
	strb_ r0,irq_time_add
	
	ldr r0,=mmc3_ntsc_pal_reset
	str_ r0,ntsc_pal_reset_hook
	
@	ldr r0,=RAMBO_IRQ_Hook
@	str_ r0,scanlinehook
	
	b enter_mmc3_mode

	mov pc,lr
@----------------------------------------------------------------------------
write0:		@$8000-8001
@----------------------------------------------------------------------------
	ldrb_ r1,cmd
	tst addy,#1
	and r2,r1,#0x0F
	mov r1,r1,lsr#5
	orr addy,r1,r1,lsl#3
	and addy,addy,#0x0C
write0_entry:
	ldrne pc,[pc,r2,lsl#2]
	b w8000	
@----------------------------------------------------------------------------
commandlist:	.word cmd0,cmd1,cmd2,cmd3,cmd4,cmd5,cmd6,cmd7
			.word cmd8,cmd9,void,void,void,void,void,cmdF
@----------------------------------------------------------------------------
w8000:
	strb_ r0,cmd
	eors r2,r1,r0,lsr#5
	bxeq lr
newmode:
	@bits of r2 tell you which modes have changed
	@re-obtain r1 and r12 based on new values
	mov r1,r0,lsr#5
	orr addy,r1,r1,lsl#3
	and addy,addy,#0x0C
	
	stmfd sp!,{lr}
	tst r2,#0x5
	@C or K has changed
	blne newchrmode
	tst r2,#2
	blne newprgmode
	ldmfd sp!,{pc}
newchrmode:
	mov r2,#0
	stmfd sp!,{r2,lr}
0:
	stmfd sp!,{r1,r2,addy}
	adrl_ r0,chr0
	ldrb r0,[r0,r2]
	bl write0_entry
	ldmfd sp!,{r1,r2,addy}
	add r2,r2,#1
	cmp r2,#6
	addeq r2,r2,#2
	cmp r2,#10
	bne 0b
	ldmfd sp!,{r2,pc}
newprgmode:
	stmfd sp!,{r1,addy,lr}
	movs r2,#6
	ldrb_ r0,prg0
	bl write0_entry
	ldmfd sp!,{r1,addy,lr}
	stmfd sp!,{r1,addy,lr}
	movs r2,#7
	ldrb_ r0,prg1
	bl write0_entry
	ldmfd sp!,{r1,addy,lr}
	movs r2,#0x0F
	ldrb_ r0,prg2
	b write0_entry
	
	
@  $8000:  [CPK. AAAA]   <---  C and K bits relevent to CHR
@addy = KC00, r1=CPK
@                  0       1       2       3       4       5       6       7 
@            +---------------+---------------+-------+-------+-------+-------+
@K=0, C=0    |     <R:0>     |     <R:1>     |  R:2  |  R:3  |  R:4  |  R:5  |
@            +---------------+---------------+-------+-------+-------+-------+
@K=0, C=1    |  R:2  |  R:3  |  R:4  |  R:5  |     <R:0>     |     <R:1>     |
@            +-------+-------+-------+-------+---------------+---------------+
@K=1, C=0    |  R:0  |  R:8  |  R:1  |  R:9  |  R:2  |  R:3  |  R:4  |  R:5  |
@            +-------+-------+-------+-------+---------------+---------------+
@K=1, C=1    |  R:2  |  R:3  |  R:4  |  R:5  |  R:0  |  R:8  |  R:1  |  R:9  |
@            +-------+-------+-------+-------+-------+-------+-------+-------+

cmd0:
	strb_ r0,chr0
	ldr pc,[pc,addy]
	.word 0
	.word chr01_rshift_,chr45_rshift_,chr0_,chr4_
cmd1:
	strb_ r0,chr1
	ldr pc,[pc,addy]
	.word 0
	.word chr23_rshift_,chr67_rshift_,chr2_,chr6_
cmd2:
	strb_ r0,chr2
	ldr pc,[pc,addy]
	.word 0
	.word chr4_,chr0_,chr4_,chr0_
cmd3:
	strb_ r0,chr3
	ldr pc,[pc,addy]
	.word 0
	.word chr5_,chr1_,chr5_,chr1_
cmd4:
	strb_ r0,chr4
	ldr pc,[pc,addy]
	.word 0
	.word chr6_,chr2_,chr6_,chr2_
cmd5:  
	strb_ r0,chr5
	ldr pc,[pc,addy]
	.word 0
	.word chr7_,chr3_,chr7_,chr3_
cmd8:
	strb_ r0,chr6
	ldr pc,[pc,addy]
	.word 0
	.word void,void,chr1_,chr5_
cmd9:
	strb_ r0,chr7
	ldr pc,[pc,addy]
	.word 0
	.word void,void,chr3_,chr7_
@               $8000   $A000   $C000   $E000  
@             +-------+-------+-------+-------+
@PRG Mode 0:  |  R:6  |  R:7  |  R:F  | { -1} |
@             +-------+-------+-------+-------+
@PRG Mode 1:  |  R:F  |  R:6  |  R:7  | { -1} |
@             +-------+-------+-------+-------+


cmd6:
	strb_ r0,prg0
	tst r1,#2
	beq_long map89_
	b_long mapAB_
cmd7:
	strb_ r0,prg1
	tst r1,#2
	beq_long mapAB_
	b_long mapCD_
cmdF:
	strb_ r0,prg2
	tst r1,#2
	beq_long mapCD_
	b_long map89_

@----------------------------------------------------------------------------
write1:		@$A000-A001
@----------------------------------------------------------------------------
	tst addy,#1
	movne pc,lr
	tst r0,#1
	b_long mirror2V_
@----------------------------------------------------------------------------
write2:		@C000-C001
@----------------------------------------------------------------------------
	tst addy,#1
	streqb_ r0,latch
	bxeq lr
	ldrb_ r1,latch
	
	mov r1,#0
	strb_ r1,countdown
	
	@set IRQ mode
	tst r0,#1
	ldrb_ r1,irqen
	bic r0,r1,#0x02
	orrne r0,r2,#0x02
	orr r0,r0,#4
	strb_ r0,irqen
	
	@now you should do IRQ mode switch
	@not yet... too lazy to add this
	
	
	@???
	ldr_ r1,cycles_to_run
	sub r1,r1,cycles,asr#CYC_SHIFT
	ldr_ r2,timestamp
	str_ r2,last_mmc3_timestamp
	
	b mmc3_set_next_timeout	@@
@----------------------------------------------------------------------------
write3:		@E000-E001
@----------------------------------------------------------------------------
	ands r0,addy,#1
	ldrb_ r1,irqen
	bic r1,r1,#1
	orr r1,r1,r0
	strb_ r1,irqen

	ldreqb_ r1,wantirq
	biceq r1,r1,#IRQ_MAPPER
	streqb_ r1,wantirq
	
	bx lr

@ .align
@ .pool
@ .section .iwram, "ax", %progbits
@ .subsection 7
@ .align
@ .pool
@
@@----------------------------------------------------------------------------
@RAMBO_IRQ_Hook:
@@----------------------------------------------------------------------------
@@	ldrb_ r0,ppuctrl1
@@	tst r0,#0x18		@no sprite/BG enable?  0x18
@@	beq default_scanlinehook			@bye..
@
@	ldr_ r0,scanline
@	cmp r0,#240		@not rendering?
@	bhi default_scanlinehook			@bye..
@
@	ldrb_ r0,countdown
@	subs r0,r0,#1
@	ldrmib_ r0,latch
@	strb_ r0,countdown
@	bne default_scanlinehook
@
@	ldrb_ r1,irqen
@	cmp r1,#0
@	beq default_scanlinehook
@
@@	mov r1,#0
@@	strb_ r1,irqen
@@	b irq6502
@	b CheckI
	@.end
