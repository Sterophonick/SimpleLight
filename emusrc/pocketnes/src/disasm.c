#include "includes.h"

#if DEBUG

typedef enum 
{
	implied=0,
	relative,
	reg_a,
	indirect,
	absolute_j,

	x_ind,
	zeropage,
	immediate,
	absolute,
	ind_y,
	zpg_x,
	abs_y,
	abs_x,
	
	zpg_y,
	
	illegal
} Addr_Mode_T;

const char branches[8][4]=
{
	"BPL",
	"BMI",
	"BVC",
	"BVS",
	"BCC",
	"BCS",
	"BNE",
	"BEQ"
};
const char aluops[8][4]=
{
	"ORA",
	"AND",
	"EOR",
	"ADC",
	"STA",
	"LDA",
	"CMP",
	"SBC"
};
const char aluops2[8][4]=
{
	"ASL",
	"ROL",
	"LSR",
	"ROR",
	"STX",
	"LDX",
	"DEC",
	"INC"
};
const char col0[8][4]=
{
	"BRK",
	"JSR",
	"RTI",
	"RTS",
	"???",
	"LDY",
	"CPY",
	"CPX"
};
const char col0C[8][4]=
{
	"???",
	"BIT",
	"JMP",
	"JMP",
	"STY",
	"LDY",
	"CPY",
	"CPX"
};
const char addrmodes_04[]=
{
	illegal,
	illegal,
	zeropage,
	illegal,
	illegal,
	illegal,
	illegal,
	illegal,
	zeropage,
	zpg_x,
	zeropage,
	zpg_x,
	zeropage,
	illegal,
	zeropage,
	illegal
};
const char addrmodes_0C[]=
{
	illegal,
	illegal,
	absolute,
	illegal,
	absolute_j,
	illegal,
	indirect,
	illegal,
	absolute,
	illegal,
	absolute,
	abs_x,
	absolute,
	illegal,
	absolute,
	illegal
};
const char col0A[16][4]=
{
	"ASL",
	"???",
	"ROL",
	"???",
	"LSR",
	"???",
	"ROR",
	"???",
	"TXA",
	"TXS",
	"TAX",
	"TSX",
	"DEX",
	"???",
	"NOP",
	"???"
};
const char col08[16][4]=
{
	"PHP",
	"CLC",
	"PLP",
	"SEC",
	"PHA",
	"CLI",
	"PLA",
	"SEI",
	"DEY",
	"TYA",
	"TAY",
	"CLV",
	"INY",
	"CLD",
	"INX",
	"SED"
};

const char addrmodes_0A[]=
{
	reg_a,	
	illegal,
	reg_a,
	illegal,
	reg_a,
	illegal,
	reg_a,
	illegal,
	implied,
	implied,
	implied,
	implied,
	implied,
	illegal,
	implied,
	illegal
};

u8 readmem_safe(int addr)
{
	const u8 *mem;
	mem = memmap_tbl[(addr&PRG_BANK_MASK)/(1024*PRG_BANK_SIZE)];
	if (addr<0x2000) addr &=0x7FF;
	else if (addr<0x6000) return 0xFF;
	return mem[addr];
}

u16 readmem_safe_16(int addr)
{
	return readmem_safe(addr)+readmem_safe(addr+1)*256;
}

u8 readmem_zp(int addr)
{
	addr=addr&0xFF;
	return NES_RAM[addr];
}
u16 readmem_zp_16(int addr)
{
	return readmem_zp(addr)+readmem_zp(addr+1)*256;
}



