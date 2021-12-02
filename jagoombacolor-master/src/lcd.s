@TODO:
@Workaround for screen garbage on UI

@check all reset code, and make sure it's resetting everything!!!

@option to use color 0 as border color
@Fix the damn UI shaking, fix the darkened UI when changing between SGB and non SGB
@Still a couple bugs involving applying the ui/border

@newer todo:
@fix flickering???

@SGB STUFF:
@do SGB colors correctly
@do sgb attributes

@other border stuff:
@custom borders



@TODO:
@* = written, + = tested
@fake bios, init sound like the real bios
@fix STOP instruction
@Save options per game, like palette, double speed mode
@Custom palettes
@Palette changes per line using vcount, may up to 4 palettes per frame total
@Savestates
@Real HDMA
@re-add GBAMP support
@Possibly separate gbz80 core from gb system stuff?
@Port to NDS
@ROM Compression

@dma: scrolling, dispcnt, bgXcnt, windowx
@border GFX takes up 1E00 bytes [240 tiles] per border

@vram usage:
@1a
@2a
@solid tiles, border tiles
@4 maps
@1b
@2b
@font, (2 maps) bg0,bg00
@(4 maps) bg0H,bg1,bg10,bg1H
@
@26, 27, 28, 29, 30, 31


@
@
@maps:
@6 GB maps
@2 UI maps
@1 border map
@1 free map

	VRAM_BASE	= 0x06000000
	OBJ_BASE	= 0x14000
	VRAM_OBJ_BASE	= VRAM_BASE+OBJ_BASE
	SCREEN_X_START	= 40
	SCREEN_Y_START	= 8
	OAM_BASE	= 0x07000000
	PALETTE_BASE	= AGB_PALETTE
	
	TILEMAP1	= 10 @formerly 26
	TILEMAP2	= 13 @formerly 29
	UI_TILEMAP	= 9 @formerly 30
@	UI_TILEMAP	= 30 @formerly 15
	BORDER_TILEMAP = 29 @formerly 13
	COLORZERO_CHARBASE = 2 @formerly 0
	BORDER_CHARBASE = 3 @formerly 1
	UI_CHARBASE = 1 @formerly 3
	UI_ROW = 11 @formerly 0
@	DEBUGSCREEN = VRAM_BASE+(UI_TILEMAP+1)*2048
	DEBUGSCREEN = VRAM_BASE+(UI_TILEMAP)*2048 + UI_ROW*64

 .if RUMBLE
	@IMPORT RumbleInterrupt
	@IMPORT StartRumbleComs
 .endif
 	global_func update_ui_border_masks
 	
 	global_func lcdstat
 	global_func FF41_modify1
 	global_func FF41_R_vblank
 	
 	.global g_lcdhack
 	global_func update_lcdhack
 	.global _dmamode
 	
	.global ui_border_visible
	.global ui_y_real
	.global ui_x
	.global darkness
	.global sgb_palette_number
	
	global_func move_ui_asm
	
	global_func newframe_vblank
	@global_func gbc_chr_update
	
	.global g_scanline
	.global _gb_oam_buffer_screen
	.global _gb_oam_buffer_writing
	.global _gb_oam_buffer_alt
	
	global_func GFX_init
	global_func GFX_init_irq
	global_func GFX_reset
	global_func FF40_R
	global_func FF40_W
	global_func FF41_R
	global_func FF41_W
	global_func FF42_R
	global_func FF42_W
	global_func FF43_R
	global_func FF43_W
	global_func FF44_R
	global_func FF44_W
	global_func FF45_R
	global_func FF45_W
	global_func FF46_W
	global_func FF47_R
	global_func FF47_W
	global_func FF48_R
	global_func FF48_W
	global_func FF49_R
	global_func FF49_W
	global_func FF4A_R
	global_func FF4A_W
	global_func FF4B_R
	global_func FF4B_W
	global_func FF4F_W
	global_func FF4F_R

	global_func FF68_R	@BCPS - BG Color Palette Specification
	global_func FF69_R	@BCPD - BG Color Palette Data
	global_func FF6A_R	@OCPS - OBJ Color Palette Specification
	global_func FF6B_R	@OCPD - OBJ Color Palette Data

	global_func FF68_W	@BCPS - BG Color Palette Specification
	global_func FF69_W	@BCPD - BG Color Palette Data
	global_func FF6A_W	@OCPS - OBJ Color Palette Specification
	global_func FF6B_W	@OCPD - OBJ Color Palette Data

	global_func OAM_W
	global_func OAM_R

	global_func vram_W_8
	global_func vram_W_9
	global_func vram_W2
	global_func debug_
	.global AGBinput
	.global EMUinput
	global_func paletteinit
	global_func PaletteTxAll
	global_func transfer_palette
	global_func update_sgb_palette
	global_func newframe
	.global lcdstate
	.global gammavalue
	.global palettebank
	global_func resetlcdregs
	.global fpsenabled
	.global FPSValue
	global_func vbldummy
	.global vblankfptr
	.global vcountfptr
	global_func vblankinterrupt
	.global gbc_palette
	global_func vcountinterrupt
	.global WINDOWBUFF
	
	.global _dma_src
	.global _dma_dest
	.global _dirty_tile_bits
@	.global _dirty_tiles
@	.global _dirty_rows
	
	.global _vrambank
	
@	global_func _set_bg_cache_full

	.global _vram_packet_dest
	.global _vram_packet_source
@	.global vram_packets
	.global dirty_map_words
	
	.global vram_packets_incoming
	.global vram_packets_registered_bank0
	.global vram_packets_registered_bank1
	.global vram_packets_dirty
	
	.global TEXTMEM
	.global DIRTY_TILE_BITS
	.global RECENT_TILENUM


@	EXPORT DISPCNTBUFF
@	EXPORT BG0CNTBUFF
@	EXPORT SCROLLBUFF1
@	EXPORT SCROLLBUFF2
@	EXPORT DMA0BUFF


 .align
 .pool
 .text
 .align
 .pool

@----------------------------------------------------------------------------

move_ui_asm:
	stmfd sp!,{globalptr}
	ldr globalptr,=GLOBAL_PTR_BASE
	ldr_ r1,_ui_y
	ldr_ r0,_ui_x
	mov r0,r0,lsl#8
	bic r0,r0,#0xFF000000
	orr r0,r0,r1,lsl#24
	ldrb_ r1,_ui_border_visible
	orr r0,r0,r1
	str_ r0,_ui_border_request
	ldmfd sp!,{globalptr}
	bx lr

GFX_init_irq:
	@install IRQ handler
	ldr r1,=AGB_IRQVECT
	ldr r2,=irqhandler
	str r2,[r1]

	mov r1,#REG_BASE
	mov r0,#0x0008
