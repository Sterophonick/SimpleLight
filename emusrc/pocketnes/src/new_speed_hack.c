#include "includes.h"

extern u16 SPEEDHACK_TEMP_BUF[48];
extern u16 SPEEDHACK_INCS[64];
extern speedhack_T speedhacks[4];

//extern u8* memmap_tbl[8];  //already defined in asmcalls.h
extern int speedhack_num_incs;
extern u32 speedhack_divider;
extern int speedhack_cycles;

#define LOOKBACK 18

static __inline u8 ins_table(int addr);
static __inline bool forward_branch_is_jump_back(const u8 *pc, int branchlength, int initpc_16, const u8 *lastbank);
static __inline bool forward_branch_is_jump_back_2(const u8 *pc, int branchlength, int start_16);
static __inline int gets8(const u8 *p, int i);
static const u8 *find_first_instruction(const u8 *initpc, const u8 *lastbank, const u8 **branchpc);
static const u8 *find_hack(const u8 *start_pc, const u8 *branchpc, const u8 *lastbank, int hacknum);

const u8 capcom_speedhack[]=
{
	0xA2,0x00,0x86,0x90,0xA0,0x04,0xB5,0x80,0xC9,0x04,0xB0,0x0A,0xE8,0xE8,0xE8,0xE8,0x88,0xD0,0xF3
};

//02 = X, 22 = Y
#define _XXX 0x02
#define _YYY 0x22

const u8 konami_speedhack_0[]={ 0xE6,_XXX,0x18,0xA5,_XXX,0x65,_YYY,0x85,_XXX };  //A+=N*(B+1)   mostgames
const u8 konami_speedhack_1[]={ 0xA5,_XXX,0x18,0x65,_YYY,0x85,_YYY }; //B+=A*N                  lifeforce
const u8 konami_speedhack_2[]={ 0xA5,_XXX,0x18,0x65,_YYY,0x85,_XXX }; //A+=B*N                   cv2
const u8 konami_speedhack_3[]={ 0xA5,_XXX,0x38,0x65,_YYY,0x85,_XXX }; //A+=(B+1)*N              jarin ko chie
const u8 konami_speedhack_4[]={ 0xA5,_XXX,0x38,0x65,_YYY,0x85,_YYY }; //B+=(A+1)*N              goonies2
const u8 konami_speedhack_5[]={ 0xA5,_XXX,0x65,_YYY,0x85,_XXX,0xE6,_XXX }; //A+=B+carry , A++   ddribble
const u8 konami_speedhack_6[]={ 0xE6,_XXX,0xA5,_XXX,0x65,_YYY,0x85,_XXX }; //A++ , A+=B+carry   superc
const u8 konami_speedhack_7[]={ 0xA5,_XXX,0x65,_YYY,0x85,_YYY }; //B+=A+carry                   contra
const u8 konami_speedhack_8[]={ 0xA5,_XXX,0x65,_YYY,0x85,_XXX }; //A+=B+carry                   gradius2

const u8 konami_speedhack_sizes[]={
	sizeof(konami_speedhack_0), //9
	sizeof(konami_speedhack_1), //7
	sizeof(konami_speedhack_2), //7
	sizeof(konami_speedhack_3), //7
	sizeof(konami_speedhack_4), //7
	sizeof(konami_speedhack_5), //8
	sizeof(konami_speedhack_6), //8
	sizeof(konami_speedhack_7), //6
	sizeof(konami_speedhack_8)  //6
};

const u8 *const konami_speedhacks[]={
	konami_speedhack_0,
	konami_speedhack_1,
	konami_speedhack_2,
	konami_speedhack_3,
	konami_speedhack_4,
	konami_speedhack_5,
	konami_speedhack_6,
	konami_speedhack_7,
	konami_speedhack_8
};

const u8 konami_speedhack_cycles[]={
	19, //mostgames
	14, //lifeforce
	14, //cv2
	14, //jarin ko chie
	14, //goonies2
	17, //ddribble
	17, //superc
	12, //contra
	12  //gradius2
};


const u8 quickhackfinder_ins_table[]=
{
	//01 = zp
	//02 = imm
	//03 = abs
	//04 = branch
	//05 = jump
	//06 = math on A
	//0F = reject
	//00 = length 0
	//10 = length 1
	//20 = length 2
	//30 = length 3
	//00 = is a read
	//40 = okay to do math on A after loading this
	//80 = is a write
	//C0 = is an increment
/*  *///impl math shft ???? x/y  math shft ???? impl imme shft ???? x/y  math shft ???? brnc ???? shft ???? x/y  math shft ???? impl math shft ???? x/y  math shft ????
/*  *///  00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F   10   11   12   13   14   15   16   17   18   19   1A   1B   1C   1D   1E   1F
/*00*/	0xFF,0xFF,0xFF,0xFF,0xFF,0x21,0xFF,0xFF,0xFF,0x22,0x16,0xFF,0xFF,0x33,0xFF,0xFF,0x24,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  //ORA,ASL
/*20*/	0xFF,0xFF,0xFF,0xFF,0x21,0x21,0xFF,0xFF,0xFF,0x22,0xFF,0xFF,0x33,0x33,0xFF,0xFF,0x24,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  //AND,ROL
/*40*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x35,0xFF,0xFF,0xFF,0x24,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  //EOR,LSR
/*60*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x24,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  //ADC,ROR
/*80*/	0xFF,0xFF,0xFF,0xFF,0xFF,0xA1,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xB3,0xFF,0xFF,0x24,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  //STA,STX
/*A0*/	0xFF,0xFF,0xFF,0xFF,0x21,0x61,0x21,0xFF,0x10,0x62,0xFF,0xFF,0x33,0x73,0x33,0xFF,0x24,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  //LDA,LDX
/*C0*/	0xFF,0xFF,0xFF,0xFF,0x21,0x21,0xFF,0xFF,0xFF,0x22,0xFF,0xFF,0x33,0x33,0xFF,0xFF,0x24,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  //CMP,DEC
/*E0*/	0xFF,0xFF,0xFF,0xFF,0x21,0xFF,0xE1,0xFF,0xFF,0xFF,0x10,0xFF,0x33,0xFF,0xF3,0xFF,0x24,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF   //SBC,INC
};

