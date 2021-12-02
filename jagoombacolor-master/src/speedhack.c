#include "includes.h"

typedef enum
{
	Nothing = 0,
	Io,
	Immediate,
	Absolute,
	ConditionalBranch,
	UnconditionalBranch,
	ConditionalJump,
	UnconditionalJump,
	ConditionalReturn,
	UnconditionalReturn,
	ChangesA,
	ReadsBC,
	ReadsHL,
	ReadsDE,
	CB,
	Reject
} INS_FLAGS;


//00 = no extra data in instruction
//01 = zeropage/io
//02 = immediate value (8-bit or 16-bit)
//03 = absolute address (16-bit value)
//04 = conditional branch
//05 = unconditional branch
//06 = conditional jump
//07 = unconditional jump
//08 = conditional return
//09 = unconditional return
//0A = changes value of A or HL to something besides a constant
//0B = reads from (bc)
//0C = reads from (hl)
//0D = reads from (de)
//0E = 
//0F = reject
//00 = length 0  (never happens)
//10 = length 1
//20 = length 2
//30 = length 3
//00 = is a read
//40 = Changes A to a constant or value from memory
//80 = is a write  (reject for now)
//C0 = is an increment  (reject for now)

//  NOP     LD BC,nn	LD (BC),A   INC BC      INC B       DEC B       LD B,n      RLC A       LD (nn),SP  ADD HL,BC   LD A,(BC)   DEC BC      INC C       DEC C       LD C,n      RRC A
//  STOP    LD DE,nn    LD (DE),A   INC DE      INC D       DEC D       LD D,n      RL A        JR n        ADD HL,DE   LD A,(DE)   DEC DE      INC E       DEC E       LD E,n      RR A
// JR NZ,n  LD HL,nn    LDI (HL),A  INC HL      INC H       DEC H       LD H,n      DAA         JR Z,n      ADD HL,HL   LDI A,(HL)  DEC HL      INC L       DEC L       LD L,n      CPL
// JR NC,n  LD SP,nn    LDD (HL),A  INC SP      INC (HL)    DEC (HL)    LD (HL),n   SCF         JR C,n      ADD HL,SP   LDD A,(HL)  DEC SP      INC A       DEC A       LD A,n      CCF
// LD B,B   LD B,C      LD B,D      LD B,E      LD B,H      LD B,L      LD B,(HL)   LD B,A      LD C,B      LD C,C      LD C,D      LD C,E      LD C,H      LD C,L      LD C,(HL)   LD C,A
// LD D,B   LD D,C      LD D,D      LD D,E      LD D,H      LD D,L      LD D,(HL)   LD D,A      LD E,B      LD E,C      LD E,D      LD E,E      LD E,H      LD E,L      LD E,(HL)   LD E,A
// LD H,B   LD H,C      LD H,D      LD H,E      LD H,H      LD H,L      LD H,(HL)   LD H,A      LD L,B      LD L,C      LD L,D      LD L,E      LD L,H      LD L,L      LD L,(HL)   LD L,A
//LD (HL),B LD (HL),C   LD (HL),D   LD (HL),E   LD (HL),H   LD (HL),L   HALT        LD (HL),A   LD A,B      LD A,C      LD A,D      LD A,E      LD A,H      LD A,L      LD A,(HL)   LD A,A
// ADD A,B  ADD A,C     ADD A,D     ADD A,E     ADD A,H     ADD A,L     ADD A,(HL)  ADD A,A     ADC A,B     ADC A,C     ADC A,D     ADC A,E     ADC A,H     ADC A,L     ADC A,(HL)  ADC A,A
// SUB A,B  SUB A,C     SUB A,D     SUB A,E     SUB A,H     SUB A,L     SUB A,(HL)  SUB A,A     SBC A,B     SBC A,C     SBC A,D     SBC A,E     SBC A,H     SBC A,L     SBC A,(HL)  SBC A,A
// AND B    AND C       AND D       AND E       AND H       AND L       AND (HL)    AND A       XOR B       XOR C       XOR D       XOR E       XOR H       XOR L       XOR (HL)    XOR A
// OR B     OR C        OR D        OR E        OR H        OR L        OR (HL)     OR A        CP B        CP C        CP D        CP E        CP H        CP L        CP (HL)     CP A
//RET NZ    POP BC      JP NZ,nn    JP nn       CALL NZ,nn  PUSH BC     ADD A,n     RST 0       RET Z       RET         JP Z,nn     *CB*        CALL Z,nn   CALL nn     ADC A,n     RST 8
//RET NC    POP DE      JP NC,nn    XX          CALL NC,nn  PUSH DE     SUB A,n     RST 10      RET C       RETI        JP C,nn     XX          CALL C,nn   XX          SBC A,n     RST 18
//LDH (n),A POP HL      LDH (C),A   XX          XX          PUSH HL     AND n       RST 20      ADD SP,d    JP (HL)     LD (nn),A   XX          XX          XX          XOR n       RST 28
//LDH A,(n) POP AF      XX          DI          XX          PUSH AF     OR n        RST 30      LDHL SP,d   LD SP,HL    LD A,(nn)   EI          XX          XX          CP n        RST 38

