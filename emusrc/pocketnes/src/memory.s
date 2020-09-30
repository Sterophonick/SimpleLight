#include "equates.h"

 .align
 .pool
 .section .iwram, "ax", %progbits
 .subsection 2
 .align
 .pool

@	.include "equates.h"
@	.include "6502.h"
@	.include "ppu.h"

	global_func void
	global_func empty_R
	global_func empty_W
	global_func ram_R
	global_func ram_W
	global_func sram_R
	global_func sram_W
	.if CARTSAVE
	global_func sram_W2
	global_func sram_W2_modify
	.endif
	global_func rom_R60
	global_func rom_R80
	global_func rom_RA0
	global_func rom_RC0
	global_func rom_RE0
.if PRG_BANK_SIZE == 4
	global_func rom_R50
	global_func rom_R70
	global_func rom_R90
	global_func rom_RB0
	global_func rom_RD0
	global_func rom_RF0
.endif


@For memory reads:
@DO NOT TOUCH R12 (addy), it needs to be preserved for Read-Modify-Write instructions
@Memory writes do not need to preserve r12.

@----------------------------------------------------------------------------
empty_R:		@read bad address (error)
@----------------------------------------------------------------------------
@	.if DEBUG
@		mov r0,addy
@		mov r1,#0
@		b debug_
@	.endif

	mov r0,addy,lsr#8
void: @- - - - - - - - -empty function
@	mov r0,#0	@VS excitebike liked this, read from $3DDE ($2006).
	mov pc,lr
@----------------------------------------------------------------------------
empty_W:		@write bad address (error)
@----------------------------------------------------------------------------
@	.if DEBUG
@		mov r0,addy
@		mov r1,#0
@		b debug_
@	.else
		mov pc,lr
@	.endif
@----------------------------------------------------------------------------
ram_R:	@ram read ($0000-$1FFF)
@----------------------------------------------------------------------------
	bic addy,addy,#0x1f800		@only 0x07FF is RAM
	ldrb r0,[cpu_zpage,addy]
	mov pc,lr
@----------------------------------------------------------------------------
ram_W:	@ram write ($0000-$1FFF)
@----------------------------------------------------------------------------
	bic addy,addy,#0x1f800		@only 0x07FF is RAM
	strb r0,[cpu_zpage,addy]
	mov pc,lr
@----------------------------------------------------------------------------
sram_R:	@sram read ($6000-$7FFF)
@----------------------------------------------------------------------------
	sub r1,addy,#0x5800
	ldrb r0,[cpu_zpage,r1]
	mov pc,lr
@----------------------------------------------------------------------------
sram_W:	@sram write ($6000-$7FFF)
@----------------------------------------------------------------------------
	sub addy,addy,#0x5800
	strb r0,[cpu_zpage,addy]
	mov pc,lr
	.if CARTSAVE
@----------------------------------------------------------------------------
sram_W2:	@write to real sram ($6000-$7FFF)
@----------------------------------------------------------------------------
	sub r1,addy,#0x5800
	strb r0,[cpu_zpage,r1]
		orr r1,addy,#AGB_SRAM	@r1=e006000+
sram_W2_modify:
 .if SAVE32
 		nop
 .else
		add r1,r1,#0x8000		@r1=e00e000+
 .endif
		strb r0,[r1]
	mov pc,lr
	.endif
@----------------------------------------------------------------------------
rom_R60:	@rom read ($6000-$7FFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_6
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
rom_R80:	@rom read ($8000-$9FFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_8
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
rom_RA0:	@rom read ($A000-$BFFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_A
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
rom_RC0:	@rom read ($C000-$DFFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_C
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
rom_RE0:	@rom read ($E000-$FFFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_E
	ldrb r0,[r1,addy]
	mov pc,lr

.if PRG_BANK_SIZE == 4
@----------------------------------------------------------------------------
rom_R50:	@rom read ($5000-$5FFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_5
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
rom_R70:	@rom read ($7000-$7FFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_7
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
rom_R90:	@rom read ($9000-$9FFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_9
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
rom_RB0:	@rom read ($B000-$BFFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_B
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
rom_RD0:	@rom read ($D000-$DFFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_D
	ldrb r0,[r1,addy]
	mov pc,lr
@----------------------------------------------------------------------------
rom_RF0:	@rom read ($F000-$FFFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_F
	ldrb r0,[r1,addy]
	mov pc,lr

.endif


@----------------------------------------------------------------------------
@rom_R	@rom read ($8000-$FFFF) (actually $6000-$FFFF now)
@----------------------------------------------------------------------------
@	adr_ r2,memmap_tbl
@	ldr r1,[r2,r1,lsr#11] @r1=addy & 0xe000
@	ldrb r0,[r1,addy]
@	mov pc,lr
@----------------------------------------------------------------------------
