@	#include "equates.h"
@	#include "mappers.h"
@	#include "memory.h"
@	#include "gbz80mac.h"
@	#include "gbz80.h"
@	#include "lcd.h"
@	#include "io.h"

	@IMPORT findrom2 @from main.c
	@IMPORT get_saved_sram
	@IMPORT make_instant_pages
	@IMPORT init_cache
	
	.if MOVIEPLAYER
	@IMPORT update_cache
	.endif

	global_func loadcart
	global_func loadcart_after_sgb_border
@	EXPORT mapBIOS_
	global_func map0123_
	global_func map4567_
@	EXPORT map01234567_
	global_func mapAB_
 .if SAVESTATES
	global_func savestate
	global_func loadstate
 .endif
	.global g_emuflags
	.global g_sramsize
	.global g_rammask
	.if RESIZABLE
	.else
	.global XGB_SRAM
	.endif
	.global romstart
	.global romnum
	.global g_cartflags
	.global g_banks
	.global MULTIBOOT_LIMIT
	.global END_OF_EXRAM
	
	.global INSTANT_PAGES
	.if MOVIEPLAYER
	.global SramName
	.global fatBuffer
	.global fatWriteBuffer
	.global globalBuffer
	.global openFiles
	.global lfnName
	.endif
	
	.global ewram_canary_1
	.global ewram_canary_2
	
@----------------------------------------------------------------------------
 .align
 .pool
 .text
 .align
 .pool
@----------------------------------------------------------------------------

mappertbl:
	.word 0x00,mbc0init
	.word 0x01,mbc1init
	.word 0x02,mbc1init
	.word 0x03,mbc1init
	.word 0x05,mbc2init
	.word 0x06,mbc2init
	.word 0x08,mbc0init
	.word 0x09,mbc0init
	.word 0x0B,mmm01init
	.word 0x0C,mmm01init
	.word 0x0D,mmm01init
	.word 0x0F,mbc3init
	.word 0x10,mbc3init
	.word 0x11,mbc3init
	.word 0x12,mbc3init
	.word 0x13,mbc3init
	.word 0x15,mbc4init
	.word 0x16,mbc4init
	.word 0x17,mbc4init
	.word 0x19,mbc5init
	.word 0x1A,mbc5init
	.word 0x1B,mbc5init
	.word 0x1C,mbc5init
	.word 0x1D,mbc5init
	.word 0x1E,mbc5init
	.word 0x22,mbc7init

@	DCD 0xFC,camerainit
@	DCD 0xFD,tama5init
	.word 0xFE,huc3init
	.word 0xFF,huc1init
@	DCD 0x00,mbc6init
	.word -1,mbc0init
mbcflagstbl:
	.byte 0
	.byte 0
	.byte MBC_RAM
	.byte MBC_RAM|MBC_SAV
	.byte 0
	.byte MBC_RAM
	.byte MBC_RAM|MBC_SAV
	.byte 0
	.byte MBC_RAM			@0x08
	.byte MBC_RAM|MBC_SAV
	.byte 0
	.byte 0
	.byte MBC_RAM
	.byte MBC_RAM|MBC_SAV
	.byte 0
	.byte MBC_TIM			@MBC_SAV
	.byte MBC_TIM+MBC_RAM+MBC_SAV	@0x10
	.byte 0
	.byte MBC_RAM
	.byte MBC_RAM|MBC_SAV
	.byte 0
	.byte 0
	.byte MBC_RAM
	.byte MBC_RAM|MBC_SAV
	.byte 0				@0x18
	.byte 0
	.byte MBC_RAM
	.byte MBC_RAM|MBC_SAV
	.byte MBC_RUM
	.byte MBC_RUM|MBC_RAM
	.byte MBC_RUM+MBC_RAM+MBC_SAV
	.byte 0
	.byte 0				@0x20
	.byte 0
	.byte MBC_RAM|MBC_SAV
	.byte 0


loadcart_after_sgb_border:
	ldr_ r1,emuflags
	ldr_ r0,romnumber
	stmfd sp!,{r0-r1,r4-r11,lr}
	ldr globalptr,=GLOBAL_PTR_BASE	@need ptr regs init'd
	mov r1,#2
	b 0f

