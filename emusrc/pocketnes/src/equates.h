#ifndef __EQUATES_H__
#define __EQUATES_H__

#include "config.h"

@constants from ppu.s
 SCREEN_HEIGHT = 160
 SCREEN_LEFT = 8
 BG_VRAM = AGB_VRAM
 SPR_VRAM_ADD = 0x10000
 SPR_VRAM = AGB_VRAM+SPR_VRAM_ADD

@Set PRG_BANK_BITS to 3 or 4.  3 is for 8k sized pages, 4 is for 4k sized pages (NSF files)
PRG_BANK_BITS = 3
PRG_BANK_SIZE = 1 << (6 - PRG_BANK_BITS)  @ = 8
PRG_BANK_COUNT = 64 / PRG_BANK_SIZE  @ = 8
PRG_BANK_MASK = ((-1) << (16 - PRG_BANK_BITS)) & 0xFFFF  @ = E000
PRG_BANK_SHIFT = (16 - PRG_BANK_BITS) @ = 13
PRG_BANK_LOOKUP_SHIFT_32K = PRG_BANK_BITS + 2 - 1 @4
PRG_BANK_LOOKUP_SHIFT_16K = PRG_BANK_BITS + 2 - 2 @3
PRG_BANK_LOOKUP_SHIFT_8K = PRG_BANK_BITS + 2 - 3  @2
PRG_BANK_LOOKUP_SHIFT_4K = PRG_BANK_BITS + 2 - 4  @1, invalid


SHIFT_8K = 13
SHIFT_16K = 14
SHIFT_32K = 15

/*
@options
 REDUCED_FONT = 1
 OLDSPEEDHACKS = 0
 DRAW_ATTRIBUTE_TABLES = 0
 HAPPY_CPU_TESTER = 1

 FULL_DMC = 0  @set to 1 to emulate dmc cycle stealing
 LINK = 1
 RESET_ALL = 1
 USE_BG_CACHE = 1
 CARTSAVE = 1
 RTCSUPPORT = 1
 DEBUG		= 0
 CRASH      = 1
 CHEATFINDER	= 1
 MOVIEPLAYER	= 0
 VERSION_IN_ROM	= 1
 DIRTYTILES = 1
 MIXED_VRAM_VROM = 1
 SPRITESCAN = 1
 LESSMAPPERS = 0
 APACK	=	1
 SAVE32 = 0
 VISOLY = 1

 SAVESTATES = 0
	.if CARTSAVE
 SAVESTATES = 1
	.endif
	.if MOVIEPLAYER
 SAVESTATES = 1
	.endif
*/

@constants for sprite cache
SPR_CACHE_SIZE = 4
SPR_CACHE_START = 8
SPR_CACHE_OFFSET = SPR_CACHE_START*2048

@constants for bg cache
	.if USE_BG_CACHE
 BG_CACHE_SIZE = 512
	.endif

@constants for dirty tiles
	.if DIRTYTILES
 MAX_RECENT_TILES = 20  @Battletoads updates up to 20 tiles per frame
	.endif

@----------------------------------------------------------------------------

#include "macro.h"

@06003000 - $1000  @used for code
@06007000 - $1000  @used for secondary nametables / buffers

@@static array allocations

@These equates are no longer used.  Code within loadcart puts them into GBA 
@VRAM if the game is not using 4-screen mirroring, or puts them into GBA EWRAM.  
@They are defined as zero because some code in the globals still refers to 
@these equates.

DMA0BUFF	= 0
DMA1BUFF	= 0
DMA3BUFF	= 0
SCROLLBUFF1	= 0
SCROLLBUFF2	= 0
DISPCNTBUFF1	= 0

@ DMA0BUFF	= 0x06004000-164*4	@scaled SCROLLBUFF
@ DMA1BUFF	= DMA0BUFF-164*2		@scaled BG0CNTBUFF
@ DMA3BUFF	= DMA1BUFF-164*2		@scaled DISPCNTBUFF
@ SCROLLBUFF1	= DMA3BUFF-240*4
@ SCROLLBUFF2	= SCROLLBUFF1-240*4
@ DISPCNTBUFF1	= SCROLLBUFF2-240*2