const u8 INS_TABLE[] =
{
//////  00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F
/*00*/0x10,0x32,0x9B,0x1F,0x1F,0x1F,0x22,0x1A,0x9F,0x1F,0x5B,0x1F,0x1F,0x1F,0x22,0x1A,
/*10*/0x1F,0x32,0x9D,0x1F,0x1F,0x1F,0x22,0x1A,0x25,0x1F,0x5D,0x1F,0x1F,0x1F,0x22,0x1A,
/*20*/0x24,0x32,0x9F,0x1F,0x1F,0x1F,0x22,0x1F,0x24,0x1F,0x5F,0x1F,0x1F,0x1F,0x22,0x1A,
/*30*/0x24,0x3F,0x9F,0x1F,0xDC,0xDF,0xAC,0x10,0x24,0x1F,0x5F,0x1F,0x1F,0x1F,0x62,0x10,
/*40*/0x10,0x10,0x10,0x10,0x10,0x10,0x1C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1C,0x10,
/*50*/0x10,0x10,0x10,0x10,0x10,0x10,0x1C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1C,0x10,
/*60*/0x1A,0x1A,0x1A,0x1A,0x10,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x10,0x1A,0x1A,
/*70*/0x9C,0x9C,0x9C,0x9C,0x9C,0x9C,0x1F,0x9C,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x50,0x10,
/*80*/0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,
/*90*/0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x50,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,
/*A0*/0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1C,0x10,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x50,
/*B0*/0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1C,0x10,
/*C0*/0x18,0x1F,0x26,0x27,0x2F,0x1F,0x1A,0x1F,0x18,0x19,0x26,0x2E,0x2F,0x2F,0x2A,0x1F,
/*D0*/0x18,0x1F,0x26,0x1F,0x2F,0x1F,0x1A,0x1F,0x18,0x19,0x26,0x1F,0x2F,0x1F,0x2A,0x1F,
/*E0*/0xA1,0x1F,0x9B,0x1F,0x1F,0x1F,0x2A,0x1F,0x3F,0x1F,0xB3,0x1F,0x1F,0x1F,0x2A,0x1F,
/*F0*/0x21,0x1F,0x1F,0x1F,0x1F,0x1F,0x2A,0x1F,0x3F,0x1F,0x73,0x10,0x1F,0x1F,0x22,0x1F
};


/*
speedhacks TODO:


back:
21 xx xx	ld hl,nnnn
CB 46		bit 0,(hl)
20 xx	jr nz ahead
18 xx	jr back
ahead:
DONE

back:
ld a,(ff00+A7)
or a
ret nz
jr back
DONE

back:
ld a,(nnnn)
cp b
ret nz
jr back
DONE

back:
ld a, (ff00 + B8)
and a
jp nz, 2D14
ld a, (FF00 + A9)
cp a,3
jr nz, back
DONE

back:
ld a,(nnnn)
cp c
jr z, back

back:
jr back

back:
ld hl,C138
ld a,(C0CF)
cp (hl)
jr c,back

ld a,(nnnn)
or a
jp nz, back

ld a,(ff00+41)
and 3
dec a
ret z
jr back

ei
ld a,(C12E)
and a
jr z,back


(taito)

ld a,(FF00+9E)
CB 7F   bit 7,a
jr z,  back
ld a,(FF00 + 40)
bit 7,a
jr nz, back



ld a (FF00+41)
and 4
jr z, back
*/