static __inline u8 ins_table(int addr)
{
	return quickhackfinder_ins_table[addr];
}

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

static __inline int gets8(const u8 *p, int i)
{
	return ((const s8*)p)[i];
}

static const u8 *find_first_instruction(const u8 *initpc, const u8 *lastbank, const u8 **branchpc)
{
	const u8 *pc_limit=initpc+16;
	const u8 *pc=initpc;
	int init_pc_16=initpc-lastbank;
	while (pc < pc_limit)
	{
		u8 ins=ins_table(*pc);
		int len=(ins>>4)&3;
		int action=ins&0x0F;
		if (action==0x0F) return NULL;
		if (action==4)
		{
			int branchlength=gets8(pc,1);
			if (branchlength<-LOOKBACK) return NULL;
			if (branchlength<=-2)
			{
				*branchpc=pc;
				return pc+2+branchlength;
			}
			//check if forward branch goes to a JMP before this PC
			if (forward_branch_is_jump_back(pc,branchlength,init_pc_16,lastbank))
			{
				pc=pc+2+branchlength;
				continue;
			}
		}
		else if (action==5)
		{
			int dest_addr=pc[1]+pc[2]*256;
			//int my_pc=pc-lastbank;
			if (dest_addr>init_pc_16) return NULL;
			if (dest_addr<init_pc_16-LOOKBACK) return NULL;
			*branchpc=pc;
			return memmap_tbl[dest_addr/(PRG_BANK_SIZE*1024)]+dest_addr;
		}
		pc+=len;
	}
	return NULL;
}

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

bool quickhackfinder(const u8 *initpc, const u8 *lastbank, int hacknum)
{
	const u8 *branchpc;
	const u8 *hackpc;
	const u8 *pc;
	
	pc=find_first_instruction(initpc,lastbank,&branchpc);
	if (!pc) return false;
	hackpc=find_hack(pc,branchpc,lastbank,hacknum);
	if (!hackpc) return false;
	return true;
}

int konami_match(const u8 *base_pc, int size)
{
	int hacknumber;
	int X,Y;
	if (size<6 || size>9) return -1;
	for (hacknumber=0;hacknumber<ARRSIZE(konami_speedhack_sizes);hacknumber++)
	{
		if (konami_speedhack_sizes[hacknumber]==size)
		{
			const u8 *hack = konami_speedhacks[hacknumber];
			int i;
			X=-1;
			Y=-1;
			for (i=0;i<size;i++)
			{
				u8 from_rom, from_hack;
				from_rom=base_pc[i];
				from_hack=hack[i];
				if (from_hack == _XXX)
				{
					if (X==-1) X=from_rom;
					if (from_rom!=X) break;
				}
				else if (from_hack == _YYY)
				{
					if (Y==-1) Y=from_rom;
					if (from_rom!=Y) break;
				}
				else
				{
					if (from_rom!=from_hack) break;
				}
			}
			if (i==size)
			{
				return hacknumber;	
			}
		}
	}
	return -1;
}



bool game_specific_hack(const u8 *initpc, const u8 *lastbank, int hacknum)
{
	//capcom uses a specific set of instructions which is quite hackable
	const u8 *jump;
	int jumppc;
	int jumpdest;
	int jumpsize;
	const u8 *hackbase;
	
	jump=(const u8*)memchr(initpc,0x4C,22);
	if (!jump) return false;
	jumppc=jump-lastbank;
	jumpdest=jump[1]+jump[2]*256;
	jumpsize=jumppc-jumpdest;
	hackbase=jumpdest+lastbank;
	
	if (jumpsize==sizeof(capcom_speedhack))
	{
		if (0==memcmp(hackbase,capcom_speedhack,sizeof(capcom_speedhack)))
		{
			speedhack_T *sh= &speedhacks[hacknum];
			sh->hack_pc=jump;
			sh->num_incs=0;
			if (((jumpdest+6)&0xFF00) == ((jumpdest+19)&0xFF00))
			{
				sh->cycles_per_iteration=273;
				sh->divider=0x00F00F01;
			}
			else	//branch crosses pages
			{
				sh->cycles_per_iteration=285;
				sh->divider=0x00E5F36D;
			}
			return true;
		}
	}
	else
	{
		int konami_hack_number;
		konami_hack_number=konami_match(hackbase,jumpsize);
		if (konami_hack_number>=0)
		{
			u32 cyc;
			speedhack_T *sh =&speedhacks[hacknum];
			sh->hack_pc=jump;
			sh->num_incs=0x80+konami_hack_number;
			cyc=konami_speedhack_cycles[konami_hack_number]*3;
			sh->cycles_per_iteration=cyc;
			sh->divider=((u64)0x100000000LL/(u64)cyc)+1;
			return true;
		}
	}
	return false;
}

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
