	.macro global_func function
	.global \function
	.type   \function STT_FUNC
	.endm

	.macro adrl2 reg,label
	mov \reg,#\label&0x0FF00000
	add \reg,\reg,#\label&0x000FFFFF
	.endm

	.macro start_map base, register
	@GBLA _map_address_
 _map_address_ = \base
	.endm

	.macro _m_ label=0,size
	.if \label != 0
 \label = _map_address_
	.endif
 _map_address_ = _map_address_ + \size
	.endm

	.macro ldr_ reg,label
	ldr \reg,[globalptr,#\label]
	.endm
	
	.macro ldrb_ reg,label
	ldrb \reg,[globalptr,#\label]
	.endm
	
	.macro ldrh_ reg,label
	ldrh \reg,[globalptr,#\label]
	.endm
	
	.macro str_ reg,label
	str \reg,[globalptr,#\label]
	.endm
	
	.macro strb_ reg,label
	strb \reg,[globalptr,#\label]
	.endm
	
	.macro strh_ reg,label
	strh \reg,[globalptr,#\label]
	.endm



	.macro ldreq_ reg,label
	ldreq \reg,[globalptr,#\label]
	.endm
	
	.macro ldreqb_ reg,label
	ldreqb \reg,[globalptr,#\label]
	.endm
	
	.macro streq_ reg,label
	streq \reg,[globalptr,#\label]
	.endm
	
	.macro streqb_ reg,label
	streqb \reg,[globalptr,#\label]
	.endm
	



	.macro ldrne_ reg,label
	ldrne \reg,[globalptr,#\label]
	.endm
	
	.macro ldrneb_ reg,label
	ldrneb \reg,[globalptr,#\label]
	.endm
	
	.macro strne_ reg,label
	strne \reg,[globalptr,#\label]
	.endm
	
	.macro strneb_ reg,label
	strneb \reg,[globalptr,#\label]
	.endm
	


	.macro ldrhi_ reg,label
	ldrhi \reg,[globalptr,#\label]
	.endm
	
	.macro ldrhib_ reg,label
	ldrhib \reg,[globalptr,#\label]
	.endm
	
	.macro strhi_ reg,label
	strhi \reg,[globalptr,#\label]
	.endm
	
	.macro strhib_ reg,label
	strhib \reg,[globalptr,#\label]
	.endm


	.macro ldrmi_ reg,label
	ldrmi \reg,[globalptr,#\label]
	.endm
	
	.macro ldrmib_ reg,label
	ldrmib \reg,[globalptr,#\label]
	.endm
	
	.macro strmi_ reg,label
	strmi \reg,[globalptr,#\label]
	.endm
	
	.macro strmib_ reg,label
	strmib \reg,[globalptr,#\label]
	.endm

	.macro ldrpl_ reg,label
	ldrpl \reg,[globalptr,#\label]
	.endm
	
	.macro ldrplb_ reg,label
	ldrplb \reg,[globalptr,#\label]
	.endm
	
	.macro strpl_ reg,label
	strpl \reg,[globalptr,#\label]
	.endm
	
	.macro strplb_ reg,label
	strplb \reg,[globalptr,#\label]
	.endm


	.macro ldrgt_ reg,label
	ldrgt \reg,[globalptr,#\label]
	.endm
	
	.macro ldrgtb_ reg,label
	ldrgtb \reg,[globalptr,#\label]
	.endm
	
	.macro strgt_ reg,label
	strgt \reg,[globalptr,#\label]
	.endm
	
	.macro strgtb_ reg,label
	strgtb \reg,[globalptr,#\label]
	.endm


	.macro ldrge_ reg,label
	ldrge \reg,[globalptr,#\label]
	.endm
	
	.macro ldrgeb_ reg,label
	ldrgeb \reg,[globalptr,#\label]
	.endm
	
	.macro strge_ reg,label
	strge \reg,[globalptr,#\label]
	.endm
	
	.macro strgeb_ reg,label
	strgeb \reg,[globalptr,#\label]
	.endm


	.macro ldrlt_ reg,label
	ldrlt \reg,[globalptr,#\label]
	.endm
	
	.macro ldrltb_ reg,label
	ldrltb \reg,[globalptr,#\label]
	.endm
	
	.macro strlt_ reg,label
	strlt \reg,[globalptr,#\label]
	.endm
	
	.macro strltb_ reg,label
	strltb \reg,[globalptr,#\label]
	.endm


	.macro ldrle_ reg,label
	ldrle \reg,[globalptr,#\label]
	.endm
	
	.macro ldrleb_ reg,label
	ldrleb \reg,[globalptr,#\label]
	.endm
	
	.macro strle_ reg,label
	strle \reg,[globalptr,#\label]
	.endm
	
	.macro strleb_ reg,label
	strleb \reg,[globalptr,#\label]
	.endm


	.macro ldrlo_ reg,label
	ldrlo \reg,[globalptr,#\label]
	.endm
	
	.macro ldrlob_ reg,label
	ldrlob \reg,[globalptr,#\label]
	.endm
	
	.macro strlo_ reg,label
	strlo \reg,[globalptr,#\label]
	.endm
	
	.macro strlob_ reg,label
	strlob \reg,[globalptr,#\label]
	.endm
	

	.macro adr_ reg,label
	add \reg,globalptr,#\label
	.endm
	
	.macro adreq_ reg,label
	addeq \reg,globalptr,#\label
	.endm
	
	.macro adrne_ reg,label
	addne \reg,globalptr,#\label
	.endm
	
	.macro adrgt_ reg,label
	addgt \reg,globalptr,#\label
	.endm
	
	.macro adrlt_ reg,label
	addlt \reg,globalptr,#\label
	.endm
	
	.macro adrge_ reg,label
	addge \reg,globalptr,#\label
	.endm
	
	.macro adrle_ reg,label
	addle \reg,globalptr,#\label
	.endm
	
	.macro adrhi_ reg,label
	addhi \reg,globalptr,#\label
	.endm

	.macro adrlo_ reg,label
	addlo \reg,globalptr,#\label
	.endm
	
	.macro adrl_ reg,label
@	.if \label&0x80000000
@	sub \reg,globalptr,#((-(\label)) & 0xFF00FF00)
@	sub \reg,\reg,#((-(\label)) & 0x000000FF)
@	.else
	add \reg,globalptr,#((\label) & 0xFF00FF00)
	add \reg,\reg,#((\label) & 0x000000FF)
@	.endif
	.endm

	.macro adrnel_ reg,label
@	.if \label&0x80000000
@	subne \reg,globalptr,#((-(\label)) & 0xFF00FF00)
@	subne \reg,\reg,#((-(\label)) & 0x000000FF)
@	.else
	addne \reg,globalptr,#((\label) & 0xFF00FF00)
	addne \reg,\reg,#((\label) & 0x000000FF)
@	.endif
	.endm


 .if VERSION_IN_ROM
	.macro bl_long label
	mov lr,pc
	ldr pc,=\label
	.endm

	.macro bleq_long label
	moveq lr,pc
	ldreq pc,=\label
	.endm

	.macro bllo_long label
	movlo lr,pc
	ldrlo pc,=\label
	.endm

	.macro blhi_long label
	movhi lr,pc
	ldrhi pc,=\label
	.endm

	.macro bllt_long label
	movlt lr,pc
	ldrlt pc,=\label
	.endm

	.macro blgt_long label
	movgt lr,pc
	ldrgt pc,=\label
	.endm

	.macro blle_long label
	movle lr,pc
	ldrle pc,=\label
	.endm

	.macro blge_long label
	movge lr,pc
	ldrge pc,=\label
	.endm

	.macro blne_long label
	movne lr,pc
	ldrne pc,=\label
	.endm

	.macro blcc_long label
	movcc lr,pc
	ldrcc pc,=\label
	.endm

	.macro blpl_long label
	movpl lr,pc
	ldrpl pc,=\label
	.endm

	.macro blx_long label
	add lr,pc,#4
	ldr r12,=\label
	bx r12
	.endm

	.macro blxeq_long label
	addeq lr,pc,#4
	ldreq r12,=\label
	bxeq r12
	.endm

	.macro blxlo_long label
	addlo lr,pc,#4
	ldrlo r12,=\label
	bxlo r12
	.endm

	.macro blxhi_long label
	addhi lr,pc,#4
	ldrhi r12,=\label
	bxhi r12
	.endm

	.macro blxlt_long label
	addlt lr,pc,#4
	ldrlt r12,=\label
	bxlt r12
	.endm

	.macro blxgt_long label
	addgt lr,pc,#4
	ldrgt r12,=\label
	bxgt r12
	.endm

	.macro blxne_long label
	addne lr,pc,#4
	ldrne r12,=\label
	bxne r12
	.endm

	.macro blxcc_long label
	addcc lr,pc,#4
	ldrcc r12,=\label
	bxcc r12
	.endm

	.macro blxpl_long label
	addpl lr,pc,#4
	ldrpl r12,=\label
	bxpl r12
	.endm

	.macro blxmi_long label
	addmi lr,pc,#4
	ldrmi r12,=\label
	bxmi r12
	.endm

	.macro b_long label
	ldr pc,=\label
	.endm

	.macro bcc_long label
	ldrcc pc,=\label
	.endm

	.macro bhs_long label
	ldrhs pc,=\label
	.endm

	.macro beq_long label
	ldreq pc,=\label
	.endm

	.macro bne_long label
	ldrne pc,=\label
	.endm

	.macro blo_long label
	ldrlo pc,=\label
	.endm

	.macro bhi_long label
	ldrhi pc,=\label
	.endm

	.macro bgt_long label
	ldrgt pc,=\label
	.endm

	.macro blt_long label
	ldrlt pc,=\label
	.endm

	.macro bge_long label
	ldrge pc,=\label
	.endm

	.macro ble_long label
	ldrle pc,=\label
	.endm

	.macro bcs_long label
	ldrcs pc,=\label
	.endm

	.macro bmi_long label
	ldrmi pc,=\label
	.endm

	.macro bpl_long label
	ldrpl pc,=\label
	.endm

	.else

	.macro blx_long label
	add lr,pc,#4
	ldr r12,=\label
	bx r12
	.endm

	.macro blxeq_long label
	addeq lr,pc,#4
	ldreq r12,=\label
	bxeq r12
	.endm

	.macro blxlo_long label
	addlo lr,pc,#4
	ldrlo r12,=\label
	bxlo r12
	.endm

	.macro blxhi_long label
	addhi lr,pc,#4
	ldrhi r12,=\label
	bxhi r12
	.endm

	.macro blxlt_long label
	addlt lr,pc,#4
	ldrlt r12,=\label
	bxlt r12
	.endm

	.macro blxgt_long label
	addgt lr,pc,#4
	ldrgt r12,=\label
	bxgt r12
	.endm

	.macro blxne_long label
	addne lr,pc,#4
	ldrne r12,=\label
	bxne r12
	.endm

	.macro blxcc_long label
	addcc lr,pc,#4
	ldrcc r12,=\label
	bxcc r12
	.endm

	.macro blxpl_long label
	addpl lr,pc,#4
	ldrpl r12,=\label
	bxpl r12
	.endm

	.macro blxmi_long label
	addmi lr,pc,#4
	ldrmi r12,=\label
	bxmi r12
	.endm

	.macro bl_long label
	bl \label
	.endm

	.macro bleq_long label
	bleq \label
	.endm

	.macro bllo_long label
	bllo \label
	.endm

	.macro blhi_long label
	blhi \label
	.endm

	.macro bllt_long label
	bllt \label
	.endm

	.macro blgt_long label
	blgt \label
	.endm
	
	.macro blle_long label
	blle \label
	.endm

	.macro blge_long label
	blge \label
	.endm

	.macro blne_long label
	blne \label
	.endm

	.macro blcc_long label
	blcc \label
	.endm

	.macro blpl_long label
	blpl \label
	.endm

	.macro b_long label
	b \label
	.endm

	.macro bcc_long label
	bcc \label
	.endm

	.macro bhs_long label
	bhs \label
	.endm

	.macro beq_long label
	beq \label
	.endm

	.macro bne_long label
	bne \label
	.endm

	.macro blo_long label
	blo \label
	.endm

	.macro bhi_long label
	bhi \label
	.endm

	.macro bgt_long label
	bgt \label
	.endm

	.macro blt_long label
	blt \label
	.endm

	.macro bge_long label
	bge \label
	.endm

	.macro ble_long label
	ble \label
	.endm

	.macro bcs_long label
	bcs \label
	.endm

	.macro bmi_long label
	bmi \label
	.endm

	.macro bpl_long label
	bpl \label
	.endm
 .endif

	@.end