#if SPEEDHACKS_NEW

/*
call_quickhackfinder:
	@r0 = hack number
	@new code was added, TESTME
	@was hack used?
	ldr addy,=speedhacks
	add addy,addy,r0,lsl#4
	ldr   r1,[addy,#4]
	tst   r1,#0x0000FF00 @test hack was used
	beq 0f
	
	bic r1,r1,#0x00FF0000 @clear frames_hack_not_used
	bic r1,r1,#0x0000FF00 @clear hack was used
	str r1,[addy,#4]
	@check if we're installing the PPU hack
	ldr r0,=ppustat_
	ldrb r0,[r0]
	and r0,r0,#0x40
	mov r0,r0,lsr#6
	@maybe include code to make it not pick itself again
	b set_cpu_hack
0:
	stmfd sp!,{r3,lr}
	mov r2,r0
	mov r0,m6502_pc
	ldr_ r1,lastbank
	
	blx_long speedhack_manager
	
	ldmfd sp!,{r3,pc}
*/

static const int LOOKBACK = 18;


	//01 = zp
	//02 = imm
	//03 = abs
	//04 = branch
	//05 = jump
	//06 = read hl
	//0F = reject
	//00 = length 0
	//10 = length 1
	//20 = length 2
	//30 = length 3
	//00 = is a read
	//40 = okay to do math on A after loading this
	//80 = is a write
	//C0 = is an increment

static __inline u8 ins_table(int ins)
{
	return INS_TABLE[ins];
	/*
	//whitelist of instructions:
	//20	jr nz
	//28	jr z
	//30	jr nc
	//38	jr c
	//7E	ld a,(hl)
	//A7	and a
	//B7	or a
	//BE	cp (hl)
	//BF	cp a
	//E6	and #
	//F0	ld a,(FF00 + nn)
	//FA	ld a,(nnnn)
	//FE	cp #nn
	switch (ins)
	{
		case 0x20:	//jr nz
			return 0x24;
		case 0x28:	//jr z
			return 0x24;
		case 0x30:	//jr nc
			return 0x24;
		case 0x38:	//jr c
			return 0x24;
		case 0x7E:	//ld a,(hl)
			return 0x16;
		case 0xA7:	//and a
			return 0x10;
		case 0xB7:	//or a
			return 0x10;
		case 0xBE:	//cp (hl)
			return 0x16;
		case 0xBF:	//cp a
			return 0x10;
		case 0xE6:	//and #
			return 0x22;
		case 0xF0:	//ld a,(FF00 + nn)
			return 0x21;
		case 0xFA:	//ld a,(nnnn)
			return 0x33;
		case 0xFE:	//cp #nn
			return 0x22;
	}
	return 0xFF;
	*/
}

/*
static __inline bool forward_branch_is_jump_back(const u8 *pc, int branchlength, int initpc_16, const u8 *lastbank)
{
	const u8* branch_dest=pc+2+branchlength;
	if ((ins_table(*branch_dest)&0x0F)==5)
	{
		int dest_addr=branch_dest[1]+branch_dest[2]*256;
		if (dest_addr<=initpc_16 && dest_addr>=initpc_16-LOOKBACK)
		{
			return true;
		}
	}
	return false;
}

static __inline bool forward_branch_is_jump_back_2(const u8 *pc, int branchlength, int start_16)
{
	const u8* branch_dest=pc+2+branchlength;
	if ((ins_table(*branch_dest)&0x0F)==5)
	{
		int dest_addr=branch_dest[1]+branch_dest[2]*256;
		if (dest_addr==start_16)
		{
			return true;
		}
	}
	return false;
}
*/

static __inline int gets8(const u8 *p, int i)
{
	return ((const s8*)p)[i];
}

