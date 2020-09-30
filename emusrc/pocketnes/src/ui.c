#include "includes.h"

EWRAM_BSS u8 autoA,autoB;				//0=off, 1=on, 2=R
EWRAM_BSS u8 stime=0;
EWRAM_BSS u8 autostate=0;

//int selected;//selected menuitem.  used by all menus.
EWRAM_BSS int mainmenuitems;//? or CARTMENUITEMS, depending on whether saving is allowed

//u32 oldkey;

EWRAM_BSS char str[32]; //ZOMG global variable!
/*
int print_2_func(int row, const char *src1, const char *src2);
int print_1_func(int row, const char *src1, const char *src2);
int strmerge_str(int unused, const char *src1, const char *src2);
int text2_str(int row);
int text1_str(int row);
*/

void dummy(void);
//void dummy()
//{
//	
//}

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

/*
#define print_1(xxxx,yyyy) row=print_1_func(row,(xxxx),(yyyy))
#define print_2(xxxx,yyyy) row=print_2_func(row,(xxxx),(yyyy))
#define print_1_1(xxxx) row=text(row,(xxxx));
#define print_2_1(xxxx) row=text2(row,(xxxx));
*/

const fptr multifnlist[]=
{
	#if CHEATFINDER
	ui5,
	#endif
	autoBset,autoAset,controller,ui3,ui2,
	#if OLDSPEEDHACKS
	ui4,
	#endif
	#if MULTIBOOT
	multiboot,
	#endif
	sleep_,restart,exit_
};

const fptr fnlist1[]=
{
	#if CHEATFINDER
	ui5,
	#endif
	autoBset,autoAset,controller,ui3,ui2,
	#if OLDSPEEDHACKS
	ui4,
	#endif
	#if MULTIBOOT
	multiboot,
	#endif
	#if SAVE
	savestatemenu,loadstatemenu,managesram,
	#endif
	sleep_,
	#if GOMULTIBOOT
	go_multiboot,
	#endif
	restart,exit_
};

const fptr fnlist2[]=
{
	vblset,fpsset,swapAB,sleepset,
	#if SAVE
	autostateset,autostateset2,
	#endif
	nextregion
};

const fptr fnlist3[]=
{
	display,flickset,brightset
	#if EDITFOLLOW
	,followramtoggle,selectfollowaddress
	#endif
};

#if !OLDSPEEDHACKS
const fptr fnlistdummy[]=
{
	dummy
};
#endif

#if OLDSPEEDHACKS
const fptr fnlist4[]={
	ppuhacktoggle,cpuhacktoggle,autohacktoggle,autodetect_speedhack
	#if EDITBRANCHHACKS
	,changehackmode,changebranchlength
	#endif
};
#endif

#if CHEATFINDER
const fptr fnlist5[]=
{
	cf_editcheats,cf_results,cf_equal,cf_notequal,cf_greater,cf_less,
	cf_greaterequal,cf_lessequal,cf_comparewith,cf_update,cf_newsearch
};
#endif

const fptr *const fnlistX[]=
{
	fnlist1,multifnlist,fnlist2,fnlist3
	#if OLDSPEEDHACKS
	,fnlist4
	#else
	,fnlistdummy
	#endif
	#if CHEATFINDER
	,fnlist5
	#endif
};
const fptr drawuiX[]=
{
	drawui1,drawui1,drawui2,drawui3
	#if OLDSPEEDHACKS
	,drawui4
	#else
	,dummy
	#endif
	#if CHEATFINDER
	,drawui5
	#else
	,dummy
	#endif
};

const char MENUXITEMS[]=
{
	ARRSIZE(fnlist1),ARRSIZE(multifnlist),ARRSIZE(fnlist2),ARRSIZE(fnlist3)
	#if OLDSPEEDHACKS
	,ARRSIZE(fnlist4)
	#else
	,0
	#endif
	#if CHEATFINDER
	,ARRSIZE(fnlist5)
	#else
	,0
	#endif
};

