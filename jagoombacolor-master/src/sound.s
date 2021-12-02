@	#include "equates.h"
@	#include "lcd.h"

	global_func Sound_reset
	global_func _FF10W
	global_func _FF11W
	global_func _FF12W
	global_func _FF13W
	global_func _FF14W
	global_func _FF16W
	global_func _FF17W
	global_func _FF18W
	global_func _FF19W
	global_func _FF1AW
	global_func _FF1BW
	global_func _FF1CW
	global_func _FF1DW
	global_func _FF1EW
	global_func _FF20W
	global_func _FF21W
	global_func _FF22W
	global_func _FF23W
	global_func _FF24W
	global_func _FF25W
	global_func _FF26W
	global_func _FF30W

	global_func _FF10R
	global_func _FF11R
	global_func _FF12R
	global_func _FF13R
	global_func _FF14R
	global_func _FF16R
	global_func _FF17R
	global_func _FF18R
	global_func _FF19R
	global_func _FF1AR
	global_func _FF1BR
	global_func _FF1CR
	global_func _FF1DR
	global_func _FF1ER
	global_func _FF20R
	global_func _FF21R
	global_func _FF22R
	global_func _FF23R
	global_func _FF24R
	global_func _FF25R
	global_func _FF26R
	global_func _FF30R
 .align
 .pool
 .text
 .align
 .pool

@----------------------------------------------------------------------------
Sound_reset:
@----------------------------------------------------------------------------
	mov r1,#REG_BASE

	ldr r0,=0x0002F377		@stop all channels, output ratio=full range. NR50,NR51 & GBA mixer
	str r0,[r1,#REG_SGCNT_L]
	mov r0,#0xF1
	strh r0,[r1,#REG_SGCNT_X]	@sound master enable. NR52


	mov r0,#0x0000
	strh r0,[r1,#REG_SG1CNT_L]	@NR10
	strh r0,[r1,#REG_SG1CNT_H]	@NR11,NR12, should read 0xF3BF
	strh r0,[r1,#REG_SG1CNT_X]	@(NR13),NR14, should read 0xBF00

	strh r0,[r1,#REG_SG2CNT_L]	@NR21,NR22
	strh r0,[r1,#REG_SG2CNT_H]	@NR24

	strh r0,[r1,#REG_SG3CNT_L]	@NR30, should read 0x7F
	strh r0,[r1,#REG_SG3CNT_H]	@NR31,NR32 should read 0x9FFF
	strh r0,[r1,#REG_SG3CNT_X]	@NR33, should read 0xBF

	strh r0,[r1,#REG_SG4CNT_L]	@NR41,NR42
	strh r0,[r1,#REG_SG4CNT_H]	@NR43,NR44

	mov pc,lr

@----------------------------------------------------------------------------
_FF10W:@		NR10 - Channel 1 Sweep register
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG1CNT_L]
	mov pc,lr
@----------------------------------------------------------------------------
_FF11W:@		NR11 - Channel 1 Sound length/Wave pattern duty
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG1CNT_H]
	strb_ r0,sound_shadow+0
	mov pc,lr
@----------------------------------------------------------------------------
_FF12W:@		NR12 - Channel 1 Volume Envelope
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG1CNT_H+1]
	mov pc,lr
@----------------------------------------------------------------------------
_FF13W:@		NR13 - Channel 1 Frequency lo
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG1CNT_X]
	strb_ r0,sound_shadow+1
	mov pc,lr
@----------------------------------------------------------------------------
_FF14W:@		NR14 - Channel 1 Frequency hi
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG1CNT_X+1]
	strb_ r0,sound_shadow+2
	mov pc,lr

@----------------------------------------------------------------------------
_FF16W:@		NR21 - Channel 2 Sound Length/Wave Pattern Duty
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG2CNT_L]
	strb_ r0,sound_shadow+3
	mov pc,lr
@----------------------------------------------------------------------------
_FF17W:@		NR22 - Channel 2 Volume Envelope
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG2CNT_L+1]
	mov pc,lr
@----------------------------------------------------------------------------
_FF18W:@		NR23 - Channel 2 Frequency lo
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG2CNT_H]
	strb_ r0,sound_shadow+4
	mov pc,lr
@----------------------------------------------------------------------------
_FF19W:@		NR24 - Channel 2 Frequency hi
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG2CNT_H+1]
	strb_ r0,sound_shadow+5
	mov pc,lr

@----------------------------------------------------------------------------
_FF1AW:@		NR30 - Channel 3 Sound on/off
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	and r0,r0,#0x80
	orr r0,r0,r0,lsr#1		@also change wave bank when turning on/off sound.
	strb r0,[addy,#REG_SG3CNT_L]
	mov pc,lr
@----------------------------------------------------------------------------
_FF1BW:@		NR31 - Channel 3 Sound Length
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG3CNT_H]
	strb_ r0,sound_shadow+6
	mov pc,lr
@----------------------------------------------------------------------------
_FF1CW:@		NR32 - Channel 3 Select output level
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	and r0,r0,#0x60
	strb r0,[addy,#REG_SG3CNT_H+1]
	mov pc,lr
@----------------------------------------------------------------------------
_FF1DW:@		NR33 - Channel 3 Frequency lo
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG3CNT_X]
	strb_ r0,sound_shadow+7
	mov pc,lr
@----------------------------------------------------------------------------
_FF1EW:@		NR34 - Channel 3 Frequency hi
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG3CNT_X+1]
	strb_ r0,sound_shadow+8
	mov pc,lr

@----------------------------------------------------------------------------
_FF30W:@		Channel 3 wave data
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	add r2,r2,#0x60			@GB 30-3F, GBA 90-9F.
	strb r0,[r1,r2]			@Maybe we should make sure it's the right waveram bank first.
	mov pc,lr			@Alleyway want's it that way.

@----------------------------------------------------------------------------
_FF20W:@		NR41 - Channel 4 Sound Length
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG4CNT_L]
	mov pc,lr
@----------------------------------------------------------------------------
_FF21W:@		NR42 - Channel 4 Volume Envelope
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG4CNT_L+1]
	mov pc,lr
