 .align
 .pool
 .text
 .align
 .pool

#include "../equates.h"

#if 0
Memory map:

    5000-5FFF = Registers and copy protection
    6000-7FFF = PRG RAM (battery backed)
    8000-FFFF = PRG ROM (switchable in 32k banks) 

Registers: (Address is masked with 0x7300)

    5200 = [....xxxx] xxxx = High nibble of 32k PRG page
    5000 = [c...xxxx] xxxx = Low nibble of 32k PRG page. If the most significant bit of this register is set, it does automatic CHR RAM switching at scanline 128. The exact way this works is unknown.
        When turned on, both 4K CHR RAM banks 0000-0FFF and 1000-1FFF map to 0000-0FFF for scanline 240 until scanline 128. Then at scanline 128, both 4K CHR banks point to 1000-1FFF. 
    5100 = When set to 6, sets the 32K PRG bank to 3. Further writes to 5200 or 5000 change the bank back to normal. 

Copy protection: (Address is masked with 0x7300, except for 5101)

    5300 = Value of security register
    5101 = If the value of this register is changed from nonzero to zero, "trigger" is toggled (XORed with 1). Initial value of this register is 1, initial value of "trigger" is 0. 

Reading: (Address is masked with 0x7700)

    5100 = Returns value of 5300
    5500 = If "trigger" is 1, returns value of 5300, otherwise returns 0 
#endif    
    prgpage = mapperdata + 0
    chrmode = mapperdata + 1
    security = mapperdata + 2
    security2 = mapperdata + 3
    trigger = mapperdata + 4
    
	
	
	
	global_func mapper163init
	global_func Mapper163HalfScreenHandler
@----------------------------------------------------------------------------
mapper163init:
@----------------------------------------------------------------------------
	.word void,void,void,void
	adr r0,mapper_163_write
	ldr r1,=empty_io_w_hook
	str r0,[r1]
	adr r0,mapper_163_read
	ldr r1,=empty_io_r_hook
	str r0,[r1]
	mov r0,#1
	strb_ r0,security2
	bx lr

mapper_163_write:
	cmp r12,#0x5000
	bxlt lr
	and r1,r12,#0x0300
	ldr pc,[pc,r1,lsr#6]
	nop
	.word WriteLowNibble
	.word WriteSecurityLow
	.word WriteHighNibble
	.word WriteSecurityHigh
WriteLowNibble:
	ldrb_ r2,chrmode
	and r1,r0,#0x80
	eors r2,r2,r1
	strneb_ r1,chrmode
	
	ldrb_ r1,prgpage
	bic r1,r1,#0x0F
	and r2,r0,#0x0F
	orr r0,r1,r2
	strb_ r0,prgpage
	
	beq map89ABCDEF_
ChangeChrMode:
	stmfd sp!,{lr}
	bl map89ABCDEF_
	ldrb_ r0,chrmode
	ands r0,r0,#0x80
	
	beq ChrSwitchingOff
ChrSwitchingOn:
	bl InstallNextTimeout
	ldmfd sp!,{pc}
ChrSwitchingOff:
	mov r0,#0
	bl chr01234567_

	@cancel timeouts
	ldr r12,=_mapper_timeout
	bl remove_timeout
	ldmfd sp!,{pc}
	
WriteSecurityLow:
	ldr r1,=0x5101
	cmp r12,r1
	beq WriteTrigger
	cmp r0,#6
	bxne lr
	b map89ABCDEF_rshift_   @set bank to 3 without changing "prgpage" variable
WriteHighNibble:
	ldrb_ r1,prgpage
	bic r1,r1,#0xF0
	mov r2,r0,lsl#4
	orr r0,r1,r2
	strb_ r0,prgpage
	b map89ABCDEF_
WriteSecurityHigh:
	strb_ r0,security
	bx lr

WriteTrigger:
	@if security2 changes from nonzero to zero, toggle trigger
	ldrb_ r1,security2
	strb_ r0,security2
	movs r1,r1
	bxeq lr
	movs r0,r0
	bxne lr
toggleTrigger:
	ldrb_ r1,trigger
	eor r1,r1,#1
	strb_ r1,trigger
	bx lr

mapper_163_read:
	@Need to preserve r12 (addy)
	cmp r12,#0x5100
	blt void
	and r1,r12,#0x0700
	cmp r1,#0x100
	beq ReadSecurityLow
	cmp r1,#0x500
	bne void
ReadSecurityHigh:
	ldrb_ r0,trigger
	movs r0,r0
	bxeq lr
	@falls through to ReadSecurityLow
ReadSecurityLow:	
	ldrb_ r0,security
	bx lr

Mapper163HalfScreenHandler:
	bl InstallNextTimeout
	b _GO
	
InstallNextTimeout:
	@get scanline
	stmfd sp!,{lr}
	bl get_scanline_2
	
	@are we in range of 128 to 240?
	cmp r12,#128
	blt tophalf
	cmp r12,#240
	bge tophalf2
bottomhalf:
	@bottom half: 1000-1FFF
	mov r0,#1
	bl chr0123_
	mov r0,#1
	bl chr4567_
	
	@next timeout at scanline 240  (241)
	ldr_ r1,frame_timestamp
	ldr_ r0,timestamp_mult
	mov r2,#241
	mul r2,r0,r2
	add r1,r1,r2,lsr#4
	b 0f
tophalf2:
	@top half: 0000-0FFF
	@bottom half: 1000-1FFF
	mov r0,#0
	bl chr0123_
	mov r0,#0
	bl chr4567_
	
	@next timeout at scanline 128
	ldr_ r1,next_frame_timestamp
	ldr_ r0,timestamp_mult
	add r0,r0,r0,lsl#7
	add r1,r1,r0,lsr#4
	b 0f
tophalf:
	@top half: 0000-0FFF
	mov r0,#0
	bl chr0123_
	mov r0,#0
	bl chr4567_
	
	@next timeout at scanline 128
	ldr_ r1,frame_timestamp
	ldr_ r0,timestamp_mult
	add r0,r0,r0,lsl#7
	add r1,r1,r0,lsr#4
0:
	adr r0,Mapper163HalfScreenHandler
	
	ldr r12,=_mapper_timeout
	bl replace_timeout_2
	ldmfd sp!,{pc}

RemoveTimeout:
	ldr r12,=_mapper_timeout
	b remove_timeout
