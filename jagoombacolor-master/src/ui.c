#include "includes.h"

EWRAM_BSS u8 autoA,autoB;				//0=off, 1=on, 2=R
EWRAM_BSS u8 stime=0;
EWRAM_BSS u8 autostate=0;
EWRAM_BSS int main_ui_selection;
//EWRAM_BSS int selected;//selected menuitem.  used by all menus.
EWRAM_BSS int mainmenuitems;//? or CARTMENUITEMS, depending on whether saving is allowed
//EWRAM_BSS u32 oldkey;//init this before using getmenuinput

#ifndef ARRSIZE
#define ARRSIZE(xxxx) (sizeof((xxxx))/sizeof((xxxx)[0]))
#endif

int savestate(void* dest){return 0;}
void loadstate(int foo, void* dest){}

#if !CARTSRAM
//void savestatemenu(){} void loadstatemenu(){} void managesram(){}
void quicksave(){} void quickload(){}
#endif
#if !MULTIBOOT
//void multiboot(){}
#endif

EWRAM_BSS char str[32]; //ZOMG global variable!
int print_2_func(int row, const char *src1, const char *src2);
int print_1_func(int row, const char *src1, const char *src2);
int strmerge_str(int unused, const char *src1, const char *src2);
int text2_str(int row);
int text1_str(int row);
//#if CHEATFINDER
//int print_cheatfinder_line_func(int row, const char *oper, int value);
//#endif

int print_1_func(int row,const char *src1,const char *src2)
{
	row=strmerge_str(row,src1,src2);
	return text1_str(row);
}

int print_2_func(int row,const char *src1,const char  *src2)
{
	row=strmerge_str(row,src1,src2);
	return text2_str(row);
}
int strmerge_str(int unused, const char  *src1,const char  *src2) {
	char *dst=str;
	if(dst!=src1)
		strcpy(dst,src1);
	strcat(dst,src2);
	return unused;
}

int text1_str(int row)
{
	drawtext(row+10-mainmenuitems/2,str,selected==row);
	return row+1;
}
int text2_str(int row)
{
	drawtext(35+row+2,str,selected==row);
	return row+1;
}

#define print_1(xxxx,yyyy) row=print_1_func(row,(xxxx),(yyyy))
#define print_2(xxxx,yyyy) row=print_2_func(row,(xxxx),(yyyy))
#define print_1_1(xxxx) row=text(row,(xxxx));
#define print_2_1(xxxx) row=text2(row,(xxxx));

//#define MENU2ITEMS 8+SPEEDHACKS_OLD			//othermenu items
//#define MENU3ITEMS 3			//displaymenu items
//
////mainmenuitems when running from cart (not multiboot)
//#define CARTMENUITEMS 7+MULTIBOOT+GOMULTIBOOT+(CARTSRAM*3)
//#define MULTIBOOTMENUITEMS 7+MULTIBOOT	//"" when running from multiboot
//
//const char MENUXITEMS[]={CARTMENUITEMS,MULTIBOOTMENUITEMS,MENU2ITEMS,MENU3ITEMS};

const fptr multifnlist[]={autoBset,autoAset,ui3,ui2,ui4,
#if MULTIBOOT
multiboot,
#endif
sleep_,restart,exit_};

const fptr fnlist1[]={autoBset,autoAset,ui3,ui2,ui4,
#if MULTIBOOT
multiboot,
#endif
#if CARTSRAM
savestatemenu,loadstatemenu,managesram,
#endif
sleep_,
#if GOMULTIBOOT
go_multiboot,
#endif
restart,exit_};

const fptr fnlist2[]={vblset,fpsset,sleepset,swapAB,autostateset,
#if SPEEDHACKS_OLD
autodetect_speedhack,
#endif
gbtype,changeautoborder,gbatype};
const fptr fnlist3[]={chpalette,brightset,sgbpalnum};

const fptr fnlist4[]={
#if SPEEDHACKS_OLD
autodetect_speedhack,
#endif
timermode,changelcdhack}; //,changedmamode};

