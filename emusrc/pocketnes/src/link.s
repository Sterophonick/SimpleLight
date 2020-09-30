#include "equates.h"

@	.include "equates.h"
@	.include "io.h"
@	.include "cart.h"
@	.include "6502.h"

	.if LINK

	global_func resetSIO
	global_func serialinterrupt
	global_func link_multi

 .align
 .pool
 .text
 .align
 .pool
@----------------------------------------------------------------------------
serialinterrupt:
@----------------------------------------------------------------------------
@executes with CPU in IRQ mode
	stmfd sp!,{r10}
	ldr globalptr,=GLOBAL_PTR_BASE
	mov r3,#REG_BASE
	add r3,r3,#0x100

	mov r0,#0x1
serWait:	subs r0,r0,#1
	bne serWait
	mov r0,#0x100			@time to wait.
	ldrh r1,[r3,#REG_SIOCNT]
	tst r1,#0x80			@Still transfering?
	bne serWait

	tst r1,#0x40			@communication error? resend?
	bne sio_err

	ldr r0,[r3,#REG_SIOMULTI0]	@Both SIOMULTI0&1
	ldr r1,[r3,#REG_SIOMULTI2]	@Both SIOMULTI2&3

	and r2,r0,#0xff00		@From Master
	cmp r2,#0xaa00
	beq resetrequest		@$AAxx means Master GBA wants to restart

	ldr_ r2,sending
	tst r2,#0x10000
	beq sio_err
	strne_ r0,received0		@store only if we were expecting something
	strne_ r1,received1		@store only if we were expecting something
	eor r2,r2,r0			@Check if master sent what we expected
	ands r2,r2,#0xff00
	strne_ r0,received2		@otherwise print value.
	strne_ r1,received3		@otherwise print value.

@	mov r3,#AGB_PALETTE
@	mov r1,#-1				@white
@	strh r1,[r3]			@BG palette
sio_err:
	strb_ r3,sending+2		@send completed, r3b=0
	ldmfd sp!,{r10}
	bx lr

resetrequest:
@	mov r3,r1,asr#16
@	cmp r3,#-1
@	moveq r3,#3			@up to 3 players.
@	movne r3,#4			@all 4 players
@	cmp r1,#-1
@	moveq r3,#2			@only 2 players.
@	mov r3,#3
@	strb_ r3,nrplayers

	adrl_ r2,received0
	strh r0,[r2]
	
	ldr_ r2,joycfg
	orr r2,r2,#0x01000000
	bic r2,r2,#0x08000000
	str_ r2,joycfg

	ldmfd sp!,{r10}
	bx lr
@---------------------------------------------
xmit:	@send byte in r0
@returns REG_SIOCNT in r1, received P1/P2 in r2, received P3/P4 in r3, Z set if successful, r4-r5 destroyed
@---------------------------------------------
	ldr_ r3,sending
	tst r3,#0x10000		@last send completed?
	bxne lr

	mov r5,#REG_BASE
	add r5,r5,#0x100
	ldrh r1,[r5,#REG_SIOCNT]
	tst r1,#0x80		@clear to send?
	movne pc,lr

	ldrb_ r4,frame
	eor r4,r4,#0x55
	bic r4,r4,#0x80
	orr r0,r0,r4,lsl#8	@r0=new data to send

	ldr_ r2,received0
	ldr_ r3,received1
	cmp r2,#-1			@Check for uninitialized
	eoreq r2,r2,#0xf00
	ldrb_ r4,nrplayers
	cmp r4,#2
	beq players2
	cmp r4,#3
	beq players3
players4:
	eor r4,r2,r3,lsr#16	@P1 & P4
	tst r4,#0xff00		@not in sync yet?
	beq players3
	ldr_ r1,lastsent
	eor r4,r1,r3,lsr#16	@Has P4 missed an interrupt?
	tst r4,#0xff00
	streq_ r1,sending	@Send the value before this.
	b iofail
players3:
	eor r4,r2,r3		@P1 & P3
	tst r4,#0xff00		@not in sync yet?
	beq players2
	ldr_ r1,lastsent
	eor r4,r1,r3		@Has P3 missed an interrupt?
	tst r4,#0xff00
	streq_ r1,sending	@Send the value before this.
	b iofail
players2:
	eor r4,r2,r2,lsr#16	@P1 & P2
	tst r4,#0xff00		@in sync yet?
	beq checkold
	ldr_ r1,lastsent
	eor r4,r1,r2,lsr#16	@Has P2 missed an interrupt?
	tst r4,#0xff00
	streq_ r1,sending	@Send the value before this.
	b iofail
checkold:
	ldr_ r4,sending
	ldr_ r1,lastsent
	eor r4,r4,r1		@Did we send an old value last time?
	tst r4,#0xff00
	bne iogood		@bne
	ldr_ r1,sending
	str_ r0,sending
	str_ r1,lastsent
iofail:	orrs r4,r4,#1		@Z=0 fail
	b notyet
iogood:	ands r4,r4,#0		@Z=1 ok
notyet:	ldr_ r1,sending
	streq_ r1,lastsent
	movne r0,r1		@resend last.

	orr r0,r0,#0x10000
	str_ r0,sending
	strh r0,[r5,#REG_SIOMLT_SEND]	@put data in buffer
	ldrh r1,[r5,#REG_SIOCNT]
	tst r1,#0x4			@Check if we're Master.
	bne endSIO

multip:	ldrh r1,[r5,#REG_SIOCNT]
	tst r1,#0x8			@Check if all machines are in multi mode.
	beq multip

	orr r1,r1,#0x80			@Set send bit
	strh r1,[r5,#REG_SIOCNT]	@start send

endSIO:
	teq r4,#0
	bx lr
@----------------------------------------------------------------------------
resetSIO:	@r0=joycfg
@----------------------------------------------------------------------------
	stmfd sp!,{r10}
	ldr r10,=GLOBAL_PTR_BASE
	bic r0,r0,#0x0f000000
	str_ r0,joycfg

	mov r2,#2		@only 2 players.
	mov r1,r0,lsr#29
	cmp r1,#0x6
	moveq r2,#4		@all 4 players
	cmp r1,#0x5
	moveq r2,#3		@3 players.
	strb_ r2,nrplayers

	mov r2,#REG_BASE
	add r2,r2,#0x100

	mov r1,#0
	strh r1,[r2,#REG_RCNT]

	tst r0,#0x80000000
	moveq r1,#0x2000
	movne r1,   #0x6000
	addne r1,r1,#0x0002	@16bit multiplayer, 57600bps
	strh r1,[r2,#REG_SIOCNT]
	ldmfd sp!,{r10}

	bx lr






@---------------------
link_multi:
@---------------------
				@r2=joycfg
	tst r2,#0x08000000	@link active?
	beq link_sync

	bl xmit			@send joypad data for NEXT frame
	movne pc,r6		@send was incomplete!

	strb_ r2,joy0state		@master is player 1



	mov r2,r2,lsr#16
	strb_ r2,joy1state		@slave1 is player 2
	ldrb_ r4,nrplayers
	cmp r4,#2
	beq fin
	strb_ r3,joy2state
	mov r3,r3,lsr#16
	cmp r4,#3
	strneb_ r3,joy3state
fin:	ands r0,r0,#0		@Z=1
	mov pc,r6

link_sync:
	mov r1,#0x8000
	str_ r1,lastsent
	tst r2,#0x03000000
	beq stage0
	tst r2,#0x02000000
	beq stage1
stage2:
	mov r0,#0x2200
	bl xmit			@wait til other side is ready to go

	moveq r1,#0x8000
	streq_ r1,lastsent
	ldr_ r2,joycfg
	biceq r2,r2,#0x03000000
	orreq r2,r2,#0x08000000
	str_ r2,joycfg

	b badmonkey
stage1:		@other GBA wants to reset
	bl sendreset		@one last time..
	bne badmonkey

	orr r2,r2,#0x02000000	@on to stage 2..
	str_ r2,joycfg

	ldr_ r0,romnumber
	tst r4,#0x4		@who are we?
	beq sg1
	ldrb_ r3,received0	@slaves uses master's timing flags
	bic r1,r1,#USEPPUHACK+NOCPUHACK+PALTIMING
	orr r1,r1,r3
sg1:	
	bl hardreset
@	bl softreset		@game reset

	mov r1,#0
	str_ r1,sending		@reset sequence numbers
	str_ r1,received0
	str_ r1,received1
badmonkey:
	orrs r0,r0,#1		@Z=0 (incomplete xfer)
	mov pc,r6
stage0:	@self-initiated link reset
	bl sendreset		@keep sending til we get a reply
	b badmonkey
sendreset:       @exits with r1=emuflags, r4=REG_SIOCNT, Z=1 if send was OK
	mov r5,#REG_BASE
	add r5,r5,#0x100

	ldr_ r1,emuflags
	and r0,r1,#USEPPUHACK+NOCPUHACK+PALTIMING
	orr r0,r0,#0xaa00		@$AAxx, xx=timing flags

	ldrh r4,[r5,#REG_SIOCNT]
	tst r4,#0x80			@ok to send?
	movne pc,lr

	strh r0,[r5,#REG_SIOMLT_SEND]
	orr r4,r4,#0x80
	strh r4,[r5,#REG_SIOCNT]	@send!
	mov pc,lr

	.endif
	
	@.end
