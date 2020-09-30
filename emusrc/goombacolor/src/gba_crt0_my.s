#include "equates.h"

	.section	".init"
	global_func _start
	.align
	.arm
@---------------------------------------------------------------------------------
_start:
@---------------------------------------------------------------------------------
	b	rom_header_end

	.fill   156,1,0			@ Nintendo Logo Character Data (8000004h)
	.fill	16,1,0			@ Game Title
	.byte   0x30,0x31		@ Maker Code (80000B0h)
	.byte   0x96			@ Fixed Value (80000B2h)
	.byte   0x00			@ Main Unit Code (80000B3h)
	.byte   0x00			@ Device Type (80000B4h)
	.fill	7,1,0			@ unused
	.byte	0x00			@ Software Version No (80000BCh)
	.byte	0xf0			@ Complement Check (80000BDh)
	.byte	0x00,0x00    		@ Checksum (80000BEh)

@---------------------------------------------------------------------------------
rom_header_end:
@---------------------------------------------------------------------------------
	b	start_vector			@ This branch must be here for proper
						@ positioning of the following header.

	.GLOBAL	__boot_method, __slave_number
@---------------------------------------------------------------------------------
__boot_method:
@---------------------------------------------------------------------------------
	.byte   0				@ boot method (0=ROM boot, 3=Multiplay boot)
@---------------------------------------------------------------------------------
__slave_number:
@---------------------------------------------------------------------------------
	.byte   0				@ slave # (1=slave#1, 2=slave#2, 3=slave#3)

	.byte   0 				@ reserved
	.byte   0 				@ reserved
	.word   0    				@ reserved
	.word   0				@ reserved
	.word   0    				@ reserved
	.word   0    				@ reserved
	.word   0    				@ reserved
	.word   0    				@ reserved

    global_func start_vector
    .align