const fptr *const fnlistX[]={fnlist1,multifnlist,fnlist2,fnlist3,fnlist4};
const fptr drawuiX[]={drawui1,drawui1,drawui2,drawui3,drawui4};
const char MENUXITEMS[]=
{
	ARRSIZE(fnlist1),ARRSIZE(multifnlist),ARRSIZE(fnlist2),ARRSIZE(fnlist3),ARRSIZE(fnlist4)
};

/*
u32 getmenuinput(int menuitems)
{
	u32 keyhit;
	u32 tmp;
	int sel=selected;

	waitframe();		//(polling REG_P1 too fast seems to cause problems)
	tmp=~REG_P1;
	keyhit=(oldkey^tmp)&tmp;
	oldkey=tmp;
	if(keyhit&UP)
		sel=(sel+menuitems-1)%menuitems;
	if(keyhit&DOWN)
		sel=(sel+1)%menuitems;
	if(keyhit&RIGHT) {
		sel+=10;
		if(sel>menuitems-1) sel=menuitems-1;
	}
	if(keyhit&LEFT) {
		sel-=10;
		if(sel<0) sel=0;
	}
	if((oldkey&(L_BTN+R_BTN))!=L_BTN+R_BTN)
		keyhit&=~(L_BTN+R_BTN);
	selected=sel;
	return keyhit;
}
*/

void ui()
{
	int key,soundvol,tm0cnt,i;
	int mb=(u32)textstart<0x8000000;
	int savesuccess=1;
	int usefade=1;

	main_ui_selection = 0;
	make_ui_visible();

	autoA=joycfg&A_BTN?0:1;
	autoA|=joycfg&(A_BTN<<16)?0:2;
	autoB=joycfg&B_BTN?0:1;
	autoB|=joycfg&(B_BTN<<16)?0:2;

	mainmenuitems=MENUXITEMS[mb];//running from rom or multiboot?
	FPSValue=0;					//Stop FPS meter

	soundvol=REG_SGCNT0_L;
	REG_SGCNT0_L=0;				//stop sound (GB)
	tm0cnt=REG_TM0CNT;
	REG_TM0CNT=0;				//stop sound (directsound)

#if MOVIEPLAYER
	if (usinggbamp)
	{
		if(g_cartflags&2 && g_rammask!=0)
		{
			ui_x=0;
			move_ui();
			cls(3);
			drawtext(9,"         Saving...",0);
			usefade=0;
			setdarkness(7);
			save_sram_CF(SramName);	
		}
	}
#endif
	selected=0;
	main_ui_selection = selected;
	drawui1();
	if (usefade)
	{
		scrollr(1);
		/*
		for(i=0;i<8;i++)
		{
			setdarkness(i);		//Darken game screen
			ui_x=224-i*32;
			move_ui_wait();
		}
		*/
	}
#if CARTSRAM
	savesuccess=backup_gb_sram(1);
#endif
	if (!savesuccess)
	{
		drawui1();
		ui_x=0;
		move_ui();
	}
	

	oldkey=~REG_P1;			//reset key input
	do {
		drawclock();
		key=getmenuinput(mainmenuitems);
		main_ui_selection = selected;
		if(key&(A_BTN)) {
			fnlistX[mb][selected]();
			selected = main_ui_selection;
			if (mb != ((u32)textstart<0x8000000))
			{
				mb=1;
				selected=0;
				main_ui_selection = selected;
			}
		}
		if(key&(A_BTN+UP+DOWN+LEFT+RIGHT))
			drawui1();
	} while(!(key&(B_BTN+R_BTN+L_BTN)));
#if CARTSRAM
	if (get_sram_owner()==0)
	{
		get_saved_sram();
	}
#ifndef EZFLASH_OMEGA_BUILD
	writeconfig();			//save any changes
#endif
#endif
	scrolll(2);
	/*
	for(i=1;i<9;i++)
	{
		setdarkness(8-i);	//Lighten screen
		ui_x=i*32;
		move_ui_wait();
	}
	*/
	while(key&(B_BTN)) {
		waitframe();		//(polling REG_P1 too fast seems to cause problems)
		key=~REG_P1;
	}
	REG_SGCNT0_L=soundvol;	//resume sound (GB)
	REG_TM0CNT=tm0cnt;		//resume sound (directsound)
	cls(3);
	make_ui_invisible();
	main_ui_selection = -1;
}