@ DMA0BUFF		= 0x06003800-164*4  @@48 remaining  ;scaled SCROLLBUFF, scrolling
@ DMA3BUFF		= 0x06004000-164*2	@scaled BG0CNTBUFF, mirroring, bg bank selection
@ DMA1BUFF		= DMA3BUFF-164*2    @@112 remaining	;scaled DISPCNTBUFF, bg&sprite on/off

	@GBLA	NEXT

 MEM_END	= 0x02040000
 NESOAMBUFF2 = MEM_END-256
 NESOAMBUFF1 = NESOAMBUFF2-256

 NEXT = NESOAMBUFF1

	.if DIRTYTILES
 RECENT_TILES1	= NEXT-(MAX_RECENT_TILES*16)
 RECENT_TILES2	= RECENT_TILES1-(MAX_RECENT_TILES*16)

 NEXT = RECENT_TILES2
	.endif

	.if OLDSPEEDHACKS
 SPEEDHACK_FIND_JMP_BUF	=	NEXT-128
 SPEEDHACK_FIND_BEQ_BUF	= SPEEDHACK_FIND_JMP_BUF-128
 SPEEDHACK_FIND_BNE_BUF	= SPEEDHACK_FIND_BEQ_BUF-128
 SPEEDHACK_FIND_BCS_BUF	=	SPEEDHACK_FIND_BNE_BUF-128
 SPEEDHACK_FIND_BCC_BUF	=	SPEEDHACK_FIND_BCS_BUF-128
 SPEEDHACK_FIND_BVS_BUF	=	SPEEDHACK_FIND_BCC_BUF-128
 SPEEDHACK_FIND_BVC_BUF	=	SPEEDHACK_FIND_BVS_BUF-128
 SPEEDHACK_FIND_BMI_BUF	=	SPEEDHACK_FIND_BVC_BUF-128
 SPEEDHACK_FIND_BPL_BUF	=	SPEEDHACK_FIND_BMI_BUF-128
 NEXT = SPEEDHACK_FIND_BPL_BUF
	.else
 SPEEDHACK_TEMP_BUF	=	NEXT-96
 SPEEDHACK_INCS	= SPEEDHACK_TEMP_BUF-128
 speedhacks = SPEEDHACK_INCS - 64
 NEXT = speedhacks
	.endif

 TEXTMEM = NEXT - 632

 BG0CNTBUFF1	= TEXTMEM-240*2
 BG0CNTBUFF2	= BG0CNTBUFF1-240*2
@DISPCNTBUFF1 @already defined
 DISPCNTBUFF2	= BG0CNTBUFF2-240*2
@SCROLLBUFF1	@already defined
@ SCROLLBUFF2		= DISPCNTBUFF2-240*4
@DMA3BUFF	@already defined
@DMA1BUFF	@already defined
@DMA0BUFF	@already defined

 BANKBUFFER1  = DISPCNTBUFF2-30*8
 BANKBUFFER2  = BANKBUFFER1-30*8

@ chr_rom_table = BANKBUFFER2-1024
 spr_cache_map = BANKBUFFER2-256
 spr_cache_disp	= spr_cache_map-16	@Must be immediately AFTER spr_cache in memory
 spr_cache	= spr_cache_disp-16

 NEXT = spr_cache

@	.if VERSION != "COMPY"
 PCMWAVSIZE		= 128  @plus 8 trailing bytes to eliminate popping
 PCMWAV = 0x05000280
@ PCMWAV			= NEXT-PCMWAVSIZE
@ MAPPED_RGB		= PCMWAV-0xC0
@ NEXT = PCMWAV
@	.endif

 PALETTE_BUFFER_2 = NEXT - 128 - 8
 PALETTE_BUFFER_1 = PALETTE_BUFFER_2 - 128 - 8
 DISPLAYED_PALETTE_BUFFER = PALETTE_BUFFER_1 - 192 - 4
 MAPPED_RGB = AGB_PALETTE + 0xC0
 @MAPPED_RGB is normally 128 bytes large, but it is temporarliy extended to 192 bytes as it remaps the palette
 DISPLAYED_PALETTE_BUFFER = AGB_PALETTE + 0x140
 @Overlaps with temp area of MAPPED_RGB, but it overwrites it after it's done with it
 
 NEXT = PALETTE_BUFFER_1

	.if CARTSAVE
 CachedConfig	= NEXT-48
 NEXT = CachedConfig
	.endif

	.if DIRTYTILES
 RECENT_TILENUM1	= NEXT-(MAX_RECENT_TILES+2)*2
 RECENT_TILENUM2	= RECENT_TILENUM1-(MAX_RECENT_TILES+2)*2
 dirty_rows  = RECENT_TILENUM2-32		@Must be immediately AFTER dirty_tiles in memory
 dirty_tiles = dirty_rows -516

 NEXT = dirty_tiles

	.endif

	.if USE_BG_CACHE
 BG_CACHE	= NEXT-BG_CACHE_SIZE

 NEXT = BG_CACHE
	.endif

 NES_VRAM2   = NEXT-2048
 NES_VRAM	= NES_VRAM2-0x2000
 NES_VRAM4   = NES_VRAM-2048
 END_OF_EXRAM	= NES_VRAM4   @!How much data is left for Multiboot to work!
