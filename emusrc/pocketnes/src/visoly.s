#include "equates.h"

 .text

#if VISOLY

global_func VISOLY_START
global_func VISOLY_END

	.pool

VISOLY_START:
	@Disable interrupts and DMA before calling this
	adr r2,rom_addresses	@the addresses to write to/read from  (or special numbers indicating a run)
	adr r3,data_values		@the 16-bit data values to write (or special numbers indicating a run)
	mov r4,#0	@r4 = number of same address in a run (address values smaller than 65536 indicate this)
	mov r5,#0	@r5 = number of same 16-bit values to write (data values smaller than 512 indicate this)
	
	@ reset EZ FLASH, SC, Visoly
	ldr r1,=6+4+8+1500	@6 writes for EZ, 4 writes for supercard, 1508 writes for Visoly, total is 1518
	bl do_writes
	
	@ reset M3, G6
	mov r1,#14+14	@14 reads for M3, 14 reads for G6
	bl do_reads
	
	mov		r0, #0
	ldr		r1,=0x3007ffa	@must be 0 before swi 0x00 is run, otherwise it tries to start from 0x02000000.
	strh		r0,[r1]

	mov		r0, #8		@VRAM clear
	swi		0x010000
	@Reboot
	swi		0x000000


@	ldmfd sp!,{lr,r4,r5}
@	bx lr
get_word:
	movs r4,r4			@any left in the run?  jump ahead
	bne 1f
	ldr r12,[r2],#4		@read address
	movs r12,r12		@if address == 0, or address > 65536, it's a legitimate address.
	bxeq lr
	cmp r12,#0x00010000	@otherwise, it's a RLE length
	bxgt lr
	sub r4,r12,#1		@process first word
	ldr r12,[r2],#4
	bx lr
1:
	ldr r12,[r2,#-4]	@read old word again
	sub r4,r4,#1		@decrease remaining count
	bx lr

get_hword:
	movs r5,r5			@any left in the run?  jump ahead
	bne 1f
	ldrh r0,[r3],#2		@read data
	movs r0,r0			@if data == 0, or data > 512, it's a proper data word
	bxeq lr
	cmp r0,#0x0200		@otherwise it's a RLE length
	bxgt lr
	sub r5,r0,#1		@process first halfword
	ldrh r0,[r3],#2
	bx lr
1:
	ldrh r0,[r3,#-2]	@read old halfword again
	sub r5,r5,#1		@decrease remaining count
	bx lr
	
do_reads:
	stmfd sp!,{lr}
0:
	bl get_word			@Get the address to read from
	ldrh r0,[r12]		@perform the read
	subs r1,r1,#1		@decrease remaining
	bne 0b				@repeat for the next address

	ldmfd sp!,{pc}

do_writes:
	stmfd sp!,{lr}
0:
	bl get_word			@Get the address to write to
	bl get_hword		@Get the value to write
	strh r0,[r12]		@perform the write
	subs r1,r1,#1		@decrease remaining
	bne 0b				@repeat for the next address and value
	
	ldmfd sp!,{pc}

rom_addresses:
	@ez flash
	.word 0x9FE0000
	.word 0x8000000
	.word 0x8020000
	.word 0x8040000
	.word 0x9880000
	.word 0x9FC0000

	@SC
	.word 4
	.word 0x09FFFFFE

	@Visoly
	.word   (0x00987654 * 2) + 0x08000000
	.word   1001
	.word   (0x00012345 * 2) + 0x08000000
	.word   (0x00987654 * 2) + 0x08000000
	.word   (0x00012345 * 2) + 0x08000000
	.word   (0x00765400 * 2) + 0x08000000
	.word   (0x00013450 * 2) + 0x08000000
	.word   500
	.word   (0x00012345 * 2) + 0x08000000
	.word   (0x00987654 * 2) + 0x08000000
	.word	0x096B592E

	@M3  (reads)
	.word 0x08E00002
	.word 0x0800000E
	.word 0x08801FFC
	.word 0x0800104A
	.word 0x08800612
	.word 0x08000000
	.word 0x08801B66
	.word 0x08800008
	.word 0x0800080E
	.word 0x08000000
	.word 0x080001E4
	.word 0x080001E4
	.word 0x08000188
	.word 0x08000188
    
	@G6  (reads)
	.word 0x09000000
	.word 0x09FFFFE0
	.word 3
	.word 0x09FFFFEC
	.word 3
	.word 0x09FFFFFC
	.word 3
	.word 0x09FFFF4A
	.word 0x09200000
	.word 0x09FFFFF0
	.word 0x09FFFFE8	

data_values:
	@ez_flash
	.hword 0xD200
	.hword 0x1500
	.hword 0xD200
	.hword 0x1500
	.hword 0x8000
	.hword 0x1500

	@SC	
	.hword 0xA55A
	.hword 0xA55A
	.hword 0
	.hword 0

	@visoly
	.hword 0x5354
	.hword 500
	.hword 0x1234
	.hword 0x5354
	.hword 500
	.hword 0x5678
	.hword 0x5354
	.hword 0x5354
	.hword 0x5678
	.hword 0x1234
	.hword 500
	.hword 0xABCD
	.hword 0x5354
	.hword 0

	.pool
VISOLY_END:

		.pool
        @.end

#endif