void subui(int menunr)
{
	int key, oldsel;

	selected=0;
	drawuiX[menunr]();
	scrolll(0);
	oldkey=~REG_P1;			//reset key input
	do {
		key=getmenuinput(MENUXITEMS[menunr]);
		if(key&(A_BTN)) {
			oldsel=selected;
			fnlistX[menunr][selected]();
			selected=oldsel;
		}
		if(key&(A_BTN+UP+DOWN+LEFT+RIGHT+L_BTN+R_BTN)) {
			drawuiX[menunr]();
		}
	} while(!(key&(B_BTN+R_BTN+L_BTN)));
	drawui1();
	scrollr(0);
	while(key&(B_BTN+L_BTN+R_BTN)) {
		waitframe();		//(polling REG_P1 too fast seems to cause problems)
		key=~REG_P1;
	}
}

void ui2()
{
	subui(2);
}
void ui3()
{
	setdarkness(0);
	subui(3);
	setdarkness(8);
}
void ui4()
{
	subui(4);
}


int text(int row,char *str) {
	drawtext(row+10-mainmenuitems/2,str,selected==row);
	return row+1;
}
int text2(int row,char *str) {
	drawtext(35+row+2,str,selected==row);
	return row+1;
}


//trying to avoid using sprintf...  (takes up almost 3k!)
void strmerge(char *dst,char *src1,char *src2)
{
	if(dst!=src1)
		strcpy(dst,src1);
	strcat(dst,src2);
}

char *const autotxt[]={"OFF","ON","with R"};
char *const vsynctxt[]={"ON","OFF","SLOWMO"};
char *const sleeptxt[]={"5min","10min","30min","OFF"};
char *const brightxt[]={"I","II","III","IIII","IIIII"};
char *const hostname[]={"Crap","Prot","GBA","GBP","NDS"};
char *const ctrltxt[]={"1P","2P","Link2P","Link3P","Link4P"};
char *const bordtxt[]={"Black","Grey","Blue","None"};
char *const paltxt[]=
{
"Pea Soup",
"Grayscale",
"Multi 1",
"Multi 2",
"Zelda.",
"Metroid",
"Adventure Island",
"Adventure Island 2",
"Balloon Kid",
"Batman",
"Batman - Return of the Joker",
"Bionic Commando",
"Castlevania Adventure",
"Dr. Mario",
"Kirby",
"Donkey Kong Land",
"Yellow",
"SGB 1A",
"SGB 1B",
"SGB 1C",
"SGB 1D",
"SGB 1E",
"SGB 1F",
"SGB 1G",
"SGB 1H",
"SGB 2A",
"SGB 2B",
"SGB 2C",
"SGB 2D",
"SGB 2E",
"SGB 2F",
"SGB 2G",
"SGB 2H",
"SGB 3A",
"SGB 3B",
"SGB 3C",
"SGB 3D",
"SGB 3E",
"SGB 3F",
"SGB 3G",
"SGB 3H",
"SGB 4A",
"SGB 4B",
"SGB 4C",
"SGB 4D",
"SGB 4E",
"SGB 4F",
"SGB 4G",
"SGB 4H",
"Balloon Kid",
"Tetris",
"Alleyway",
"Mario's Picross",
"Space Invaders",
"X",
"Pocket Camera",
"Radar Mission",
"Pokemon Blue",
"Kaeru no Tame ni Kane wa Naru",
"Pokemon Red",
"Pokemon Green",
"Dr. Mario",
"Pinocchio",
"Mole Mania",
"Game and Watch Gallery",
"Yoshi",
"Donkey Kong",
"Kirby's Pinball Land",
"Super Mario Land",
"Pocket Bomberman",
"Kid Icarus",
"Golf",
"Dynablaster",
"Battletoads",
"Wario Blast",
"Tetris Attack",
"Yoshi's Cookie",
"Qix",
"Wario Land",
"Donkey Kong Land",
"Soccer",
"Baseball",
"Kirby's Dream Land",
"Super Mario Land 2",
"Wave Race",
"Donkey Kong Land 2",
"Killer Instinct",
"Mystic Quest",
"Mega Man",
"Zelda Link's Awakening",
"Star Wars",
"Metroid II",
"Wario Land II",
"Pac-in-Time"
};
char *const gbtxt[]={"GB","Prefer SGB over GBC","Prefer GBC over SGB","GBC+SGB","Prefer GBC over GB"};
char *const clocktxt[]={"Full","Half speed"};
char *const lcdhacktxt[]={"OFF","Low","Medium","High"};
//char *const dmamodetxt[]={"Buffered", "Direct to VRAM", "WayForward"};

