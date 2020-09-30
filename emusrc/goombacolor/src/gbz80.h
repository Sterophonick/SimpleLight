	@IMPORT novblankwait

	@IMPORT XGB_RAM
	@IMPORT XGB_HRAM
	@IMPORT CHR_DECODE

	@IMPORT GLOBAL_PTR_BASE
	@IMPORT emu_reset
	@IMPORT default_scanlinehook
	@IMPORT cpustate
	@IMPORT rommap
	@IMPORT checkIRQ
	@IMPORT line145_to_end
	@IMPORT gbc_mode
	
	@IMPORT immediate_check_irq

	.if RESIZABLE
	@IMPORT XGB_sram
	@IMPORT XGB_sramsize
	@IMPORT XGB_vram
	@IMPORT XGB_vramsize
	@IMPORT GBC_exram
	@IMPORT GBC_exramsize
	@IMPORT END_of_exram
	@IMPORT XGB_vram_8000
	@IMPORT XGB_vram_1800
	@IMPORT XGB_vram_1C00
	.endif


	@.end
