#include "equates.h"

@	.include "equates.h"

	.global NES_RAM
	.global NES_SRAM
	.global CHR_DECODE
	.global YSCALE_EXRTA
	.global YSCALE_LOOKUP
	
	.global IWRAM_CANARY_0
	.global IWRAM_CANARY_1
	.global IWRAM_CANARY_2

	.global TEXTMEM
	.global MULTIBOOT_LIMIT	

 .align
 .pool
 .section .bss_prefix, "ax", %progbits
 .subsection 0
 .align
 .pool
NES_RAM: .skip 0x800
NES_SRAM: .skip 0x2000

 .align
 .pool
 .section .bss.end
 .subsection 9999
 .align
 .pool

IWRAM_CANARY_0: .skip 4
CHR_DECODE: .skip 0x400
IWRAM_CANARY_1: .skip 4
YSCALE_EXRTA: .skip 0x50
YSCALE_LOOKUP: .skip 0x108
IWRAM_CANARY_2: .skip 4

@Data END: 0x0300FC50
@Stack END: 0x0300FD80
@Breathing room: 304 bytes

