#include "includes.h"

#define page_size (16)
#define page_size_2 (page_size*1024)

#define CRAP_AMOUNT 512

u8 *const bank_1=(u8*)0x06010000-CRAP_AMOUNT;

#if MOVIEPLAYER
int cache_queue_cursor;
u16 prgcache[MAX_CACHE_SIZE];
u8* cache_location[MAX_CACHE_SIZE];
u8* cachebase;
u8 cachepages;
u32* pageoffsets;
#endif

u8 *make_instant_pages(u8* rom_base)
{
	//this is for cases where there is no caching!
	u32 *p=(u32*)rom_base;
	u8 *page0_rom;
//	u8 cartsizebyte;
	int i;
	
#if MOVIEPLAYER
	if (usingcache)
	{
		return rom_base;
	}
#endif

#if USETRIM
	if (*p==TRIM)
	{
		p+=2;
//		num_pages=p[0]/4-8;
//		page_mask=num_pages-1;
		for (i=0;i<256;i++)
		{
			INSTANT_PAGES[i]=rom_base+p[i];//&page_mask];
		}
	}
	else
#endif
	{
//		num_pages=(2<<rom_base[148]);
//		page_mask=num_pages-1;
		for (i=0;i<256;i++)
		{
			INSTANT_PAGES[i]=rom_base+16384*(i);//&page_mask);
		}
	}
	page0_rom=INSTANT_PAGES[0];
//	cartsizebyte=page0_rom[0x148];

//	if (cartsizebyte>0)
	{
		//copy bank 0 to VRAM
//		memcpy(bank_1,page0_rom,16384);
		memcpy(bank_1,page0_rom,16384+CRAP_AMOUNT);
		INSTANT_PAGES[0]=bank_1;
	}
	return page0_rom;
}

#if !MOVIEPLAYER
void init_cache() {}
#endif

#if MOVIEPLAYER

void init_cache()
{
	int i;
	u8* dest=ewram_start;
	
#if RESIZABLE
	u8 *end_of_cache=END_of_exram;
#else
	u8 *end_of_cache=(u8*)(&END_OF_EXRAM);
#endif
	if (!usingcache) return;
	
//	g_banks[0]=0;
//	g_banks[1]=1;
	
	cachebase=dest;
	cachepages=(end_of_cache-cachebase)/page_size_2;
	//set up cache locations, first few are sequential
	for (i=0;i<cachepages;i++)
	{
		cache_location[i+1]=cachebase+page_size_2*i;
	}
	//extra page in VRAM to accelerate games
	cache_location[0]=bank_1;
	cachepages++;
	
	clear_instant_prg();
	flushcache();
//	usingcompcache=0;
}

void flushcache()
{
	int i;

	for (i=0;i<cachepages;i++)
	{
		prgcache[i]=65535;
	}
	cache_queue_cursor=0;
}

void clear_instant_prg()
{
	int i;
	int l = 256;
	u32 *instant_prg = (u32*)INSTANT_PAGES;
	for (i=0;i<l;i++)
	{
		instant_prg[i]=0xC0000000;
	}
}

void regenerate_instant_prg()
{
	int i;
	u8**instant_prg=INSTANT_PAGES;

	for (i=0;i<cachepages;i++)
	{
		int p=prgcache[i];
		if (p<65535)
		{
			instant_prg[p]=cache_location[i];
		}
	}
}

void loadcachepage(int i,int bank) //i=slot, bank=bank that goes into the slot
{
	u8 *dest;
	if (usinggbamp)
	{
		FAT_fseek(rom_file, bank*page_size_2,SEEK_SET);
		dest =cache_location[i];
		FAT_fread(dest,1,page_size_2,rom_file);
	}
	else
	{
		dest =cache_location[i];
		memcpy(dest,romstart+bank*page_size_2,page_size_2);
		waitframe();
		waitframe();
		waitframe();
	}
}

void getbank(int kilobyte)
{
	int bank;
	u32 i,j;
	int slotcontent;
//	u8 *src, *dest;
	u32 *banks=g_banks;
	bank=kilobyte/page_size;
	
	//page is in cache?
	for (i=0;i<cachepages;i++)
	{
		if (prgcache[i]==bank)
		{
			return;
		}
	}
	
slot_is_locked:
	i=cache_queue_cursor;
	cache_queue_cursor++;
	slotcontent=prgcache[i];
	if (cache_queue_cursor>=cachepages) cache_queue_cursor=0;

	for (j=0;j<2;j++)
	{
		if (slotcontent!=65535)
		{
			if (banks[j]*16/page_size==slotcontent)
			{
				goto slot_is_locked;
			}
		}
	}
	prgcache[i]=bank;

#if 0
	if (usingcompcache)
	{
		int srcoffset;
		srcoffset = 16 + pageoffsets[bank];
//		dest = (u8*)06014000;
//		FAT_fseek(rom_file,srcoffset,SEEK_SET);
//		src=FAT_fread_16(dest,1,16384,rom_file);

		dest = cachebase+0x4000*cachepages;
		FAT_fseek(rom_file,srcoffset,SEEK_SET);
		FAT_fread(dest,1,16384,rom_file);
		src=dest;
		dest =cachebase+0x4000*i;
		depack(src,dest);
	}
	else
#endif
	loadcachepage(i,bank);
}

void get_rom_map()
{
	u32 *banks=g_banks;
	u8**memmap = g_memmap_tbl;
	u8**instant_prg = INSTANT_PAGES;
	int i;
	int j;

	for (i=0;i<2;i++)
	{
		u8* data=instant_prg[banks[i]]-(i*16384);
		for (j=0;j<4;j++)
		{
			memmap[i*4+j]=data;
		}
	}
}
void update_cache()
{
	//updates the cache's state, and all the lookup tables
	//also fixes the memory map and vram map
	u32 *banks=g_banks;
	int i;

	clear_instant_prg();
	for (i=0;i<2;i++)
	{
		getbank(banks[i]*16);
	}
	regenerate_instant_prg();
	get_rom_map();
}

#if RESIZABLE
void add_exram()
{
	GBC_exramsize=0x6000;
	GBC_exram=END_of_exram-GBC_exramsize;
	memset(GBC_exram,0,GBC_exramsize);
	END_of_exram=GBC_exram;
	init_cache();
	update_cache();
}
#endif

/*
void reload_vram_page1()
{
	int i=cachepages-2;
	int bank=prgcache[i];
	if (bank<65535)
	{
		loadcachepage(i,bank);
	}
}
*/

#endif