@ FREQTBL2			= NES_VRAM4-0x1000
@ END_OF_EXRAM	= FREQTBL2	@!How much data is left for Multiboot to work!

 MULTIBOOT_LIMIT = END_OF_EXRAM

@ CHEATFINDER_VALUES = 0x6014000-10240
@ CHEATFINDER_BITS = CHEATFINDER_VALUES-1280  @2624 (A40) remaining
@ CHEATFINDER_CHEATS = CHEATFINDER_BITS-900 @must be multiple of 3 and 2 (4?)

 AGB_IRQVECT		= 0x3007FFC
 AGB_PALETTE		= 0x5000000
 AGB_VRAM		= 0x6000000
 AGB_OAM			= 0x7000000
 AGB_SRAM		= 0xE000000
 AGB_BG			= AGB_VRAM+0x6000
 DEBUGSCREEN		= AGB_VRAM+0x2000

 REG_BASE		= 0x4000000
 REG_DISPCNT		= 0x00
 REG_DISPSTAT	= 0x04
 REG_VCOUNT		= 0x06
 REG_BG0CNT		= 0x08
 REG_BG1CNT		= 0x0A
 REG_BG2CNT		= 0x0C
 REG_BG3CNT		= 0x0E
 REG_BG0HOFS		= 0x10
 REG_BG0VOFS		= 0x12
 REG_BG1HOFS		= 0x14
 REG_BG1VOFS		= 0x16
 REG_BG2HOFS		= 0x18
 REG_BG2VOFS		= 0x1A
 REG_BG3HOFS		= 0x1C
 REG_BG3VOFS		= 0x1E
 REG_WIN0H		= 0x40
 REG_WIN1H		= 0x42
 REG_WIN0V		= 0x44
 REG_WIN1V		= 0x46
 REG_WININ		= 0x48
 REG_WINOUT		= 0x4A
 REG_BLDCNT		= 0x50
 REG_BLDALPHA	= 0x52
 REG_BLDY		= 0x54
 REG_SG1CNT_L	= 0x60
 REG_SG1CNT_H	= 0x62
 REG_SG1CNT_X	= 0x64
 REG_SG2CNT_L	= 0x68
 REG_SG2CNT_H	= 0x6C
 REG_SG3CNT_L	= 0x70
 REG_SG3CNT_H	= 0x72
 REG_SG3CNT_X	= 0x74
 REG_SG4CNT_L	= 0x78
 REG_SG4CNT_H	= 0x7c
 REG_SGCNT_L		= 0x80
 REG_SGCNT_H		= 0x82
 REG_SGCNT_X		= 0x84
 REG_SGBIAS		= 0x88
 REG_SGWR0_L		= 0x90

 REG_SOUND1CNT_L	= 0x60
 REG_SOUND1CNT_H	= 0x62
 REG_SOUND1CNT_X	= 0x64
 REG_SOUND2CNT_L	= 0x68
 REG_SOUND2CNT_H	= 0x6C
 REG_SOUND3CNT_L	= 0x70
 REG_SOUND3CNT_H	= 0x72
 REG_SOUND3CNT_X	= 0x74
 REG_SOUND4CNT_L	= 0x78
 REG_SOUND4CNT_H	= 0x7c
 REG_SOUNDCNT_L	= 0x80
 REG_SOUNDCNT_H	= 0x82
 REG_SOUNDCNT_X	= 0x84
 REG_SOUNDBIAS	= 0x88
 REG_SOUNDWR0_L	= 0x90

 REG_FIFO_A_L	= 0xA0
 REG_FIFO_A_H	= 0xA2
 REG_FIFO_B_L	= 0xA4
 REG_FIFO_B_H	= 0xA6
 REG_DM0SAD		= 0xB0
 REG_DM0DAD		= 0xB4
 REG_DM0CNT_L	= 0xB8
 REG_DM0CNT_H	= 0xBA
 REG_DM1SAD		= 0xBC
 REG_DM1DAD		= 0xC0
 REG_DM1CNT_L	= 0xC4
 REG_DM1CNT_H	= 0xC6
 REG_DM2SAD		= 0xC8
 REG_DM2DAD		= 0xCC
 REG_DM2CNT_L	= 0xD0
 REG_DM2CNT_H	= 0xD2
 REG_DM3SAD		= 0xD4
 REG_DM3DAD		= 0xD8
 REG_DM3CNT_L	= 0xDC
 REG_DM3CNT_H	= 0xDE

 REG_DMA0SAD	= 0xB0
 REG_DMA0DAD	= 0xB4
 REG_DMA0CNT_L	= 0xB8
 REG_DMA0CNT_H	= 0xBA
 REG_DMA1SAD	= 0xBC
 REG_DMA1DAD	= 0xC0
 REG_DMA1CNT_L	= 0xC4
 REG_DMA1CNT_H	= 0xC6
 REG_DMA2SAD	= 0xC8
 REG_DMA2DAD	= 0xCC
 REG_DMA2CNT_L	= 0xD0
 REG_DMA2CNT_H	= 0xD2
 REG_DMA3SAD	= 0xD4
 REG_DMA3DAD	= 0xD8
 REG_DMA3CNT_L	= 0xDC
 REG_DMA3CNT_H	= 0xDE

 REG_TM0CNT_L	= 0x100
 REG_TM0CNT_H	= 0x102
 REG_TM1CNT_L	= 0x104
 REG_TM1CNT_H	= 0x106
 REG_TM2CNT_L	= 0x108
 REG_TM2CNT_H	= 0x10A
 REG_TM3CNT_L	= 0x10C
 REG_TM3CNT_H	= 0x10E
 REG_IE			= 0x200
 REG_IME			= 0x208
 REG_IF			= 0x4000202
 REG_P1			= 0x4000130
 REG_P1CNT		= 0x132
 REG_WAITCNT		= 0x4000204

 REG_SIOMULTI0	= 0x20 @+100
 REG_SIOMULTI1	= 0x22 @+100
 REG_SIOMULTI2	= 0x24 @+100
 REG_SIOMULTI3	= 0x26 @+100
 REG_SIOCNT		= 0x28 @+100
 REG_SIOMLT_SEND	= 0x2a @+100
 REG_RCNT		= 0x34 @+100

		@r0,r1,r2=temp regs
 m6502_nz	.req r3 @bit 31=N, Z=1 if bits 0-7=0
 m6502_mmap	.req r4 @memmap_tbl
 m6502_a		.req r5 @bits 0-23=0, also used to clear bytes in memory
 m6502_x		.req r6 @bits 0-23=0
 m6502_y		.req r7 @bits 0-23=0
 cycles		.req r8 @also VDIC flags
 m6502_pc	.req r9
 globalptr	.req r10 @=wram_globals* ptr
 m6502_optbl	.req r10
 cpu_zpage	.req r11 @=CPU_RAM
 addy		.req r12 @keep this at r12 (scratch for APCS)
		@r13=SP
		@r14=LR
		@r15=PC
 zero_byte	.req r5

