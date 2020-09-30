#include "equates.h"

@ SCREEN_HEIGHT = 160
@ SCREEN_LEFT = 8
@ BG_VRAM = AGB_VRAM
@ SPR_VRAM_ADD = 0x10000
@ SPR_VRAM = AGB_VRAM+SPR_VRAM_ADD

@TODO:
@	Hack for sprite priority if it detects NO$GBA
@	Real Punchout Mode (using windowing)

@	.include "equates.h"
@	.include "memory.h"
@	.include "cart.h"
@	.include "io.h"
@	.include "6502.h"
@	.include "timeout.h"
@	.include "speedhack_asm.h"
@	.include "sound.h"
@	.include "mappers.h"
@	.include "link.h"

	@IMPORT crash
	
	.global _nextx
	.global _scrollX
	
	global_func remap_pal  @for build logs
	.if USE_BG_CACHE
	global_func display_bg
	.endif
	
@	global_func scroll_threshhold_mod
	
	.global DMA0BUFF
	.global DMA3BUFF
	.global DMA1BUFF
	.global DISPCNTBUFF1
	.global DISPCNTBUFF2
	.global SCROLLBUFF1
	.global SCROLLBUFF2
	.global BG0CNTBUFF1
	.global BG0CNTBUFF2
	.global _vramaddr
	.global _vramaddr_dummy

	.if DIRTYTILES
	.global dirty_rows
	.global dirty_tiles
	.endif
	
	.if MIXED_VRAM_VROM
	.global VRAM_chr3
	.endif
	
@	.global nesoambuff
	.global MAPPED_RGB
	
@	global_func findsprite0
	
	.global spr_cache_map
	.global spr_cache
	.global spr_cache_disp
	
	.if USE_BG_CACHE
	.global _bg_cache_full
	.endif
	
	.global ppustat_
	.global ppustat_savestate
@	global_func update_Y_hit
	global_func stat_R_simple
	global_func stat_R_clearvbl
@	global_func stat_R_sameline

	.global _ppuctrl0frame
	
	@IMPORT recache_sprites
	@IMPORT sprite_cache_size

	.global NESOAMBUFF1
	.global NESOAMBUFF2
	
	.global BANKBUFFER1
	.global BANKBUFFER2
	
	.global _dmabankbuffer
	.global _dmanesoambuff
	
	global_func update_bankbuffer

	.global _nesoambuff
	.global _dmanesoambuff
	
	.global _has_vram
	.global _bankable_vrom
	.global _vram_page_mask
	.global _vram_page_base
	

	.if DIRTYTILES
	global_func nes_chr_update
	.global _recent_tiles
	.global _dmarecent_tiles
	.global _recent_tilenum
	.global _dmarecent_tilenum
	.endif
	global_func newframe_nes_vblank
	global_func dma_W
@	global_func PPU_init
	global_func PPU_reset
	global_func PPU_R
	global_func PPU_W
@	.global agb_nt_map
	global_func VRAM_name0
	global_func VRAM_name1
	global_func VRAM_name2
	global_func VRAM_name3

	.global _vram_map
	.global _vram_write_tbl
	global_func vram_write_direct
@	global_func vram_read_direct
	global_func VRAM_chr
@	global_func debug_
	.global AGBinput
	.global EMUinput
	global_func paletteinit
	global_func PaletteTxAll
	global_func Update_Palette
	global_func newframe
	.global ppustate
	global_func writeBG
	.global wtop
	.global gammavalue
	global_func ctrl0_W
	global_func ctrl1_W
	global_func newX
	global_func newY
	global_func newframe_set0
	.global fpsenabled
	.global FPSValue
@	global_func vbldummy
@	.global vblankfptr
	global_func vblankinterrupt
	.global NES_VRAM
	.global _scrollbuff
	.global _dmascrollbuff
	.global _bg0cntbuff
	
	.global _PAL60
	.global novblankwait
	
@	global_func stat_R_ppuhack
	.global PPU_read_tbl

	global_func build_chr_decode
	
	.global _nes_palette

	.global _frameready
	.global _firstframeready
	
	global_func turn_screen_on
	global_func turn_screen_off
	global_func catchups
	
	global_func newX2
	global_func ctrl1_W_force
	
	global_func sprite_zero_handler_2
	global_func sprite_zero_handler_3
	global_func fine_x_handler
	
	.global _okay_to_run_hdma

	.global _scrollbuff
	.global _dmascrollbuff
	.global _nesoambuff
	.global _dmanesoambuff
	.global _bg0cntbuff
	.global _dmabg0cntbuff
	.global _dispcntbuff
	.global _dmadispcntbuff

	.global _dma0buff
	.global _dma1buff
	.global _dma3buff
	.global _nes_vram
	
	global_func hblankinterrupt
	global_func vcountinterrupt
	global_func set_bg_cache_available
	global_func vram_write_modify
	global_func vramaddrinc_modify1
	global_func vramaddrinc_modify2
	global_func set_bg_cache_produce_limit_consume_begin
	global_func UpdateXYScroll_V_equals_T

 .align
 .pool
 .text
 .align
 .pool

nes_rgb:
@	.byte 0x6E,0x6E,0x6E, 0x27,0x19,0xA6, 0x00,0x07,0xA1, 0x44,0x00,0x96, 0xA1,0x00,0x86, 0xB2,0x00,0x28, 0xC1,0x06,0x00, 0x8C,0x17,0x00
@	.byte 0x5C,0x41,0x00, 0x10,0x47,0x00, 0x05,0x4C,0x00, 0x00,0x45,0x2E, 0x16,0x51,0x5B, 0x00,0x00,0x00, 0x21,0x21,0x21, 0x04,0x04,0x04
@	.byte 0xBF,0xBF,0xBF, 0x00,0x94,0xF7, 0x39,0x43,0xE8, 0x7D,0x16,0xF3, 0xDE,0x07,0xC9, 0xF1,0x1E,0x65, 0xE8,0x31,0x21, 0xD6,0x64,0x00
@	.byte 0xA3,0x81,0x00, 0x40,0x80,0x00, 0x05,0x8F,0x00, 0x00,0x8A,0x55, 0x05,0xA2,0xAA, 0x35,0x35,0x35, 0x09,0x09,0x09, 0x09,0x09,0x09
@	.byte 0xFF,0xFF,0xFF, 0x2F,0xD7,0xFF, 0x89,0x9E,0xF8, 0xB4,0x74,0xFB, 0xFF,0x52,0xF3, 0xFC,0x61,0x8B, 0xF7,0x7A,0x60, 0xFF,0x90,0x3D
@	.byte 0xFA,0xBC,0x2F, 0x9F,0xE3,0x26, 0x2B,0xED,0x35, 0x3C,0xE3,0x9A, 0x06,0xDB,0xE3, 0x7E,0x7E,0x7E, 0x0D,0x0D,0x0D, 0x0D,0x0D,0x0D
@	.byte 0xFF,0xFF,0xFF, 0xA6,0xE2,0xFF, 0xC3,0xD2,0xFF, 0xD2,0xAB,0xFF, 0xFF,0xA8,0xF9, 0xFF,0xB1,0xC4, 0xFF,0xBF,0xB7, 0xFF,0xE7,0xA6
@	.byte 0xFF,0xF7,0x9C, 0xD7,0xFC,0x95, 0xA6,0xFE,0xAF, 0xA2,0xF2,0xDA, 0x99,0xF7,0xFF, 0xCD,0xCD,0xCD, 0x11,0x11,0x11, 0x11,0x11,0x11

	.byte 117,117,117, 39,27,143, 0,0,171, 71,0,159, 143,0,119, 171,0,19, 167,0,0, 127,11,0
	.byte 67,47,0, 0,71,0, 0,81,0, 0,63,23, 27,63,95, 0,0,0, 0,0,0, 0,0,0
	.byte 188,188,188, 0,115,239, 35,59,239, 131,0,243, 191,0,191, 231,0,91, 219,43,0, 203,79,15
	.byte 139,115,0, 0,151,0, 0,171,0, 0,147,59, 0,131,139, 0,0,0, 0,0,0, 0,0,0
	.byte 255,255,255, 63,191,255, 95,151,255, 167,139,253, 247,123,255, 255,119,183, 255,119,99, 255,155,59
	.byte 243,191,63, 131,211,19, 79,223,75, 88,248,152, 0,235,219, 102,102,102, 0,0,0, 0,0,0
	.byte 255,255,255, 171,231,255, 199,215,255, 215,203,255, 255,199,255, 255,199,219,255, 191,179,255, 219,171
	.byte 255,231,163, 227,255,163, 171,243,191, 179,255,207, 159,255,243, 209,209,209, 0,0,0, 0,0,0

vs_palmaps:
@freedomforce/gradius/hoogansalley/pinball/platoon
	.byte 0x35,0x3f,0x16,0x22,0x1c,0x09,0x30,0x15,0x30,0x00,0x27,0x05,0x04,0x28,0x08,0x30
	.byte 0x21,0x3f,0x3f,0x3f,0x3c,0x32,0x36,0x12,0x3f,0x2b,0x3f,0x3f,0x3f,0x3f,0x24,0x01
	.byte 0x3f,0x31,0x3f,0x2a,0x2c,0x0c,0x3f,0x14,0x3f,0x07,0x34,0x06,0x3f,0x02,0x26,0x0f
	.byte 0x3f,0x19,0x10,0x0a,0x3f,0x3f,0x37,0x17,0x3f,0x11,0x1a,0x3f,0x3f,0x25,0x18,0x3f
@castlevania/golf/machrider/slalom
	.byte 0x0f,0x27,0x18,0x3f,0x3f,0x25,0x3f,0x34,0x16,0x13,0x3f,0x34,0x20,0x23,0x3f,0x0b
	.byte 0x3f,0x23,0x06,0x3f,0x1b,0x27,0x3f,0x22,0x3f,0x24,0x3f,0x3f,0x32,0x08,0x3f,0x03
	.byte 0x3f,0x37,0x26,0x33,0x11,0x3f,0x10,0x22,0x14,0x3f,0x00,0x09,0x12,0x0f,0x3f,0x30
	.byte 0x3f,0x3f,0x2a,0x17,0x0c,0x01,0x15,0x19,0x3f,0x2c,0x07,0x37,0x3f,0x05,0x3f,0x3f
@excitebike/excitebike-alt (probably not complete yet)
	.byte 0x3f,0x3f,0x1c,0x3f,0x1a,0x30,0x01,0x07,0x02,0x3f,0x3f,0x30,0x3f,0x3f,0x3f,0x30
	.byte 0x32,0x1c,0x11,0x12,0x3f,0x18,0x17,0x26,0x0c,0x3f,0x3f,0x02,0x16,0x3f,0x3f,0x21
	.byte 0x3f,0x3f,0x0f,0x37,0x3f,0x28,0x27,0x3f,0x29,0x3f,0x21,0x3f,0x11,0x3f,0x0f,0x3f
	.byte 0x31,0x3f,0x3f,0x06,0x0f,0x2a,0x30,0x3f,0x3f,0x28,0x3f,0x3f,0x13,0x3f,0x3f,0x3f
@battlecity/clucluland/iceclimber/smb/starluster/topgun?
	.byte 0x18,0x3f,0x1c,0x3f,0x3f,0x3f,0x01,0x17,0x10,0x3f,0x2a,0x3f,0x36,0x37,0x1a,0x39
	.byte 0x25,0x3f,0x12,0x3f,0x0f,0x3f,0x3f,0x26,0x3f,0x1b,0x22,0x19,0x04,0x0f,0x3a,0x21
	.byte 0x3f,0x0a,0x07,0x06,0x13,0x3f,0x00,0x15,0x0c,0x3f,0x11,0x3f,0x3f,0x38,0x3f,0x3f
	.byte 0x3f,0x30,0x07,0x16,0x3f,0x3b,0x30,0x3c,0x0f,0x27,0x3f,0x31,0x29,0x3f,0x11,0x09
@drmario/goonies/soccer
	.byte 0x0f,0x3f,0x3f,0x10,0x1a,0x30,0x31,0x3f,0x01,0x0f,0x36,0x3f,0x15,0x3f,0x3f,0x3c
	.byte 0x3f,0x3f,0x3f,0x12,0x19,0x18,0x17,0x3f,0x00,0x3f,0x3f,0x02,0x16,0x3f,0x3f,0x3f
	.byte 0x3f,0x3f,0x3f,0x37,0x3f,0x27,0x26,0x20,0x3f,0x04,0x22,0x3f,0x11,0x3f,0x3f,0x3f
	.byte 0x2c,0x3f,0x3f,0x3f,0x07,0x2a,0x28,0x3f,0x0a,0x3f,0x32,0x38,0x13,0x3f,0x3f,0x0c

@old palette data, removed because it wasn't actually used except for debug testing
@nes_rgb15:
@	.incbin "../src/nespal.bin"

