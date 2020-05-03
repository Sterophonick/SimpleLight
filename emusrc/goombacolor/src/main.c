#include "includes.h"

//#define UI_TILEMAP_NUMBER 30
//#define SCREENBASE (u16*)(MEM_VRAM+UI_TILEMAP_NUMBER*2048)
//#define FONT_MEM (u16*)(MEM_VRAM+0x4000)
#define COLOR_ZERO_TILES (u16*)(MEM_VRAM+0xC000)


#if MOVIEPLAYER
int usinggbamp;
int usingcache;
//File rom_file=NO_FILE;
char save_slot;
#endif

//EWRAM_BSS int ui_y;
EWRAM_BSS u32 oldinput;
EWRAM_BSS u8 *textstart;//points to first GB rom (initialized by boot.s)
EWRAM_BSS u8 *ewram_start;//points to first NES rom (initialized by boot.s)
EWRAM_BSS int roms;//total number of roms
EWRAM_BSS int selectedrom=0;
#if POGOSHELL
EWRAM_BSS char pogoshell_romname[32];	//keep track of rom name (for state saving, etc)
EWRAM_BSS char pogoshell=0;
#endif
#if RTCSUPPORT
EWRAM_BSS char rtc=0;
#endif
EWRAM_BSS char gameboyplayer=0;
EWRAM_BSS char gbaversion;
EWRAM_BSS u32 max_multiboot_size;		//largest possible multiboot transfer (init'd by boot.s)

#define TRIM 0x4D495254

//82048 is an upper bound on the save state size
//The formula is an upper bound LZO estimate on worst case compression
//#define WORSTCASE ((82048)+(82048)/64+16+4+64)

#if GCC
EWRAM_BSS u32 copiedfromrom=0;
int main()
{
	//this function does what boot.s used to do
	extern u8 __rom_end__[]; //using this instead of __end__, because it's also cart compatible
	extern u8 __eheap_start[];
	
	u32 end_addr = (u32)(&__rom_end__);
	textstart = (u8*)(end_addr);
	u32 heap_addr = (u32)(&__eheap_start);
	ewram_start = (u8*)heap_addr;
	
	extern u8 MULTIBOOT_LIMIT[];
	u32 multiboot_limit = (u32)(&MULTIBOOT_LIMIT);
	max_multiboot_size = multiboot_limit - heap_addr;
	
	if (end_addr < 0x08000000 && copiedfromrom)
	{
		textstart += (0x08000000 - 0x02000000);
	}

	bool is_multiboot = (copiedfromrom == 0) && (end_addr < 0x08000000);
	if (is_multiboot)
	{
		//copy appended data to __iwram_lma
		u8* append_src=(u8*)end_addr;
		u8* append_dest=(u8*)ewram_start;
		u8* EWRAM_end=(u8*)0x02040000;
		memmove(append_dest,append_src,EWRAM_end-append_src);
		textstart=append_dest;
		ewram_start=append_dest;
		max_multiboot_size=((u32)(MULTIBOOT_LIMIT))-((u32)(ewram_start));
	}
	
	ewram_canary_1 = 0xDEADBEEF;
	ewram_canary_2 = 0xDEADBEEF;

	C_entry();
	return 0;
	/*
	u32 end_addr=(u32)(&__load_stop_iwram9);
	
	extern u32 MULTIBOOT_LIMIT;
	bool is_multiboot = (copiedfromrom==0)&&(end_addr<0x08000000);
	
	if (is_multiboot)
	{
		//copy appended data to __iwram_lma
		u8* append_src=(u8*)end_addr;
		u8* append_dest=(u8*)(&__iwram_lma);
		u8* EWRAM_end=(u8*)0x02040000;
		memmove(append_dest,append_src,EWRAM_end-append_src);
		textstart=append_dest;
		ewram_start=append_dest;
		max_multiboot_size=((u32)(MULTIBOOT_LIMIT))-((u32)(ewram_start));
	}
	else //running from cart
	{
		//is it compiled for multiboot?
		if (end_addr<0x08000000)
		{
			textstart=(u8*)((end_addr)-0x02000000+0x08000000);
			ewram_start=(u8*)(((u32)__iwram_lma));
			max_multiboot_size=((u32)(MULTIBOOT_LIMIT))-((u32)(ewram_start));
		}
		else
		{
			textstart=(u8*)(end_addr);
			ewram_start=(u8*)0x02000000;
			max_multiboot_size=0;
		}
	}
	max_multiboot_size=((u32)(MULTIBOOT_LIMIT))-((u32)(ewram_start));
	C_entry();
	return 0;
	*/
}