@----------------------------------------------------------------------------

@ start_map 0,cpu_zpage
@ _m_ nes_ram,0x800
@ _m_ nes_sram,0x2000
@ _m_ chr_decode,0x400

@everything in wram_globals* areas:

 start_map 0,globalptr

	@6502.s  (wram_globals0)

@negative area (before op table)
 _m_ ,-(PRG_BANK_COUNT * 3 + 2 + 1)  *4
 _m_ vramaddr,4

.if PRG_BANK_SIZE == 4
 _m_ writemem_tbl_base,0
 _m_ writemem_0_,4
 _m_ writemem_F,4
 _m_ writemem_E,4
 _m_ writemem_D,4
 _m_ writemem_C,4
 _m_ writemem_B,4
 _m_ writemem_A,4
 _m_ writemem_9,4
 _m_ writemem_8,4
 _m_ writemem_7,4
 _m_ writemem_6,4
 _m_ writemem_5,4
 _m_ writemem_4,4
 _m_ writemem_3,4
 _m_ writemem_2,4
 _m_ writemem_1,4
 _m_ writemem_0,0
 _m_ writemem_tbl,4

 _m_ memmap_tbl,0
 _m_ memmap_0,4
 _m_ memmap_1,4
 _m_ memmap_2,4
 _m_ memmap_3,4
 _m_ memmap_4,4
 _m_ memmap_5,4
 _m_ memmap_6,4
 _m_ memmap_7,4
 _m_ memmap_8,4
 _m_ memmap_9,4
 _m_ memmap_A,4
 _m_ memmap_B,4
 _m_ memmap_C,4
 _m_ memmap_D,4
 _m_ memmap_E,4
 _m_ memmap_F,4

 _m_ readmem_tbl_base,0
 _m_ readmem_0_,4
 _m_ readmem_F,4
 _m_ readmem_E,4
 _m_ readmem_D,4
 _m_ readmem_C,4
 _m_ readmem_B,4
 _m_ readmem_A,4
 _m_ readmem_9,4
 _m_ readmem_8,4
 _m_ readmem_7,4
 _m_ readmem_6,4
 _m_ readmem_5,4
 _m_ readmem_4,4
 _m_ readmem_3,4
 _m_ readmem_2,4
 _m_ readmem_1,4
 _m_ readmem_0,0
 _m_ readmem_tbl,4