const char *const autotxt[]={"OFF","ON","with R"};
const char *const vsynctxt[]={"ON","OFF","SLOWMO"};
const char *const sleeptxt[]={"5min","10min","30min","OFF"};
const char *const brightxt[]={"I","II","III","IIII","IIIII"};
const char *const hostname[]={"Crap","Prot","GBA","GBP","NDS"};
const char *const ctrltxt[]={"1P","2P","1P+2P","Link2P","Link3P","Link4P"};
const char *const disptxt[]={"Unscaled","Unscaled (Follow)","Scaled BG, Full OBJ","Scaled BG and OBJ"};
const char *const flicktxt[]={"No Flicker","Flicker"};
const char *const cntrtxt[]={"US (NTSC)","Europe (PAL)", "Dendy (Russia)", "Overclocked NTSC"};
#if EDITFOLLOW
const char *const followtxt[]={"Sprite","RAM"};
const char *const followtxt2[]={"Sprite Number: ","Address: "};
#endif
#if BRANCHHACKDETAIL
const char *const branchtxt[]={"None","BPL","BMI","BVC","BVS","BCC","BCS","BNE","BEQ","JMP"};
#endif
#define EMUNAME "PocketNES"

void drawui1()
{
	int row=0;
	cls(1);
#if !PREVIEWBUILD
	strmerge(str,EMUNAME " " VERSION_NUMBER " on ",hostname[(int)gbaversion]);
	drawtext(19,str,0);
#else
	drawtext(17,"PocketNES PREVIEW BUILD 2",0);
	drawtext(18,"FOR TESTING ONLY",0);
	drawtext(19,"DO NOT DISTRIBUTE",0);
#endif
	
	#if CHEATFINDER
	print_1_1("Cheat Finder->");
	#endif
	print_1("B autofire: ",autotxt[autoB]);
	print_1("A autofire: ",autotxt[autoA]);
	print_1("Controller: ",ctrltxt[(joycfg>>29)-1]);
	print_1_1("Display->");
	print_1_1("Other Settings->");
	#if OLDSPEEDHACKS
	print_1_1("Speed Hacks->");
	#endif
	#if MULTIBOOT
	print_1_1("Link Transfer");
	#endif
	if(mainmenuitems==ARRSIZE(multifnlist)) {
		print_1_1("Sleep");
		print_1_1("Restart");
		print_1_1("Exit");
	} else {
		#if SAVE
		print_1_1("Save State->");
		print_1_1("Load State->");
		print_1_1("Manage SRAM->");
		#endif
		print_1_1("Sleep");
		#if GOMULTIBOOT
		print_1_1("Go Multiboot");
		#endif
		print_1_1("Restart");
		print_1_1("Exit");
	}
}

int GetRegion()
{
	int region = (emuflags & EMUFLAGS_PALTIMING) != 0;  // (0 = NTSC, 1 = PAL, 2 = DENDY, 3 = OVERCLOCKED)
	if (emuflags & EMUFLAGS_DENDY)
	{
		region = 2;
		if ((emuflags & EMUFLAGS_FPS50) == 0)
		{
			region = 3;
		}
	}
	return region;
}

void SetRegion(int newRegion)
{
	emuflags &= ~0x1C;
	switch (newRegion)
	{
		case 0:
			break;
		case 1:
			emuflags |= REGION_PAL;
			break;
		case 2:
			emuflags |= REGION_DENDY;
			break;
		case 3:
			emuflags |= REGION_OVERCLOCKED;
			break;
	}
	ntsc_pal_reset();
}

void drawui2()
{
	int row=0;
	int region;
	cls(2);
	drawtext(32,"       Other Settings",0);
	print_2("VSync: ",vsynctxt[novblankwait]);
	print_2("FPS-Meter: ",autotxt[(int)fpsenabled]);
	print_2("Swap A-B: ",autotxt[(joycfg>>10)&1]);
	print_2("Autosleep: ",sleeptxt[stime]);
	#if SAVE
	print_2("Autoload state: ",autotxt[autostate&1]);
	print_2("Autosave state: ",autotxt[(autostate&2)>>1]);
	#endif
	region = GetRegion();
	print_2("Region: ",cntrtxt[region]);
}

void drawui3()
{
	int row=0;
	cls(2);
	drawtext(32,"      Display Settings",0);
	print_2("Display: ",disptxt[scaling&3]);
	print_2("Scaling: ",flicktxt[(int)flicker]);
	print_2("Gamma: ",brightxt[(int)gammavalue]);
	#if EDITFOLLOW
	print_2("Sprite Follow by: ",followtxt[(emuflags & 32)>>5]);		//MEMFOLLOW=32
	print_2(followtxt2[(emuflags & 32)>>5],hex4(*((short*)((&scaling)+1))));
	#endif
	
}
// drawui4 moved to speedhack.c
// drawui5 moved to cheat.c