#endif

/*
void loadfont()
{
	LZ77UnCompVram(&font_lz77,FONT_MEM);
}
*/

#if LITTLESOUNDDJ
EWRAM_BSS vu16 *const SC_UNLOCK = (vu16*)0x09FFFFFE;
EWRAM_BSS vu16 *const M3_UNLOCK = (vu16*)0x09FFEFFE;

bool test_mem()
{
	u32 oldbyte;
	u32 deadbeef=0xDEADBEEF;
	vu32* address=(u32*)0x09876540;
	bool success;
	oldbyte=*address;
	*address=deadbeef;
	success=*address==deadbeef;
	*address=oldbyte;
	return success;
}

__inline bool unlock_sc()
{
	//unlock SuperCard
	*SC_UNLOCK = 0xA55A;
	*SC_UNLOCK = 0xA55A;
	*SC_UNLOCK = 0x0005;
	*SC_UNLOCK = 0x0005;
	return test_mem();
}
__inline bool unlock_g6()
{
	//unlock G6
	*SC_UNLOCK = 0xAA55;
	return test_mem();
}
__inline bool unlock_m3()
{
	//unlock M3
	*M3_UNLOCK = 0xAA55;
	return test_mem();
}

int enable_ram()
{
	int cardtype;
	cardtype=0;
	if (test_mem())
	{
		cardtype=4;
	}
	else if (unlock_sc())
	{
		cardtype=1;
	}
	else if (unlock_g6())
	{
		cardtype=2;
	}
	else if (unlock_m3())
	{
		cardtype=3;
	}
	return cardtype;
}
#endif



void C_entry()
{
	int i,j;
	#if RTCSUPPORT
	vu16 *timeregs=(u16*)0x080000c8;
	#endif
	#if POGOSHELL
	u32 temp=(u32)(*(u8**)0x0203FBFC);
	pogoshell=((temp & 0xFE000000) == 0x08000000)?1:0;

	if(pogoshell)
	{
		char *d=(char*)0x203fc08;
		do d++; while(*d);
		do d++; while(*d);
		do d--; while(*d!='/');
		d++;			//d=GB rom name

		roms=1;
		textstart=(*(u8**)0x0203FBFC);
		memcpy(pogoshell_romname,d,32);
	}
	#endif

	#if !GCC
	ewram_start=(u8*)&Image$$RO$$Limit;
	if (ewram_start>=(u8*)0x08000000)
	{
		ewram_start=(u8*)0x02000000;
	}
	#endif



	#if LITTLESOUNDDJ
	enable_ram();
	#endif

	#if RTCSUPPORT
	*timeregs=1;
	#ifndef EZFLASH_OMEGA_BUILD
	if(*timeregs & 1)
	#endif
		rtc=1;
	#endif
	gbaversion=CheckGBAVersion();
	vblankfptr=&vbldummy;
	
	GFX_init_irq();
//	vcountfptr=&vbldummy;
	#if RUMBLE
	SerialIn = 0;
	#endif

	// The maximal space
	#if CARTSRAM
	{
		//buffer1=(ewram_start);
		//buffer2=(ewram_start+0x10000);
		//buffer3=(ewram_start+0x20000);
	}
	#endif

	#if POGOSHELL
	if(!pogoshell)
	#endif
	{
		int gbx_id=0x6666edce;
		u8 *p;
		u8 *q;

		#if MOVIEPLAYER
		if (!disc_IsInserted())
		{
		#endif
		#if SPLASH
		bool wantToSplash = false;
		const u16 *splashImage = NULL;
		//splash screen present?
		p=textstart;
		#if USETRIM
		if(*((u32*)p)==TRIM) p+=((u32*)p)[2];
		#endif
		if(*(u32*)(p+0x104)!=gbx_id) {
			wantToSplash = true;
			splashImage = (const u16*)textstart;
			//splash();
			textstart+=76800;
		}
		#endif	

		i=-1;
		p=textstart;
		//count roms
		do
		{
			#if USETRIM
			if(*((u32*)p)==TRIM)
			{
				q=p+((u32*)p)[2];
				p=p+((u32*)p)[1];
			}
			else
			#endif
			{
				q=p;
				p+=(0x8000<<(*(p+0x148)));
			}
			i++;
		} while (*(u32*)(q+0x104)==gbx_id);
		roms=i;
		if(!i)roms=1;					//Stop Goomba from crashing if there are no ROMs
		
		if (i == 0)
		{
			get_ready_to_display_text();
			cls(3);
			ui_x=0;
			move_ui();
			drawtext(0,"No ROMS found!",0);
			drawtext(1,"Use Goomba Front",0);
			drawtext(2,"to build a compilation ROM,",0);
			drawtext(3,"or use Pogoshell with a",0);
			drawtext(4,"supported flash cartridge.",0);
			drawtext(19,"Goomba Color " VERSION,0);
			while (1)
			{
				waitframe();
			}
		}
		if (wantToSplash)
		{
			splash(splashImage);
		}
		
		#if MOVIEPLAYER
		}
		#endif
	}
	
	//Fade either from white, or from whatever bitmap is visible
	if(REG_DISPCNT==FORCE_BLANK)	//is screen OFF?
	{
		REG_DISPCNT=0;				//screen ON
	}
	//start up graphics
	*MEM_PALETTE=0x7FFF;			//white background
	REG_BLDMOD=0x00ff;				//brightness decrease all
	for (i=0;i<17;i++)
	{
		REG_BLDY=i;
		waitframe();
	}
	*MEM_PALETTE=0;					//black background (avoids blue flash when doing multiboot)
	REG_DISPCNT=0;					//screen ON, MODE0
	
	//clear VRAM
	memset32(MEM_VRAM,0,0x18000);
	
	//new: load VRAM code
	extern u8 __vram1_start[], __vram1_lma[], __vram1_end[];
	int vram1_size = ((((u8*)__vram1_end - (u8*)__vram1_start) - 1) | 3) + 1;
	memcpy32((u32*)__vram1_start,(const u32*)__vram1_lma,vram1_size);
	
	//Start up interrupt system
	GFX_init();
	vblankfptr=&vblankinterrupt;
	


//	vcountfptr=&vcountinterrupt;
#if CARTSRAM
	lzo_init();	//init compression lib for savestates
#endif

	//make 16 solid tiles
	{
		u32*  p=(u32*)COLOR_ZERO_TILES;
		for (i=0;i<16;i++)
		{
			u32 val=(u32)i*(u32)0x11111111;
			for (j=0;j<8;j++)
			{
				*p=val;
				p++;
			}
		}
	}


	//load font+palette
	loadfont();
	loadfontpal();
#if CARTSRAM
	readconfig();
#endif

	jump_to_rommenu();
}

