 .section .iwram.0, "ax", %progbits

rng_hack_ram_address:
	.word XGB_RAM + 0x381
@----------------------------------------------------------------------------
_EB:@	Shantae Speed Hack (RNG hack)
@----------------------------------------------------------------------------
	@xx = [c381]
	@xx = xx + xx << 4 + 0x5393
	@[C381] = xx
	@got +13 FPS from this hack
	ldr r12,rng_hack_ram_address
	ldrb r1,[r12,#0]
	ldrb r0,[r12,#1]
	orr r1,r1,r0,lsl#8
	add r2,r1,r1,lsl#4
	add r2,r2,#0x0093
	strb r2,[r12,#0]
	add r2,r2,#0x5300
	mov r2,r2,lsr#8
	strb r2,[r12,#1]
	add gb_pc,gb_pc,#2
	fetch 272

 @.text

@----------------------------------------------------------------------------
_EC:@	Shantae Speed Hack (copy 32 bytes)
@----------------------------------------------------------------------------
@copy 0x20 bytes from HL (not word aligned) to DE (word aligned), jump ahead 0x5F bytes, eat 644 cycles
@	@okay to clobber r3,r4 (set to zero afterwards),lr
@	@got +4 FPS from this hack
@	and r1,gb_hl,#0xF0000000
@	adr_ r2,memmap_tbl
@	ldr r1,[r2,r1,lsr#26]
@	add r1,r1,gb_hl,lsr#16
@	and r0,gb_de,#0xF0000000
@	ldr r0,[r2,r0,lsr#26]
@	add r0,r0,gb_de,lsr#16
@	
@	mov r2,#32
@	bl memcpy_unaligned_src
@	add gb_hl,gb_hl,#0x00200000
@	add gb_de,gb_de,#0x00200000
@	add gb_pc,gb_pc,#0x5F
@	fetch 644
	
	@new hack: treat it as canonical source of DMA buffer C4C0
	and r1,gb_hl,#0xF0000000
	adr_ r2,memmap_tbl
	ldr r1,[r2,r1,lsr#26]
	add r1,r1,gb_hl,lsr#16
	@r1 = true source address  (in GBA ROM/VRAM)
	@and r0,gb_de,#0xF0000000
	@ldr r0,[r2,r0,lsr#26]
	@add r0,r0,gb_de,lsr#16
	mov r0,gb_de,lsr#16
	@r0 = dest address value (in GBC RAM)
	
	adrl_ r12,vram_packet_dest
	ldrh r2,[r12,#0] @vram_packet_dest
	cmp r0,r2
	bne not_continuing_packet
	ldr r2,[r12,#4] @vram_packet_src
	cmp r1,r2
	bne not_continuing_packet
	add r2,r0,#0x20
	strh r2,[r12,#0] @vram_packet_dest
	add r2,r1,#0x20
	str r2,[r12,#4] @vram_packet_src
continuing_packet:
	
	add gb_hl,gb_hl,#0x00200000
	add gb_de,gb_de,#0x00200000
	add gb_pc,gb_pc,#0x5F
	
	fetch 644
	
not_continuing_packet:
	blx_long new_dma_packet
	b continuing_packet


	
@	@old hack:
@	
@	
@	@copy 0x20 bytes from HL (not word aligned) to DE (word aligned), jump ahead 0x5F bytes, eat 644 cycles
@	@okay to clobber r3,r4 (set to zero afterwards),lr
@	@got +4 FPS from this hack
@	
@	
@	
@	
@	and r1,gb_hl,#0xF0000000
@	adr_ r2,memmap_tbl
@	ldr r1,[r2,r1,lsr#26]
@	add r1,r1,gb_hl,lsr#16
@	and r0,gb_de,#0xF0000000
@	ldr r0,[r2,r0,lsr#26]
@	add r0,r0,gb_de,lsr#16
@	
@	mov r2,#32
@	ands r12,r1,#3
@	
@	beq 3f
@	
@	stmfd sp!,{r3,r4}
@	
@	mov r12,r12,lsl#3
@	rsb lr,r12,#32
@	@r12 = RSHIFT
@	@lr = LSHIFT
@	bic r1,r1,#3
@	ldr r3,[r1],#4
@	mov r3,r3,lsr r12
@0:	
@	ldr r4,[r1],#4
@	orr r3,r3,r4,lsl lr
@	str r3,[r0],#4
@	mov r3,r4,lsr r12
@	subs r2,r2,#4
@	bne 0b
@	ldmfd sp!,{r3,r4}
@1:
@	add gb_hl,gb_hl,#0x00200000
@	add gb_de,gb_de,#0x00200000
@	add gb_pc,gb_pc,#0x5F
@	fetch 644
@3:
@	adr lr,1b
@	@aligned
@	b memcpy__

@----------------------------------------------------------------------------
_FC:@	Shantae Speed Hack (off-page call)
@----------------------------------------------------------------------------
	@adds up to +2FPS
@ROM0:0540 E1               pop hl
	@pop hl
	and r1,gb_sp,#0xF0000000
	adr_ r2,memmap_tbl
	ldr r1,[r2,r1,lsr#26]
	add r1,r1,gb_sp,lsr#16
	ldrb r12,[r1]   @low byte
	ldrb r0,[r1,#1]	@high byte
	orr r0,r12,r0,lsl#8
	
	@r1 = address of stack before pop hl
	@r0 = hl from stack
	
	@read 16 bit address from (hl)
	and r12,r0,#0xF000
	@r2 = memmap_tbl
	ldr r12,[r2,r12,lsr#10]
	add r12,r12,r0
	@r12 = address of hl from stack
	ldrb lr,[r12],#1
	ldrb r2,[r12],#1
	
	@store to C200, C201
	strb_ lr,xgb_ram+0x200
	strb_ r2,xgb_ram+0x201
	
	orr r2,lr,r2,lsl#8
	
	@r2 = address to jump to
	ldrb lr,[r12],#1
	@lr = bank number to switch to
	
	@put address to jump to into hl
	mov gb_hl,r2,lsl#16
	
	add r0,r0,#3
	
	@store hl to stack
	strb r0,[r1,#0] @high byte
	mov r0,r0,lsr#8
	strb r0,[r1,#1] @low byte
	
	@store value from FF91 to stack
	adr_ r12,xgb_hram
	ldrb r0,[r12,#0x11]
	strb r0,[r1,#-1] @high byte
	@todo: encode flags and store to r1,#-2
	
_FC_modify:
	mov r0,#0x05	@modified to 0x04 for Zapped
	strb r0,[r1,#-3] @high byte
	mov r0,#0x64
	strb r0,[r1,#-4] @low byte
	
	sub gb_sp,gb_sp,#0x00040000
	
	and r0,gb_flg,#0xA0000000	@nC
	and r12,gb_flg,#0x50000000	@Zh
	mov r12,r12,lsr#23
	orr r0,r12,r0,lsr#25			@N
	
	strb r0,[r1,#-2] @low byte
	
	@set E = A, set D = page number, set [ff91] = page number
	mov r0,lr
	mov gb_de,gb_a,lsr#8
	orr gb_de,gb_de,r0,lsl#24
	adr_ r12,xgb_hram
	strb r0,[r12,#0x11]
	@bankswitch to page number
	bl map4567_
	@jump to hl
	mov gb_pc,gb_hl,lsr#16
	encodePC
	fetch 224
	
	@.text
	
@----------------------------------------------------------------------------
_FD:@	Shantae Speed Hack (replaces 5 instructions)
@----------------------------------------------------------------------------
	@1314:
	@push de	16
	@ld hl,0	16
	@add hl,bc	8
	@ld a,(hl)	8
	@cp a,ff	8
	@game doesn't actually need new values of HL or A, just whether the memory byte = FF or not
	
	@SP is in C000-CFFF
	ldr_ r1,memmap_tbl + 0x0C * 4
	subs gb_sp,gb_sp,#0x00020000
	mov r0,gb_de,lsr#16
	strmib r0,[r1,gb_sp,lsr#16]!
	mov r0,r0,lsr#8
	strmib r0,[r1,#1]
	

	@BC is in D000-DFFF
	ldr_ r1,memmap_tbl + 0x0D * 4
	ldrb r0,[r1,gb_bc,lsr#16]
	@mov gb_a,r0,lsl#24   @game doesn't care about A here
	@game only cares if byte = FF, not other flags
	cmp r0,#0xFF
	moveq gb_flg,#PSR_Z
	movne gb_flg,#PSR_C
	
	add gb_pc,gb_pc,#7
	
	fetch 56

@----------------------------------------------------------------------------
_F4:@	Shantae Speed Hack (replaces 7 instructions)
@----------------------------------------------------------------------------
	@1383:
	@ld a,c		4
	@add a,0x7E	8		(0x7B for wendy)
	@ld c,a		4
	@jc nc,138a	8/12
	@inc b		4
	@pop de		12
	@dec e		4
	@total:		44
	add gb_bc,gb_bc,#0x007E0000
	@SP is in C000-CFFF
	ldr_ r1,memmap_tbl + 0x0C * 4
	ldrb r0,[r1,gb_sp,lsr#16]!
	ldrb r2,[r1,#1]
	subs r0,r0,#1
	add gb_sp,gb_sp,#0x00020000
	mov gb_de,r0,lsl#16
	orr gb_de,r2,lsl#24
	@only cares about zero flag
	moveq gb_flg,#PSR_Z
	movne gb_flg,#0

	add gb_pc,gb_pc,#8

	fetch 44

@----------------------------------------------------------------------------
_E3:@	Shantae Speed Hack (replaces 13 instructions)
@----------------------------------------------------------------------------
	@131E:
	@ld hl,0016		16
	@add hl,bc		8
	@ld a,(hl)		8
	@sub a,00		8
	@ldi (hl),a		8
	@ld a,(hl)		8
	@sbc a,01		8
	@ldd (hl),a		8
	@ldi a,(hl)		8
	@cp a,80		8
	@ld a,(hl)		8
	@sbc a,00		8
	@bit 7,a		8
	@total:			112 cycles
	ldr_ r1,memmap_tbl + 0x0D * 4
	add r1,r1,gb_bc,lsr#16
	ldrb r0,[r1,#0x17]
	sub r0,r0,#1
	strb r0,[r1,#0x17]
	ldrb r1,[r1,#0x16]
	cmp r1,#0x80
	sublt r0,r0,#1
	tst r0,#0x80
	moveq gb_flg,#PSR_Z
	movne gb_flg,#0
	add gb_pc,gb_pc,#(0x1332 - 0x131E - 1)  @19
	@does not care about values of a or hl
	fetch 112
	
@[bc + 0x17] -= 1		@bc is in RAM D000
@a = [bc + 0x17]
@if ([bc + 0x16] < 0x80)
@	a--
@Z = !(a & 0x80)
	
	

@----------------------------------------------------------------------------
_E4:@	Shantae Speed Hack (replaces 9 instructions)
@----------------------------------------------------------------------------
	@1334:
	@ld hl,0002		16
	@add hl,bc		8
	@ldi a,(hl)		8
	@ld e,a			4
	@ldi a,(hl)		8
	@ld d,a			4
	@ld a,(hl)		8
	@ld (FF00+91),a	12
	@ld (2000),a	16
	@total:			84
	ldr_ r1,memmap_tbl+0x0D * 4
	add r1,r1,gb_bc,lsr#16
	ldrb r0,[r1,#0x4]
	adr_ r2,xgb_hram - 0x80
	strb r0,[r2,#0x91]
	strb_ r0,mapperdata
	ldrb gb_de,[r1,#2]
	ldrb r2,[r1,#3]
	orr r2,gb_de,r2,lsl#8
	mov gb_de,r2,lsl#16
	bl map4567_
	add gb_pc,gb_pc,#(0x1342 - 0x1334 - 1)  @13
	@does not care about values of a or hl
	fetch 84

@BC in RAM D000
@de = [bc + 0x02]  new DE in ROM 4000
@newBank = [bc + 0x04]
@[ff91] = newBank
@set new bank


@----------------------------------------------------------------------------
_DB:@	Shantae Speed Hack (replaces 8 instructions)
@----------------------------------------------------------------------------
	@1342:		
	@ld a,(de)	8
	@inc de 	8
	@ld l,a 	4
	@ld h,06 	8
	@ldi a,(hl)	8
	@ld h,(hl)	8
	@ld l,a 	4
	@@jp hl 	4
	@total:		52 - 4
	ldr_ r1,memmap_tbl+0x04*4
	add r1,r1,gb_de,lsr#16
	add gb_de,gb_de,#0x10000
	ldrb r0,[r1,#0]
	orr r0,r0,#0x0600
	ldr_ r1,memmap_tbl+0x00*4
	ldrb r0,[r1,r0]!
	ldrb r1,[r1,#1]
	orr r0,r0,r1,lsl#8
	mov gb_hl,r0,lsl#16
	mov gb_a,r0,lsl#24
	
	sub cycles,cycles,#48 * CYCLE
	@proceed to JP HL
	b_long _E9
	
@hl = 0x0600 + [de++]
@hl = [hl]
@a = l
@jp hl

@----------------------------------------------------------------------------
_DD:@	Shantae Speed Hack (replaces 15 instructions)
@----------------------------------------------------------------------------
	@1362:
	@ld hl, 0016	16
	@add hl,bc 		8
	@ld a, (de) 	8	load low byte
	@inc de 		8
	@add (hl) 		8	add to low byte
	@ldi (hl),a 	8	store back low byte
	@ld a, (de) 	8	load high byte
	@inc de 		8
	@adc (hl) 		8	add to high byte with carry
	@ldd (hl),a 	8	store back high byte
	@ldi a, (hl) 	8	load low byte
	@cp a,80 		8	compare with 0x80
	@ld a, (hl) 	8	load high byte
	@sbc a,00 		8	
	@bit 7,a 		8
	@total:			128
	
	@de in ROM at 4000-7FFF, not halfword aligned
	ldr_ r12,memmap_tbl+0x04*4
	ldrb r0,[r12,gb_de,lsr#16]!
	ldrb r1,[r12,#1]
	orr r0,r0,r1,lsl#8
	add gb_de,gb_de,#0x20000
	@bc in RAM D000, is NOT halfword aligned
	ldr_ r12,memmap_tbl+0x0D*4
	add r12,r12,gb_bc,lsr#16
	ldrb r2,[r12,#0x17]
	ldrb r1,[r12,#0x16]
	orr r1,r1,r2,lsl#8
	add r0,r0,r1
	strb r0,[r12,#0x16]
	mov r1,r0,lsr#8
	strb r1,[r12,#0x17]
	tst r0,#0x80
	subeq r0,r0,#0x100
	tst r0,#0x8000
	moveq gb_flg,#PSR_Z
	movne gb_flg,#0
	add gb_pc,gb_pc,#(0x1376 - 0x1362 - 1)  @19
	@does not care about values of a or hl
	fetch 128

@----------------------------------------------------------------------------
_D3:@	Shantae Speed Hack (replaces 8 instructions)
@----------------------------------------------------------------------------
	@1378:				
	@ld hl,0002 		16
	@add hl,bc 			8
	@ld a,e 			4
	@ldi (hl),a 		8
	@ld a,d 			4
	@ldi (hl),a 		8
	@ld a, (ff00+91)	8
	@ld (hl),a 			8
	@total:				64
	ldr_ r1,memmap_tbl+0x0D*4
	add r1,r1,gb_bc,lsr#16
	mov r0,gb_de,lsr#16
	strb r0,[r1,#2]!
	mov r0,r0,lsr#8
	strb r0,[r1,#1]
	adr_ r2,xgb_hram - 0x80
	ldrb r0,[r2,#0x91]
	strb r0,[r1,#2]
	add gb_pc,gb_pc,#(0x1383 - 0x1378 - 1) @10
	@does not care about values of a or hl
	fetch 64

@ROM0:0541 5F               ld e,a			4
@ROM0:0542 2A               ldi a,(hl)		8
@ROM0:0543 EA 00 C2         ld (c200),a		16
@ROM0:0546 2A               ldi a,(hl)		8
@ROM0:0547 EA 01 C2         ld (c201),a		16
@ROM0:054A 2A               ldi a,(hl)		8
@ROM0:054B 57               ld d,a			4
@ROM0:054C E5               push hl			16
@ROM0:054D F0 91            ld a,(ff00+91)	12
@ROM0:054F F5               push af			16
@ROM0:0550 21 64 05         ld hl,0564		16
@ROM0:0553 E5               push hl			16
@ROM0:0554 FA 00 C2         ld a,(c200)		16
@ROM0:0557 6F               ld l,a			4
@ROM0:0558 FA 01 C2         ld a,(c201)		16
@ROM0:055B 67               ld h,a			4
@ROM0:055C 7A               ld a,d			4
@ROM0:055D E0 91            ld (ff00+91),a	16
@ROM0:055F EA 00 20         ld (2000),a		16
@ROM0:0562 7B               ld a,e			4
@ROM0:0563 E9               jp hl			4

	.text

	@sabrina zapped RNG
_EB_2:
@0AE1:
	ldr r1,=XGB_RAM - 0xC000 + 0xC274
	ldr r0,[r1]
	add r0,r0,#0x00000001
	
	@todo: more accurate estimation for cycles of 32-bit increment
	msr cpsr_f,gb_flg
	mov r3,#0xFF	@using r3 (gb_flg) as a mask, it gets replaced later
	and r12,r3,r0,lsr#24
	adc r12,r12,#0xC0
	movs r2,r12,lsl#24
	
	and r12,r3,r0,lsr#16
	adc r12,r12,#0x45
	movs r12,r12,lsl#24
	orr r2,r2,r12,lsr#8
	
	and r12,r3,r0,lsr#8
	adc r12,r12,#0xA5
	movs r12,r12,lsl#24
	orr r2,r2,r12,lsr#16
	
	and r12,r3,r0,lsr#0
	adc r12,r12,#0x55
	movs r12,r12,lsl#24
	orr r0,r2,r12,lsr#24
	
	ldr r2,=0xBEBAFECA
	eor r0,r0,r2
	str r0,[r1]
	@value of A removed:
	eor r0,r0,r0,lsl#8
	eor r0,r0,r0,lsl#16
	movs gb_a,r0,lsl#24
	moveq gb_flg,#PSR_Z
	movne gb_flg,#0
	@mov gb_flg,#0
	add gb_pc,gb_pc,#2
	fetch (91 + 4) * 4

@----------------------------------------------------------------------------
_EB_dw3:@	Dragon Warrior 3 RNG
@----------------------------------------------------------------------------
	@seed = [C29D]
	ldr r12,=XGB_RAM
	ldr r0,[r12,#0x29C]
	mov r0,r0,lsr#8
	ldrb r2,[r12,#0x2A0]
	orr r0,r0,r2,lsl#24

	@seed = ((seed << 17) >> 15) + ((seed & 0xFF000000) << 1) + ((seed & 0xFF0000) + ((seed << 1) & 0xFF0000));

	@gb_a used as temp register
	mov gb_a,r0,lsl#1
	mov r1,r0,lsl#17
	and r2,gb_a,#0xFC000000
	add r1,r2,r1,lsr#15
	and r2,r0,#0x00FF0000
	add r1,r1,r2
	and r2,gb_a,#0x00FF0000
	add r0,r1,r2

	@seed += 0x1357 + (counter++)
	add r0,r0,#0x57
	add r0,r0,#0x1300
	ldrh r1,[r12,#0xFC]
	adds r0,r0,r1
	add r1,r1,#1
	strh r1,[r12,#0xFC]
	@set A register to (seed >> 16)
	mov gb_a,r0,lsl#8
	and gb_a,gb_a,#0xFF000000
	@todo: set zero based on highest byte of r0, and carry based on carry flag
	add gb_pc,gb_pc,#2
	fetch (154 * 4)

_EB_drymouth:
	@hack for drymouth
	add gb_pc,gb_pc,#0x11
	fetch 20*33*4
_D3_drymouth:
	@hack for drymouth
	@314A: divide bc by de, and stick result into bc and hl, de = modulo
	mov r0,gb_bc,lsr#16
	mov r1,gb_de,lsr#16
	@destroys r3
	blx_long __aeabi_idivmod
	mov gb_bc,r0,lsl#16
	mov gb_hl,r0,lsl#16
	mov gb_de,r1,lsl#16
	mov gb_a,#0
	mov gb_flg,#PSR_Z
	
	@eat a bunch of cycles
	ldr r0,=(707-14-4)*4*CYCLE
	sub cycles,cycles,r0
	
	@proceed to RET instruction
	b_long _C9

@----------------------------------------------------------------------------------------------------
_EC_bionic:     @Bionic Commando memcpy 
@----------------------------------------------------------------------------------------------------
@copy B bytes from HL to DE C times, add 0x20 to HL and DE between every iteration
	@get real addresses of HL and DE
	adr_ r2,memmap_tbl
	
	and r0,gb_de,#0xF0000000
	and r1,gb_hl,#0xF0000000
	ldr r0,[r2,r0,lsr#26]
	ldr r1,[r2,r1,lsr#26]
	add r0,r0,gb_de,lsr#16
	add r1,r1,gb_hl,lsr#16
	
	@if C is 1, copy B bytes
	mov r12,gb_bc,lsr#24
	mov r2,gb_bc,lsr#16
	and r2,r2,#0xFF
	cmp r2,#1
	beq .L_copy_b
	@otherwise, if B is 0x20, copy 0x20 * C bytes
	cmp r12,#0x20
	beq .L_copy_20c
	@otherwise copy B bytes C times, adding 0x20 each time
	stmfd sp!,{r3,r4,r5,r6}
	mov r3,r0
	mov r4,r1
	mov r5,r2
	mov r6,r12
	
@	mov r6,r1
@	mov r4,r0
@	mov r3,r2
@	mov r5,r2
0:	
	mov r2,r6
	bl_long memcpy
	add r3,r3,#0x20
	mov r0,r3
	add r4,r4,#0x20
	mov r1,r4
	subs r5,r5,#1
	bgt 0b
	ldmfd sp!,{r3,r4,r5,r6}
	b_long _C9
	
.L_copy_b:	
	mov r2,r12
	bl_long memcpy
	b_long _C9

.L_copy_20c:
	mov r2,r2,lsl#5
	bl_long memcpy
	b_long _C9
	
#if 0

	mov r11,r11
	@391B:
	@ldi a,(hl)
	@ld (de),a
	@inc de
	@deb b
	@ld a,b
	@or a
	@jr nz, 391B
	
	@B is always 32
	@DE is always in page C000
	@HL is always in page D000
	adr_ r2,memmap_tbl
	and r0,gb_de,#0xF0000000
	and r1,gb_hl,#0xF0000000

	cmp r0,#0xC0000000
	beq 0f
	mov r11,r11
	0:
	
	cmp r1,#0xD0000000
	beq 0f
	mov r11,r11
	0:

	ldr r0,[r2,r0,lsr#26]
	ldr r1,[r2,r1,lsr#26]
	add r0,r0,gb_de,lsr#16
	add r1,r1,gb_hl,lsr#16
	mov r2,gb_bc,lsr#24
	
	ands r12,r2,#3
	beq 0f
	mov r11,r11
	0:
	
	bl_long memcpy

	mov r2,gb_bc,lsr#24
	@advance hl, de, bc
	add gb_hl,gb_hl,r2,lsl#16
	add gb_de,gb_de,r2,lsl#16
	and gb_bc,gb_bc,#0x00FF0000
	
	@multiply b by 3
	add r2,r2,r2,lsl#1
	@consume cycles
	sub cycles,cycles,r2,lsl#(CYC_SHIFT + 2)    @r2 = iterations * 3, want CYCLE (16) * 4 * iterations * 3
	add gb_pc,gb_pc,#8-1
	
	
	fetch -8
#endif
	
	
	
	
.text

game_specific_hacks_reset:
	stmfd sp!,{r3-r9,r11,lr}
	ldr r10,=GLOBAL_PTR_BASE
	
	mov r0,#0
	strb_ r0,speedhack_mode
	strb_ r0,lcdhack

	bl update_lcdhack

	@set DMA MODE to 0
	mov r0,#0
	ldr r1,=_dmamode
	strb r0,[r1]

	@clear wayforward hacks
	adr r12,wayforward_hack_bytes
	ldr r1,=_xx
	mov r2,#10
0:
	ldrb r0,[r12],#1
	str r1,[r10,r0,lsl#2]
	subs r2,r2,#1
	bgt 0b
	
	bl hack_get_game_number
	ldrpl pc,[pc,r0,lsl#2]
	b hack_unsupported_game
hack_table:
	.word wayforward_hacks	@shantae
	.word wayforward_hacks	@wendy
	.word wayforward_hacks	@spooked
	.word wayforward_hacks	@betrayal
	.word wayforward_hacks	@zapped
	.word hack_done			@xtremesport
	.word hack_dw3			@dw 3
	.word hack_drymouth		@drymouth
	.word hack_lcd			@alice
	@.word hack_lcd			@wario land 2
	@.word hack_lcd			@scooby doo
	.word hack_lcd_high		@megaman x2
	.word hack_done			@balloon gb
	.word hack_conker		@conker
	@.word hack_done			@donkey kong land
	.word hack_bionic
	
hack_game_names:
	.asciz "SHANTAE"
	.asciz "WENDY"
	.asciz "SPOOKED"
	.asciz "BETRAYAL"
	.asciz "ZAPPED"
	.asciz "XTREMESPORTBXGE"  @no hacks for this game
	.asciz "DW 3"			@RNG hack
	.asciz "DRYMOUTH"		@33D2: burns 20*33*4 cycles, add 0x11 to PC
	.asciz "ALICE"			@LCD scanline hack low
	@.asciz "CGBWARIOLAND2"	@LCD scanline hack low
	@.asciz "SCOOBY DOO"	@frequently spins on LCD stat, LCD scanline hack low
	.asciz "MEGAMANX2SEB2XE"   @0703: spins on LCD stat and scanline, LCD scanline hack high
	.asciz "BALLOON GB"	@JR Z hack (highram, and a / or a / cp), LCD scanline hack low
	.asciz "CONKER CGB"	@JR Z hack (ram, and a / or a / cp)
	@.asciz "DKL DX"	@JR NZ hack cp (hl)
	.asciz "BIONIC-COMMAV4E"  @speed hack
	.byte 0
	.align 2
	
hack_get_game_number:
	ldr_ r1,memmap_tbl+0
	add r1,r1,#0x134
	adr r12,hack_game_names
	
	mov r0,#0	@list entry number
	
hggn_trygame:
	mov r2,#0	@character index
0:
	ldrb r3,[r1,r2]
	ldrb r4,[r12,r2]
	cmp r3,r4
	bne hggn_nomatch
	cmp r3,#0
	beq hggn_match
	add r2,r2,#1
	cmp r2,#15
	blt 0b
hggn_match:
	movs r0,r0
	bx lr
hggn_nomatch:
0:
	ldrb r4,[r12,r2]
	add r2,r2,#1
	cmp r4,#0
	bne 0b
1:
	add r12,r12,r2
	ldrb r4,[r12]
	cmp r4,#0
	beq hggn_nomatch_done
	add r0,r0,#1
	b hggn_trygame
	
hggn_nomatch_done:
	mvns r0,#0
	bx lr

hack_lcd_high:
	mov r0,#3
	b 0f

hack_lcd:
	mov r0,#1
0:
	strb_ r0,lcdhack
	bl update_lcdhack
	b hack_done
	

hack_dw3:
	mov r0,#0xEB
	ldr r1,=0x01FD
	bl _pokevram
	ldr r1,=_EB_dw3
	str r1,[r10,r0,lsl#2]
	
	b hack_done

hack_drymouth:
	mov r0,#0xEB
	ldr r1,=0x33D2
	bl _pokevram
	ldr r1,=_EB_drymouth
	str r1,[r10,r0,lsl#2]
	
	mov r0,#0xD3
	ldr r1,=0x314A
	bl _pokevram
	ldr r1,=_D3_drymouth
	str r1,[r10,r0,lsl#2]
	
	b hack_done

hack_bionic:
@	mov r0,#0xEC
@	ldr r1,=0x391B
@	bl _pokevram
@	ldr r1,=_EC_memcpy
@	str r1,[r10,r0,lsl#2]
	mov r0,#0xEC
	ldr r1,=0x390E
	bl _pokevram
	ldr r1,=_EC_bionic
	str r1,[r10,r0,lsl#2]

	@patch the delay loop code - speedhack finder finds something else instead and it's slow
	adr r1,0f
	ldr_ r0,memmap_tbl+0
	mov r2,#0x10
	bl_long memcpy
	ldr_ r0,memmap_tbl+0
	add r0,r0,#0x0300
	add r0,r0,#0x00D8
	mov r1,#0x03
	str r1,[r0]
	ldr r1,[r0,#-4]
	bic r1,#0xFF000000
	orr r1,#0xCD000000
	str r1,[r0,#-4]
	ldr r1,[r0,#4]
	bic r1,#0x000000FF
	str r1,[r0,#4]
	b hack_done
0:
	.byte 0xC3,0x13,0x38,0xFA,0x4A,0xC1,0xA7,0x20,0x05,0xFB,0x76,0x00,0x18,0xF5,0xC9,0x00

hack_conker:
	@enable RAM address mode for speed hacks
	ldrb_ r0,speedhack_mode
	orr r0,r0,#1
	strb_ r0,speedhack_mode
	b hack_done


wayforward_hacks:
	mov r11,r0
	cmp r0,#0
	@patch _F4 code, either 7E for shantae, or 7B for all other games
	moveq r1,#0x7E
	movne r1,#0x7B
	ldr r2,=_F4
	strb r1,[r2]
	@patch _FC code, either 04 for zapped, or 05 for all other games
	cmp r0,#4
	mov r1,#4
	movne r1,#5
	ldr r2,=_FC_modify
	strb r1,[r2]
	
	
	adr r12,wayforward_misc_addresses
	add r12,r12,r0,lsl#2
	ldrh r1,[r12],#2
	ldr r2,=dmaBaseAddress
	str r1,[r2]
	ldrh r1,[r12],#2
	ldr r2,=XGB_RAM - 0xC000
	add r1,r1,r2
	ldr r2,=rng_hack_ram_address
	str r1,[r2]

	@set DMA MODE to 2 (Wayforward, shantae, etc...)
	mov r1,#2
	ldr r2,=_dmamode
	strb r1,[r2]
	
	adr r3,wayforward_hack_bytes
	adr r4,wayforward_hack_instructions
	adr r5,wayforward_hack_addresses
	mov r1,#20
	mla r5,r0,r1,r5
	
	mov r6,#10
0:
	ldrb r0,[r3],#1
	ldr r1,[r4],#4
	@store to instruction table
	str r1,[r10,r0,lsl#2]
	ldrh r1,[r5],#2
	movs r1,r1
	blne _pokevram
	subs r6,r6,#1
	bgt 0b
	
	cmp r11,#4
	@different RNG for zapped
	ldreq r1,=_EB_2
	streq r1,[r10,#0xEB*4]

hack_done:
hack_unsupported_game:
	ldmfd sp!,{r3-r9,r11,lr}
	bx lr
	
wayforward_hack_bytes:
	.byte 0xFC,0xEB,0xEC,0xFD,0xE3,0xE4,0xDB,0xDD,0xD3,0xF4
	.byte 0,0
wayforward_hack_instructions:
	.word  _FC, _EB, _EC, _FD, _E3, _E4, _DB, _DD, _D3, _F4
wayforward_hack_addresses:
	@      call    rng     memcpy  ai_1    ai_2    ai_3    ai_4    ai_5    ai_6    ai_7
	@shantae
	.short 0x0540, 0x0855, 0x1E33, 0x1314, 0x131E, 0x1334, 0x1342, 0x1362, 0x1378, 0x1383
	@wendy
	.short 0x0540, 0x089C, 0x1E2E, 0x1373, 0x137D, 0x1393, 0x13A1, 0x13C1, 0x13D7, 0x13E2
	@spooked
	.short 0x0540, 0x089C, 0x1E2E, 0x1373, 0x137D, 0x1393, 0x13A1, 0x13C1, 0x13D7, 0x13E2
	@betrayal
	.short 0x0540, 0x084B, 0x1C70, 0x11D2, 0x11DC, 0x11F2, 0x1200, 0x1220, 0x1236, 0x1241
	@zapped  (different RNG)
	.short 0x0440, 0x0822, 0x1E07, 0x13C8, 0x13D2, 0x13E8, 0x0000, 0x1413, 0x1429, 0x1434
	
wayforward_misc_addresses:
	@shantae
	.short 0xC4C0, 0xC381
	@wendy
	.short 0xC4C0, 0xC378
	@spooked
	.short 0xC4C0, 0xC378
	@betrayal
	.short 0xC3C0, 0xC276
	@zapped
	.short 0xC460, 0xC274
	
_pokevram:
	ldr_ r2,memmap_tbl+0
	add r1,r2,r1
	tst r1,#1
	bic r1,r1,#1
	ldrh r2,[r1]
	bne _pokevram_notaligned
	bic r2,r2,#0xFF
	orr r2,r2,r0
	strh r2,[r1]
	bx lr
_pokevram_notaligned:
	bic r2,r2,#0xFF00
	orr r2,r2,r0,lsl#8
	strh r2,[r1]
	bx lr

