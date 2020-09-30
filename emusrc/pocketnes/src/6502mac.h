#ifndef __6502MAC_H__
#define __6502MAC_H__

PSR_N = 0x80000000	@ARM flags
PSR_Z = 0x40000000
PSR_C = 0x20000000
PSR_V = 0x10000000

C = 0x01	@6502 flags
Z = 0x02
I = 0x04
D = 0x08
B = 0x10	@(allways 1 except when IRQ pushes it)
R = 0x20	@(locked at 1)
V = 0x40
N = 0x80


.macro modeFIQ		@Change CPU mode from System to FIQ
	mrs r0,cpsr
	bic r0,r0,#0x0e
	msr cpsr_cf,r0
.endm

.macro modeSystem	@Change CPU mode from FIQ to System
	mrs r0,cpsr
	orr r0,r0,#0x0e
	msr cpsr_cf,r0
.endm

.macro encodePC		@translate from 6502 PC to rom offset
	and r1,m6502_pc,#PRG_BANK_MASK
	ldr r0,[m6502_mmap,r1,lsr#(PRG_BANK_SHIFT-2)]
	str_ r0,lastbank
	add m6502_pc,m6502_pc,r0
.endm

.macro encodeP extra	@pack 6502 flags into r0
	and r0,cycles,#CYC_V+CYC_D+CYC_I+CYC_C
	tst m6502_nz,#PSR_N
	orrne r0,r0,#N				@N
	tst m6502_nz,#0xff
	orreq r0,r0,#Z				@Z
	orr r0,r0,#\extra			@R(&B)
.endm

.macro decodeP	@unpack 6502 flags from r0
	bic cycles,cycles,#CYC_V+CYC_D+CYC_I+CYC_C
	and r1,r0,#V+D+I+C
	orr cycles,cycles,r1		@VDIC
	bic m6502_nz,r0,#0xFD			@r0 is signed
	eor m6502_nz,m6502_nz,#Z
.endm

.macro fetch count
	subs cycles,cycles,#\count*3*CYCLE
	.if DEBUG
	ldr_ pc,fetch_function
	.else
	ldrplb r0,[m6502_pc],#1
	ldrpl pc,[m6502_optbl,r0,lsl#2]
	b timeout_handler
	.endif
.endm

.macro fetch_branch count
	subs cycles,cycles,#\count*3*CYCLE
	.if DEBUG
	ldr_ pc,fetch_function
	.else
	ldrplb r0,[m6502_pc],#1
	ldrpl pc,[m6502_optbl,r0,lsl#2]
	b timeout_handler_branch_taken
	.endif
.endm

.macro fetch_c count	@same as fetch except it adds the Carry (bit 0) also.
						@This is unsafe because writemem can change processor flags. 
						@(It's only safe if it's a write to the zero page or register A)
	sbcs cycles,cycles,#\count*3*CYCLE
	.if DEBUG
	ldr_ pc,fetch_function
	.else
	ldrplb r0,[m6502_pc],#1
	ldrpl pc,[m6502_optbl,r0,lsl#2]
	b timeout_handler
	.endif
.endm

.macro clearcycles
	and cycles,cycles,#CYC_MASK		@Save CPU bits
.endm

.macro readmemabs
	@warning, addy may contain a value up to 0x100FE, so tables need a dummy entry to handle the mirrored ram read
	mvn r1,addy,lsr#PRG_BANK_SHIFT	@get top 3 bits of addy, negate, subtract 1
	adr lr,0f
	ldr pc,[r10,r1,lsl#2]	@in: addy,r1=(addy>>13) (for rom_R)
0:				@out: r0=val (bits 8-31=0 (LSR,ROR,INC,DEC,ASL)), addy preserved for RMW instructions
.endm

.macro readmemzp
	ldrb r0,[cpu_zpage,addy]
.endm

.macro readmemzpi
	ldrb r0,[cpu_zpage,addy,lsr#24]
.endm

.macro readmemzps
	ldrsb m6502_nz,[cpu_zpage,addy]
.endm

.macro readmemimm
	ldrb r0,[m6502_pc],#1
.endm

.macro readmemimms
	ldrsb m6502_nz,[m6502_pc],#1
.endm

.macro readmem
	.if _type == _ABS
		readmemabs
	.endif
	.if _type == _ZP
		readmemzp
	.endif
	.if _type == _ZPI
		readmemzpi
	.endif
	.if _type == _IMM
		readmemimm
	.endif
.endm

.macro readmems
	.if _type == _ABS
		readmemabs
		orr m6502_nz,r0,r0,lsl#24
	.endif
	.if _type == _ZP
		readmemzps
	.endif
	.if _type == _IMM
		readmemimms
	.endif
.endm


.macro writememabs
	@warning, addy may be as high as 0x100FE, so the write mem table gets an extra entry for the overflow entry
	mvn r1,addy,lsr#PRG_BANK_SHIFT
	adr lr,0f
	ldr pc,[m6502_mmap,r1,lsl#2]	@in: addy,r0=val(bits 8-31=?)
0:				@out: r0,r1,r2,addy=?
.endm

.macro writememzp
	strb r0,[cpu_zpage,addy]
.endm

.macro writememzpi
	strb r0,[cpu_zpage,addy,lsr#24]
.endm

.macro writemem
	.if _type == _ABS
		writememabs
	.endif
	.if _type == _ZP
		writememzp
	.endif
	.if _type == _ZPI
		writememzpi
	.endif
.endm
@----------------------------------------------------------------------------

.macro push16		@push r0
	mov r1,r0,lsr#8
	ldr_ r2,m6502_s
	strb r1,[r2],#-1
	orr r2,r2,#0x100
	strb r0,[r2],#-1
	strb_ r2,m6502_s
.endm		@r1,r2=?

.macro push8 x
	ldr_ r2,m6502_s
	strb \x,[r2],#-1
	strb_ r2,m6502_s
.endm		@r2=?

.macro pop16		@pop m6502_pc
	ldr_ r2,m6502_s
	add r2,r2,#2
	strb_ r2,m6502_s
	bic r2,r2,#0x200
	orr r2,r2,#0x100
	ldrb r0,[r2],#-1
	orr r2,r2,#0x100
	ldrb m6502_pc,[r2]
	orr m6502_pc,m6502_pc,r0,lsl#8
.endm		@r0,r1=?

.macro pop8 x
	ldrb_ r2,m6502_s
	add r2,r2,#1
	strb_ r2,m6502_s
	orr r2,r2,#0x100
	ldrsb \x,[r2,cpu_zpage]		@signed for PLA & PLP
.endm	@r2=?

@----------------------------------------------------------------------------
@doXXX: load addy, increment m6502_pc

	@GBLA _type

_IMM	= 1						@immediate
_ZP		= 2						@zero page
_ZPI	= 3						@zero page indexed
_ABS	= 4						@absolute

.macro doABS                           @absolute               $nnnn
	_type	=      _ABS
	ldrb addy,[m6502_pc],#1
	ldrb r0,[m6502_pc],#1
	orr addy,addy,r0,lsl#8
.endm

.macro doAIX                           @absolute indexed X     $nnnn,X
	_type	=      _ABS
	ldrb addy,[m6502_pc],#1
	ldrb r0,[m6502_pc],#1
	orr addy,addy,r0,lsl#8
	add addy,addy,m6502_x,lsr#24
@	bic addy,addy,#0xff0000 @Base Wars needs this
.endm


.macro doAIX_PAGECROSS                           @absolute indexed X     $nnnn,X
	_type	=      _ABS
	ldrb addy,[m6502_pc],#1
	adds r0,m6502_x,addy,lsl#24
	subcs cycles,cycles,#CYCLE*3	@waste a cycle if address crosses a page (for Battletoads)
	ldrb r0,[m6502_pc],#1
	orr addy,addy,r0,lsl#8
	add addy,addy,m6502_x,lsr#24
@	bic addy,addy,#0xff0000 @Base Wars needs this
.endm


.macro doAIY                           @absolute indexed Y     $nnnn,Y
	_type	=      _ABS
	ldrb addy,[m6502_pc],#1
	ldrb r0,[m6502_pc],#1
	orr addy,addy,r0,lsl#8
	add addy,addy,m6502_y,lsr#24
@	bic addy,addy,#0xff0000 @Tecmo Bowl needs this
.endm


.macro doAIY_PAGECROSS                           @absolute indexed Y     $nnnn,Y
	_type	=      _ABS
	ldrb addy,[m6502_pc],#1
	adds r0,m6502_y,addy,lsl#24
	subcs cycles,cycles,#CYCLE*3	@waste a cycle if address crosses a page (for Battletoads)
	ldrb r0,[m6502_pc],#1
	orr addy,addy,r0,lsl#8
	add addy,addy,m6502_y,lsr#24
@	bic addy,addy,#0xff0000 @Tecmo Bowl needs this
.endm

.macro doIMM                           @immediate              #$nn
	_type	=      _IMM
.endm

.macro doIIX                           @indexed indirect X     ($nn,X)
	_type	=      _ABS
	ldrb r0,[m6502_pc],#1
	add r0,m6502_x,r0,lsl#24
	ldrb addy,[cpu_zpage,r0,lsr#24]
	add r0,r0,#0x01000000
	ldrb r1,[cpu_zpage,r0,lsr#24]
	orr addy,addy,r1,lsl#8
.endm

.macro doIIY                           @indirect indexed Y     ($nn),Y
	_type	=      _ABS
	ldrb r0,[m6502_pc],#1
@	.if HAPPY_CPU_TESTER
	cmp r0,#0xFF
	ldrb addy,[r0,cpu_zpage]!
	subeq r0,r0,#0x100
	ldrb r1,[r0,#1]
@	.else
@	ldrb addy,[r0,cpu_zpage]!
@	ldrb r1,[r0,#1]
@	.endif

	orr addy,addy,r1,lsl#8
	add addy,addy,m6502_y,lsr#24
@	bic addy,addy,#0xff0000 @Zelda2 needs this
.endm

.macro doIIY_PAGECROSS                           @indirect indexed Y     ($nn),Y
	_type	=      _ABS
	ldrb r0,[m6502_pc],#1
@	.if HAPPY_CPU_TESTER
	cmp r0,#0xFF
	ldrb addy,[r0,cpu_zpage]!
	subeq r0,r0,#0x100
	ldrb r1,[r0,#1]
@	.else
@	ldrb addy,[r0,cpu_zpage]!
@	ldrb r1,[r0,#1]
@	.endif
	
	adds r2,m6502_y,addy,lsl#24
	subcs cycles,cycles,#3*CYCLE	@waste a cycle if address crosses a page
	
	orr addy,addy,r1,lsl#8
	add addy,addy,m6502_y,lsr#24
@	bic addy,addy,#0xff0000 @Zelda2 needs this
.endm

.macro doZPI							@Zeropage indirect     ($nn)
	_type	=      _ABS
	ldrb r0,[m6502_pc],#1
	ldrb addy,[r0,cpu_zpage]!
	ldrb r1,[r0,#1]
	orr addy,addy,r1,lsl#8
.endm

.macro doZ                             @zero page              $nn
	_type	=      _ZP
	ldrb addy,[m6502_pc],#1
.endm

@.macro doZ2							@zero page              $nn
@	_type	=      _ZP
@	ldrb addy,[m6502_pc],#2			@ugly thing for bbr/bbs
@.endm

.macro doZIX                           @zero page indexed X    $nn,X
	_type	=      _ZP
	ldrb addy,[m6502_pc],#1
	add addy,addy,m6502_x,lsr#24
	and addy,addy,#0xff @Rygar needs this
.endm

.macro doZIXf							@zero page indexed X    $nn,X
	_type	=      _ZPI
	ldrb addy,[m6502_pc],#1
	add addy,m6502_x,addy,lsl#24
.endm

.macro doZIY                           @zero page indexed Y    $nn,Y
	_type	=      _ZP
	ldrb addy,[m6502_pc],#1
	add addy,addy,m6502_y,lsr#24
	and addy,addy,#0xff
.endm

.macro doZIYf							@zero page indexed Y    $nn,Y
	_type	=      _ZPI
	ldrb addy,[m6502_pc],#1
	add addy,m6502_y,addy,lsl#24
.endm

@----------------------------------------------------------------------------

.macro opADC
	readmem
	movs r1,cycles,lsr#1		@get C
	subcs r0,r0,#0x00000100
	adcs m6502_a,m6502_a,r0,ror#8
	mov m6502_nz,m6502_a,asr#24		@NZ
	orr cycles,cycles,#CYC_C+CYC_V	@Prepare C & V
	bicvc cycles,cycles,#CYC_V	@V
.endm

.macro opAND
	readmem
	and m6502_a,m6502_a,r0,lsl#24
	mov m6502_nz,m6502_a,asr#24		@NZ
.endm

.macro opASL_c
	readmem
	 add r0,r0,r0
	 orrs m6502_nz,r0,r0,lsl#24		@NZ
	 orr cycles,cycles,#CYC_C		@Prepare C
	writemem
.endm

.macro opASL
	readmem
	 add r0,r0,r0
	 orrs m6502_nz,r0,r0,lsl#24		@NZ
	 orrcs cycles,cycles,#CYC_C		@Prepare C
	 biccc cycles,cycles,#CYC_C		@writemem may modify the current state.. By huiminghao
	writemem
.endm

.macro opBIT
	readmem
	bic cycles,cycles,#CYC_V		@reset V
	tst r0,#V
	orrne cycles,cycles,#CYC_V		@V
	and m6502_nz,r0,m6502_a,lsr#24	@Z
	orr m6502_nz,m6502_nz,r0,lsl#24	@N
.endm

.macro opCOMP x			@A,X & Y
	readmem
	subs m6502_nz,\x,r0,lsl#24
	mov m6502_nz,m6502_nz,asr#24	@NZ
	orr cycles,cycles,#CYC_C	@Prepare C
.endm

.macro opDEC
	readmem
	sub r0,r0,#1
	orr m6502_nz,r0,r0,lsl#24		@NZ
	writemem
.endm

.macro opEOR
	readmem
	eor m6502_a,m6502_a,r0,lsl#24
	mov m6502_nz,m6502_a,asr#24		@NZ
.endm

.macro opINC
	readmem
	add r0,r0,#1
	orr m6502_nz,r0,r0,lsl#24		@NZ
	writemem
.endm

.macro opLOAD x
	readmems
	mov \x,m6502_nz,lsl#24
.endm

.macro opLSR
	.if _type == _ABS
		readmemabs
		movs r0,r0,lsr#1
		orrcs cycles,cycles,#CYC_C		@Prepare C
		biccc cycles,cycles,#CYC_C
		mov m6502_nz,r0				@Z, (N=0)
		writememabs
	.endif
	.if _type == _ZP
		ldrb m6502_nz,[cpu_zpage,addy]
		movs m6502_nz,m6502_nz,lsr#1	@Z, (N=0)
		orrcs cycles,cycles,#CYC_C		@Prepare C
		biccc cycles,cycles,#CYC_C
		strb m6502_nz,[cpu_zpage,addy]
	.endif
	.if _type == _ZPI
		ldrb m6502_nz,[cpu_zpage,addy,lsr#24]
		movs m6502_nz,m6502_nz,lsr#1	@Z, (N=0)
		orrcs cycles,cycles,#CYC_C		@Prepare C
		biccc cycles,cycles,#CYC_C
		strb m6502_nz,[cpu_zpage,addy,lsr#24]
	.endif
.endm

.macro opLSR_c
	.if _type == _ZP
		ldrb m6502_nz,[cpu_zpage,addy]
		movs m6502_nz,m6502_nz,lsr#1	@Z, (N=0)
		orr cycles,cycles,#CYC_C	@Prepare C
		strb m6502_nz,[cpu_zpage,addy]
	.endif
	.if _type == _ZPI
		ldrb m6502_nz,[cpu_zpage,addy,lsr#24]
		movs m6502_nz,m6502_nz,lsr#1	@Z, (N=0)
		orr cycles,cycles,#CYC_C	@Prepare C
		strb m6502_nz,[cpu_zpage,addy,lsr#24]
	.endif
.endm

.macro opORA
	readmem
	orr m6502_a,m6502_a,r0,lsl#24
	mov m6502_nz,m6502_a,asr#24
.endm

.macro opROL
	readmem
	 movs cycles,cycles,lsr#1		@get C
	 adc r0,r0,r0
	 orrs m6502_nz,r0,r0,lsl#24		@NZ
	 adc cycles,cycles,cycles		@Set C
	writemem
.endm

.macro opROR
	readmem
	 movs cycles,cycles,lsr#1		@get C
	 orrcs r0,r0,#0x100
	 movs r0,r0,lsr#1
	 orr m6502_nz,r0,r0,lsl#24		@NZ
	 adc cycles,cycles,cycles		@Set C
	writemem
.endm

.macro opSBC
	readmem
	movs r1,cycles,lsr#1			@get C
	sbcs m6502_a,m6502_a,r0,lsl#24
	and m6502_a,m6502_a,#0xff000000
	mov m6502_nz,m6502_a,asr#24 		@NZ
	orr cycles,cycles,#CYC_C+CYC_V	@Prepare C & V
	bicvc cycles,cycles,#CYC_V		@V
.endm

.macro opSLO_c
	readmem
	 add r0,r0,r0
	 orrs m6502_a,m6502_a,r0,lsl#24
	 orr m6502_nz,m6502_a,m6502_a,lsr#24		@NZ
	 orr cycles,cycles,#CYC_C		@Prepare C
	writemem
.endm

.macro opSLO
	readmem
	add r0,r0,r0
	orrs m6502_a,m6502_a,r0,lsl#24
	orr m6502_nz,m6502_a,m6502_a,lsr#24		@NZ
	orrcs cycles,cycles,#CYC_C		@Prepare C
	biccc cycles,cycles,#CYC_C
	writemem
.endm

.macro opSTORE x
	mov r0,\x,lsr#24
	writemem
.endm
@----------------------------------------------------
	@.end

#endif