#if SPLASH
//show splash screen
void splash(const u16 *splashImage)
{
	int i;

	REG_DISPCNT=FORCE_BLANK;	//screen OFF
	memcpy((u16*)MEM_VRAM,splashImage,240*160*2);
	waitframe();
	REG_BG2CNT=0x0000;
	REG_DISPCNT=BG2_EN|MODE3;
	for(i=16;i>=0;i--) {	//fade from white
		setbrightnessall(i);
		waitframe();
	}
	for(i=0;i<150;i++) {	//wait 2.5 seconds
		waitframe();
		if (REG_P1==0x030f)
		{
			gameboyplayer=1;
			gbaversion=3;
		}
	}
}
#endif

#if MOVIEPLAYER
int get_saved_sram()
{
	return get_saved_sram_CF(SramName);
}

int get_saved_sram_CF(char* sramname)
{
	if(g_cartflags&2 && g_rammask!=0)
	{	//if rom uses SRAM
		File file;
		file=FAT_fopen(sramname,"r");
		if (file!=NO_FILE)
		{
#if !RESIZABLE
#define XGB_sram XGB_SRAM
#endif
			FAT_fread(XGB_sram,1,g_rammask+1,file);
			FAT_fclose(file);
			return 1;
		}
		return 2;
	}
	else
	{
		return 0;
	}
}
int save_sram_CF(char* sramname)
{
	if(g_cartflags&2 && g_rammask!=0)
	{	//if rom uses SRAM
		File file;
		file=FAT_fopen(sramname,"r+");
		if (file==NO_FILE)
			file=FAT_fopen(sramname,"w");
		if (file!=NO_FILE)
		{
			FAT_fwrite(XGB_sram,1,g_rammask+1,file);
			FAT_fclose(file);
		}
		return 1;
	}
	return 0;
}
#endif