#define EMUNAME "Jagoomba Color"
//char *const emuname = "Goomba Color ";
char *const palnumtxt[]={"0","1","2","3"};

#if SPEEDHACKS_OLD
char *const hacknames[]={"jr nz","jr z", "jr nc", "jr c"};
#endif

void drawui1()
{
	int oldsel = selected;
	int row=0;
	cls(1);
	if (main_ui_selection == -1)
	{
		return;
	}
	selected = main_ui_selection;
	
	strmerge(str,EMUNAME " " VERSION " on ",hostname[(u32)gbaversion]);
	drawtext(18,str,0);
	drawtext(19,"By FluBBa, Dwedit, Jaga",0);

	print_1("B autofire: ",autotxt[autoB]);
	print_1("A autofire: ",autotxt[autoA]);
	print_1_1("Display->");
	print_1_1("Other Settings->");
	print_1_1("Speed Hacks->");
#if MULTIBOOT
	print_1_1("Link Transfer");
#endif
	if(mainmenuitems==ARRSIZE(multifnlist)) {
		print_1_1("Sleep");
	} else {
#if CARTSRAM
		print_1_1("Save State->");
		print_1_1("Load State->");
		print_1_1("Manage SRAM->");
#endif
		print_1_1("Sleep");
#if GOMULTIBOOT
		print_1_1("Go Multiboot");
#endif
	}
	print_1_1("Restart");
	print_1_1("Exit");
	
	selected = oldsel;
}

void drawui2()
{
	int row=0;
	cls(2);
	drawtext(32,"       Other Settings",0);
	print_2("VSync: ",vsynctxt[novblankwait]);
	print_2("FPS-Meter: ",autotxt[fpsenabled]);
	print_2("Autosleep: ",sleeptxt[stime]);
	print_2("Swap A-B: ",autotxt[(joycfg>>10)&1]);
	print_2("Autoload state: ",autotxt[autostate&1]);
	print_2("Game Boy: ",gbtxt[request_gb_type]);
	print_2("Auto SGB border: ",autotxt[auto_border]);
	print_2("Identify as GBA: ",autotxt[request_gba_mode]);
}

void drawui3()
{
	int row=0;
	cls(2);
	drawtext(32,"      Display Settings",0);
	print_2("Palette: ",paltxt[palettebank]);
	print_2("Gamma: ",brightxt[gammavalue]);
	print_2("SGB Palette Number: ",palnumtxt[sgb_palette_number]);
}

void drawui4()
{
	int row=0;
	cls(2);
	/////////////0123456789ABCDEF0123456789ABCD
	drawtext(32, "        Speed Hacks",0);
#if SPEEDHACKS_OLD
	if (g_hackflags==0)
	{
		print_2_1("Autodetect Speed Hack");
	}
	else
	{
		print_2("Speed Hack: ",hacknames[(g_hackflags-2)&3]);
	}
#endif
	print_2("Double Speed: ",clocktxt[doubletimer==1]);
	print_2("LCD scanline hack: ", lcdhacktxt[g_lcdhack]);
    // Maybe we should add an option here for our HDMA experiment
//	print_2("DMA Mode: ", dmamodetxt[_dmamode]);
}