//u32 oldkey;//init this before using getmenuinput

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
	if (menuitems==0) return keyhit;
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
	int key,soundvol,oldsel,tm0cnt,i;
	int mb=(u32)textstart<0x8000000;

	autoA=joycfg&A_BTN?0:1;
	autoA|=joycfg&(A_BTN<<16)?0:2;
	autoB=joycfg&B_BTN?0:1;
	autoB|=joycfg&(B_BTN<<16)?0:2;

	mainmenuitems=MENUXITEMS[mb];
//	mainmenuitems=((u32)textstart>0x8000000?CARTMENUITEMS:MULTIBOOTMENUITEMS);//running from rom or multiboot?
	FPSValue=0;					//Stop FPS meter

	soundvol=REG_SOUNDCNT_L;
	REG_SOUNDCNT_L=0;				//stop sound (GB)
	tm0cnt=REG_TM0CNT_H;
	REG_TM0CNT_H=0;				//stop sound (directsound)

	selected=0;
//	drawuiX[mb]();
	drawui1();
	for(i=0;i<8;i++)
	{
		waitframe();
		setdarknessgs(i);		//Darken game screen
		ui_x=224-i*32;	//Move screen right
		move_ui();
	}

	#if SAVE
	{
		int savesuccess;
		savesuccess=backup_nes_sram(1);
		if (!savesuccess)
		{
			drawui1();
			ui_x=0;
			move_ui();
		}
	}
	#endif

	oldkey=~REG_P1;			//reset key input
	do {
		#if RTCSUPPORT
		drawclock();
		#endif
		key=getmenuinput(MENUXITEMS[mb]);
		if(key&(A_BTN)) {
			oldsel=selected;
			fnlistX[mb][selected]();
			selected=oldsel;
			if (mb != ((u32)textstart<0x8000000))
			{
				mb=1;
				selected=0;
			}
		}
		if(key&(A_BTN+UP+DOWN+LEFT+RIGHT))
//			drawuiX[mb]();
			drawui1();
	} while(!(key&(B_BTN+R_BTN+L_BTN)));
	
#if SAVE
	if (get_sram_owner()==0)
	{
		get_saved_sram();
	}
#ifndef EZFLASH_OMEGA_BUILD
	writeconfig();			//save any changes
#endif
#endif
	for(i=1;i<9;i++)
	{
		waitframe();
		setdarknessgs(8-i);	//Lighten screen
		ui_x=i*32;	//Move screen left
		move_ui();
	}
	while(key&(B_BTN)) {
		waitframe();		//(polling REG_P1 too fast seems to cause problems)
		key=~REG_P1;
	}
	REG_SOUNDCNT_L=soundvol;	//resume sound (GB)
	REG_TM0CNT_H=tm0cnt;		//resume sound (directsound)
	cls(3);
}

void subui(int menunr) {
	int key,oldsel;

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
		if(key&(A_BTN+UP+DOWN+LEFT+RIGHT))
		{
			drawuiX[menunr]();
		}
	} while(!(key&(B_BTN+R_BTN+L_BTN)));
	scrollr();
	while(key&(B_BTN)) {
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
	subui(3);
}
void ui4()
{
	subui(4);
}

//ui5 moved to cheat.c

int text(int row,const char *str) {
	drawtext(row+10-mainmenuitems/2,str,selected==row);
	return row+1;
}
int text2(int row,const char *str) {
	drawtext(35+row+2,str,selected==row);
	return row+1;
}


//trying to avoid using sprintf...  (takes up almost 3k!)
void strmerge(char *dst,const char *src1,const char *src2) {
	if(dst!=src1)
		strcpy(dst,src1);
	strcat(dst,src2);
}
void strmerge3(char *dst,const char *src1,const char *src2, const char *src3) {
	if(dst!=src1)
		strcpy(dst,src1);
	strcat(dst,src2);
	strcat(dst,src3);
}
void strmerge4(char *dst,const char *src1,const char *src2, const char *src3, const char *src4) {
	if(dst!=src1)
		strcpy(dst,src1);
	strcat(dst,src2);
	strcat(dst,src3);
	strcat(dst,src4);
}

#if CHEATFINDER | EDITFOLLOW
char *hexn(unsigned int n, int digits)
{
	int i;

	static char hexbuffer[9];
	const char hextable[]="0123456789ABCDEF";
	hexbuffer[8]=0;
	for (i=7;i>=8-digits;--i)
	{
		hexbuffer[i]=hextable[n&15];
		n>>=4;
	}
	return hexbuffer+8-digits;
}
#endif