void jump_to_rommenu(void)
{
#if GCC
	extern u8 __sp_usr[];
	u32 newstack=(u32)(&__sp_usr);
	__asm__ volatile ("mov sp,%0": :"r"(newstack));
#else
	__asm {mov r0,#0x3007f00}		//stack reset
	__asm {mov sp,r0}
#endif
	rommenu();
	run(1);
	while (true);
}


void rommenu(void)
{
	cls(3);
	ui_x=0x100;

	setdarkness(16);
//	resetSIO((joycfg&~0xff000000) + 0x40000000);//back to 1P
	
#if MOVIEPLAYER
	usingcache=MOVIEPLAYERDEBUG;
	usinggbamp=0;
#endif
	cls(3);
	make_ui_visible();
#if CARTSRAM
	backup_gb_sram(0); //includes emergency delete menu
#endif

#if MOVIEPLAYER
	if (disc_IsInserted())
	{
		File file;
		usinggbamp=1;
		if (rom_file!=NO_FILE)
		{
			FAT_fclose(rom_file);
		}
		file=cfmenu();
		rom_file=file;
		cls(3);
		
		loadcart(0,g_emuflags&0x300);
//		get_saved_sram_CF(SramName);
	}
	else
#endif

#if POGOSHELL
	if(pogoshell)
	{
		loadcart(0,g_emuflags&0x300);
//#if CARTSRAM
//		get_saved_sram();
//#endif
	}
	else
#endif
	{
		int i,lastselected=-1;
		int key;

		int romz=roms;	//globals=bigger code :P
		int sel=selectedrom;

		oldinput=AGBinput=~REG_P1;

		if(romz>1){
			i=drawmenu(sel);
			loadcart(sel,i|(g_emuflags&0x300));  //(keep old gfxmode)
//#if CARTSRAM
//			get_saved_sram();
//#endif
			lastselected=sel;
			for(i=0;i<8;i++)
			{
				ui_x=224-i*32;
				move_ui_wait();
			}
			waitframe();
			setdarkness(7);			//Lighten screen
		}
		do {
			key=getinput();
			if(key&RIGHT) {
				sel+=10;
				if(sel>romz-1) sel=romz-1;
			}
			if(key&LEFT) {
				sel-=10;
				if(sel<0) sel=0;
			}
			if(key&UP)
				sel=sel+romz-1;
			if(key&DOWN)
				sel++;
			selectedrom=sel%=romz;
			if(lastselected!=sel) {
				i=drawmenu(sel);
				loadcart(sel,i|(g_emuflags&0x300));  //(keep old gfxmode)
//#if CARTSRAM
//				get_saved_sram();
//#endif
				lastselected=sel;
			}
			run(0);
		} while(romz>1 && !(key&(A_BTN+B_BTN+START)));
		for(i=1;i<9;i++)
		{
			setdarkness(8-i);		//Lighten screen
			ui_x=i*32;
			move_ui_scroll();
			run(0);
			move_ui_expose();
		}
		cls(3);	//leave BG2 on for debug output
		while(AGBinput&(A_BTN+B_BTN+START)) {
			AGBinput=0;
			run(0);
		}
	}
#if CARTSRAM
	if(autostate)quickload();
#endif
	setdarkness(0);
	make_ui_invisible();
	
	//run(1);
}

#if MOVIEPLAYER
u8 *findrom(int n)
{
	return cache_location[0];
}

u8 *findrom2(int n)
{
	return cache_location[0];
}
#else

//returns the start address of the ROM (including TRIM header)
u8 *findrom2(int n)
{
	u8 *p=textstart;
#if POGOSHELL
	if (pogoshell) return p;
#endif
	while(n--)
	{
#if USETRIM
		if (*((u32*)p)==TRIM) //trimmed
		{
			p+=((u32*)p)[1];
		}
		else
		{
			p+=(0x8000<<(*(p+0x148)));
		}
#else
		p+=(0x8000<<(*(p+0x148)));
#endif
	}
	return p;
}

//returns the first page of the ROM
u8 *findrom(int n)
{
	u8 *p=findrom2(n);
#if USETRIM
	if (*((u32*)p)==TRIM) //trimmed
	{
		p+=((u32*)p)[2];
	}
#endif
	return p;
}
#endif

char *getcartname(u8 *rom_base)
{
	//assigns cartridge name (with gamecode removed) to global variable, str
	bool gbcmode;
	char *cartname;
	int i;
	bool anyzeroes;

	cartname=str;
	strncpy(cartname,(char*)(rom_base+0x134),16);
	cartname[16]=0;
	gbcmode=rom_base[0x143] >= 0x80;
	if (gbcmode)
	{
		cartname[15]=0;
		anyzeroes=false;
		for (i=11;i<=14;i++)
		{
			if (cartname[i]==0)
			{
				anyzeroes=true;
				break;
			}
		}
		if (!anyzeroes)
		{
			for (i=11;i<=14;i++)
			{
				cartname[i]=0;
			}
		}
	}
	return cartname;
}

//returns options for selected rom
int drawmenu(int sel)
{
	int i,j,topline,toprow,romflags=0;
	int top_displayed_line=0;
	u8 *p;
//	romheader *ri;

	if(roms>20) {
		topline=8*(roms-20)*sel/(roms-1);
		toprow=topline/8;
		j=(toprow<roms-20)?21:20;

		ui_y=topline%8;
	} else {
		int ui_row;
		
		ui_row = (160-roms*8)/2;
		ui_row/=4;
		if (ui_row&1)
		{
			ui_y=4;
			ui_row++;
		}
		ui_row/=2;
		top_displayed_line=ui_row;

		toprow=0;
		j=roms;
	}

	move_ui_wait();

	for(i=0;i<j;i++)
	{
		char *cartname;
		p=findrom(toprow+i);
		cartname=getcartname(p);
		if(roms>1)drawtextl(i+top_displayed_line,cartname,i==(sel-toprow)?1:0,16);
		if(i==sel-toprow) {
			//ri=(romheader*)p;
			//romflags=(*ri).flags|(*ri).spritefollow<<16;
		}
	}
	return romflags;
}

int getinput()
{
	static int lastdpad,repeatcount=0;
	int dpad;
	int keyhit=(oldinput^AGBinput)&AGBinput;
	oldinput=AGBinput;

	dpad=AGBinput&(UP+DOWN+LEFT+RIGHT);
	if(lastdpad==dpad) {
		repeatcount++;
		if(repeatcount<25 || repeatcount&3)	//delay/repeat
			dpad=0;
	} else {
		repeatcount=0;
		lastdpad=dpad;
	}
	EMUinput=0;	//disable game input
	return dpad|(keyhit&(A_BTN+B_BTN+START));
}
/*
void move_ui()
{
	ui_y_real = ui_y;
	move_ui_asm();
}

void cls(int chrmap)
{
	int i,len;
	u32 *scr=(u32*)SCREENBASE;
	if (chrmap&1)
	{
		len=0x540/4;
		for(i=0;i<len;i++)				//512x256
		{
			scr[i]=0x00000000;
		}
	}
	if(chrmap&2)
	{
		len=0x540/4+0x200;
		for(i=0x200;i<len;i++)				//512x256
		{
			scr[i]=0x00000000;
		}
	}
	ui_y=0;
	move_ui();
}
*/

/*
void drawtext(int row,char *str,int hilite)
{
	drawtextl(row,str,hilite,29);
}
void drawtextl(int row,char *str,int hilite,int len)
{
	u16 *here=SCREENBASE+row*32;
	int i=0;

	*here=hilite?0x500a:0x5000;
	hilite=(hilite<<12)+0x5000;
	here++;
	while(str[i]>=' ' && i<len) {
		here[i]=(str[i]-32)|hilite;
		i++;
	}
	for(;i<31;i++)
		here[i]=0x5000;
}
*/
/*
void setdarkness(int dark)
{
	darkness=dark;
	//int ui_layer_bit=(ui_border_visible==3 ? 4 : 8);
	//REG_BLDMOD=0x1FF ^ ui_layer_bit;
	//REG_BLDY=dark;					//Darken screen
	//REG_BLDALPHA=(0x10-dark)<<8;	//set blending for OBJ affected BG0
}

void setbrightnessall(int light)
{
	darkness=light|0x80;
//	REG_BLDMOD=0x00bf;				//brightness increase all
//	REG_BLDY=light;
}
*/

#if 0
void make_ui_visible()
{
	ui_border_visible|=1;
	
	loadfont();
	cls(3);
//	REG_WININ=0x3D3A;  //BG3 visible regardless of window
//	REG_WINOUT=0x3F28; //BG3 visible outside of window
	move_ui();
}

void make_ui_invisible()
{
	ui_border_visible&=~1;
//	REG_WININ=0x353A;  //settings back to normal
//	REG_WINOUT=0x3F20; 
//#if MOVIEPLAYER
//	reload_vram_page1();
//#endif
	move_ui();
}
#endif