void drawclock()
{
#if RTCSUPPORT
    char *s=str+20;
    int timer,mod;

	if(rtc)
    {
	strcpy(str,"                    00:00:00");
	timer=gettime();
	mod=(timer>>4)&3;				//Hours.
	*(s++)=(mod+'0');
	mod=(timer&15);
	*(s++)=(mod+'0');
	s++;
	mod=(timer>>12)&15;				//Minutes.
	*(s++)=(mod+'0');
	mod=(timer>>8)&15;
	*(s++)=(mod+'0');
	s++;
	mod=(timer>>20)&15;				//Seconds.
	*(s++)=(mod+'0');
	mod=(timer>>16)&15;
	*(s++)=(mod+'0');

	drawtext(0,str,0);
    }
#endif
}

void autoAset()
{
	autoA++;
	joycfg|=A_BTN+(A_BTN<<16);
	if(autoA==1)
		joycfg&=~A_BTN;
	else if(autoA==2)
		joycfg&=~(A_BTN<<16);
	else
		autoA=0;
}

void autoBset()
{
	autoB++;
	joycfg|=B_BTN+(B_BTN<<16);
	if(autoB==1)
		joycfg&=~B_BTN;
	else if(autoB==2)
		joycfg&=~(B_BTN<<16);
	else
		autoB=0;
}

void sleepset()
{
	stime++;
	if(stime==1)
		sleeptime=60*60*10;			// 10min
	else if(stime==2)
		sleeptime=60*60*30;			// 30min
	else if(stime==3)
		sleeptime=0x7F000000;		// 360days...
	else if(stime>=4){
		sleeptime=60*60*5;			// 5min
		stime=0;
	}
}

void vblset()
{
	novblankwait++;
	if(novblankwait>=3)
		novblankwait=0;
}

void fpsset()
{
	fpsenabled = (fpsenabled^1)&1;
}

void brightset()
{
	gammavalue++;
	if (gammavalue>4) gammavalue=0;
	g_update_border_palette=1;
	transfer_palette();				//make new palette visible
}

#if MULTIBOOT
void multiboot()
{
	int i;
	cls(1);
	drawtext(9,"          Sending...",0);
	i=SendMBImageToClient();
	if(i) {
		if(i<3)
			drawtext(9,"         Link error.",0);
		else
			drawtext(9,"  Game is too big to send.",0);
		if(i==2) drawtext(10,"       (Check cable?)",0);
		for(i=0;i<90;i++)			//wait a while
			waitframe();
	}
}
#endif

void restart()
{
#if MOVIEPLAYER
	restart_rommenu();
	return;
#endif

#if CARTSRAM
	writeconfig();					//save any changes
#endif
	scrolll(1);
	jump_to_rommenu();
}

void exit_()
{
#if CARTSRAM
#ifndef EZFLASH_OMEGA_BUILD
	writeconfig();					//save any changes
#endif
#endif
	fadetowhite();
	REG_DISPCNT=FORCE_BLANK;		//screen OFF
	REG_BG0HOFS=0;
	REG_BG0VOFS=0;
	REG_BLDMOD=0;					//no blending
	doReset();
}

void sleep_()
{
	fadetowhite();
	suspend();
	setdarkness(7);				//restore screen
	while((~REG_P1)&0x3ff) {
		waitframe();				//(polling REG_P1 too fast seems to cause problems)
	}
}
void fadetowhite()
{
	int i;
	for(i=7;i>=0;i--) {
		setdarkness(i);			//go from dark to normal
		waitframe();
	}
	for(i=0;i<17;i++) {				//fade to white
		setbrightnessall(i);		//go from normal to white
		waitframe();
	}
}

