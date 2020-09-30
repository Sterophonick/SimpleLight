#include "includes.h"

extern u64 simpleswap32(u32 *A, u32 *B, u32 sizeInBytes);

/*
#include "gba.h"
#include "asmcalls.h"
#include "cache.h"
#include <string.h>
*/

void* memcpy_if_okay(void *dest, const void *src, size_t size)
{
	if (do_not_decompress)
	{
		return NULL;
	}
	else
	{
		return memcpy32(dest,src,size);
	}
}

void simpleswap_if_okay(void *a_in, void* b_in, u32 size)
{
	if (do_not_decompress)
	{
		return;
	}
	else
	{
		simpleswap32(a_in,b_in,size);
	}
}

void assign_chr_pages(u8* base, int start, int end)  //in units of 1k
{
	int i;
	for (i=start;i<end;i++)
	{
		instant_chr_banks[i]=base+i*1024;
	}
}

void assign_prg_pages(u8* base, int start, int end)  //in units of PRG_BANK_SIZE kilobytes
{
	int i;
	for (i=start;i<end;i++)
	{
		instant_prg_banks[i]=base+i*1024*PRG_BANK_SIZE;
	}
}
void assign_prg_pages2(u8* base, int start, int numpages) //in units of PRG_BANK_SIZE kilobytes
{
	assign_prg_pages(base-start*1024*PRG_BANK_SIZE,start,start+numpages);
}
void assign_chr_pages2(u8* base, int start, int numpages)
{
	assign_chr_pages(base-start*1024,start,start+numpages);
}

void swap_prg_pages(int page1, int page2, int numpages)
{
	int i;
	for (i=0;i<numpages;i++)
	{
		u8 *B1,*B2;
		B1=instant_prg_banks[page1+i];
		B2=instant_prg_banks[page2+i];
		simpleswap_if_okay(B1,B2,PRG_BANK_SIZE*1024);
		instant_prg_banks[page1+i]=B2;
		instant_prg_banks[page2+i]=B1;
	}
}

#if USE_GAME_SPECIFIC_HACKS
void make_contiguous(u8* cachebase, int page1, int page2) //16k page size
{
	//Copy bank 3 to EWRAM
	u8* dest=cachebase;
	int page3=page1+1;
	
	u8* bank3_addr=instant_prg_banks[page1*PRG_16];
	u8* bank4_addr=instant_prg_banks[page3*PRG_16];
	u8* bank7_addr=instant_prg_banks[page2*PRG_16];
	int bank3_memtype = (((u32)bank3_addr)&0x0E000000)/0x01000000;
	int bank4_memtype = (((u32)bank4_addr)&0x0E000000)/0x01000000;
	int bank7_memtype = (((u32)bank7_addr)&0x0E000000)/0x01000000;
	
	//Banks 3, 4, 7 none in rom (like when it's compressed)
	if (bank3_memtype!=8 && bank4_memtype!=8 && bank7_memtype!=8)
	{
		swap_prg_pages(page3*PRG_16,page2*PRG_16,1*PRG_16);
	}
	//Rom inside ROM
	if (bank3_memtype==8)
	{
		u8* bank3_dest=dest;
		u8* bank7_dest=dest+16384;
		memcpy_if_okay(bank3_dest,bank3_addr,16384);
		memcpy_if_okay(bank7_dest,bank7_addr,16384);
		assign_prg_pages2(bank3_dest,page1*PRG_16,1*PRG_16);
	}
}
#endif


#if SAVE
void get_rom_map()
{
	//FIXME: bank numbers in savestates are still for 8k banks, they are converted to 4k as they are loaded...
	u8* banks=bank6;
	u8**srammap = (&memmap_6);
	u8**instant_prg = instant_prg_banks;
	u32 sram_W_func;
	int i;
	
	bool dont_mess_with_6000 = false;
	
	//prevent messing up mapper writes
	const u8* sram_w_1 = (u8*)sram_W2;
	const u8* sram_w_2 = (u8*)sram_W;
	const u8* sram_w_3 = (u8*)empty_W;
	u8* current_write_func;
	current_write_func = ((u8*)writemem_6);
	if (current_write_func != sram_w_1 &&
	    current_write_func != sram_w_2 &&
	    current_write_func != sram_w_3)
	{
		dont_mess_with_6000=true;
	}
	
	if (using_flashcart() && (cartflags & SRAM_) )
	{
		sram_W_func=(u32)sram_W2;
	}
	else
	{
		sram_W_func=(u32)sram_W;
	}
	
	for (i=0;i<5;i++)
	{
		if (banks[i]<255)
		{
			#if PRG_BANK_SIZE == 8
			srammap[i]=instant_prg[banks[i]]-(3+i)*8192;
			#endif
			#if PRG_BANK_SIZE == 4
			//int bankNumber = banks[i];
			u8* bankAddress = instant_prg[banks[i]*2] - (3+i)*8192;
			srammap[i*2]=bankAddress;
			srammap[i*2+1]=bankAddress;
			#endif
		}
	}
	
	if (!dont_mess_with_6000)
	{
		if (banks[0]==255)
		{
			//it's sram
			#if PRG_BANK_SIZE == 8
			*((u8**)&readmem_6)=(u8*)sram_R;
			*((u8**)&writemem_6)=(u8*)sram_W_func;
			*((u8**)&memmap_6)=(u8*)(NES_RAM-0x5800);
			#endif
			#if PRG_BANK_SIZE == 4
			*((u8**)&readmem_6)=(u8*)sram_R;
			*((u8**)&readmem_7)=(u8*)sram_R;
			*((u8**)&writemem_6)=(u8*)sram_W_func;
			*((u8**)&writemem_7)=(u8*)sram_W_func;
			*((u8**)&memmap_6)=(u8*)(NES_RAM-0x5800);
			*((u8**)&memmap_7)=(u8*)(NES_RAM-0x5800);
			#endif
		}
		else
		{
			//it's rom
			#if PRG_BANK_SIZE == 8
			*((u8**)&readmem_6)=(u8*)rom_R60;
			*((u8**)&writemem_6)=(u8*)empty_W;
			#endif
			#if PRG_BANK_SIZE == 4
			*((u8**)&readmem_6)=(u8*)rom_R60;
			*((u8**)&readmem_7)=(u8*)rom_R60;
			*((u8**)&writemem_6)=(u8*)empty_W;
			*((u8**)&writemem_7)=(u8*)empty_W;
			#endif
		}
	}
}
void get_vram_map()
{
	u8* banks=Cbank0;
	u8**vrammap =vram_map;
	u8**instant_chr = instant_chr_banks;
	int i;
	
	for (i=0;i<8;i++)
	{
		vrammap[i]=instant_chr[banks[i]];
	}
}
void rebankswitch()
{
	get_rom_map();
	get_vram_map();
}
#endif
