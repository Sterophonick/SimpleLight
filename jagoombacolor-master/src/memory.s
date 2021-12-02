 .section .iwram.2, "ax", %progbits

@	#include "equates.h"
@	#include "gbz80.h"
@	#include "lcd.h"

	global_func void
	global_func empty_R
	global_func empty_W
	global_func mem_R00
	global_func mem_R20
	global_func mem_R40
	global_func mem_R60
	global_func mem_R80
	global_func mem_RA0
	global_func mem_RC0
	global_func mem_RC0_2
	global_func sram_W
	global_func sram_W2
	global_func wram_W
	global_func wram_W_2
	global_func echo_W
	global_func echo_R
	
	global_func memset32_
	global_func memcpy32_
	global_func memset32
	global_func memcpy32
	global_func memcpy_unaligned_src
	
	.global sram_W2_modify
@----------------------------------------------------------------------------
empty_R:		@read bad address (error)
@----------------------------------------------------------------------------
	.if DEBUG
		mov r0,addy
		mov r1,#0
		b debug_
	.endif

@	mov gb_flg,addy,lsr#8
@	mov pc,lr
void: @- - - - - - - - -empty function
	mov r0,#0xff	@is this good?
	mov pc,lr
@----------------------------------------------------------------------------
empty_W:		@write bad address (error)
@----------------------------------------------------------------------------
	.if DEBUG
		mov r0,addy
		mov r1,#0
		b debug_
	.else
		mov pc,lr
	.endif