@----------------------------------------------------------------------------
_FF22W:@		NR43 - Channel 4 Polynomial Counter
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG4CNT_H]
	mov pc,lr
@----------------------------------------------------------------------------
_FF23W:@		NR44 - Channel 4 Counter/consecutive; Inital
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SG4CNT_H+1]
	mov pc,lr

@----------------------------------------------------------------------------
_FF24W:@		NR50 - Channel control / ON-OFF / Volume
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SGCNT_L]
	mov pc,lr
@----------------------------------------------------------------------------
_FF25W:@		NR51 - Selection of Sound output terminal
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SGCNT_L+1]
	mov pc,lr
@----------------------------------------------------------------------------
_FF26W:@		NR52 - Sound on/off
@----------------------------------------------------------------------------
	mov addy,#REG_BASE
	strb r0,[addy,#REG_SGCNT_X]
	mov pc,lr

@----------------------------------------------------------------------------


@----------------------------------------------------------------------------
_FF10R:@		NR10 - Channel 1 Sweep register
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG1CNT_L]
	mov pc,lr
@----------------------------------------------------------------------------
_FF11R:@		NR11 - Channel 1 Sound length/Wave pattern duty
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG1CNT_H]
	mov pc,lr
@----------------------------------------------------------------------------
_FF12R:@		NR12 - Channel 1 Volume Envelope
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG1CNT_H+1]
	mov pc,lr
@----------------------------------------------------------------------------
_FF13R:@		NR13 - Channel 1 Frequency lo
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG1CNT_X]
	mov pc,lr
@----------------------------------------------------------------------------
_FF14R:@		NR14 - Channel 1 Frequency hi
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG1CNT_X+1]
	mov pc,lr

@----------------------------------------------------------------------------
_FF16R:@		NR21 - Channel 2 Sound Length/Wave Pattern Duty
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG2CNT_L]
	mov pc,lr
@----------------------------------------------------------------------------
_FF17R:@		NR22 - Channel 2 Volume Envelope
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG2CNT_L+1]
	mov pc,lr
@----------------------------------------------------------------------------
_FF18R:@		NR23 - Channel 2 Frequency lo
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG2CNT_H]
	mov pc,lr
@----------------------------------------------------------------------------
_FF19R:@		NR24 - Channel 2 Frequency hi
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG2CNT_H+1]
	mov pc,lr

@----------------------------------------------------------------------------
_FF1AR:@		NR30 - Channel 3 Sound on/off
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG3CNT_L]
	and r0,r0,#0x80
	mov pc,lr
@----------------------------------------------------------------------------
_FF1BR:@		NR31 - Channel 3 Sound Length
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG3CNT_H]
	mov pc,lr
@----------------------------------------------------------------------------
_FF1CR:@		NR32 - Channel 3 Select output level
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG3CNT_H+1]
	and r0,r0,#0x60
	mov pc,lr
@----------------------------------------------------------------------------
_FF1DR:@		NR33 - Channel 3 Frequency lo
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG3CNT_X]
	mov pc,lr
@----------------------------------------------------------------------------
_FF1ER:@		NR34 - Channel 3 Frequency hi
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG3CNT_X+1]
	mov pc,lr

@----------------------------------------------------------------------------
_FF30R:@		Channel 3 wave data
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	add r2,r2,#0x60			@GB 30-3F, GBA 90-9F.
	ldrb r0,[r1,r2]
	mov pc,lr

@----------------------------------------------------------------------------
_FF20R:@		NR41 - Channel 4 Sound Length
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG4CNT_L]
	mov pc,lr
@----------------------------------------------------------------------------
_FF21R:@		NR42 - Channel 4 Volume Envelope
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG4CNT_L+1]
	mov pc,lr
@----------------------------------------------------------------------------
_FF22R:@		NR43 - Channel 4 Polynomial Counter
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG4CNT_H]
	mov pc,lr
@----------------------------------------------------------------------------
_FF23R:@		NR44 - Channel 4 Counter/consecutive; Inital
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SG4CNT_H+1]
	mov pc,lr

@----------------------------------------------------------------------------
_FF24R:@		NR50 - Channel control / ON-OFF / Volume
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SGCNT_L]
	mov pc,lr
@----------------------------------------------------------------------------
_FF25R:@		NR51 - Selection of Sound output terminal
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SGCNT_L+1]
	mov pc,lr
@----------------------------------------------------------------------------
_FF26R:@		NR52 - Sound on/off
@----------------------------------------------------------------------------
	mov r1,#REG_BASE
	ldrb r0,[r1,#REG_SGCNT_X]
	@work around for bugs in VBA-M, fixes Zelda Oracles games when running Goomba Color in that emulator
	ldrb r2,[r1,#REG_SG3CNT_L]	@is channel 3 "play" flag off?
	tst r2,#0x80
	biceq r0,#0x04				@clear "channel 3 is playing" bit
	
	mov pc,lr

@----------------------------------------------------------------------------
	@.end