static __inline int getu16(const u8 *p, int i)
{
	return p[i] + p[i+1] * 0x100;
}

static const u8 *find_first_instruction(const u8 *initpc, const u8 *lastbank, const u8 **branchpc)
{
	const u8 *pc_limit=initpc+LOOKBACK - 2;
	const u8 *pc=initpc;
	int init_pc_16=initpc-lastbank;
	while (pc < pc_limit)
	{
		u8 ins=ins_table(*pc);
		int len=(ins>>4)&3;
		int action=ins&0x0F;
		int targetPc = -1;
		
		switch (action)
		{
			case Nothing:
				break;
			case Io:
				break;
			case Immediate:
				break;
			case Absolute:
				break;
			case ConditionalBranch:
			case UnconditionalBranch:
				{
					int branchLength = gets8(pc,1);
					targetPc = pc + 1 - lastbank + branchLength;
					
					//conditional branch backwards:
					//  to satisfy the loop rules:
					//	  must be <= to initial PC
					//    must be within LOOKBACK distance
					//  otherwise treat it as a non-looping branch
					//conditional branch forwards:
					//	there must be a later branch that satisfies the rules
					
					if (targetPc <= init_pc_16 && branchLength >= -LOOKBACK)
					{
						//accept it as a loop, then it needs further analysis
						*branchpc = pc;
						return pc + 2 + branchLength;
					}
				}
				//TODO
				break;
			case ConditionalJump:
			case UnconditionalJump:
				{
					targetPc = getu16(pc,1);
					int pc16 = pc - lastbank;
					int branchLength = targetPc - (pc16 + 2);
					
					if (targetPc <= init_pc_16 && branchLength >= -LOOKBACK)
					{
						*branchpc = pc;
						return pc + 3 + branchLength;
					}
				}
				break;
			case ConditionalReturn:
				break;
			case UnconditionalReturn:
				return NULL;
			case ChangesA:
				break;
			case ReadsBC:
				break;
			case ReadsHL:
				break;
			case ReadsDE:
				break;
			case CB:
				break;
			case Reject:
				return NULL;
		}
		pc+=len;
	}
	return NULL;
}

static const u8 *find_hack(const u8 *start_pc, const u8 *branchpc, const u8 *lastbank)
{
	//breakpoint();
	//breakpoint();
	const u8 *pc = start_pc;
	int aDirty = 0;
	while (pc < branchpc)
	{
		u8 ins = ins_table(*pc);
		//breakpoint();
		int len = (ins >> 4) & 3;
		if (len == 0)
		{
			//shouldn't happen
			len = 1;
		}
		int action = (ins & 0x0F);
		int isWrite = (ins >> 6);
		//for now, reject all writes
		if (isWrite >= 2)
		{
			return NULL;
		}
		
		int addressOfRead = -1;
		int addressOfJump = -1;
		
		switch (action)
		{
			case Nothing:
				break;
			case Io:
				addressOfRead = 0xFF00 + pc[1];
				break;
			case Immediate:
				break;
			case Absolute:
				addressOfRead = getu16(pc, 1);
				break;
			case ConditionalBranch:
			case UnconditionalBranch:
				addressOfJump = pc + 1 + gets8(pc, 1) - lastbank;
				break;
			case ConditionalJump:
			case UnconditionalJump:
				addressOfJump = getu16(pc, 1);
				break;
			case ConditionalReturn:
				break;
			case UnconditionalReturn:
				return NULL;
			case ChangesA:
				aDirty |= 1;
				break;
			case ReadsBC:
				addressOfRead = STATE_BC;
				break;
			case ReadsHL:
				addressOfRead = STATE_HL;
				break;
			case ReadsDE:
				addressOfRead = STATE_DE;
				break;
			case CB:
				{
					int b = pc[1];
					if (b >= 0x40 && b <= 0x7F)
					{
						if ((b & 0x07) == 6)
						{
							addressOfRead = STATE_HL;
						}
					}
					else
					{
						return NULL;
					}
				}
				break;
			case Reject:
				return NULL;
		}
		if (isWrite == 1)
		{
			aDirty = 2;
		}
		
		//check if we are rejecting an IO read
		if (addressOfRead >= 0xFF00)
		{
			int addr = addressOfRead & 0xFF;
			if (addr < 0x80)
			{
				//timer, link, sound registers  (not joypad)
				if (addr >= 0x01 && addr < 0x40)
				{
					return NULL;
				}
				if (addr == 0x44 || addr == 0x41 || addr == 0x55 || addr == 0x56)
				{
					return NULL;
				}
			}
		}
		
		//rules for branches (TODO)
		pc += len;
	}
	if (aDirty == 1)
	{
		return NULL;
	}
	u8 ins2 = ins_table(*branchpc) & 0x0F;
	if (ins2 == ConditionalBranch || ins2 == UnconditionalBranch)
	{
		return branchpc + 2;
	}
	else
	{
		return branchpc + 1;
	}
}