/*
void scrolll(int f)
{
	int i;
	for(i=0;i<9;i++)
	{
		if(f) setdarkness(8+i);	//Darken screen
		ui_x=i*32;
		move_ui_wait();
	}
}
void scrollr()
{
	int i;
	for(i=8;i>=0;i--)
	{
		ui_x=i*32;
		move_ui_wait();
	}
	cls(2);							//Clear BG2
}
*/

void swapAB()
{
	joycfg^=0x400;
}

void autostateset()
{
	autostate = (autostate^1)&1;
}

void draw_palette_list()
{
	int scroll_top,scroll_bottom,row,item,num_pals;
	
	
	palettebank=selected;
	paletteinit();
	PaletteTxAll();
	transfer_palette();
	
	cls(2);
	
	num_pals=ARRSIZE(paltxt);

	scroll_top=selected-10;
	scroll_bottom=num_pals-20;
	if (scroll_top<0) scroll_top=0;
	if (scroll_top > scroll_bottom) scroll_top=scroll_bottom;
	
	for (item=scroll_top; item<scroll_top+20; item++)
	{
		row=item-scroll_top;
		drawtext(32+row,paltxt[item],selected==item);
	}
}


void chpalette()
{
	int key;//,oldsel;

	setdarkness(0);

	selected=palettebank;
	draw_palette_list();
	oldkey=~REG_P1;			//reset key input
	do {
		key=getmenuinput(ARRSIZE(paltxt));
		if(key&(UP+DOWN+LEFT+RIGHT)) {
			draw_palette_list();
		}
	} while(!(key&(A_BTN+B_BTN+R_BTN+L_BTN)));
	while(key&(B_BTN+L_BTN+R_BTN)) {
		waitframe();		//(polling REG_P1 too fast seems to cause problems)
		key=~REG_P1;
	}

	setdarkness(8);
}

/*void border() {
	bcolor++;
	if(bcolor>=4)
		bcolor=0;
	makeborder();
}
*/

//void detect(void) {
//	gbadetect^=1;
//}
void gbtype()
{
	request_gb_type=(request_gb_type+1) % 5;
}
void gbatype()
{
	request_gba_mode=!request_gba_mode;
}
void sgbpalnum()
{
	sgb_palette_number=(sgb_palette_number+1)&3;
}
void timermode()
{
	doubletimer=(doubletimer+1) % 3;
	if (doubletimer==0) doubletimer=1;
	update_doublespeed_ui();
}
void changeautoborder()
{
	auto_border=!auto_border;
}
void changelcdhack()
{
	g_lcdhack++;
	if (g_lcdhack>=4) g_lcdhack=0;
	update_lcdhack();
}
#if 0
void changedmamode()
{
	if (_dmamode < 2)
	{
		_dmamode ^= 1;
	}
}
#endif