.endif

.if PRG_BANK_SIZE == 8

 _m_ writemem_tbl_base,0
 _m_ writemem_0_,4
 _m_ writemem_E,4
 _m_ writemem_C,4
 _m_ writemem_A,4
 _m_ writemem_8,4
 _m_ writemem_6,4
 _m_ writemem_4,4
 _m_ writemem_2,4
 _m_ writemem_0,0
 _m_ writemem_tbl,4

 _m_ memmap_tbl,0
 _m_ memmap_0,4
 _m_ memmap_2,4
 _m_ memmap_4,4
 _m_ memmap_6,4
 _m_ memmap_8,4
 _m_ memmap_A,4
 _m_ memmap_C,4
 _m_ memmap_E,4

 _m_ readmem_tbl_base,0
 _m_ readmem_0_,4
 _m_ readmem_E,4
 _m_ readmem_C,4
 _m_ readmem_A,4
 _m_ readmem_8,4
 _m_ readmem_6,4
 _m_ readmem_4,4
 _m_ readmem_2,4
 _m_ readmem_0,0
 _m_ readmem_tbl,4

.endif

@positive area
 _m_ opz,256*4

@###begin cpustate
 _m_ cpuregs,7*4
 _m_ m6502_s,4

 _m_ wantirq,1
 _m_ ,3
@###end cpustate

 _m_ frame,4
@ _m_ scanline,4
 _m_ lastbank,4
 _m_ nexttimeout,4    @not used anymore
@ _m_ line_end_timeout,4
@ _m_ line_mid_timeout,4
@ _m_ scanlinehook,4
@ _m_ midlinehook,4
@ _m_ cyclesperscanline1,4
@ _m_ cyclesperscanline2,4
@ _m_ lastscanline,4

			@ppu.s (wram_globals1)
 _m_ fpsvalue,4
 _m_ AGBjoypad,4
 _m_ NESjoypad,4

@###begin ppustate
 _m_ vramaddr_dummy,4
 _m_ loopy_t,4
 _m_ scrollX,4
 _m_ scrollY,4
 _m_ scrollY_v,4 
 _m_ readtemp,1
 _m_ oam_addr,1
 _m_ ,1
 _m_ ,1

 _m_ ,1
 _m_ vramaddrinc,1
 _m_ ,1 @was ppustat, used for savestates
 _m_ toggle,1
 _m_ ppuctrl0,1
 _m_ ppuctrl0frame,1
 _m_ ppuctrl1,1
 _m_ ppuoamadr,1
 _m_ scrollX_v,4