/*
static const u8 *find_hack(const u8 *start_pc, const u8 *branchpc, int hl)
{
	const u8 *pc = start_pc;
	while (pc < branchpc)
	{
		u8 ins = ins_table(*pc);
		int len = (ins>>4)&3;
		int action = ins&0x0F;
		int addr = 0;
		//int iswrite = (ins>>6);
		switch (action)
		{
			case 0:
				break;
			case 1:
				addr = 0xFF00 + pc[1];
				break;
			case 2:
				break;
			case 3:
				addr = pc[1] + (pc[2] << 8);
				break;
			case 4:
			case 5:
			default:
				return NULL;
			case 6:
				addr = hl;
				break;
		}
		//check if the read is to a hardware register that could change
		if (addr >= 0xFF00 && addr < 0xFF80)
		{
			addr = addr & 0xFF;
			if (addr >= 0x01 && addr < 0x40)
			{
				return NULL;
			}
			if (addr == 0x44 || addr == 0x41 || addr == 0x55 || addr == 0x56)
			{
				return NULL;
			}
		}
		pc += len;
	}
	return branchpc + 2;
}
*/

/*
static const u8 *find_hack(const u8 *start_pc, const u8 *branchpc, const u8 *lastbank, int hacknum)
{
	int start_16=start_pc-lastbank;
	const int MAX_READS=16;
	const int MAX_WRITES=16;
	const int MAX_INCS=16;
	
	u16 *const incs= &SPEEDHACK_TEMP_BUF[0];
	u16 *const reads= &SPEEDHACK_TEMP_BUF[16];
	u16 *const writes= &SPEEDHACK_TEMP_BUF[32];
	
	int num_reads=0;
	int num_writes=0;
	int num_incs=0;
	int addr;

	int total_cycles=0;

	int math_a_okay=0;
	int last_instruction_was_increment=0;
	
	const u8 *pc=start_pc;
	if (start_pc==NULL) return NULL;
	while (pc<branchpc)
	{
		//check if an instruction disqualifies
		u8 ins=ins_table(*pc);
		int len=(ins>>4)&3;
		int action=ins&0x0F;
		int iswrite=(ins>>6);
		addr=-1;
		switch (action)
		{
		case 6:  //"math on A", for now just ASL A
			if (!math_a_okay) return NULL;
		case 0:  //implied addressing mode
		case 2:  //immediate addressing mode
			total_cycles+=2;
			break;
		case 1:  //zeropage addressing mode
			addr=pc[1];
			total_cycles+=3;
			break;
		case 3:  //absolute addressing mode
			addr=pc[1]+pc[2]*256;
			total_cycles+=4;
			break;
		case 4:  //branch
			total_cycles+=2;
			{
				int branchlength;
				if (last_instruction_was_increment)
				{
					return NULL;
				}
				branchlength=gets8(pc,1);
				if (forward_branch_is_jump_back_2(pc,gets8(pc,1),start_16))
				{
					total_cycles+=1;
					goto out_loop;
				}
				if (branchlength<0) return NULL;  //only the last branch can be a jump back
			}
			break;
		case 5:	//reject unconditional jumps except at end
		case 0x0F: //reject
		default:
			return NULL;
		}
		if (addr!=-1)
		{
			if (iswrite<2)  //is a read, or "Math on A is okay"
			{
				last_instruction_was_increment = 0;
				if (num_reads==MAX_READS) return NULL;
				reads[num_reads++]=addr;
			}
			else  //is a write or incrmenet
			{
				if ((addr>=0x2000 && addr<0x6000) || (addr>=0x8000)) return NULL;
				if (num_writes==MAX_WRITES) return NULL;
				writes[num_writes++]=addr;
			}
			if (iswrite==1)  //LDA zpg, imm, or abs
			{
				math_a_okay=1;
			}

			if (iswrite==3)  //is an increment
			{
				last_instruction_was_increment = 1;
				total_cycles+=2;
				if (num_incs==MAX_INCS) return NULL;
				incs[num_incs++]=addr;
			}
		}
		pc+=len;
	}
out_loop:
	if (pc!=branchpc) return NULL;
	
	if ((*pc & 0x1F)==0x10) //if it's a branch
	{
		if (last_instruction_was_increment)
		{
			return NULL;
		}
		int end_16=(pc-lastbank)+2;
		total_cycles+=3;
		if ((start_16 & 0xFF00) != (end_16 & 0xFF00))
		{
			total_cycles+=1;
		}
	}
	else	//it's a jump
	{
		total_cycles+=3;
	}
	
	
	//check read and write list for collisions
	if (num_reads!=0 && num_writes!=0)
	{
		int i,j;
		int a,b,lasta,lastb;
		lasta=-1;
		for (i=0;i<num_reads;i++)
		{
			a=reads[i];
			if (lasta==a) continue;
			lasta=a;
			lastb=-1;
			for (j=0;j<num_writes;j++)
			{
				b=writes[j];
				if (lastb==b) continue;
				lastb=b;
				if (a==b) return NULL;
			}
		}
	}
	{
		speedhack_T *hack=&speedhacks[hacknum];
		hack->hack_pc=branchpc;
		hack->num_incs=num_incs;
		total_cycles*=3;
		hack->cycles_per_iteration=total_cycles;
		hack->divider=(u32)((u64)0x100000000LL/(u64)total_cycles)+1;
	}
	memcpy32(&SPEEDHACK_INCS[hacknum*16],&incs[0],16*sizeof(u16));
	return branchpc;
}
*/