@----------------------------------------------------------------------------
mem_R00:	@rom read ($0000-$1FFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_tbl
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
mem_R20:	@rom read ($2000-$3FFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_tbl+8
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
mem_R40:	@rom read ($4000-$5FFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_tbl+16
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
mem_R60:	@rom read ($6000-$7FFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_tbl+24
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
mem_R80:	@vram read ($8000-$9FFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_tbl+32
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
mem_RA0:	@sram read ($A000-$BFFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_tbl+40
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
mem_RC0:	@ram read ($C000-$CFFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_tbl+48
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
mem_RC0_2:	@ram read ($D000-$DFFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_tbl+52
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
sram_W2:	@write to real sram ($A000-$BFFF)  AND emulated sram
@----------------------------------------------------------------------------
	orr r1,addy,#0xe000000	@r1=e00A000+
sram_W2_modify:
 .if SRAM_32
 	sub r1,r1,#0x4000   @32k sram: A000>>6000
 .else @64k sram
	add r1,r1,#0x4000   @64k sram: A000>>E000
 .endif
	strb r0,[r1]
@----------------------------------------------------------------------------
sram_W:	@sram write ($A000-$BFFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_tbl+40
	strb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
wram_W:	@wram write ($C000-$CFFF)
@----------------------------------------------------------------------------
@	ldr r1,memmap_tbl+48
	adrl_ r1,xgb_ram-0xC000
	strb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
wram_W_2:	@wram write ($D000-$DFFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_tbl+52
	strb r0,[r1,addy]
	mov pc,lr
.text
echo_R:
	sub addy,addy,#0x2000
	tst addy,#0x1000
	beq mem_RC0
	b_long mem_RC0_2

echo_W:
	sub addy,addy,#0x2000
	tst addy,#0x1000
	beq wram_W
	b_long wram_W_2
 .section .iwram.2, "ax", %progbits


memset32:
@word aligned only
@r0 = dest, r1 = data, r2 = byte count
@if byte count is not word aligned, will overwrite more bytes until aligned
	subs r2,r2,#32
	blt 2f
	stmfd sp!,{r3-r9}
	mov r3,r1
	mov r4,r1
	mov r5,r1
	mov r6,r1
	mov r7,r1
	mov r8,r1
	mov r9,r1
0:	
	stmia r0!,{r1,r3-r9}
	subs r2,r2,#32
	blt 1f
	stmia r0!,{r1,r3-r9}
	subs r2,r2,#32
	blt 1f
	stmia r0!,{r1,r3-r9}
	subs r2,r2,#32
	blt 1f
	stmia r0!,{r1,r3-r9}
	subs r2,r2,#32
	bge 0b
1:
	ldmfd sp!,{r3-r9}
2:
	adds r2,r2,#32
	bxle lr
0:
	str r1,[r0],#4
	subs r2,r2,#4
	bgt 0b
	bx lr
memcpy32:
@dest and source must be word-aligned, count may be unaligned
@r0=dest, r1=src, r2=byte count
	subs r2,r2,#32
	blt 2f
	stmfd sp!,{r3-r10}
0:
	ldmia r1!,{r3-r10}
	stmia r0!,{r3-r10}
	subs r2,r2,#32
	blt 1f
	ldmia r1!,{r3-r10}
	stmia r0!,{r3-r10}
	subs r2,r2,#32
	blt 1f
	ldmia r1!,{r3-r10}
	stmia r0!,{r3-r10}
	subs r2,r2,#32
	blt 1f
	ldmia r1!,{r3-r10}
	stmia r0!,{r3-r10}
	subs r2,r2,#32
	bge 0b
1:
	ldmfd sp!,{r3-r10}
2:
	adds r2,r2,#32
	bxle lr
0:
	@subtract first to see if we go negative, don't do a copy step if we're negative
	subs r2,r2,#4
	ldrge r12,[r1],#4
	strge r12,[r0],#4
	bgt 0b
	bxeq lr
	@trailing bytes
	@fall thru to bytecopy_plus4

global_func bytecopy
bytecopy_plus4:
	add r2,r2,#4
bytecopy:
	ldrb r12,[r1],#1
	strb r12,[r0],#1
	subs r2,r2,#1
	bgt bytecopy
	bx lr

global_func memset8
memset8:
	strb r1,[r0],#1
	subs r2,r2,#1
	bgt memset8
	bx lr
	


global_func memcpy

memcpy:
@r0 = dest, r1 = src, r2 = count
	@unaligned destination?  Use bytecopy instead.
	ands r12,r0,#3
	bne bytecopy
	
@dest must be word aligned, count may be unaligned
memcpy_unaligned_src:
	@get shift values/alignment
	ands r12,r1,#3
	@use aligned memcpy if aligned
	beq memcpy32
	stmfd sp!,{r3,r4,lr}
	mov r12,r12,lsl#3
	rsb lr,r12,#32
	@r12 = RSHIFT
	@lr = LSHIFT
	bic r1,r1,#3
	ldr r3,[r1],#4
	mov r3,r3,lsr r12
0:	
	@subtract first to see if we go negative, don't do a store if we're negative
	subs r2,r2,#4
	@loop: load word, merge with old word (at proper shift location), store word
	ldr r4,[r1],#4
	orr r3,r3,r4,lsl lr
	strge r3,[r0],#4
	movge r3,r4,lsr r12
	bgt 0b
	ldmeqfd sp!,{r3,r4,lr}
	bxeq lr
@do remaining bytes of r3
	add r2,r2,#4
0:
	strb r3,[r0],#1
	mov r3,r3,lsr#8
	subs r2,r2,#1
	bgt 0b
	ldmfd sp!,{r3,r4,lr}
	bx lr
	

 .align
 .pool
 .text
 .align
 .pool
memset32_:
	b_long memset32
memcpy32_:
	b_long memcpy32

 
@filler_ ;r0=data r1=dest r2=word count
@@	exit with r0 unchanged
@@----------------------------------------------------------------------------
@	subs r2,r2,#1
@	str r0,[r1,r2,lsl#2]
@	bne filler_
@	bx lr
@@----------------------------------------------------------------------------
@copy_
@	;r0=dest, r1=src, r2=count, addy=destroyed
@	subs r2,r2,#1
@	ldr addy,[r1,r2,lsl#2]
@	str addy,[r0,r2,lsl#2]
@	bne copy_
@	bx lr

	@.end