#if RTCSUPPORT
void drawclock()
{
    char str[30];
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
}
#endif

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

void controller()					//see io.s: refreshNESjoypads
{
	u32 i=joycfg+0x20000000;
	if(i>=0xe0000000)
		i-=0xc0000000;
	#if LINK
	resetSIO(i);					//reset link state
	#endif
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
	paletteinit();
	//vblank routine automatically updates the palette now
	
	//PaletteTxAll();
	//Update_Palette();
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
#if SAVE
	writeconfig();					//save any changes
#endif
	scrolll(1);
#if GCC
	extern u8 __sp_usr[];
	u32 newstack=(u32)(&__sp_usr);
	__asm__ volatile ("mov sp,%0": :"r"(newstack));
#else
	__asm {mov r0,#0x3007f00}		//stack reset
	__asm {mov sp,r0}
#endif
	rommenu();
}
void exit_()
{
#if SAVE
#ifndef EZFLASH_OMEGA_BUILD
	writeconfig();					//save any changes
#endif
	if (autostate&2)
	{
		quicksave();
	}
#endif
	fadetowhite();
	REG_DISPCNT=FORCE_BLANK;		//screen OFF
	ui_x=0;
	ui_y=0;
	move_ui();
	REG_BLDCNT=0;					//no blending
	doReset();
}

void sleep_()
{
	fadetowhite();
	suspend();
	setdarknessgs(7);				//restore screen
	while((~REG_P1)&0x3ff) {
		waitframe();				//(polling REG_P1 too fast seems to cause problems)
	}
}
void fadetowhite()
{
	int i;
	for(i=7;i>=0;i--) {
		setdarknessgs(i);			//go from dark to normal
		waitframe();
	}
	for(i=0;i<17;i++) {				//fade to white
		setbrightnessall(i);		//go from normal to white
		waitframe();
	}
}

void scrolll(int f) {
	int i;
	for(i=0;i<9;i++)
	{
		if(f) setdarknessgs(8+i);	//Darken screen
		ui_x=i*32;			//Move screen left
		move_ui();
		waitframe();
	}
}
void scrollr() 
{
	int i;
	for(i=8;i>=0;i--)
	{
		waitframe();
		ui_x=i*32;			//Move screen right
		move_ui();
	}
	cls(2);							//Clear BG2
}

void swapAB()
{
	joycfg^=0x400;
}

void display()
{
	char sc;
	wtop=0;
	scaling=sc=(scaling+1)&3;
	spriteinit();
}

void flickset()
{
	flicker++;
	if(flicker > 1){
		flicker=0;
		twitch=1;
	}
}

void autostateset()
{
	autostate^=1;
}
void autostateset2()
{
	autostate^=2;
}

void nextregion()
{
	int region = GetRegion();
	region = (region + 1) & 3;
	SetRegion(region);
}

void cpuhacktoggle()
{
	emuflags^=2;
	cpuhack_reset();
}

void ppuhacktoggle()
{
	emuflags^=1;
	cpuhack_reset();
}

//void autohacktoggle()
//{
//	hackflags3=!hackflags3;
//}


void followramtoggle(void)
{
	emuflags^=32;
}

#if CHEATFINDER | EDITFOLLOW
void draw_input_text(int row, int column, char* str, int hilitedigit)
{
	int i=0;
	int map_add = 0x1000*FONT_PALETTE_NUMBER;
	int map_add_hilite = map_add + 0x1000;
	
//	const int hilite=(1<<12)+0x4100,nohilite=0x4100;
	
	u16 *here;
	row+=37;
	row&=0x1F;
	here=SCREENBASE+row*32+column+1;
	while(str[i]>=' ')
	{
		here[i]=lookup_character(str[i])+map_add;
		if (i==hilitedigit)
		{
			if (str[i]==' ')
			   here[i]=lookup_character('_')+map_add_hilite;
			else
			   here[i]=lookup_character(str[i])+map_add_hilite;
		}
		i++;
	}
}

u32 inputhex(int row, int column, u32 value, u32 digits)
{
	int key,tmp;
	u32 digit,addthis;
	digit=digits-1;
	
	draw_input_text(row,column,hexn(value,digits),digit);
	
	oldkey=~REG_P1;			//reset key input
	do {
		waitframe();		//(polling REG_P1 too fast seems to cause problems)
		tmp=~REG_P1;
		key=(oldkey^tmp)&tmp;
		oldkey=tmp;
		if (key&(RIGHT)) ++digit;
		if (key&(LEFT)) --digit;
		digit%=digits;
		addthis=1<<((digits-digit-1)<<2);
		if (key&UP) value+=addthis;
		if (key&DOWN) value-=addthis;

		if(key&(UP+DOWN+LEFT+RIGHT))
		{
			draw_input_text(row,column,hexn(value,digits),digit);
		}
	} while(!(key&(A_BTN+B_BTN+R_BTN+L_BTN)));
	while(key&(B_BTN|A_BTN)) {
		waitframe();		//(polling REG_P1 too fast seems to cause problems)
		key=~REG_P1;
	}
	return value;
}
#endif

#if CHEATFINDER
void inputtext(int row, int column, char *text, u32 length)
{
	int key,tmp;//,fast;
	u32 pos=0;
	const u8 textrange=127-' ';
	
	draw_input_text(row,column,text,pos);
	
	oldkey=~REG_P1;			//reset key input
	do {
		waitframe();		//(polling REG_P1 too fast seems to cause problems)
		tmp=~REG_P1;
		key=(oldkey^tmp)&tmp;
		oldkey=tmp;
	
		if (key&(RIGHT)) ++pos;
		if (key&(LEFT))	pos+=(length-1);
		pos%=length;
		if (key&UP) text[pos]++;
		if (key&L_BTN) text[pos]+=10;
		if (key&DOWN) text[pos]+=(textrange-1);
		if (key&R_BTN) text[pos]+=(textrange-10);
		text[pos]=((text[pos]-' ')%textrange)+' ';

		if(key&(UP+DOWN+LEFT+RIGHT+R_BTN+L_BTN))
		{
			draw_input_text(row,column,text,pos);
		}
	} while(!(key&(A_BTN+B_BTN)));
	while(key&(B_BTN|A_BTN)) {
		waitframe();		//(polling REG_P1 too fast seems to cause problems)
		key=~REG_P1;
	}
}
#endif

#if EDITFOLLOW
void selectfollowaddress()
{
	unsigned short followaddress;
	drawui3();
	followaddress=(*((short*)((&scaling)+1)));
	followaddress=inputhex(4,strlen(followtxt2[(emuflags & 32)>>5]),followaddress,4);
	(*((short*)((&scaling)+1)))=followaddress;
}
#endif

char *number_at(char *dest, unsigned int n)
{
	unsigned int n2=n;
	int digits=0;
	char *retval;
	do
	{
		n2/=10;
		digits++;
	} while (n2);
	dest+=digits;
	retval=dest;
	*(dest--)='\0';
	do
	{
		*(dest--)=(n%10)+'0';
		n/=10;
	} while (n);
	return retval;
}

#if BRANCHHACKDETAIL | CHEATFINDER


char *number(unsigned short n)
{
	static char numbuffer[12];
	number_at(numbuffer,n);
	return numbuffer;
/*	number_at(
	int i;

	static char numbuffer[12];
	numbuffer[5]=0;
	i=5;
	if (n==0)
	{
		--i;
		numbuffer[i]='0';
	}
	while(n>0)
	{
		--i;
		numbuffer[i]=(n%10)+'0';
		n/=10;
	}
	return &numbuffer[i];
	*/
}
#endif







//extern char NES_VRAM;
//extern char rombase;
//extern char vrombase;
//extern int selectedrom;
//u8 *findrom(int);
//void loadcart(int, int);
//extern int roms;
//extern int max_multiboot_size;
//extern int romnum;

#if GOMULTIBOOT
//THIS CODE HASN'T BEEN COMPILED IN FOR A WHILE AND NEEDS FIXING!
void go_multiboot()
{
	u8 *src, *dest;
	int size;
	int key;
	int rom_size;
	
	if(pogoshell) rom_size=48+16+(*(u8*)(findrom(romnum)+48+4))*16*1024+(*(u8*)(findrom(romnum)+48+5))*8*1024;  //need to read this from ROM
	else rom_size=sizeof(romheader)+*(u32*)(findrom(romnum)+32);
	src=findrom(selectedrom);
	dest=ewram_start;
	size=(NES_VRAM)-dest+0x2000;
	if (rom_size>size)
	{
		cls(1);
		drawtext(8, "Game is too big to multiboot",0);
		drawtext(9,"      Attempt anyway?",0);
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
	loadcart(selectedrom,emuflags&0x300,0);
	mainmenuitems=MENUXITEMS[1];
	roms=1;
}
#endif