void disassemble(char *output, int x, int y, int pc)
{
	u8 ins = readmem_safe(pc);
	int addrmode;
	const char *name;
	
	addrmode=illegal;
	
	if ((ins&0x1F) == 0x10)				//branches from column 0
	{
		int branch_number=ins/0x20;
		addrmode=relative;
		name=branches[branch_number];
	}
	else if ((ins & 0x03) == 0x01)		//alu operations
	{
		int alu_op_number=ins/0x20;
		int addr_mode_number = (ins & 0x1C)/4;
		addrmode = x_ind + addr_mode_number;
		name=aluops[alu_op_number];
		
		if (ins==0x89) addrmode=illegal;
	}
	else if ((ins & 0x07) == 0x6)		//alu operations 2
	{
		int alu_op_number=ins/0x20;
		int addr_mode_number = (ins & 0x1C)/4;
		addrmode = x_ind + addr_mode_number;
		name=aluops2[alu_op_number];
		if (ins>=0x80)
		{
			if (addrmode == zpg_x) addrmode=zpg_y;
			if (addrmode == abs_x) addrmode=abs_y;
			if (ins==0x9E) addrmode=illegal;
		}
	}
	//remaining: columns 0,2,4,8,A,C
	else if ((ins & 0x0F) == 0)
	{
		int ins_number=ins/0x20;
		addrmode=implied;
		name=col0[ins_number];
		if (ins==0x20) addrmode=absolute_j; //JSR
		if (ins>0x80) addrmode=immediate; //LDY #, CPY #, CPX #
		if (ins==0x80) addrmode=illegal;
	}
	else if ((ins & 0x0F) == 0x04)
	{
		int ins_number=ins/0x20;
		name=col0C[ins_number];
		addrmode=addrmodes_04[ins/0x10];
	}
	else if ((ins & 0x0F) == 0x0C)
	{
		int ins_number=ins/0x20;
		name=col0C[ins_number];
		addrmode=addrmodes_0C[ins/0x10];
	}
	else if ((ins & 0x0F) == 0x0A)
	{
		int ins_number=ins/0x10;
		name=col0A[ins_number];
		addrmode=addrmodes_0A[ins/0x10];
	}
	else if ((ins & 0x0F) == 0x08)
	{
		name=col08[ins/0x10];
		addrmode=implied;
	}
	else if (ins==0xA2)
	{
		name="LDX";
		addrmode=immediate;
	}
	
	//make all illegal instructions into "???"
	if (addrmode==illegal)
	{
		name="???";
	}
	if (0==strcmp(name,"???"))
	{
		addrmode=illegal;
	}
	
	{
		//char *cursor;
		
		u16 addr16 = readmem_safe_16(pc+1);
		u8 imm = readmem_safe(pc+1);
		u16 addrat;
		u8 valueat;

		//cursor=output;
		
		strmerge3(output,"$",hex4(pc),":");
		strmerge3(output,output,hex2(ins)," ");
		//cursor+=siprintf(cursor,"$%04X:%02X ",pc,ins);
		
		
		
		//write bytes to output string
		switch (addrmode)
		{
			case implied:
			case illegal:
			case reg_a:
				//one byte
				strcat(output,"       ");
				//cursor+=siprintf(cursor,"       ");
				break;
			case relative:
			case immediate:
			case x_ind:
			case ind_y:
			case zeropage:
			case zpg_x:
			case zpg_y:
				//two bytes
				strmerge3(output,output,hex2(imm),"     ");
				//cursor+=siprintf(cursor,"%02X     ",imm);
				break;
			case absolute_j:
			case absolute:
			case abs_y:
			case abs_x:
			case indirect:
				//three bytes
				strmerge3(output,output,hex2(imm)," ");
				strmerge3(output,output,hex2(readmem_safe(pc+2)),"  ");
				//cursor+=siprintf(cursor,"%02X %02X  ",imm,readmem_safe(pc+2));
				break;
		}
		
		//write instruction to output string
		switch (addrmode)
		{
			case implied:
			case illegal:
				strcat(output,name);
				//cursor+=siprintf(cursor,"%s",name);
				break;
			case immediate:
				strmerge4(output,output,name," #$",hex2(imm));
				//cursor+=siprintf(cursor,"%s #$%02X",name,imm);
				break;
			case reg_a:
				strcat(output,name);
				//cursor+=siprintf(cursor,"%s",name);
				break;
			case absolute_j:
				strmerge4(output,output,name," $",hex4(addr16));
				//cursor+=siprintf(cursor,"%s $%04X",name,addr16);
				break;
			case relative:
				addrat=pc+2+(s8)imm;
				strmerge4(output,output,name," $",hex4(addrat));
				//cursor+=siprintf(cursor,"%s $%04X",name,addrat);
				break;
			case absolute:
				valueat=readmem_safe(addr16);
				strmerge4(output,output,name," $",hex4(addr16));
				strmerge3(output,output," = #$",hex2(valueat));
				//cursor+=siprintf(cursor,"%s $%04X = #$%02X",name,addr16,valueat);
				break;
			case zeropage:
				valueat=readmem_zp(imm);
				strmerge4(output,output,name," $",hex4(imm));
				strmerge3(output,output," = #$",hex2(valueat));
				//cursor+=siprintf(cursor,"%s $%04X = #$%02X",name,imm,valueat);
				break;
			case abs_x:
				addrat=addr16+x;
				valueat=readmem_safe(addrat);
				strmerge4(output,output,name," $",hex4(addr16));
				strmerge3(output,output,",X @ $", hex4(addrat));
				strmerge3(output,output," = #$", hex2(valueat));
				//cursor+=siprintf(cursor,"%s $%04X,X @ $%04X = #$%02X",name,addr16,addrat,valueat);
				break;
			case abs_y:
				addrat=addr16+y;
				valueat=readmem_safe(addrat);
				strmerge4(output,output,name," $",hex4(addr16));
				strmerge3(output,output,",Y @ $", hex4(addrat));
				strmerge3(output,output," = #$", hex2(valueat));
				//cursor+=siprintf(cursor,"%s $%04X,Y @ $%04X = #$%02X",name,addr16,addrat,valueat);
				break;
			case indirect:
				addrat=readmem_safe_16(addr16);
				strmerge4(output,output,name," ($",hex4(addr16));
				strmerge3(output,output,") = $", hex4(addrat));
				//cursor+=siprintf(cursor,"%s ($%04X) = $%04X",name,addr16,addrat);
				break;
			case zpg_x:
				addrat=(imm+x)&0xFF;
				valueat=readmem_zp(addrat);
				strmerge4(output,output,name," $",hex2(imm));
				strmerge3(output,output,",X @ $", hex4(addrat));
				strmerge3(output,output," = #$", hex2(valueat));
				//cursor+=siprintf(cursor,"%s $%02X,X @ $%04X = #$%02X",name,imm,addrat,valueat);
				break;
			case zpg_y:
				addrat=(imm+y)&0xFF;
				valueat=readmem_zp(addrat);
				strmerge4(output,output,name," $",hex2(imm));
				strmerge3(output,output,",Y @ $", hex4(addrat));
				strmerge3(output,output," = #$", hex2(valueat));
				//cursor+=siprintf(cursor,"%s $%02X,Y @ $%04X = #$%02X",name,imm,addrat,valueat);
				break;
			case x_ind:
				addrat=(imm+x)&0xFF;
				addrat=readmem_zp_16(addrat);
				valueat=readmem_safe(addrat);
				strmerge4(output,output,name," ($",hex2(imm));
				strmerge3(output,output,",X) @ $", hex4(addrat));
				strmerge3(output,output," = #$", hex2(valueat));
				//cursor+=siprintf(cursor,"%s ($%02X,X) @ $%04X = #$%02X",name,imm,addrat,valueat);
				break;
			case ind_y:
				addrat=readmem_zp_16(imm);
				addrat+=y;
				valueat=readmem_safe(addrat);
				strmerge4(output,output,name," ($",hex2(imm));
				strmerge3(output,output,"),Y @ $", hex4(addrat));
				strmerge3(output,output," = #$", hex2(valueat));
				//cursor+=siprintf(cursor,"%s ($%02X),Y @ $%04X = #$%02X",name,imm,addrat,valueat);
				break;
		}
	}
}

#endif