@---------------------------------------------------------------------------------
start_vector:
@---------------------------------------------------------------------------------
	mov	r0, #0x4000000			@ REG_BASE
	str	r0, [r0, #0x208]

	mov	r0, #0x12			@ Switch to IRQ Mode
	msr	cpsr, r0
	ldr	sp, =__sp_irq			@ Set IRQ stack
	mov	r0, #0x1f			@ Switch to System Mode
	msr	cpsr, r0
	ldr	sp, =__sp_usr			@ Set user stack

@---------------------------------------------------------------------------------
@ Enter Thumb mode
@---------------------------------------------------------------------------------
	add	r0, pc, #1
	bx	r0

	.thumb
	
	mov r7,#0
goback:
	ldr	r0, =__text_start
	lsl	r0, #5				@ Was code compiled at 0x08000000 or higher?
@	bcs     DoEWRAMClear			@ yes, you can not run it in external WRAM
	bcs     SkipEWRAMClear			@ yes, you can not run it in external WRAM

	mov     r0, pc
	lsl     r0, #5				@ Are we running from ROM (0x8000000 or higher) ?
	bcc     SkipEWRAMClear			@ No, so no need to do a copy.

@---------------------------------------------------------------------------------
@ We were started in ROM, silly emulators. :P
@ So we need to copy to ExWRAM.
@---------------------------------------------------------------------------------
	mov r7,#1
	
	mov	r2, #2
	lsl	r2, r2, #24			@ r2= 0x02000000
	ldr	r3, =__end__			@ last ewram address
	sub	r3, r2				@ r3= actual binary size
	mov	r6, r2				@ r6= 0x02000000
	lsl	r1, r2, #2			@ r1= 0x08000000

	bl	CopyMem
	ldr r0,=goback+1
	mov pc,r0

@	bx	r6				@ Jump to the code to execute

@---------------------------------------------------------------------------------
@DoEWRAMClear:					@ Clear External WRAM to 0x00
@---------------------------------------------------------------------------------
@	mov	r1, #0x40
@	lsl	r1, #12				@ r1 = 0x40000
@	lsl	r0, r1, #7			@ r0 = 0x2000000
@	bl	ClearMem

@---------------------------------------------------------------------------------
SkipEWRAMClear:					@ Clear Internal WRAM to 0x00
@---------------------------------------------------------------------------------

@---------------------------------------------------------------------------------
@ Copy initialized data (data section) from LMA to VMA (ROM to RAM)
@---------------------------------------------------------------------------------
	ldr	r1, =__data_lma
	ldr	r2, =__data_start__
	ldr	r4, =__data_end__
	bl	CopyMemChk

@---------------------------------------------------------------------------------
@ Copy internal work ram (iwram section) from LMA to VMA (ROM to RAM)
@---------------------------------------------------------------------------------
	ldr	r1,= __iwram_lma
	ldr	r2,= __iwram_start__
	ldr	r4,= __iwram_end__
	bl	CopyMemChk

@---------------------------------------------------------------------------------
@ Copy internal work ram overlay 0 (iwram0 section) from LMA to VMA (ROM to RAM)
@---------------------------------------------------------------------------------
	ldr	r2,= __load_stop_iwram0
	ldr	r1,= __load_start_iwram0
	sub	r3, r2, r1			@ Is there any data to copy?
	beq	CIW0Skip			@ no

	ldr	r2,= __iwram_overlay_start
	bl	CopyMem
@---------------------------------------------------------------------------------
CIW0Skip:
@---------------------------------------------------------------------------------
@ Copy external work ram (ewram section) from LMA to VMA (ROM to RAM)
@---------------------------------------------------------------------------------
	ldr	r1, =__ewram_lma
	ldr	r2, =__ewram_start
	ldr	r4, =__ewram_end
	bl	CopyMemChk

@---------------------------------------------------------------------------------
@ Clear BSS section to 0x00
@---------------------------------------------------------------------------------
	ldr	r0, =__bss_start__
	ldr	r1, =__bss_end__
	sub	r1, r0
	bl	ClearMem

@---------------------------------------------------------------------------------
@ Clear SBSS section to 0x00
@---------------------------------------------------------------------------------
	ldr	r0, =__sbss_start__
	ldr	r1, =__sbss_end__
	sub	r1, r0
	bl	ClearMem

@---------------------------------------------------------------------------------
CEW0Skip:
@---------------------------------------------------------------------------------
@ set heap end
@---------------------------------------------------------------------------------
	ldr	r1, =fake_heap_end
	ldr	r0, =__eheap_end
	str	r0, [r1]
@---------------------------------------------------------------------------------
@ global constructors
@---------------------------------------------------------------------------------
	ldr	r3, =__libc_init_array
	bl	_blx_r3_stub
@---------------------------------------------------------------------------------
@ Jump to user code
@---------------------------------------------------------------------------------
	ldr r0,=copiedfromrom
	str r7,[r0]
	
	mov	r0, #0				@ int argc
	mov	r1, #0				@ char	*argv[]
	ldr	r3, =main
	bl	_blx_r3_stub
@---------------------------------------------------------------------------------
@ Clear memory to 0x00 if length != 0
@---------------------------------------------------------------------------------
@ r0 = Start Address
@ r1 = Length
@---------------------------------------------------------------------------------
ClearMem:
@---------------------------------------------------------------------------------
	mov	r2,#3				@ These	commands are used in cases where
	add	r1,r2				@ the length is	not a multiple of 4,
	bic	r1,r2				@ even though it should be.

	beq	ClearMX				@ Length is zero so exit

	mov	r2,#0
@---------------------------------------------------------------------------------
ClrLoop:
@---------------------------------------------------------------------------------
	stmia	r0!, {r2}
	sub	r1,#4
	bne	ClrLoop
@---------------------------------------------------------------------------------
ClearMX:
@---------------------------------------------------------------------------------
	bx	lr

@---------------------------------------------------------------------------------
_blx_r3_stub:
@---------------------------------------------------------------------------------
	bx	r3

@---------------------------------------------------------------------------------
@ Copy memory if length	!= 0
@---------------------------------------------------------------------------------
@ r1 = Source Address
@ r2 = Dest Address
@ r4 = Dest Address + Length
@---------------------------------------------------------------------------------
CopyMemChk:
@---------------------------------------------------------------------------------
	sub	r3, r4, r2			@ Is there any data to copy?
@---------------------------------------------------------------------------------
@ Copy memory
@---------------------------------------------------------------------------------
@ r1 = Source Address
@ r2 = Dest Address
@ r3 = Length
@---------------------------------------------------------------------------------
CopyMem:
@---------------------------------------------------------------------------------
	mov	r0, #3				@ These commands are used in cases where
	add	r3, r0				@ the length is not a multiple	of 4,
	bic	r3, r0				@ even	though it should be.
	beq	CIDExit				@ Length is zero so exit

@---------------------------------------------------------------------------------
CIDLoop:
@---------------------------------------------------------------------------------
	ldmia	r1!, {r0}
	stmia	r2!, {r0}
	sub	r3, #4
	bne	CIDLoop
@---------------------------------------------------------------------------------
CIDExit:
@---------------------------------------------------------------------------------
	bx	lr

	.align
	.pool
	.end

