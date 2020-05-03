#include "equates.h"

@	.include "equates.h"
@	.include "mem.h"

	.if CHEATFINDER

 .align
 .pool
 .text
 .align
 .pool

 @IMPORT compare_value
 @IMPORT cheatfinderstate
 global_func cheat_test
 
 @IMPORT cheatfinder_values
 @IMPORT cheatfinder_bits

 results .req r0
 bits .req r5
 values .req r7
 ram .req r2
 mask .req r3
 word .req r4
 index .req r6
 changeok .req r1
 temp .req r8
 prev .req r10
 compare .req r9

@0 ==
@4 != 
@8 >
@12 <
@16 >=
@20 <=
@24 true

cheat_test:
	stmfd sp!,{r1-r10,lr}
@init self modify code
	@more init code, need to modify this code
	ldr temp,=cheatfinderstate
	ldr temp,[temp]
	cmp temp,#1
	ldreq temp,mod1table
	ldrne temp,mod1table+4
	
	ldr r2,=ct_modify1
	str temp,[r2]
	ldr temp,=mod2table
	ldr temp,[temp,r0]
	ldr r2,=ct_modify2
	str temp,[r2]

	mov results,#0

	mov mask,#0x00000001
	ldr ram,=NES_RAM
	ldr values,=cheatfinder_values
	ldr values,[values]
	ldr bits,=cheatfinder_bits
	ldr bits,[bits]
	sub bits,bits,#4
	mov index,#0
	ldr compare,=compare_value
	ldrb compare,[compare]
	b_long ctloop

mod1table:
	cmp temp,prev
	cmp temp,compare
mod2table:
@eq -> ne
@ne -> eq
@gt -> le
@lt -> ge
@ge -> lt
@le -> gt

	.word 0x0A000003	@beq 3
	.word 0x1A000003	@bne 3
	.word 0xCA000003	@bgt 3
	.word 0xBA000003	@blt 3
	.word 0xAA000003	@bge 3
	.word 0xDA000003	@ble 3
	.word 0xEA000003  @b   3

	
 .align
 .pool
 .section .iwram, "ax", %progbits
 .subsection 5
 .align
 .pool
	
ctloop:
	movs mask,mask,ror #1
	ldrcs word,[bits,#4]!
	tst word,mask
	beq aftermark

	ldrb temp,[ram,index]
	ldrb prev,[values,index]
ct_modify1:
	cmp temp,prev
ct_modify2:
	beq dontmark
	cmp changeok,#0
	bicne word,word,mask
	strne word,[bits]
	b aftermark
dontmark:
	add results,results,#1
aftermark:
	add index,index,#1
	cmp index,#10240
	bne ctloop
	ldmfd sp!,{r1-r10,lr}
	bx lr
 @.end

.unreq results
.unreq bits
.unreq values
.unreq ram
.unreq mask
.unreq word
.unreq index
.unreq changeok
.unreq temp
.unreq prev
.unreq compare

	.endif
