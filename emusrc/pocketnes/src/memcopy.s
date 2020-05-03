#include "equates.h"

 .align
 .pool
 .section .iwram, "ax", %progbits
 .subsection 2
 .align
 .pool

	global_func memset8
	global_func memset16
	global_func memset32
	global_func memset32_
	global_func memcpy32
	global_func memset16_
	global_func memcpy32_
	global_func simpleswap32
	global_func memmove32


memset32:
	mov r2,r2,lsr#1
	b 0f
memset16:
	@r0 = dest
	@r1 = halfword to fill
	@r2 = number of halfwords to write
	orr r1,r1,r1,lsl#16
	@get aligned
	tst r0,#2
	strneh r1,[r0],#2
	subne r2,r2,#1
0:
	@pre-subtract, jump ahead if not enough remaining
	subs r2,r2,#12
	bmi 2f
	stmfd sp!,{r3-r5,lr}
	mov r3,r1
	mov r4,r1
	mov r5,r1
	mov r12,r1
	mov lr,r1
1:
	stmia r0!,{r1,r3-r5,r12,lr} @24 bytes
	subs r2,r2,#12
	bmi 3f
	stmia r0!,{r1,r3-r5,r12,lr}
	subs r2,r2,#12
	bpl 1b
3:
	adds r2,r2,#12
	ldmeqfd sp!,{r3-r5,lr}
	bxeq lr
	subs r2,r2,#4
	bmi 3f
1:
	stmia r0!,{r1,r12}
	subs r2,r2,#4
	bmi 3f
	stmia r0!,{r1,r12}
	subs r2,r2,#4
	bpl 1b
3:
	sub r2,r2,#8
	ldmfd sp!,{r3,r4,r5,lr}
2:
	adds r2,r2,#12
	bxle lr
1:	
	strh r1,[r0],#2
	subs r2,r2,#1
	bgt 1b
	bx lr

memcpy32:
@word aligned only
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
	bge 0b
1:
	ldmfd sp!,{r3-r10}
2:
	adds r2,r2,#32
	bxle lr
memcpy32_words:
0:
	ldr r12,[r1],#4
	str r12,[r0],#4
	subs r2,r2,#4
	bgt 0b
	bx lr

bytecopy:
	@r0=dest, r1=src, r2=byte count
	ldrb r12,[r0],#1
	strb r12,[r1],#1
	subs r2,r2,#1
	bgt bytecopy
	bx lr

memmove32:
	@r0=dest, r1=src, r2=byte count
	cmp r0,r1
	@dest < src: copy forwards
	ble memcpy32
	@otherwise copy backwards
	add r0,r0,r2
	add r1,r1,r2
	subs r2,r2,#32
	blt 2f
	stmfd sp!,{r3-r9}
0:
	ldmdb r1!,{r3-r9,r12}
	stmdb r0!,{r3-r9,r12}
	subs r2,r2,#32
	bge 0b
	ldmfd sp!,{r3-r9}
2:
	adds r2,r2,#32
	bxle lr
0:
	ldr r12,[r1,#-4]!
	str r12,[r0,#-4]!
	subs r2,r2,#4
	bgt 0b
	bx lr

simpleswap32:
	@r0=dest, r1=src, r2=byte count
	subs r2,r2,#16
	blt 2f
	stmfd sp!,{r3-r9}
0:
	ldmia r1,{r3,r4,r5,r6}
	ldmia r0,{r7,r8,r9,r12}
	stmia r0!,{r3,r4,r5,r6}
	stmia r1!,{r7,r8,r9,r12}
	subs r2,r2,#16
	bge 0b
	ldmfd sp!,{r3-r9}
2:
	adds r2,r2,#16
	bxle lr
0:
	ldr r12,[r1]
	ldr r3,[r0]
	str r3,[r1],#4
	str r12,[r0],#4
	subs r2,r2,#4
	bgt 0b
	bx lr

 .align
 .pool
 .text
 .align
 .pool
memset8:
	orr r1,r1,r1,lsl#8
	orr r1,r1,r1,lsl#16
	b_long memset32
	
memset32_:
	b_long memset32
memset16_:
	b_long memset16
memcpy32_:
	b_long memcpy32