@###end ppustate

 _m_ scrollXold,4
 _m_ scrollXline,1
 _m_ scrollXline_previous,1
 _m_ scrollXline_previous2,1
 _m_ scrollXline_previous3,1
 _m_ scrollYold,4
 _m_ scrollYline,1
 _m_ scrollYline_previous,1
 _m_ scrollYline_previous2,1
 _m_ scrollYline_previous3,1

 _m_ scroll_y_timestamp,4

 _m_ y_in_sprite0,1
 _m_ PAL60,1
 _m_ novblankwait_,1
 _m_ windowtop,1

 _m_ adjustblend,1
 _m_ okay_to_run_nes_chr_update_this_frame,1
 _m_ has_vram,1
 _m_ bankable_vrom,1

 _m_ vram_page_mask,1
 _m_ vram_page_base,1
 _m_ windowtop_scaled6_8,1
 _m_ windowtop_scaled7_8,1

	.if DIRTYTILES
 _m_ recent_tiles,4
 _m_ dmarecent_tiles,4
 _m_ recent_tilenum,4
	_m_ dmarecent_tilenum,4
	.endif

	.if USE_BG_CACHE
 _m_ bg_cache_produce_cursor,4
 _m_ bg_cache_produce_base_consume_end,4
 _m_ bg_cache_produce_limit_consume_begin,4
 _m_ bg_cache_full,1
 _m_ bg_cache_updateok,1
 _m_ ,2  @padding
	.endif

			@cart.s (wram_globals2)
@###begin mapperstate
 _m_ mapperdata,32
 _m_ ,3
 _m_ bank6,1
 _m_ bank8,1
 _m_ bankA,1
 _m_ bankC,1
 _m_ bankE,1
 _m_ nes_chr_map,8
@###end mapperstate
 _m_ agb_bg_map,16
 _m_ agb_bg_map_requested,16
 _m_ agb_real_bg_map,16
 _m_ bg_recent,4

 _m_ rombase,4
 _m_ romnumber,4
 _m_ emuflags,4
 _m_ BGmirror,4

 _m_ rommask,4
 _m_ vrombase,4
 _m_ vrommask,4

 _m_ instant_prg_banks,4
 _m_ instant_chr_banks,4

 _m_ cartflags,1
 _m_ hackflags,1   @not used anymore
 _m_ hackflags2,1   @not used anymore
 _m_ mapper_number,1

 _m_ rompages,1
 _m_ vrompages,1
 _m_ fourscreen,1
 _m_ ,1

 _m_ chrold,4

_m_ chrline,1
_m_ chrline_previous,1
_m_ chrline_previous2,1
_m_ chrline_previous3,1

 _m_ twitch,1
 _m_ flicker,1
_m_ ,10 @padding

@############################

	@sound.s (wram_globals3)

 _m_ reg_4000,1
 _m_ reg_4001,1
 _m_ reg_4002,1
 _m_ reg_4003,1
 _m_ reg_4004,1
 _m_ reg_4005,1
 _m_ reg_4006,1
 _m_ reg_4007,1
 _m_ reg_4008,1
 _m_ ,1
 _m_ reg_400A,1
 _m_ reg_400B,1
 _m_ reg_400C,1
 _m_ ,1
 _m_ reg_400E,1
 _m_ reg_400F,1

 _m_ frame_sequencer_step,1
 _m_ sq1_length,1
 _m_ reg_4001_written,1
 _m_ reg_4003_written,1

 _m_ frame_mode,1 
 _m_ sq2_length,1
 _m_ reg_4005_written,1
 _m_ reg_4007_written,1

 _m_ tri_linear,1
 _m_ tri_length,1
 _m_ ,1
 _m_ reg_400B_written,1

 _m_ channel_enables,1
 _m_ noise_length,1
 _m_ ,1
 _m_ reg_400F_written,1

 _m_ sq1_env,1
 _m_ sq1_env_delay,1
 _m_ sq1_sweep_delay,1
 _m_ sq1_neg_offset,1

 _m_ sq2_env,1
 _m_ sq2_env_delay,1
 _m_ sq2_sweep_delay,1
 _m_ sq2_neg_offset,1

 _m_ noise_env,1
 _m_ noise_env_delay,1
 _m_ ,1
 _m_ ,1

 _m_ sq1_period,4
 _m_ sq2_period,4
 _m_ tri_period,4

 _m_ pcmctrl,4		@bit7=irqen, bit6=loop.  bit 12=PCM enable (from $4015). bits 8-15=old $4015
 _m_ pcmlength,4		@total bytes
 _m_ pcmcount,4		@bytes remaining
 _m_ pcmlevel,4

 _m_ pcmstart,4		@starting addr
 _m_ pcmcurrentaddr,4	@current addr

 _m_ gba_sound1cnt_x,4
 _m_ gba_sound2cnt_h,4
 _m_ gba_timer_2,4
 _m_ gba_timer_3,4
 _m_ gba_sound4cnt_h,4

	_m_ freq_mult,4
 _m_ frame_sequencer_interval,4