#if GOMULTIBOOT
void go_multiboot()
{
#if ROMVERSION
	u8* rom_addr;
	u32 max_mb_size=128*1024;
	u32 romsize;
	u8 *emu_src=(u8*)goomba_mb_gba;
	u8 *emu_dest=(u8*)0x02000000;
	u32 emu_size=GOOMBA_MB_GBA_SIZE;
	int i;
	int key;
	
	rom_addr=(u8*)findrom(selectedrom);
	romsize = (0x8000 << (*(rom_addr+0x148)));
	if (romsize>max_mb_size)
	{
		cls(1);
		drawtext(8, "Game is too big to multiboot",0);
		for(i=0;i<90;i++)			//wait a while
		{
			waitframe();
		}
		return;
	}
	else
	{
		cls(1);
		drawtext(8, "This will reset the emulator!",0);
		drawtext(9, "       Are you sure?",0);
		drawtext(10,"        A=YES, B=NO",0);
		oldkey=~REG_P1;			//reset key input
		do {
			key=getmenuinput(10);
			if(key&(B_BTN + R_BTN + L_BTN ))
				return;
		} while(!(key&(A_BTN)));
		oldkey=~REG_P1;			//reset key input
	}
	REG_IME=0;
	REG_DM0CNT_H=0;
	REG_DM1CNT_H=0;
	REG_DM2CNT_H=0;
	REG_DM3CNT_H=0;
	memcpy(emu_dest,emu_src,emu_size);
	memcpy(emu_dest+emu_size,rom_addr,romsize);
	jump_r0(0x02000000);
#else
	u8 *src, *dest;
	int size;
	int key;
	int romsize;
	int i;

	src=(u8*)findrom(selectedrom);
	dest=ewram_start;
	romsize = (0x8000 << (*(src+0x148)));
	
	size=max_multiboot_size;
	if (romsize>size)
	{
		cls(1);
		drawtext(8, "Game is too big to multiboot",0);
		for(i=0;i<90;i++)			//wait a while
		{
			waitframe();
		}
		return;
	}
	else
	{
		cls(1);
		drawtext(8, "This will reset the emulator!",0);
		drawtext(9, "       Are you sure?",0);
		drawtext(10,"        A=YES, B=NO",0);
		oldkey=~REG_P1;			//reset key input
		do {
			key=getmenuinput(10);
			if(key&(B_BTN + R_BTN + L_BTN ))
				return;
		} while(!(key&(A_BTN)));
		oldkey=~REG_P1;			//reset key input
	}

	memcpy (dest,src,size);
	textstart=dest;	
	selectedrom=0;
	loadcart(selectedrom,g_emuflags&0x300);
	mainmenuitems=MENUXITEMS[1];
	roms=1;
#endif
}
#endif


#if SPEEDHACKS_OLD
u32*const speedhack_buffers[]=
{
	SPEEDHACK_FIND_JR_NZ_BUF,
	SPEEDHACK_FIND_JR_Z_BUF,
	SPEEDHACK_FIND_JR_NC_BUF,
	SPEEDHACK_FIND_JR_C_BUF
};
const int num_speedhack_buffers=4;
const int MAX_SPEEDHACK_LENGTH=16;

__inline void clear_speedhack_find_buffers(void)
{
	int i;
	for (i=0;i<num_speedhack_buffers;i++)
	{
		memset (speedhack_buffers[i],0,64);
	}
}
void autodetect_speedhack(void)
{
	int oldvblank;
	if (g_hackflags==0)
	{
		clear_speedhack_find_buffers();
		g_hackflags=1;
		cpuhack_reset();
		oldvblank=novblankwait;  //preserve vblank
		
#if CARTSRAM
		//Ensure that game has SRAM before running
		if (get_sram_owner()==0)
		{
			get_saved_sram();
		}
		writeconfig();			//save any changes
#endif
		
		run(0);
		
#if CARTSRAM
		{
			//If game changed sram, save it now.
			int savesuccess=backup_gb_sram(1);
			if (!savesuccess)
			{
				drawui1();
				REG_BG2HOFS=0;
			}
		}
#endif		
		
		
		
		novblankwait=oldvblank;
		dontstop=1;
		find_best_speedhack();
	}
	else // if (g_hackflags!=1)  //no more deleayed searches
	{
		g_hackflags=0;
		cpuhack_reset();
	}
}


void find_best_speedhack(void)
{
	unsigned int max=0,val,branchlength;
	int hacktype=-1;
	int h;
	u32 *arr;
	int i,maxindex=-1;
	for (h=0;h<num_speedhack_buffers;h++)
	{
		arr=speedhack_buffers[h];
		for (i=0;i<MAX_SPEEDHACK_LENGTH;i++)
		{
			val=arr[i];
			if (val>max)
			{
				maxindex=i;
					hacktype=h;
				max=val;
			}
		}
	}
	
	if (hacktype>=0)
	{
		branchlength=maxindex+2;
//		hacktype=hacktypes[hacktype];
//		if (hacktype==0x4C)
//		{
//			branchlength-=2;
//		}
		g_hackflags=hacktype+2;
		g_hackflags2=branchlength;
		cpuhack_reset();
	}
	else
	{
		g_hackflags=0;
		cpuhack_reset();
	}
}
#endif