@----------------------------------------------------------------------------
loadcart: @called from C:  r0=rom number, r1=emuflags
@----------------------------------------------------------------------------
	stmfd sp!,{r0-r1,r4-r11,lr}
	ldr globalptr,=GLOBAL_PTR_BASE	@need ptr regs init'd
	mov r1,#0
0:
	strb_ r1,autoborderstate

	@ldr r1,=findrom2
	@bl thumbcall_r1
	@ldr r1,=make_instant_pages
	@bl thumbcall_r1
	
	blx_long findrom2
	blx_long make_instant_pages
	
	mov r3,r0		@r0 now points to rom image
	str_ r3,rombase		@set rom base
				@r3=rombase til end of loadcart so DON'T FUCK IT UP

@	mov r1,#0
@	ldr r0,=AGB_VRAM+0x4000	;clear AGB Tiles
@	mov r2,#0x8000
@	bl memset32_

@	ldr r1,=0x01000100
@	ldr r0,=AGB_BG1		;clear AGB BG (GB Window sides)
@	mov r2,#0x2000
@	bl memset32_

	ldmfd sp!,{r0-r1}
	str_ r0,romnumber
        str_ r1,emuflags
        
	@check for SGB support, r5 = supports SGB
	ldrb r1,[r3,#0x146]
	cmp r1,#3
	ldreqb r1,[r3,#0x14B]
	cmpeq r1,#0x33
	moveq r5,#1
	movne r5,#0
	
	@check for GBC support, r4 = supports GBC
        ldrb r1,[r3,#0x143]
        cmp r1,#0x80
        cmpne r1,#0xC0
        moveq r4,#1
        movne r4,#0

	@check what GB type we want:
	@0 = GB
	@1 = perfer SGB over GBC
	@2 = perfer GBC over SGB
	@3 = Weird GBC+SGB
	@4 = perfer GBC over GB
	
	@---
	@if supports SGB+CGB, autoborder==1, and autoborderstate==0,
	@then we're doing a "dry run" where it loads the border then resets
	@also doesn't load SRAM in that case
	cmp r5,#1
	cmpeq r4,#1
	bne 0f
	ldrb_ r0,autoborder
	cmp r0,#1
	bne 0f
	ldrb_ r0,autoborderstate
	movs r0,r0
	bne 0f
	@eq if autoborder==1 and autoborderstate==0
	ldrb_ r0,request_gb_type_
	cmp r0,#2
	bne 0f
	
	mov r0,#1
	strb_ r0,autoborderstate
	mov r4,#0
	b 1f
0:	
	@---
	ldrb_ r0,request_gb_type_
        
        cmp r0,#0
        moveq r4,#0
        moveq r5,#0
        
        cmp r0,#1
        cmpeq r5,#1
        moveq r4,#0 @disable GBC if SGB supported
        
        cmp r0,#2
        cmpeq r4,#1
        moveq r5,#0 @disable SGB if GBC supported

	cmp r0,#4
        moveq r5,#0 @disable SGB
1:
        strb_ r4,gbcmode
        strb_ r5,sgbmode
       	
 .if RESIZABLE
 	cmp r1,#0x00
	moveq r0,#0x2000
	movne r0,#0x4000
	str_ r0,xgb_vramsize
 .endif

	mov r2,#0x8000
	ldrb r1,[r3,#0x148]	@get size in 32kByte chunks.
	mov r1,r2,lsl r1
	sub r1,r1,#1
	str_ r1,rommask		@rommask=romsize-1
	
	ldrb r0,[r3,#0x147]
	cmp r0,#5 @mbc2 has that funky 512 nibbles of ram
	cmpne r0,#6
	moveq r0,#5 @invalid value used just for mbc2
	
	ldrneb r0,[r3,#0x149]	@get ram size.
	strneb_ r0,sramsize
	
	adr r1,rammasktbl
	ldr r0,[r1,r0,lsl#2]
	str_ r0,rammask
	
 .if RESIZABLE
	@build the dynamic memory!
	cmp r0,#0
	addne r0,r0,#1
	str_ r0,xgb_sramsize
	ldr r1,=END_OF_EXRAM
	sub r0,r1,r0
	str_ r0,xgb_sram
	ldr_ r1,xgb_vramsize
	sub r0,r0,r1
	str_ r0,xgb_vram

	add r2,r0,#0x1800
	str_ r2,xgb_vram_1800
	add r2,r0,#0x1C00
	str_ r2,xgb_vram_1C00
	
	ldrb_ r2,sgbmode
	movs r2,r2
	moveq r2,#0
	movne r2,r0

	@if SGB mode, these grow down
	@otherwise, set 0 for these
	subne r2,r2,#4096
	str_ r2,sgb_pals
	subne r2,r2,#4096
	str_ r2,sgb_atfs
	subne r2,r2,#112
	str_ r2,sgb_packet
	subne r2,r2,#360
	str_ r2,sgb_attributes
	movne r0,r2

	str_ r0,end_of_exram
		
	mov r0,#0
	str_ r0,gbc_exram
	str_ r0,gbc_exramsize
 .endif

	stmfd sp!,{r0-addy,lr}
	@ldr r1,=init_cache
	@bl thumbcall_r1
	blx_long init_cache
	ldmfd sp!,{r0-addy,lr}
	
	mov r0,#0
	str_ r0,bank0
	mov r0,#1
	str_ r0,bank1
	
	mov r0,#0		@default ROM mapping
	bl_long map0123_		@0123=1st 16k
	mov r0,#1
	bl_long map4567_		@4567=2nd 16k

@	bl FixRealBios
@	bl mapBIOS_		;01=BIOS

	ldrb r0,[r3,#0x147]	@get mbc#
	cmp r0,#0xFF		@HuC-1
	moveq r0,#3
	adr r1,mbcflagstbl
	ldrb r0,[r1,r0]		@get mbc flags.
	
	@autoborderstate == 1 overrides the flags, we want NO SRAM while we're getting the SGB border
	@-----
	ldrb_ r1,autoborderstate
	cmp r1,#1
	biceq r0,r0,#MBC_SAV
	@-----

	strb_ r0,cartflags	@set cartflags
	ldr r1,=empty_W
	tst r0,#MBC_RAM
	ldrne r1,=sram_W
	tst r0,#MBC_SAV
	beq dont_use_true_sram
	tst r3,#0x08000000
	beq dont_use_true_sram
	ldrb_ r0,sramsize
	cmp r0,#3
	ldrne r1,=sram_W2
dont_use_true_sram:
	str_ r1,sramwptr
	
	ldr r0,=default_scanlinehook
	str_ r0,scanlinehook	@no mapper irq

	mov r1,#0xe0		@was 0xe0
	mov r0,#AGB_OAM
	mov r2,#0x400
	bl memset32_		@no stray sprites please

	mov r1,#0		@clear gb ram+hram
	ldr r0,=XGB_RAM
	mov r2,#0x2080
	bl memset32_
 .if RESIZABLE
	mov r1,#0

	ldr_ r0,xgb_vram
	ldr_ r2,xgb_vramsize
	blne memset32_		@clear GB VRAM

	ldr_ r0,xgb_sram		@clear gb sram, it will be loaded later anyway
	ldr_ r2,xgb_sramsize
	blne memset32_
 .else
	ldr r0,=XGB_SRAM	@clear gb sram, it will be loaded later anyway
	mov r2,#0x8000
	bl memset32_
 .endif

	ldr r0,=mapperstate	@clear mapperdata so we don't have to do that in every MapperInit.
	mov r2,#32
	bl memset32_

	ldr r0,=joy0_W
	ldr r1,=joypad_write_ptr
	str r0,[r1]		@reset FF00 write (SGB messes with it)

@	ldr r1,=sram_W2		;could be used for RTC?
@	ldr r1,=empty_W		;could be used for RTC?
	ldr_ r1,sramwptr		@could be used for RTC?
	str_ r1,writemem_tbl+40
	str_ r1,writemem_tbl+44
 .if RESIZABLE
	ldr_ r1,xgb_vram
	sub r1,r1,#0x8000
	str_ r1,memmap_tbl+32
	str_ r1,memmap_tbl+36
	
	ldr_ r1,xgb_sram
	sub r1,r1,#0xA000
	str_ r1,memmap_tbl+40
	str_ r1,memmap_tbl+44
 .else
	ldr r1,=XGB_VRAM-0x8000
	str_ r1,memmap_tbl+32
	str_ r1,memmap_tbl+36
	ldr r1,=XGB_SRAM-0xA000
	str_ r1,memmap_tbl+40
	str_ r1,memmap_tbl+44
 .endif
	ldr r1,=XGB_RAM-0xC000
	str_ r1,memmap_tbl+48
	str_ r1,memmap_tbl+52
	ldr r1,=XGB_HRAM-0xFF80
	str_ r1,memmap_tbl+56
	str_ r1,memmap_tbl+60

	mov gb_pc,#0		@(eliminates any encodePC errors during mapper*init)
	str_ gb_pc,lastbank

	ldrb r0,[r3,#0x147]	@get mapper#
				@lookup mapper*init
	adr r1,mappertbl
lc0:	ldr r2,[r1],#8
	teq r2,r0
	beq lc1
	bpl lc0
lc1:				@call mapper*init
	adr lr,0f
	adr_ r5,writemem_tbl
	ldr r0,[r1,#-4]
	ldmia r0!,{r1-r4}
	str r1,[r5],#4
	str r1,[r5],#4
	str r2,[r5],#4
	str r2,[r5],#4
	str r3,[r5],#4
	str r3,[r5],#4
	str r4,[r5],#4
	str r4,[r5],#4
@	stmia r5,{r1-r4}
	mov pc,r0		@Jump to MapperInit
0:
@	bl mirror1_		;(call after mapperinit to allow mappers to set up cartflags first)
	
	#if LITTLESOUNDDJ
	@Little Sound DJ stuff
	ldrb_ r0,sramsize
	cmp r0,#4
	bleq little_sound_dj_init
	@END Little Sound DJ stuff
	#endif

	bl emu_reset		@reset everything else
	ldr r0,=get_saved_sram
	mov lr,pc
	bx r0
	
	ldmfd sp!,{r4-r11,lr}
	bx lr

@----------------------------------------------------------------------------
rammasktbl:
	.word 0x0000
	.word 0x07FF
	.word 0x1FFF
	.word 0x7FFF
	.word 0x7FFF @little sound DJ 128k
	.word 0x01FF

	#if LITTLESOUNDDJ
@LITTLE SOUND DJ STUFF!

@----------------------------------------------------------------------------
sram_W_LSDJ:	@sram write ($A000-$BFFF)
@----------------------------------------------------------------------------
	ldr_ r1,memmap_tbl+40
	tst addy,#1
	bic addy,addy,#1
	ldrh r2,[r1,addy]
	bicne r2,r2,#0xFF00
	orrne r2,r2,r0,lsl#8
	biceq r2,r2,#0x00FF
	orreq r2,r2,r0
	strh r2,[r1,addy]
	mov pc,lr

 M3_SRAM_BUFFER = 0x9FE0000

LSDJ_MAP:
	strb_ r0,mapperdata+4
LSDJ_MAP2:
	ldrb_ r0,mapperdata+4		@rambank
	ldr r1,=sram_W_LSDJ
	str_ r1,sramwptr
	str_ r1,writemem_tbl+40
	str_ r1,writemem_tbl+44
	ldr r1,=mem_RA0
	str_ r1,readmem_tbl_-40
	str_ r1,readmem_tbl_-44
	ldr r1,=M3_SRAM_BUFFER-0xA000
	ldr r2,=0x1FFFF
	and r0,r2,r0,lsl#13
	add r0,r1,r0
	str_ r0,memmap_tbl+40
	str_ r0,memmap_tbl+44
	b flush

little_sound_dj_init:
	ldr r1,=sram_W_LSDJ
	str_ r1,sramwptr
	str_ r1,writemem_tbl+40
	str_ r1,writemem_tbl+44
	adr_ r2,writemem_tbl
	ldr r1,=LSDJ_MAP
	str r1,[r2,#16]
	str r1,[r2,#20]
	ldr r1,=LSDJ_MAP2
	str r1,[r2,#0]
	str r1,[r2,#4]
	
	mov addy,lr
	@clear M3_SRAM_BUFFER
	mov r1,#0
	ldr r0,=M3_SRAM_BUFFER
	mov r2,#128*1024
	bl memset32_
	mov lr,addy
	
	bx lr

@END LITTLE SOUND DJ STUFF!
	#endif


@----------------------------------------------------------------------------
FixRealBios:@		r3=rombase
@----------------------------------------------------------------------------
	adr r2,DMGBIOS
	str_ r2,biosbase
	mov r1,#0x100
biosloop:
	ldr r0,[r3,r1]
	str r0,[r2,r1]
	add r1,r1,#4
	cmp r1,#0x200
	bne biosloop
	bx lr

 .if SAVESTATES

@----------------------------------------------------------------------------
savestate:	@called from ui.c.
@int savestate(void *here): copy state to <here>, return size
@----------------------------------------------------------------------------
	stmfd sp!,{r4-r6,globalptr,lr}

	ldr globalptr,=GLOBAL_PTR_BASE

	ldr_ r2,rombase
	rsb r2,r2,#0			@adjust rom maps,etc so they aren't based on rombase
	bl fixromptrs			@(so savestates are valid after moving roms around)

	mov r6,r0			@r6=where to copy state
	mov r0,#0			@r0 holds total size (return value)

	adr r4,savelst			@r4=list of stuff to copy
	mov r3,#(lstend-savelst)/8	@r3=items in list
ss1:	ldr r2,[r4],#4				@r2=what to copy
	ldr r1,[r4],#4				@r1=how much to copy
	add r0,r0,r1
ss0:	ldr r5,[r2],#4
	str r5,[r6],#4
	subs r1,r1,#4
	bne ss0
	subs r3,r3,#1
	bne ss1

	ldr_ r2,rombase
	bl fixromptrs

	ldmfd sp!,{r4-r6,globalptr,lr}
	bx lr


@saveversion DCB "A005"
@@AF,BC,DE,HL,SP,PC
@
@savetags DCB "VERS","CFG ","REGS","RAM ","HRAM","VRAM","RAM2","SRAM","PAL ","MAPR","BANK","CPUS","GFXS"
@savesizes DCB 4,4,

@savelst	DCD rominfo,4,XGB_RAM,0x2080,XGB_VRAM,0x2000,XGB_SRAM,0x8000,GBOAM_BUFFER,0xA0,gbc_palette,128
@	DCD mapperstate,32,rommap,16,cpustate,52,lcdstate,16
@lstend

@savelst	DCD rominfo,4,XGB_RAM,0x2080,XGB_VRAM,0x2000,XGB_SRAM,0x8000,GBOAM_BUFFER,0xA0,agb_pal,96
@	DCD vram_map,64,agb_nt_map,16,mapperstate,32,rommap,16,cpustate,52,lcdstate,16


fixromptrs:	@add r2 to some things
	adr_ r1,memmap_tbl
	ldmia r1,{r3-r6}
	add r3,r3,r2
	add r4,r4,r2
	add r5,r5,r2
	add r6,r6,r2
	stmia r1,{r3-r6}
	adr_ r1,memmap_tbl+16
	ldmia r1,{r3-r6}
	add r3,r3,r2
	add r4,r4,r2
	add r5,r5,r2
	add r6,r6,r2
	stmia r1,{r3-r6}

	ldr_ r3,lastbank
	add r3,r3,r2
	str_ r3,lastbank

	ldr_ r3,cpuregs+6*4	@GB-Z80 PC
	add r3,r3,r2
	str_ r3,cpuregs+6*4

	mov pc,lr
@----------------------------------------------------------------------------
loadstate:	@called from ui.c
@void loadstate(int rom#,u32 *stateptr)	 (stateptr must be word aligned)
@----------------------------------------------------------------------------
	stmfd sp!,{r4-r7,globalptr,r11,lr}

	mov r6,r1		@r6=where state is at
	ldr globalptr,=GLOBAL_PTR_BASE

	ldr r1,[r6]             @emuflags
	bl loadcart		@cart init

	mov r0,#(lstend-savelst)/8	@read entire state
	adr r4,savelst
ls1:	ldr r2,[r4],#4
	ldr r1,[r4],#4
ls0:	ldr r5,[r6],#4
	str r5,[r2],#4
	subs r1,r1,#4
	bne ls0
	subs r0,r0,#1
	bne ls1

	ldr_ r2,rombase		@adjust ptr shit (see savestate above)
	bl fixromptrs

	ldr r3,=XGB_VRAM	@init tiles+tilemaps
	mov r4,#0x8000
ls3:	ldrb r0,[r3],#1
	mov addy,r4
	bl vram_W
	add r4,r4,#1
	tst r4,#0x2000
	beq ls3

	bl resetlcdregs

	ldmfd sp!,{r4-r7,globalptr,r11,lr}
	bx lr
 .endif

@----------------------------------------------------------------------------
@m0000	DCD 0x1102,XGB_VRAM+0x1800,XGB_VRAM+0x1800,XGB_VRAM+0x1800,XGB_VRAM+0x1800
@		DCD AGB_BG1+0x0000,AGB_BG1+0x0000,AGB_BG1+0x0000,AGB_BG1+0x0000

@----------------------------------------------------------------------------
DMGBIOS:
@	INCBIN DMGBIOS.ROM
@	% 256

@----------------------------------------------------------------------------
@mirror1_
@	ldr r0,=m0000
@	stmfd sp!,{r0,r3-r5,lr}
@
@	ldr r0,[sp],#4
@	ldr r3,[r0],#4
@
@	ldr r1,=vram_map+32
@	ldmia r0!,{r2-r5}
@	stmia r1,{r2-r5}
@	ldr r1,=agb_nt_map
@	ldmia r0!,{r2-r5}
@	stmia r1,{r2-r5}
@	ldmfd sp!,{r3-r5,pc}
@----------------------------------------------------------------------------
mapBIOS_:
@----------------------------------------------------------------------------
	ldr_ r0,biosbase
	str_ r0,memmap_tbl
	str_ r0,memmap_tbl+4
	b_long flush
@----------------------------------------------------------------------------
map0123_:
@----------------------------------------------------------------------------
	ldr_ r1,rommask
	and r0,r0,r1,lsr#14
	str_ r0,bank0
	ldr r1,=INSTANT_PAGES
	ldr r0,[r1,r0,lsl#2]
	subs r0,r0,#0x0000
	.if MOVIEPLAYER
	bmi need_to_use_cache
	.endif
	str_ r0,memmap_tbl
	str_ r0,memmap_tbl+4
	str_ r0,memmap_tbl+8
	str_ r0,memmap_tbl+12
@	ldr r1,rombase
@	ldr r2,rommask
@	and r0,r2,r0,lsl#14
@	add r0,r1,r0
@	str r0,memmap_tbl
@	str r0,memmap_tbl+4
@	str r0,memmap_tbl+8
@	str r0,memmap_tbl+12
	b_long flush
@@----------------------------------------------------------------------------
@mapCDEF_
@@----------------------------------------------------------------------------
@	ldr r1,rommask
@	and r0,r0,r1,lsr#14
@	mov r0,r0,lsl#1
@	strb r0,bankC
@	add r2,r0,#1
@	strb r2,bankE
@	ldr r1,instant_prg_banks
@	ldr r0,[r1,r0,lsl#2]!
@	subs r0,r0,#0xC000
@	bmi need_to_use_cache
@@don't bother with checking if page is whole
@@	ldr r2,[r1,#4]!
@@	subs r2,r2,#0xE000
@@	cmp r0,r2
@@	bne need_to_use_cache
@	str r0,memmap_tbl+24
@	str r0,memmap_tbl+28
@	b flush

@----------------------------------------------------------------------------
 .section .iwram.4, "ax", %progbits
@----------------------------------------------------------------------------
map4567_:
@----------------------------------------------------------------------------
	ldr_ r1,rommask
	and r0,r0,r1,lsr#14
	str_ r0,bank1
	ldr r1,=INSTANT_PAGES
	ldr r0,[r1,r0,lsl#2]
	subs r0,r0,#0x4000
	.if MOVIEPLAYER
	bmi need_to_use_cache
	.endif
	str_ r0,memmap_tbl+16
	str_ r0,memmap_tbl+20
	str_ r0,memmap_tbl+24
	str_ r0,memmap_tbl+28
@	
@	
@	ldr r1,rombase
@	sub r1,r1,#0x4000
@	ldr r2,rommask
@	and r0,r2,r0,lsl#14
@	add r0,r1,r0
@	str r0,memmap_tbl+16
@	str r0,memmap_tbl+20
@	str r0,memmap_tbl+24
@	str r0,memmap_tbl+28
flush:		@update gb_pc & lastbank
	ldr_ r1,lastbank
	sub gb_pc,gb_pc,r1
	encodePC
	mov pc,lr

	.if MOVIEPLAYER
call_update_cache:
	ldr r0,=update_cache
	bx r0

need_to_use_cache:
	stmfd sp!,{r0-addy,lr}
	bl call_update_cache
	ldmfd sp!,{r0-addy,lr}
	b flush
	.endif

@@----------------------------------------------------------------------------
@map01234567_
@@----------------------------------------------------------------------------
@	mov r0,r0,lsl#1
@	ldr r1,rommask
@	and r0,r0,r1,lsr#14
@	str r0,bank0
@	add r1,r0,#1
@	str r1,bank1
@	ldr r1,=INSTANT_PAGES
@	ldr r0,[r1,r0,lsl#2]
@	subs r0,r0,#0x0000
@@	bmi need_to_use_cache
@	str r0,memmap_tbl 
@	str r0,memmap_tbl+4
@	str r0,memmap_tbl+8
@	str r0,memmap_tbl+12
@	str r0,memmap_tbl+16
@	str r0,memmap_tbl+20
@	str r0,memmap_tbl+24
@	str r0,memmap_tbl+28
@
@
@@	ldr r1,rombase
@@	ldr r2,rommask
@@	and r0,r2,r0,lsl#15
@@	add r0,r1,r0
@@	str r0,memmap_tbl
@@	str r0,memmap_tbl+4
@@	str r0,memmap_tbl+8
@@	str r0,memmap_tbl+12
@@	str r0,memmap_tbl+16
@@	str r0,memmap_tbl+20
@@	str r0,memmap_tbl+24
@@	str r0,memmap_tbl+28
@	b flush

@----------------------------------------------------------------------------
mapAB_:
	str_ r0,srambank
 .if RESIZABLE
	ldr_ r1,xgb_sram
	sub r1,r1,#0xA000
 .else
	ldr r1,=XGB_SRAM-0xA000
 .endif
	ldr_ r2,rammask
	and r0,r2,r0,lsl#13
	add r0,r1,r0
	str_ r0,memmap_tbl+40
	str_ r0,memmap_tbl+44
	b flush
@----------------------------------------------------------------------------
 .section .iwram.end.102, "ax", %progbits

g_banks:
	.word 0	@bank0
	.word 0	@bank1
	.word 0   @srambank
mapperstate:
	.skip 32	@mapperdata
	@0: MBC1: low bits of bank
	@1: 
	@2: ram enable
	@3: 
	@4: sram bank
	@5: bankswitch ram/rom mode
	@more....
	
	
	.word 0	@sramwptr
biosstart:
	.word 0	@biosbase
romstart:
	.word 0	@rombase
romnum:
	.word 0	@romnumber
rominfo:			@emuflags (for savestate/loadstate)
g_emuflags:	.byte 0	@emuflags        (label this so UI.C can take a peek) see equates.h for bitfields
	.skip 3		@(sprite follow val)

	.word 0	@rommask
g_rammask:
	.word 0	@rammask
g_cartflags:
	.byte 0	@cartflags
g_sramsize:	
	.byte 0	@sramsize

	.byte 0
	.byte 0

@----------------------------------------------------------------------------
	@.end
