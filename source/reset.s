@---------------------------------------------------------------------------------
	.section	.iwram,"ax",%progbits


@;--------------------------------------------------------------
	.code 16
	.global	SoftReset_now
SoftReset_now:
	ldr r2,=SoftReset_now16
	bx  r2
	.code 32
SoftReset_now16:
	adr r4,SoftReset
	adr r3,SoftReset_end
	mov r2,#0x03000000
	add r2,r2,#0x7e00
copy_loop:
	ldr r5,[r4],#4
	str r5,[r2],#4
	cmp r4,r3
	blt copy_loop

@	mov		r2,#0x3000000
@	@;add		r3,r2,#0x1000
@	@ldr		r3,=SoftReset_now
@	adr		r3,SoftReset_now
@	mov		r0,#0
@clearL:
@	str		r0,[r2],#+4
@	cmp		r2,r3
@	blt		clearL

@	adr		r2,SoftReset_end
@	@ldr		r2,=SoftReset_end
@	@ldr		r2,=RegisterRamReset
@	ldr		r3,=0x3008000
@	mov		r0,#0
@clearL2:
@	str		r0,[r2],#+4
@	cmp		r2,r3
@	blt		clearL2
		
@	adr r0,SoftReset
@	mov r0,#0x02000000
@	add r0,r0,#0x1000
	mov r2,#0x03000000
	add r2,r2,#0x7e00
	add r2,r2,#1
	bx r2
@----------------------------------------------
	.code 16

	.global RegisterRamReset
RegisterRamReset:
	swi		1
	bx		lr	
	.global LZ77UnCompWram
LZ77UnCompWram:
	swi		0x12
	bx		lr
@----------------------------------------------	
	.global	SoftReset
SoftReset:
		ldr		r3, =0x04000208
		mov		r2, #0
		str		r2, [r3, #0]
		@;ldr		r3, =0x03007FFA
		@;mov		r0, #0x2 @;FD
		@;strb		r0, [r3, #0]
		@;swi		0x1
		@ldr		r3, =0x03007F00
		@mov		r2,#1
		@str		r2, [r3]
		ldr		r3, =0x03007FFA
		@mov		r2, #0x0
		strb		r0, [r3, #0]
		ldr		r2, =0x03007f00
		mov		sp, r2
		cmp		r1, #0xff
		bgt		NoRamReset
		mov		r0, r1
		swi 1
		
NoRamReset:
		swi 0
		
		
	@ldr		r3, =0x03007FFA		@ restart flag
	@strb	r0,[r3, #0]
	@ldr		r3, =0x04000208		@ REG_IME
	@mov		r2, #0
	@strb	r2, [r3, #0]
	@ldr		r1, =0x03007f00
	@mov		sp, r1
	@swi		1
	@swi		0

@----------------------------------------------
	.global	HardReset
HardReset:
	ldr		r3, =0x04000208
	mov		r2, #0
	str		r2, [r3, #0]
	ldr		r1, =0x3007f00
	mov		SP, r1
	swi		0x26
	
	.align
	.ltorg
SoftReset_end:
