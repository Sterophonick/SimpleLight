@;--------------------------------------------------------------------
	.section   	.text,"ax",%progbits
		
	.global  Fire_Emblem_0378_patch_start
	.global  Fire_Emblem_0378_patch_end


@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
	.thumb
Fire_Emblem_0378_patch_start:
	MOV		R9, R4
	POP		{R4-R7}
	POP		{R0}
	PUSH	{R0-R5}
	LDR		R1, =0x80987AF
	CMP		R1, R0
	BNE		exit
Set:
	BL		Set_save
exit:
	POP		{R0-R5}
	BX 		R0
@;----------------------------------
	MOV		R9, R4
	POP		{R4-R6}
	POP 	{R0}
	PUSH  {R0-R5}
	LDR 	R1, =0x808847D
	CMP		R1, R0
	BNE  	exit
	B			Set
@;----------------------------------
	POP 	{R4,R5}
	POP 	{R0}
	PUSH 	{R0-R5}
	LDR  	R1, =0x8088551
	CMP 	R1, R0
	BNE  	exit
	B			Set
@;----------------------------------
	PUSH	{R0-R5}
	BNE 	exit2
	BL		Set_save
exit2:
	POP		{R0-R5}
	MOV 	R8, R3
	POP  	{R4-R7}
	POP  	{R0}
	BX 		R0
@;----------------------------------
	PUSH 	{R0-R3}
	BL		Set_save
	POP 	{R0-R3}
	POP   {R4-R7}
	POP   {R0}
	BX 		R0
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
Set_save:
	PUSH	{R1-R7,LR}
	MOV		R0,#0x02
	BL		Set_AUTO_save
	MOV		R0,#0x00
	BL		Set_AUTO_save
	POP		{R1-R7}	
	POP		{R0}
	BX		R0
@;----------------------------------
Set_AUTO_save:
	LDR     R2, =0xFFFFD200
	LDR     R3, =0x9FE0000
	MOVS    R1, #0x80
	STRH    R2, [R3]
	MOVS    R3, #0xA8
	LSL     R1, R1, #0x14
	LSL     R3, R3, #5
	STRH    R3, [R1]
	LDR     R1, =0x8020000
	STRH    R2, [R1]
	LDR     R2, =0x8040000
	STRH    R3, [R2]
	LDR     R2, =0x96C0000
	STRH    R0, [R2]
	LDR     R2, =0x9FC0000
	STRH    R3, [R2]
	BX      LR
	
	.ltorg	
	.align	
Fire_Emblem_0378_patch_end:
   .end