@----------------------------------------------------------------------------
remap_pal:
@map_palette	@(for VS unisys)	r0-r2,r4-r7 modified
@----------------------------------------------------------------------------
	ldr r5,=nes_rgb
	ldr r6,=MAPPED_RGB
	mov r7,#64*3
	ldrb_ r0,cartflags
	tst r0,#VS
	beq nomap

	ldr_ r0,memmap_tbl+(PRG_BANK_COUNT-1)*4
	ldr r1,=NMI_VECTOR
	ldrb r1,[r0,r1]!
	ldrb r2,[r0,#1]!
	ldrb r4,[r0,#1]!
	ldrb r0,[r0,#1]
	orr r1,r1,r2,lsl#8
	orr r1,r1,r4,lsl#16
	orr r1,r1,r0,lsl#24

	adr r2,vslist
mp0:	ldr r0,[r2],#8
	cmp r0,r1			@find which rom...
	beq remap
	cmp r0,#0
	bne mp0
nomap:
	mov r0,r6
	mov r1,r5
	mov r2,r7
	b_long memcpy32
@	ldr r0,[r5],#4
@	str r0,[r6],#4
@	subs r7,r7,#4
@	bne nomap
@	mov pc,lr
remap:
	ldr r1,[r2,#-4]
	@r1 = source address inside vs_palmaps
	@r6 = destination address
	@r5 = nes_rgb palette
mp1:
	@copy 3 bytes at a time from nes_rgb to mapped_rgb using the vs_palmap to remap colors
	ldrb r2,[r1],#1
	add r2,r2,r2,lsl#1
	ldrb r0,[r2,r5]!
	ldrb addy,[r2,#1]
	orr r0,r0,addy,lsl#8
	strh r0,[r6],#2
	ldrb r0,[r2,#2]
	ldrb r2,[r1],#1
	add r2,r2,r2,lsl#1
	ldrb addy,[r2,r5]!
	orr r0,r0,addy,lsl#8
	strh r0,[r6],#2
	ldrb r0,[r2,#1]
	ldrb addy,[r2,#2]
	orr r0,r0,addy,lsl#8
	strh r0,[r6],#2
	subs r7,r7,#6
	bne mp1
	mov pc,lr

vslist:	.word 0xfff3f318,vs_palmaps+64*0 @Freedom Force	RP2C04-0001
	.word 0xf422f492,vs_palmaps+64*0 @Gradius				RP2C04-0001
	.word 0x8000809c,vs_palmaps+64*0 @Hoogans Alley		RP2C04-0001
	.word 0x80008281,vs_palmaps+64*0 @Pinball				RP2C04-0001
	.word 0xfff3fd92,vs_palmaps+64*0 @Platoon				RP2C04-0001
	.word 0x800080ce,vs_palmaps+64*1 @(lady)Golf			RP2C04-0002
	.word 0x80008053,vs_palmaps+64*1 @Mach Rider			RP2C04-0002
	.word 0xc008c062,vs_palmaps+64*1 @Castlevania			RP2C04-0002
	.word 0x8050812f,vs_palmaps+64*1 @Slalom				RP2C04-0002
	.word 0x85af863f,vs_palmaps+64*2 @Excitebike			RP2C04-0003
	.word 0x859a862a,vs_palmaps+64*2 @Excitebike(a1)		RP2C04-0003
	.word 0x8000810a,vs_palmaps+64*3 @Super Mario Bros	RP2C04-0004
	.word 0xb578b5de,vs_palmaps+64*3 @Ice Climber			RP2C04-0004
	.word 0xc298c325,vs_palmaps+64*3 @Clu Clu Land		RP2C04-0004
	.word 0x804c8336,vs_palmaps+64*3 @Star Luster			RP2C04-0004
	.word 0xc070d300,vs_palmaps+64*3 @Battle City			RP2C04-0004
	.word 0xc298c325,vs_palmaps+64*3 @Top Gun				RP2C04-0004?
	.word 0x800080ba,vs_palmaps+64*4 @Soccer
	.word 0xf007f0a5,vs_palmaps+64*4 @Goonies
	.word 0xff008005,vs_palmaps+64*4 @Dr. Mario
@	.word 0xf1b8f375,vs_palmaps+64*? @Super Sky Kid		doesn't need palette
@	.word 0xffdac0c4,vs_palmaps+64*? @TKO Boxing			doesn't start
@	.word 0xf958f88f,vs_palmaps+64*3 @Super Xevious		doesn't start
	.word 0, vs_palmaps+64*3 @prevent garbage palette for non-matched rom
@----------------------------------------------------------------------------
paletteinit:@	r0-r3 modified.
@called by ui.c:  void map_palette(char gammavalue)
@----------------------------------------------------------------------------
	stmfd sp!,{r4-r10,lr}
	ldr globalptr,=GLOBAL_PTR_BASE
	bl remap_pal
@	palette preview code - uncomment to see the old palette
@	ldr r8,=0x05000100
@	adr r6,nes_rgb15
@	mov r4,#64
@gloop0:
@	ldrh r0,[r6],#2
@	strh r0,[r8],#2
@	subs r4,r4,#1
@	bne gloop0

	ldr r6,=MAPPED_RGB
	mov r7,r6
	ldr r1,=gammavalue
	ldrb r1,[r1]	@gamma value = 0 -> 4
	mov r4,#64			@pce rgb, r1=R, r2=G, r3=B
gloop:					@map 0bbbbbgggggrrrrr  ->  0bbbbbgggggrrrrr
	ldrb r0,[r6],#1
	bl gammaconvert
	mov r5,r0

	ldrb r0,[r6],#1
	bl gammaconvert
	orr r5,r5,r0,lsl#5

	ldrb r0,[r6],#1
	bl gammaconvert
	orr r5,r5,r0,lsl#10

	strh r5,[r7],#2
@	palette preview code - uncomment to see the adjusted palette on the screen
@	strh r5,[r8],#2
	subs r4,r4,#1
	bne gloop

	ldmfd sp!,{r4-r10,lr}
	bx lr

@----------------------------------------------------------------------------
gammaconvert:@	takes value in r0(0-0xFF), gamma in r1(0-4),returns new value in r0=0x1F
@----------------------------------------------------------------------------
	rsb r2,r0,#0x100
	mul r3,r2,r2
	rsbs r2,r3,#0x10000
	rsb r3,r1,#4
	orr r0,r0,r0,lsl#8
	mul r2,r1,r2
	mla r0,r3,r0,r2
	mov r0,r0,lsr#13

	bx lr
@@----------------------------------------------------------------------------
@PaletteTxAll:
@@----------------------------------------------------------------------------
@	stmfd sp!,{r0-r4}
@
@	@monochrome mode stuff
@	ldr r4,=_ppuctrl1
@	ldrb r4,[r4]
@
@	mov r2,#0x1F
@pxall:
@	adr_ r1,nes_palette
@	ldrb r0,[r1,r2]	@load from nes palette
@	@monochrome test
@	tst r4,#1
@	andne r0,r0,#0x30
@
@	ldr r1,=MAPPED_RGB
@	add r0,r0,r0
@	ldrh r0,[r1,r0]	@lookup RGB
@	adr_ r1,agb_pal
@	mov r3,r2,lsl#1
@	strh r0,[r1,r3]	@store in agb palette
@	subs r2,r2,#1
@	bpl pxall
@	
@	ldmfd sp!,{r0-r4}
@	bx lr
@
@Update_Palette:
@	stmfd sp!,{r0-addy}
@	mov r8,#AGB_PALETTE		@palette transfer
@	adr_ addy,agb_pal
@up8:	ldmia addy!,{r0-r7}
@	stmia r8,{r0,r1}
@	add r8,r8,#32
@	stmia r8,{r2,r3}
@	add r8,r8,#32
@	stmia r8,{r4,r5}
@	add r8,r8,#32
@	stmia r8,{r6,r7}
@	add r8,r8,#0x1a0
@	tst r8,#0x200
@	bne up8			@(2nd pass: sprite pal)
@	ldmfd sp!,{r0-addy}
@	bx lr


build_chr_decode:
	mov r1,#0xffffff00		@build chr decode tbl
	ldr r2,=CHR_DECODE
ppi0:	mov r0,#0
	tst r1,#0x01
	orrne r0,r0,#0x10000000
	tst r1,#0x02
	orrne r0,r0,#0x01000000
	tst r1,#0x04
	orrne r0,r0,#0x00100000
	tst r1,#0x08
	orrne r0,r0,#0x00010000
	tst r1,#0x10
	orrne r0,r0,#0x00001000
	tst r1,#0x20
	orrne r0,r0,#0x00000100
	tst r1,#0x40
	orrne r0,r0,#0x00000010
	tst r1,#0x80
	orrne r0,r0,#0x00000001
	str r0,[r2],#4
	adds r1,r1,#1
	bne ppi0
	
	@store canary 1
	ldr r0,=0xDEADBEEF
	str r0,[r2]
	
	
	bx lr
	
	
	
	
@@----------------------------------------------------------------------------
@PPU_init:	@(called from main.c) only need to call once
@@----------------------------------------------------------------------------
@	stmfd sp!,{r10,lr}
@	ldr r10,=GLOBAL_PTR_BASE
@
@	bl build_chr_decode
@
@@	mov r1,#REG_BASE
@@	mov r0,#0x0008
@@	strh r0,[r1,#REG_DISPSTAT]	@vblank en
@@
@@	add r0,r1,#REG_BG0HOFS		@DMA0 always goes here
@@	str r0,[r1,#REG_DM0DAD]
@@	mov r0,#1					@1 word transfer
@@	strh r0,[r1,#REG_DM0CNT_L]
@@	ldr r0,=DMA0BUFF+4			@dmasrc=
@@	str r0,[r1,#REG_DM0SAD]
@@
@@	str r1,[r1,#REG_DM1DAD]		@DMA1 goes here
@	
@	.if CRASH
@	mov r0,#1
@	strb_ r0,crash_disabled
@	.endif
@
@@	add r2,r1,#REG_IE
@@	mov r0,#-1
@@	strh r0,[r2,#2]		@stop pending interrupts
@@	ldr r0,=0x10F1
@@	strh r0,[r2]		@key,vblank,timer1,timer2,timer3,serial interrupt enable
@@	mov r0,#1
@@	strh r0,[r2,#8]		@master irq enable
@@
@@	ldr r1,=AGB_IRQVECT
@@	ldr r2,=irqhandler
@@	str r2,[r1]
@
@	ldmfd sp!,{r10,lr}
@	bx lr
@----------------------------------------------------------------------------
PPU_reset:	@called with CPU reset
@----------------------------------------------------------------------------
	stmfd sp!,{globalptr,lr}
	ldr globalptr,=GLOBAL_PTR_BASE
	mov r0,#0
	strb_ r0,ppuctrl0	@NMI off
	strb_ r0,ppuctrl1	@screen off
	strb_ r0,inside_vblank

	@might get rid of these, throw them into the master reset code
	str_ r0,screen_off_hook1
	str_ r0,screen_off_hook2
	str_ r0,screen_on_hook1
	str_ r0,screen_on_hook2
	str_ r0,ntsc_pal_reset_hook
	
	ldr r1,=ppustat_
	strb r0,[r1]	@flags off
	ldr r2,=stat_R_simple
	ldr r1,=PPU_read_tbl+8
	str r2,[r1]
	str_ r2,stat_r_simple_func

	strb_ r0,windowtop

	@strb r0,toggle
	@mov r0,#1
	@strb r0,vramaddrinc

	mov r0,#1
	strb_ r0,screen_off

	@Set PPU 2005 write, 2006 write, 2007 read to Screen-Off version
	ldr r1,=PPU_read_tbl + 7*4
	ldr r0,=vmdata_R
	str r0,[r1]
	ldr r1,=PPU_write_tbl + 5*4
	ldr r0,=bgscroll_W_screen_off
	str r0,[r1],#4
	ldr r0,=vmaddr_W_screen_off
	str r0,[r1],#4

@	bl reset_buffers
	
	mov r1,#0x0440
	str_ r1,ctrl1old
	orr r1,r1,r1,lsl#16
	
	@clear dispcnt buffers
	ldr_ r0,dispcntbuff
	mov r2,#240*2
	bl memset32_
	ldr_ r0,dmadispcntbuff
	mov r2,#240*2
	bl memset32_
	ldr_ r0,dma1buff
	mov r2,#(SCREEN_HEIGHT)*2
@	mov r2,#(SCREEN_HEIGHT+4)*2
	bl memset32_

	mov r1,#0
@	ldr_ r0,nes_vram
@	mov r2,#0x3000
@	bl memset32_
	ldr r0,=NES_VRAM2
	mov r2,#0x800
	bl memset32_			@clear nes VRAM
@@@@might not want to do this for COMPY or GBAMP
@@@@
@	ldr r0,=NES_VRAM4
@	mov r2,#0x800
@	bl memset32_
@@@@

	.if DIRTYTILES
	@clear dirtytiles and dirtyrows
	ldr r0,=dirty_tiles
	mov r2,#512+32
	bl memset32_
	
	@clear RECENT_TILENUM
	@this is safe to do, since firstframeready was already set to 0 within loadcart.c
	mvn r1,#0
	ldr r0,=RECENT_TILENUM1
	mov r2,#(MAX_RECENT_TILES*2)+4
	bl memset32_
	ldr r0,=RECENT_TILENUM2
	mov r2,#(MAX_RECENT_TILES*2)+4
	bl memset32_
	.endif

	@ldr r0,=MEM_AGB_SCREEN	@clear AGB BG
	@mov r2,#32*32*2
	@bl memset32_

	mov r1,#0xe0		@was 0xe0
	mov r0,#AGB_OAM
	mov r2,#0x400
	bl memset32_			@no stray sprites please
	
	mov r1,#0xF0
	ldr r0,=NESOAMBUFF1
	str_ r0,nextnesoambuff
	mov r2,#0x100
	bl memset32_

	ldr r0,=NESOAMBUFF2
	str_ r0,nesoambuff
	str_ r0,dmanesoambuff
	mov r2,#0x100
	bl memset32_
	
	mvn r0,#0
	str_ r0,nes_chr_map
	str_ r0,nes_chr_map+4

	bl self_modify_reset

	@erase bankbuffer?
	@set buffer line numbers to zero?
	@erase all buffers?
	@erase GBA VRAM corresponding to graphics?
	@
	@
	bl paletteinit		@do palette mapping (for VS) & gamma
	
	
	
	ldmfd sp!,{globalptr,lr}
	bx lr

nop_instruction:
	nop

self_modify_reset:
	.if DIRTYTILES
	.if MIXED_VRAM_VROM
	ldrb_ r1,bankable_vrom
	ldrb_ r2,has_vram
	tst r1,r2
	
_ZZZ1 = (consume_recent_tiles_2_entry-_consume_recent_tiles_entry-8)/4
_ZZZ2 = (consume_recent_tiles_entry-_consume_recent_tiles_entry-8)/4
_ZZZ3 = (render_recent_tiles_2-_render_recent_tiles-8)/4
_ZZZ4 = (render_recent_tiles-_render_recent_tiles-8)/4
	
	ldrne r0,=_ZZZ1
	ldreq r0,=_ZZZ2
	ldr r1,=_consume_recent_tiles_entry
	strh r0,[r1,#_consume_recent_tiles_entry-_consume_recent_tiles_entry]

	ldrne r0,=_ZZZ3
	ldreq r0,=_ZZZ4
	strh r0,[r1,#_render_recent_tiles-_consume_recent_tiles_entry]
	
	.endif	
	.endif

	.if USE_BG_CACHE
	ldr r1,=writeBG_mapper_9_mod
	ldrb_ r0,mapper_number
	cmp r0,#9
	cmpne r0,#10
	
	mov r0,#0xEA000000  @B
	orr r0,r0,#(writeBG_mapper_9_checks-writeBG_mapper_9_mod)/4-2
	ldrne r0,nop_instruction
	str r0,[r1]
	.endif
	
	bx lr



@----------------------------------------------------------------------------
showfps_:		@fps output, r0-r2=used.
@----------------------------------------------------------------------------
	ldr r1,=fpschk
	ldrb r0,[r1]
	subs r0,r0,#1
	movmi r0,#59
	strb r0,[r1]
	bxpl lr					@End if not 60 frames has passed

	ldr r0,=fpsenabled
	ldrb r0,[r0]
	tst r0,#1
	bxeq lr					@End if not enabled

	ldr_ r0,fpsvalue
	cmp r0,#0
	bxeq lr					@End if fps==0, to keep it from appearing in the menu
	mov r1,#0
	str_ r1,fpsvalue
	
	@IMPORT ui_x
	ldr r2,=ui_x
	ldr r2,[r2]
	cmp r2,#256
	bxne lr					@UI must be to the right
	

	mov r1,#100
	swi 0x060000			@Division r0/r1, r0=result, r1=remainder.
	add r0,r0,#'0'
	ldr r2,=fpstext+5
	strb r0,[r2]
	mov r0,r1
	mov r1,#10
	swi 0x060000			@Division r0/r1, r0=result, r1=remainder.
	add r0,r0,#'0'
	ldr r2,=fpstext+6
	strb r0,[r2]
	add r1,r1,#'0'
	ldr r2,=fpstext+7
	strb r1,[r2]
	

	ldr r0,=fpstext
	ldr r2,=DEBUGSCREEN
	.if REDUCED_FONT
	ldr r12,=0x4000+298-32-10
	.else
	ldr r12,=0x4000+298-32
	.endif
db1:
	ldrb r1,[r0],#1
	.if REDUCED_FONT
	cmp r1,#' '
	addeq r1,r1,#'*'-' '
	.endif
	
	add r1,r1,r12
	strh r1,[r2],#2
	tst r2,#15
	bne db1

	bx lr
@@----------------------------------------------------------------------------
@debug_:		@debug output, r0=val, r1=line, r2=used.
@@----------------------------------------------------------------------------
@ .if DEBUG
@	ldr r2,=DEBUGSCREEN
@	add r2,r2,r1,lsl#6
@db0:
@	mov r0,r0,ror#28
@	and r1,r0,#0x0f
@	cmp r1,#9
@	addhi r1,r1,#7
@	add r1,r1,#0x30
@	orr r1,r1,#0x4100
@	strh r1,[r2],#2
@	tst r2,#15
@	bne db0
@ .endif
@	bx lr

	
	
	
	



	
@----------------------------------------------------------------------------
 .align
 .pool
 .section .iwram, "ax", %progbits
 .subsection 6
 .align
 .pool
@----------------------------------------------------------------------------
@this stuff can't be in rom!
fpstext: .ascii "FPS:    "
fpsenabled: .byte 0
fpschk:	.byte 0
gammavalue: .byte 0
		.byte 0

	.if DIRTYTILES

@ .align
@ .pool
@ .text
@ .align
@ .pool

@r0-r3 are used as temporary words for memory copying, do not need to be preserved on calls to flush_recent_tiles
 decodeptr	.req r2 @mem_chr_decode
 nesptr		.req r4 @chr src
 r_tiles 	.req r5
 r_tnum		.req r6
 d_tiles	.req r7 @dirtytiles
 d_rows		.req r8 @dirtyrows
 tilenum	.req r9
@ zero		.req r10 @0
 tilesleft	.req r11
 agbptr  	.req r7 @chr dst
 agbptr2 	.req r8 @chr dst2

	.pushsection .vram1, "ax", %progbits
@Store dirty tiles into a cache
nes_chr_update:
	@no chr ram? bye
	ldrb_ r0,has_vram
	movs r0,r0
	bxeq lr

	stmfd sp!,{r0-addy,lr}
	bl store_recent_tiles
	ldmfd sp!,{r0-addy,pc}
	
store_recent_tiles:
	@first - check if any tiles are dirty
	ldr d_rows,=dirty_rows
	ldmia d_rows,{r0-r7}
	orr r0,r0,r1
	orr r0,r0,r2
	orr r0,r0,r3
	orr r0,r0,r4
	orr r0,r0,r5
	orr r0,r0,r6
	orrs r0,r0,r7
	bxeq lr

	stmfd sp!,{lr}
	ldr d_tiles,=dirty_tiles
	ldr_ r_tiles,recent_tiles
	ldr_ r_tnum,recent_tilenum
	ldrh r0,[r_tnum]
	tst r0,#0x8000
	bleq_long flush_recent_tiles
	mov tilesleft,#MAX_RECENT_TILES

updatetiles:
	@coarse version on words
	ldr decodeptr,=CHR_DECODE
@	mov zero,#0
	mov tilenum,#0

	b_long updatetiles_loop
	.popsection

updatetiles_loop:
	cmp tilenum,#512
	bge_long updatetiles_done
	ldr r0,[d_rows,tilenum,lsr#4]
	movs r0,r0
	bne updatetiles_fine
	add tilenum,tilenum,#64
	b updatetiles_loop

updatetiles_fine:
	@fine - operates on bytes
	@jump here only from 64-aligned tilenum, r0=word from dirtyrows
	tst r0,#0x000000FF
	addeq tilenum,tilenum,#16
	tsteq r0,#0x0000FF00
	addeq tilenum,tilenum,#16
	tsteq r0,#0x00FF0000
	addeq tilenum,tilenum,#16

updaterow_loop:
	ldrb r0,[d_tiles,tilenum]
	movs r0,r0
	bne store_recent_tile
	add tilenum,tilenum,#1
backto_updaterow_loop:
	tst tilenum,#0x0F
	bne updaterow_loop
updatetiles_resume:
	tst tilenum,#63
	beq updatetiles_loop
updatetiles_fine2:
	@byte aligned version of updatetiles, jumps back once aligned
	ldrb r0,[d_rows,tilenum,lsr#4]
	movs r0,r0
	bne updaterow_loop
	add tilenum,tilenum,#16
	b updatetiles_resume

store_recent_tile:
	ldr_ nesptr,nes_vram
	add nesptr,nesptr,tilenum,lsl#4
storetileloop:
	subs tilesleft,tilesleft,#1
	blmi flush_recent_tiles
	ldmia nesptr!,{r0-r3}
	stmia r_tiles!,{r0-r3}
	strh tilenum,[r_tnum],#2
	mov r0,#0
	strb r0,[d_tiles,tilenum]
	add tilenum,tilenum,#1
	ldrb r0,[d_tiles,tilenum]
	movs r0,r0
	bne storetileloop
	b backto_updaterow_loop

	.pushsection .text, "ax", %progbits
updatetiles_done:
	mov r0,#0x8000
	strh r0,[r_tnum]
	mov r0,#0
	mov r1,#0
	mov r2,#0
	mov r3,#0
	stmia d_rows!,{r0-r3}
	stmia d_rows!,{r0-r3}
	ldmfd sp!,{pc}
	.popsection

consume_recent_tiles:
	ldr_ r0,nesoamdirty  @4 bytes: nesoamdirty, consumetiles_ok, frameready, firstframeready
	tst r0,#0xFF00
	tsteq r0,#0xFF0000
	bxeq lr
	
_consume_recent_tiles_entry:	b consume_recent_tiles_entry
_render_recent_tiles:	b render_recent_tiles

flush_recent_tiles:
	stmfd sp!,{r4,r7-r9,lr}
	bl _consume_recent_tiles_entry
	ldr_ r_tiles,recent_tiles
	ldr_ r_tnum,recent_tilenum
	stmfd sp!,{r5,r6}
	bl _render_recent_tiles
	mov tilesleft,#MAX_RECENT_TILES-1
	ldmfd sp!,{r5,r6}
	mov r0,#0x8000
	strh r0,[r_tnum]
	ldmfd sp!,{r4,r7-r9,pc}

consume_recent_tiles_entry:
	ldr decodeptr,=CHR_DECODE
	ldr_ r_tiles,dmarecent_tiles
	ldr_ r_tnum,dmarecent_tilenum
	mov r0,#0x8000
	ldrh tilenum,[r_tnum]
	strh r0,[r_tnum],#2
	mov r4,r5
	stmfd sp!,{r10,lr}
	b render_recent_tiles_loop
	
render_recent_tiles:
	mov r4,r5
	@want to count number of consecutive tiles which would be rendered
	stmfd sp!,{r10,lr}
	ldr decodeptr,=CHR_DECODE
	ldrh tilenum,[r_tnum],#2
render_recent_tiles_loop:
	tst tilenum,#0x8000
	ldmnefd sp!,{r10,pc}
	
	@r2 = nes ptr
	mov r3,tilenum
	ldr agbptr,=BG_VRAM
	add agbptr,agbptr,tilenum,lsl#5
	add agbptr2,agbptr,#(SPR_VRAM-BG_VRAM)+SPR_CACHE_OFFSET @change this to sprite memory
	tst tilenum,#0x100
	addne agbptr,agbptr,#0x2000
0:
	add r0,tilenum,#1
	ldrh tilenum,[r_tnum],#2
	cmp r0,#0x100
	beq 1f
	cmp r0,tilenum
	beq 0b
1:
	@r3 = original tilenum, r0 = highest consecutive tilenum
	sub r3,r0,r3
	adr lr,render_recent_tiles_loop
	@destroys r0,r1,r3,r4,r9,r10,r11,r12
	b render_tiles_2
	
	.if MIXED_VRAM_VROM
consume_recent_tiles_2_entry:
	ldr_ r_tiles,dmarecent_tiles
	ldr_ r_tnum,dmarecent_tilenum
	stmfd sp!,{r_tnum,lr}
	bl render_recent_tiles_2
	ldmfd sp!,{r_tnum,lr}
	mov r0,#0x8000
	strh r0,[r_tnum]
	bx lr
	
	
render_recent_tiles_2:
	ldrh tilenum,[r_tnum]
	tst tilenum,#0x8000
	bxne lr
	stmfd sp!,{r10,r11,r12,lr}
	sub sp,sp,#14*4
	
0:
	bl_long render_a_kilobyte
	tst tilenum,#0x8000
	beq 0b

	add sp,sp,#14*4
	ldmfd sp!,{r10,r11,r12,lr}
	bx lr
	
 .if 0
We: use a list of base addresses, because in the worst case, there could be 14 copies of the same vram page
base_addr_list[14]:
base_addr_cursor=0:

banknumber: is not a vram page number, but a "mapper" bank number
kb: is a kilobyte number from the VRAM address

kb=tilenum/1024:
banknumber=vram_page_base+kb:
sprite_page_number=spr_cache_map[banknumber]:
if: sprite_page_number is positive
	base_addr=AGB_VRAM+0x10000+SPR_CACHE_OFFSET
	base_addr+=sprite_page_number*64*32
	store base_addr

for: cursor=0 to 15
	bg_page=agb_bg_map[cursor]
	bg_page&=vram_page_mask
	if banknumber == bg_page
		within_group=(cursor&3)*16*4*32
		group_base=(cursor/4)*16384
		base_addr=AGB_VRAM+within_group+group_base
		store base_addr
 .endif		

@ .align
@ .pool
@ .section .iwram, "ax", %progbits
@ .subsection 0
@ .align
@ .pool
render_a_kilobyte:
	@translate input tilenum to AGB pointers
	
	
	@agb_bg_map, and spr_cache_map
	@base, mask

	
	 @base_addr_list[14]
	ldr globalptr,=GLOBAL_PTR_BASE
	ldrb_ r11,vram_page_mask
	
	 @base_addr_cursor=0
	mov r8,#0
	 @kb=tilenum/64
	mov r0,tilenum,lsr#6   @kilobyte number
	
	strb r0,9f	@save kilobyte number for checking
	
	@banknumber=vram_page_base+kb
	ldrb_ r1,vram_page_base
	add r1,r0,r1
	 @sprite_page_number=spr_cache_map[banknumber]
	ldr r2,=spr_cache_map
	ldrsb r0,[r2,r1]
	
	 @base_addr=AGB_VRAM+0x10000+SPR_CACHE_OFFSET
	ldr r7,=SPR_VRAM @+SPR_CACHE_OFFSET  ;not that part, because it's included from spr_cache_map
	 @base_addr+=sprite_page_number*64*32
	adds r7,r7,r0,lsl#11
	 @if sprite_page_number is positive
	bcs 1f
	 @store base_addr
	str r7,[sp,r8,lsl#2]
	add r8,r8,#1
1:
	 @for cursor=0 to 15
	mov r3,#0
	adr_ r2,agb_real_bg_map
1:	
	 @bg_page=agb_real_bg_map[cursor]
	ldrb r0,[r2,r3]
	 @bg_page&=vram_page_mask
	and r0,r0,r11
	 @if banknumber == bg_page
	cmp r0,r1
	bne 2f
	 @within_group=(cursor&3)*16*4*32
	and r0,r3,#3
	ldr r7,=BG_VRAM
	add r7,r7,r0,lsl#11
	 @group_base=(cursor/4)*16384
	mov r0,r3,lsr#2
	add r7,r7,r0,lsl#14
	 @store base_addr
	str r7,[sp,r8,lsl#2]
	add r8,r8,#1
2:
	add r3,r3,#1
	cmp r3,#16
	blt 1b
	
	movs r8,r8
	beq skip_this_kilobyte
	@self modify something so I don't need to use so many registers
	strb r8,8f
	subs r8,r8,#1
	strb r8,7f

	@render the tile
render_tiles_loop_2:
	ldrh tilenum,[r_tnum]
	mov r0,tilenum,lsr#6
9:	cmp r0,#255		@modify
	bxne lr
	add r_tnum,r_tnum,#2
	
	and tilenum,tilenum,#0x3F
	ldr agbptr,[sp]
	add agbptr,agbptr,tilenum,lsl#5
	ldr decodeptr,=CHR_DECODE
0:
	ldrb r0,[r_tiles],#1  @first plane
	ldrb r1,[r_tiles,#7]  @second plane
	
	ldr r0,[decodeptr,r0,lsl#2]
	ldr r1,[decodeptr,r1,lsl#2]
	orr r0,r0,r1,lsl#1
	
	str r0,[agbptr],#4
	tst agbptr,#0x1F @finished with AGB tile?
	bne 0b
	
	add r_tiles,r_tiles,#8
	
7:	movs r0,#255	@modify, change to 0 if there are no dupes
	beq 1f	
	
	@fetch recently encoded title
	sub agbptr,agbptr,#32
	ldmia agbptr,{r0,r1,r2,r3, r4,r10,r11,r12}
	mov r8,#1
0:
	ldr agbptr,[sp,r8,lsl#2]
	add agbptr,agbptr,tilenum,lsl#5
	stmia agbptr,{r0,r1,r2,r3, r4,r10,r11,r12}
	add r8,r8,#1
8:	cmp r8,#255  @modify, number of copies of the tile
	blt 0b
1:	
	b render_tiles_loop_2

skip_this_kilobyte:
	mov r0,tilenum,lsr#6
0:
	ldrh tilenum,[r_tnum,#2]!
	add r_tiles,r_tiles,#16
	mov r1,tilenum,lsr#10
	cmp r0,r1
	beq 0b
	bx lr


@ .align
@ .pool
@ .text
@ .align
@ .pool


	.endif @MIXED_VRAM_VROM

	.endif @DIRTYTILES

	.if USE_BG_CACHE
@
@
@

@r2 = NES_VRAM address
@addy = GBA_VRAM address
update_tile_init:
	ldr r5,=0x00FF00FF
	ldr r4,=0x00030003
	bx lr

@update_one
@	ldrb r0,[r2],#1
@	ldrh r1,[addy]
@	bic r1,r1,#0xFF
@	orr r0,r0,r1
@	strh r0,[addy],#2
@	bx lr
@update_two
@	ldrh r0,[r2],#2
@	orr r0,r0,r0,lsl#8
@	bic r0,r0,#0xFF00
@	ldr r1,[addy]
@	bic r1,r1,r5 @#0x00FF00FF
@	orr r0,r0,r1
@	str r0,[addy],#4
@	bx lr
display_bg:
	ldrb_ r0,bg_cache_updateok
	movs r0,r0
	bxeq lr
	mov r0,#0
	strb_ r0,bg_cache_updateok
	
	ldrb_ r0,bg_cache_full
	movs r0,r0
	bne_long display_whole_map
consume_bg_cache:
	ldr_ r6,bg_cache_produce_limit_consume_begin
	ldr_ r7,bg_cache_produce_base_consume_end
	cmp r6,r7
	bxeq lr
	stmfd sp!,{r10,lr}
	@bg_cache_produce_base_consume_end = end
	@bg_cache_produce_limit_consume_begin = start
	bl update_tile_init
	@don't touch r4,r5
	@r0,r1,r3 get destroyed
	@r6-r11,lr free
	@r2 = src addr
	@r12 = dest addr
	
	@r6 = cursor, r7 = limit
	@r8 = cache base
	@r9 = Next tile
	ldr r8,=BG_CACHE
	ldr r10,=NES_VRAM2  @FIXME need to add 4-screen mirroring here
	ldr r11,=AGB_BG
	mov r9,#-1

3:
	cmp r6,r7
	beq 2f
1:
	@r0=tile number
	ldrh r0,[r8,r6]
	add r6,r6,#2
	bic r6,r6,#BG_CACHE_SIZE
	
	bic r2,r0,#1
	cmp r2,r9
	beq 3b
	
	@test if tile is attribute tile
	bic r1,r0,#0xFC00
	cmp r1,#0x3C0
	bge do_attribute

	@do two tiles, r2=tile number

@depends on r2,r5,r10,r11
	mov r9,r2
5:

@vram 4 hack code
	tst r2,#0x800
	mov r0,r10
	subne r0,r0,#0x3000

	ldrh r0,[r0,r2]
	orr r0,r0,r0,lsl#8
	bic r0,r0,#0xFF00
	ldr r1,[r11,r2,lsl#1]
	bic r1,r1,r5 @#0x00FF00FF
	orr r0,r0,r1
	str r0,[r11,r2,lsl#1]

	cmp r6,r7
	bne 1b
2:
	ldmfd sp!,{r10,lr}
	@"bg_cache_produce_limit_consume_begin" has now advanced to "bg_cache_produce_base_consume_end".
	@Set the new consume_begin
	mov r0,r6
	b_long set_bg_cache_produce_limit_consume_begin
@	str_ r6,bg_cache_produce_limit_consume_begin
@	bx lr
	.if DRAW_ATTRIBUTE_TABLES
4:
	@if you don't want to see attributes, replcace adr lr,4b with 3b, and delete this code
	@force it to run tile update code after it updates attributes, because some games display the attribute tables
	cmp r6,#0
	sub r0,r6,#2
	addeq r0,r0,#BG_CACHE_SIZE
	ldrh r0,[r8,r0]
	bic r2,r0,#1
	b 5b
	.endif
	



do_attribute:
	tst r2,#0x800	@check if we're on screen 3 or 4

	and r1,r0,#0x7		@dest column number
	add r12,r11,r1,lsl#3	@4 tiles per attribute column
	and r1,r0,#0x38		@dest row number
	add r12,r12,r1,lsl#5	@[nes_row*8]/8*4*32*2
	and r1,r0,#0xC00
	add r12,r12,r1,lsl#1
	add r2,r10,r0

	subne r2,r2,#0x3000	@subtract from memory address if we're on screen 3 or 4

	.if DRAW_ATTRIBUTE_TABLES
	adr lr,4b
	.else
	adr lr,3b
	.endif	
update_attributes:
	ldrb r0,[r2],#1
	orr r0,r0,r0,lsl#16

	and r3,r0,r4,lsl#2
	ldr r1,[addy,#4]
	and r1,r1,r5 @#0x00FF00FF
	orr r1,r1,r3,lsl#10
	str r1,[addy,#4]
		ldr r1,[addy,#0x44]
		and r1,r1,r5 @#0x00FF00FF
		orr r1,r1,r3,lsl#10
		str r1,[addy,#0x44]
	and r3,r0,r4,lsl#4
	ldr r1,[addy,#0x80]
	and r1,r1,r5 @#0x00FF00FF
	orr r1,r1,r3,lsl#8
	str r1,[addy,#0x80]
		ldr r1,[addy,#0xC0]
		and r1,r1,r5 @#0x00FF00FF
		orr r1,r1,r3,lsl#8
		str r1,[addy,#0xC0]
	and r3,r0,r4,lsl#6
	ldr r1,[addy,#0x84]
	and r1,r1,r5 @#0x00FF00FF
	orr r1,r1,r3,lsl#6
	str r1,[addy,#0x84]
		ldr r1,[addy,#0xC4]
		and r1,r1,r5 @#0x00FF00FF
		orr r1,r1,r3,lsl#6
		str r1,[addy,#0xC4]
	and r3,r0,r4,lsl#0
		ldr r1,[addy,#0x40]
		and r1,r1,r5 @#0x00FF00FF
		orr r1,r1,r3,lsl#12
		str r1,[addy,#0x40]
	ldr r1,[addy]
	and r1,r1,r5 @#0x00FF00FF
	orr r1,r1,r3,lsl#12
	str r1,[addy],#8
	
	bx lr
	

@display_whole_map

update_whole_map:
	stmfd sp!,{lr}
	.if DRAW_ATTRIBUTE_TABLES
	add r6,r2,#0x400	@use 0x3C0 if you don't want to see attribute tables
	.else
	add r6,r2,#0x3C0
	.endif
0:
	ldrh r0,[r2],#2
	orr r0,r0,r0,lsl#8
	bic r0,r0,#0xFF00
	ldr r1,[addy]
	bic r1,r1,r5 @#0x00FF00FF
	orr r0,r0,r1
	str r0,[addy],#4
	cmp r2,r6
	blt 0b
	@now update attribute table
	.if DRAW_ATTRIBUTE_TABLES
	sub addy,addy,#0x800	@use 0x780 with no subtraction if you don't want to see attribute tables
	sub r2,r2,#0x40
	.else
	sub addy,addy,#0x780
	.endif
	mov r7,#8
1:
	mov r6,#8
0:
	bl update_attributes
	subs r6,r6,#1
	bne 0b
	add addy,addy,#0xC0  @=0x100-0x40
	subs r7,r7,#1
	bne 1b

	ldmfd sp!,{pc}
	
	.endif @USE_BG_CACHE


install_scrollx_timeout:
@	@do we already have a timeout?
@	ldr_ r2,fine_x_next
@	tst r2,#1
@	bxeq lr

	@find timestamp of next 257-12 starting from now
	@let t0=frame_timestamp-245
	@let SL=int((now-t0)/341)
	@let t = (SL+1)*341 + 245
	

	ldr_ r2,cycles_to_run
	sub r2,r2,cycles,asr#CYC_SHIFT
	ldr_ r1,timestamp
	add r1,r1,r2
	ldr_ r2,frame_timestamp_minus_96  @ntsc=-96, pal=-91
	sub r1,r1,r2
	ldr_ addy,timestamp_div
	umull r2,addy,r1,addy
	ldr_ r2,timestamp_mult
	mul r1,r2,addy
	add r1,r1,#0x0F  @for pal, does nothing for ntsc
	mov r1,r1,lsr#4
	ldr_ r2,frame_timestamp_plus_245 @ntsc=+245, pal=+228
	add r1,r1,r2
	
	adrl_ r12,fine_x_timeout
	adr r0,fine_x_handler
	b replace_timeout_2

fine_x_handler:
	ldr_ r0,scrollX
	ldr_ r1,scrollX_v
	cmp r1,r0
	strne_ r0,scrollX_v
	blne newX2
	b _GO
	

update_bankbuffer:
	@destroys r3,r4,r5
	adrl_ r0,nes_chr_map
	adr_ r1,bankbuffer_last
	ldmia r0,{r2,r3}
	ldmia r1,{r4,r5}
	cmp r2,r4
	cmpeq r3,r5
	stmneia r1,{r2,r3}
	bxeq lr

	stmfd sp!,{lr}
	bl_long get_scanline_2
	mov r0,r12
	ldmfd sp!,{lr}
bankbuffer_finish_2:
	cmp r0,#236	@why was this in here again?
	bge 0f
	subs r0,r0,#1
	movle r0,#0
	mov r0,r0,lsr#3
	cmp r0,#30
0:
	movge r0,#30
	@r4,r5 = data to fill
bankbuffer_finish:		@newframe jumps here
	@r1=lastline, lastline=scanline, r0=scanline
	ldrb_ r1,bankbuffer_line
	strb_ r0,bankbuffer_line
	@sl == 0 >> exit
	@sl < prev  >> prev = 0
	@sl == prev >> exit
	cmp r0,#0
	bxeq lr
	subs r0,r0,r1
	bxeq lr
	addlt r0,r0,r1  @could this happen?
	movlt r1,#0
	@r0 = number of scanlines to fill
	
	ldr_ r2,bankbuffer
	@should not be zero
	add r1,r2,r1,lsl#3
	
memset64:
	@r1 = dest
	@r0 = 64-bit word count
	@r4,r5 = what to fill with
	@may destroy r4-r7
	cmp r0,#3
	bmi 2f
	sub r0,r0,#3

	stmfd sp!,{r6,lr}
	mov r2,r4
	mov r3,r5
	mov r6,r4
	mov lr,r5
1:
	stmia r1!,{r2,r3,r4,r5,r6,lr}
	subs r0,r0,#3
	bmi 3f
	stmia r1!,{r2,r3,r4,r5,r6,lr}
	subs r0,r0,#3
	bpl 1b
3:
	adds r0,r0,#3
	ldmeqfd sp!,{r6,pc}
	ldmfd sp!,{r6,lr}
2:
	stmia r1!,{r4,r5}
	subs r0,r0,#1
	bgt 2b
	bx lr

	

	.if SPRITESCAN
spritescan:
	@call this before swapping buffers
	@can use r0-r9,r12
	
	@fixme
	ldr_ r0,dmanesoambuff
	
	mvn r2,#0
	mvn r3,#0
	mov r4,#1
	mov r5,#0
	mov r6,#0
	mov r7,#17
	ldr r1,[r0],#4
	bic r1,r1,#0xFF000000
sprscanloop:
	subs r7,r7,#1
	beq_long sprscanend
	ldr r8,[r0],#4
	@filter out X coordinate
	bic r8,r8,#0xFF000000
	cmp r8,r1
	addeq r4,r4,#1
	beq sprscanloop
	cmp r8,r2
	addeq r5,r5,#1
	beq sprscanloop
	movs r5,r5
	moveq r5,#1
	moveq r2,r8
	beq sprscanloop
	cmp r8,r3
	addeq r6,r6,#1
	beq sprscanloop
	movs r6,r6
	bxne lr
	moveq r6,#1
	moveq r3,r8
	b sprscanloop
sprmask_apply_loop:
	ldrh r1,[r0]
	bic r1,r1,#0x1000
	strh r1,[r0],#2
	subs r4,r4,#1
	bne sprmask_apply_loop
	bx lr
	
 .align
 .pool
 .text
 .align
 .pool

need_to_fetch_sprite_data:
	ldr r4,=alreadylooked
	ldr r4,[r4]
	movs r4,r4
	mov r4,#PRIORITY
	bxeq lr
	mov r4,#0
	ldr r0,=alreadylooked
	str r4,[r0]
	ldr lr,=update_sprites_enter
	ldr r0,=recache_sprites
	bx r0

@vram_read_direct
@	and r0,addy,#0x3c00
@	adr_ r1,vram_map
@	ldr r0,[r1,r0,lsr#8]
@	bic r1,addy,#0xfc00
@	ldrb r0,[r0,r1]
@	bx lr

sprscanend:
	mov addy,lr
	cmp r4,#8
	bleq sprmasktherows
	cmp r5,#8
	moveq r1,r2
	bleq sprmasktherows
	cmp r6,#8
	moveq r1,r3
	bleq sprmasktherows
	bx addy
sprmasktherows:
	and r1,r1,#0xFF
	add r1,r1,#1
	cmp r1,#240
	bxge lr
	
	@force dispcnt buffer to not remember changes
	ldrb_ r0,ctrl1line
	mov r4,#0xFFFFFF00
	orr r0,r0,r4
	str_ r0,ctrl1line
	
	ldrb_ r0,ppuctrl0frame
	tst r0,#0x20
	movne r4,#16
	moveq r4,#8

	add r0,r1,r4
	cmp r0,#240
	rsbge r4,r1,#240

	ldr_ r0,dispcntbuff

	add r0,r0,r1,lsl#1
	b_long sprmask_apply_loop
	.else
 .align
 .pool
 .text
 .align
 .pool
	.endif @SPRITESCAN

	.if USE_BG_CACHE
display_whole_map:
	stmfd sp!,{lr}
	mov r0,#0
	str_ r0,bg_cache_produce_cursor
	str_ r0,bg_cache_produce_base_consume_end
	bl set_bg_cache_produce_limit_consume_begin
	bl set_bg_cache_available
@	str_ r0,bg_cache_produce_limit_consume_begin
@	strb_ r0,bg_cache_full
	
	bl_long update_tile_init	
	
	ldr addy,=AGB_BG
	ldr r2,=NES_VRAM2
	bl_long update_whole_map
	ldr addy,=AGB_BG+0x800
	ldr r2,=NES_VRAM2+0x400
	bl_long update_whole_map

	ldrb_ r0,fourscreen
	movs r0,r0
	beq 0f
	
	ldr addy,=AGB_BG+0x1000
	ldr r2,=NES_VRAM4
	bl_long update_whole_map
	ldr addy,=AGB_BG+0x1800
	ldr r2,=NES_VRAM4+0x400
	bl_long update_whole_map
	
0:
	ldmfd sp!,{pc}
	.endif

vrom_update_tiles:
	ldrb_ r0,bankable_vrom
	ldrb_ r1,frameready
	movs r0,r0
	movnes r1,r1
	bxeq lr
	
	stmfd sp!,{lr}
	
@	ldr r8,=sprite_cache_size
@	ldr r8,[r8]
@	movs r8,r8
@	bxeq lr
	
	ldr r6,=spr_cache
	mov addy,#SPR_CACHE_SIZE
	ldr r5,=SPR_VRAM+SPR_CACHE_OFFSET
0:
	ldr r0,[r6]
	ldr r1,[r6,#spr_cache_disp-spr_cache]
@	str r0,[r6,#spr_cache_disp-spr_cache]
	eors r1,r1,r0
	addeq r6,r6,#4
	addeq r5,r5,#0x2000
	blne_long im_lazy
	subs addy,addy,#1
	bne 0b
	
	@AGB BG is dirty?
	adrl_ r9,agb_bg_map_requested
	ldmia r9,{r0-r3}
	adrl_ r9,agb_real_bg_map
	ldmia r9,{r4-r7}
	cmp r0,r4
	cmpeq r1,r5
	cmpeq r2,r6
	cmpeq r3,r7
	beq bgmap_clean
	
	
	adrl_ r6,agb_bg_map_requested
vut0:
	ldr r0,[r6]
	ldr r1,[r6,#16]
	eors r1,r1,r0
	addeq r6,r6,#4
	adrnel_ r0,agb_bg_map_requested
	subne r0,r6,r0
	movne r5,#BG_VRAM
	addne r5,r5,r0,lsl#12	@0000/4000/8000/C000
	blne_long im_lazy
	adrl_ r0,agb_bg_map_requested+16
	cmp r6,r0
	bne vut0
	
@	adrl_ r9,agb_bg_map_requested
@	ldmia r9,{r0-r3}
@	adrl_ r9,agb_real_bg_map
@	stmia r9,{r0-r3}
	
bgmap_clean:

	
	
	ldmfd sp!,{pc}


check_canaries:
	.if CRASH
	ldrb_ r0,crash_disabled
	movs r0,r0
	bne 0f
	ldrb_ r0,crash_framecounter
	add r0,r0,#1
	strb_ r0,crash_framecounter
	cmp r0,#180
	blt 0f

	mov r0,#1
	strb_ r0,crash_disabled
	mov r0,sp
	
	mrs	r2, cpsr
	bic	r2, r2, #0xdf		@ \__
	orr	r2, r2, #0x92		@ /  --> Disable IRQ. Enable FIQ. Set CPU mode to IRQ.
	msr	cpsr, r2
	
	mov r1,sp
	
	mrs	r2, cpsr
	bic	r2, r2, #0xdf		@ \__
	orr	r2, r2, #0x1f		@ /  --> Enable IRQ & FIQ. Set CPU mode to System.
	msr	cpsr,r2
	
	ldr r2,=crash
	mov r11,r11
	bx r2
0:
	.endif

	stmfd sp!,{lr}
	ldr r1,=IWRAM_CANARY_1
	ldr r2,=0xDEADBEEF
	ldr r0,[r1]
	cmp r0,r2
	bne canary1_fail

	ldr r1,=IWRAM_CANARY_2
	ldr r2,=0xDEAFBEEF
	ldr r0,[r1]
	cmp r0,r2
	bne canary2_fail
	
	ldmfd sp!,{pc}
canary1_fail:
	mov r11,r11
	@test if it's safe
	cmp r1,sp	@is canary before the stack?  Then it's safe.
	bge 0f
	bl_long build_chr_decode
	b 1f
canary2_fail:
	mov r11,r11
	cmp r1,sp	@is canary before the stack?  Then it's safe.
	bge 0f
1:
	ldr r0,=_scaling
	ldrb r0,[r0]
	and r0,r0,#3
	bl_long spriteinit
0:
	ldmfd sp!,{pc}

scale75:
	stmfd sp!,{r10,lr}
	@r0 = ?, r1 = ?, r2 = LTS/?, r3 = skipflags, r11 = S, r12 = S-D
	@r7 = src_xy, r8 = src_dispcnt, r9 = src_bg0cnt
	@r4 = dest_xy, r5 = dest_dispcnt, r6 = dest_bg0cnt
	ldrb_ r11,windowtop_scaled6_8
	
	ldr_ r7,dmascrollbuff
	ldr_ r8,dmadispcntbuff
	ldr_ r9,dmabg0cntbuff
	
	ldr_ r4,dma0buff
	ldr_ r5,dma1buff
	ldr_ r6,dma3buff
	
	mov r12,r11
	
	@self modifying code
	ldrb_ r0,adjustblend
	add r0,r0,#1
	and r0,r0,#3
	ldr r1,=_twitchline_mod1
	strb r0,[r1,#_twitchline_mod1-_twitchline_mod1]
	add r0,r0,#1
	strb r0,[r1,#_twitchline_mod3-_twitchline_mod1]
	and r0,r0,#3
	strb r0,[r1,#_twitchline_mod2-_twitchline_mod1]
	
	ldrb_ r0,twitch
	eor r0,r0,#1
	ldrb_ r1,flicker
	ands r1,r0,r1
	strb_ r0,twitch
	
	add r10,r4,#160*4  @dma0buff + 160 * 4
	
	add r8,r8,r11,lsl#1
	add r9,r9,r11,lsl#1

	bne_long scale75_odd
	b_long scale75_even

ppu2003_W:
	strb_ r0,oam_addr
	bx lr
ppu2004_W:
	ldr_ r1,nesoambuff
	ldrb_ r2,oam_addr
	strb r0,[r1,r2]
	add r2,r2,#1
	strb_ r2,oam_addr
	bx lr

ppu2004_r:
@DO NOT TOUCH R12 (addy), it needs to be preserved for Read-Modify-Write instructions
	ldrb_ r0,screen_off
	movs r0,r0
	bne 0f
	stmfd sp!,{r12,lr}
	bl_long get_scanline_2
	cmp r2,#0xC0000000
	movlo r0,#0
	movhi r0,#0xFF
	ldmfd sp!,{r12,pc}
0:
	ldr_ r1,nesoambuff
	ldrb_ r2,oam_addr
	ldrb r0,[r1,r2]
	and r2,r2,#0x03
	cmp r2,#2
	andeq r0,r0,#0xE3
	bx lr

ppuctrl0_install_NMI_timeout:
	@assume a 12 cycle instruction brought us here
	@get current timestamp
	ldr_ r2,cycles_to_run
	sub r2,r2,cycles,asr#CYC_SHIFT
	ldr_ r1,timestamp
	add r2,r2,r1
	add r1,r2,#13
	ldr r0,=nmi_handler
	adrl_ r12,nmi_timeout
	b_long replace_timeout_2

screen_on_sprite0:
	ldr r1,=ppustat_
	ldrb r0,[r1]
	tst r0,#0x40
	bxne lr
	
	@FIXME: NEED TO HANDLE BACKDATED SPRITES (ie. turning screen on late, and sprite starts above our position)
	
	@read sprite Y
	ldr_ r0,nesoambuff
	ldrb r1,[r0]
	@check if it's in range
	cmp r1,#240
	bxge lr
	@read sprite X, check if it's in range
	ldrb r2,[r0,#3]
	cmp r2,#255
	bxge lr
	add r1,r1,#2
	
	ldr_ r12,timestamp_mult
	mul r1,r12,r1
	ldrb_ r12,timestamp_mult_2
	mla r1,r2,r12,r1
	
	sub r1,r1,#12*16
	
	ldr_ r0,frame_timestamp
	add r1,r0,r1,asr#4
	
	@check if sprite hit time is before current time
	ldr_ r2,cycles_to_run
	sub r2,r2,cycles,asr#CYC_SHIFT
	ldr_ r12,timestamp
	add r2,r2,r12
	
	cmp r1,r2
	bxmi lr  @should this be mi or le?
	
	@set to 0, this will be incremented every time it checks the scanline for a sprite 0 collision
	mov r0,#0
	strb_ r0,y_in_sprite0
	
	ldr r0,=sprite_zero_handler_2
	adrl_ r12,sprite_zero_timeout
	b_long replace_timeout_2

.pushsection .text, "ax", %progbits

byte_reverse_table:
	.byte 0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0
	.byte 0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8
	.byte 0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4
	.byte 0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC
	.byte 0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2
	.byte 0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA
	.byte 0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6
	.byte 0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE
	.byte 0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1
	.byte 0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9
	.byte 0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5
	.byte 0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD
	.byte 0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3
	.byte 0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB
	.byte 0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7
	.byte 0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF

.popsection

 .pushsection .vram1, "ax", %progbits

sprite_zero_handler_2:
	@check if both sprites and background are enabled
	ldrb_ r2,ppuctrl1
	and r2,r2,#0x18
	cmp r2,#0x18
	bne nohit	@no sprite hit possible when both are disabled
	
	@If we are looking at 8x8 sprites, are we examining row 8-15?
	ldrb_ r2,ppuctrl0
	ldrb_ r1,y_in_sprite0
	tst r2,#0x20	@8x16 sprite flag
	bne 0f			@skip row number >=8 check
	cmp r1,#8
	bcs nohit		@no hit if row number >= 8
	tst r2,#0x20
@	b 0f
0:	
	stmfd sp!,{r3,r4}
	
	@Get the two graphical bytes for the sprite
	ldr_ addy,nesoambuff	@read pointer to sprite 0
	ldrb r0,[addy,#1]	@tile number
	ldrb r3,[addy,#2]	@flip flags
	andne addy,r0,#1	@for 8x16 sprites, use low bit as page selection
	bicne r0,r0,#1		@clear low bit of sprite number for 8x16 sprites
	andeq addy,r2,#0x08		@for 8x8 sprites, use ppuctrl0 bit #3
	moveq addy,addy,lsr#3	@move to bit #0
	add r0,r0,addy,lsl#8	@add sprite table number * 256 to sprite index
	mov r4,r0,lsr#6			@fetch the tile: get vram address (per 64 tiles)
	adr_ addy,vram_map		@
	ldr r4,[addy,r4,lsl#2]	@load base address from vram map
	and r0,r0,#0x3F			@mask tile number to 0-63
	add r4,r4,r0,lsl#4		@multiply tile number by 16 bytes
	tst r3,#0x80			@check for Y-flipping
	beq 0f					@skip ahead if there's no flipping
	tst r2,#0x20		@Y-Flipping: check for 8x16 sprite
	rsbeq r1,r1,#7		@use row = 7-row for 8x8 sprites
	rsbne r1,r1,#23		@use row = 23-row for 8x16 sprites (skipping ahead 16 for sprite boundaries)
0:
	ldrb r0,[r4,r1]!	@read first sprite byte
	ldrb r4,[r4,#8]		@read second sprite byte
	orrs r0,r0,r4		@combine them - r0 stays at this value for a while now.
	beq nohit_A			@no sprite pixels?  no hit.
	b_long 0f			@jump to next section
	.popsection
0:
	@X-flipping
	tst r3,#0x40					@check for X-flipping
	ldrne r3,=byte_reverse_table	@reverse the bits in the byte if there is X-flipping
	ldrneb r0,[r3,r0]				@
	
	@clip left?
	ldr_ r12,nesoambuff		@read sprite X coordinate
	ldrb r2,[r12,#3]
	
	ldrb_ r1,ppuctrl1	@if we are masking sprites or BG from the left edge of the screen, clip them out of the sprite pixels.
	and r1,r1,#0x06		@bg or sprite masking bits
	cmp r1,#0x06		@is it zero?
	beq 0f				@no left clipping
	cmp r2,#7			@is sprite X > 7?
	bgt 0f				@no left clipping
	mov r3,#0xFF		@set mask
	
	@clip:
	@        00000000 (shift left X)
	@        xxxxxxxx
	
	bics r0,r0,r3,lsl r2	@remove pixels from the sprite image
	beq_long nohit_A		@if this erased the sprite pixels, no hit.
0:
	@clip right?
	cmp r2,#248		@is sprite X < 248?
	blt 0f			@no right clipping
	mov r3,#0xFF	@set mask
	rsb r4,r2,#255	@get point to clip at: 255 - sprite X
	
	@255-255 = 0
	@255-254 = 1
	@...
	@255-249 = 6
	@255-248 = 7

	@clip:
	@00000000  (shift right 255-X)
	@xxxxxxxx
	
	bics r0,r0,r3,lsr r4	@mask the sprite pixels
	beq_long nohit_A		@if this erased the sprite pixels, no hit.
0:
	@may destroy everything but r0
	@r0 = byte from sprite
	@find out background addresses
	stmfd sp!,{r5,r6}
	ldr_ r1,sprite_zero_timestamp	@r1 = sprite 0 (X,Y) timestamp on the screen
	add r1,r1,#12					@add 12 because we are testing this early, so code that loads the value will get the right value
	
	ldr_ addy,frame_timestamp		@r12 = frame timestamp
	sub r1,r1,addy					@r1 = timestamp on screen
	ldr_ addy,timestamp_div			@r12 = timestamp division reciprocal
	umull r2,r4,r1,addy				@r2 = fraction, r4 = Y coordinate (plus 1)

	ldr_ r1,scroll_y_timestamp		@r1 = timestamp of last Y scroll write
	umull r3,r5,r1,addy				@r3 = fraction, r5 = Y coordinate of Y scroll write (plus 1)
	@r2 = new fraction
	@r3 = old fraction
	@if old fraction is before 0xb3000000
	@	scanline difference increments
	@if new fraction is before 0xb3000000 (we are treating it like it always is!)
	@	scanline difference decrements
	sub r5,r4,r5					@r5 = scanline distance between sprite 0 Y and Y scroll 
	
	ldr_ r1,scroll_threshhold_value		@check if effective Y scroll is increased by 1
	cmp r3,r1						@fraction < scroll threshhold?
	addcc r5,r5,#1					@add 1
	sub r5,r5,#1					@subtract??
	
	ldr_ r1,scrollY_v	@
	and addy,r1,#0xFF	@
	cmp addy,#240		@
	add addy,addy,r5	@
	bge 0f				@
	cmp addy,#240		@
	addge r1,r1,#16		@
	b 1f
0:
	cmp addy,#256
	addge r1,r1,#256
1:
	add r1,r1,r5
	bic r1,r1,#0xFE00
	ldr_ addy,nesoambuff
	ldrb r2,[addy,#3] @X coordinate
	ldr_ r3,scrollX_v
	@.............xxx = fine x
	@........xxxxx... = coarse x
	@.......x........ = high bit x
	@.............xxx = fine y
	@........xxxxx... = coarse y
	@.......x........ = high bit y
	
	@want to get:
	@...........xxxxx = coarse x
	@......xxxxx..... = coarse y
	@.....x.......... = high bit x
	@....x........... = high bit y
	@or in other words, a PPU address from two scrolls
	add r2,r2,r3
	add r5,r2,#8
	tst r2,#0x100
	and r2,r2,#0xFF
	orrne r2,r2,#0x2000
	tst r5,#0x100
	and r5,r5,#0xFF
	orrne r5,r5,#0x2000
	
	and r3,r1,#0xF8
	mov r3,r3,lsl#2
	tst r1,#0x100
	addne r3,r3,#0x800
	
	add r4,r3,r2,lsr#3
	add r5,r3,r5,lsr#3
	
	@r1 = Y, r2 = X
	adr_ addy,vram_map+8*4  @nametables
	mov r3,r4,lsr#10
	ldr r3,[addy,r3,lsl#2]
	bic r4,r4,#0xFC00
	ldrb r4,[r3,r4]
	mov r3,r5,lsr#10
	ldr r3,[addy,r3,lsl#2]
	bic r5,r5,#0xFC00
	ldrb r5,[r3,r5]
	@r4, r5 = tile numbers
	ldrb_ r3,ppuctrl0
	tst r3,#0x10
	addne r4,r4,#0x100
	addne r5,r5,#0x100
	
	adr_ addy,vram_map  @nametables
	mov r3,r4,lsr#6
	ldr r3,[addy,r3,lsl#2]
	and r4,r4,#0x3F
	add r4,r3,r4,lsl#4

	mov r3,r5,lsr#6
	ldr r3,[addy,r3,lsl#2]
	and r5,r5,#0x3F
	add r5,r3,r5,lsl#4
	
	@r4, r5 = CHR data address
	and r1,r1,#0x07
	@r1 = fine vertical
	ldrb r3,[r4,r1]!
	ldrb r4,[r4,#8]
	orr r4,r4,r3
	ldrb r3,[r5,r1]!
	ldrb r5,[r5,#8]
	orr r5,r5,r3
	orr r4,r5,r4,lsl#8
	@r4 = 00000000 00000000 DATA1... DATA2...
	@r0 = 00000000 00000000          xxxxxxxx (shifted left 8-X&7)
	and r2,r2,#0x07
	rsb r3,r2,#8
	ands r0,r4,r0,lsl r3 @r3 = collision mask
	beq_long nohit_B
	
	@count leading zeroes in r0
	mov r1,#0
	tst r0,#0x0000F000
	moveq r0,r0,lsl#4
	addeq r1,r1,#4
	tst r0,#0x0000C000
	moveq r0,r0,lsl#2
	addeq r1,r1,#2
	tst r0,#0x00008000
	addeq r1,r1,#1
	
	@install the "you hit it" timeout to go off in R1 cycles
	ldmfd sp!,{r5,r6}
	ldmfd sp!,{r3,r4}
	
	@subtract X mod of sprite+bgscroll
	subs r1,r1,r2	
	beq sprite_zero_handler_3	@if it is zero, sprite collision immediately
	
	ldr_ r0,sprite_zero_timestamp
	add r1,r1,r0
	adr r0,sprite_zero_handler_3
	b_long 1f

.pushsection .vram1, "ax", %progbits

nohit_B:
	ldmfd sp!,{r5,r6}
nohit_A:
	ldmfd sp!,{r3,r4}
nohit:
	ldrb_ r1,y_in_sprite0
	add r1,r1,#1
	cmp r1,#8
	moveq r1,#16
	cmp r1,#24
	beq 0f  @no more hit checks
	strb_ r1,y_in_sprite0
	@insert code here to install the next timeout
	ldr_ r1,sprite_zero_timestamp
	ldr_ r0,timestamp_mult  @fixme for pal
	add r1,r1,r0,lsr#4
	adr r0,sprite_zero_handler_2
1:
	adrl_ r12,sprite_zero_timeout
	bl_long install_timeout_2
0:
	b_long _GO
.popsection

store_palette_force:
	stmfd sp!,{r0,r1,r2,r12,lr}
	mov r12,#0
	b 0f
store_palette:
	stmfd sp!,{r0,r1,r2,r12,lr}
	strb_ zero_byte,palette_dirty
	ldrb_ r0,palette_number
	cmp r0,#4
	ldmgefd sp!,{r0,r1,r2,r12,pc}
	bl_long get_scanline_2
	cmp r12,#240
	ldmgefd sp!,{r0,r1,r2,r12,pc}
0:
	ldrb_ r0,palette_number
	add r1,r0,#1
	strb_ r1,palette_number
	ldr_ r1,current_palette
	add r2,r1,#128
	strb r12,[r2,r0]!
	ldrb_ r12,ppuctrl1
	strb r12,[r2,#4]
	add r0,r1,r0,lsl#5
	adr_ r1,nes_palette
	mov r2,#32
	bl_long memcpy32
	ldmfd sp!,{r0,r1,r2,r12,pc}

sprite_zero_handler_3:
	.if OLDSPEEDHACKS
	.else
	bic cycles,cycles,#BRANCH
	.endif
	mov r0,#1
	bl call_quickhackfinder
@	bl set_cpu_hack

	ldr r0,=ppustat_
	ldrb r1,[r0]
	orr r1,r1,#0x40
	strb r1,[r0]
	b_long _GO

turn_screen_on:
	@exit if screen is already on
	ldrb_ r0,screen_off
	movs r0,r0
	bxeq lr
	
	stmfd sp!,{lr}
	
	@call the MMC3 screen-on hook if it exists
	ldr_ r0,screen_on_hook1
	movs r0,r0
	adrne lr,0f
	bxne r0
0:	
	@indicate that the screen is no longer off
	mov r0,#0
	strb_ r0,screen_off
	
	@Set PPU 2005 write, 2006 write, 2007 read to Screen-On version
	ldr r1,=PPU_read_tbl + 7*4
	ldr r0,=vmdata_R_screen_on
	str r0,[r1]
	ldr r1,=PPU_write_tbl + 5*4
	ldr r0,=bgscroll_W_screen_on
	str r0,[r1],#4
	ldr r0,=vmaddr_W_screen_on
	str r0,[r1],#4
	
	@store the palette if it's dirty
	ldrb_ r0,palette_dirty
	movs r0,r0
	blne store_palette
	
	@scroll X and Y to VRAM address, with loopy_v X scroll applied next scanline
	ldr_ r1,vramaddr
	bl_long UpdateXYScroll	@FIXME: assigns V=T for X scrolling bits
	
	bl screen_on_sprite0
	
	bl_long updateBGCHR_	@this is just for situations where we disabled BG changes during rendering
	

	
	@if we make any code test for screen_off to delay updates, force them to run here.  ??
	ldr_ r0,screen_on_hook2
	movs r0,r0
	adrne lr,0f
	bxne r0
0:	

	ldmfd sp!,{lr}
	bx lr

get_scrolly_scanline:
	@returns into r1
	@destroys r3,r4
	@screen must be on obviously
	ldr_ r2,cycles_to_run
	sub r2,r2,cycles,asr#CYC_SHIFT
	ldr_ r1,timestamp
	add r1,r1,r2
	ldr_ addy,frame_timestamp
	sub r1,r1,addy
	ldr_ addy,timestamp_div
	umull r2,r4,r1,addy
	
	ldr_ r1,scroll_y_timestamp
	umull r3,r0,r1,addy
	
	@r2 = new fraction
	@r3 = old fraction
	@if old fraction is before 0xb3000000
	@	scanline difference increments
	@if new fraction is before 0xb3000000
	@	scanline difference decrements
	
	sub r0,r4,r0
	
	ldr_ r1,scroll_threshhold_value
	cmp r3,r1
	addcc r0,r0,#1
	cmp r2,r1
	subcc r0,r0,#1
	
	ldr_ r1,scrollY_v
	@let r12=r1&0xFF
	@if r12<240 && r12+r0 <240, just accept r1+r0
	@if r12<240 && r12+r0 >=240, add additional 16
	@if r12>=240 && r12+r0 >=256, add additional 256
	and addy,r1,#0xFF
	cmp addy,#240
	add addy,addy,r0
	bge 0f
	cmp addy,#240
	addge r1,r1,#16
	b 1f	
0:
	cmp addy,#256
	addge r1,r1,#256
1:
	add r1,r1,r0
	bic r1,r1,#0xFE00
	bx lr


catchups:
	stmfd sp!,{r3,r4,r5,lr}
	bl_long get_scanline_2
	mov r4,addy
	b 2f
	
turn_screen_off:
	@exit if screen is already off
	ldrb_ r0,screen_off
	movs r0,r0
	bxne lr

	stmfd sp!,{r3,r4,r5,lr}
	
	@call the MMC3 screen-off hook if it exists
	ldr_ r0,screen_off_hook1
	movs r0,r0
	adrne lr,0f
	bxne r0
0:	
	@indicate that the screen is no longer off
	mov r0,#1
	strb_ r0,screen_off
	
	@Set PPU 2005 write, 2006 write, 2007 read to Screen-Off version
	ldr r1,=PPU_read_tbl + 7*4
	ldr r0,=vmdata_R
	str r0,[r1]
	ldr r1,=PPU_write_tbl + 5*4
	ldr r0,=bgscroll_W_screen_off
	str r0,[r1],#4
	ldr r0,=vmaddr_W_screen_off
	str r0,[r1],#4
	
	.if DIRTYTILES
	@if we crossed the half screen point, run nes_chr_update when the screen turns off
	@because some games update sprite graphics before switching to the status bar (Wizards and Warriors 3)
	ldrb_ r0,okay_to_run_nes_chr_update_this_frame
	cmp r0,#1
	mov r0,#2
	streqb_ r0,okay_to_run_nes_chr_update_this_frame
	bleq_long nes_chr_update
	.endif
	
	@DONE: run scroll catchups, backswitch catchups, MMC3 catchups, more?
	@TODO: the scroll catchups also affect loopy_v (currently Y coordinate only)
	@TODO: loopy_v gets put on the PPU so possibly clock MMC3?
	
	@TODO: cancel X scroll timeouts which would update the screen at 257
	adrl_ r12,fine_x_timeout
	bl_long remove_timeout
	adrl_ r12,sprite_zero_timeout
	bl_long remove_timeout
	
	@TODO: cancel MMC3 timeouts ??

@	b catchups2
catchups2:
@	stmfd sp!,{r3,r4,r5,lr}
	
	@run Y scrolling, and get scanline and scrolled Y output value
	bl get_scrolly_scanline
	@r1 = scrolly scanline
	str_ r1,scrollY_v	@do we need this?
	
	@split to loopy_v (okay to use r0,r12)
	@loopy_v:xxx............ = y:......xxx  (shift left 12 - 0 = 12)
	@loopy_v:...x........... = y:x........  (shift left 11 - 8 = 3)
	@loopy_v:.....xxxxx..... = y:.xxxxx...  (shift left 5 - 3 = 2)
	ldr_ r0,vramaddr
	bic r0,r0,#0x7B
	bic r0,r0,#0xE0
	and r12,r1,#0x100
	orr r0,r0,r12,lsl#3
	and r12,r1,#0x7
	orr r0,r0,r12,lsl#12
	and r12,r1,#0xF8
	orr r0,r0,r12,lsl#2
	str_ r0,vramaddr
	
	cmp r2,#0x60000000	@subtract 1 if r2 less than 60000000
	sbcs r4,r4,#0
	movmi r4,#0		@set to zero if this would make it negative
2:
	cmp r4,#240
	movge r4,#240
@	stmfd sp!,{r4}
	
	@adjusts scrollY_v but not yet VRAM_ADDR

@run catchups -------------
@addy=scanline to run to
	ldr_ r1,ctrl1old
	ldrb_ r0,ctrl1line
	cmp r0,#0
	cmpeq r4,#240
	bne 0f
	strb_ r0,ctrl1line_previous
	ldrb_ r12,ctrl1line_previous3
	movs r12,r12
	bne 0f
	ldr_ r12,dispcntbuff
	add r12,r12,r0,lsl#1
	ldrh r12,[r12]
	cmp r1,r12
	bne 0f
	strb_ r4,ctrl1line
	b 1f
0:
@r1 = value to set, r0 = scanline to start from, r12 = scanline to fill until
	mov addy,r4
	bl_long ctrl1finish
1:
@--------------------------
	ldr_ r0,scrollXold
	ldrb_ r1,scrollXline
	cmp r1,#0
	cmpeq r4,#240
	bne 0f
	strb_ r1,scrollXline_previous
	ldrb_ r12,scrollXline_previous3
	movs r12,r12
	bne 0f
	ldr_ r12,scrollbuff
	add r12,r12,r1,lsl#2
	ldrh r12,[r12]
	sub r12,r12,#SCREEN_LEFT
	cmp r0,r12
	bne 0f
	strb_ r4,scrollXline
	b 1f
0:
	mov addy,r4
	bl_long scrollXfinish
1:
@--------------------------
	ldr_ r2,scrollYold
	ldrb_ r1,scrollYline
	cmp r1,#0
	cmpeq r4,#240
	bne 0f
	strb_ r1,scrollYline_previous
	ldrb_ r12,scrollYline_previous3
	movs r12,r12
	bne 0f
	ldr_ r12,scrollbuff
	add r12,r12,r1,lsl#2
	ldrh r12,[r12,#2]
	cmp r2,r12
	bne 0f
	strb_ r4,scrollYline
	b 1f
0:
	mov addy,r4
	bl_long scrollYfinish
1:
@--------------------------
	ldrb_ r1,chrline
	cmp r1,#0
	cmpeq r4,#240
	bne 0f
	strb_ r1,chrline_previous
	ldrb_ r12,chrline_previous3
	movs r12,r12
	bne 0f
	ldr_ r12,bg0cntbuff
	add r12,r12,r1,lsl#1
	ldrh r12,[r12]
	ldr_ r0,chrold
	cmp r0,r12
	bne 0f
	strb_ r4,chrline
	b 1f
0:	
	mov r2,r4
	bl_long chrfinish2
1:
@--------------------------
	adr_ r2,bankbuffer_last
	ldmia r2,{r3,r5}

	ldrb_ r1,bankbuffer_line
	cmp r1,#0
	cmpeq r4,#240
	bne 0f
	strb_ r1,bankbuffer_line_previous
	ldrb_ r12,bankbuffer_line_previous3
	movs r12,r12
	bne 0f
	ldr_ r12,bankbuffer
	add r12,r12,r1,lsl#3
	ldmia r12,{r0,r1}
	cmp r0,r4
	cmpeq r1,r5
	bne 0f
	strb_ r4,bankbuffer_line
	b 1f	
0:
	mov r0,r4
	mov r4,r3
	bl_long bankbuffer_finish_2
1:
@done with catchups -------

	ldr_ r0,screen_off_hook2
	movs r0,r0
	adrne lr,0f
	bxne r0
0:	
	ldmfd sp!,{r3,r4,r5,lr}
	bx lr


newframe_set0:
@called at line 1
	mov r0,#0
	strb_ r0,ctrl1line
	strb_ r0,chrline
	strb_ r0,scrollXline
	strb_ r0,scrollYline
	strb_ r0,bankbuffer_line
	
@	mov r0,#512
@	str r0,ppuhack_line
	
	bx lr




 .align
 .pool
 .section .iwram, "ax", %progbits
 .subsection 1
 .align
 .pool
@irqhandler	@r0-r3,r12 are safe to use
@@----------------------------------------------------------------------------
@	mov r2,#REG_BASE
@	mov r3,#REG_BASE
@	ldr r1,[r2,#REG_IE]!
@	and r1,r1,r1,lsr#16	@r1=IE&IF
@	ldrh r0,[r3,#-8]
@	orr r0,r0,r1
@	strh r0,[r3,#-8]
@
@		@---these CAN'T be interrupted
@		ands r0,r1,#0x80
@		strneh r0,[r2,#2]		@IF clear
@		.if LINK
@		bne_long serialinterrupt
@		.endif
@		@---
@		adr r12,irq0
@
@		@---these CAN be interrupted
@		ands r0,r1,#0x01
@		ldrne r12,vblankfptr
@		bne jmpintr
@		ands r0,r1,#0x10
@		ldrne r12,=timer1interrupt
@		bne jmpintr
@		ands r0,r1,#0x20
@		ldrne r12,=timer_2_interrupt
@		bne jmpintr
@		ands r0,r1,#0x40
@		ldrne r12,=timer_3_interrupt
@		bne jmpintr
@		@----
@		moveq r0,r1		@if unknown interrupt occured clear it.
@jmpintr
@	strh r0,[r2,#2]		@IF clear
@
@	ldr r1,[sp,#0x14]
@
@	mrs r3,spsr
@	stmfd sp!,{r3,lr}
@	mrs r3,cpsr
@	bic r3,r3,#0x9f
@	orr r3,r3,#0x1f			@--> Enable IRQ & FIQ. Set CPU mode to System.
@	msr cpsr_cf,r3
@	stmfd sp!,{lr}
@	adr lr,irq0
@
@	bx r12
@
@
@irq0
@	ldmfd sp!,{lr}
@	mrs r3,cpsr
@	bic r3,r3,#0x9f
@	orr r3,r3,#0x92        		@--> Disable IRQ. Enable FIQ. Set CPU mode to IRQ
@	msr cpsr_cf,r3
@	ldmfd sp!,{r0,lr}
@	msr spsr_cf,r0
@vbldummy
@	bx lr
@@----------------------------------------------------------------------------
@vblankfptr: .word vbldummy			@later switched to vblankinterrupt


@scale75 moved to ROM code
scale75_even:
scale75_even_loop:
	bl scale75_findscanline
	@r2 = line_to_skip+1
	
	rsb r2,r2,#5
	mov r3,#4
0:
	cmp r2,r3
	bleq scale75_skip
	
	ldr r0,[r7,r11,lsl#2]
	add r0,r0,r12,lsl#16
	str r0,[r4],#4
	ldrh r0,[r8],#2
	strh r0,[r5],#2
	ldrh r0,[r9],#2
	strh r0,[r6],#2
	add r11,r11,#1
	
	subs r3,r3,#1
	bne 0b
	
	cmp r4,r10
	blt scale75_even_loop
scale75_exit:
	ldmfd sp!,{r10,pc}

scale75_odd:
	mov r3,#-2
scale75_odd_loop:
	bl scale75_findscanline
	
	rsb r2,r2,#4
	add r3,r3,#8
0:
	tst r3,#1
	blne scale75_skip2

	ldr r0,[r7,r11,lsl#2]
	add r0,r0,r12,lsl#16
	str r0,[r4],#4
	ldrh r0,[r8],#2
	strh r0,[r5],#2
	ldrh r0,[r9],#2
	strh r0,[r6],#2
	add r11,r11,#1

	cmp r2,r3,asr#1
	@if true, skip next
	orreq r3,r3,#1
	subs r3,r3,#2
	bpl 0b
	
	cmp r4,r10
	blt scale75_odd_loop

@	mov r0,#(((DMA0BUFF+160*4)/4)&0xFF)<<24
@	cmp r0,r4,lsl#22
@	bne scale75_odd_loop
	b scale75_exit

	
scale75_findscanline:
	@find scanline to skip
	mov r2,#0
0:
	@r1 = Y
	add r1,r2,r11
	add r2,r2,#1 @inc Y1 now...
	@read Y scroll value (most significant 16 bits of table at r7)
	ldr r0,[r7,r1,lsl#2]
	add r1,r1,r0,lsr#16
	and r1,r1,#3
	@compare against twitch line
_twitchline_mod1:
	cmp r1,#255
	bne 1f
	@compare against next value
	add r1,r2,r11
	ldr r0,[r7,r1,lsl#2]
	add r1,r1,r0,lsr#16
	and r1,r1,#3
_twitchline_mod2:
	cmp r1,#255
	@match
	bxeq lr
1:
	cmp r2,#4
	blt 0b
_twitchline_mod3:
	mov r2,#255	@if it can't find a good line to jitter, jitter 'twitchline'
	bx lr

scale75_skip2:
	bic r3,r3,#1
scale75_skip:
	add r12,r12,#1
	add r8,r8,#2
	add r9,r9,#2
@	add r11,r11,#1
	add pc,lr,#7*4
@scale75_copy
@	@copy a line
@	@XY data
@	ldr r0,[r7,r11,lsl#2]
@	add r0,r0,r12,lsl#16
@	str r0,[r4],#4
@	mov r1,r11,lsl#1
@	ldrh r0,[r8,r1]
@	strh r0,[r5],#2
@	ldrh r0,[r9,r1]
@	strh r0,[r6],#2
@	add r11,r11,#1
@	bx lr





vblankinterrupt:@
@----------------------------------------------------------------------------
	stmfd sp!,{r4-addy,lr}
	ldr globalptr,=GLOBAL_PTR_BASE
	
	@immediately stop HDMA or else we might transfer junk into REG_DISPCNT, triggering GBC mode switch
	mov r5,#REG_BASE
	strh r5,[r5,#REG_DM0CNT_H]		@DMA0 stop
	strh r5,[r5,#REG_DM1CNT_H]		@DMA1 stop
	strh r5,[r5,#REG_DM3CNT_H]		@DMA3 stop
	
	bl_long check_canaries
	
	ldrb_ r0,inside_gba_vblank
	cmp r0,#2
	bge exit_gba_vblank
	stmfd sp!,{r0}
	
	mov r0,#2
	strb_ r0,inside_gba_vblank

	@set Interrupt Master Enable so other interrupts can interrupt this long handler
	mov	r0, #REG_BASE		@ REG_BASE
	mov r1,#1
	strb r1, [r0,#REG_IME]	@ enable IME
	
	ldr_ r0,emuflags
	tst r0,#FPS50
	beq nopal60
	ldrb_ r0,PAL60
	add r0,r0,#1
	cmp r0,#6
	movpl r0,#0
	strb_ r0,PAL60
nopal60:
	bl_long showfps_
	
	ldrb_ r0,okay_to_run_hdma
	movs r0,r0
	beq skipdma
	
	ldr_ r2,dma0buff	@setup DMA buffer for scrolling:
	add r3,r2,#SCREEN_HEIGHT*4
	ldr_ r1,dmascrollbuff
	ldrb_ r0,emuflags+1
	cmp r0,#SCALED
	bhs vblscaled

vblunscaled:
@copy to scaled buffers instead of using directly
	ldrb_ r0,windowtop
	add r1,r1,r0,lsl#2		@(unscaled)
vbl6:
	ldmia r1!,{r4-r7}
	add r4,r4,r0,lsl#16
	add r5,r5,r0,lsl#16
	add r6,r6,r0,lsl#16
	add r7,r7,r0,lsl#16
	stmia r2!,{r4-r7}
	cmp r2,r3
	bmi vbl6
	
	tst r0,#1
	bne 2f
	
	mov r3,r0,lsl#1
	ldr_ r1,dmadispcntbuff
	ldr_ r0,dma1buff
	mov r2,#SCREEN_HEIGHT*2
	add r1,r1,r3
	bl memcpy32
	ldr_ r1,dmabg0cntbuff
	ldr_ r0,dma3buff
	mov r2,#SCREEN_HEIGHT*2
	add r1,r1,r3
	bl memcpy32
	b 1f
2:
	ldr_ r1,dmadispcntbuff
	ldr_ r2,dmabg0cntbuff
	add r1,r1,r0,lsl#1
	add r2,r2,r0,lsl#1
	ldr_ r3,dma1buff
	ldr_ r4,dma3buff
	mov r5,#SCREEN_HEIGHT
0:	
	ldrh r0,[r1],#2
	strh r0,[r3],#2
	ldrh r0,[r2],#2
	strh r0,[r4],#2
	subs r5,r5,#1
	bne 0b
	b 1f
vblscaled:					@(scaled)
	bl_long scale75
1:
vbl5:
	mov r5,#REG_BASE
	add r4,r5,#REG_DMA0SAD
	
	ldr_ r0,dma0buff				@setup HBLANK DMA for display scroll:
	ldr r1,[r0],#4
	add r2,r5,#REG_BG0HOFS
	str r1,[r2]
	ldr r3,=0xA6600001				@noIRQ hblank 32bit repeat incsrc inc_reloaddst, 1 word transfer
	stmia r4!,{r0,r2,r3}			@write dma0 source address, destination address, control/word count
	
	ldr_ r0,dma1buff
	ldrh r1,[r0],#2
	add r2,r5,#REG_DISPCNT
	strh r1,[r2]
	ldr r3,=0xA2400001				@noIRQ hblank 16bit repeat incsrc fixeddst, 1 word transfer
	stmia r4!,{r0,r2,r3}			@write dma0 source address, destination address, control/word count
	
	add r4,r4,#12
	ldr_ r0,dma3buff
	ldrh r1,[r0],#2
	add r2,r5,#REG_BG0CNT
	strh r1,[r2]
	stmia r4!,{r0,r2,r3}			@write dma0 source address, destination address, control/word count
skipdma:
	ldmfd sp!,{r0}
	cmp r0,#1
	bge exit_gba_vblank
	mov r0,#1
	strb_ r0,inside_gba_vblank
	
	ldrb_ r0,firstframeready
	movs r0,r0
	beq 0f
	
	bl update_sprites
	bl_long vrom_update_tiles
	.if DIRTYTILES
	bl_long consume_recent_tiles
	.endif
	.if USE_BG_CACHE
	bl_long display_bg
	.endif
	
	bl_long run_palette
	
0:
	mov r0,#0
	strb_ r0,frameready
	strb_ r0,inside_gba_vblank
	
	@are we in the rom menu?
	ldr r0,=rommenu_state
	ldr r0,[r0]
	movs r0,r0
	blxne_long rommenu_frame

exit_gba_vblank:
	ldmfd sp!,{r4-addy,pc}

@totalblend:	.word 0

ystart:	.byte YSTART
	.byte 0
	.byte 0
	.byte 0

	.pushsection .text, "ax", %progbits
	.pushsection .vram1, "ax", %progbits
run_palette:
	stmfd sp!,{lr}
	@--------
	@ step 1: scale or scroll scanline numbers to physical GBA scanline numbers
	@--------
	@scaled: multiply by 3/4, subtract 12
	@unscaled: subtract windowstart
	@* Need to subtract an additional 1, because when you're using scanline interrupts,
	@  you enter the handler during rendering time.  You need to wait for hblank time.
	@Values should be forced to the range of 0 to SCREEN_HEIGHT-1

	ldr_ r1,visible_palette
	add r2,r1,#128+4	@pointer to scanline numbers at the end of the visible_palette block
	ldrb_ r0,emuflags+1	@are we scaled or unscaled?
	cmp r0,#SCALED
	mov r0,#4			@number of palettes to look at
	mov r4,#0			@32-bit accumulator to avoid byte writes
	bcc run_palette_unscaled
	@scaled
0:
	ldrb r3,[r2,#-1]!
	add r3,r3,r3,lsl#1
	mov r3,r3,lsr#2
	subs r3,r3,#13 @normally 12 (screen clips at 16), but subtract 1 extra scanline
	movle r3,#0
	cmp r3,#SCREEN_HEIGHT-1
	movge r3,#SCREEN_HEIGHT-1
	orr r4,r3,r4,lsl#8
	subs r0,r0,#1
	bgt 0b
	b 1f
run_palette_unscaled:	
	@unscaled
	ldrb_ r5,windowtop
	add r5,r5,#1  @one extra scanline for LY interrupts needing to be one scanline early
0:
	ldrb r3,[r2,#-1]!
	subs r3,r3,r5
	movle r3,#0
	cmp r3,#SCREEN_HEIGHT-1
	movge r3,#SCREEN_HEIGHT-1
	orr r4,r3,r4,lsl#8
	subs r0,r0,#1
	bgt 0b
1:
	@--------
	@ step 2: determine the range of palettes to update (usually games don't use multiple palettes)
	@--------
	@We want to skip multiple palettes which start at scanline 0,
	@because scaling or scrolling the screen may cut palettes off.
	@Then we want to set what scanline to trigger the first IRQ at.
	
@	str_ r4,displayed_palette_scanlines
	mov r0,#0
	@skip repeating 00s, also skip first one no matter what
	mov r4,r4,lsr#8
	orr r4,r4,#0xFF000000
0:
	ands r2,r4,#0xFF
	bne 1f
	mov r4,r4,asr#8
	add r0,r0,#1
	b 0b
1:
	@r0 = index of first displayed palette (for the screen, next index is for the first scanline-triggered change)
	@r2 = scanline for palette change
	@r4 = scanlines for all palette changes (up to 3)
	str_ r4,displayed_palette_scanlines
	movs r2,r2
	cmpne r2,#SCREEN_HEIGHT-1
	@ge = no palette changes beyond first
	ldr r3,=REG_BASE + REG_DISPSTAT
	ldrb r5,[r3]
	orr r5,r5,#0x20
	bicge r5,r5,#0x20
	strb r5,[r3]		@enable/disable vcount interrupt
	strltb r2,[r3,#1]	@set scanline to trigger interrupt at
	
	@count remaining palette changes beyond initial one
	mov r7,#0
0:
	ands r2,r4,#0xFF
	cmp r2,#SCREEN_HEIGHT-1
	bge 1f
	add r7,r7,#1
	mov r4,r4,asr#8
	b 0b
1:	
	@r0 = index of first palette change (for the screen)
	@r7 = number of palette changes to store into the buffer
	
	add r8,r1,#128+4	@pointer to ppumask for each palette
	add r8,r8,r0
	add r1,r1,r0,lsl#5	@add screen index to palette source pointer
	
	ldrb r9,[r8],#1
	
	@copy first palette to screen
	@then copy remaining palettes to the buffer
	ldr r0,=AGB_PALETTE
	ldr r2,=MAPPED_RGB
	
	tst r9,#0x01
	mov r4,#0x7E
	movne r4,#0x60

	bl copy_four_colors
	bl copy_four_colors_2
	bl copy_four_colors_2
	bl copy_four_colors_2
	add r0,r0,#512-(32*3+8)
	bl copy_four_colors
	bl copy_four_colors_2
	bl copy_four_colors_2
	bl copy_four_colors_2

	@copy remaining palettes to buffer if necessary
	subs r7,r7,#1
	blt 2f
	
	ldr r0,=DISPLAYED_PALETTE_BUFFER
	sub r3,r0,#64
	str_ r3,displayed_palette_pointer
1:
	ldrb r9,[r8],#1
	tst r9,#0x01
	mov r4,#0x7E
	movne r4,#0x60
	mov r12,#8
0:
	bl copy_four_colors
	subs r12,r12,#1
	bgt 0b
	subs r7,r7,#1
	bge 1b
2:
	ldmfd sp!,{pc}
	
copy_four_colors_2:
	add r0,r0,#32-8
copy_four_colors:	
	@r0 = destination address
	@r1 = source address
	@r2 = MAPPED_RGB
	@r3 = temp (destroyed)
	@r4 = 0x7E (mask), or 0x60 (monochrome mask)
	@r5 = temp (destroyed)
	@r6 = temp (destroyed)
	ldr r3,[r1],#4
	and r6,r4,r3,lsl#1
	ldrh r6,[r2,r6]
	and r5,r4,r3,lsr#7
	ldrh r5,[r2,r5]
	orr r5,r6,r5,lsl#16
	and r6,r4,r3,lsr#15
	ldrh r6,[r2,r6]
	and r3,r4,r3,lsr#23
	ldrh r3,[r2,r3]
	orr r6,r6,r3,lsl#16
	stmia r0!,{r5,r6}
	bx lr
	.popsection

	.pushsection .vram1, "ax", %progbits

vcountinterrupt:
	@r0 = REG_BASE
	@increment displayed_palette_pointer
	ldr r12,=GLOBAL_PTR_BASE
vcountinterrupt_again:
	ldr r2,[r12,#displayed_palette_pointer]
	add r2,r2,#64
	str r2,[r12,#displayed_palette_pointer]
0:
	@move to next vcount interrupt scanline
	ldr r2,[r12,#displayed_palette_scanlines]
	and r1,r2,#0xFF
	mov r2,r2,asr#8
	str r2,[r12,#displayed_palette_scanlines]
	and r2,r2,#0xFF
	
	@prevent crash if we're trying to work with an out-of-range scanline
	cmp r1,#SCREEN_HEIGHT-1
	bxge lr
	
	@is next interrupt at the same time as this one?  Run that instead.
	cmp r2,r1
	ble vcountinterrupt_again
1:
	cmp r2,#SCREEN_HEIGHT-1
	ldrb r1,[r0,#REG_DISPSTAT]
	orr r1,r1,#0x30		@enable hblank IRQ and vcount IRQ
	bicge r1,r1,#0x20	@disable vcount IRQ if next scanline is out of range
	strb r1,[r0,#REG_DISPSTAT]
	movge r2,#0xFF
	strb r2,[r0,#REG_DISPSTAT+1]	@set next scanline for vcount interrupt
	ldrb r1,[r0,#REG_VCOUNT]	@look at current displayed scanline
	cmp r1,r2					@are we ahead of the upcoming interrupt?  
	bge vcountinterrupt_again		@Run that instead.

	ldr r1,[r0,#REG_IE]		@read interrupt flags
	ands r2,r1,#0x020000	@check for hblank interrupt
	bxeq lr
	
	add r1,r0,#REG_IE		@clear hblank interrupt
	mov r2,#0x02
	strb r2,[r1,#2]
	@continue to hblankinterrupt

hblankinterrupt:
	@r0 = REG_BASE
	@first disable hblank interrupts
	ldrb r1,[r0,#REG_DISPSTAT]
	bic r1,r1,#0x10
	strb r1,[r0,#REG_DISPSTAT]
	
	stmfd sp!,{r4,r5,lr}

	ldr r5,=GLOBAL_PTR_BASE
	ldr r5,[r5,#displayed_palette_pointer]
	mov r1,#AGB_PALETTE
	
	@BG palette
	ldmia r5!,{r2,r3,r4,r12}
	stmia r1,{r2,r3}
	add r1,r1,#32
	stmia r1,{r4,r12}
	add r1,r1,#32
	ldmia r5!,{r2,r3,r4,r12}
	stmia r1,{r2,r3}
	add r1,r1,#32
	stmia r1,{r4,r12}
	add r1,r1,#512-96
	
	@Sprite palettes
	ldmia r5!,{r2,r3,r4,r12}
	stmia r1,{r2,r3}
	add r1,r1,#32
	stmia r1,{r4,r12}
	add r1,r1,#32
	ldmia r5!,{r2,r3,r4,r12}
	stmia r1,{r2,r3}
	add r1,r1,#32
	stmia r1,{r4,r12}
	
	@check for vcount match
	ldrb r1,[r0,#REG_DISPSTAT+1]
	ldrb r2,[r0,#REG_VCOUNT]
	cmp r2,r1
	ldmltfd sp!,{r4,r5,pc}
	ldmfd sp!,{r4,r5,lr}
	
	mov r1,#0x04
	add r2,r0,#REG_IE
	strb r1,[r2,#2] @clear vcount interrupt
	
	b vcountinterrupt

	.popsection

	.popsection


	.pushsection .vram1, "ax", %progbits
@----------------------------------------------------------------------------
newframe_nes_vblank: 	@called at line 242
@----------------------------------------------------------------------------
@	stmfd sp!,{r3-r11,lr}
	stmfd sp!,{lr}
	
@these are now done by turn_screen_off

@	ldr_ r0,ctrl1old
@	ldr_ r1,ctrl1line
@	mov addy,#240
@	bl ctrl1finish
@@-----------------------
@	ldr_ r0,scrollXold
@	mov addy,#240
@	bl scrollXfinish
@@--------------------------
@	ldr_ r0,scrollYold
@	mov addy,#240
@	bl scrollYfinish
@@--------------------------
@	bl chrfinish
@@------------------------
@	adr_ r2,bankbuffer_last
@	ldmia r2,{r4,r5}
@	mov r0,#30
@	bl bankbuffer_finish
@--------------------------	
	.if SPRITESCAN
	bl_long spritescan
	.endif

	.if DIRTYTILES

	ldrb_ r0,okay_to_run_nes_chr_update_this_frame
	cmp r0,#2
	mov r0,#0
	strb_ r0,okay_to_run_nes_chr_update_this_frame
	blne_long nes_chr_update
	
	.endif
	
	@advance "previous" lines, this is okay to do now since vblank code doesn't look at this stuff
	mov r1,#0xFF00
	ldr_ r0,ctrl1line
	orr r2,r1,r0,lsl#8
	and r0,r0,#0xFF
	orr r0,r0,r2
	str_ r0,ctrl1line

	ldr_ r0,scrollXline
	orr r2,r1,r0,lsl#8
	and r0,r0,#0xFF
	orr r0,r0,r2
	str_ r0,scrollXline

	ldr_ r0,scrollYline
	orr r2,r1,r0,lsl#8
	and r0,r0,#0xFF
	orr r0,r0,r2
	str_ r0,scrollYline

	ldr_ r0,bankbuffer_line
	orr r2,r1,r0,lsl#8
	and r0,r0,#0xFF
	orr r0,r0,r2
	str_ r0,bankbuffer_line

	ldr_ r0,chrline
	orr r2,r1,r0,lsl#8
	and r0,r0,#0xFF
	orr r0,r0,r2
	str_ r0,chrline
	
	@finish palette buffer - write FF bytes to remaining scanlines
	ldrb_ r0,palette_number
	movs r0,r0
	@if zero, store palette
	bleq_long store_palette_force
	
	ldrb_ r0,palette_number
	ldr_ r1,current_palette
	add r1,r1,#128
	mov r2,#0xFF
0:
	cmp r0,#4
	bge 0f
	strb r2,[r1,r0]
	add r0,r0,#1
	b 0b
0:
	mov r0,#0
	strb_ r0,palette_number
	
	@disable GBA vblank interrupts while swapping buffers
	ldr addy,=REG_IE+REG_BASE
	ldrh r2,[addy]
	bic r2,r2,#1
	strh r2,[addy]
	@HANDS OFF R2!!!

	@flip scroll buffer
	ldr_ r0,scrollbuff
	ldr_ r1,dmascrollbuff
	str_ r1,scrollbuff
	str_ r0,dmascrollbuff
	@flip mirroring/bg bank buffer
	ldr_ r0,bg0cntbuff
	ldr_ r1,dmabg0cntbuff
	str_ r1,bg0cntbuff
	str_ r0,dmabg0cntbuff
	@flip bg/spr enable/disable buffer
	ldr_ r0,dispcntbuff
	ldr_ r1,dmadispcntbuff
	str_ r1,dispcntbuff
	str_ r0,dmadispcntbuff

	.if DIRTYTILES
	@recent tiles buffer
	ldr_ r0,recent_tiles
	ldr_ r1,dmarecent_tiles
	str_ r1,recent_tiles
	str_ r0,dmarecent_tiles
	ldr_ r0,recent_tilenum
	ldr_ r1,dmarecent_tilenum
	str_ r1,recent_tilenum
	str_ r0,dmarecent_tilenum
	.endif

	.if USE_BG_CACHE
	@advance bg cache
	ldr_ r0,bg_cache_produce_cursor
	str_ r0,bg_cache_produce_base_consume_end
	mov r0,#1
	strb_ r0,bg_cache_updateok
	
	@sanity check - if bg cache is blocked by a return instruction, make sure it is set as full
	ldr r3,=writeBG_bg_cache_full_mod
	ldr r1,=writeBG_instruction_2
	ldr r3,[r3]
	ldr r1,[r1]
	cmp r3,r1
	streqb_ r0,bg_cache_full
	
	.endif
	
	@flip 'bank buffer'
	adrl_ r3,bankbuffer
	ldmia r3,{r0,r1}
	str r1,[r3,#0]
	str r0,[r3,#4]
	
	@flip palette buffer
	ldr_ r0,current_palette
	ldr_ r1,visible_palette
	str_ r0,visible_palette
	str_ r1,current_palette
	
	@advance agb_bg_map to agb_bg_map_requested
	adrl_ r0,agb_bg_map
	adrl_ r1,agb_bg_map_requested
	ldmia r0,{r3,r4,r5,r6}
	stmia r1,{r3,r4,r5,r6}
	
	@NES OAM unchanged?
	ldrb_ r0,nesoamdirty
	movs r0,r0
	beq nesoam_was_clean
	
	mov r0,#0
	strb_ r0,nesoamdirty
	@flip nes sprite buffer if dirty
	ldr_ r0,nesoambuff
	ldr_ r1,dmanesoambuff
	str_ r1,nextnesoambuff
	str_ r0,dmanesoambuff
nesoam_was_clean:
	mov r0,#0x0FF00000
	str_ r0,nesoamdirty @set nesoamdirty = 0, consumetiles_ok = 0, frameready = F0, firstframeready = 0F
	
	@reenable GBA vblank interrupt
	orr r2,r2,#1
	strh r2,[addy]
	
	.if DIRTYTILES
	
	@do we have old recent tiles from last frame that never got drawn?  (used to be dma_recent_tilenum is now recent_tilenum)
	ldr_ r_tnum,recent_tilenum
	ldrsh tilenum,[r_tnum]
	movs tilenum,tilenum
	bmi 0f
	
	ldr_ r_tiles,recent_tiles
	bl_long _render_recent_tiles
	ldr_ r_tnum,recent_tilenum
	mov r0,#0x8000
	strh r0,[r_tnum]
0:	
	mov r0,#1
	strb_ r0,consumetiles_ok
	
@	bl _consume_recent_tiles_entry
	
	.endif

	@todo:
	@copy new chr changes to buffer
	
	@copy NES map to buffer?
	
	@wait for vblank

	ldrb_ r4,novblankwait_
	teq r4,#1					@NoVSync?
	beq l03
l01:
	mov r0,#0					@don't wait if not necessary
	mov r1,#1					@VBL wait
	swi 0x040000				@ Turn of CPU until VBLIRQ if not too late already.
	ldrb_ r0,PAL60				@wait for AGB VBL
	cmp r0,#5
	beq l01
	teq r4,#2					@Slomo?
	moveq r4,#0
	beq l01
l03:
	ldmfd sp!,{lr}
	bx lr
@	ldmfd sp!,{r3-r11,pc}

	.popsection
	
	.pushsection .text, "ax", %progbits
@----------------------------------------------------------------------------
_4014w:
dma_W:	@(4014)		sprite DMA transfer
@----------------------------------------------------------------------------
 PRIORITY = 0x800	@0x800=AGB OBJ priority 2/3
	@Quirk: DMA takes one less CPU cycle if the DMA transfer happens on an odd clock cycle
	@So divide timestamp by 3 to convert from PPU dots to CPU cycles, then AND with 1 to test if it's even or odd
	@fixme: inaccurate after timestamp wraps around from FFFFFFFF to 00000000
	
	@get timestamp
	ldr_ r2,cycles_to_run
	sub r2,r2,cycles,asr#CYC_SHIFT
	ldr_ r1,timestamp
	add r1,r1,r2
	ldr r2,=0x55555556
	umull r12,r1,r2,r1
	tst r1,#1
	
	ldr r1,=3*513*CYCLE
	sub cycles,cycles,r1
	subeq cycles,cycles,#3*CYCLE
	
	mov r1,#1
	strb_ r1,nesoamdirty
	
	and r1,r0,#0xe0
	ldr addy,[m6502_mmap,r1,lsr#3]
	and r0,r0,#0xff
	add r1,addy,r0,lsl#8	@addy=DMA source

@r0=dest, r1=src, r2=byte count

	ldr_ r0,nextnesoambuff
	str_ r0,nesoambuff
	mov r2,#256
	
	ldrb_ r12,oam_addr
	movs r12,r12
	bne_long 0f
	
	b memcpy32
	.popsection
	.pushsection .text, "ax", %progbits
dma_W_unaligned:

	@unaligned DMA copy
0:
	sub r2,r2,r12
	add r0,r0,r12
	stmfd sp!,{r12}
1:
	ldrb r12,[r1],#1
	strb r12,[r0],#1
	subs r2,r2,#1
	bne 1b
	ldmfd sp!,{r12}
	sub r0,r0,#256
1:
	ldrb r2,[r1],#1
	strb r2,[r0],#1
	subs r12,r12,#1
	bne 1b
	bx lr
	.popsection
	
	
	

	
@	@find where sprite 0 collides
@	@this is "yucky" because it only gets called once, and doesn't check again if the NES changes its graphics
@findsprite0
@	ldr_ addy,nesoambuff
@
@	ldrb_ r0,ppuctrl0	@8x16?
@	tst r0,#0x20
@	bne hit16
@@- - - - - - - - - - - - - 8x8 size
@hit8
@					@get sprite0 hit pos:
@	tst r0,#0x08			@CHR base? (0000/1000)
@	ldrb r0,[addy,#1]		@sprite tile#
@	
@	adr_ r1,vram_map
@	addne r1,r1,#16
@	mov r2,r0,lsr#6
@	ldr r1,[r1,r2,lsl#2]
@	@r1 = address of NES data
@	and r0,r0,#0x3F
@	add r0,r1,r0,lsl#4		@r0=NES base+tile*16
@	
@	mov addy,#0
@
@	@note: does not yet factor in Y flipping
@
@1
@	ldr r2,[r0],#4
@	ldr r1,[r0,#4]
@	orr r1,r1,r2
@	tst   r1,#0x000000FF
@	addeq addy,addy,#1
@	tsteq r1,#0x0000FF00
@	addeq addy,addy,#1
@	tsteq r1,#0x00FF0000
@	addeq addy,addy,#1
@	tsteq r1,#0xFF000000
@	addeq addy,addy,#1
@	bne 0f
@	cmp addy,#8
@	blt 1b
@0	
@	
@	ldr_ r0,nesoambuff
@	ldrb r1,[r0]
@	add r1,r1,#2	@add 2, one for sprite y correction, one for prerender scanline
@	add r1,r1,addy
@@	moveq r1,#512			@blank tile=no hit
@	cmp r1,#240
@	movhi r1,#512			@no hit if Y>239
@	str r1,sprite0y
@	ldrb r1,[r0,#3]		@r1=sprite0 x
@	strb r1,sprite0x
@	bx lr
@hit16
@@- - - - - - - - - - - - - 8x16 size
@	ldrb r0,[addy,#1]		@sprite tile#
@	tst r0,#0x01
@	adr_ r1,vram_map
@	addne r1,r1,#16
@	mov r2,r0,lsr#6
@	ldr r1,[r1,r2,lsl#2]
@	@r1 = address of NES data
@	and r0,r0,#0x3E
@	add r0,r1,r0,lsl#4		@r0=NES base+tile*16
@	
@	mov addy,#0
@
@	@note: does not yet factor in Y flipping
@1
@	ldr r2,[r0],#4
@	ldr r1,[r0,#4]
@	orr r1,r1,r2
@	tst   r1,#0x000000FF
@	addeq addy,addy,#1
@	tsteq r1,#0x0000FF00
@	addeq addy,addy,#1
@	tsteq r1,#0x00FF0000
@	addeq addy,addy,#1
@	tsteq r1,#0xFF000000
@	addeq addy,addy,#1
@	bne 0f
@	tst addy,#0x04
@	bne 1b
@	tst addy,#0x10
@	addeq r0,r0,#8
@	beq 1b
@0
@	ldr_ r0,nesoambuff
@	ldrb r1,[r0]
@	add r1,r1,#2	@add 2, one for sprite y correction, one for prerender scanline
@	add r1,r1,addy
@@	moveq r1,#512			@blank tile=no hit
@	cmp r1,#240
@	movhi r1,#512			@no hit if Y>239
@	str r1,sprite0y
@	ldrb r1,[r0,#3]		@r1=sprite0 x
@	strb r1,sprite0x
@	bx lr

update_sprites:
	@call this to finish the job
	stmfd sp!,{lr}
	mov r0,#1
	str r0,alreadylooked
	
update_sprites_enter:
	
	mov r11,#PRIORITY
	
	ldr_ r7,dmascrollbuff
	add r7,r7,#1
	ldr_ r8,dmabankbuffer
	ldr r9,=spr_cache_map
	

	ldr_ addy,dmanesoambuff

@	ldr r2,oambuffer+4		@r2=dest
@	ldr r1,oambuffer+8
@	ldr r0,oambuffer
@	str r2,oambuffer
@	str r1,oambuffer+4
@	str r0,oambuffer+8
	mov r2,#AGB_OAM

	ldr_ r1,emuflags
	and r5,r1,#0x300
	cmp r5,#SCALED_SPRITES*256
	moveq r6,#0x300			@r6=rot/scale flag + double
	movne r6,#0

	cmp r5,#UNSCALED_AUTO*256	@do autoscroll
	bne dm0
	ldr_ r3,AGBjoypad
	ands r3,r3,#0x300
	eornes r3,r3,#0x300
	bne dm0					@stop if L or R pressed (manual scroll)
	mov r3,r1,lsr#16		@r3=follow value
	tst r1,#FOLLOWMEM
	ldreqb r0,[addy,r3,lsl#2]			@follow sprite
	ldrne r0,=NES_RAM
	ldrneb r0,[r0,r3]			@follow memory
	cmp r0,#239
	bhi dm0
	add r0,r0,r0,lsl#2
	mov r0,r0,lsr#4
	strb_ r0,windowtop
dm0:
	ldrb_ r0,windowtop @FIXME
	ldr r5,=YSCALE_LOOKUP
	sub r5,r5,r0

	ldrb_ r0,ppuctrl0frame	@8x16?
	tst r0,#0x20
	bne dm4
	@-------------------------- 8 x 8 ---------------------------

@	mov r4,#PRIORITY
@	tst r0,#0x08			@CHR base? (0000/1000)
@	moveq r4,#0+PRIORITY	@r4=CHR set+AGB priority
@	movne r4,#0x100+PRIORITY
dm11:
	ldr r3,[addy],#4		@r3 = 4 bytes from NES OAM (X vhp...pp T Y)  X position, vertical flip, horizontal flip, priority, palette, tile, Y position
	and r0,r3,#0xff			@r0 = Y
	cmp r0,#239
	bhi dm10				@skip if sprite Y>239

@	spr_ptable=(scrollbuff[spr_y*4]&0x80)/0x20;
@	spr_t=oambuff[i*4+1];
@	spr_high_t=spr_t>>6;
@	spr_bank=bankbuffer[(spr_y&0xF8)+spr_ptable+spr_high_t];
@ 	cache_bank=map[spr_bank];

	@(y/8)*8
	and r4,r0,#0xF8
	@fetch from scroll buffer
	ldrb r1,[r7,r0,lsl#2]
	and r1,r1,#0x80
	@add 4 if requests right pattern table
	add r4,r4,r1,lsr#5
	and r1,r3,#0xC000
	@add sprite number's KB position
	add r4,r4,r1,lsr#14
	@Get Bank number
	ldrb r4,[r8,r4]
	@Get Sprite Number/64
	ldrsb r4,[r9,r4]
	@mult by 64
	tst r4,#0x80000000
	add r4,r11,r4,lsl#6
	blne_long need_to_fetch_sprite_data
	@get tile number
	and r1,r3,#0x3F00
	add r4,r4,r1,lsr#8
	@end new code

	ldrb r0,[r5,r0]			@y = scaled y

	ands r1,r6,#0x100		@If we are using sprite scaling, we will subtract 4 from the position of the sprite later
	add r1,r1,#SCREEN_LEFT << 6		@add screen left, shifted value corrected later
	tstne r3,#0x00400000	@NES horizontal flip
	addne r1,r1,#0x40		@If scaling sprites, add one pixel to X coordinate when flipping if scaling

	subs r1,r3,r1,lsl#18	@Subtract X coordinate (ignoring rest of bits)
@#0x0c000000	@x-8
	and r1,r1,#0xff000000	@mask only X coordinate
	orr r0,r0,r1,lsr#8		@shift to GBA bit position
	orrcc r0,r0,#0x01000000	@Adjustment?
	and r1,r3,#0x00c00000	@flip
	orr r0,r0,r1,lsl#6		@set GBA bit position
	and r1,r3,#0x00200000	@priority
	orr r0,r0,r1,lsr#11		@Set Transp OBJ.
	orr r0,r0,r6			@set rot/scale bits
	str r0,[r2],#4			@store OBJ Atr 0,1
	@GBA OAM attributes:
	@SS p M mm d s yyyyyyyy    shape, 256 color palettes, mosaic, transparency mode, double size, scale, Y coordinate
	@ss vh rrrrr xxxxxxxx      size, vertical/horizontal flip, rotation/scale matrix selection, X coordinate
	@PPPP pp tttttttttt        palette, priority, tile
	and r0,r3,#0x00030000
	orr r0,r4,r0,lsr#4
@	and r1,r3,#0x0000ff00	@tile#
@	and r0,r3,#0x00030000	@color
@	orr r0,r1,r0,lsl#4
@	orr r0,r4,r0,lsr#8		@tileset+priority
	strh r0,[r2],#4			@store OBJ Atr 2
dm9:
	tst addy,#0xff
	bne dm11
	ldmfd sp!,{pc}
dm10:
	mov r0,#0x2a0			@double, y=160
	str r0,[r2],#8
	b dm9

dm4:	@----------------------- 8 x 16 -----------------------------
	orr r6,r6,#0x8000		@8x16 flag
dm12:
	ldr r3,[addy],#4
	and r0,r3,#0xff
	cmp r0,#239
	bhi dm13				@skip if sprite Y>239

	@new code
	and r4,r0,#0xF8
	and r1,r3,#0x0100
	add r4,r4,r1,lsr#6
	and r1,r3,#0xC000
	add r4,r4,r1,lsr#14
	ldrb r4,[r8,r4]
	ldrsb r4,[r9,r4]
	tst r4,#0x80000000
	add r4,r11,r4,lsl#6
	blne_long need_to_fetch_sprite_data
	@get tile number
	and r1,r3,#0x3E00
	add r4,r4,r1,lsr#8
	@end new code




	tst r6,#0x300
	subne r0,r0,#5
	andne r0,r0,#0xff
	ldrb r0,[r5,r0]			@y

	ands r1,r6,#0x100
	add r1,r1,#SCREEN_LEFT << 6
	tstne r3,#0x00400000
	addne r1,r1,#0x40

	subs r1,r3,r1,lsl#18
@#0x0c000000	@x-8
	and r1,r1,#0xff000000
	orr r0,r0,r1,lsr#8
	orrcc r0,r0,#0x01000000
	and r1,r3,#0x00c00000	@flip
	orr r0,r0,r1,lsl#6
	and r1,r3,#0x00200000	@priority
	orr r0,r0,r1,lsr#11		@Set Transp OBJ.
	orr r0,r0,r6			@8x16+rot/scale
	str r0,[r2],#4			@store OBJ Atr 0,1
	
	and r0,r3,#0x00030000
	orr r0,r4,r0,lsr#4
@	and r1,r3,#0x0000ff00	@tile#
@	movs r0,r1,lsr#9
@	orrcs r0,r0,#0x80
@	orr r0,r4,r0,lsl#1		@priority, tile#*2
@	and r1,r3,#0x00030000	@color
@	orr r0,r0,r1,lsr#4
	strh r0,[r2],#4			@store OBJ Atr 2
dm14:
	tst addy,#0xff
	bne dm12
	ldmfd sp!,{pc}
dm13:
	mov r0,#0x2a0			@double, y=160
	str r0,[r2],#8
	b dm14

alreadylooked:
	.word 0

@----------------------------------------------------------------------------
PPU_R:@
@----------------------------------------------------------------------------
@DO NOT TOUCH R12 (addy), it needs to be preserved for Read-Modify-Write instructions
	and r0,addy,#7
	ldr pc,[pc,r0,lsl#2]
	.word 0
PPU_read_tbl:
	.word empty_PPU_R	@$2000
	.word empty_PPU_R	@$2001
	.word stat_R_simple @$2002
	.word empty_PPU_R	@$2003
	.word ppu2004_r	@$2004
	.word empty_PPU_R	@$2005
	.word empty_PPU_R	@$2006
	.word vmdata_R	@$2007
@----------------------------------------------------------------------------
PPU_W:@
@----------------------------------------------------------------------------
	and r2,addy,#7
	ldr pc,[pc,r2,lsl#2]
	.word 0
PPU_write_tbl:
	.word ctrl0_W		@$2000
	.word ctrl1_W		@$2001
	.word void		@$2002
	.word ppu2003_W	@$2003
	.word ppu2004_W	@$2004
	.word bgscroll_W_screen_off	@$2005
	.word vmaddr_W_screen_off	@$2006
	.word vmdata_W	@$2007


@----------------------------------------------------------------------------
empty_PPU_R:
@----------------------------------------------------------------------------
	mov r0,#0
	mov pc,lr

.pushsection .text, "ax", %progbits
ctrl0_trigger_nmi:
	stmfd sp!,{r0,r2,r12,lr}
	bl ppuctrl0_install_NMI_timeout		@trigger an NMI becuase NMI enable flag became set while Vblank flag is on
	ldmfd sp!,{r0,r2,r12,lr}
	b_long ctrl0_continue
.popsection

@----------------------------------------------------------------------------
ctrl0_W:		@(2000)  PPUCTRL
@----------------------------------------------------------------------------
	ldrb_ r1,ppuctrl0	@read previous value  (lowest two bits of previous value are meaningless)
	strb_ r0,ppuctrl0	@write new value
	eor r2,r1,r0		@find what bits have changed bewteen old and new value
	tst r2,#0x80		@NMI enable bit has changed?
	tstne r0,#0x80		@NMI enable bit has turned on?
	beq 0f				@jump ahead if not.
	
	ldrb r1,ppustat_	@check if vblank flag is on
	tst r1,#0x80
	bne_long ctrl0_trigger_nmi
ctrl0_continue:
0:
	tst r2,#0x10		@check if background tileset selection has changed
	beq 0f				@jump ahead if not
	stmfd sp!,{r0,r2,r12,lr}
	blne updateBGCHR_	@check for tileset switch (OBJ CHR gets checked at frame end)
	ldmfd sp!,{r0,r2,r12,lr}
0:
	tst r2,#4			@check if increment mode has changed
	beq 0f				@jump ahead if not
	tst r0,#4			@requesting 1 or 32?
	moveq r1,#1
	movne r1,#32
	strb_ r1,vramaddrinc		@store dummy value (getting rid of this later)
	strb r1,vramaddrinc_modify1	@store to PPU_WRITE modifyable instruction (add xx,xx,#1)
	strb r1,vramaddrinc_modify2	@store to PPU_READ modifyable instruction (add xx,xx,#1)
0:
	@set bits of loopy_t
	@loopy_t:...xx.......... = r0:......xx
	ldr_ r1,loopy_t
	bic r1,r1,#0xC00
	and addy,r0,#0x03
	orr r1,r1,addy,lsl#10
	str_ r1,loopy_t
	
	@hacky code: make most significant bit of GBA scroll buffer equal to sprite pattern table selection
	and addy,r0,#1
	tst r0,#0x08
	orrne addy,addy,#0x80
	ldrb_ r1,scrollX+1
	cmp addy,r1
	bxeq lr
	strb_ addy,scrollX+1
	ldrb_ r1,screen_off
	movs r1,r1
	bxne lr
	b newX

@	mov r1,r0,lsr#1			@Y scroll
@	and r1,r1,#1			@ should be 1
@	strb_ r1,scrollY+1

@----------------------------------------------------------------------------
ctrl1_W:		@(2001)  PPUMASK
@----------------------------------------------------------------------------
	ldrb_ r1,ppuctrl1
	strb_ r0,ppuctrl1
	eors r2,r1,r0
	bxeq lr	@return if nothing changed
ctrl1_W_force:  @set r1 to r0, and r2 to 00 before calling this, bypasses the "has this changed" check (for loading savestates)
	stmfd sp!,{r0,lr}
	
	@have monochrome mode or color emphasis bits changed?
	tst r2,#0xE1
	@update palette
	beq 0f
@	stmfd sp!,{r2}
	bl_long store_palette
@	ldmfd sp!,{r2}
0:
	
	@has sprite/bg enable changed?
	tst r2,#0x18
	beq 0f
	
	@if we're inside vblank, we can't turn screen off or on
	ldrb_ r2,inside_vblank
	movs r2,r2
	bne 0f
	
	@do we want to turn off the screen?
	tst r0,#0x18
	adr lr,0f
	beq_long turn_screen_off
	@if screen was off, we want to turn on the screen
	tst r1,#0x18
	beq_long turn_screen_on
0:	
	bl get_scanline_2
	ldmfd sp!,{r1,lr}	@r1 is now the value written to PPUMASK instead of r0

@	ldr r0,=0x2440		@1d sprites, BG2 enable, Window enable. DISPCNTBUFF startvalue. 0x2440
	mov r0,#0x0440		@1d sprites, BG2 enable, Window disable. DISPCNTBUFF startvalue. 0x0440
	tst r1,#0x08		@bg en?
	orrne r0,r0,#0x0100
	tst r1,#0x10		@obj en?
	orrne r0,r0,#0x1000

	ldr_ r1,ctrl1old	@r1=lastval
	str_ r0,ctrl1old

	cmp addy,#240
	movhi addy,#240

ctrl1finish:
	@r0=lastline, lastline=scanline
	ldrb_ r0,ctrl1line
	strb_ addy,ctrl1line
	@sl == 0 >> exit
	@sl < prev  >> prev = 0
	@sl == prev >> exit
	cmp addy,#0
	bxeq lr
	subs r2,addy,r0
	bxeq lr
	addlt r2,r2,r0  @could this happen?
	movlt r0,#0
	@addy = number of scanlines to fill

	ldr_ addy,dispcntbuff
	add r0,addy,r0,lsl#1
	b memset16
@ct1
@	strh r0,[r1],#2 	@fill forwards from lastline to scanline-1
@	subs addy,addy,#1
@	bgt ct1
@
@	mov pc,lr

@update_Y_hit
@	adr r1,stat_R_sameline
@	str r1,PPU_read_tbl+8
@	ldr r1,sprite0y
@	strb r1,stat_R_Y_modify
@	ldrb r1,sprite0x
@	@maybe put PAL fixing code here later
@	rsb r1,r1,#255
@	strb r1,stat_R_X_modify
@	bx lr	

@stat_R_ppuhack
@	ldr_ r0,scanline
@	ldr r1,ppuhack_line
@	str r0,ppuhack_line
@	cmp r0,r1
@	bne 0f
@	ldrb r2,ppuhack_count
@	add r2,r2,#1
@	strb r2,ppuhack_count
@	cmp r2,#5
@	ble 1f
@	andgt cycles,cycles,#CYC_MASK	@skip this scanline
@0:
@	strb m6502_a,ppuhack_count
@1:

@DO NOT TOUCH R12 (addy), it needs to be preserved for Read-Modify-Write instructions
stat_R:
stat_R_simple:
	strb_ m6502_a,toggle
ppustat_:
	mov r0,#1
	bx lr

stat_R_clearvbl:
	strb_ m6502_a,toggle
	ldr_ r0,stat_r_simple_func
	str r0,PPU_read_tbl+8
	ldrb r0,ppustat_
	bic r1,r0,#0x80
	strb r1,ppustat_
	bx lr

@stat_R_sameline
@	strb_ m6502_a,toggle
@	ldrb r0,ppustat_
@	ldr_ r1,scanline
@stat_R_Y_modify
@	cmp r1,#1
@	bne stat_R_hit	@scanline doesn't match: hit
@stat_R_X_modify
@	cmp cycles,#255*CYCLE	@cycles < (256-X)*CYCLE  then  Hit
@	bxgt lr	@no hit otherwise
@stat_R_hit
@	orr r0,r0,#0x40
@	strb r0,ppustat_
@	ldr_ r1,stat_r_simple_func
@	str r1,PPU_read_tbl+8
@	bx lr

@----------------------------------------------------------------------------
bgscroll_W_screen_off:	@(2005)  PPUSCROLL
@----------------------------------------------------------------------------
	ldrb_ r1,toggle
	eors r1,r1,#1
	strb_ r1,toggle
	beq bgscrollY_screen_off
	
	@set loopy_t:...xxxxx to r0:xxxxx...
	ldrb_ r1,loopy_t
	bic r1,r1,#0x1F
	orr r1,r1,r0,lsr#3
	strb_ r1,loopy_t
	strb_ r0,scrollX
	bx lr

bgscrollY_screen_off:
	@set loopy_t:.....xxxxx..... to r0:xxxxx...
	@and loopy_t:xxx............ to r0:.....xxx
	ldr_ r1,loopy_t
	bic r1,r1,#0x03E0
	bic r1,r1,#0x7000
	and r2,r0,#0xF8
	orr r1,r1,r2,lsl#2
	and r2,r0,#0x07
	orr r1,r1,r2,lsl#12
	str_ r1,loopy_t
	bx lr

@----------------------------------------------------------------------------
bgscroll_W_screen_on:	@(2005)  PPUSCROLL
@----------------------------------------------------------------------------
	ldrb_ r1,toggle
	eors r1,r1,#1
	strb_ r1,toggle
	beq bgscrollY

	@set loopy_t
	ldrb_ addy,loopy_t
	bic addy,addy,#0x1F
	mov r1,r0,lsr#3
	orr addy,addy,r1
	strb_ addy,loopy_t
	
	ldrb_ r1,scrollX_v
	
	bic r2,r1,#7
	and r12,r0,#7
	orr r2,r2,r12
	
	ldrb_ r12,scrollX
	@r12 = old scrollX, r1=old scrollX_v, r2=new scrollX_v, r0=new scrollX
	@return if old == new for all of them
	cmp r1,r2
	strneb_ r2,scrollX_v
	cmp r12,r0
	strneb_ r0,scrollX
	cmpeq r1,r2
	bxeq lr
	
newX:			@ctrl0_W, loadstate jumps here
	ldr_ r1,scrollX
	ldr_ r0,scrollX_v
	cmp r0,r1
	beq 1f
	
	ldrb_ r2,screen_off
	movs r2,r2
	bne 1f
	
	stmfd sp!,{r0,lr}
	bl install_scrollx_timeout
	ldmfd sp!,{r0,lr}
newX2:			@vmaddr_W jumps here
1:
	stmfd sp!,{lr}
	bl get_scanline_2
	ldmfd sp!,{lr}

newX2_entry:
	adr_ r1,scrollXold
	swp r0,r0,[r1]		@r0=lastval

	@get the real scanline

	cmp addy,#240
	movhi addy,#240
	@r1=lastline, lastline=scanline, addy=scanline
	ldrb_ r1,scrollXline
scrollXfinish:		@turn_screen_off calls this
	strb_ addy,scrollXline
	@sl == 0 >> exit
	@sl < prev  >> prev = 0
	@sl == prev >> exit
	cmp addy,#0
	bxeq lr
	subs addy,addy,r1
	bxeq lr
	addlt addy,addy,r1  @could this happen?
	movlt r1,#0
	@addy = number of scanlines to fill
	
	add r0,r0,#SCREEN_LEFT @add 8 for screen offset
	ldr_ r2,scrollbuff
	@should not be zero
	add r1,r2,r1,lsl#2

memset16_alternating:
	subs addy,addy,#4
	blt 0f
1:
	strh r0,[r1],#4
	strh r0,[r1],#4
	strh r0,[r1],#4
	strh r0,[r1],#4
	subs addy,addy,#4
	bge 1b
@	strh r0,[r1],#4
@	strh r0,[r1],#4
@	strh r0,[r1],#4
@	strh r0,[r1],#4
@	subs addy,addy,#4
@	bge 1b
0:
	adds addy,addy,#4
	bxeq lr
1:
	strh r0,[r1],#4
	subs addy,addy,#1
	bgt 1b
	bx lr

@scrollXold: .word 0 @last write
@scrollXline: .word 0 @..was when?

bgscrollY:
	strb_ r0,scrollY	@do we need this?

	ldr_ r1,loopy_t	@the link between Y scrolling and VRAM address
	bic r1,r1,#0x7300
	bic r1,r1,#0x00e0
	and r2,r0,#0xf8
	and r0,r0,#7
	orr r1,r1,r2,lsl#2
	orr r1,r1,r0,lsl#12
	str_ r1,loopy_t
	
	mov pc,lr

@----------------------------------------------------------------------------
vmaddr_W_screen_off:	@(2006)  VRAMADDR
@----------------------------------------------------------------------------
	ldrb_ r1,toggle
	eors r1,r1,#1
	strb_ r1,toggle
	@runs if toggle was 0
	andne r0,r0,#0x3F
	strneb_ r0,loopy_t+1
	bxne lr
	@runs if toggle was 1
	strb_ r0,loopy_t
	ldr_ r1,loopy_t
	str_ r1,vramaddr
	bx lr
	
@----------------------------------------------------------------------------
vmaddr_W_screen_on:		@(2006)  VRAMADDR
@----------------------------------------------------------------------------
	ldrb_ r1,toggle
	eors r1,r1,#1
	strb_ r1,toggle
	@runs if toggle was 0
	andne r0,r0,#0x3f
	strneb_ r0,loopy_t+1
	bxne lr
	@runs if toggle was 1
	strb_ r0,loopy_t
	ldr_ r1,loopy_t
	str_ r1,vramaddr

UpdateXYScroll_V_equals_T:
	@split vramaddr into Y scrolling components:
	@y:......xxx = loopy_v:xxx............ (right shift 12)
	@y:.xxxxx... = loopy_v:.....xxxxx..... (right shift 2)
	@y:x........ = loopy_v:...x........... (right shift 3)
	mov r0,r1,lsr#12
	and r2,r1,#0x03E0
	orr r0,r0,r2,lsr#2
	and r2,r1,#0x0800
	orr r0,r0,r2,lsr#3
	str_ r0,scrollY

	@split vramaddr into X scrolling components:
	@x:.xxxxx... = loopy_v:..........xxxxx
	@x:x........ = loopy_v:....x..........
	ldr_ r0,scrollX		@preserve low 3 bits and CHR Bank bit (most significant bit)
	bic r0,r0,#0x7F00
	bic r0,r0,#0x00F8
	and r2,r1,#0x001f
	and addy,r1,#0x0400
	orr r0,r0,r2,lsl#3
	orr r0,r0,addy,lsr#2
	str_ r0,scrollX
	str_ r0,scrollX_v

	str lr,[sp,#-4]!
	bl newX2
	ldr lr,[sp],#4
	b newY

UpdateXYScroll:
	@r1 = vramaddr
	@split vramaddr into Y scrolling components:
	@y:......xxx = loopy_v:xxx............ (right shift 12)
	@y:.xxxxx... = loopy_v:.....xxxxx..... (right shift 2)
	@y:x........ = loopy_v:...x........... (right shift 3)
	mov r0,r1,lsr#12
	and r2,r1,#0x03E0
	orr r0,r0,r2,lsr#2
	and r2,r1,#0x0800
	orr r0,r0,r2,lsr#3
	str_ r0,scrollY
	
	@split vramaddr into X scrolling components:
	@x:.xxxxx... = loopy_v:..........xxxxx
	@x:x........ = loopy_v:....x..........
	ldr_ r0,scrollX		@preserve low 3 bits and CHR Bank bit (most significant bit)
	bic r0,r0,#0x7F00
	bic r0,r0,#0x00F8
	and r2,r1,#0x001f
	orr r0,r0,r2,lsl#3
	and r2,r1,#0x0400
	orr r0,r0,r2,lsr#2
	str_ r0,scrollX_v
	
	@split loopy_t into X scrolling components
	ldr_ addy,loopy_t
	bic r0,r0,#0x7F00
	bic r0,r0,#0x00F8
	and r2,addy,#0x001F
	orr r0,r0,r2,lsl#3
	and r2,addy,#0x0400
	orr r0,r0,r2,lsr#2
	str_ r0,scrollX
	
	str lr,[sp,#-4]!
	bl newX
	ldr lr,[sp],#4
	@falls through to newY
@- - - - - -
newY:
	ldr_ r0,scrollY
	str_ r0,scrollY_v
newY_2:
	stmfd sp!,{lr}
	bl get_scanline_2
	str_ r1,scroll_y_timestamp
	ldmfd sp!,{lr}
	cmp r2,#0x60000000
	bcc 0f
	ldr_ r1,scroll_threshhold_value	@239/341, about 0xb3000000...
	cmp r2,r1
	bcs 0f
	.if DRAW_ATTRIBUTE_TABLES
	cmp addy,#0		@will we need this later?
	beq 0f
	add r0,r0,#1
	and r1,r0,#0xFF
	cmp r1,#0
	addeq r0,r0,#256
	cmp r1,#240
	addeq r0,r0,#16
	bic r0,r0,#512
	.else
	cmp addy,#0	
	addne r0,r0,#1
	.endif
0:
	ldr_ r2,scrollYold
	str_ r0,scrollYold
	
	cmp addy,#240
	movhi addy,#240
	
	@r1=lastline, lastline=scanline
	@r2=scroll value to fill from, addy=scanline to fill to
	ldrb_ r1,scrollYline
scrollYfinish:
	strb_ addy,scrollYline

	@sl == 0 >> exit
	@sl < prev  >> prev = 0
	@sl == prev >> exit
	cmp addy,#0
	bxeq lr
	subs addy,addy,r1
	bxeq lr
	addlt addy,addy,r1  @could this happen?
	
	stmfd sp!,{r3,r4,lr}
	mov r3,r2
	sub r0,r3,r1
	movlt r1,#0
	
	@addy = number of scanlines to fill
	ldr_ r2,scrollbuff
	add r1,r2,r1,lsl#2
	add r1,r1,#2

recheck239:	
	@do the 239 check
	and r2,r3,#0xFF
	add r2,r2,addy
	subs r2,r2,#240
	bgt sy_goesover
	bl memset16_alternating
	ldmfd sp!,{r3,r4,pc}

sy_goesover:
	@r2 = scanline count amount after moving down
	@addy = total scanline count
	@if addy<=r2, then source was offscreen
	subs r4,addy,r2
	ble sy_wasover
	add r3,r0,#16
	mov addy,r4
	bl memset16_alternating
	mov r0,r3
	mov addy,r2
	bl memset16_alternating
	ldmfd sp!,{r3,r4,pc}

sy_wasover:
	.if DRAW_ATTRIBUTE_TABLES
	@add 16 to r4 to get scanlines remaining until 256
	add r4,r4,#16
	@get remaining scanlines after r4
	subs r2,addy,r4
	add r3,r0,#256
	bgt sy2
	b sy1	
	.else
	add r0,r0,#240
	add r3,r3,#240
	b recheck239
	.endif

@scrollYold: .word 0 @last write
@scrollYline: .word 0 @..was when?

	.pushsection .text, "ax", %progbits
vmdata_R_screen_on:
	@reading 2007 with the screen on shifts the display on increments Y scroll by 1
	stmfd sp!,{r3,r4,r12,lr}
	bl get_scrolly_scanline
	@r1 = Y scroll
	@if r1 & FF == FF, subtract 0x100
	@if r1 & FF == EF, add 0x10
	and r2,r1,#0xFF
	add r1,r1,#1
	cmp r2,#0xFF
	subeq r1,r1,#0x100
	cmp r2,#0xEF
	addeq r1,r1,#0x10
	bic r1,r1,#0x200
	str_ r1,scrollY
	bl_long newY
	mov r0,#0
	ldmfd sp!,{r3,r4,r12,pc}

	.popsection

@----------------------------------------------------------------------------
vmdata_R:	@(2007)  PPUDATA
@----------------------------------------------------------------------------
@DO NOT TOUCH R12 (addy), it needs to be preserved for Read-Modify-Write instructions
	ldr_ r0,vramaddr
vramaddrinc_modify2:
	add r2,r0,#0xFF		@self modify code, this is changed to 0x01 or 0x20 depending on what's written to PPUCTRL
	strh_ r2,vramaddr
	bic r0,r0,#0xfc000

	and r1,r0,#0x3c00
	adr_ r2,vram_map
	ldr r1,[r2,r1,lsr#8]
	bic r2,r0,#0xfc00

	ldrb r1,[r1,r2]

	cmp r0,#0x3f00
	bhs_long palread

	ldrb_ r0,readtemp
	strb_ r1,readtemp
	mov pc,lr

	.pushsection .text, "ax", %progbits
palread:
	strb_ r1,readtemp
	
	and r0,r0,#0x1f
	adr_ r1,nes_palette
	ldrb r0,[r1,r0]
	mov pc,lr
	.popsection

@----------------------------------------------------------------------------
vmdata_W:	@(2007)
@----------------------------------------------------------------------------
	ldr_ addy,vramaddr
vramaddrinc_modify1:
	add r2,addy,#0xFF	@self modify code, this is changed to 0x01 or 0x20 depending on what is written to PPUCTRL
	strh_ r2,vramaddr
	bic addy,addy,#0xfc000 @AND $3fff

vram_write_direct:
	ands r1,addy,#0x3c00
	adr_ r2,vram_write_tbl
vram_write_modify:
	ldr pc,[r2,r1,lsr#8]	@self modify code, this is changed to ldrne when using CHR-RAM
	@this will fall thru for VRAM_chr 0000-03FF
@----------------------------------------------------------------------------
VRAM_chr:@	0000-1fff
@----------------------------------------------------------------------------
	ldr_ r2,nes_vram
VRAM_chr_entry:
	strb r0,[r2,addy]
	.if DIRTYTILES
@	mov r0,#1
	@this is okay as long as r2 is not 00
	add r2,r2,#dirty_tiles-NES_VRAM
	strb r2,[r2,addy,lsr#4]
	add r2,r2,#dirty_rows-dirty_tiles
	strb r2,[r2,addy,lsr#8]
	bx lr
	.else
	bic addy,addy,#8
	ldrb r0,[r2,addy]!	@read 1st plane
	ldrb r1,[r2,#8]		@read 2nd plane

	ldr r2,=CHR_DECODE
@	adr_ r2,chr_decode	
	
	ldr r0,[r2,r0,lsl#2]
	ldr r1,[r2,r1,lsl#2]
	orr r0,r0,r1,lsl#1

	and r2,addy,#7		@r2=tile line#
	add addy,addy,r2
	add r1,addy,addy
	add r1,r1,#BG_VRAM		@AGB BG tileset
	add addy,r1,#(SPR_VRAM-BG_VRAM)+SPR_CACHE_OFFSET
	tst r1,#0x2000		@1st or 2nd page?
	addne r1,r1,#0x2000	@0000/4000 for BG, 14000/16000 for OBJ

	str r0,[r1]
	str r0,[addy]

	bx lr
	.endif
	
	.if MIXED_VRAM_VROM
	.if DIRTYTILES
@----------------------------------------------------------------------------
VRAM_chr3:@	0000-1fff
@----------------------------------------------------------------------------
	mov r2,addy,lsr#10
	adr_ r1,vram_map
	ldr r1,[r1,r2,lsl#2]	@r1 = physical address of the memory block, one of +(000,400,800,c00,1000,1400,1800,1c00)
	ldr_ r2,nes_vram		@r2 = "nes_vram", physical address of memory start
	@want to see if (r1-r2) is between 0 and 2000
	subs r1,r1,r2
	bxlt lr @bounds checking
	cmp r1,#0x2000
	bxge lr @bounds checking
	
	bic addy,addy,#0xFC00
	add addy,addy,r1
	b VRAM_chr_entry
	.endif
	.endif


	.if USE_BG_CACHE
@r1 = address & 0x3C00
@bits guaranteed to be set in r1: 0x2000
@r1 >> 8 = an address in a word table to a word (such as an address)
@r1 >> 10 = an address is a byte table (such as a tile number offset)

@	adr r2,some_table-32
@	ldr r1,[r2,r1,lsr#8]!
@	ldr r2,[r2,#8]

@----------------------------------------------------------------------------
VRAM_name0:	@(2000-23ff)
@----------------------------------------------------------------------------
	ldr r1,=NES_VRAM2
	mov r2,#0
writeBG:
	bic addy,addy,#0xFC00
	strb r0,[r1,addy]
writeBG_mapper_9_mod:
	nop
writeBG_mapper_9_mod_return:
writeBG_bg_cache_full_mod:
	@Self-Modify code: replace with bx lr if bg cache is full
	add addy,addy,r2
	@store
	ldr r1,=BG_CACHE
	ldr_ r2,bg_cache_produce_cursor
	strh addy,[r1,r2]
	add r2,r2,#2
	bic r2,r2,#BG_CACHE_SIZE
	str_ r2,bg_cache_produce_cursor
writeBG_bg_cache_produce_limit_consume_begin_mod_high:
	sub r2,r2,#0xFF00
writeBG_bg_cache_produce_limit_consume_begin_mod_low:
	subs r2,r2,#0xFF
	bxne lr
	
	@set full
	b_long set_bg_cache_full
	.pushsection .text, "ax", %progbits
set_bg_cache_full:
	@called from main thread
	@suppress GBA VBLANK interrupt so variables can't change
	ldr addy,=REG_IE + REG_BASE
	ldrh r2,[addy]
	bic r2,r2,#1
	strh r2,[addy]
	
	@verify equality (other thread can modify code in the middle of the comparison)
	ldr_ r0,bg_cache_produce_limit_consume_begin
	ldr_ r1,bg_cache_produce_cursor
	cmp r0,r1
	bne set_bg_cache_full_nomatch
	
	mov r1,#1
	strb_ r1,bg_cache_full
	ldr r1,writeBG_instruction_2
	ldr r0,=writeBG_bg_cache_full_mod
	str r1,[r0]
	
set_bg_cache_full_nomatch:
	orr r2,r2,#1
	strh r2,[addy]
	
	@continue to return instruction below
writeBG_instruction_2:
	bx lr
	
set_bg_cache_available:
	@called from consume code when drawing entire map
	mov r1,#0
	strb_ r1,bg_cache_full
	ldr r1,writeBG_instruction_1
	ldr r0,=writeBG_bg_cache_full_mod
	str r1,[r0]
	bx lr
	
writeBG_instruction_1:
	add addy,addy,r2

set_bg_cache_produce_limit_consume_begin:
	@called from consume code
	str_ r0,bg_cache_produce_limit_consume_begin
	ldr r1,=writeBG_bg_cache_produce_limit_consume_begin_mod_high
	strb r0,[r1,#4]
	mov r0,r0,lsr#8
	strb r0,[r1]
	bx lr
	
	.popsection


@----------------------------------------------------------------------------
VRAM_name1:	@(2400-27ff)
@----------------------------------------------------------------------------
	ldr r1,=NES_VRAM2+0x400
	mov r2,#0x400
	b writeBG
@----------------------------------------------------------------------------
VRAM_name2:	@(2800-2bff)
@---------------------------------------------------------------------------
	ldr r1,=NES_VRAM4
	mov r2,#0x800
	b writeBG
@----------------------------------------------------------------------------
VRAM_name3:	@(2c00-2fff)
@----------------------------------------------------------------------------
	ldr r1,=NES_VRAM4+0x400
	mov r2,#0xC00
	b writeBG



writeBG_mapper_9_checks:
	b_long writeBG_mapper_9_checks_
	.pushsection .text, "ax", %progbits
writeBG_mapper_9_checks_:
	cmp r0,#0xFD
	blt_long writeBG_mapper_9_mod_return
	cmp addy,#0x3C0
	bge_long writeBG_mapper_9_mod_return
	stmfd sp!,{r0,r2,addy,lr}
	add addy,addy,addy
	bl_long mapper9BGcheck
	ldmfd sp!,{r0,r2,addy,lr}
	b_long writeBG_mapper_9_mod_return
	.popsection

	.else
@----------------------------------------------------------------------------
VRAM_name0:	@(2000-23ff)
@----------------------------------------------------------------------------
	ldr_ r1,nes_nt0
	ldr r2,=AGB_BG+0x0000
writeBG:		@loadcart jumps here
	bic addy,addy,#0xfc00	@AND $03ff
	strb r0,[r1,addy]
	cmp addy,#0x3c0
	bhs writeattrib
@writeNT
	add addy,addy,addy	@lsl#1
	ldrh r1,[r2,addy]	@use old color
	and r1,r1,#0xf000
	orr r1,r0,r1
	strh r1,[r2,addy]	@write tile#
		cmp r0,#0xfd	@mapper 9 shit..
		bxlt lr
		ldrb_ r1,mapper_number
		cmp r1,#9
		cmpne r1,#10
		bxne lr
		b_long mapper9BGcheck
writeattrib:
	stmfd sp!,{r3,r4,lr}

	orr r0,r0,r0,lsl#16
	and r1,addy,#0x38
	and addy,addy,#0x07
	add addy,addy,r1,lsl#2
	add addy,r2,addy,lsl#3
	ldr r3,=0x00ff00ff
	ldr r4,=0x00030003

	ldr r1,[addy]
	and r2,r0,r4
	and r1,r1,r3
	orr r1,r1,r2,lsl#12
	str r1,[addy]
		ldr r1,[addy,#0x40]
		and r1,r1,r3
		orr r1,r1,r2,lsl#12
		str r1,[addy,#0x40]
	ldr r1,[addy,#4]
	and r2,r0,r4,lsl#2
	and r1,r1,r3
	orr r1,r1,r2,lsl#10
	str r1,[addy,#4]
		ldr r1,[addy,#0x44]
		and r1,r1,r3
		orr r1,r1,r2,lsl#10
		str r1,[addy,#0x44]
	ldr r1,[addy,#0x80]
	and r2,r0,r4,lsl#4
	and r1,r1,r3
	orr r1,r1,r2,lsl#8
	str r1,[addy,#0x80]
		ldr r1,[addy,#0xc0]
		and r1,r1,r3
		orr r1,r1,r2,lsl#8
		str r1,[addy,#0xc0]
	ldr r1,[addy,#0x84]
	and r2,r0,r4,lsl#6
	and r1,r1,r3
	orr r1,r1,r2,lsl#6
	str r1,[addy,#0x84]
		ldr r1,[addy,#0xc4]
		and r1,r1,r3
		orr r1,r1,r2,lsl#6
		str r1,[addy,#0xc4]
	ldmfd sp!,{r3,r4,pc}
@----------------------------------------------------------------------------
VRAM_name1:	@(2400-27ff)
@----------------------------------------------------------------------------
	ldr_ r1,nes_nt1
	ldr r2,=AGB_BG+0x0800
@	ldr_ r2,agb_nt1
	b writeBG
@----------------------------------------------------------------------------
VRAM_name2:	@(2800-2bff)
@---------------------------------------------------------------------------
	ldr_ r1,nes_nt2
	ldr r2,=AGB_BG+0x1000
@	ldr_ r2,agb_nt2
	b writeBG
@----------------------------------------------------------------------------
VRAM_name3:	@(2c00-2fff)
@----------------------------------------------------------------------------
	ldr_ r1,nes_nt3
	ldr r2,=AGB_BG+0x1800
@	ldr_ r2,agb_nt3
	b writeBG
	.endif
@----------------------------------------------------------------------------
VRAM_pal:	@write to VRAM palette area ($3F00-$3F1F)
@----------------------------------------------------------------------------
	cmp addy,#0x3f00
	bmi VRAM_name3

	mov r1,#1
	strb_ r1,palette_dirty
	
	and r0,r0,#0x3f		@(only colors 0-63 are valid)
	adr_ r1,nes_palette
	and addy,addy,#0x1f
	strb r0,[r1,addy]	@store in nes palette
		tst addy,#0x03
		eoreq addy,addy,#0x10	@background color mirroring
		streqb r0,[r1,addy]
		biceq addy,addy,#0x10	@background color mirroring
	mov pc,lr
@----------------------------------------------------------------------------

 .section .data.105, "w", %progbits

_nesoamdirty:	.byte 0
_consumetiles_ok:	.byte 0
_frameready:	.byte 0
_firstframeready:	.byte 0

_vram_write_tbl:	@for vmdata_W, r0=data, addy=vram addr
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word VRAM_name0	@$2000
	.word VRAM_name1	@$2400
	.word VRAM_name2	@$2800
	.word VRAM_name3	@$2c00
	.word VRAM_name0	@$3000
	.word VRAM_name1	@$3400
	.word VRAM_name2	@$3800
	.word VRAM_pal	@$3c00

_vram_map:	@for vmdata_R
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
_nes_nt0: .word NES_VRAM2+0x0000 @$2000
_nes_nt1: .word NES_VRAM2+0x0000 @$2400
_nes_nt2: .word NES_VRAM2+0x0400 @$2800
_nes_nt3: .word NES_VRAM2+0x0400 @$2c00
	.word NES_VRAM2+0x0000 @$3xxx=?
	.word NES_VRAM2+0x0000
	.word NES_VRAM2+0x0400
	.word NES_VRAM2+0x0400

@_agb_nt_map	@set thru mirror*
@_agb_nt0: .word 0
@_agb_nt1: .word 0
@_agb_nt2: .word 0
@_agb_nt3: .word 0

_nes_palette:	.skip 32	@NES $3F00-$3F1F

_scrollbuff: .word SCROLLBUFF1
_dmascrollbuff: .word SCROLLBUFF2
_nesoambuff:	.word NESOAMBUFF2
_dmanesoambuff:	.word NESOAMBUFF2
_bg0cntbuff:	.word BG0CNTBUFF1
_dmabg0cntbuff:	.word BG0CNTBUFF2
_dispcntbuff:	.word DISPCNTBUFF1
_dmadispcntbuff:	.word DISPCNTBUFF2

_dma0buff:	.word DMA0BUFF
_dma1buff:	.word DMA1BUFF
_dma3buff:	.word DMA3BUFF
_nes_vram:	.word NES_VRAM

_bankbuffer_last: .word 0,0
_bankbuffer:	.word BANKBUFFER1
_dmabankbuffer:	.word BANKBUFFER2
_bankbuffer_line:	.byte 0
_bankbuffer_line_previous:	.byte 0
_bankbuffer_line_previous2:	.byte 0
_bankbuffer_line_previous3:	.byte 0

_ctrl1old:	.word 0x0440	@last write

_ctrl1line:	.byte 0 @when?
_ctrl1line_previous: .byte 0
_ctrl1line_previous2: .byte 0
_ctrl1line_previous3: .byte 0

_stat_r_simple_func:	.word 0
_nextnesoambuff:	.word NESOAMBUFF1

_screen_off: .byte 0
_inside_vblank: .byte 0
_timestamp_mult_2: .byte 0  @is this the oddball?
_suppress_vbl: .byte 0

_inside_gba_vblank: .byte 0
_okay_to_run_hdma: .byte 0xFF
_palette_number: .byte 0 @palette_number
_palette_dirty: .byte 0  @palette_dirty

_displayed_palette: .word PALETTE_BUFFER_1
_current_palette: .word PALETTE_BUFFER_2
_displayed_palette_scanlines: .word 0
_displayed_palette_pointer: .word 0

@----------------------------------------------------------------------------

 .section .data.099, "w", %progbits
 .align 4
_vramaddr: .word 0 @vramaddr  (moved here so halfword instructions can reach it)

 .section .data.101, "w", %progbits

FPSValue:
	.word 0
AGBinput:		@this label here for main.c to use
	.word 0 @AGBjoypad (why is this in ppu.s again?  um.. i forget)
EMUinput:	.word 0 @NESjoypad (this is what NES sees)
@wtop:	.word 0,0,0,0 @windowtop  (this label too)   L/R scrolling in unscaled mode


@begin ppustate
ppustate:
_vramaddr_dummy:	.word 0 @vramaddr (loopy_v)  (not used anymore)
_loopy_t:	.word 0 @loopy_t
_scrollX:	.word 0 @scrollX
_scrollY:	.word 0 @scrollY
_scrollY_v:	.word 0 @scrollY_v
_readtemp:	.byte 0 @readtemp
_oam_addr:	.byte 0 @oam_addr
	.byte 0
	.byte 0
	
	.byte 0
	.byte 1 @vramaddrinc (placeholder for savestates, self modify code is used instead)
ppustat_savestate:	.byte 0 @was once ppustat (not used)
	.byte 0 @toggle
_ppuctrl0:	.byte 0 @ppuctrl0
_ppuctrl0frame:
	.byte 0 @ppuctrl0frame	;state of $2000 at frame start
_ppuctrl1:
	.byte 0 @ppuctrl1
	.byte 0 @ppuoamadr
_nextx:	.word 0 @scrollX_v
@not in ppustate

	.word 0 @scrollXold
	.byte 0 @scrollXline
	.byte 0 @scrollXline_previous
	.byte 0 @scrollXline_previous2
	.byte 0
	
	.word 0 @scrollYold
	.byte 0 @scrollYline
	.byte 0 @scrollYline_previous
	.byte 0 @scrollYline_previous2
	.byte 0
_scroll_y_timestamp: .word 0

	.byte 0 @y_in_sprite0
_PAL60:	.byte 0 @PAL60
novblankwait:	.byte 0 @novblankwait_
wtop:	.byte 0 @windowtop
	.byte 0 @adjustblend
	.byte 0 @okay_to_run_nes_chr_update_this_frame
_has_vram:	.byte 0 @has_vram
_bankable_vrom:	.byte 0 @bankable_vrom

_vram_page_mask: .byte 0 @vram_page_mask
_vram_page_base: .byte 0 @vram_page_base
wtop_scaled6_8: .byte 16 @windowtop_scaled6_8
wtop_scaled7_8: .byte 0 @windowtop_scaled7_8

	.if DIRTYTILES
_recent_tiles:	.word RECENT_TILES1
_dmarecent_tiles:	.word RECENT_TILES2
_recent_tilenum:	.word RECENT_TILENUM1
_dmarecent_tilenum:	.word RECENT_TILENUM2
	.endif
	
	.if USE_BG_CACHE
	.word 0 @bg_cache_produce_cursor
	.word 0 @bg_cache_produce_base_consume_end
	.word 0 @bg_cache_produce_limit_consume_begin
	
_bg_cache_full:	.byte 0 @bg_cache_full
	.byte 0 @bg_cache_updateok
	
	.byte 0,0  @padding
	.endif


	
	
@...update load/savestate if you move things around in here
@----------------------------------------------------------------------------
	@.end

.if DIRTYTILES
.unreq decodeptr
.unreq nesptr
.unreq r_tiles
.unreq r_tnum
.unreq d_tiles
.unreq d_rows
.unreq tilenum
.unreq tilesleft
.unreq agbptr
.unreq agbptr2
.endif
