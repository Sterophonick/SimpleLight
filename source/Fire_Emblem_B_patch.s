@;--------------------------------------------------------------------
	.section   	.text,"ax",%progbits
		
	.global  Fire_Emblem_B_patch_start
	.global  Fire_Emblem_B_patch_end
	.global  Modify_address_B

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
	.thumb
Fire_Emblem_B_patch_start:
	PUSH  {R0-R4}
	BL		Set_save
	POP   {R0-R4}
	POP   {R4,R5}
	POP 	{R0}
	BX 		R0
@;----------------------------------
	PUSH 	{R0-R4}
	BL		Set_save
	POP  	{R0-R4}
	POP  	{R4-R6}
	POP   {R0}
	BX    R0
@;----------------------------------
	PUSH	{R0-R4}
	BL		Set_save
	POP 	{R0-R4}
	MOV  	R10, R5
	POP 	{R4-R7}
	POP  	{R0}
	BX    R0
@;----------------------------------
	MOV		R10, R5
	POP		{R4-R7}
	POP 	{R0}
	PUSH  {R0}
	PUSH  {R1-R5}
	LDR  	R1, =0x80B5D6B
	CMP 	R0, R1
	BNE   exit
	BL		Set_save
exit:
	POP   {R1-R5}
	POP  	{R0}
	BX    R0
@;----------------------------------
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
Modify_address_B:
	.ltorg	
	.align	
Fire_Emblem_B_patch_end:
   .end