@	mov r0,#0x0028
	strh r0,[r1,#REG_DISPSTAT]	@vblank en

	add r2,r1,#REG_IE
	mov r0,#-1
	strh r0,[r2,#2]		@stop pending interrupts
@	ldr r0,=0x1081
@	strh r0,[r2]		;key,vblank,serial interrupt enable
	ldr r0,=0x1085
	strh r0,[r2]		@key,vcount,vblank,serial interrupt enable
	mov r0,#1
	strh r0,[r2,#8]		@master irq enable

	bx lr

@----------------------------------------------------------------------------
GFX_init:	@(called from main.c) only need to call once
@----------------------------------------------------------------------------
	mov addy,lr
	bl build_chr_decode

	mov r1,#0
	ldr r0,=PALETTE_BASE			@clear some of the AGB Palette
	mov r2,#0x80
	bl memset32_

	ldr r1,=0x5F405F40
	ldr r0,=DISPCNTBUFF			@clear DISPCNT+DMA1BUFF
	mov r2,#288
	bl memset32_
	ldr r0,=DISPCNTBUFF2			@clear DISPCNT+DMA1BUFF
	mov r2,#288
	bl memset32_

@	mov r1,#REG_BASE
@	mov r0,#8
@	strh r0,[r1,#REG_BLDY]		;darkness setting for faded screens (bigger number=darker)
@	ldr r0,=0x353A
@	strh r0,[r1,#REG_WININ]		;WinIN0/1, BG0 not enable in Win0
@	ldr r0,=0x3F20
@	strh r0,[r1,#REG_WINOUT]	;WinOUT0/1, Everything enabled outside Windows


@	add r0,r1,#REG_BG0HOFS		;DMA0 always goes here
@	str r0,[r1,#REG_DM0DAD]
@	mov r0,#1					;1 word transfer
@	strh r0,[r1,#REG_DM0CNT_L]
@@	ldr r0,=DMA0BUFF			;DMA0 src=
@@	str r0,[r1,#REG_DM0SAD]
@
@	str r1,[r1,#REG_DM1DAD]		;DMA1 goes here
@	mov r0,#1					;1 word transfer
@	strh r0,[r1,#REG_DM1CNT_L]

	bx addy
build_chr_decode:
	mov r1,#0xffffff00			@build chr decode tbl
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
	
	@set canary at end of chr_decode table
	ldr r0,=0xDEADBEEF
	str r0,[r2],#4
	
	bx lr


@----------------------------------------------------------------------------
GFX_reset:	@called with CPU reset
@----------------------------------------------------------------------------
	stmfd sp!,{r3,addy,lr}

	ldrb_ r0,gbcmode
	movs r0,r0
	bne 1f
	
	@get GBC palette
	ldr_ r0,memmap_tbl
	blx_long GetGbcPaletteNumber
	@if zero, pick Wario Blast palette
	cmp r0,#0
	moveq r0,#74
	ldr r1,=palettebank
	strb r0,[r1]
	bl paletteinit
1:
	
	mov r1,#0

@	ldr r0,=AGB_BG		;clear most of the GB VRAM
@	mov r2,#0x2000
@	bl memset32_

@@--------
	@clear dirty tile bits
	ldr r0,=DIRTY_TILE_BITS
	mov r2,#48
	bl memset32_

	@clear dirty map words
	ldr r0,=dirty_map_words
	mov r2,#64
	bl memset32_
	
	@clear all VRAM packets
	ldr r0,=vram_packets_incoming
	mov r2,#128
	bl memset32_
	ldr r0,=vram_packets_registered_bank0
	mov r2,#128
	bl memset32_
	ldr r0,=vram_packets_registered_bank1
	mov r2,#128
	bl memset32_
	ldr r0,=vram_packets_dirty
	mov r2,#132
	bl memset32_
	

	
	
@	ldr r0,=DIRTY_TILES
@	mov r2,#(768+48+4)
@	bl memset32_
	
@	@clear RECENT_TILENUM
@	mvn r1,#0
@	ldr r0,=RECENT_TILENUM
@	mov r2,#(MAX_RECENT_TILES*2)+4
@	bl memset32_
	
@	@clear RECENT_TILES for no reason
@	mov r1,#0
@	ldr r0,=RECENT_TILES
@	mov r2,#MAX_RECENT_TILES*16
@	bl memset32_
	
	.if RESIZABLE
	ldr_ r0,xgb_vram
	ldr_ r2,xgb_vramsize
	bl memset32_
	.else
	@clear XGB_VRAM
	ldr r0,=XGB_VRAM
	mov r2,#0x4000   @maybe this needs to change for 'resizable'
	bl memset32_
	.endif
	
	@clear GBA vram corresponding to XGB_VRAM
	ldr r0,=VRAM_BASE
	mov r2,#0x4000
	bl memset32_
	ldr r0,=VRAM_BASE+0x8000
	mov r2,#0x4000
	bl memset32_
	ldr r0,=VRAM_OBJ_BASE
	mov r2,#0x4000
	bl memset32_
	@clear GBA tilemaps corresponding to XGB_VRAM
	ldr r0,=VRAM_BASE+TILEMAP1*2048
	mov r2,#0x3000
	bl memset32_
	
	@clear GB OAM
	mov r1,#200
	ldr r0,=GBOAMBUFF1
	mov r2,#160
	bl memset32_
	ldr r0,=GBOAMBUFF2
	mov r2,#160
	bl memset32_
	ldr r0,=GBOAMBUFF3
	mov r2,#160
	bl memset32_

@@--------


	mov r0,#0
	bl_long FF4F_W
	
	@reset emu buffers FIXME
@	mov r0,#0
@	strb r0,frame_windowy
@	strb r0,active_windowy
@	strb r0,gboamdirty
	

	
	mov r0,#0
	ldr r1,=lcdstat
	strb r0,[r1]@lcdstat		;flags off
	strb r0,[r1,#-12] @fixme
	strb_ r0,scrollX
	strb_ r0,scrollY
	strb_ r0,windowX
	strb_ r0,windowY
	strb_ r0,lcdyc
	strb_ r0,vrambank
	strb_ r0,BCPS_index
	strb_ r0,OCPS_index
	strb_ r0,doublespeed
	str_ r0,dma_src @and dma_dest

	mov r0,#0x91
	strb_ r0,lcdctrl		@LCDC
	orr cycles,cycles,#CYC_LCD_ENABLED

	mov r1,#REG_BASE
	ldr r0,=(SCREEN_X_START*256 + (SCREEN_X_START+160))
	strh r0,[r1,#REG_WIN1H]		@Win0H
	ldr r0,=(SCREEN_Y_START*256 + (SCREEN_Y_START+144))
	strh r0,[r1,#REG_WIN1V]		@Win0V

@	mov r0,#0x0000
@	strh r0,[r1,#REG_BG3CNT]	;Border

	bl paletteinit
	mov r0,#0xfc
	strb_ r0,bgpalette
	mov r0,#0xff
	strb_ r0,ob0palette
	strb_ r0,ob1palette
	bl resetlcdregs
	@init the buffer filling engine
	mov r0,#0xFF
	strb_ r0,rendermode
	bl_long newframeinit

	bl_long update_ui_border_masks
	
	bl selfmodify_reset
	
	
	ldmfd sp!,{r3,addy,lr}


	bx lr

selfmodify_reset:
@	ldrb_ r1,gbcmode
@	adr r2,0f
@	add r2,r2,r1,lsl#3
@	ldr r0,[r2],#4
@	ldr r1,=bgcache_modify1
@	str r0,[r1]
@	ldr r0,[r2],#4
@	ldr r1,=bgcache_modify2
@	str r0,[r1]
	bx lr



0:
	mov r1,#0
	mov r4,#0
	ldrh r1,[r2]
	ldr r4,[addy,r7,lsr#3]


@@----------------------------------------------------------------------------
@move_ui
@@----------------------------------------------------------------------------
@	stmfd sp!,{r0-addy,lr}
@	ldr r0,=ui_x
@	ldr r0,[r0]
@	ldr r3,=ui_y_real
@	ldr r3,[r3]
@	orr r0,r0,r3,lsl#16
@	ldr r3,=BG0CNTBUFF+6
@	ldr r4,=DISPCNTBUFF
@	mov r1,#160
@	ldr r2,=dmascrollbuff
@	ldr r2,[r2]
@	add r2,r2,#12
@@	ldr r7,=DMA0BUFF
@@	add r7,r7,#12
@	ldr r8,=scrollbuff
@	ldr r8,[r8]
@	add r8,r8,#12
@@	ldr r7,=
@@	ldr r2,=DMA0BUFF
@@	ldr r6,=dmascrollbuff
@@	str r2,[r6]
@@	ldr r2,=DMA0BUFF+12
@	ldr r6,=0x5A0C
@move_ui_loop
@	str r0,[r2],#16
@@	str r0,[r7],#16
@	str r0,[r8],#16
@	ldrh r5,[r4]
@	orr r5,r5,#0x0800
@	strh r5,[r4],#2
@	strh r6,[r3],#8
@	subs r1,r1,#1
@	bne move_ui_loop
@
@	ldmfd sp!,{r0-addy,lr}
@	bx lr

@----------------------------------------------------------------------------
resetlcdregs:
@----------------------------------------------------------------------------
	str lr,[sp,#-4]!

@	ldrb r0,lcdctrl
@	bl_long FF40_W
	ldr r1,=lcdstat
	ldrb r0,[r1]@lcdstat
	bl_long FF41_W
@	ldrb r0,scrollY
@	bl_long FF42_W
@	ldrb r0,scrollX
@	bl_long FF43_W
	ldrb_ r0,lcdyc
	bl_long FF45_W
@	ldrb r0,windowY
@	bl_long FF4A_W
@	ldrb r0,windowX
@	bl_long FF4B_W
	bl PaletteTxAll
	
	ldr pc,[sp],#4
@----------------------------------------------------------------------------
PaletteTxAll:@		also called from UI.c
@----------------------------------------------------------------------------
	str lr,[sp,#-4]!
	ldrb_ r0,bgpalette
	bl_long FF47_W_
	ldrb_ r0,ob0palette
	bl_long FF48_W_
	ldrb_ r0,ob1palette
	bl_long FF49_W_
	bl copy_gbc_palette
	ldr lr,[sp],#4
	bx lr

copy_gbc_palette:
	ldr r1,=gbc_palette
	ldr r0,=gbc_palette2
	mov r2,#128
	b_long memcpy32

@	@copy GBC palette
@	ldr r1,=gbc_palette
@	ldr r2,=gbc_palette2
@	mov addy,#128
@palcopyloop:
@	ldr r0,[r1],#4
@	str r0,[r2],#4
@	subs addy,addy,#4
@	bne palcopyloop
@	bx lr

@----------------------------------------------------------------------------
paletteinit:@	r0-r3 modified.
@called by ui.c:  void map_palette(char gammavalue)
@----------------------------------------------------------------------------
	stmfd sp!,{r4-r7,globalptr,lr}
	ldr globalptr,=GLOBAL_PTR_BASE
	ldr r7,=GBPalettes
	ldr_ r1,_palettebank	@Which color set (yellow, grey...)
	add r1,r1,r1,lsl#1	@r1 x 3
	add r7,r7,r1,lsl#4	@r7 + r1 x 16
	ldr r6,=SGB_PALETTE
@	ldrb r1,gammavalue	;gamma value = 0 -> 4
	mov r4,#16
nomap:					@map rrrrrrrrggggggggbbbbbbbb  ->  0bbbbbgggggrrrrr
	ldrb r0,[r7],#1		@Red ready
@	bl gammaconvert
	mov r0,r0,lsr#3
	mov r5,r0

	ldrb r0,[r7],#1		@Green ready
@	bl gammaconvert
	mov r0,r0,lsr#3
	orr r5,r5,r0,lsl#5

	ldrb r0,[r7],#1		@Blue ready
@	bl gammaconvert
	mov r0,r0,lsr#3
	orr r5,r5,r0,lsl#10

	strh r5,[r6],#2
	subs r4,r4,#1
	bpl nomap

	ldmfd sp!,{r4-r7,globalptr,lr}
	bx lr

transfer_palette:
	b_long transfer_palette_

@----------------------------------------------------------------------------
showfps_:		@fps output, r0-r3=used.
@----------------------------------------------------------------------------
	ldr r2,=fpsenabled
	ldrb r0,[r2,#1] @fpschk
	subs r0,r0,#1
	movmi r0,#59
	strb r0,[r2,#1]
	bxpl lr					@End if not 60 frames has passed

 .if RUMBLE
	str lr,[sp,#-4]!
	ldr r1,=StartRumbleComs
	adr lr,ret_
	bx r1
ret_:
	ldr lr,[sp],#4
	ldr r2,=fpsenabled
 .endif


	ldrb r0,[r2,#0]
	tst r0,#1
	bxeq lr					@End if not enabled

	@must be drawn with UI not at left position
	ldr r0,=ui_x
	ldr r0,[r0]
	cmp r0,#0
	bxeq lr
	
	ldr_ r0,fpsvalue
	cmp r0,#0
	bxeq lr					@End if fps==0, to keep it from appearing in the menu
	mov r1,#0
	str_ r1,fpsvalue

	mov r1,#100
	swi 0x060000			@Division r0/r1, r0=result, r1=remainder.
	add r0,r0,#0x30
	strb r0,[r2,#-3]@fpstext+5
	mov r0,r1
	mov r1,#10
	swi 0x060000			@Division r0/r1, r0=result, r1=remainder.
	add r0,r0,#0x30
	strb r0,[r2,#-2]@fpstext+6
	add r1,r1,#0x30
	strb r1,[r2,#-1]@fpstext+7
	

	ldr r0,=fpstext
	ldr r2,=DEBUGSCREEN
@	add r2,r2,r1,lsl#6
db1:
	ldrb r1,[r0],#1
	subs r1,r1,#32 + 10 * REDUCED_FONT
	movlt r1,#0
	orr r1,r1,#0x5000
	
	strh r1,[r2],#2
	tst r2,#15
	bne db1

	bx lr
@----------------------------------------------------------------------------
debug_:		@debug output, r0=val, r1=line, r2=used.
@----------------------------------------------------------------------------
 .if DEBUG
	ldr r2,=DEBUGSCREEN
	add r2,r2,r1,lsl#6
db0:
	mov r0,r0,ror#28
	and r1,r0,#0x0f
	cmp r1,#9
	addhi r1,r1,#7
	add r1,r1,#0x30
	sub r1,r1,#32 + 10 * REDUCED_FONT
	orr r1,r1,#0x5000
	strh r1,[r2],#2
	tst r2,#15
	bne db0
 .endif
	bx lr

update_sgb_palette:
	stmfd sp!,{lr}
	bl update_sgb_bg_palette
	bl update_sgb_obj0_palette
	bl update_sgb_obj1_palette
	ldmfd sp!,{pc}

update_sgb_bg_palette:
	stmfd sp!,{lr}
	ldrb_ r0,bgpalette
	ldr r2,=gbc_palette
	ldr addy,=SGB_PALETTE
	bl_long dopalette
	ldr r2,=gbc_palette+8
	ldr addy,=SGB_PALETTE+8
	bl_long dopalette
	ldr r2,=gbc_palette+16
	ldr addy,=SGB_PALETTE+16
	bl_long dopalette
	ldr r2,=gbc_palette+24
	ldr addy,=SGB_PALETTE+24
	bl_long dopalette
	ldmfd sp!,{pc}

update_sgb_obj0_palette:
	stmfd sp!,{lr}
	ldrb_ r0,ob0palette
	ldr r2,=gbc_palette+64
	ldr addy,=SGB_PALETTE
	bl_long dopalette
	ldr r2,=gbc_palette+80
	ldr addy,=SGB_PALETTE+8
	bl_long dopalette
	ldr r2,=gbc_palette+96
	ldr addy,=SGB_PALETTE+16
	bl_long dopalette
	ldr r2,=gbc_palette+112
	ldr addy,=SGB_PALETTE+24
	bl_long dopalette
	ldmfd sp!,{pc}
update_sgb_obj1_palette:
	stmfd sp!,{lr}
	ldrb_ r0,ob1palette
	ldr r2,=gbc_palette+64+8
	ldr addy,=SGB_PALETTE
	bl_long dopalette
	ldr r2,=gbc_palette+80+8
	ldr addy,=SGB_PALETTE+8
	bl_long dopalette
	ldr r2,=gbc_palette+96+8
	ldr addy,=SGB_PALETTE+16
	bl_long dopalette
	ldr r2,=gbc_palette+112+8
	ldr addy,=SGB_PALETTE+24
	bl_long dopalette
	ldmfd sp!,{pc}


 .section .iwram.2, "ax", %progbits
	tileNumber		.req r12
	bits			.req r11
	bits_ptr		.req r0
	mask			.req r8
	decodePtr		.req r9
	sourceAddress	.req r1
	byteCount		.req r2
	destAddress		.req r3
	destAddress2	.req r0
	destAddress3	.req r10
@	zero			.req r1
	tileNumberMasked .req r0
	gbaWord			.req r4
	pixels0			.req r5
	dest0			.req r6
	pixels1			.req r7

consume_dirty_tiles:
	@todo: wait for okay and no SGB MASK
	ldrb_ r0,sgb_mask
	eor r0,r0,#1
	ldrb_ r1,consume_dirty
	ands r0,r0,r1
	bxeq lr
	mov r0,#0
	strb_ r0,consume_dirty


render_dirty_tiles:
	@destroys all registers
	stmfd sp!,{r10,lr}

	ldr decodePtr,=CHR_DECODE
	@mov mask,#0xFF
	bl GetNextTileAndLength_dirty_first
	beq render_dirty_tiles_done2
0:
	bl render_tiles_vram
	bl GetNextTileAndLength_dirty
	bne 0b
render_dirty_tiles_done:
	bl ClearDirtyTiles
render_dirty_tiles_done2:
	ldmfd sp!,{r10,pc}


	recentTileNumber .req r4
	recentTilenumPointer	.req r5
	recentTilenumLimit	.req r6
	recentTilesBase .req r7

store_recent_tiles:
	stmfd sp!,{r3-r11,lr}
	ldr recentTilenumPointer,=RECENT_TILENUM
	add recentTilenumLimit,recentTilenumPointer,#RECENT_TILENUM_SIZE - 4
	ldr recentTilesBase,=RECENT_TILES
	mov recentTileNumber,#0
	bl GetNextTileAndLength_dirty_first
	beq store_recent_tiles_done
0:
	add r3,recentTileNumber,byteCount,lsr#4
	cmp r3,#MAX_RECENT_TILES
	bgt recent_tiles_full
	cmp recentTilenumPointer, recentTilenumLimit
	bge recent_tiles_full_
	strh byteCount,[recentTilenumPointer],#2
	strh tileNumber,[recentTilenumPointer],#2
	subs r0,tileNumber,#384
	movmi r0,tileNumber
	ldr r1,=XGB_VRAM
	add r1,r0,lsl#4
	addpl r1,r1,#0x2000
	add r0,recentTilesBase,recentTileNumber,lsl#4
	mov recentTileNumber,r3
	add r3,tileNumber,byteCount,lsr#4
	bl memcpy32
	mov tileNumber,r3
	mov byteCount,#0
	bl GetNextTileAndLength_dirty
	bne 0b

store_recent_tiles_done:
	mov r1,#0
	str r1,[recentTilenumPointer]
	bl ClearDirtyTiles
store_recent_tiles_abort:
	ldmfd sp!,{r3-r11,pc}

recent_tiles_full_:
	mov r11,r11
recent_tiles_full:
	ldmfd sp!,{r3-r11,lr}
	ldr r0,=RECENT_TILENUM
	mov r1,#0
	str r1,[r0]
	@want to consume dirty because storing to the buffer failed
	mov r0,#1
	strb_ r0,consume_dirty
	bx lr
	b store_recent_tiles_abort


	.unreq recentTileNumber
	.unreq recentTilenumPointer
	.unreq recentTilenumLimit
	.unreq recentTilesBase

consume_recent_tiles:
	ldrb_ r0,consume_buffer
	movs r0,r0
	bxeq lr
render_recent_tiles_:
	mov r0,#0
	strb_ r0,consume_buffer

render_recent_tiles:
	stmfd sp!,{r10,lr}
	mov mask,#0xFF
	ldr bits,=RECENT_TILENUM
	ldr sourceAddress,=RECENT_TILES
	ldr decodePtr,=CHR_DECODE
0:
	ldrh byteCount,[bits],#2
	ldrh tileNumber,[bits],#2
	movs byteCount,byteCount
	beq render_recent_tiles_done
	
	adr lr,0b
	subs tileNumberMasked,tileNumber,#384
	movmi tileNumberMasked,tileNumber
	b render_tiles_entry
render_recent_tiles_done:
	@clear recent tilenum to mark it as clean
	mov r0,#0
	ldr r1,=RECENT_TILENUM
	str r0,[r1]
	ldmfd sp!,{r10,pc}
	
ClearDirtyTiles:
	ldr r0,=DIRTY_TILE_BITS
	mov r1,#0
	mov r2,#48
	b memset32
	
	
@GetNextTileAndLength_dirty:
@	Inputs: bits, tileNumber
@	Returns the next tile number as tileNumber, and the next length as byteCount
@	Zero flag set (byteCount == 0) if done

GetNextTileAndLength_dirty_first:
	mov mask,#0xFF
	ldr r1,=VRAM_chr_lastAddr
	strb mask,[r1]
	mov byteCount,#0
	mov tileNumber,#0
update_dirty_tiles_loop:
	@total tile numbers: 384 per bank (768 total)
	@word number for tile number / 2:
	@(tile_number/2) / 32
	ldr bits_ptr,=DIRTY_TILE_BITS
	mov bits,tileNumber,lsr#6
	add bits_ptr,bits,lsl#2
1:
	ldr bits,[bits_ptr],#4
	movs bits,bits
	bne row_contains_dirty_tiles
	add tileNumber,tileNumber,#2*32
	cmp tileNumber,#768
	blt 1b
update_dirty_tiles_done:
	movs byteCount,byteCount
	bx lr
	
row_contains_dirty_tiles:
	@mov zero,#0
	@strne zero,[bits_ptr,#-4]
GetNextTileAndLength_dirty:
row_contains_dirty_tiles_loop:
	movs bits,bits,lsr#1
	bcs found_dirty_tile
	beq dirty_tiles_row_finished
	add tileNumber,tileNumber,#2
	b row_contains_dirty_tiles_loop
found_dirty_tile:
0:
	addcs byteCount,byteCount,#32
	@count consecutive dirty tiles
	movs bits,bits,lsr#1
	bcs 0b
	movs bits,bits,lsl#1
	bxne lr
	@check if we're moving from 64->65, 192->193, etc...
	add r1,tileNumber,byteCount,lsr#4
	tst r1,#0x3F
	bne update_dirty_tiles_done
	tst r1,#0x40
	beq update_dirty_tiles_done
	@peek ahead to see if next bit is set
	ldr bits_ptr,=DIRTY_TILE_BITS
	ldrb r1,[bits_ptr,r1,lsr#4]!
	tst r1,#1
	beq update_dirty_tiles_done
	b 1b
	
dirty_tiles_row_finished:
	ands tileNumberMasked,tileNumber,#0x3F
	rsbne tileNumberMasked,tileNumberMasked,#0x40
	addne tileNumber,tileNumber,tileNumberMasked
	cmp tileNumber,#768
	blt update_dirty_tiles_loop
	bx lr

render_tiles_vram:
	@calculate source address
	subs tileNumberMasked,tileNumber,#384
	@pl if bank 1, mi if bank 0
	movmi tileNumberMasked,tileNumber
	add tileNumber,tileNumber,byteCount,lsr#4
	
	ldr sourceAddress,=XGB_VRAM
	add sourceAddress,tileNumberMasked,lsl#4
	addpl sourceAddress,sourceAddress,#0x2000
render_tiles_entry:
	@r0 = tileNumberMasked, r8 = 0xFF, r9 = CHR_DECODE
	
	@select destination address:
	@8000-87FF: (tiles 00-7F) sprite tiles (06000000, 06014000)
	@8800-8FFF: (tiles 80-FF) shared tiles (06001000, 06009000, 06015000)
	@9000-97FF: (tiles 100-17F) bg tiles   (06008000)
	mov destAddress,#VRAM_BASE
	addpl destAddress,destAddress,#0x2000
	add destAddress,destAddress,tileNumberMasked,lsl#5
	cmp tileNumberMasked,#0x80
	blt render_tiles_2
	cmp tileNumberMasked,#0x100
	blt render_tiles_3
	@change 06002000 -> 06008000
	add destAddress,destAddress,#0x6000
render_tiles_1:
@	ldr decodePtr,=CHR_DECODE
@	mov mask,#0xFF
render_tiles_1_loop:
	ldr gbaWord,[sourceAddress],#4
	ands pixels0,mask,gbaWord
	ldrne pixels0,[decodePtr,pixels0,lsl#2]
	ands pixels1,mask,gbaWord,lsr#8
	ldrne pixels1,[decodePtr,pixels1,lsl#2]
	orr dest0,pixels0,pixels1,lsl#1
	ands pixels0,mask,gbaWord,lsr#16
	ldrne pixels0,[decodePtr,pixels0,lsl#2]
	ands pixels1,mask,gbaWord,lsr#24
	ldrne pixels1,[decodePtr,pixels1,lsl#2]
	orr pixels1,pixels0,pixels1,lsl#1
	stmia destAddress!,{dest0,pixels1}
	subs byteCount,byteCount,#4
	bgt render_tiles_1_loop
	bx lr
render_tiles_2:
	@change 06000000 -> 060
	add destAddress2,destAddress,#OBJ_BASE
@	ldr decodePtr,=CHR_DECODE
@	mov mask,#0xFF
render_tiles_2_loop:
	ldr gbaWord,[sourceAddress],#4
	ands pixels0,mask,gbaWord
	ldrne pixels0,[decodePtr,pixels0,lsl#2]
	ands pixels1,mask,gbaWord,lsr#8
	ldrne pixels1,[decodePtr,pixels1,lsl#2]
	orr dest0,pixels0,pixels1,lsl#1
	ands pixels0,mask,gbaWord,lsr#16
	ldrne pixels0,[decodePtr,pixels0,lsl#2]
	ands pixels1,mask,gbaWord,lsr#24
	ldrne pixels1,[decodePtr,pixels1,lsl#2]
	orr pixels1,pixels0,pixels1,lsl#1
	stmia destAddress!,{dest0,pixels1}
	stmia destAddress2!,{dest0,pixels1}
	subs byteCount,byteCount,#4
	bgt render_tiles_2_loop
	bx lr
render_tiles_3:
	@change 06001000 -> 06009000, 06015000
	add destAddress2,destAddress,#0x8000
	add destAddress3,destAddress,#OBJ_BASE
@	ldr decodePtr,=CHR_DECODE
@	mov mask,#0xFF
render_tiles_3_loop:
	ldr gbaWord,[sourceAddress],#4
	ands pixels0,mask,gbaWord
	ldrne pixels0,[decodePtr,pixels0,lsl#2]
	ands pixels1,mask,gbaWord,lsr#8
	ldrne pixels1,[decodePtr,pixels1,lsl#2]
	orr dest0,pixels0,pixels1,lsl#1
	ands pixels0,mask,gbaWord,lsr#16
	ldrne pixels0,[decodePtr,pixels0,lsl#2]
	ands pixels1,mask,gbaWord,lsr#24
	ldrne pixels1,[decodePtr,pixels1,lsl#2]
	orr pixels1,pixels0,pixels1,lsl#1
	stmia destAddress!,{dest0,pixels1}
	stmia destAddress2!,{dest0,pixels1}
	stmia destAddress3!,{dest0,pixels1}
	subs byteCount,byteCount,#4
	bgt render_tiles_3_loop
	bx lr

@GetNextTileAndLength_recent_first:
@	mov mask,#0xFF
@	ldr bits,=RECENT_TILENUM
@	ldr sourceAddress,=RECENT_TILES
@	ldr decodePtr,=CHR_DECODE
@GetNextTileAndLength_recent:
@	ldrh byteCount,[bits],#2
@	ldrh tileNumber,[bits],#2
@	movs byteCount,byteCount
@	bx lr

	.unreq tileNumber
	.unreq bits
	.unreq bits_ptr
	.unreq mask
	.unreq decodePtr
	.unreq sourceAddress
	.unreq byteCount
	.unreq destAddress
	.unreq destAddress2
	.unreq destAddress3
@	.unreq zero
	.unreq tileNumberMasked
	.unreq gbaWord
	.unreq pixels0
	.unreq dest0
	.unreq pixels1

	.text

store_dirty_packets:
	stmfd sp!,{r3,r4,lr}
	ldrb_ r0,consume_buffer
	movs r0,r0
	blne flush_recent_tiles
	
	@are sprite tiles dirty from writes separate from the packets system?
	ldr r0,=DIRTY_TILE_BITS
	ldr r1,[r0]
	ldr r2,[r0,#0x18]
	orrs r1,r2
	beq 0f
	mov r0,#0
	@invalidate registered packets if tiles are dirty
	ldr r1,=vram_packets_registered_bank0
	str r0,[r1]
	ldr r1,=vram_packets_registered_bank1
	str r0,[r1]
0:
	
	ldr r0,=RECENT_TILES
	ldr r3,=vram_packets_dirty
	ldr r4,=RECENT_TILENUM
store_dirty_packets_loop:
	ldr r2,[r3],#4
	str r2,[r4],#4
	movs r2,r2,lsl#16
	beq store_dirty_packets_finished
	mov r2,r2,lsr#16
	ldr r1,[r3],#4
	bl memcpy_unaligned_src
	b store_dirty_packets_loop
store_dirty_packets_finished:
	mov r0,#1
	strb_ r0,consume_dirty
	strb_ r0,consume_buffer
	ldmfd sp!,{r3,r4,pc}

flush_recent_tiles:
	stmfd sp!,{r3-r11,lr}
	bl_long render_recent_tiles_
	ldmfd sp!,{r3-r11,pc}


































@ decodeptr	.req r2 @mem_chr_decode
@ gbcptr		.req r4 @chr src
@ d_tiles		.req r5 @dirtytiles
@ d_rows		.req r6 @dirtyrows
@ tilenum		.req r8
@ r_tiles 	.req r9
@ r_tnum		.req r7
@ tilesleft	.req r11
@ agbptr_1	.req r3  @shared with ldmia/stmia copy registers
@ agbptr_2	.req r12
@ agbptr_3	.req r11 @shared with tilesleft
@
@@So here's the new tile update system
@
@@update_tile_hook isn't used yet, but I could replace storing/drawing with just drawing.  Probably should do that.
@update_tile_hook: .word store_recent_tile
@@this determines whether to draw stored tiles, or just fail when the tile buffer gets full.  When I replace storing/drawing with just drawing, I won't need this.
@@recent_tiles_full_hook DCD abort_recent_tiles
@init_hook: .word storetiles_init
@
@consumedirty_init:
@	ldr r0,=render_dirty_tile
@	str r0,update_tile_hook
@	mov r_tnum,#0
@	ldr decodeptr,=CHR_DECODE
@	bx lr
@storetiles_init:
@	ldr r0,=store_recent_tile
@	str r0,update_tile_hook
@@	ldr r0,=abort_recent_tiles
@@	str r0,recent_tiles_full_hook
@	ldr r_tiles,=RECENT_TILES
@	ldr r_tnum,=RECENT_TILENUM
@	mov tilesleft,#MAX_RECENT_TILES
@	
@	@seek through recent tiles for first empty slot
@storetiles_init_seek_empty_loop:
@	ldrh r0,[r_tnum]
@	tst r0,#0x8000
@	bne storetiles_init_firstempty
@	subs tilesleft,tilesleft,#1
@	beq storetiles_init_full
@	add r_tnum,r_tnum,#2
@	add r_tiles,r_tiles,#16
@	b storetiles_init_seek_empty_loop
@	
@storetiles_init_firstempty:
@	bx lr
@
@storetiles_init_full:
@	bx lr
@
@@Directly consume dirty tiles
@consume_dirty_tiles:
@	ldrb_ r0,consume_dirty
@	movs r0,r0
@	bxeq lr
@	mov r0,#0
@	strb_ r0,consume_dirty
@
@	ldr r0,=consumedirty_init
@	b set_init_hook_
@@Store dirty tiles into a cache
@gbc_chr_update:
@	ldr r0,=storetiles_init
@set_init_hook_:
@	str r0,init_hook
@check_dirty_rows:
@	@first - check if any tiles are dirty the fast way, look at all rows
@	ldr addy,=DIRTY_ROWS
@	ldmia addy!,{r0-r2}
@	orrs r0,r0,r1
@	orreqs r0,r0,r2
@	ldmeqia addy!,{r0-r2}
@	orreqs r0,r0,r1
@	orreqs r0,r0,r2
@	ldmeqia addy!,{r0-r2}
@	orreqs r0,r0,r1
@	orreqs r0,r0,r2
@	ldmeqia addy!,{r0-r2}
@	orreqs r0,r0,r1
@	orreqs r0,r0,r2
@	bxeq lr @quit if tiles are clean
@	
@	stmfd sp!,{r0-r12,lr}
@	bl process_dirty_tiles
@	ldmfd sp!,{r0-r12,pc}
@
@process_dirty_tiles:
@	stmfd sp!,{lr}
@	mov lr,pc
@	ldr pc,init_hook
@update_tiles:
@	@push lr before entering here
@	ldr d_rows,=DIRTY_ROWS
@	ldr d_tiles,=DIRTY_TILES
@updatetiles:
@	@coarse version on words
@	mov tilenum,#0
@updatetiles_loop:
@	cmp tilenum,#768
@	bge updatetiles_done
@	ldr r0,[d_rows,tilenum,lsr#4]
@	movs r0,r0
@	bne updatetiles_fine
@	add tilenum,tilenum,#64
@	b updatetiles_loop
@
@updatetiles_fine:
@	@fine - operates on bytes
@	@jump here only from 64-aligned tilenum, r0=word from dirtyrows
@	tst r0,#0x000000FF
@	addeq tilenum,tilenum,#16
@	tsteq r0,#0x0000FF00
@	addeq tilenum,tilenum,#16
@	tsteq r0,#0x00FF0000
@	addeq tilenum,tilenum,#16
@
@updaterow_loop:
@	ldrb r0,[d_tiles,tilenum]
@	movs r0,r0
@@@
@	ldrne pc,update_tile_hook
@@was	bne store_recent_tile
@	add tilenum,tilenum,#1
@backto_updaterow_loop:
@	tst tilenum,#0x0F
@	bne updaterow_loop
@updatetiles_resume:
@	tst tilenum,#63
@	beq updatetiles_loop
@updatetiles_fine2:
@	@byte aligned version of updatetiles, jumps back once aligned
@	ldrb r0,[d_rows,tilenum,lsr#4]
@	movs r0,r0
@	bne updaterow_loop
@	add tilenum,tilenum,#16
@	b updatetiles_resume
@
@get_agb_vram_address:
@	ldr agbptr_1,=VRAM_BASE
@	and r1,tilenum,#0x7F
@	add agbptr_1,agbptr_1,r1,lsl#5
@	subs r0,tilenum,#384
@	movlt r0,tilenum
@	addge agbptr_1,agbptr_1,#0x2000
@	tst r0,#0x100
@	addne agbptr_3,agbptr_1,#0x8000  @255-383: bg 8000
@	addne agbptr_2,agbptr_1,#0x8000
@	addne agbptr_1,agbptr_1,#0x8000
@	bxne lr
@	tst r0,#0x080
@	addeq agbptr_3,agbptr_1,#OBJ_BASE @0-127: sprite 0000, bg 0000
@	addeq agbptr_2,agbptr_1,#0x0000
@	addeq agbptr_1,agbptr_1,#0x0000
@
@	addne agbptr_3,agbptr_1,#OBJ_BASE+0x1000 @128-255: sprite 1000, bg 1000, bg 9000
@	addne agbptr_2,agbptr_1,#0x9000
@	addne agbptr_1,agbptr_1,#0x1000
@
@	bx lr
@
@render_dirty_tile:
@	bl get_agb_vram_address
@	.if RESIZABLE
@	ldr_ gbcptr,xgb_vram
@	.else
@	ldr gbcptr,=XGB_VRAM
@	.endif
@	add gbcptr,gbcptr,r0,lsl#4
@	cmp tilenum,#384
@	addge gbcptr,gbcptr,#0x2000
@
@render_dirty_tile_loop:
@	ldrb r0,[gbcptr],#1  @first plane
@	ldrb r1,[gbcptr],#1  @second plane
@	
@	ldr r0,[decodeptr,r0,lsl#2]
@	ldr r1,[decodeptr,r1,lsl#2]
@	orr r0,r0,r1,lsl#1
@	
@	str r0,[agbptr_1],#4
@	str r0,[agbptr_2],#4
@	str r0,[agbptr_3],#4
@	tst agbptr_1,#0x1F @finished with AGB tile?
@	bne render_dirty_tile_loop
@	@next tile
@	mov r0,#0
@	strb r0,[d_tiles,tilenum]
@	add tilenum,tilenum,#1
@	ldrb r0,[d_tiles,tilenum]
@	movs r0,r0
@	beq backto_updaterow_loop
@	tst tilenum,#0x7F
@	bne render_dirty_tile_loop
@	b render_dirty_tile
@
@store_recent_tile:
@	.if RESIZABLE
@	ldr_ gbcptr,xgb_vram
@	.else
@	ldr gbcptr,=XGB_VRAM
@	.endif
@	subs r0,tilenum,#384
@	movlt r0,tilenum
@	add gbcptr,gbcptr,r0,lsl#4
@	addge gbcptr,gbcptr,#0x2000
@storetileloop:
@	subs tilesleft,tilesleft,#1
@@@@@
@@@	movmi lr,pc
@@@	ldrmi pc,recent_tiles_full_hook
@@was	blmi flush_recent_tiles
@	bmi abort_recent_tiles
@
@	ldmia gbcptr!,{r0-r3}
@	stmia r_tiles!,{r0-r3}
@	strh tilenum,[r_tnum],#2
@	mov r0,#0
@	strb r0,[d_tiles,tilenum]
@	add tilenum,tilenum,#1
@	ldrb r0,[d_tiles,tilenum]
@	movs r0,r0
@	beq backto_updaterow_loop
@	cmp tilenum,#384
@	bne storetileloop
@	b store_recent_tile
@
@abort_recent_tiles:
@	mov r0,#1
@	strb_ r0,consume_dirty
@@@	strb r0,recenttilesfull
@	mov r0,#0x8000
@	strh r0,[r_tnum]
@	ldmfd sp!,{pc}
@	
@updatetiles_done:
@	mov r0,#0x8000
@	movs r_tnum,r_tnum
@	strneh r0,[r_tnum]
@	mov r1,#12
@	mov r0,#0
@0:	
@	str r0,[d_rows],#4
@	subs r1,r1,#1
@	bne 0b
@	ldmfd sp!,{pc}
@
@@flush_recent_tiles
@@	stmfd sp!,{r4,r7-r9,lr}
@@	bl consume_recent_tiles_entry
@@	ldr r_tiles,recent_tiles
@@	ldr r_tnum,recent_tilenum
@@	stmfd sp!,{r5,r6}
@@	bl render_recent_tiles
@@	mov tilesleft,#MAX_RECENT_TILES-1
@@	ldmfd sp!,{r5,r6}
@@	ldmfd sp!,{r4,r7-r9,pc}
@
@consume_recent_tiles:
@	ldrb_ r0,consume_buffer
@	movs r0,r0
@	bxeq lr
@	mov r0,#0
@	strb_ r0,consume_buffer
@	
@	stmfd sp!,{lr}
@	
@@	ldrb r0,recenttilesfull
@@	movs r0,r0
@@	beq consume_recent_tiles_entry
@@	
@@	stmfd sp!,{r0-addy,lr}
@@	bl consume_recent_tiles_entry
@@	mov r0,#0
@@	str r0,recenttilesfull
@@	ldr r0,=flush_recent_tiles
@@	str r0,recent_tiles_full_hook
@@	bl flush_dirty_tiles
@@	bl flush_recent_tiles
@@	ldmfd sp!,{r0-addy,pc}
@@consume_recent_tiles_entry
@	ldr decodeptr,=CHR_DECODE
@	ldr r_tiles,=RECENT_TILES
@	ldr r_tnum,=RECENT_TILENUM
@	mov r0,#0x8000
@	ldrh tilenum,[r_tnum]
@	strh r0,[r_tnum],#2
@@	b render_next_tile
@@	
@@render_recent_tiles
@@	ldr decodeptr,=CHR_DECODE
@@	ldrh tilenum,[r_tnum],#2
@consume_next_tile:
@	tst tilenum,#0x8000
@	ldmnefd sp!,{pc}
@	
@	bl get_agb_vram_address
@consume_tile_loop:
@	ldrb r0,[r_tiles],#1  @first plane
@	ldrb r1,[r_tiles],#1  @second plane
@
@	ldr r0,[decodeptr,r0,lsl#2]
@	ldr r1,[decodeptr,r1,lsl#2]
@	orr r0,r0,r1,lsl#1
@	
@	str r0,[agbptr_1],#4
@	str r0,[agbptr_2],#4
@	str r0,[agbptr_3],#4
@	tst agbptr_1,#0x1F @finished with AGB tile?
@	bne consume_tile_loop
@	@next tile
@	add r0,tilenum,#1
@	ldrh tilenum,[r_tnum],#2
@	cmp r0,tilenum
@	bne consume_next_tile
@	tst tilenum,#0x7F  @crossing 128 tile boundary?
@	bne consume_tile_loop
@	b consume_next_tile
@
@ 	.unreq decodeptr
@ 	.unreq gbcptr
@ 	.unreq d_tiles
@ 	.unreq d_rows
@ 	.unreq tilenum
@ 	.unreq r_tiles
@ 	.unreq r_tnum
@ 	.unreq tilesleft
@ 	.unreq agbptr_1
@ 	.unreq agbptr_2
@ 	.unreq agbptr_3

 .section .iwram.1, "ax", %progbits
irqhandler:	@r0-r3,r12 are safe to use
@----------------------------------------------------------------------------
	mov r2,#REG_BASE
	mov r3,#REG_BASE
	ldr r1,[r2,#REG_IE]!
	and r1,r1,r1,lsr#16	@r1=IE&IF
	ldrh r0,[r3,#-8]
	orr r0,r0,r1
	strh r0,[r3,#-8]

		@---this CAN'T be interrupted
		ands r0,r1,#0x80
		strneh r0,[r2,#2]		@IF clear
		ldrne r12,serialfptr
		bxne r12
		@---
		adr r12,irq0

		@---this CAN be interrupted
		ands r0,r1,#0x01
		ldrne r12,vblankfptr
		bne jmpintr
		ands r0,r1,#0x04
		ldrne r12,vcountfptr
		bne jmpintr
		
		@----
		moveq r0,r1				@if unknown interrupt occured clear it.
jmpintr:
	strh r0,[r2,#2]				@IF clear

	mrs r3,spsr
	stmfd sp!,{r3,lr}
	mrs r3,cpsr
	bic r3,r3,#0x9f
	orr r3,r3,#0x1f				@--> Enable IRQ & FIQ. Set CPU mode to System.
	msr cpsr_cf,r3
	stmfd sp!,{lr}
	adr lr,irq0

	bx r12


irq0:
	ldmfd sp!,{lr}
	mrs r3,cpsr
	bic r3,r3,#0x9f
	orr r3,r3,#0x92        		@--> Disable IRQ. Enable FIQ. Set CPU mode to IRQ
	msr cpsr_cf,r3
	ldmfd sp!,{r0,lr}
	msr spsr_cf,r0
vbldummy:
	bx lr
@----------------------------------------------------------------------------
vblankfptr: .word vbldummy			@later switched to vblankinterrupt
@serialfptr DCD serialinterrupt
 .if RUMBLE
serialfptr: .word RumbleInterrupt
 .else
serialfptr: .word vbldummy
 .endif
vcountfptr: .word vcountinterrupt
vcountstate: .word 0
vblankinterrupt:@
@----------------------------------------------------------------------------
	stmfd sp!,{r4-addy,lr}
	ldr globalptr,=GLOBAL_PTR_BASE
	
	@check canary
	ldr r0,=CANARY2
	cmp sp,r0
	ble canary_stack_too_deep
	ldr r1,=0xDEADBEEF
	ldr r0,[r0]
	cmp r0,r1
	bne canary_value_doesnt_match
	b 0f
canary_stack_too_deep:
	mov r11,r11
	b 0f
canary_value_doesnt_match:
	mov r11,r11
	bl_long build_chr_decode
0:
	mov r0,#1
	strb_ r0,vblank_happened
	
	bl display_frame
		
	bl display_sprites
	bl consume_recent_tiles
	bl consume_dirty_tiles
	bl_long force_ui_at_top
	
	ldr r0,=do_gba_hdma
	str r0,vcountfptr

	bl_long showfps_

	ldmfd sp!,{r4-addy,pc}

@@@@@@@
@@@@@@
@@@@@
@@@@
@@@
@@
@
	.ltorg

	.pushsection .text
force_ui_at_top:
	@force first 8 scnalines to contain UI
	@part 1 - save old, find out NEW
	adr_ r4,ui_border_cnt_bic
	ldmia r4,{r0-r3}
	stmfd sp!,{r0-r4,lr}
	ldr_ r1,_ui_border_screen
	orr r1,r1,#1
	bl_long update_ui_border_masks_2
	
	mov r2,#REG_BASE
	@write buffer values for scanline 0
	ldr_ r1,bigbufferbase2
	ldmia r1,{r3-r8}
	
	@force UI visible in first 8 scanlines
	@part 2: Apply changes
	ldr_ r1,ui_border_cnt_bic
	bic r4,r4,r1
	ldr_ r1,ui_border_cnt_orr
	orr r4,r4,r1
	ldr_ r1,_ui_border_screen
	ands r1,r1,#2
	ldr_ r0,_ui_y
	ldr_ r8,_ui_x
	orr r8,r8,r0,lsl#16
	movne r7,r8
	ldrne_ r8,ui_border_scroll3
	
	orr r1,r1,#1
	@update WININ register
	cmp r1,#2
	ldrle r0,=0xFFE8FFFA @one layer
	cmp r1,#3
	ldreq r0,=0xFFECFFFE @two layers
	str r0,[r2,#REG_WININ]
	
	add r1,r2,#REG_BG0CNT
	stmia r1,{r3-r8}
	
	mov r0,#0
	strh r0,[r2,#REG_WIN0H]
	
	ldr_ r1,dispcntbase2
	ldrh r0,[r1]
	bic r0,r0,#0x2000 @remove window 0 if present
	strh r0,[r2,#REG_DISPCNT]
	
	@finish up the job at scanline 7, writing new values as scanline 8 starts
	@ldr r0,=do_gba_hdma
	@ldr r1,=vcountfptr
	@str r0,[r1]

	ldr r0,=0x28  + 256*(SCREEN_Y_START-1) @scanline 7, enable vblank+vcount interrupts
	strh r0,[r2,#REG_DISPSTAT]

	@Force UI visible in first 8 scanlines
	@part 3: Restore old values
	ldmfd sp!,{r0-r4,lr}
	stmia r4,{r0-r3}
	bx lr
	.popsection

	.pushsection .vram1, "ax", %progbits
newframeinit:
	mov r0,#0
	strb_ r0,buffer_lastscanline

	ldr_ r0,bigbufferbase@2
	str_ r0,bigbuffer
	ldr_ r0,dispcntbase@2
	str_ r0,dispcntaddr

	mvn r0,#(SCREEN_Y_START-1)  @-8
	mov r0,r0,lsl#16
	str_ r0,windowyscroll
	b_long newmode
	.popsection

	
	
	@fall thru to newmode  (calls newmode then returns)

newmode:
	ldrb_ r0,lcdctrl
	tst r0,#0x20
	beq entermode0
	ldr_ r0,scanline_oam_position  @if cycles >=scanline_oam_position, inside hblank
	cmp cycles,r0
	ldrb_ r0,scanline
	addmi r0,r0,#1
	cmp r0,#144
	movge r0,#0
	ldrb_ r1,windowY
	cmp r0,r1
	blt entermode0
	
	ldrb_ r1,windowX
	cmp r1,#166
	bgt entermode0
	cmp r1,#7
	ble entermode1

	@r1=bigbuffer
	@r2=bg01cnt
	@r3=bg23cnt
	@r4=xyscroll
	@r5=xyscroll2
	
	@dispcntdata
	@windata
	@dispcntaddr
entermode2:
@	ldrb r0,rendermode
@	cmp r0,#2
@	moveq r1,pc
@	bxeq lr
entermode2_:
	mov r0,#2
	strb_ r0,rendermode
	@get LCDC
	ldrb_ r0,lcdctrl
	@default dispcnt | enable win 0, win 1, all 4 bg layers
	ldr r1,=0x5F40 | 0x6F00
	tst r0,#0x02 @sprite enable
	biceq r1,r1,#0x1000
	tst r0,#0x80 @screen off?
@	orreq r1,r1,#0x0F00  ;enable all layers, not necessary since they are on
	biceq r1,r1,#0x3000  @disable win 0, sprites, let the white palette mask the screen
	orr r1,r1,r1,lsl#16
	str_ r1,dispcntdata
	
	@default bgXcnt: 000mmmmm0000ccpp
	ldr r1,= TILEMAP1*0x100 + 0x02	@formerly 1A02
	tst r0,#0x10 @Tile Select
	orreq r1,r1,#0x0008

	orr r1,r1,r1,lsl#16
	tst r0,#0x08 @BG Tilemap
	addne r1,r1,#0x0300
	tst r0,#0x40 @Win Tilemap
	addne r1,r1,#0x03000000

	orr r1,r1,#0x00010000 @priority fix?

	tst r0,#0x01 @BG enable
	bne 0f
	ldrb_ r0,gbcmode
	cmp r0,#1
	orreq r1,r1,#0x0003	@For GBC, low priority for all layers
	addne r1,r1,#0x0100	@For non-gbc, make all tiles color zero
	.if COLORZERO_CHARBASE == 2
	orrne r1,r1,#0x0008
	.else
	bicne r1,r1,#0x000C
	.endif
0:
	str_ r1,bg01cnt
	add r1,r1,#0x00000100 @for color 0 tiles (BG)
	add r1,r1,#0x01000000
	.if COLORZERO_CHARBASE == 2
	orr r1,r1,#0x00000008
	orr r1,r1,#0x00080000
	.else
	bic r1,r1,#0x0000000C
	bic r1,r1,#0x000C0000
	.endif
	str_ r1,bg23cnt

mode2_update_scroll:
	ldr_ r1,windowyscroll
	
	@get wx
	ldrb_ r0,windowX
	
	@wx to screen scroll
	rsb r0,r0,#0
	sub r0,r0,#(SCREEN_X_START-7)
	mov r0,r0,lsl#16
	orr r1,r1,r0,lsr#16
	str_ r1,xyscroll2
	
	@wx to GBA window position
	rsb r0,r0,#0
	mov r0,r0,lsr#8
	orr r0,r0,#(SCREEN_X_START+160)
	orr r0,r0,r0,lsl#16
	str_ r0,windata
	
	@get y pos
	ldrb_ r0,scrollY
	sub r0,r0,#SCREEN_Y_START
	mov r1,r0,lsl#16
	
	@get x pos
	ldrb_ r0,scrollX
	sub r0,r0,#SCREEN_X_START
	mov r0,r0,lsl#16
	orr r1,r1,r0,lsr#16
	str_ r1,xyscroll
	bx lr
	
@nowindow
entermode0:
entermode0_:
	mov r0,#0
	strb_ r0,rendermode
	@get LCDC
	ldrb_ r0,lcdctrl
	
	@default dispcnt, win 1, all 4 bg layers
	ldr r1,=0x5F40
	tst r0,#0x02 @sprite enable
	biceq r1,r1,#0x1000
	orr r1,r1,r1,lsl#16
	str_ r1,dispcntdata
	
	@default bgXcnt: 000mmmmm000cccpp
	ldr r1,= TILEMAP1*0x100 + 0x02	@formerly 1A02
	tst r0,#0x10 @Tile Select
	orreq r1,r1,#0x0008
	tst r0,#0x08 @BG Tilemap
	addne r1,r1,#0x0300
	orr r1,r1,r1,lsl#16
	add r1,r1,#0x01000000 @for color 0 tiles
	orr r1,r1,#0x00010000
	.if COLORZERO_CHARBASE == 2
	orr r1,r1,#0x00080000
	.else
	bic r1,r1,#0x000C0000
	.endif
	tst r0,#0x01 @BG enable
	bne 0f
	ldrb_ r0,gbcmode
	cmp r0,#1
	addne r1,r1,#0x0100
@	orrne r1,r1,#0x0001  ;because there's the unconditional right below this
	.if COLORZERO_CHARBASE == 2
	orrne r1,r1,#0x0008
	.else
	bicne r1,r1,#0x000C
	.endif
	orr r1,r1,#0x0001
	movs r0,#0
0:	
	str_ r1,bg01cnt
	addne r1,r1,#0x0200
	subne r1,r1,#1
	str_ r1,bg23cnt
mode0_update_scroll:
	@get y pos
	ldrb_ r0,scrollY
	sub r0,r0,#SCREEN_Y_START
	mov r1,r0,lsl#16
	
	@get x pos
	ldrb_ r0,scrollX
	sub r0,r0,#SCREEN_X_START
	mov r0,r0,lsl#16
	orr r1,r1,r0,lsr#16
	str_ r1,xyscroll
	bx lr

@nobg
entermode1:
@	ldrb r0,rendermode
@	cmp r0,#1
@	moveq r1,pc
@	bxeq lr
entermode1_:
	mov r0,#1
	strb_ r0,rendermode
	@get LCDC
	ldrb_ r0,lcdctrl
	
	@default dispcnt, win 1, all 4 bg layers
	ldr r1,=0x5F40
	tst r0,#0x02 @sprite enable
	biceq r1,r1,#0x1000
	orr r1,r1,r1,lsl#16
	str_ r1,dispcntdata
	
	@default bgXcnt: 000mmmmm000cccpp
	ldr r1,= TILEMAP1*0x100 + 0x02	@formerly 1A02
	tst r0,#0x10 @Tile Select
	orreq r1,r1,#0x0008
	tst r0,#0x40 @Window Tilemap
	addne r1,r1,#0x0300
	orr r1,r1,r1,lsl#16
	add r1,r1,#0x01000000 @for color 0 tiles
	orr r1,r1,#0x00010000
	.if COLORZERO_CHARBASE == 2
	orr r1,r1,#0x00080000
	.else
	bic r1,r1,#0x000C0000
	.endif
	tst r0,#0x01 @BG enable
	bne 0f
	ldrb_ r0,gbcmode
	cmp r0,#1
	orreq r1,r1,#0x0001
0:	
	str_ r1,bg01cnt
	addne r1,r1,#0x0200
	subne r1,r1,#1
	str_ r1,bg23cnt
	
mode1_update_scroll:
	ldr_ r1,windowyscroll
	@get wx
	ldrb_ r0,windowX
	
	@wx to screen scroll
	rsb r0,r0,#0
	sub r0,r0,#(SCREEN_X_START-7)
	mov r0,r0,lsl#16
	orr r1,r1,r0,lsr#16
	str_ r1,xyscroll
	bx lr
 .pushsection .iwram.3

bufferfinish:
	mov r2,#144
	b 0f
tobuffer:
	ldrb_ r2,scanline
	
	@if in vblank time, always treat scanline as zero
	ldrb r0,lcdstat
	tst r0,#0x01
	movne r2,#0
	bne 0f
	
	@fixme: make it not add 1 for the pre-scanline
	ldr_ r0,scanline_oam_position  @if cycles >=scanline_oam_position, inside hblank
	cmp cycles,r0
	bpl 1f
	@want to know if we are in vblank but scanline 0
	addmi r2,r2,#1
3:
	cmp r2,#145
	b 2f
1:	
	cmp r2,#144
2:
	movge r2,#0
0:
	ldrb_ r1,buffer_lastscanline
	subs r0,r2,r1
	
	bxeq lr
	ble bufferfinish
	
	@check if will reach wy
	ldrb_ addy,windowY
	@it will reach if:
	@s0<wy && s1>=wy
	cmp r1,addy
	bge 0f
	cmp r2,addy
	blt 0f
	@check if will reach a new mode, can only be in "no window" since window wasn't hit yet
	ldrb_ r1,lcdctrl
	tst r1,#0x20
	beq 0f
	ldrb_ r1,windowX
	cmp r1,#166
	bgt 0f
	
	ldrb_ r1,buffer_lastscanline
	@Will enter a window mode, so call rest of routine, then come back to finish
	sub r0,addy,r1
	stmfd sp!,{r2,addy,lr}
	bl 0f
	ldrb_ r1,windowX
	cmp r1,#7
	bgt 1f
	bl entermode1_
	cmp r0,r0
1:
	blgt entermode2_
	ldmfd sp!,{r2,addy,lr}

	subs r0,r2,addy
	bxeq lr
0:	
	strb_ r2,buffer_lastscanline
	ldrb_ r1,rendermode
	cmp r1,#1
	bgt tobuffer_split
	ldrlt_ addy,windowyscroll
	sublt addy,addy,r0,lsl#16
	strlt_ addy,windowyscroll
	
	@r0 = scanlines remaining
	
	@r1=bigbuffer
	@r2=bg01cnt
	@r3=bg23cnt
	@r4=xyscroll
	@r5=xyscroll2
	
	@dispcntdata
	@windata
	@dispcntaddr
	
	@windowyscroll
	
	stmfd sp!,{r3-r8,lr}
	@now we need intervention of UI and Border here
	adr_ addy,bigbuffer
	ldmia addy,{r1-r4}
	mov r5,r4
	mov r6,r4
	mov r7,r4
	
	ldr_ r8,ui_border_cnt_bic
	tst r8,#0xFF000000
	beq 0f
	bic r3,r3,r8
	ldr_ r8,ui_border_cnt_orr
	orr r3,r3,r8
	tst r8,#0x0000FF00
	ldrne_ r6,ui_border_scroll2
	ldr_ r7,ui_border_scroll3
0:	
	mov r8,r0
	
	
	
1:
	stmia r1!,{r2,r3,r4,r5,r6,r7}
	subs r0,r0,#1
	beq 2f
	stmia r1!,{r2,r3,r4,r5,r6,r7}
	subs r0,r0,#1
	beq 2f
	stmia r1!,{r2,r3,r4,r5,r6,r7}
	subs r0,r0,#1
	beq 2f
	stmia r1!,{r2,r3,r4,r5,r6,r7}
	subs r0,r0,#1
	bne 1b
2:
	str_ r1,bigbuffer
	
	adrl_ addy,dispcntdata
	ldr_ r2,dispcntaddr
@	add r4,r2,#144*2
	bl apply16
	str_ r2,dispcntaddr
@	mov r2,r4
@	bl apply16
	ldmfd sp!,{r3-r8,pc}
.popsection
tobuffer_split:
	stmfd sp!,{r3-r8,lr}
	adr_ addy,bigbuffer
	ldmia addy,{r1-r5}
	mov r6,r4
	mov r7,r5

	ldr_ r8,ui_border_cnt_bic
	tst r8,#0xFF000000
	beq 0f
	bic r3,r3,r8
	ldr_ r8,ui_border_cnt_orr
	orr r3,r3,r8
	tst r8,#0x0000FF00
	ldrne_ r6,ui_border_scroll2
	ldr_ r7,ui_border_scroll3
0:	
	mov r8,r0

1:
	stmia r1!,{r2,r3,r4,r5,r6,r7}
	subs r0,r0,#1
	beq 2f
	stmia r1!,{r2,r3,r4,r5,r6,r7}
	subs r0,r0,#1
	beq 2f
	stmia r1!,{r2,r3,r4,r5,r6,r7}
	subs r0,r0,#1
	beq 2f
	stmia r1!,{r2,r3,r4,r5,r6,r7}
	subs r0,r0,#1
	bne 1b
2:
	str_ r1,bigbuffer

	adrl_ addy,dispcntdata
	ldr_ r2,dispcntaddr
	add r4,r2,#144*2
	bl apply16
	str_ r2,dispcntaddr
	mov r2,r4
	adrl_ addy,windata
	bl apply16
	ldmfd sp!,{r3-r8,pc}

apply16:
	@sets halfwords, plus one extra one if end is not aligned
	@r8 = count (in halfwords)
	@r2 = dest, does not have to be word aligned
	@addy = pointer to halfword repeated - 4
	
	@out: r2 = next halfword to write to
	mov r0,r8,lsl#1
	
	@make aligned
	tst r2,#2
	ldrne r1,[addy]!
	strneh r1,[r2],#2
	subne r0,r0,#2
	
	@DMA transfer, possibly plus one extra halfword
	mov r1,#REG_BASE
	str addy,[r1,#REG_DM3SAD]
	add addy,r0,#2  @add extra halfword if final halfword is not word aligned
	movs addy,addy,lsr#2
	strne r2,[r1,#REG_DM3DAD]
	orrne addy,addy,#0x85000000 @enable, fixed src, inc dest
	strne addy,[r1,#REG_DM3CNT_L]
	bicne addy,addy,#0xFF000000
	add r2,r2,r0
	bx lr

	.ltorg
	
@
@@
@@@
@@@@
@@@@@
@@@@@@
@@@@@@@

	.pushsection .text
move_ui_2:
	adr_ r2,ui_border_cnt_bic
	ldmia r2,{r4-r7}
@	ldr r4,ui_border_cnt_bic
@	ldr r5,ui_border_cnt_orr
@	ldr r6,ui_border_scroll2
@	ldr r7,ui_border_scroll3
	tst r4,#0xFF000000
	bxeq lr
	ldr_ r1,bigbufferbase2
	mov r2,#144
	tst r4,#0x0000FF00
	beq_long _move_ui_2_core
1:
0:
	str r5,[r1,#4]
	str r6,[r1,#16]
	str r7,[r1,#20]
	add r1,r1,#24
	subs r2,r2,#1
	bne 0b
	bx lr
	.popsection
_move_ui_2_core:
0:
	ldr r0,[r1,#4]
	bic r0,r0,r4
	orr r0,r0,r5
	str r0,[r1,#4]
	str r7,[r1,#20]
	add r1,r1,#24
	subs r2,r2,#1
	bne 0b
	bx lr

add_ui_border:
	@Check if state of UI and border is correct
	ldr_ r1,_ui_border_screen
	ldr_ r0,_ui_border_request
	str_ r0,_ui_border_screen
	
	and r12,r0,#3
	@update WININ and WINOUT registers
	cmp r12,#2
	ldrle r2,=0xFFE8FFFA @one layer
	cmp r12,#3
	ldreq r2,=0xFFECFFFE @two layers
	cmp r12,#0
	ldreq r2,=0xFFE0FFFA @no extra layers
	mov r3,#REG_BASE
	str r2,[r3,#REG_WININ]

	@set BLDY
	ldrb_ r2,_darkness
	strh r2,[r3,#REG_BLDY]
	tst r2,#0x1F
	moveq r2,#0x0000
	bne_long _add_ui_border_2
_add_ui_border_3:
	str r2,[r3,#REG_BLDMOD]
	
	@now check if UI or border has moved
	cmp r1,r0
	bxeq lr

	.pushsection .text
_add_ui_border_2:
	beq 0f
	tst r2,#0x80	@if set the MSBit, brightness up instead of down

	@Set BLDMOD
	moveq r2,#0x00DF	@brightness down, all layers + obj
	movne r2,#0x00BF	@brightness up, all layers+obj+backdrop
	bne 0f			@brightness up applies to all layers
	cmp r12,#3	@UI and Border visible?
	biceq r2,r2,#0x0004 @no dark for BG2
	cmp r12,#1	@only UI visible?
	biceq r2,r2,#0x0008 @no dark for BG3
0:
	b_long _add_ui_border_3
	.popsection

	
	b_long _add_ui_border_4
	.pushsection .text
_add_ui_border_4:
	@now apply the settings to the active (to be displayed) scanline buffers
	adr_ r12,ui_border_cnt_bic
	ldmia r12,{r0-r3}
	stmfd sp!,{r0-r3,lr}		@push old
	ldr_ r1,_ui_border_screen
	bl_long update_ui_border_masks_2	@this gets restored afterwards
	bl_long move_ui_2
	ldmfd sp!,{r0-r3,lr}
	stmia r12,{r0-r3}		@restore old
	bx lr
	.popsection

update_ui_border_masks:
	ldr_ r1,_ui_border_request
	str_ r1,_ui_border_buffer
update_ui_border_masks_2:	
	and r0,r1,#3
	cmp r0,#1
	beq 1f
	cmp r0,#2
	beq 2f
	cmp r0,#3
	beq 3f
0: @neither visible
	mov r0,#0
	str_ r0,ui_border_cnt_bic
	str_ r0,ui_border_cnt_orr
	bx lr
1: @UI visible
	ldr r0,=0xFFFF0000
	str_ r0,ui_border_cnt_bic
	@ldr r0,=0x40000000 + UI_TILEMAP*0x1000000 + UI_CHARBASE*0x40000
	ldr r0,=0x00000000 + UI_TILEMAP*0x1000000 + UI_CHARBASE*0x40000
	str_ r0,ui_border_cnt_orr

	mov r0,r1,lsr#8    @ui_x
	bic r0,r0,#0x00FF0000
	mov r1,r1,lsr#24   @ui_y
	orr r0,r0,r1,lsl#16
	
	str_ r0,ui_border_scroll3
	bx lr
2: @Border visible
	ldr r0,=0xFFFF0000
	str_ r0,ui_border_cnt_bic
	ldr r0,= BORDER_TILEMAP*0x1000000 + BORDER_CHARBASE*0x40000
	str_ r0,ui_border_cnt_orr
@snes_screen_y = (224-144)/2 = 40
@gba_screen_y = (160-144)/2 = 8
@snes y >> gba y: 40-8
@snes x >> gba x: 48-40
	ldr r0,= (40-SCREEN_Y_START)*65536 + (48-SCREEN_X_START)
	str_ r0,ui_border_scroll3
	bx lr
3: @UI and Border visible
	mvn r0,#0
	str_ r0,ui_border_cnt_bic
	@ldr r0,=0x00004000 + BORDER_TILEMAP*0x1000000 + BORDER_CHARBASE*0x40000 + UI_TILEMAP*0x100 + UI_CHARBASE*0x04
	ldr r0,=0x00000000 + BORDER_TILEMAP*0x1000000 + BORDER_CHARBASE*0x40000 + UI_TILEMAP*0x100 + UI_CHARBASE*0x04
	str_ r0,ui_border_cnt_orr
	ldr r0,= (40-SCREEN_Y_START)*65536 + (48-SCREEN_X_START)
	str_ r0,ui_border_scroll3

	mov r0,r1,lsr#8    @ui_x
	bic r0,r0,#0x00FF0000
	mov r1,r1,lsr#24   @ui_y
	orr r0,r0,r1,lsl#16

	str_ r0,ui_border_scroll2

	bx lr


display_frame:	@called at vblank
	stmfd sp!,{globalptr,lr}
	@init windows
	mov r1,#REG_BASE
	ldr r0,=(SCREEN_X_START*256 + (SCREEN_X_START+160))
	strh r0,[r1,#REG_WIN1H]		@Win1H
	ldr r0,=(SCREEN_Y_START*256 + (SCREEN_Y_START+144))*0x10001
	str r0,[r1,#REG_WIN0V]		@Win0V

	@fill line buffers
@	bl fill_line_buffers
	bl add_ui_border
	ldrb_ r0,bg_cache_updateok
	movs r0,r0
	blne transfer_palette_

	bl display_bg
	
	ldmfd sp!,{globalptr,pc}

display_bg:
	ldrb_ r0,bg_cache_updateok
	ldrb_ r1,sgb_mask
	eor r1,r1,#1
	ands r0,r0,r1
	bxeq lr
render_dirty_bg:
	ldr r5,=0x00680068
	ldr r6,=0x00070007
	ldr r7,=0x00010001
	ldr r8,=0x72007200
	ldr r9,=0xFDFF
	
	stmfd sp!,{lr}
	.if RESIZABLE
	ldr addy,=XGB_vram_1800
	ldr addy,[addy]
	.else
	ldr addy,=XGB_VRAM+0x1800
	.endif
	
	ldr r1,=dirty_map_words
	str r1,dirty_map_base

	ldr r11,=VRAM_BASE+TILEMAP1*2048 @0x06005000
	bl update_whole_map_2
	.if RESIZABLE
	ldr addy,=XGB_vram_1C00
	ldr addy,[addy]
	.else
	ldr addy,=XGB_VRAM+0x1800+0x400
	.endif
	
	ldr r1,=dirty_map_words + 32
	str r1,dirty_map_base
	
	ldr r11,=VRAM_BASE+TILEMAP2*2048 @0x06006800
	bl update_whole_map_2
	ldmfd sp!,{pc}

dirty_map_base:
	.word 0

update_whole_map_2:
	ldr r1,dirty_map_base
0:
	and r2,r11,#0x700  @tile number (aligned to multiple of 128)
	ldr r10,[r1,r2,lsr#6]
	
	@clear the word
	mov r0,#0
	str r0,[r1,r2,lsr#6]
	
	movs r10,r10
	bne row_contains_dirty
	@increment destination address and source address by a row
	add r11,r11,#0x100
	add addy,addy,#0x80
	tst r11,#0x7C0
	bne 0b
	bx lr
row_contains_dirty:
	movs r10,r10,lsr#1
	bcs this_word_dirty
	beq no_dirty_remaining
	add addy,addy,#4
	add r11,r11,#8
	b row_contains_dirty
this_word_dirty:
	ldr r4,[addy,r7,lsr#3] @constant 0x2000
	ldr r3,[addy],#4
1:
	and r2,r3,#0xFF00
	and r0,r3,#0xFF
	orr r0,r0,r2,lsl#8
	and r2,r4,#0xFF00
	and r1,r4,#0xFF
	orr r1,r1,r2,lsl#8
	
	@destroys: r0,r1,r2
	@increments: r11 += 4
	@uses masks: r5,r6,r7,r8,r9
	@preserves carry flag
	and r2,r1,r5 @#0x00680068 ;vflip, hflip, +100
	orr r0,r0,r2,lsl#5
	and r2,r1,r6 @#0x00070007 ;palette
	add r2,r2,r7,lsl#3 @#0x00080008 ;+8
	orr r0,r0,r2,lsl#12
	
	add r2,r2,r8 @#0x72007200 ;color 0 tile
	
	str r0,[r11],#4
	str r2,[r11,#0x800-4]
	
	tst r1,#0x80	@high priority tile
	biceq r0,r0,r9	@0xFDFF ;set to tile 512
	orreq r0,r0,#0x200
	tst r1,#0x800000
	biceq r0,r0,r9,lsl#16
	orreq r0,r0,#0x2000000
	str r0,[r11,#0x1000-4]
	
	tst r11,#4
	movne r3,r3,lsr#16
	movne r4,r4,lsr#16
	bne 1b
	b row_contains_dirty

no_dirty_remaining:
	ands r0,r11,#0xFE @tile number 
	movne r1,#0x80
	subne r1,r1,r0,lsr#1
	addne r11,r11,r1,lsl#1
	addne r12,r12,r1
	tst r11,#0x7C0
	bne update_whole_map_2
	bx lr
	
@----------------------------------------------------------------------------
transfer_palette_:
@----------------------------------------------------------------------------
	stmfd sp!,{r0-r9,globalptr,lr}
	ldr globalptr,=GLOBAL_PTR_BASE
	
	ldrb_ r0,update_border_palette
	movs r0,r0
	blne_long _update_the_border_palette
	
	ldr r4,=PALETTE_BASE+256

	ldrb_ r2,sgb_mask
	cmp r2,#2
	bge_long sgb_mask_palette

	ldrb_ r2,lcdctrl0frame
	tst r2,#0x80 @screen off?
	beq_long white_palette
	
	ldr r5,=gbc_palette2
	
	mov r8,#16 @16 palettes

	ldrb_ r1,_gammavalue
	movs r1,r1   
	beq nogamma1
	
pal_loop1:
	mov r9,#4
pal_loop11:
	bl gamma_convert_one
	
	subs r9,r9,#1
	bne pal_loop11
	add r4,r4,#24
	subs r8,r8,#1
	bne pal_loop1
	b aftergamma
nogamma1:
pal_loop2:
	ldmia r5!,{r0,r1}
	stmia r4,{r0,r1}
	add r4,r4,#32
	subs r8,r8,#1
	bne pal_loop2
aftergamma:	
	
	@Copy SGB palette to first entry based on UI selection, this will be removed when proper colorizing is supported
	ldrb_ r0,sgbmode
	movs r0,r0
	beq 9f
	ldrb_ r0,_sgb_palette_number
	movs r0,r0
@	beq 9f
	bne_long _transfer_palette_sgb
_transfer_palette_sgb_2:
9:	
	@for the hell of it, copy first transparent color to color zero
	@DELETEME?
	@only do it if SGB border is visible
	ldrb_ r0,_ui_border_screen
	tst r0,#2
	
	@otherwise pick black border?
	ldr r0,=PALETTE_BASE
	ldr r1,=PALETTE_BASE+0x100
	ldrneh r3,[r1]
	moveq r3,#0
	strh r3,[r0]
	
	@now copy first 8 transparent colors to solid blocks
	ldr r0,=PALETTE_BASE+0xF0
	ldr r1,=PALETTE_BASE+0x100
	mov r2,#8
pal_loop3:
	ldrh r3,[r1],#32
	strh r3,[r0],#2
	
	subs r2,r2,#1
	bne pal_loop3
	
	ldmfd sp!,{r0-r9,globalptr,lr}
	bx lr

	.pushsection .text
_transfer_palette_sgb:	
	ldr r2,=PALETTE_BASE+0x100
	ldr r1,[r2,r0,lsl#5]
	str r1,[r2]
	add r2,r2,#4
	ldr r1,[r2,r0,lsl#5]
	str r1,[r2]
	ldr r2,=PALETTE_BASE+0x200
	ldr r1,[r2,r0,lsl#6]
	str r1,[r2]
	add r2,r2,#4
	ldr r1,[r2,r0,lsl#6]
	str r1,[r2]
	add r2,r2,#0x1C
	ldr r1,[r2,r0,lsl#6]
	str r1,[r2]
	add r2,r2,#4
	ldr r1,[r2,r0,lsl#6]
	str r1,[r2]
	b_long _transfer_palette_sgb_2
	.popsection


	.pushsection .text
white_palette:
	ldrb_ r0,gbcmode
	movs r0,r0
	mvn r0,#0
	bne 3f
	mov r2,#4
sgb_mask_palette:
	mov r0,#0
	cmp r2,#4
	ldreq r0,=SGB_PALETTE @if 3, use black, if 4, use SGB color #0
	ldreqh r0,[r0]
	orreq r0,r0,r0,lsl#16
3:
	mov r1,r0
	mov r2,#16
	b_long whitepal_loop
	.popsection
whitepal_loop:
	stmia r4,{r0,r1}
	add r4,r4,#32
	subs r2,r2,#1
	bne whitepal_loop
	b aftergamma

	.pushsection .text

_update_the_border_palette:
	mov r0,#0
	strb_ r0,update_border_palette

update_the_border_palette:
	stmfd sp!,{lr}
	
	ldr r4,=PALETTE_BASE+32
	ldr r5,=BORDER_PALETTE
	
	mov r9,#64 @64 colors

	ldrb_ r1,_gammavalue
	movs r1,r1
	beq 1f
	
0:
	bl_long gamma_convert_one
	subs r9,r9,#1
	bne 0b
	ldmfd sp!,{pc}
	
1:
	ldr r0,[r5],#4
	str r0,[r4],#4
	subs r9,r9,#2
	bne 1b
	ldmfd sp!,{pc}
	.popsection

gamma_convert_one:
	@r5 = src, r4 = dest
	stmfd sp!,{lr}

	ldrh r6,[r5],#2 @read color
	ands r0,r6,#0x1F  @red gamma
	addne r0,r0,#1
	mov r0,r0,lsl#3
	subne r0,r0,#1
	bl gammaconvert
	mov r7,r0
	ands r0,r6,#0x3E0 @green gamma
	addne r0,r0,#0x20
	mov r0,r0,lsr#2
	subne r0,r0,#1
	bl gammaconvert
	orr r7,r7,r0,lsl#5
	ands r0,r6,#0x7C00 @blue gamma
	addne r0,r0,#400
	mov r0,r0,lsr#7
	subne r0,r0,#1
	bl gammaconvert
	orr r7,r7,r0,lsl#10 @all combined
	strh r7,[r4],#2
	
	ldmfd sp!,{pc}
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

@----------------------------------------------------------------------------
FF46_W:@		sprite DMA transfer
@----------------------------------------------------------------------------
@	and r0,r0,#0xff		;not needed?
	and r1,r0,#0xF0
	adr_ r2,memmap_tbl
	ldr r1,[r2,r1,lsr#2]	@in: addy,r1=addy&0xE000 (for rom_R)
	add r1,r1,r0,lsl#8	@r1=DMA source
	
	@new code: check for 3E 28 3D 20 FD C9
	ldrb r0,[gb_pc,#0]
	cmp r0,#0x3E
	ldreqb r0,[gb_pc,#5]
	cmpeq r0,#0xC9
	ldreqb r0,[gb_pc,#2]
	cmpeq r0,#0x3D
	bne 1f
	@first burn 3*4 cycles, then burn 160*4 cycles
	sub cycles,cycles,#CYCLE*163*4
	@return to RET instruction _C9
	ldr lr,[r10,#0xC9*4]
1:
	@are we on scanline 100 or higher (but not in vblank time)
	ldrb_ r0,scanline
	tst cycles,#CYC_LCD_ENABLED
	moveq r0,#0
	cmp r0,#144
	movge r0,#0
	cmp r0,#100
	bge_long upload_sprites_early
2:
	mov r0,#1
	strb_ r0,gboamdirty
	ldr_ r0,gb_oam_buffer_writing
after_upload_sprites_early:
	mov r2,#160		@number of sprites on the GB
	b memcpy32

.pushsection .text
upload_sprites_early:
	@donkey kong land, beavis and butthead, etc upload sprites early (before vblank time)
	mov r0,#3
	strb_ r0,gboamdirty
	@swap oam_buffer_alt and oam_buffer_writing and
	@write new OAM data to oam_buffer_writing
	ldr_ r0,gb_oam_buffer_alt
	ldr_ r2,gb_oam_buffer_writing
	str_ r2,gb_oam_buffer_alt
	str_ r0,gb_oam_buffer_writing
	
	stmfd sp!,{r0,r1,r12,lr}
	bl flush_recent_tiles   @preserves registers
	bl_long store_recent_tiles
	mov r0,#2
	strb_ r0,consume_buffer
	ldmfd sp!,{r0,r1,r12,lr}
	b_long after_upload_sprites_early
.popsection


display_sprites:
@----------------------------------------------------------------------------
OAMfinish:@		transfer OAM from GB to GBA
@----------------------------------------------------------------------------
 PRIORITY = 0x800	@0x800=AGB OBJ priority 2
	ldr_ addy,gb_oam_buffer_screen
	add r9,addy,#0xA0
	mov r2,#AGB_OAM
 	
	ldrb_ r5,gbcmode

	ldrb_ r0,lcdctrl0frame
@	ldr r0,active_lcdcbuff
@	ldrb r0,[r0,#71]	;8x16?
@	ldrb r0,lcdctrl		;8x16?
	tst r0,#0x04
	bne dm4
dm11:
	ldr r3,[addy],#4
	ands r0,r3,#0xff
	beq dm10		@skip if sprite Y=0
	cmp r0,#159
	bhi dm10		@skip if sprite Y>159

	add r0,r0,#(-16+SCREEN_Y_START)
	and r0,r0,#0xff

	and r1,r3,#0xff00
	add r1,r1,#(SCREEN_X_START-8)*256	@adjusted x
	orr r0,r0,r1,lsl#8
	and r1,r3,#0x60000000	@flip
	orr r0,r0,r1,lsr#1
	str r0,[r2],#4		@store OBJ Atr 0,1

@  Bit7   OBJ-to-BG Priority (0=OBJ Above BG, 1=OBJ Behind BG color 1-3)
@         (Used for both BG and Window. BG color 0 is always behind OBJ)
@  Bit6   Y flip          (0=Normal, 1=Vertically mirrored)
@  Bit5   X flip          (0=Normal, 1=Horizontally mirrored)
@  Bit4   Palette number  **Non CGB Mode Only** (0=OBP0, 1=OBP1)
@  Bit3   Tile VRAM-Bank  **CGB Mode Only**     (0=Bank 0, 1=Bank 1)
@  Bit2-0 Palette number  **CGB Mode Only**     (OBP0-7)

@  Bit   Expl.
@  0-9   Character Name          (0-1023=Tile Number)
@  10-11 Priority relative to BG (0-3; 0=Highest)
@  12-15 Palette Number   (0-15) (Not used in 256 color/1 palette mode)


	mov r1,#PRIORITY
	tst r3,#0x80000000	@priority
	orrne r1,r1,#PRIORITY>>1
	cmp r5,#0
	@gbc vram bank
	andne r0,r3,#0x08000000
	orrne r1,r1,r0,lsr#19
	@gbc color
	andne r0,r3,#0x07000000
	orrne r1,r1,r0,lsr#12
@	;gb color
	andeq r0,r3,#0x10000000
	orreq r1,r1,r0,lsr#16
	@tile
	and r0,r3,#0x00FF0000
	orr r1,r1,r0,lsr#16
	.if OBJ_BASE == 0x14000	
	add r1,r1,#512
	.endif
	strh r1,[r2],#4		@store OBJ Atr 2
dm9:
	cmp addy,r9
	bne dm11
	bx lr
dm10:
	mov r0,#0x2a0		@double, y=160
	str r0,[r2],#8
	b dm9


dm4:	@- - - - - - - - - - - - -8x16

dm12:
	ldr r3,[addy],#4
	ands r0,r3,#0xff
	beq dm13		@skip if sprite Y=0
	cmp r0,#159
	bhi dm13		@skip if sprite Y>159

	add r0,r0,#(-16+SCREEN_Y_START)
	and r0,r0,#0xff

	and r1,r3,#0xff00
	add r1,r1,#(SCREEN_X_START-8)*256	@adjusted x
	orr r0,r0,r1,lsl#8
	and r1,r3,#0x60000000	@flip
	orr r0,r0,r1,lsr#1
	orr r0,r0,#0x8000	@8x16
	str r0,[r2],#4		@store OBJ Atr 0,1

	mov r1,#PRIORITY
	tst r3,#0x80000000	@priority
	orrne r1,r1,#PRIORITY>>1
	cmp r5,#0
	@gbc vram bank
	andne r0,r3,#0x08000000
	orrne r1,r1,r0,lsr#19
	@gbc color
	andne r0,r3,#0x07000000
	orrne r1,r1,r0,lsr#12
	@gb color
	andeq r0,r3,#0x10000000
	orreq r1,r1,r0,lsr#16
	@tile
	and r0,r3,#0x00FF0000
	orr r1,r1,r0,lsr#16
	bic r1,r1,#0x0001
	.if OBJ_BASE == 0x14000	
	add r1,r1,#512
	.endif

	strh r1,[r2],#4		@store OBJ Atr 2
dm14:
	cmp addy,r9
	bne dm12
	bx lr
dm13:
	mov r0,#0x2a0		@double, y=160
	str r0,[r2],#8
	b dm14



@----------------------------------------------------------------------------
vcountinterrupt:
@----------------------------------------------------------------------------

do_gba_hdma:
	mov r12,globalptr
	ldr globalptr,=GLOBAL_PTR_BASE
	ldr r0,=REG_BASE+REG_DM0SAD
	ldr_ r1,bigbufferbase2
@	add r1,r1,#24  ;because first scanline was already displayed
	ldr r2,=REG_BASE+REG_BG0CNT
	ldr r3,=0xA6600006
	stmia r0!,{r1-r3}
	ldr_ r1,dispcntbase2
@	add r1,r1,#2   ;because first scanline was already displayed
	ldr r2,=REG_BASE+REG_DISPCNT
	ldr r3,=0xA2600001
	stmia r0!,{r1-r3}
@	ldr_ r1,dispcntbase2
	add r1,r1,#288
@	add r1,r1,#2   ;because first scanline was already displayed
	ldr r2,=REG_BASE+REG_WIN0H
	ldr r3,=0xA2600001
	stmia r0!,{r1-r3}

	adr r0,end_gba_hdma
	str r0,vcountfptr

	mov r2,#REG_BASE
	ldr r0,=0x28 + 256*(SCREEN_Y_START+144-1) @scanline 144+8-1, enable vblank+vcount interrupts
	strh r0,[r2,#REG_DISPSTAT]

	ldr_ r0,_ui_border_screen
	and r0,r0,#3
	@update WININ and WINOUT registers
	cmp r0,#2
	ldrle r1,=0xFFE8FFFA @one layer
	cmp r0,#3
	ldreq r1,=0xFFECFFFE @two layers
	cmp r0,#0
	ldreq r1,=0xFFE0FFFA @no extra layers
	str r1,[r2,#REG_WININ]



	mov globalptr,r12
	bx lr
	
end_gba_hdma:
	mov r2,#REG_BASE
	strh r2,[r2,#REG_DM0CNT_H]	@DMA stop
	strh r2,[r2,#REG_DM1CNT_H]
	strh r2,[r2,#REG_DM2CNT_H]
@	strh r2,[r2,#REG_DM3CNT_H]

	ldr r0,=vbldummy
	str r0,vcountfptr

	mov r0,#0x0008 @scanline 0, enable vblank interrupt
	strh r0,[r2,#REG_DISPSTAT]

	bx lr
	
	


@@----------------------------------------------------------------------------
@vcountinterrupt; for palette changes
@@----------------------------------------------------------------------------
@	stmfd sp!,{r4-r7}
@	ldr r4,=PALETTE_BASE
@	adr r0,palrptr
@	ldr r1,[r0]
@	ldr r6,[r0,#4]
@	cmp r1,r6
@	beq vci_return
@	
@	ldr r2,=palbuff
@	ldr r12,[r1,r2]
@vci_loop1
@	mov r5,r12,lsr#16
@	;bg color?
@	tst r12,#0x00000600
@	and r3,r12,#0x00007800
@	moveq r7,r3,lsr#10
@	streqh r5,[r4,r7]
@	mov r3,r3,lsr#6
@	and r7,r12,#0x00000600
@	mov r7,r7,lsr#8
@	orr r3,r3,r7
@	add r3,r3,#256
@	strh r5,[r4,r3]
@	add r1,r1,#4
@	bic r1,r1,#0x200
@	str r1,[r0]
@	cmp r1,r6
@	beq vci_return
@	and r3,r12,#0x000000FF
@	ldr r12,[r1,r2]
@	and r5,r12,#0x000000FF
@	cmp r3,r5
@	beq vci_loop1
@	ldr r7,=REG_BASE+REG_DISPSTAT
@	ldrh r0,[r7]
@	bic r0,r0,#0xFF00
@	orr r0,r0,r12,lsl#8
@	strh r0,[r7]	
@vci_return	
@	ldmfd sp!,{r4-r7}
@	bx lr
	



@this code is broken...
@merge_recent_tiles
@	ldrb r0,recenttilesfull
@	movs r0,r0
@	bxne lr
@
@	;This shouldn't happen when VSYNC is on, but it does anyway!  Find out why!
@	
@	;Only used when vsync is off, fixes garbage when there are two tile updates before a GBA vblank.
@	
@	;psuedocode:
@	;if len1*2+len2*2>max_len*2, then just give up and flush buffer1
@	;copy tile numbers and tile data from buffer2 to buffer1, then swap again
@	
@	;find length of buffer2
@	ldr r1,dmarecent_tilenum  ;buffer2
@	sub r1,r1,#2
@strlen1
@	ldrh r0,[r1,#2]!
@	tst r0,#0x8000
@	beq strlen1
@	ldr r0,dmarecent_tilenum  ;buffer2
@	subs r0,r1,r0
@	;if buffer2 is empty, just swap.
@	beq just_unswap
@	
@	;now r0=length of buffer2*2, r1=buffer2+r0
@	
@	;find length of buffer1
@	stmfd sp!,{r2-addy,lr}
@	ldr r3,recent_tilenum
@	sub r4,r3,#2
@strlen2
@	ldrh r6,[r4,#2]!
@	tst r6,#0x8000
@	beq strlen2
@	sub r5,r4,r3
@	add r6,r0,r5
@	cmp r6,#MAX_RECENT_TILES*2
@	bgt merge_giveup
@
@	;r0 = length of buffer2*2
@	;r5 = length of buffer1*2
@	
@	ldr r0,recent_tilenum
@	ldr r1,recent_tiles
@	add r0,r0,r5
@	add r1,r1,r5,lsl#3
@	ldr r2,dmarecent_tilenum
@	ldr r3,dmarecent_tiles
@	;r0 = dest inside buffer2
@	;r1 = dest position inside buffer2
@	;r2 = src inside buffer1
@	;r3 = src position inside buffer1
@	
@	
@	
@	ldr r2,dmarecent_tiles
@	add r0,r2,r0,lsl#3
@	ldr r2,recent_tiles
@	
@	;copy!
@	ldrh r6,[r2]
@	mov r7,#0x8000 ;mark as empty
@	strh r7,[r2],#2
@mergeloop
@	strh r6,[r0],#2
@	ldmia r3!,{r6,r7,r8,r9}
@	stmia r1!,{r6,r7,r8,r9}
@	ldrh r6,[r2],#2
@	subs r5,r5,#2
@	bne mergeloop
@	ldmfd sp!,{r2-addy,lr}
@	b just_unswap
@merge_giveup
@	bl flush_recent_tiles
@	ldmfd sp!,{r2-addy,pc}


	.pushsection .vram1, "ax", %progbits
@----------------------------------------------------------------------------
newframe_vblank:	@called at line 144	(??? safe to use)
@----------------------------------------------------------------------------
	stmfd sp!,{r0-r12,lr}

@	ldrb r0,windowY
@	strb r0,frame_windowy

	@Finish buffers: Part 1  (remember to do part 2)
	bl_long bufferfinish

@	;finish dispcontrol buffer
@	ldr addy,lcdcbuff
@	ldrb r1,lcdctrl
@	ldrb r0,lcdctrlline
@	mov r2,#144
@	bl fill_byte_buff
@	
@	;finish yscroll buffer
@	ldr addy,yscrollbuff
@	ldrb r1,scrollY
@	ldrb r0,yscrollline
@	mov r2,#144
@	bl fill_byte_buff
@	
@	;finish xscroll buffer
@	ldr addy,xscrollbuff
@	ldrb r1,scrollX
@	ldrb r0,xscrollline
@	mov r2,#144
@	bl fill_byte_buff
@
@	;finish windowx buffer
@	ldr addy,wxbuff
@	ldrb r1,windowX
@	ldrb r0,wxline
@	mov r2,#144
@	bl fill_byte_buff
@	
@
@	mov r0,#0
@	str r0,lcdctrlline ;also writes yscrollline, xscrollline


	@disable GBA vblank interrupts while swapping buffers
	mov r0,#REG_BASE
	ldrb r1,[r0,#REG_IE]
	bic r1,r1,#1
	strb r1,[r0,#REG_IE]
	
	@reset the bios's vblank happened flag
	ldrb r1,[r0,#-8]
	bic r1,r1,#1
	strb r1,[r0,#-8]
	
	@---
	@check for SGB MASKING
	@---
	ldrb_ r0,sgb_mask
	movs r0,r0
	bne no_swap

	@rotate out ui_border visibility flags
	ldr_ r0,_ui_border_buffer
	str_ r0,_ui_border_screen
	bl_long update_ui_border_masks

	@copy lcdctrl0's mid-frame status
	ldrb_ r0,lcdctrl
	@screen on?
	tst r0,#0x80
	streqb_ r0,lcdctrl0midframe
	ldrneb_ r0,lcdctrl0midframe	
	strb_ r0,lcdctrl0frame
	
	@swap "big" buffer
	ldr_ r0,bigbufferbase
	ldr_ r1,bigbufferbase2
	str_ r1,bigbufferbase
	str_ r0,bigbufferbase2

	@swap dispcnt+window buffer
	ldr_ r0,dispcntbase
	ldr_ r1,dispcntbase2
	str_ r1,dispcntbase
	str_ r0,dispcntbase2

	@@advance 
	@ldr_ r0,bg_cache_cursor
	@str_ r0,bg_cache_base
	mov r0,#1
	strb_ r0,bg_cache_updateok
	
	

@	;swap xscroll buffer
@	ldr r0,xscrollbuff
@	ldr r1,active_xscrollbuff
@	str r1,xscrollbuff
@	str r0,active_xscrollbuff
@
@	;swap yscroll buffer
@	ldr r0,yscrollbuff
@	ldr r1,active_yscrollbuff
@	str r1,yscrollbuff
@	str r0,active_yscrollbuff
@
@	;swap control buffer
@	ldr r0,lcdcbuff
@	ldr r1,active_lcdcbuff
@	str r1,lcdcbuff
@	str r0,active_lcdcbuff
@	
@	;swap windowx buffer
@	ldr r0,wxbuff
@	ldr r1,active_wxbuff
@	str r1,wxbuff
@	str r0,active_wxbuff

@	;copy window coordinates
@	ldrb r0,frame_windowy
@	strb r0,active_windowy

	@swap gb oam buffer if not dirty
	ldrb_ r0,gboamdirty
	movs r0,r0
	beq gb_oam_is_clean
	cmp r0,#2 @use copy instead of buffer swap?
	beq 0f
	cmp r0,#3 @Late sprite DMA, want to make put the ALT buffer on the screen
	beq 1f
	

	ldr_ r0,gb_oam_buffer_writing
	ldr_ r1,gb_oam_buffer_screen
	str_ r1,gb_oam_buffer_writing
	str_ r0,gb_oam_buffer_screen
	b 2f
1:
	ldr_ r0,gb_oam_buffer_alt
	ldr_ r1,gb_oam_buffer_screen
	str_ r1,gb_oam_buffer_alt
	str_ r0,gb_oam_buffer_screen
	b 2f
0:
	bl_long copyoam
2:
	
	mov r0,#0
	strb_ r0,gboamdirty
gb_oam_is_clean:
@	bl_long copy_gbc_palette
	
	@new code
@	if v=1 then
@		add tiles to buffer
@		B=1
@		if buffer becomes full, D=1
@		do not wait for vblank
@	else
@		D=1
@		wait for vblank
@	v=0
	
	ldrb_ r0,novblankwait_
	cmp r0,#1
	@fast mode overrides vblank system
	ldrneb_ r0,vblank_happened
	movs r0,r0
	beq dirty_ok_and_wait
	
	@dma mode 2 (SHANTAE): store dma tiles as recent tiles
	ldrb_ r0,dmamode
	cmp r0,#2
	bne 0f
	
	bl_long store_dirty_packets
	b 1f
0:
	@don't use recent tiles if screen is off
	@ldrb_ r0,lcdctrl
	@tst r0,#0x80
	tst cycles,#CYC_LCD_ENABLED
	moveq r0,#1
	streqb_ r0,consume_dirty
	beq after_wait
	
	ldrb_ r0,consume_buffer
	cmp r0,#2
	@2: stored earlier in frame, allow it to be consumed now
	@1: something remaining in the buffer
	beq 2f
	movs r0,r0
	blne_long flush_recent_tiles
	
	bl_long store_recent_tiles
2:
	mov r0,#1
	strb_ r0,consume_buffer
1:
	mov r0,#0
	strb_ r0,vblank_happened
	
@reenable_vblank:
@	adr lr,after_wait
@	b_long reenable_gba_vblank
	b after_wait
dirty_ok_and_wait:
	@if buffer contains earlier tiles, allow them to be consumed
	ldrb_ r0,consume_buffer
	cmp r0,#2
	mov r0,#1
	streqb_ r0,consume_buffer
	@do not allow consuming dirty tiles in that case
	
	strneb_ r0,consume_dirty
	@dma mode 2 (SHANTAE): store dma tiles as recent tiles
	ldrb_ r0,dmamode
	cmp r0,#2
	bleq_long store_dirty_packets

reenable_vblank:
no_swap:
	bl_long waitframe2

	mov r0,#0
	strb_ r0,vblank_happened
	
after_wait:

	@Finish buffers: Part 2  (after doing part 1)
	bl_long newframeinit

	@slow motion? Vsync again...
	ldrb_ r0,novblankwait_
	cmp r0,#2
	bne 1f
	bl_long waitframe2
	mov r0,#0
	strb_ r0,vblank_happened
1:
	@ensure that GBA vblank is enabled when this function exits
	bl reenable_gba_vblank
	ldmfd sp!,{r0-r12,pc}

@no_swap:
@	b reenable_vblank

@	
@	;sgb display is masked off...
@	ldrb r0,novblankwait_
@	cmp r0,#1
	.popsection

	.pushsection .text
reenable_gba_vblank:
	@reenable GBA vblank interrupt
	mov r0,#REG_BASE
	ldrb r1,[r0,#REG_IE]
	orr r1,r1,#1
	strb r1,[r0,#REG_IE]
	@reenable GBA interrupt master enable, and ARM interrupt enable flag
	strb r1,[r0,#REG_IME]
	@clear ARM interrupt disable flag
	mrs r0,cpsr
	bic r0,r0,#0x80
	msr cpsr,r0
	bx lr

waitframe2:
	mov r12,lr
	bl reenable_gba_vblank
	mov lr,r12

	@VSYNC!
	mov r0,#0				@wait if needed
	mov r1,#1				@VBL wait
	swi 0x040000			@Turn off CPU until IRQ
	
	@clear the BIOS's vblank happened flag
	mov r1,#REG_BASE
	ldrb r0,[r1,#-8]
	bic r0,r0,#1
	strb r0,[r1,#-8]
	
	bx lr
	
	
	.popsection
	
	
@@----------------------------------------------------------------------------
@newframe:	@called at line 0	(r0-r9 safe to use)
@@----------------------------------------------------------------------------
@	str lr,[sp,#-4]!
	
@	bl OAMfinish
@-----------------------
@	ldr r0,ctrl1old
@	ldr r1,ctrl1line
@	mov addy,#159
@	ldr r3,chrold
@	ldr r5,chrold2
@	bl ctrl1finish
@------------------------
@	ldr r0,scrollXold
@	ldr r1,scrollXline
@	mov addy,#159
@	bl scrollXfinish
@--------------------------
@	ldr r0,scrollYold
@	ldr r1,scrollYline
@	mov addy,#159
@	bl scrollYfinish
@--------------------------
@	ldr r0,windowXold
@	ldr r1,windowXline
@	mov addy,#159
@	bl windowXfinish
@--------------------------
@	ldr r0,windowYold
@	ldr r1,windowYline
@	mov addy,#159
@	bl windowYfinish
@--------------------------
@	mov r0,#0
@	str r0,ctrl1line
@	str r0,scrollXline
@	str r0,scrollYline
@	str r0,windowXline
@	str r0,windowYline
@--------------------------

@	ldrb r2,windowX
@	strb r2,windowXbuf
@
@	ldr r0,scrollbuff
@	ldr r1,dmascrollbuff
@	str r1,scrollbuff
@	str r0,dmascrollbuff
@
@	ldr r0,oambuffer
@	str r0,dmaoambuffer
@
@	ldr pc,[sp],#4

@----------------------------------------------------------------------------
FF40_R:@		LCD Control
@----------------------------------------------------------------------------
	ldrb_ r0,lcdctrl
	mov pc,lr
@----------------------------------------------------------------------------
FF40_W:@		LCD Control
@----------------------------------------------------------------------------
	ldrb_ r1,lcdctrl
	cmp r0,r1
	bxeq lr
FF40W_entry:
	tst r0,#0x80
	orrne cycles,#CYC_LCD_ENABLED
	biceq cycles,#CYC_LCD_ENABLED
	eor r2,r1,r0
	and r2,r2,r0
	tst r2,#0x80		@Is LCD turned on?
	beq 0f
#if EARLY_LINE_0
	ldr r2,=toLineZero
	str_ r2,nexttimeout
	mov r2,#0
	strb_ r2,scanline
#else
	ldr r2,=line145_to_end
	str_ r2,nexttimeout
	mov r2,#152
	strb_ r2,scanline
#endif

	and cycles,cycles,#CYC_MASK
0:	
	stmfd sp!,{r0,lr}
	bl tobuffer
	ldr r0,[sp]
	strb_ r0,lcdctrl
	bl newmode
@	mov r2,r1,lsr#16
@	cmp r2,#0x0300
@	bxeq r1
	ldmfd sp!,{r0,pc}


 .pushsection .iwram.3
@----------------------------------------------------------------------------
FF41_W:@		LCD Status
@----------------------------------------------------------------------------
	ldrb r1,lcdstat
	and r2,r1,#0x07		@Preserve VBlank bit, mode flags, and LCDYC bit
	and r0,r0,#0x78		@only use new interrupt and LCDYC test flags
	orr r2,r0,r2
	eors r0,r2,r1		@anything changed?
	bxeq lr
	strb r2,lcdstat
	strb r2,lcdstat2
	b lcdyc_check

#if 0	
	ands r1,r2,r0       @which bits have changed from 0 to 1?
	ldrb_ r0,lcdyc
	tst r1,#0x28		@turned on HBLANK or MODE 2 interrupt?
	beq lcdyc_check
0:	
	@TODO: real hblank interrupts
	@in hblank time?  No HBLANK or MODE 2 interrupt now.
	and r1,r2,#3
	cmp r1,#0x01
	beq lcdyc_check
	
	ldrb_ r1,gb_if
	orr r1,r1,#0x02
	strb_ r1,gb_if
	
	stmfd sp!,{lr}
	bl lcdyc_check
	bl immediate_check_irq
	ldmfd sp!,{pc}
	
@	mov pc,lr
#endif

FF41_R_vblank_hack:
	sub cycles,cycles,#21*CYCLE
FF41_R_vblank:
lcdstat2:
	mov r0,#1
#if LCD_HACKS
	b FF41_R_look_at_top_3
#else
	bx lr
#endif
FF41_R_hack:
	sub cycles,cycles,#21*CYCLE
@@----------------------------------------------------------------------------
@FF41_R:@		LCD Status
@@----------------------------------------------------------------------------
@lcdstat:
@	mov r0,#1
@FF41_modify1:	cmp cycles,#204*CYCLE
@	bxlt lr				@return if after VRAM access
@	orr r0,r0,#2
@FF41_modify2:	cmp cycles,#376*CYCLE
@@	bxlt lr				;return if after OAM access
@@	orr r0,r0,#3
@	orrge r0,r0,#3
@	bx lr
	
@----------------------------------------------------------------------------
FF41_R:@		LCD Status
@----------------------------------------------------------------------------
lcdstat:
	mov r0,#1
FF41_modify1:
	cmp cycles,#204*CYCLE	@mode 0 HBLANK if 0-203
#if LCD_HACKS
	blt 0f
#else
	bxlt lr
#endif
	orr r0,r0,#3
FF41_modify2:
	cmp cycles,#376*CYCLE	@mode 3 OAM + VRAM if 204-375, otherwise mode 2 OAM if 375-455
	bicge r0,r0,#1
#if !LCD_HACKS
	bx lr
 .popsection
#else  //LCD_HACKS
0:
FF41_R_look_at_top_3:
@	mov r11,r11
	@look at top 3 handlers
	ldr_ r1,FF41_PC
	cmp gb_pc,r1
	ldreq_ pc,FF41_handler
	ldr_ r1,FF41_PC_2
	cmp gb_pc,r1
	ldreq_ pc,FF41_handler_2
	ldr_ r1,FF41_PC_3
	cmp gb_pc,r1
	ldreq_ pc,FF41_handler_3
	ldr_ r1,FF41_PC_4
	cmp gb_pc,r1
	ldreq_ pc,FF41_handler_4
	b_long FF41_more
@	b_long FF41_lookup_last_5
 .popsection
	.pushsection .text
FF41_more:
@	@look at remaining 5 handlers
@	ldr r1,=_FF41_PC_4
	stmfd sp!,{r0,r12}
@	mov r12,#21  @24 handlers total besides top 3
@0:
@	ldr r2,[r1],#8
@	cmp gb_pc,r1
@	beq found_ff41_handler
@	subs r12,r12,#1
@	bne 0b
@	@not found: analyze the code and store a handler

	@are we 'ld a,(FF00 + 41)' F0 41?
	ldrb r0,[gb_pc,#-2]
	ldrb r1,[gb_pc,#-1]
	cmp r0,#0xF0
	cmpeq r1,#0x41
	beq .L_we_are_ld_a
	@are we 'bit 1,(hl)' CB 4E?  
	cmp r0,#0xCB
	cmpeq r1,#0x4E
	beq .L_we_are_bit_1_hl
	
	@are we A6 'and (hl)'
	cmp r1,#0xA6
	beq .L_we_are_and_hl
	
	b ff41_notsupported
	
.L_we_are_ld_a:
	@we are ld a,(FF00+41) ...
	@read first 4 bytes at PC
	ldrb r0,[gb_pc]
	ldrb r1,[gb_pc,#1]
	ldrb r2,[gb_pc,#2]
	ldrb r12,[gb_pc,#3]
	
	@'and 2', 'and 3', etc...
	cmp r0,#0xE6
	beq .L_ld_a_and_x

	@'bit 1,a'
	cmp r0,#0xCB
	cmpeq r1,#0x4F
	beq .L_bit_1_a
	
	@'and c'
	cmp r0,#0xA1
	beq .L_and_c
	
	b ff41_notsupported
	
.L_ld_a_and_x:	
	@ld a,(ff00+41) \ and x
	
	@check for 'jr nz, -5'
	cmp r2,#0x20
	cmpeq r12,#0xFA
	beq .L_andx_jr_nz_neg5
	
	cmp r2,#0x28
	cmpeq r12,#0xFA
	beq .L_andx_jr_z_neg5
	
	@'and 3, cp 01'
	cmp r1,#0x03
	cmpeq r2,#0xFE
	cmpeq r12,#0x01
	beq .L_jr_nz_8
	
	@'and 3, dec a, ret z, jr -8'
	cmp r1,#0x03
	cmpeq r2,#0x3D
	cmpeq r12,#0xC8
	beq .L_dec_a_ret_z_jr_8
	

	b ff41_notsupported

.L_andx_jr_nz_neg5:
	@ld a,(ff00+41) \ and x \ jr nz, back
	@8 * 4 cycles per repetition
	cmp r1,#0x02
	beq add_wantmode01_handler_8
	cmp r1,#0x03
	beq add_wantmode0_handler_8
@	cmp r1,#0x04
@	beq add_wantlyc_handler_8
	
	b ff41_notsupported
.L_andx_jr_z_neg5:
	@ld a,(ff00+41) \ and x \ jr z, back
	@8 * 4 cycles per repetition
	cmp r1,#0x03
	beq add_wantmode123_handler_8
	
	b ff41_notsupported

.L_jr_nz_8:
	@ld a,(ff00+41) \ and 3 \ cp 1 \ jr nz, back
	@8 * 4 cycles per repetition
	@check for 'jr nz, -8'
	ldrb r1,[gb_pc,#4]
	ldrb r2,[gb_pc,#5]
	cmp r1,#0x20
	cmpeq r2,#0xF8
	beq add_wantmode1_handler_8
	
	b ff41_notsupported

.L_dec_a_ret_z_jr_8:
	@ld a,(ff00+41) \ and 3 \ dec a \ ret z \ jr back
	@11 * 4 cycles per repetition
	@check for 'jr -8'
	ldrb r1,[gb_pc,#4]
	ldrb r2,[gb_pc,#5]
	cmp r1,#0x18
	cmpeq r2,#0xF8
	beq add_wantmode1_handler_11
	
	b ff41_notsupported


.L_bit_1_a:
	@ld a,(ff00+41) \ bit 1,a \ jr nz, back
	@8 * 4 cycles per repetition
	@check for 'jr nz, -5'
	cmp r2,#0x20
	cmpeq r12,#0xFA
	beq add_wantmode01_handler_8
	
	ldrb r0,[gb_pc,#4]
	@check for 'ret z, jr -6'
	@10 * 4 cycles per repetition
	cmp r2,#0x4F
	cmpeq r12,#0x18
	cmpeq r0,#0xF9
	beq add_wantmode01_handler_10
	
	b ff41_notsupported

.L_and_c:
	@ld a,(ff00+41) \ and c \ jr nz, back
	@check for 'jr nz, -4'
	@7 * 4 cycles per repetition
	cmp r1,#0x20
	cmpeq r2,#0xFB
	beq add_wantmodeC_handler_7

.L_we_are_bit_1_hl:
	@bit 1,(hl) \ jr nz, back
	@6 * 4 cycles per repetition
	ldrb r0,[gb_pc]
	ldrb r1,[gb_pc,#1]
	cmp r0,#0x20
	cmpeq r1,#0xFC
	beq add_wantmode01_handler_6
	
	b ff41_notsupported
.L_we_are_and_hl:
	b ff41_notsupported

add_wantmode0_handler_8:
	ldr r12,=FF41_want_mode0_8
	b add_handler
	
add_wantmode01_handler_6:
	ldr r12,=FF41_want_mode01_6
	b add_handler

add_wantmode01_handler_8:
	ldr r12,=FF41_want_mode01_8
	b add_handler

add_wantmode01_handler_10:
	ldr r12,=FF41_want_mode01_10
	b add_handler

add_wantmode1_handler_8:
	ldr r12,=FF41_want_mode1_8
	b add_handler
	
add_wantmode1_handler_11:
	ldr r12,=FF41_want_mode1_11
	b add_handler
	
add_wantmodeC_handler_7:
	ldr r12,=FF41_want_modeC_7
	b add_handler

add_wantmode123_handler_8:
	ldr r12,=FF41_want_mode123_8
	b add_handler

@add_wantlyc_handler_8:
@	ldr r12,=FF41_want_lyc_8
@	b add_handler


ff41_notsupported:
@	mov r11,r11
	ldr r12,=FF41_null_handler
add_handler:
	@r12 = address of handler, gb_pc = value of PC associated with this handler
	@search through list of bottom 5 for an empty slot
@	ldr r1,=_FF41_PC_4
@	mov r0,#21
@0:
@	ldr r2,[r1],#8
@	movs r2,r2
@	beq found_empty_ff41_handler
@	subs r0,r0,#1
@	bne 0b
@	@didn't find one... just use the last slot instead.
@found_empty_ff41_handler:
@	mov r2,gb_pc
@	b 1f
@found_ff41_handler:
@	@found: move it to top 3:
@	@do the swap: this one > #1 > #2 > #3 > this one
@	@then jump to handler
@	@r1 = address in handler table + 8
@
@	ldr r12,[r1,#-4]
@	@r2 = PC value, r12 = address of handler
@1:
@	ldr_ r0,FF41_handler_3
@	str r0,[r1,#-8]
@	ldr_ r0,FF41_PC_3
@	str r0,[r1,#-4]
	ldr_ r0,FF41_PC_3
	str_ r0,FF41_PC_4
	ldr_ r0,FF41_handler_3
	str_ r0,FF41_handler_4
	ldr_ r0,FF41_PC_2
	str_ r0,FF41_PC_3
	ldr_ r0,FF41_handler_2
	str_ r0,FF41_handler_3
	ldr_ r0,FF41_PC
	str_ r0,FF41_PC_2
	ldr_ r0,FF41_handler
	str_ r0,FF41_handler_2
	str_ gb_pc,FF41_PC
	str_ r12,FF41_handler
	mov r1,r12
	ldmfd sp!,{r0,r12}
	bx r1



	.popsection

#if LCD_HACKS_ACCURATE

#if LCD_HACKS_ACCURATE_DIV
	
FF41_null_handler:
	bx lr
FF41_want_mode01_6:
	ldr r1,=0x00062AAB
	b FF41_want_mode01
FF41_want_mode01_10:
	ldr r1,=0x000A199A
	b FF41_want_mode01
FF41_want_mode01_8:
	ldr r1,=0x00082000
FF41_want_mode01:
	@are we mode 0 or 1 already?  Nothing to do if we are.
	tst r0,#0x02
	bxeq lr
FF41_hack_setmode0:
	subs r2,cycles,#204*CYCLE
	bxlt lr
0:
	bic r12,r1,#0x00FF0000
	mov r2,r2,asr#(2+CYC_SHIFT)
	mul r12,r2,r12
	mov r12,r12,asr#16
	mov r1,r1,lsr#16
	mul r12,r1,r12
	sub cycles,cycles,r12,lsl#(2+CYC_SHIFT)
	bx lr
FF41_want_mode0_8:
	ldr r1,=0x00082000
FF41_want_mode0:
	@in mode 0 already?  Nothing to do if we are
	ands r2,r0,#3
	bxeq lr
	@if we aren't in mode 1 (vblank), proceed to hblank time
	cmp r2,#1
	bne FF41_hack_setmode0
	@otherwise proceed to next scanline
@	mov r11,r11
	b stealcycles
	@and cycles,cycles,#CYC_MASK
	bx lr
FF41_want_mode1_11:
	ldr r1,=0x000B1746
	b FF41_want_mode1
FF41_want_mode1_8:
	ldr r1,=0x00082000
FF41_want_mode1:
	and r2,r0,#3
	cmp r2,#1
	bne stealcycles
	@andne cycles,cycles,#CYC_MASK
	bx lr

FF41_want_modeC_7:
	ldr r1,=0x00072493
FF41_want_modeC:
	and r2,gb_bc,#0x00FF0000
	@check for 'and 03' or 'and 02'
	cmp r2,#0x030000
	beq FF41_want_mode0
	cmp r2,#0x020000
	beq FF41_want_mode01
	bx lr

FF41_want_mode123_8:
	ldr r1,=0x00082000
FF41_want_mode123:
	@not in mode 0?  We're done here
	ands r2,r0,#3
	@we are in mode 0, proceed to end of scanline
	beq stealcycles
	@andeq cycles,cycles,#CYC_MASK
@	orreq r0,r0,#2
	bx lr

@FF41_want_lyc_8:
@	ldr r1,=0x00082000
@FF41_want_lyc:
@	ands r2,r0,#4
@	beq stealcycles
@	@andeq cycles,cycles,#CYC_MASK
@	bx lr

stealcycles:
	bic r2,r1,#0x00FF0000
	mov r12,cycles,asr#(2+CYC_SHIFT)
	mul r2,r12,r2
	mov r2,r2,asr#16
	mov r1,r1,lsr#16
	mul r2,r1,r2
	sub cycles,cycles,r2,lsl#(2+CYC_SHIFT)
	bx lr
@	subs cycles,cycles,r1,lsl#(2+CYC_SHIFT)
@	bpl stealcycles
@	add cycles,cycles,r1,lsl#(2+CYC_SHIFT)
@	bx lr

#else
FF41_null_handler:
	bx lr
FF41_want_mode01_6:
	mov r1,#6
	b FF41_want_mode01
FF41_want_mode01_10:
	mov r1,#10
	b FF41_want_mode01
FF41_want_mode01_8:
	mov r1,#8
FF41_want_mode01:
	@are we mode 0 or 1 already?  Nothing to do if we are.
	tst r0,#0x02
	bxeq lr
FF41_hack_setmode0:
	subs r2,cycles,#204*CYCLE
	bxlt lr
0:
	subs r2,r2,r1,lsl#(2+CYC_SHIFT)
	bpl 0b
	add cycles,r2,r1,lsl#(2+CYC_SHIFT)
FF41_modify1b:
	add cycles,cycles,#204*CYCLE
@	bic r0,r0,#0x03
	bx lr
FF41_want_mode0_8:
	mov r1,#8
FF41_want_mode0:
	@in mode 0 already?  Nothing to do if we are
	ands r2,r0,#3
	bxeq lr
	@if we aren't in mode 1 (vblank), proceed to hblank time
	cmp r2,#1
	bne FF41_hack_setmode0
	@otherwise proceed to next scanline
@	mov r11,r11
	b stealcycles
	@and cycles,cycles,#CYC_MASK
	bx lr
FF41_want_mode1_11:
	mov r1,#11
	b FF41_want_mode1
FF41_want_mode1_8:
	mov r1,#8
FF41_want_mode1:
	and r2,r0,#3
	cmp r2,#1
	bne stealcycles
	@andne cycles,cycles,#CYC_MASK
	bx lr

FF41_want_modeC_7:
	mov r1,#7
FF41_want_modeC:
	and r2,gb_bc,#0x00FF0000
	@check for 'and 03' or 'and 02'
	cmp r2,#0x030000
	beq FF41_want_mode0
	cmp r2,#0x020000
	beq FF41_want_mode01
	bx lr

FF41_want_mode123_8:
	mov r1,#8
FF41_want_mode123:
	@not in mode 0?  We're done here
	ands r2,r0,#3
	@we are in mode 0, proceed to end of scanline
	beq stealcycles
	@andeq cycles,cycles,#CYC_MASK
@	orreq r0,r0,#2
	bx lr

@FF41_want_lyc_8:
@	mov r1,#8
@FF41_want_lyc:
@	ands r2,r0,#4
@	beq stealcycles
@	@andeq cycles,cycles,#CYC_MASK
@	bx lr

stealcycles:
	subs cycles,cycles,r1,lsl#(2+CYC_SHIFT)
	bpl stealcycles
	add cycles,cycles,r1,lsl#(2+CYC_SHIFT)
	bx lr
#endif

#else  //not LCD_HACKS_ACCURATE

FF41_null_handler:
	bx lr
FF41_want_mode01_6:
FF41_want_mode01_10:
FF41_want_mode01_8:
FF41_want_mode01:
	@are we mode 0 or 1 already?  Nothing to do if we are.
	tst r0,#0x02
	bxeq lr
FF41_hack_setmode0:
	@set cycles to beginning of hblank
	and cycles,cycles,#CYC_MASK
FF41_modify1b:
	add cycles,cycles,#204*CYCLE
	bic r0,r0,#0x03
	bx lr
FF41_want_mode0_8:
FF41_want_mode0:
	@in mode 0 already?  Nothing to do if we are
	ands r2,r0,#3
	bxeq lr
	@if we aren't in mode 1 (vblank), proceed to hblank time
	cmp r2,#1
	bne FF41_hack_setmode0
	@otherwise proceed to next scanline
@	mov r11,r11
	and cycles,cycles,#CYC_MASK
	bx lr
FF41_want_mode1_11:
FF41_want_mode1_8:
FF41_want_mode1:
	and r2,r0,#3
	cmp r2,#1
	andne cycles,cycles,#CYC_MASK
	bx lr

FF41_want_modeC_7:
FF41_want_modeC:
	and r2,gb_bc,#0x00FF0000
	@check for 'and 03' or 'and 02'
	cmp r2,#0x030000
	beq FF41_want_mode0
	cmp r2,#0x020000
	beq FF41_want_mode01
	bx lr

FF41_want_mode123_8:
FF41_want_mode123:
	@not in mode 0?  We're done here
	ands r2,r0,#3
	@we are in mode 0, proceed to end of scanline
	andeq cycles,cycles,#CYC_MASK
@	orreq r0,r0,#2
	bx lr

@FF41_want_lyc_8:
@FF41_want_lyc:
@	ands r2,r0,#4
@	andeq cycles,cycles,#CYC_MASK
@	bx lr
#endif

#endif  //LCD_HACKS
	
 .section .iwram.1, "ax", %progbits




@----------------------------------------------------------------------------
FF42_W:@		SCY - Scroll Y
@----------------------------------------------------------------------------
	ldrb_ r1,scrollY
	cmp r0,r1
	bxeq lr
FF42W_entry:
	stmfd sp!,{r0,lr}
	bl tobuffer
	ldr r0,[sp]
	strb_ r0,scrollY
	ldmfd sp!,{r0,lr}
	b scrollx_entry
@----------------------------------------------------------------------------
FF43_W:@		SCX - Scroll X
@----------------------------------------------------------------------------
	ldrb_ r1,scrollX
	cmp r0,r1
	bxeq lr
FF43W_entry:
	stmfd sp!,{r0,lr}
	bl tobuffer
	ldr r0,[sp]
	strb_ r0,scrollX
	ldmfd sp!,{r0,lr}

scrollx_entry:
	ldrb_ r0,rendermode
	cmp r0,#1
	blt mode0_update_scroll
	bgt mode2_update_scroll
	bx lr

#if 0
@----------------------------------------------------------------------------
FF44_R:@      LCD Scanline
@----------------------------------------------------------------------------
	ands r0,cycles,#CYC_LCD_ENABLED
@	ldrb_ r0,lcdctrl
@	ands r0,r0,#0x80
	ldrneb_ r0,scanline
	@ldr r0,scanline
	mov pc,lr
#endif

@----------------------------------------------------------------------------
FF44_R:@      LCD Scanline
@----------------------------------------------------------------------------
	ands r0,cycles,#CYC_LCD_ENABLED
	ldrneb_ r0,scanline
#if !LCD_HACKS
	bx lr
#else //LCD_HACKS

	@check for recent PC
@	mov r11,r11
	
	ldr_ r1,FF44_PC
	cmp gb_pc,r1
	ldreq_ pc,FF44_handler
	ldr_ r1,FF44_PC_2
	cmp gb_pc,r1
	ldreq_ pc,FF44_handler_2
	b_long FF44_analyze_code

.pushsection .text
FF44_analyze_code:
@	mov r11,r11
	@supports these:
	@ld a,(FF00+44)
	@cp [xx, b, c]
	@jr [c,nz,nc], back
	@
	@cp (hl)
	@jr nz, back
	@
	@check which instruction is executing this
	@ld a,(FF00+44)  F0 44
	ldrb r1,[gb_pc,#-2]
	ldrb r2,[gb_pc,#-1]
	cmp r1,#0xF0
	cmpeq r2,#0x44
	beq ff44_ld_a
	@cp (hl)  BE
	cmp r2,#0xBE
	beq ff44_cp_hl
	b ff44_not_supported
ff44_ld_a:
	ldrb r1,[gb_pc,#0]
	ldrb r2,[gb_pc,#2]
	@cp xx  FE xx
	cmp r1,#0xFE
	beq .L_cp_x
	@cp b   B8
	ldrb r2,[gb_pc,#1]
	cmp r1,#0xB8
	beq .L_cp_b
	@cp c   B9
	cmp r1,#0xB9
	beq .L_cp_c
	@cp c   BA
	cmp r1,#0xBA
	beq .L_cp_d
	@cp c   BB
	cmp r1,#0xBB
	beq .L_cp_e
	b ff44_not_supported
.L_cp_x:
	ldrb r1,[gb_pc,#3]
	@branch value of -6 (FA)
	cmp r1,#0xFA
	bne ff44_not_supported
	ldr r1,=ff44_null_handler
	cmp r2,#0x20  @jr nz
	ldreq r1,=ff44_want_eq_x_handler
	cmp r2,#0x38  @jr c
	ldreq r1,=ff44_want_ge_x_handler
	cmp r2,#0x30  @jr nc
	ldreq r1,=ff44_want_lt_x_handler
	b ff44_install_handler
.L_cp_b:
	ldrb r1,[gb_pc,#2]
	@branch value of -5 (FB)
	cmp r1,#0xFB
	bne ff44_not_supported
	ldr r1,=ff44_null_handler
	cmp r2,#0x20  @jr nz
	ldreq r1,=ff44_want_eq_b_handler
	cmp r2,#0x38  @jr c
	ldreq r1,=ff44_want_ge_b_handler
	b ff44_install_handler
.L_cp_c:
	ldrb r1,[gb_pc,#2]
	@branch value of -5 (FB)
	cmp r1,#0xFB
	bne ff44_not_supported
	ldr r1,=ff44_null_handler
	cmp r2,#0x20  @jr nz
	ldreq r1,=ff44_want_eq_c_handler
	cmp r2,#0x38  @jr c
	ldreq r1,=ff44_want_ge_c_handler
	b ff44_install_handler
.L_cp_d:
	ldrb r1,[gb_pc,#2]
	@branch value of -5 (FB)
	cmp r1,#0xFB
	bne ff44_not_supported
	ldr r1,=ff44_null_handler
	cmp r2,#0x20  @jr nz
	ldreq r1,=ff44_want_eq_d_handler
	cmp r2,#0x38  @jr c
	ldreq r1,=ff44_want_ge_d_handler
	b ff44_install_handler
.L_cp_e:
	ldrb r1,[gb_pc,#2]
	@branch value of -5 (FB)
	cmp r1,#0xFB
	bne ff44_not_supported
	ldr r1,=ff44_null_handler
	cmp r2,#0x20  @jr nz
	ldreq r1,=ff44_want_eq_e_handler
	cmp r2,#0x38  @jr c
	ldreq r1,=ff44_want_ge_e_handler
	b ff44_install_handler
ff44_cp_hl:
	ldrb r1,[gb_pc,#1]
	@branch value of -3 (FD)
	cmp r1,#0xFD
	bne ff44_not_supported
	ldr r1,=ff44_null_handler
	ldrb r2,[gb_pc,#0]
	cmp r2,#0x20  @jr nz
	ldreq r1,=ff44_want_eq_a_handler
	b ff44_install_handler

ff44_not_supported:
	ldr r1,=ff44_null_handler
ff44_install_handler:
	ldr_ r2,FF44_PC
	str_ r2,FF44_PC_2
	ldr_ r2,FF44_handler
	str_ r2,FF44_handler_2
	str_ gb_pc,FF44_PC
	str_ r1,FF44_handler
	bx r1
	
.popsection


ff44_want_ge_x_handler:
	ldrb r1,[gb_pc,#1]
ff44_want_ge:
@	mov r11,r11
	cmp r0,r1
	andlt cycles,cycles,#CYC_MASK
	bx lr

ff44_want_lt_x_handler:
	ldrb r1,[gb_pc,#1]
ff44_want_lt:
@	mov r11,r11
	cmp r0,r1
	andge cycles,cycles,#CYC_MASK
	bx lr

ff44_want_eq_x_handler:
	ldrb r1,[gb_pc,#1]
ff44_want_eq:
@mov r11,r11
	cmp r0,r1
	andne cycles,cycles,#CYC_MASK
	addne r0,r0,#1
	bx lr

ff44_want_eq_a_handler:
	mov r1,gb_a,lsr#24
	b ff44_want_eq

ff44_want_ge_c_handler:
	mov r1,gb_bc,lsr#16
	and r1,r1,#0xFF
	b ff44_want_ge
	
ff44_want_ge_b_handler:
	mov r1,gb_bc,lsr#24
	b ff44_want_ge

ff44_want_ge_e_handler:
	mov r1,gb_de,lsr#16
	and r1,r1,#0xFF
	b ff44_want_ge
	
ff44_want_ge_d_handler:
	mov r1,gb_de,lsr#24
	b ff44_want_ge

ff44_want_eq_c_handler:
	mov r1,gb_bc,lsr#16
	and r1,r1,#0xFF
	b ff44_want_eq
	
ff44_want_eq_b_handler:
	mov r1,gb_bc,lsr#24
	b ff44_want_eq
	
ff44_want_eq_e_handler:
	mov r1,gb_de,lsr#16
	and r1,r1,#0xFF
	b ff44_want_eq
	
ff44_want_eq_d_handler:
	mov r1,gb_de,lsr#24
	b ff44_want_eq
	
ff44_null_handler:
	bx lr
#endif

@----------------------------------------------------------------------------
FF44_R_hack:@		LCD Scanline
@----------------------------------------------------------------------------
	ands r0,cycles,#CYC_LCD_ENABLED
@	ldrb_ r0,lcdctrl
@	ands r0,r0,#0x80
	ldrneb_ r0,scanline
	ldrb r1,[gb_pc]
	cmp r1,#0xFE		@CP instruction
	bne 0f
	ldrb r1,[gb_pc,#1]
	cmp r0,r1
	beq 0f
number_of_times:
	mov r1,#1
	add r1,r1,#1
	strb r1,number_of_times
	cmp r1,#2
	bxne lr
	and cycles,cycles,#CYC_MASK  @LCD HACK
0:
	mov r1,#0
	strb r1,number_of_times
 	bx lr
 
 
   
   
 .pushsection .iwram.3

@----------------------------------------------------------------------------
FF45_R:@		LCD Y Compare
@----------------------------------------------------------------------------
	ldrb_ r0,lcdyc
	mov pc,lr
@----------------------------------------------------------------------------
FF45_W:@		LCD Y Compare
@----------------------------------------------------------------------------
	strb_ r0,lcdyc
lcdyc_check:
	ldrb_ r1,scanline
	cmp r0,r1
	ldrb r0,lcdstat
	orreq r2,r0,#4
	bicne r2,r0,#4
	strb r2,lcdstat
	strb r2,lcdstat2
	bxne lr
	b_long _lcdyc_check_2
	.popsection
	.pushsection .text
	@moved to .text section
_lcdyc_check_2:
	tst r2,#0x40
	bxeq lr
	
	@new code: don't cause multiple interrupts while signal stays high
	tst r0,#0x04
	bxne lr
	
	ldrb_ r0,gb_if
	orr r0,r0,#0x02		@2=LCD STAT
	strb_ r0,gb_if
	
	b_long immediate_check_irq
	.popsection

 .section .iwram.1, "ax", %progbits

@----------------------------------------------------------------------------
FF47_W:@		BGP - BG Palette Data  (GB MODE ONLY)
@----------------------------------------------------------------------------
	ldrb_ r1,bgpalette
	cmp r0,r1
	bxeq lr
	b_long FF47_W_
	.pushsection .text
FF47_W_:
	strb_ r0,bgpalette
	
	ldrb_ r1,gbcmode
	cmp r1,#0
	bxne lr
	ldrb_ r1,sgbmode
	cmp r1,#0
	bne update_sgb_bg_palette
	
	ldr r2,=gbc_palette
	ldr addy,=SGB_PALETTE
	str lr,[sp,#-4]!
	bl_long dopalette
	ldr lr,[sp],#4
	
	ldr r2,=gbc_palette+4*2
	ldr addy,=SGB_PALETTE+16
	b_long dopalette
	.popsection
dopalette:
	and r1,r0,#0x03
	add r1,addy,r1,lsl#1
	ldrh r1,[r1]
	strh r1,[r2]		@store in agb palette
	and r1,r0,#0x0C
	add r1,addy,r1,lsr#1
	ldrh r1,[r1]
	strh r1,[r2,#2]		@store in agb palette
	and r1,r0,#0x30
	add r1,addy,r1,lsr#3
	ldrh r1,[r1]
	strh r1,[r2,#4]		@store in agb palette
	and r1,r0,#0xC0
	add r1,addy,r1,lsr#5
	ldrh r1,[r1]
	strh r1,[r2,#6]		@store in agb palette

	bx lr	

@dopalette_invert
@	and r1,r0,#0x03
@	eor r1,r1,#0x03
@	add r1,addy,r1,lsl#1
@	ldrh r1,[r1]
@	strh r1,[r2]		;store in agb palette
@	and r1,r0,#0x0C
@	eor r1,r1,#0x0C
@	add r1,addy,r1,lsr#1
@	ldrh r1,[r1]
@	strh r1,[r2,#2]		;store in agb palette
@	and r1,r0,#0x30
@	eor r1,r1,#0x30
@	add r1,addy,r1,lsr#3
@	ldrh r1,[r1]
@	strh r1,[r2,#4]		;store in agb palette
@	and r1,r0,#0xC0
@	eor r1,r1,#0xC0
@	add r1,addy,r1,lsr#5
@	ldrh r1,[r1]
@	strh r1,[r2,#6]		;store in agb palette
@
@	bx lr	

@----------------------------------------------------------------------------
FF48_W:@		OBP0 - OBJ 0 Palette Data
@----------------------------------------------------------------------------
	ldrb_ r1,ob0palette
	cmp r1,r0
	bxeq lr
	b_long FF48_W_
	.pushsection .text
FF48_W_:
	strb_ r0,ob0palette
	
	ldrb_ r1,gbcmode
	cmp r1,#0
	bxne lr
	ldrb_ r1,sgbmode
	cmp r1,#0
	bne update_sgb_obj0_palette

	ldr r2,=gbc_palette+64
	ldr addy,=SGB_PALETTE+16
	b_long dopalette
	.popsection
@----------------------------------------------------------------------------
FF49_W:@		OBP1 - OBJ 1 Palette Data
@----------------------------------------------------------------------------
	ldrb_ r1,ob1palette
	cmp r1,r0
	bxeq lr
	b_long FF49_W_
	.pushsection .text
FF49_W_:
	strb_ r0,ob1palette

	ldrb_ r1,gbcmode
	cmp r1,#0
	bxne lr
	ldrb_ r1,sgbmode
	cmp r1,#0
	bne update_sgb_obj1_palette

	ldr r2,=gbc_palette+64+8
	ldr addy,=SGB_PALETTE+24
	b_long dopalette
	.popsection
@----------------------------------------------------------------------------
FF4A_W:@		WINY - Window Y
@----------------------------------------------------------------------------
	ldrb_ r1,windowY
	cmp r0,r1
	bxeq lr
FF4A_W_:
	stmfd sp!,{r0,lr}
	bl tobuffer
	ldr r0,[sp]
	strb_ r0,windowY
	bl newmode
	ldmfd sp!,{r0,pc}

@----------------------------------------------------------------------------
FF4F_W:@		VBK - VRAM Bank - CGB Mode Only
@----------------------------------------------------------------------------
	ldrb_ r1,gbcmode
	cmp r1,#0
	moveq r0,#0
FF4F_W_:
	ands r0,r0,#1
	strb_ r0,vrambank
	
	
	
 .if RESIZABLE
 	ldr_ addy,xgb_vram
 	sub addy,addy,#0x8000
 .else
	ldr addy,=XGB_VRAM-0x8000
 .endif
	addne addy,addy,r0,lsl#13	
	str_ addy,memmap_tbl+32	@8000
	str_ addy,memmap_tbl+36	@9000
	
@	mov addy,#AGB_VRAM
@	addne addy,addy,r0,lsl#13
@@	sub addy,addy,#0x2000
@	str addy,agb_vrambank
	ldr addy,=DIRTY_TILE_BITS
	addne addy,addy,#24
	str_ addy,dirty_tile_bits
	
	mov r0,#0xFF
	strb r0,VRAM_chr_lastAddr
	
	mov pc,lr
@----------------------------------------------------------------------------
FF4F_R:@		VBK - VRAM Bank - CGB Mode Only
@----------------------------------------------------------------------------
	ldrb_ r0,vrambank
	mov pc,lr

@----------------------------------------------------------------------------
FF4B_W:@		WINX - Window X
@----------------------------------------------------------------------------
	ldrb_ r1,windowX
	cmp r0,r1
	bxeq lr
FF4B_W_:
	stmfd sp!,{r0,lr}
	bl tobuffer
	ldr r0,[sp]
	strb_ r0,windowX
	bl newmode
	ldmfd sp!,{r0,pc}

@----------------------------------------------------------------------------
vram_W_8:@  8000-8FFF
@----------------------------------------------------------------------------
	ldr_ r2,memmap_tbl+32
	strb r0,[r2,addy]
@----------------------------------------------------------------------------
VRAM_chr:@	8000-97FF
@----------------------------------------------------------------------------
	@r1 = tile number / 2 (00-BF)
	mov r1,addy,lsr#5
	and r1,r1,#0xFF
VRAM_chr_lastAddr:
	teq r1,#0xFF
	bxeq lr
	strb r1,VRAM_chr_lastAddr
	ldr_ addy,dirty_tile_bits
	ldrb r0,[addy,r1,lsr#3]!
	and r1,r1,#0x07
	mov r2,#1
	orr r0,r0,r2,lsl r1
	strb r0,[addy]
	bx lr

@----------------------------------------------------------------------------
vram_W_9:@   9000-9FFF
@----------------------------------------------------------------------------
	ldr_ r2,memmap_tbl+32
	strb r0,[r2,addy]
	subs r1,addy,#0x9800
	bmi VRAM_chr
@----------------------------------------------------------------------------
VRAM_name0:	@(9800-9FFF)
@----------------------------------------------------------------------------
	ldr r2,=dirty_map_words
	@get byte number for map word 
	@address = (tile number / 4) / 8
	ldrb r0,[r2,r1,lsr#5]!
	@r1 = tile number, want to get bit within word
	@bitmask = (r1 >> 2) & 7
	mov r1,r1,lsr#2
	and r1,r1,#7
	mov r12,#1
	orr r0,r0,r12,lsl r1
	strb r0,[r2]
	bx lr

@----------------------------------------------------------------------------
vram_W2:
@----------------------------------------------------------------------------
	@may enter with (addy<<16) in 0000 or 8000-9FFF
	@most likely not writing to nametables
	movs addy,addy,lsr#16
	bxeq lr @null pointer test
	subs r1,addy,#0x9800
	bmi VRAM_chr
	cmp r1,#0x2000
	bmi VRAM_name0
	bx lr
@@----------------------------------------------------------------------------
@VRAM_nameD:	@(9800-9FFF)    Bloody Hack for Push16.
@@----------------------------------------------------------------------------
@	stmfd sp!,{addy,lr}
@	bl VRAM_name0
@	ldmfd sp!,{addy,lr}
@	add addy,addy,#1
@	tst addy,#0x2000 @For games which stack crash, prevent the VRAM code from seeing the write to A000
@	beq VRAM_name0
@	bx lr
@	
@	ldr r2,memmap_tbl+32
@	ldrb r1,vrambank
@	tst r1,#1
@	movne pc,lr  ;hack for GBC mode...
@	
@	
@	sub addy,addy,#1
@	ldrb r0,[r2,addy]!	;read 1st char
@	ldrb r1,[r2,#1]		;read 2nd char
@
@	ldr r2,agb_nt0
@	bic addy,addy,#0xf800	;AND $07ff
@	add addy,addy,addy	;lsl#1
@	tst addy,#0x0800	;for WIN color.
@	addne addy,addy,#0x1800
@	orr r0,r0,#0x300
@	orr r1,r1,#0x300
@	strh r0,[r2,addy]!	;write tile#
@	strh r1,[r2,#2]		;write tile#
@
@	bic r0,r0,#0x100	;for BG color.
@	bic r1,r1,#0x100	;for BG color.
@	add r2,r2,#0x1800
@	strh r0,[r2]		;write tile#
@	strh r1,[r2,#2]		;write tile#
@	mov pc,lr

@----------------------------------------------------------------------------

FF69_W:	@BCPD - BG Color Palette Data
	ldrb_ r1,BCPS_index
	and r2,r1,#0x3F
	ldr addy,=gbc_palette
	strb r0,[addy,r2]
	tst r1,#0x80
	addne r1,r1,#1
    @Minucce's tweaks
    andne r1, r1, #0x3F
    orrne r1, r1, #0x80
    
	strneb_ r1,BCPS_index
	bx lr
@FF69_W	;BCPD - BG Color Palette Data
@	ldrb r1,BCPS_index
@	and r2,r1,#0x3F
@	ldr addy,=gbc_palette
@	add addy,addy,r2
@	ldrb r2,[addy]
@	strb r0,[addy]
@	tst r1,#0x80
@	addne r1,r1,#1
@	strneb r1,BCPS_index
@	and r1,r1,#0x3E
@in_ff69
@	cmp r2,r0
@	bxeq lr
@store_palette
@	ldr addy,=gbc_palette
@	ldr r0,scanline
@	add r0,r0,#8
@	orr r0,r0,r1,lsl#8
@	ldrh r2,[addy,r1]
@	orr r0,r0,r2,lsl#16
@	ldr addy,=palrptr
@	ldr r1,[addy],#4
@	ldr r2,[addy]
@	cmp r1,r2
@	ldr r1,=palbuff
@	str r0,[r1,r2]
@	add r2,r2,#4
@	bic r2,r2,#0x200
@	str r2,[addy]
@	bxne lr
@	ldr r1,=REG_BASE+REG_DISPSTAT
@	ldrh r2,[r1]
@	bic r2,r2,#0xFF00
@	and r0,r0,#0xFF
@	orr r2,r2,r0,lsl#8
@	strh r2,[r1]
@	bx lr
FF6B_W:	@OCPD - OBJ Color Palette Data
	ldrb_ r1,OCPS_index
	and r2,r1,#0x3F
	ldr addy,=gbc_palette+64
	strb r0,[addy,r2]
	tst r1,#0x80
	addne r1,r1,#1
    @ Minucce's tweaks
    andne r1, r1, #0x3F
    orrne r1,r1,#0x80
    
	strneb_ r1,OCPS_index
	bx lr
@FF6B_W	;OCPD - OBJ Color Palette Data
@	ldrb r1,OCPS_index
@	and r2,r1,#0x3F
@	ldr addy,=gbc_palette+64
@	add addy,addy,r2
@	ldrb r2,[addy]
@	strb r0,[addy]
@	tst r1,#0x80
@	addne r1,r1,#1
@	strneb r1,OCPS_index
@	and r1,r1,#0x3E
@	add r1,r1,#0x40
@	b in_ff69
@@	adr addy,agb_pal+256
@@	mov r1,r2,lsr#3
@@	mov r1,r1,lsl#5
@@	and r2,r2,#0x07
@@	add r2,r2,r1
@@	strb r0,[addy,r2]
@@	bx lr

copyoam:
	stmfd sp!,{r0,r1,r2,addy,lr}
	ldr_ r1,gb_oam_buffer_writing
	ldr_ r0,gb_oam_buffer_screen
	mov r2,#160
	bl memcpy32
	ldmfd sp!,{r0,r1,r2,addy,pc}

OAM_R:
	cmp addy,#0xFE00
	bmi echo_R
	ldr_ r1,gb_oam_buffer_writing
	ldrb r0,[r1,r2]
	mov pc,lr
OAM_W:
	cmp addy,#0xFE00
	bmi echo_W
	mov r1,#2
	strb_ r1,gboamdirty
	ldr_ r1,gb_oam_buffer_writing
	@do not write out of bounds
	cmp r2,#0xA0
	strltb r0,[r1,r2]
	mov pc,lr


@----------------------------------------------------------------------------
@no, you're not going in a READ_ONLY area!
fpstext: .ascii "FPS:    "
fpsenabled: .byte 0
fpschk:	.byte 0
	.byte 0
		.byte 0

@----------------------------------------------------------------------------

@gbc_palette:	.skip 128	@CGB $FF68-$FF6D???
@gbc_palette2:	.skip 128

@----------------------------------------------------------------------------

 .align
 .pool
 .text
 .align
 .pool
@----------------------------------------------------------------------------
FF42_R:@		SCY - Scroll Y
@----------------------------------------------------------------------------
	ldrb_ r0,scrollY
	mov pc,lr
@----------------------------------------------------------------------------
FF43_R:@		SCX - Scroll X
@----------------------------------------------------------------------------
	ldrb_ r0,scrollX
	mov pc,lr
@----------------------------------------------------------------------------
FF44_W:@		LCD Scanline
@----------------------------------------------------------------------------
	mov pc,lr
@----------------------------------------------------------------------------
FF47_R:@		BGP - BG Palette Data
@----------------------------------------------------------------------------
	ldrb_ r0,bgpalette
	mov pc,lr
@----------------------------------------------------------------------------
FF48_R:@		OBP0 - OBJ 0 Palette Data
@----------------------------------------------------------------------------
	ldrb_ r0,ob0palette
	mov pc,lr
@----------------------------------------------------------------------------
FF49_R:@		OBP1 - OBJ 1 Palette Data
@----------------------------------------------------------------------------
	ldrb_ r0,ob1palette
	mov pc,lr
@----------------------------------------------------------------------------
FF4A_R:@		WINY - Window Y
@----------------------------------------------------------------------------
	ldrb_ r0,windowY
	mov pc,lr
@----------------------------------------------------------------------------
FF4B_R:@		WINX - Window X
@----------------------------------------------------------------------------
	ldrb_ r0,windowX
	mov pc,lr
FF68_R:	@BCPS - BG Color Palette Specification
	ldrb_ r0,BCPS_index
	bx lr
FF69_R:	@BCPD - BG Color Palette Data
	ldrb_ r1,BCPS_index
	and r1,r1,#0x3F
	ldr r0,=gbc_palette
	ldrb r0,[r0,r1]
	bx lr
FF6A_R:	@OCPS - OBJ Color Palette Specification
	ldrb_ r0,OCPS_index
	bx lr
FF6B_R:	@OCPD - OBJ Color Palette Data
	ldrb_ r1,OCPS_index
	and r1,r1,#0x3F
	ldr r0,=gbc_palette+64
	ldrb r0,[r0,r1]
	bx lr
FF68_W:	@BCPS - BG Color Palette Specification
	strb_ r0,BCPS_index
	bx lr
FF6A_W:	@OCPS - OBJ Color Palette Specification
	strb_ r0,OCPS_index
	bx lr

update_lcdhack:
	stmfd sp!,{r3,r4,r10,lr}
	ldr r10,=GLOBAL_PTR_BASE
	
	ldrb_ r0,lcdhack
	movs r0,r0
	adrne r1,hack_cycle_subtractions-4
	ldrne r4,[r1,r0,lsl#2]
	movne r0,#1
	
	
	adr r3,lcdhacks
	ldr r2,[r3,r0,lsl#2]!
	ldr r1,=FF44_R_ptr
	str r2,[r1]
	
	adrl_ r1,FF41_R_function
	ldr r2,[r3,#8]!
	str r2,[r1]
	strne r4,[r2]
	ldr r2,[r3,#8]!
	str r2,[r1,#4]
	strne r4,[r2]
	
	ldmfd sp!,{r3,r4,r10,lr}
	bx lr
hack_cycle_subtractions:
	sub cycles,cycles,#0*CYCLE
	sub cycles,cycles,#32*CYCLE
	sub cycles,cycles,#240*CYCLE
lcdhacks:
	.word FF44_R,FF44_R_hack
FF41_R_functions:
	.word FF41_R,FF41_R_hack
	.word FF41_R_vblank,FF41_R_vblank_hack




 .section .iwram.end.101, "ax", %progbits

FPSValue:
	.word 0
AGBinput:		@this label here for main.c to use
	.word 0 @AGBjoypad (why is this in lcd.s again?  um.. i forgot)
EMUinput:	.word 0 @XGBjoypad (this is what GB sees)

lcdstate:
	.byte 0 @lcdctrl  ff40
_lcdstat_save:
	.byte 0 @lcdstat_save
_scrollX:
	.byte 0 @scrollX
_scrollY:
	.byte 0 @scrollY
	
_scanline:
g_scanline:	.byte 0 @scanline
_lcdyc:
	.byte 0 @lcdyc
_dma_blocks_total:	.byte 0 @[unused] dma start address
_bgpalette:
	.byte 0 @bgpalette
_ob0palette:
	.byte 0 @ob0palette
_ob1palette:
	.byte 0 @ob1palette
_windowX:
	.byte 0 @windowX
_windowY:
	.byte 0 @windowY
_BCPS_index:	
	.byte 0 @BCPS_index  ;actually ff68
_doublespeed:
	.byte 0 @doublespeed
_OCPS_index:
	.byte 0 @OCPS_index  ;actually ff6a
_vrambank:
	.byte 0 @vrambank

_dma_src:	.word 0 @dma_src, dma_dest
_dma_dest = _dma_src + 2
@@end of lcdstate

_dirty_tile_bits:	.word DIRTY_TILE_BITS
_gb_oam_buffer_alt:	.word GBOAMBUFF3		@secondary OAM buffer for games which writre
@_dirty_rows:	.word DIRTY_ROWS		@dirty_rows

@_dirty_tiles:	.word DIRTY_TILES		@dirty_tiles
@_dirty_rows:	.word DIRTY_ROWS		@dirty_rows

_bigbuffer:
	.word 0 @bigbuffer
_bg01cnt:
	.word 0 @bg01cnt
_bg23cnt:
	.word 0 @bg23cnt
_xyscroll:
	.word 0 @xyscroll
_xyscroll2:
	.word 0 @xyscroll2
_dispcntdata:
	.word 0 @dispcntdata
_windata:
	.word 0 @windata
_dispcntaddr:
	.word 0 @dispcntaddr
_windowyscroll:
	.word 0 @windowyscroll
_buffer_lastscanline:
	.word 0 @buffer_lastscanline
_lcdctrl0midframe:
	.byte 0 @lcdctrl0midframe
_lcdctrl0frame:
lcdctrl0frame_:
	.byte 0 @lcdctrl0frame
_rendermode:
	.byte 0 @rendermode
ui_border_visible:	.byte 0 @_ui_border_visible

sgb_palette_number: .byte 0 @_sgb_palette_number
gammavalue:	.byte 0 @_gammavalue
darkness:	.byte 0 @_darkness
_dma_blocks_remaining:	.byte 0


_ui_border_cnt_bic:
	.word 0 @ui_border_cnt_bic
_ui_border_cnt_orr:
	.word 0 @ui_border_cnt_orr
_ui_border_scroll2:
	.word 0 @ui_border_scroll2
_ui_border_scroll3:
	.word 0 @ui_border_scroll3
ui_x:	.word 0 @_ui_x
ui_y_real:	.word 0 @_ui_y
ui_border_request:	.word 0 @_ui_border_request # 4
ui_border_screen:	.word 0 @_ui_border_screen # 4
ui_border_buffer:	.word 0 @_ui_border_buffer # 4

_dispcntbase:
	.word DISPCNTBUFF		@dispcntbase
_dispcntbase2:
	.word DISPCNTBUFF2		@dispcntbase2
_bigbufferbase:
	.word BG0CNT_SCROLL_BUFF	@bigbufferbase
_bigbufferbase2:
	.word BG0CNT_SCROLL_BUFF2	@bigbufferbase2

_gboamdirty:
	.byte 0				@gboamdirty
_consume_dirty:
	.byte 0				@consume_dirty
_consume_buffer:
	.byte 0				@consume_buffer
_vblank_happened:
	.byte 0				@vblank_happened

_gb_oam_buffer_screen:	.word GBOAMBUFF1		@finished OAM buffer for the screen
_gb_oam_buffer_writing:	.word GBOAMBUFF2		@OAM buffer we are writing to

palettebank:	.word 1			@_palettebank
	.word 0 @unused, was bg_cache_cursor
	.word 0 @unused, was bg_cache_base
	.word 0 @unused, was bg_cache_limit

	.byte 0 @unused, was bg_cache_full
_bg_cache_updateok:
	.byte 0 @bg_cache_updateok
_lcdhack:
g_lcdhack:	.byte 0	@lcdhack
_dmamode:	.byte 0	@dmamode

 .section .iwram.end.106, "ax", %progbits
	.word FF41_R 	@FF41_R_function
	.word FF41_R_vblank @FF41_R_vblank_function
_vram_packet_dest:	
	.word 0			@vram_packet_dest
_vram_packet_source:
	.word 0			@vram_packet_src

_FF41_PC: 			.word 0
_FF41_handler:		.word 0
_FF41_PC_2:			.word 0
_FF41_handler_2:	.word 0
_FF41_PC_3:			.word 0
_FF41_handler_3:	.word 0
_FF41_PC_4:			.word 0
_FF41_handler_4:	.word 0

_FF44_PC:
	.word 0
_FF44_handler:
	.word 0
_FF44_PC_2:
	.word 0
_FF44_handler_2:
	.word 0



@...update load/savestate if you move things around in here
@----------------------------------------------------------------------------
	@.end