_m_ dmc_last_timestamp,4
_m_ dmc_tick_counter,4
_m_ dmc_period,4
_m_ dmc_period_reciprocal,4

_m_ dmc_length_counter,4

_m_ dmc_remaining_bits,1
_m_ dmc_extra_byte,1
_m_ ,2
_m_ ,8


@############################


	@io.s (wram_globals4)
 _m_ sending,4
 _m_ lastsent,4
 _m_ received0,4
 _m_ received1,4
 _m_ received2,4
 _m_ received3,4

 _m_ joycfg,4
 _m_ joy0state,1
 _m_ joy1state,1
 _m_ joy2state,1
 _m_ joy3state,1
 _m_ joy0serial,4
 _m_ joy1serial,4
 _m_ nrplayers,1
 _m_ joystrobe,1
 _m_ ,2

	@ppu.s (wram_globals5)

 _m_ nesoamdirty,1
 _m_ consumetiles_ok,1
 _m_ frameready,1
	_m_ firstframeready,1

 _m_ vram_write_tbl,16*4
 _m_ vram_map,8*4
 _m_ nes_nt0,4
 _m_ nes_nt1,4
 _m_ nes_nt2,4
 _m_ nes_nt3,4
 _m_ ,4*4
@ _m_ agb_nt_map,4
@ _m_ agb_nt0,4
@ _m_ agb_nt1,4
@ _m_ agb_nt2,4
@ _m_ agb_nt3,4

	_m_ nes_palette,32

	_m_ scrollbuff,4
	_m_ dmascrollbuff,4
	_m_ nesoambuff,4
	_m_ dmanesoambuff,4
	_m_ bg0cntbuff,4
	_m_ dmabg0cntbuff,4
	_m_ dispcntbuff,4
	_m_ dmadispcntbuff,4
	
	_m_ dma0buff,4
	_m_ dma1buff,4
	_m_ dma3buff,4
	_m_ nes_vram,4

	_m_ bankbuffer_last,4*2
	_m_ bankbuffer,4
	_m_ dmabankbuffer,4
	_m_ bankbuffer_line,1
	_m_ bankbuffer_line_previous,1
	_m_ bankbuffer_line_previous2,1
	_m_ bankbuffer_line_previous3,1

	_m_ ctrl1old,4
	_m_ ctrl1line,1
	_m_ ctrl1line_previous,1
	_m_ ctrl1line_previous2,1
	_m_ ctrl1line_previous3,1

 _m_ stat_r_simple_func,4
 _m_ nextnesoambuff,4

 _m_ screen_off,1
 _m_ inside_vblank,1	@set to 0 at prerender scanline, set to 1 after last visible scanline (but before vblank interrupt)
 _m_ timestamp_mult_2,1
 _m_ suppress_vbl,1

 _m_ inside_gba_vblank,1
 _m_ okay_to_run_hdma,1
 _m_ palette_number,1
 _m_ palette_dirty,1
 
 _m_ visible_palette,4
 _m_ current_palette,4
 _m_ displayed_palette_scanlines,4
 _m_ displayed_palette_pointer,4

