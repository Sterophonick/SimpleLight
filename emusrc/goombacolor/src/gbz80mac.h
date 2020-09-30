 PSR_n = 0x80000000
 PSR_Z = 0x40000000
 PSR_C = 0x20000000
 PSR_h = 0x10000000


 Z = 0b10000000	@gb-Z80 flags
 n = 0b01000000	@was the last opcode + or -
 h = 0b00100000	@half carry
 C = 0b00010000	@carry


	.macro encodePC		@translate gb_pc from GB-Z80 PC to rom offset
	and r1,gb_pc,#0xF000
	adr_ r2,memmap_tbl
	ldr r0,[r2,r1,lsr#10]
	str_ r0,lastbank
	add gb_pc,gb_pc,r0
	.endm

	.macro encodePC_afterpush16		@push16 already does "adr r2,memmap_tbl"
	and r1,gb_pc,#0xF000
@	adr r2,memmap_tbl
	ldr r0,[r2,r1,lsr#10]
	str_ r0,lastbank
	add gb_pc,gb_pc,r0
	.endm

	.macro encodeFLG		@pack GB-Z80 flags into r0
	and r0,gb_flg,#0xA0000000	@nC
	and r1,gb_flg,#0x50000000	@Zh
	mov r1,r1,lsr#23
	orr r0,r1,r0,lsr#25			@N
	.endm

	.macro decodeFLG		@unpack GB-Z80 flags from r0
	and gb_flg,r0,#0xA0			@Zh
	and r0,r0,#0x50				@nC
	mov r0,r0,lsl#25
	orr gb_flg,r0,gb_flg,lsl#23	@N
	.endm


	.macro fetch count
 .if PROFILE
	subs cycles,cycles,#\count*CYCLE
	b_long fetch_profile
	.pool
 .else
	subs cycles,cycles,#\count*CYCLE
	ldrplb r0,[gb_pc],#1
	ldrpl pc,[gb_optbl,r0,lsl#2]
	ldr_ pc,nexttimeout
 .endif
	.endm

	.macro readmemHL
	mov addy,gb_hl,lsr#16
	readmem
	.endm

	.macro readmem
	mvn r1,addy,lsr#12
	adr lr,0f
	ldr pc,[r10,r1,lsl#2]	@in: addy,r1=addy&0xE000 (for rom_R)
0:				@out: r0=val (bits 8-31=0 ), addy preserved for RMW instructions
	.endm

	.macro writememHL
	mov addy,gb_hl,lsr#16
	writemem
	.endm

	.macro writemem
	and r1,addy,#0xF000
	adr_ r2,writemem_tbl
	adr lr,0f
	ldr pc,[r2,r1,lsr#10]	@in: addy,r0=val(bits 8-31=?)
0:				@out: r0,r1,r2,addy=?
	.endm

@----------------------------------------------------------------------------

@note: stack writes to SRAM do not properly save to GBA sram as well.

	.macro push16		@push r0
	subs gb_sp,gb_sp,#0x00020000	@use "negative flag" as indicator we are writing to ROM
	and r1,gb_sp,#0xF0000000
	adr_ r2,memmap_tbl
	ldr r2,[r2,r1,lsr#26]
	strmib r0,[r2,gb_sp,lsr#16]		@reject rom write
	adds addy,gb_sp,#0x00010000
	mov r0,r0,lsr#8
	strmib r0,[r2,addy,lsr#16]		@reject rom write
	tstmi r1,#0x60000000	@just to solve some games
	bleq vram_W2			@that use push16 to write to vram
	.endm		@r1,r2=?

	.macro push16_novram		@push r0 with no VRAM check (because who would fill VRAM with PC?)
	subs gb_sp,gb_sp,#0x00020000	@use "negative flag" as indicator we are writing to ROM
	and r1,gb_sp,#0xF0000000
	adr_ r2,memmap_tbl
	ldr addy,[r2,r1,lsr#26]
	strmib r0,[addy,gb_sp,lsr#16]		@reject rom write
	adds r1,gb_sp,#0x00010000
	mov r0,r0,lsr#8
	strmib r0,[addy,r1,lsr#16]		@reject rom write
	.endm		@r1,r2=?

	.macro pop16 x		@pop BC,DE,HL,PC
	and r1,gb_sp,#0xF0000000
	adr_ r2,memmap_tbl
	ldr r1,[r2,r1,lsr#26]
	ldrb \x,[r1,gb_sp,lsr#16]
	add gb_sp,gb_sp,#0x00010000
	ldrb r0,[r1,gb_sp,lsr#16]
	add gb_sp,gb_sp,#0x00010000
	orr \x,\x,r0,lsl#8
	.endm		@r0,r1=?

	.macro popAF			@pop AF
	and r1,gb_sp,#0xF0000000
	adr_ r2,memmap_tbl
	ldr r1,[r2,r1,lsr#26]
	ldrb r0,[r1,gb_sp,lsr#16]
	add gb_sp,gb_sp,#0x00010000
	ldrb gb_a,[r1,gb_sp,lsr#16]
	add gb_sp,gb_sp,#0x00010000
	mov gb_a,gb_a,lsl#24
	.endm		@r0=flags,r1=?
@----------------------------------------------------------------------------

	.macro opADC
	msr cpsr_f,gb_flg				@get C
	subcs r0,r0,#0x100
	eor r1,gb_a,r0,ror#8			@prepare for check of half carry.
	adcs gb_a,gb_a,r0,ror#8
	eor gb_flg,r1,gb_a
	and gb_flg,gb_flg,#PSR_h		@h
	orrcs gb_flg,gb_flg,#PSR_C		@C
	orreq gb_flg,gb_flg,#PSR_Z		@Z
	.endm

	.macro opADCA
	msr cpsr_f,gb_flg				@get C
	orrcs gb_a,gb_a,#0x00800000
	adds gb_a,gb_a,gb_a
	and gb_flg,gb_a,#PSR_h			@h
	orrcs gb_flg,gb_flg,#PSR_C		@C
	orreq gb_flg,gb_flg,#PSR_Z		@Z
	fetch 4
	.endm

	.macro opADCH x
	mov r0,\x,lsr#24
	opADC
	fetch 4
	.endm

	.macro opADCL x
	mov r0,\x,lsr#16
	and r0,r0,#0xFF
	opADC
	fetch 4
	.endm

	.macro opADCb
	opADC
	fetch 8
	.endm
@---------------------------------------

	.macro opADD16 x
	and gb_flg,gb_flg,#PSR_Z		@save zero, clear n
	eor r1,gb_hl,\x
	adds gb_hl,gb_hl,\x
	eor r1,r1,gb_hl
	orrcs gb_flg,gb_flg,#PSR_C
	tst r1,#0x10000000				@h, correct.
	orrne gb_flg,gb_flg,#PSR_h
	fetch 8
	.endm

	.macro opADD16_2
	and gb_flg,gb_flg,#PSR_Z		@save zero, clear n
	adds gb_hl,gb_hl,gb_hl
	orrcs gb_flg,gb_flg,#PSR_C
	tst gb_hl,#0x10000000			@h, correct.
	orrne gb_flg,gb_flg,#PSR_h
	fetch 8
	.endm
@---------------------------------------

	.macro opADD x,y
	eor r1,gb_a,\x,lsl#\y
	adds gb_a,gb_a,\x,lsl#\y
	eor gb_flg,r1,gb_a
	and gb_flg,gb_flg,#PSR_h		@h
	orrcs gb_flg,gb_flg,#PSR_C		@C
	orreq gb_flg,gb_flg,#PSR_Z		@Z
	.endm

	.macro opADDA
	adds gb_a,gb_a,gb_a
	and gb_flg,gb_a,#PSR_h			@h
	orrcs gb_flg,gb_flg,#PSR_C		@C
	orreq gb_flg,gb_flg,#PSR_Z		@Z
	fetch 4
	.endm

	.macro opADDH x
	and r0,\x,#0xFF000000
	opADD r0,0
	fetch 4
	.endm

	.macro opADDL x
	opADD \x,8
	fetch 4
	.endm

	.macro opADDb
	opADD r0,24
	fetch 8
	.endm
@---------------------------------------

	.macro opAND x,y
	mov gb_flg,#PSR_h			@set h, clear C & n.
	ands gb_a,gb_a,\x,lsl#\y
	orreq gb_flg,gb_flg,#PSR_Z	@Z
	.endm

	.macro opANDA
	opAND gb_a,0
	fetch 4
	.endm

	.macro opANDH x
	opAND \x,0
	fetch 4
	.endm

	.macro opANDL x
	opAND \x,8
	fetch 4
	.endm

	.macro opANDb
	opAND r0,24
	fetch 8
	.endm
@---------------------------------------

	.macro opBIT x
	mov r0,r0,lsr#3
	and gb_flg,gb_flg,#PSR_C	@keep C
	orr gb_flg,gb_flg,#PSR_h	@set h
	tst \x,r1,lsl r0			@r0 0x08-0x0F
	orreq gb_flg,gb_flg,#PSR_Z	@Z
	.endm

	.macro opBITH x
	mov r1,#0x00010000
	opBIT \x
	.endm

	.macro opBITL x
	mov r1,#0x00000100
	opBIT \x
	.endm
@---------------------------------------

	.macro opCP x,y
	eor r1,gb_a,\x,lsl#\y			@prepare for check of half carry.
	subs r0,gb_a,\x,lsl#\y
	eor gb_flg,r1,r0
	and gb_flg,gb_flg,#PSR_h		@h
	orr gb_flg,gb_flg,#PSR_n		@n
	orrcc gb_flg,gb_flg,#PSR_C		@C
	orreq gb_flg,gb_flg,#PSR_Z		@Z
	.endm

	.macro opCPA
	mov gb_flg,#PSR_n|PSR_Z			@set n & Z
	fetch 4
	.endm

	.macro opCPH x
	and r0,\x,#0xFF000000
	opCP r0,0
	fetch 4
	.endm

	.macro opCPL x
	opCP \x,8
	fetch 4
	.endm

	.macro opCPb
	opCP r0,24
	fetch 8
	.endm
@---------------------------------------

	.macro opDEC8 x
	and gb_flg,gb_flg,#PSR_C	@save carry
	orr gb_flg,gb_flg,#PSR_n	@set n
	tst \x,#0x0f000000			@h
	orreq gb_flg,gb_flg,#PSR_h
	subs \x,\x,#0x01000000
	orreq gb_flg,gb_flg,#PSR_Z
	.endm

	.macro opDEC8A
	opDEC8 gb_a
	fetch 4
	.endm

	.macro opDEC8H x
	and gb_flg,gb_flg,#PSR_C	@save carry
	orr gb_flg,gb_flg,#PSR_n	@set n
	tst \x,#0x0f000000			@h
	orreq gb_flg,gb_flg,#PSR_h
	sub \x,\x,#0x01000000
	tst \x,#0xff000000			@Z
	orreq gb_flg,gb_flg,#PSR_Z
	fetch 4
	.endm

	.macro opDEC8L x
	mov r0,\x,lsl#8
	opDEC8 r0
	and \x,\x,#0xFF000000
	orr \x,\x,r0,lsr#8
	fetch 4
	.endm

	.macro opDEC8b
	and gb_flg,gb_flg,#PSR_C	@save carry
	orr gb_flg,gb_flg,#PSR_n	@set n
	tst r0,#0x0f				@h
	orreq gb_flg,gb_flg,#PSR_h
	subs r0,r0,#0x01
@	ands r0,r0,#0xff			;Not needed!?!
	orreq gb_flg,gb_flg,#PSR_Z	@Z
	.endm

	.macro opDEC16 x
	sub \x,\x,#0x00010000
	.endm
@---------------------------------------

	.macro opINC8 x
	and gb_flg,gb_flg,#PSR_C	@save carry, clear n
	adds \x,\x,#0x01000000
	orreq gb_flg,gb_flg,#PSR_Z
	tst \x,#0x0f000000			@h
	orreq gb_flg,gb_flg,#PSR_h
	.endm

	.macro opINC8A
	opINC8 gb_a
	fetch 4
	.endm

	.macro opINC8H x
	and gb_flg,gb_flg,#PSR_C	@save carry, clear n
	add \x,\x,#0x01000000
	tst \x,#0xff000000			@Z
	orreq gb_flg,gb_flg,#PSR_Z
	tst \x,#0x0f000000			@h
	orreq gb_flg,gb_flg,#PSR_h
	fetch 4
	.endm

	.macro opINC8L x
	mov r0,\x,lsl#8
	opINC8 r0
	and \x,\x,#0xFF000000
	orr \x,\x,r0,lsr#8
	fetch 4
	.endm

	.macro opINC8b
	and gb_flg,gb_flg,#PSR_C	@save carry, clear n
	add r0,r0,#0x01
	ands r0,r0,#0xff			@Z
	orreq gb_flg,gb_flg,#PSR_Z
	tst r0,#0x0f				@h
	orreq gb_flg,gb_flg,#PSR_h
	.endm

	.macro opINC16 x
	add \x,\x,#0x00010000
	.endm
@---------------------------------------

	.macro opLDIM16
	ldrb r0,[gb_pc],#1
	ldrb r1,[gb_pc],#1
	orr r0,r0,r1,lsl#8
	.endm

	.macro opLDIM8H x
	ldrb r0,[gb_pc],#1
	and \x,\x,#0x00ff0000
	orr \x,\x,r0,lsl#24
	fetch 8
	.endm

	.macro opLDIM8L x
	ldrb r0,[gb_pc],#1
	and \x,\x,#0xff000000
	orr \x,\x,r0,lsl#16
	fetch 8
	.endm
@---------------------------------------

	.macro opOR x,y
	mov gb_flg,#0				@clear flags.
	orrs gb_a,gb_a,\x,lsl#\y
	orreq gb_flg,gb_flg,#PSR_Z	@Z
	.endm

	.macro opORA
	opOR gb_a,0
	fetch 4
	.endm

	.macro opORH x
	and r0,\x,#0xFF000000
	opOR r0,0
	fetch 4
	.endm

	.macro opORL x
	opOR \x,8
	fetch 4
	.endm

	.macro opORb
	opOR r0,24
	fetch 8
	.endm
@---------------------------------------

	.macro opRES x
	mov r0,r0,lsr#3
	bic \x,\x,r1,lsl r0		@r0 0x10-0x17
	.endm

	.macro opRESH x
	mov r1,#0x00000100
	opRES \x
	.endm

	.macro opRESL x
	mov r1,#0x00000001
	opRES \x
	.endm
@---------------------------------------

	.macro opRL x
	tst gb_flg,#PSR_C				@check C
	orrne \x,\x,#0x00800000
	adds \x,\x,\x
	mrs gb_flg,cpsr					@C & Z
	and gb_flg,gb_flg,#PSR_Z|PSR_C	@only keep C & Z
	.endm

	.macro opRLH x
	and r0,\x,#0xFF000000
	opRL r0
	and \x,\x,#0x00FF0000
	orr \x,\x,r0
	fetch 8
	.endm

	.macro opRLL x
	mov r0,\x,lsl#8
	opRL r0
	and \x,\x,#0xFF000000
	orr \x,\x,r0,lsr#8
	fetch 8
	.endm

	.macro opRLb
	mov r0,r0,lsl#24
	opRL r0
	mov r0,r0,lsr#24
	.endm
@---------------------------------------

	.macro opRLC x
	mov gb_flg,#0				@clear flags
	adds \x,\x,\x
	orrcs gb_flg,gb_flg,#PSR_C	@set C
	orrcss \x,\x,#0x01000000
	orreq gb_flg,gb_flg,#PSR_Z	@set Z
	.endm

	.macro opRLCH x
	and r0,\x,#0xFF000000
	opRLC r0
	and \x,\x,#0x00FF0000
	orr \x,\x,r0
	fetch 8
	.endm

	.macro opRLCL x
	mov r0,\x,lsl#8
	opRLC r0
	and \x,\x,#0xFF000000
	orr \x,\x,r0,lsr#8
	fetch 8
	.endm

	.macro opRLCb
	mov r0,r0,lsl#24
	opRLC r0
	mov r0,r0,lsr#24
	.endm
@---------------------------------------

	.macro opRR x
	movs gb_flg,gb_flg,lsr#30	@get C, clear flags
	mov \x,\x,rrx
	tst \x,#0x00800000
	orrne gb_flg,gb_flg,#PSR_C
	ands \x,\x,#0xFF000000
	orreq gb_flg,gb_flg,#PSR_Z
	.endm

	.macro opRRH x
	and r0,\x,#0xFF000000
	opRR r0
	and \x,\x,#0x00FF0000
	orr \x,\x,r0
	fetch 8
	.endm

	.macro opRRL x
	mov r0,\x,lsl#8
	opRR r0
	and \x,\x,#0xFF000000
	orr \x,\x,r0,lsr#8
	fetch 8
	.endm

	.macro opRRb
	mov r0,r0,lsl#24
	opRR r0
	mov r0,r0,lsr#24
	.endm
@---------------------------------------

	.macro opRRC x,y
	mov gb_flg,#0				@clear flags
	movs \x,\x,lsr#\y
	orrcs gb_flg,gb_flg,#PSR_C	@set C
	orrcss \x,\x,#0x00000080
	orreq gb_flg,gb_flg,#PSR_Z	@set Z
	.endm

	.macro opRRCA
	opRRC gb_a,25
	mov gb_a,gb_a,lsl#24
	fetch 8
	.endm

	.macro opRRCH x
	and r0,\x,#0xFF000000
	opRRC r0,25
	and \x,\x,#0x00FF0000
	orr \x,\x,r0,lsl#24
	fetch 8
	.endm

	.macro opRRCL x
	and r0,\x,#0x00FF0000
	opRRC r0,17
	and \x,\x,#0xFF000000
	orr \x,\x,r0,lsl#16
	fetch 8
	.endm

	.macro opRRCb
	opRRC r0,1
	.endm
@---------------------------------------

	.macro opSBC x,y
	eor gb_flg,gb_flg,#PSR_C		@invert C.
	movs gb_flg,gb_flg,lsr#30		@get C, clear flags.
	eor r1,gb_a,\x,lsl#\y			@prepare for check of half carry.
	sbcs gb_a,gb_a,\x,lsl#\y
	eor gb_flg,r1,gb_a
	and gb_flg,gb_flg,#PSR_h		@h
	orr gb_flg,gb_flg,#PSR_n		@n
	orrcc gb_flg,gb_flg,#PSR_C		@C
	ands gb_a,gb_a,#0xFF000000
	orreq gb_flg,gb_flg,#PSR_Z		@Z
	.endm

	.macro opSBCA
	movs gb_flg,gb_flg,lsr#30		@get C.
	movcc gb_a,#0x00000000
	movcs gb_a,#0xFF000000
	movcc gb_flg,#PSR_n+PSR_Z
	movcs gb_flg,#PSR_n+PSR_C+PSR_h
	fetch 4
	.endm

	.macro opSBCH x
	and r0,\x,#0xFF000000
	opSBC r0,0
	fetch 4
	.endm

	.macro opSBCL x
	opSBC \x,8
	fetch 4
	.endm

	.macro opSBCb
	opSBC r0,24
	fetch 8
	.endm
@---------------------------------------

	.macro opSET x
	mov r0,r0,lsr#3
	and r0,r0,#7
	orr \x,\x,r1,lsl r0		@r0 0-7
	.endm

	.macro opSETH x
	mov r1,#0x01000000
	opSET \x
	.endm

	.macro opSETL x
	mov r1,#0x00010000
	opSET \x
	.endm
@---------------------------------------

	.macro opSLA x,y,z
	movs \y,\x,lsl#\z
	mrs gb_flg,cpsr          		@C & Z
	and gb_flg,gb_flg,#PSR_Z|PSR_C	@only keep C & Z
	.endm

	.macro opSLAA
	opSLA gb_a,gb_a,1
	fetch 8
	.endm

	.macro opSLAH x
	and r0,\x,#0xFF000000
	opSLA r0,r0,1
	and \x,\x,#0x00FF0000
	orr \x,\x,r0
	fetch 8
	.endm

	.macro opSLAL x
	opSLA \x,r0,9
	and \x,\x,#0xFF000000
	orr \x,\x,r0,lsr#8
	fetch 8
	.endm

	.macro opSLAb
	opSLA r0,r0,25
	mov r0,r0,lsr#24
	.endm
@---------------------------------------

	.macro opSRA x
	movs r0,\x,asr#25
	mrs gb_flg,cpsr          		@C & Z
	and gb_flg,gb_flg,#PSR_Z|PSR_C	@only keep C & Z
	.endm

	.macro opSRAA
	opSRA gb_a
	mov gb_a,r0,lsl#24
	fetch 8
	.endm

	.macro opSRAH x
	opSRA \x
	and \x,\x,#0x00FF0000
	orr \x,\x,r0,lsl#24
	fetch 8
	.endm

	.macro opSRAL x
	mov r0,\x,lsl#8
	opSRA r0
	and r0,r0,#0xff
	and \x,\x,#0xFF000000
	orr \x,\x,r0,lsl#16
	fetch 8
	.endm

	.macro opSRAb
	mov r0,r0,lsl#24
	opSRA r0
@	and r0,r0,#0xff
	.endm
@---------------------------------------

	.macro opSRLx x,y,z
	movs \y,\x,lsr#\z
	mrs gb_flg,cpsr          		@C & Z
	and gb_flg,gb_flg,#PSR_Z|PSR_C	@only keep C & Z
	.endm

	.macro opSRLA
	opSRLx gb_a,gb_a,25
	mov gb_a,gb_a,lsl#24
	fetch 8
	.endm

	.macro opSRLH x
	opSRLx \x,r0,25
	and \x,\x,#0x00FF0000
	orr \x,\x,r0,lsl#24
	fetch 8
	.endm

	.macro opSRLL x
	and r0,\x,#0x00FF0000
	opSRLx r0,r0,17
	and \x,\x,#0xFF000000
	orr \x,\x,r0,lsl#16
	fetch 8
	.endm
@---------------------------------------

	.macro opSUB x,y
	eor r1,gb_a,\x,lsl#\y
	subs gb_a,gb_a,\x,lsl#\y
	eor gb_flg,r1,gb_a
	and gb_flg,gb_flg,#PSR_h		@h
	orr gb_flg,gb_flg,#PSR_n		@n
	orrcc gb_flg,gb_flg,#PSR_C		@C
	orreq gb_flg,gb_flg,#PSR_Z		@Z
	.endm

	.macro opSUBA
	mov gb_a,#0
	mov gb_flg,#PSR_n|PSR_Z			@n & Z
	fetch 4
	.endm

	.macro opSUBH x
	and r0,\x,#0xFF000000
	opSUB r0,0
	fetch 4
	.endm

	.macro opSUBL x
	opSUB \x,8
	fetch 4
	.endm

	.macro opSUBb
	opSUB r0,24
	fetch 8
	.endm
@---------------------------------------

	.macro opSWAP x
	mov \x,\x,ror#28
	orr \x,\x,\x,lsl#24
	ands \x,\x,#0xFF000000
	mrs gb_flg,cpsr          	@Z
	and gb_flg,gb_flg,#PSR_Z	@only keep Z
	.endm

	.macro opSWAPA
	opSWAP gb_a
	fetch 8
	.endm

	.macro opSWAPL x
	and r0,\x,#0x00FF0000		@mask low to r0
	and \x,\x,#0xFF000000		@mask out high
	mov r0,r0,ror#20
	orrs r0,r0,r0,lsl#24
	mrs gb_flg,cpsr          	@Z
	and gb_flg,gb_flg,#PSR_Z	@only keep Z
	orr \x,\x,r0,lsr#8
	fetch 8
	.endm

	.macro opSWAPH x
	and r0,\x,#0xFF000000		@mask high to r0
	opSWAP r0
	and \x,\x,#0x00FF0000		@mask out low
	orr \x,\x,r0
	fetch 8
	.endm

	.macro opSWAPb
	mov r0,r0,ror#4
	orr r0,r0,r0,lsl#24
	movs r0,r0,lsr#24
	mrs gb_flg,cpsr          	@Z
	and gb_flg,gb_flg,#PSR_Z	@only keep Z
	.endm
@---------------------------------------

	.macro opXOR x,y
	mov gb_flg,#0				@clear flags.
	eors gb_a,gb_a,\x,lsl#\y
	orreq gb_flg,gb_flg,#PSR_Z	@Z
	.endm

	.macro opXORA
	mov gb_a,#0					@clear A.
	mov gb_flg,#PSR_Z			@Z
	fetch 4
	.endm

	.macro opXORH x
	and r0,\x,#0xFF000000
	opXOR r0,0
	fetch 4
	.endm

	.macro opXORL x
	opXOR \x,8
	fetch 4
	.endm

	.macro opXORb
	opXOR r0,24
	fetch 8
	.endm
@---------------------------------------
	@.end