bool quickhackfinder(const u8 *initpc, const u8 *lastbank) //, int hacknum)
{
	const u8 *branchpc;
	const u8 *hackpc = NULL;
	const u8 *pc;
	
	pc = find_first_instruction(initpc, lastbank, &branchpc);
	if (pc)
	{
		hackpc=find_hack(pc, branchpc, lastbank);
		if (hackpc)
		{
			u8 ins = ins_table(branchpc[0]) & 0x0F;
			int isJump = (ins == ConditionalJump || ins == UnconditionalJump);
			install_speedhack(hackpc, isJump);
			return true;
		}
	}
	if (hackpc == NULL && _speedhack_pc != NULL)
	{
		speedhack_reset();
	}
	
	return false;
}

/*

void speedhack_manager(const u8* initpc, const u8* lastbank, int hacknum)
{
	int hack_to_install;
	
	speedhack_T *sh=&speedhacks[hacknum];

	hack_to_install=0;
	if (hacknum==0 && (ppustat_ & 0x40) )
	{
		hack_to_install=1;
	}

	
//	if (sh->hack_was_used)   //this check was moved to ASM
//	{
//		sh->frames_hack_not_used=0;
//		sh->hack_was_used=0;
//	}
//	else
	{
		sh->frames_hack_not_used++;
		if (sh->frames_hack_not_used==8)
		{
			sh->frames_hack_not_used=0;
			if (quickhackfinder(initpc,lastbank,hacknum))
			{
//				set_cpu_hack(hacknum2);
			}
			else
			{
				sh->hack_pc=NULL;
				set_cpu_hack(hack_to_install);	//remove hack
//				if (frametotal<24)
//				{
//					breakpoint();
					game_specific_hack(initpc,lastbank,hacknum);
//				}
			}
		}
	}
	if (speedhacks[hack_to_install].hack_pc)
	{
		set_cpu_hack(hack_to_install);
	}
}
*/

#endif