@timeout.s  (wram_globals6)

 _m_ cycles_to_run,4
 _m_ timestamp,4
 _m_ frame_timestamp,4
 _m_ next_frame_timestamp,4

 _m_ firsttimeout,4
 _m_ queue_dummy_timeout,4
 _m_ queue_dummy_timestamp,4
 _m_ queue_dummy_next,4
 _m_ prerender_line_timeout,4
 _m_ prerender_line_timestamp,4
 _m_ prerender_line_next,4
 _m_ line_zero_timeout,4
 _m_ line_zero_timestamp,4
 _m_ line_zero_next,4
 _m_ render_end_timeout,4
 _m_ render_end_timestamp,4
 _m_ render_end_next,4
 _m_ vblank_timeout,4
 _m_ vblank_timestamp,4
 _m_ vblank_next,4
 _m_ sprite_zero_timeout,4
 _m_ sprite_zero_timestamp,4
 _m_ sprite_zero_next,4
 _m_ fine_x_timeout,4
 _m_ fine_x_timestamp,4
 _m_ fine_x_next,4
 _m_ mapper_timeout,4
 _m_ mapper_timestamp,4
 _m_ mapper_next,4
 _m_ mapper_irq_timeout,4
 _m_ mapper_irq_timestamp,4
 _m_ mapper_irq_next,4
 _m_ dmc_timeout,4
 _m_ dmc_timestamp,4
 _m_ dmc_next,4
 _m_ frame_irq_timeout,4
 _m_ frame_irq_timestamp,4
 _m_ frame_irq_next,4
 _m_ half_screen_timeout,4
 _m_ half_screen_timestamp,4
 _m_ half_screen_next,4
 _m_ nmi_timeout,4
 _m_ nmi_timestamp,4
 _m_ nmi_next,4
 _m_ frame_sequencer_timeout,4
 _m_ frame_sequencer_timestamp,4
 _m_ frame_sequencer_next,4

 _m_ TIMEOUT_END,0

 _m_ screen_off_hook1,4
 _m_ screen_off_hook2,4
 _m_ screen_on_hook1,4
 _m_ screen_on_hook2,4
 _m_ ntsc_pal_reset_hook,4

 _m_ pcmirqbakup,4
 _m_ pcmirqcount,4

 _m_ cyclesperframe,4
 _m_ cyclesperscanline,4
 _m_ timestamp_div,4
 _m_ scroll_threshhold_value,4
 _m_ vblank_time,4
 _m_ render_end_time,4
 _m_ half_screen_time,4
 _m_ line_zero_start_time,4

 _m_ frame_timestamp_minus_96,4
 _m_ timestamp_mult,4
 _m_ frame_timestamp_plus_245,4
 _m_ frame_timestamp_plus_128,4

 _m_ _dontstop,1
 _m_ crash_framecounter,1
 _m_ crash_disabled,1
 _m_ ,1

	.if DEBUG
 _m_ fetch_function,4
	.endif


@speedhack_asm.s  (wram_globals7)

 _m_ speedhack_pc,4
 _m_ speedhack_pc2,4
 _m_ speedhacknumber,1
 _m_ ,3
 _m_ deadbeef,4

@----------------------------------------------------------------------------
 IRQ_VECTOR		= 0xfffe @ IRQ/BRK interrupt vector address
 RES_VECTOR		= 0xfffc @ RESET interrupt vector address
 NMI_VECTOR		= 0xfffa @ NMI interrupt vector address
@-----------------------------------------------------------wantirq
 IRQ_MAPPER		= 0x02 @Mapper is requesting an IRQ
 IRQ_APU			= 0x40 @APU frame sequencer is requesting an IRQ
 IRQ_DMC			= 0x80 @DMC is requesting an IRQ
@-----------------------------------------------------------cartflags
 MIRROR			= 0x01 @horizontal mirroring
 SRAM			= 0x02 @save SRAM
 TRAINER			= 0x04 @trainer present
 SCREEN4			= 0x08 @4way screen layout
 VS				= 0x10 @VS unisystem
@-----------------------------------------------------------hackflags
 NoHacks			= 0x00
 FindingHacks	= 0x01
 BplHack			= 0x10
 BneHack			= 0xD0
 BeqHack			= 0xF0
@-----------------------------------------------------------emuflags
 USEPPUHACK		= 1	@use $2002 hack (obsolete)
 NOCPUHACK		= 2	@don't use JMP hack (obsolete)
 PALTIMING		= 4	@0=NTSC 1=PAL
 FPS50			= 8 @0=60FPS, 1=50FPS
 DENDY          = 16 @0=NTSC/PAL, 1=Dendy
 FOLLOWMEM		= 32  @0=follow sprite, 1=follow mem
@ ?				= 64
@ ?				= 128

				@bits 8-15=scale type

 UNSCALED_NOAUTO	= 0	@display types
 UNSCALED_AUTO	= 1
 SCALED			= 2
 SCALED_SPRITES	= 3

				@bits 16-31=sprite follow val

@----------------------------------------------------------------------------
 CYC_SHIFT		= 8
 CYCLE			= 1<<CYC_SHIFT @one cycle (341*CYCLE cycles per scanline)

@cycle flags- (stored in cycles reg for speed)

 CYC_C			= 0x01	@Carry bit
 BRANCH			= 0x02	@branch instruction encountered
 CYC_I			= 0x04	@IRQ mask
 CYC_D			= 0x08	@Decimal bit
 CYC_V			= 0x40	@Overflow bit
 CYC_MASK		= CYCLE-1	@Mask
@----------------------------------------------------------------------------
 YSTART			= 16 @scaled NES screen starts on this line
		@.end

#endif
