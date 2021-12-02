#include "includes.h"

static const int savestate_size_estimate = 0xC800;
void cleanup_ewram();
int FindStateByIndex(int index, int type, stateheader **stateptr);

extern int SaveState(u8 *dest);
extern int LoadState(u8 *source, int maxLength);

int loadstate2(int romNumber, stateheader *sh);
int savestate2(void);

#if !CARTSRAM
#if !MOVIEPLAYER
int get_saved_sram(void)
{
	return 0;
}
#endif
#else

#define SAVE_START_32K 0x6000
#define SAVE_START_64K 0xE000

#if SRAM_SIZE==32
	//for 32k SRAM
	#define SAVE_START SAVE_START_32K
	//#define GBA_SRAM_SIZE 0x8000
#else
	//for 64k SRAM
	#define SAVE_START SAVE_START_64K
	//#define GBA_SRAM_SIZE 0x10000
#endif

EWRAM_DATA u32 save_start = SAVE_START;



#define STATEID 0x57a731d8
#define STATEID2 0x57a731d9

#define STATESAVE 0
#define SRAMSAVE 1
#define CONFIGSAVE 2
#define MBC_SAV 2

EWRAM_BSS int totalstatesize;		//how much SRAM is used
EWRAM_BSS u32 sram_owner=0;

EWRAM_BSS u8 *sram_copy = NULL;      //located at ewram_start
EWRAM_BSS u8 *lzo_workspace = NULL;  //located at ewram_start or ewram_start + 0xE000
EWRAM_BSS u8 *uncompressed_save = NULL;  //located at ewram_start + 10000
EWRAM_BSS u8 *compressed_save = NULL;    //located around ewram_start + 1C800, moved back to + 10000 after first step
EWRAM_BSS stateheader *current_save_file = NULL;

//EWRAM_BSS u8 *buffer1;
//EWRAM_BSS u8 *buffer2;
//EWRAM_BSS u8 *buffer3;

EWRAM_BSS bool doNotLoadSram;

/*
#if LITTLESOUNDDJ
u8 *M3_SRAM_BUFFER =(u8*)0x9FE0000;
void *M3_COMPRESS_BUFFER = (u32*)0x9FD0000;
#endif
*/


/*
extern u8 Image$$RO$$Limit;
extern u8 g_cartflags;	//(from GB header)
extern int bcolor;		//Border Color
extern int palettebank;	//Palette for DMG games
extern u8 gammavalue;	//from lcd.s
//extern u8 gbadetect;	//from gb-z80.s
extern u8 stime;		//from ui.c
extern u8 autostate;	//from ui.c
extern u8 *textstart;	//from main.c

extern char pogoshell;	//main.c

//-------------------
u8 *findrom(int);
void cls(int);		//main.c
void drawtext(int,char*,int);
void setdarkness(int dark);
void scrolll(int f);
void scrollr(void);
void waitframe(void);
u32 getmenuinput(int);
void writeconfig(void);
void setup_sram_after_loadstate(void);
void no_sram_owner();
void register_sram_owner();

extern int roms;		//main.c
extern int selected;	//ui.c
extern char pogoshell_romname[32];	//main.c
//----asm stuff------
int savestate(void*);		//cart.s
void loadstate(int,void*);		//cart.s

extern u8 *romstart;	//from cart.s
extern u32 romnum;	//from cart.s
extern u32 frametotal;	//from gb-z80.s
//-------------------

typedef struct {
	u16 size;	//header+data
	u16 type;	//=STATESAVE or SRAMSAVE
	u32 uncompressed_size;
	u32 framecount;
	u32 checksum;
	char title[32];
} stateheader;

typedef struct {		//(modified stateheader)
	u16 size;
	u16 type;	//=CONFIGSAVE
	char bordercolor;
	char palettebank;
	char misc;
	char reserved3;
	u32 sram_checksum;	//checksum of rom using SRAM e000-ffff
	u32 zero;	//=0
	char reserved4[32];  //="CFG"
} configdata;
*/

/*
void bytecopy(u8 *dst,u8 *src,int count)
{
	do
	{
		*dst++ = *src++;
	} while (--count);
}
*/

/*
void debug_(u32 n,int line);
void errmsg(char *s) {
	int i;

	drawtext(32+9,s,0);
	for(i=30;i;--i)
		waitframe();
	drawtext(32+9,"                     ",0);
}*/

void flush_end_sram()
{
	u8* sram=MEM_SRAM;
	int i;
	int save_end = save_start + 0x2000;
	for (i=save_start;i<save_end;i++)
	{
		sram[i]=0;
	}
}


void probe_sram_size()
{
	vu8* sram=MEM_SRAM;
	vu8* sram2=MEM_SRAM + 0x8000;
	u32 val1;
	u32 val2;
	u32 newval2;
	
	val1 = sram[0]+(sram[1]<<8)+(sram[2]<<16)+(sram[3]<<24);
	val2 = sram2[0]+(sram2[1]<<8)+(sram2[2]<<16)+(sram2[3]<<24);
	
	if (val2 == val1)
	{
		if (val1 == STATEID || val1 == STATEID2)
		{
			sram[0] = (val1^(STATEID^STATEID2)) & 0xFF;
			newval2 = sram2[0]+(sram2[1]<<8)+(sram2[2]<<16)+(sram2[3]<<24);
			//value has changed => 32k save is mirrored
			if (newval2 != val2)
			{
				save_start = SAVE_START_32K;
			}
			else
			{
				save_start = SAVE_START_64K;
			}
			sram[0] = STATEID & 0xFF;
		}
		else
		{
			//no state ID in SRAM, xor first byte with FF and see if it changed elsewhere as well
			sram[0] ^= 0xFF;
			if (sram2[0] == sram[0])
			{
				save_start = SAVE_START_32K;
			}
			else
			{
				save_start = SAVE_START_64K;
			}
			sram[0] ^= 0xFF;
		}
	}
	else
	{
		//no match, it's 64K
		save_start = SAVE_START_64K;
	}
}

/*
void flush_xgb_sram()
{
	u8* sram=(u8*)XGB_SRAM;
	int i;
	for (i=0x0;i<0x8000;i++)
	{
		sram[i]=0;
	}
}
*/



void getsram()	//copy GBA sram to sram_copy
{
	//called by: managesram, deletemenu, savestatemenu, findstate, loadstatemenu
	if (sram_copy == NULL)
	{
		sram_copy = ewram_start;
		sram_copy[0] = 0;
		lzo_workspace = NULL;
		//uncompressed_save = NULL;
		//compressed_save = NULL;
	}
	
	u8 *sram = MEM_SRAM;
	u8 *sramCopy = sram_copy;
	u32 *sramCopy32 = (u32*)sramCopy;
	
	if(sramCopy32[0] != STATEID)	//if sram hasn't been copied already
	{
		probe_sram_size();  //stop NO$GBA from copying out-of-range data
		bytecopy(sramCopy, sram, save_start);	//copy everything to sram_copy
		if(!(sramCopy32[0] == STATEID || sramCopy32[0] == STATEID2)) //valid gba save ram data?
		{
			sramCopy32[0] = STATEID;	//nope.  initialize
			sramCopy32[1] = 0;
		}
	}
}

#if USETRIM
//quick & dirty rom checksum
u32 checksum_this()
{
	u8 *p = romstart;
	u32 sum=0;
	int i;
//	u32 addthis;
	
	u8* end = (u8*)INSTANT_PAGES[1];
	
	u8 endchar = end[-1];
	for (i = 0; i < 128; i++)
	{
		if (p < end)
		{
			sum += *p | (*(p + 1) << 8) | (*(p + 2) << 16) | (*(p + 3) << 24);
		}
		else
		{
			sum += endchar | (endchar << 8) | (endchar << 16) | (endchar << 24);
		}
		p += 128;
	}
	return sum;
}

u32 checksum_mem(u8 *p)
{
	u32 sum=0;
	int i;
	for (i = 0; i < 128; i++)
	{
		sum += *p | (*(p + 1) << 8) | (*(p + 2) << 16) | (*(p + 3) << 24);
		p += 128;
	}
	return sum;
}

u32 checksum_romnum(int romNumber)
{
	u8 *romBase = findrom2(romNumber);
	u32 *rom32 = (u32*)romBase;
	if (*rom32 == TRIM)
	{
		u8 *page0_start = romBase + rom32[2];
		u8 *page0_end = romBase + rom32[3];
		u8 *p = page0_start;

		u32 sum=0;
		int i;
		
		u8 endchar=page0_end[-1];
		for (i = 0; i < 128; i++)
		{
			if (p < page0_end)
			{
				sum += *p | (*(p + 1) << 8) | (*(p + 2) << 16) | (*(p + 3) << 24);
			}
			else
			{
				sum += endchar | (endchar << 8) | (endchar << 16) | (endchar << 24);
			}
			p+=128;
		}
		return sum;
	}
	else
	{
		return checksum_mem(romBase);
	}
}

#else
//quick & dirty rom checksum
u32 checksum(u8 *p) {
	u32 sum=0;
	int i;
	for(i=0;i<128;i++) {
		sum+=*p|(*(p+1)<<8)|(*(p+2)<<16)|(*(p+3)<<24);
		p+=128;
	}
	return sum;
}
#endif

void writeerror() {
	int i;
	cls_secondary();
	
	drawtext_secondary(9,"  Write error! Memory full.",0);
	drawtext_secondary(10,"     Delete some saves.",0);
	
	scrolll(0);
	
	for(i=90;i;--i)
	{
		waitframe();
	}
	scrollr(0);
}


/*
void memset8(u8 *p, int value, int size)
{
	while (size > 0)
	{
		*p++ = (u8)value;
		size--;
	}
}
*/

//(sram_copy=copy of GBA SRAM, current_save_file=new data)
//overwrite:  index=state#, erase=0
//new:  index=big number (anything >=total saves), erase=0
//erase:  index=state#, erase=1
//returns TRUE if successful
//IMPORTANT!!! totalstatesize is assumed to be current
//need to check this
int updatestates(int index,int erase,int type)
{
	if (sram_copy == NULL)
	{
		getsram();
	}
	
	stateheader *newdata = current_save_file;
	stateheader *foundState;
	
	int stateFound = FindStateByIndex(index, type, &foundState);
	int oldSize = 0;
	int newSize;
	if (erase || newdata == NULL)
	{
		newSize = 0;
	}
	else
	{
		newSize = newdata->size;
	}
	
	u8 *saveEnd = sram_copy + 4 + totalstatesize;
	u8 *dest;
	
	if (stateFound)
	{
		dest = (u8*)foundState;
		oldSize = foundState->size;
	}
	else
	{
		dest = saveEnd;
	}
	
	u8 *newSaveEnd = saveEnd + newSize - oldSize;
	
	//out of memory?
	if (newSaveEnd - sram_copy + 8 > save_start)
	{
		return 0;
	}
	
	u8 *src = dest + oldSize;
	u8 *copySrc = src;
	u8 *copyDest = dest + newSize;
	int copyCount = saveEnd - src;
	if (copyCount > 0 && copyDest != copySrc)
	{
		memmove(copyDest, copySrc, copyCount);
	}
	if (newSize > 0)
	{
		memcpy32(dest, (void*)newdata, newSize);
	}
	
	u32 *terminator = (u32*)newSaveEnd;
	terminator[0] = 0;
	terminator[1] = 0xFFFFFFFF;
	
	totalstatesize = newSaveEnd - sram_copy;
	bytecopy(MEM_SRAM, sram_copy, totalstatesize + 8);
	memset8(MEM_SRAM + totalstatesize + 8, 0, save_start - (totalstatesize + 8));
	return 1;
	/*
	
	
	int i;
	int srcsize;
	int srctype;
	int total=totalstatesize;
	u8 *src=sram_copy;
	u8 *dst;
	u8 *newdst;
	u8 *mem_end = sram_copy + save_start;
	stateheader *newdata=current_save_file;

	src+=4;//skip STATEID

	//skip ahead to where we want to write

	srcsize=((stateheader*)src)->size;
	srctype=((stateheader*)src)->type;
	i=(srctype==type || (type==-1 && srctype!=CONFIGSAVE)  )?0:-1;
	while(i<index && srcsize) {	//while (looking for state) && (not out of data)
		src+=srcsize;
		srcsize=((stateheader*)src)->size;
		srctype=((stateheader*)src)->type;
		if(srctype==type || (type==-1 && srctype!=CONFIGSAVE))
			i++;
	}

	dst=src;
	
	src+=srcsize;
	total-=srcsize;
	if(!erase) {
		i=newdata->size;
		total+=i;
		if(total>save_start) // **OUT OF MEMORY**
			return 0;
		newdst=(u8*)newdata + i;
		srcsize=((stateheader*)src)->size;
		while(srcsize) {		//copy trailing old data to after new data.
			memcpy(newdst,src,srcsize);
			newdst+=srcsize;
			src+=srcsize;
			srcsize=((stateheader*)src)->size;
		}
		*(u32*)newdst=0;	//terminate
		*(u32*)(newdst+4)=0xffffffff;	//terminate
		src=(u8*)newdata;
	}

	srcsize=((stateheader*)src)->size;

	//copy everything back to sram_copy
	while(srcsize) {
		memcpy(dst,src,srcsize);
		dst+=srcsize;
		src+=srcsize;
		srcsize=((stateheader*)src)->size;
	}

	*(u32*)dst=0;	//terminate
	*(u32*)(dst+4)=0xffffffff;	//terminate
	dst+=8;
	total+=8;

	//copy everything to GBA sram

	totalstatesize=total;
	while(total<save_start)
	{
		*dst++=0;
		total++;
	}
	bytecopy(MEM_SRAM,sram_copy,total);	//copy to sram
	return 1;
	*/
}

//more dumb stuff so we don't waste space by using sprintf
int twodigits(int n,char *s) {
	int mod=n%10;
	n=n/10;
	*(s++)=(n+'0');
	*s=(mod+'0');
	return n;
}

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

void number_cat(char *str, unsigned int n)
{
	number_at(str + strlen(str), n);
}


void getstatetimeandsize(char *s,int time,u32 size,u32 freespace)
{
#if 0
	strcpy(s,"00:00:00 - 00/00k");
	twodigits(time/216000,s);
	s+=3;
	twodigits((time/3600)%60,s);
	s+=3;
	twodigits((time/60)%60,s);
	s+=5;
	twodigits(size/1024,s);
	s+=3;
	twodigits(totalsize/1024,s);
#else
	/////////012345678901234567890123456789
	//        12:34:56 - 65535, free 65535
	strcpy(s,"00:00:00 - ");
	
	twodigits(time/216000,s);
	s+=3;
	twodigits((time/3600)%60,s);
	s+=3;
	twodigits((time/60)%60,s);
	s+=5;
	s=number_at(s,size);
	strcat(s,", free ");
	s=number_at(s+7,freespace);
#endif
}

#define LOADMENU 0
#define SAVEMENU 1
#define SRAMMENU 2
#define DELETEMENU 3
#define FIRSTLINE 2
#define LASTLINE 16

//sram_copy holds copy of SRAM
//draw save/loadstate menu and update global totalstatesize
//returns a pointer to current selected state
//update *states on exit
stateheader* drawstates(int menutype,int *menuitems,int *menuoffset, int needed_size)
{
	if (sram_copy == NULL)
	{
		getsram();
	}
	
	int type;
	int offset=*menuoffset;
	int sel=selected;
	int startline;
	int size;
	int statecount;
	int total;
	int freespace;
	char *s=str;
	//char s[30];
	stateheader *selectedstate;
	int time;
	int selectedstatesize;
	stateheader *sh=(stateheader*)(sram_copy + 4);

	type=(menutype==SRAMMENU)?SRAMSAVE:STATESAVE;

	statecount=*menuitems;
	if(sel-offset>LASTLINE-FIRSTLINE-3 && statecount>LASTLINE-FIRSTLINE+1) {		//scroll down
		offset=sel-(LASTLINE-FIRSTLINE-3);
		if(offset>statecount-(LASTLINE-FIRSTLINE+1))	//hit bottom
			offset=statecount-(LASTLINE-FIRSTLINE+1);
	}
	if(sel-offset<3) {				//scroll up
		offset=sel-3;
		if(offset<0)					//hit top
			offset=0;
	}
	*menuoffset=offset;
	
	startline=FIRSTLINE-offset;
	cls(2);
	statecount=0;
	total=8;	//header+null terminator
	while(sh->size) {
		size=sh->size;
		if(sh->type==type || (menutype==DELETEMENU && sh->type!=CONFIGSAVE)  ) {
			if(startline+statecount>=FIRSTLINE && startline+statecount<=LASTLINE) {
				drawtext(32+startline+statecount,sh->title,sel==statecount);
			}
			if(sel==statecount) {		//keep info for selected state
				time=sh->framecount;
				selectedstatesize=size;
				selectedstate=sh;
			}
			statecount++;
		}
		total+=size;
		sh=(stateheader*)((u8*)sh+size);
	}

	freespace=save_start-total;

	if(sel!=statecount) {//not <NEW>
		getstatetimeandsize(s,time,selectedstatesize,freespace);
		drawtext(32+18,s,0);
	}
	else
	{
		//show data for the state to be created
		sh=current_save_file;
		if (sh != NULL)
		{
			time=sh->framecount;
			selectedstatesize=sh->size;

			getstatetimeandsize(s,time,selectedstatesize,freespace);
			drawtext(32+18,s,0);
		}
	}
	
	if(statecount)
		drawtext(32+19,"Push SELECT to delete",0);
	if(menutype==SAVEMENU) {
		if(startline+statecount<=LASTLINE)
			drawtext(32+startline+statecount,"<NEW>",sel==statecount);
		drawtext(32,"Save state:",0);
		statecount++;	//include <NEW> as a menuitem
	} else if(menutype==LOADMENU) {
		drawtext(32,"Load state:",0);
	} else if(menutype==SRAMMENU) {
		drawtext(32,"Erase SRAM:",0);
	} else if(menutype==DELETEMENU) {
		int freethis=needed_size-freespace;
		if (freethis>0)
		{
			strcpy(str,"Please free up ");
			number_cat(str, freethis);
			strcat(str," bytes");
			drawtext(32,str,0);
		}
		else
		{             //012345678901234567890123456789
			drawtext(32,"We now have enough space",0);
		}
	}
	*menuitems=statecount;
	totalstatesize=total;
	return selectedstate;
}

//compress src into dest (adding header), using 64k of workspace
void compressstate(lzo_uint size,u16 type,const u8 *src,u8 *dest, void *workspace)
{
	//called by save_new_sram only
	
	lzo_uint compressedsize;
	stateheader *sh;

	if (workspace == NULL) {
		memcpy(dest+sizeof(stateheader),src,size);
		compressedsize=size;
	} else {
		lzo1x_1_compress(src,size,dest+sizeof(stateheader),&compressedsize,workspace);	//workspace needs to be 64k
	}

	//setup header:
	sh=(stateheader*)dest;
	sh->size=(compressedsize+sizeof(stateheader)+3)&~3;	//size of compressed state+header, word aligned
	sh->type=type;
	sh->uncompressed_size=size;	//size of compressed state
	sh->framecount=frametotal;
	sh->checksum=checksum_romnum(romnum);	//checksum
#if POGOSHELL
    if(pogoshell)
    {
		strcpy(sh->title,pogoshell_romname);
    }
    else
#endif
    {
		strncpy(sh->title,(char*)findrom(romnum)+0x134,15);
    }
    cleanup_ewram();
}

void managesram() {
//need to check this
	int i;
	int menuitems;
	int offset=0;

	getsram();

	selected=0;
	drawstates(SRAMMENU,&menuitems,&offset,0);
	if(!menuitems)
		return;		//nothing to do!

	scrolll(0);
	do {
		i=getmenuinput(menuitems);
		if(i&SELECT) {
			updatestates(selected,1,SRAMSAVE);
			if(selected==menuitems-1) selected--;	//deleted last entry.. move up one
		}
		if(i&(SELECT+UP+DOWN+LEFT+RIGHT))
			drawstates(SRAMMENU,&menuitems,&offset,0);
	} while(menuitems && !(i&(L_BTN+R_BTN+B_BTN)));
	drawui1();
	scrollr(0);
}

void deletemenu(int statesize)
{
	int old_ui_x = ui_x;
	ui_x = 0;
	move_ui();
	
	int i;
	int menuitems;
	int offset=0;

	getsram();

	
	selected=0;
	drawstates(DELETEMENU,&menuitems,&offset,statesize);
	if (!menuitems)
	{
		return;
	}
	scrolll(0);
	do {
		i=getmenuinput(menuitems);
		if(i&SELECT)
		{
			updatestates(selected,1,-1);
			if (selected==menuitems-1) selected--;
		}
		if(i&(SELECT+UP+DOWN+LEFT+RIGHT))
			drawstates(DELETEMENU,&menuitems,&offset,statesize);
	} while(!(i&(L_BTN+R_BTN+B_BTN)));
	getsram();
	
	scrollr(0);
	ui_x = old_ui_x;
	move_ui();
}



void savestatemenu() {
//need to check this
	int i;
	int menuitems;
	int offset=0;
	
	SAVE_FORBIDDEN;

	i = savestate2();
	if (i <= 0 || i >= 57344 - 64)
	{
		writeerror();
		return;
	}
	//compressstate(i,STATESAVE,buffer2,buffer1);

	getsram();

	selected=0;
	drawstates(SAVEMENU,&menuitems,&offset,0);
	scrolll(0);
	do {
		i=getmenuinput(menuitems);
		if(i&(A_BTN)) {
			if(!updatestates(selected,0,STATESAVE))
				writeerror();
		}
		if(i&SELECT)
			updatestates(selected,1,STATESAVE);
		if(i&(SELECT+UP+DOWN+LEFT+RIGHT))
			drawstates(SAVEMENU,&menuitems,&offset,0);
	} while(!(i&(L_BTN+R_BTN+A_BTN+B_BTN)));
	drawui1();
	scrollr(0);
}

int FindStateByIndex(int index, int type, stateheader **stateptr)
{
	getsram();
	stateheader *sh = (stateheader*)(sram_copy+4);
	int size = sh->size;
	int i = 0;
	int total = 0;
	int foundstate = 0;
	while (size != 0)
	{
		if (sh->type == type)
		{
			if (index == i)
			{
				*stateptr = sh;
				foundstate = 1;
			}
			i++;
		}
		total += size;
		sh = (stateheader*)(((u8*)sh) + size);
		size = sh->size;
	}
	totalstatesize = total;
	return foundstate;
}


//locate last save by checksum
//returns save index (-1 if not found) and updates stateptr
//updates totalstatesize (so quicksave can use updatestates)
int findstate(u32 checksum,int type,stateheader **stateptr)
{
	if (sram_copy == NULL)
	{
		getsram();
	}
	
//need to check this
	int state,size,foundstate,total;
	stateheader *sh;

	getsram();
	sh=(stateheader*)(sram_copy+4);

	state=-1;
	foundstate=-1;
	total=8;
	size=sh->size;
	while(size) {
		if(sh->type==type) {
			state++;
			if(sh->checksum==checksum) {
				foundstate=state;
				if (stateptr != NULL)
				{
					*stateptr=sh;
				}
			}
		}
		total+=size;
		sh=(stateheader*)(((u8*)sh)+size);
		size=sh->size;
	}
	totalstatesize=total;
	return foundstate;
}

/*
void uncompressstate(int rom,stateheader *sh) {
//need to check this
	lzo_uint statesize=sh->size-sizeof(stateheader);
	lzo1x_decompress((u8*)(sh+1),statesize,buffer2,&statesize,NULL);
	loadstate(rom,buffer2);
	frametotal=sh->framecount;		//restore global frame counter
	setup_sram_after_loadstate();		//handle sram packing
}
*/

int using_flashcart() {
#if MOVIEPLAYER
	if (usingcache)
	{
		return 0;
	}
#endif

	return (u32)textstart&0x8000000;
}

void quickload() {
	stateheader *sh;
	int i;
	
	SAVE_FORBIDDEN;

	if(!using_flashcart())
		return;

	i=findstate(checksum_this(),STATESAVE,&sh);
	if(i>=0)
		loadstate2(romnum,sh);
}

void quicksave() {
	stateheader *sh;
	int i;
	
	SAVE_FORBIDDEN;

	if(!using_flashcart())
		return;

	ui_y=0;
	ui_x=256;
	cls(3);
	make_ui_visible();
	move_ui();
	//setdarkness(7);	//darken
	drawtext_secondary(9,"           Saving...",0);
	scrolll(1);
	
	i=savestate2();
	if (i == 0 || i >= 57344 - 64)
	{
		writeerror();
		scrollr(2);
		//cls(2);
		//setdarkness(0);	//darken
		return;
	}


	//compressstate(i,STATESAVE,buffer2,buffer1);
	i=findstate(checksum_this(),STATESAVE,&sh);
	if(i<0) i=65536;	//make new save if one doesn't exist
	if(!updatestates(i,0,STATESAVE))
	{
		writeerror();
	}
	scrollr(2);
	cls(3);
}

int backup_gb_sram(int called_from)
{
	if (sram_copy == NULL)
	{
		getsram();
	}

	int already_tried_to_save = 0;
//need to check this
	int i;
restart:
	i=0;
	configdata *cfg;
	stateheader *sh;
	lzo_uint compressedsize;
	u32 chk=0;
	
	if (called_from==1 && romstart != NULL)
	{
		chk=checksum_this();
	}
	
	if(!using_flashcart())
		return 1;

	lzo_workspace = sram_copy + 0xE000;
	compressed_save = lzo_workspace + 0x10000;
	current_save_file = (stateheader*)compressed_save;
	
	if (called_from==1 && g_sramsize==3 ) //called from UI and 32K sram size
	{
		i=findstate(chk,SRAMSAVE,&sh);//find out where to save
		if(i>=0)
		{
			memcpy(compressed_save,sh,sizeof(stateheader));//use old info, in case the rom for this sram is gone and we can't look up its name.
			lzo1x_1_compress(XGB_SRAM,0x8000,compressed_save+sizeof(stateheader),&compressedsize,lzo_workspace);	//workspace needs to be 64k
			lzo_workspace = NULL;
			sh=current_save_file;
			sh->size=(compressedsize+sizeof(stateheader)+3)&~3;	//size of compressed state+header, word aligned
			sh->checksum = chk;
			sh->uncompressed_size=0x8000;	//size of compressed state
			int success = updatestates(i,0,SRAMSAVE);
			cleanup_ewram();
			if (!success)
			{
				writeerror();
				if (!already_tried_to_save)
				{
					already_tried_to_save = 1;
					deletemenu(sh->size);
					goto restart;
				}
				compressed_save = NULL;
				current_save_file = NULL;
				return 0;
			}
		}
		compressed_save = NULL;
		current_save_file = NULL;
		return 1;
	}
	
	/*
	#if 0
	#if LITTLESOUNDDJ
	if (called_from==1 && g_sramsize==4 )
	{
		i=findstate(chk,SRAMSAVE,&sh);//find out where to save
		if(i>=0)
		{
			u8 * old_buffer2 = buffer2;
			u8 * old_buffer3 = buffer3;
			buffer3=buffer2;
			
			memcpy(compressed_save,sh,sizeof(stateheader));//use old info, in case the rom for this sram is gone and we can't look up its name.
			lzo1x_1_compress(M3_SRAM_BUFFER,0x20000,compressed_save + sizeof(stateheader),&compressedsize,M3_COMPRESS_BUFFER);	//workspace needs to be 64k
			sh=current_save_file;
			sh->size=(compressedsize+sizeof(stateheader)+3)&~3;	//size of compressed state+header, word aligned
			sh->checksum = chk;
			sh->uncompressed_size=0x20000;	//size of compressed state
			if (!updatestates(i,0,SRAMSAVE))
			{
				buffer2 = old_buffer2;
				buffer3 = old_buffer3;
				writeerror();
				if (!already_tried_to_save)
				{
					already_tried_to_save = 1;
					deletemenu(sh->size);
					goto restart;
				}
				return 0;
			}
			buffer2 = old_buffer2;
			buffer3 = old_buffer3;
		}
		return 1;
	}
	#endif
	#endif
	*/
	
	i=findstate(0,CONFIGSAVE,(stateheader**)&cfg);	//find config
	
	if (called_from==1 && chk==sram_owner)
	{
		//copy XGB_SRAM to MEM_SRAM, because some instructions (push) don't properly modify GBA SRAM
		bytecopy(MEM_SRAM+save_start,XGB_SRAM,0x2000);
	}
	
	if(i>=0 && cfg->sram_checksum)	//SRAM is occupied?
	{
		i=findstate(cfg->sram_checksum,SRAMSAVE,&sh);//find out where to save
		if(i>=0)
		{
			int save_size=0x2000;

			memcpy(compressed_save,sh,sizeof(stateheader));//use old info, in case the rom for this sram is gone and we can't look up its name.
			lzo1x_1_compress(MEM_SRAM+save_start,save_size,compressed_save+sizeof(stateheader),&compressedsize,lzo_workspace);	//workspace needs to be 64k
			sh=(stateheader*)current_save_file;
			sh->size=(compressedsize+sizeof(stateheader)+3)&~3;	//size of compressed state+header, word aligned
			sh->checksum = cfg->sram_checksum;
			sh->uncompressed_size=save_size;	//size of compressed state
			int success = updatestates(i,0,SRAMSAVE);
			cleanup_ewram();
			if (!success)
			{
				writeerror();
				if (!already_tried_to_save)
				{
					already_tried_to_save = 1;
					deletemenu(sh->size);
					goto restart;
				}
				compressed_save = NULL;
				current_save_file = NULL;
				return 0;
			}
			else
			{
				i=findstate(0,CONFIGSAVE,(stateheader**)&cfg);	//find config
				no_sram_owner();
			}
		}
		else
		{
			compressed_save = NULL;
			current_save_file = NULL;
			//could not find SRAM file, but we still have something to save!
			int r;
			r=find_rom_number_by_checksum(cfg->sram_checksum);
			if (r>=0)
			{
				int save_success;
				u32 oldromnum=romnum;
				//u8* oldromstart=romstart;
				//romstart=findrom2(r);
				romnum=r;
				save_success=save_new_sram(MEM_SRAM+save_start);
				romnum=oldromnum;
				//romstart=oldromstart;
				if (!save_success)
				{
					writeerror();
					if (!already_tried_to_save)
					{
						already_tried_to_save = 1;
						sh=current_save_file;
						deletemenu(sh->size);
						goto restart;
					}
					compressed_save = NULL;
					current_save_file = NULL;
					return 0;
				}
				else
				{
					//all saved, now erase the sram
					no_sram_owner();
				}
			}
		}
	}
	compressed_save = NULL;
	current_save_file = NULL;
	return 1;
}

//make new saved sram (using XGB_SRAM contents)
//this is to ensure that we have all info for this rom and can save it even after this rom is removed
int save_new_sram(u8 *SRAM_SOURCE)
{
	int sramsize=0;
	if (g_sramsize==1) sramsize=0x2000;			//8KB
	else if (g_sramsize==2) sramsize=0x2000;	//8KB
	else if (g_sramsize==3) sramsize=0x8000;	//32KB
	else if (g_sramsize==4) sramsize=0x8000;	//32KB
	else if (g_sramsize==5) sramsize=512;		//512 b
	if (SRAM_SOURCE != XGB_SRAM)
	{
		breakpoint();
		//if we are saving from MEM_SRAM instead of XGB_SRAM, always use size 8KB
		sramsize = 0x2000;
	}
	if (sram_copy == NULL)
	{
		getsram();
	}
	lzo_workspace = sram_copy + 0xE000;
	compressed_save = lzo_workspace + 0x10000;
	current_save_file = (stateheader*) compressed_save;
	
	compressstate(sramsize,SRAMSAVE,SRAM_SOURCE,compressed_save,lzo_workspace);
	int result = updatestates(65536,0,SRAMSAVE);
	
	lzo_workspace = NULL;
	compressed_save = NULL;
	current_save_file = NULL;
	return result;
}

int get_saved_sram(void)
{
	//returns:
	// 0 - game doesn't use SRAM
	// 1 - successfully loaded
	// 2 - file not found
	
	int i,j;
	int retval;
	u32 chk;
	configdata *cfg;
	stateheader *sh;
	lzo_uint statesize;

	if(!using_flashcart())
	{
		return 0;
	}
	if (doNotLoadSram)
	{
		return 0;
	}

	if(g_cartflags&MBC_SAV)
	{	//if rom uses SRAM
		chk=checksum_this();
		i=findstate(0,CONFIGSAVE,(stateheader**)&cfg);	//find config
		j=findstate(chk,SRAMSAVE,&sh);	//see if packed SRAM exists
		
		//probably shouldn't do this
/*
		if(i>=0) if(chk==cfg->sram_checksum) {	//SRAM is already ours
			bytecopy(XGB_SRAM,MEM_SRAM+save_start,0x2000);
			if(j<0) save_new_sram();	//save it if we need to
			return;
		}
		*/
//		flush_end_sram();
		
		if(j>=0) {//packed SRAM exists: unpack into XGB_SRAM
			statesize=sh->size-sizeof(stateheader);
			/*
			#if LITTLESOUNDDJ
			if (g_sramsize!=4)
			{
				lzo1x_decompress((u8*)(sh+1),statesize,XGB_SRAM,&statesize,NULL);
			}
			else
			{
				lzo1x_decompress((u8*)(sh+1),statesize,buffer2,&statesize,NULL);
				memcpy(M3_SRAM_BUFFER,buffer2,0x20000);
			}
			#else
			*/
			lzo1x_decompress((u8*)(sh+1),statesize,XGB_SRAM,&statesize,NULL);
			//#endif
			retval=1;
		} else { //pack new sram and save it.
			save_new_sram(XGB_SRAM);
			retval=2;
		}
		
		//For 32k SRAM, don't bother storing anything in real SRAM, in fact, flush it out.
		if (g_sramsize==3 || g_sramsize==4)
		{
			no_sram_owner();
		}
		else
		{
			//otherwise, use the sram saving system
			bytecopy(MEM_SRAM+save_start,XGB_SRAM,0x2000);
			register_sram_owner();//register new sram owner
		}
		return retval;
	}
	else
	{
		return 0;
	}
}

void register_sram_owner()
{
	sram_owner=checksum_this();
	writeconfig();
}

void no_sram_owner()
{
	sram_owner=0;
	writeconfig();
	flush_end_sram();
}

void setup_sram_after_loadstate() {
//need to check this
	int i;
	u32 chk;
	configdata *cfg;

	if(g_cartflags&MBC_SAV) {	//if rom uses SRAM
		chk=checksum_this();
		i=findstate(0,CONFIGSAVE,(stateheader**)&cfg);	//find config
		if(i>=0) if(chk!=cfg->sram_checksum) {//if someone else was using sram, save it
			backup_gb_sram(0);
		}
		
		if (g_sramsize < 3)
		{
			//For 8KB size or less:
			bytecopy(MEM_SRAM+save_start,XGB_SRAM,0x2000);		//copy gb sram to real sram
		}
		else
		{
			no_sram_owner();
		}
		i=findstate(chk,SRAMSAVE,(stateheader**)&cfg);	//does packed SRAM for this rom exist?
		if(i<0)						//if not, create it
			save_new_sram(XGB_SRAM);
		if (g_sramsize < 3)
		{
			//For 8KB size or less:
			register_sram_owner();//register new sram owner
		}
		else
		{
			//otherwise rewrite the SRAM to the save file immediately
			backup_gb_sram(1);
		}
	}
}

//returns a rom number, or -1
int find_rom_number_by_checksum(u32 sum)
{
	int i;
	for (i=0;i<roms;i++)
	{
		if(sum==checksum_romnum(i))
			return i;
	}
	return -1;
}


void loadstatemenu() {
	stateheader *sh;
	u32 key;
	int i;
	int offset=0;
	int menuitems;
	u32 sum;
	
	SAVE_FORBIDDEN;

	getsram();

	selected=0;
	sh=drawstates(LOADMENU,&menuitems,&offset,0);
	if(!menuitems)
		return;		//nothing to load!

	scrolll(0);
	do {
		key=getmenuinput(menuitems);
		if(key&(A_BTN)) {
			sum=sh->checksum;
			i=0;
			do {
				if(sum==checksum_romnum(i)) {	//find rom with matching checksum
					loadstate2(i,sh);
					i=8192;
				}
				i++;
			} while(i<roms);
			if(i<8192) {
				cls(2);
				drawtext(32+9,"       ROM not found.",0);
				for(i=0;i<60;i++)	//(1 second wait)
					waitframe();
			}
		} else if(key&SELECT) {
			updatestates(selected,1,STATESAVE);
			if(selected==menuitems-1) selected--;	//deleted last entry? move up one
		}
		if(key&(SELECT+UP+DOWN+LEFT+RIGHT))
			sh=drawstates(LOADMENU,&menuitems,&offset,0);
	} while(menuitems && !(key&(L_BTN+R_BTN+A_BTN+B_BTN)));
	drawui1();
	scrollr(0);
}

const configdata configtemplate={
	sizeof(configdata),
	CONFIGSAVE,
	0,0,0,0,0,0,
	"CFG"
};

void writeconfig()
{
	if (sram_copy == NULL)
	{
		getsram();
	}
	configdata *cfg;
	int i,j;

	if(!using_flashcart())
		return;
	
	compressed_save = sram_copy + 0xE000;
	current_save_file = (stateheader*)compressed_save;
	
	i=findstate(0,CONFIGSAVE,(stateheader**)&cfg);
	if(i<0) {//make new config
		memcpy(compressed_save,&configtemplate,sizeof(configdata));
		cfg=current_save_file;
	}
//	cfg->bordercolor=bcolor;					//store current border type
	cfg->palettebank=palettebank;				//store current DMG palette
	j = stime & 0x3;							//store current autosleep time
//	j |= (gbadetect & 0x1)<<3;					//store current gbadetect setting
	j |= (autostate & 0x1)<<4;					//store current autostate setting
	j |= (gammavalue & 0x7)<<5;					//store current gamma setting
	cfg->misc = j;
	cfg->sram_checksum=sram_owner;
	if(i<0) {	//create new config
		updatestates(0,0,CONFIGSAVE);
	} else {		//config already exists, update sram directly (faster)
		bytecopy((u8*)cfg-sram_copy+MEM_SRAM,(u8*)cfg,sizeof(configdata));
	}
	
	compressed_save = NULL;
	current_save_file = NULL;
}

void readconfig() {
	int i;
	configdata *cfg;
	if(!using_flashcart())
		return;

	i=findstate(0,CONFIGSAVE,(stateheader**)&cfg);
	if(i>=0) {
//		bcolor=cfg->bordercolor;
		palettebank=cfg->palettebank;
		i = cfg->misc;
		stime = i & 0x3;						//restore current autosleep time
//		gbadetect = (i & 0x08)>>3;				//restore current gbadetect setting
		autostate = (i & 0x10)>>4;				//restore current autostate setting
		gammavalue = (i & 0xE0)>>5;				//restore current gamma setting
		sram_owner=cfg->sram_checksum;
	}
}
/*
void clean_gb_sram() {
	int i;
	u8 *gb_sram_ptr = MEM_SRAM+save_start;
	configdata *cfg;

	if(!using_flashcart())
		return;

	for(i=0;i<0x2000;i++) *gb_sram_ptr++ = 0;

	i=findstate(0,CONFIGSAVE,(stateheader**)&cfg);
	if(i<0) {//make new config
		memcpy(buffer3,&configtemplate,sizeof(configdata));
		cfg=(configdata*)buffer3;
	}
	cfg->bordercolor=bcolor;					//store current border type
	cfg->palettebank=palettebank;				//store current DMG palette
	cfg->misc = stime & 0x3;					//store current autosleep time
	cfg->misc |= (autostate & 0x1)<<4;			//store current autostate setting
	cfg->sram_checksum=0;						// we don't want to save the empty sram
	if(i<0) {	//create new config
		updatestates(0,0,CONFIGSAVE);
	} else {		//config already exists, update sram directly (faster)
		bytecopy((u8*)cfg-buffer1+MEM_SRAM,(u8*)cfg,sizeof(configdata));
	}
}

*/

int savestate2()
{
	//if successful, updates compressed_save and current_save_file
	sram_copy = NULL;
	lzo_workspace = ewram_start;
	uncompressed_save = ewram_start + 0x10000;
	
	//Save State - part 1, the system state excluding SRAM
	
	u8 *workspace = lzo_workspace;
	u8 *uncompressedState = uncompressed_save; //+ savestate_size_estimate * 17 / 16;
	
	int stateSize = SaveState(uncompressedState);
	if (stateSize == 0) //does not happen for now
	{
		goto fail;
	}
	
	compressed_save = uncompressed_save + stateSize;
	current_save_file = (stateheader*)compressed_save;
	
	u8 *out = compressed_save + sizeof(stateheader) + 8;
	lzo_uint compressedSize1;
	lzo1x_1_compress(uncompressedState, stateSize, out, &compressedSize1, workspace);
	u32 part1_size = ((compressedSize1 - 1) | 3) + 1;
	
	*((u32*)(out - 4)) = part1_size;
	*((u32*)(out - 8)) = stateSize;
	
	int outSize = out + part1_size + 8 - compressed_save;
	
	//move save file to second buffer to try to not hit the RAM limit
	memcpy32(uncompressed_save, compressed_save, outSize);
	//update pointers
	out = out - (compressed_save - uncompressed_save);
	compressed_save = uncompressed_save;
	current_save_file = (stateheader*)compressed_save;
	uncompressed_save = NULL;
	
	//Part 2 - SRAM
	u8 *out2 = out + part1_size + 8;
	
	int sramSize = 0;
	if (g_sramsize == 0)
	{
		sramSize = 0;
	}
	else if (g_sramsize == 3)
	{
		sramSize = 0x8000;
	}
	else
	{
		sramSize = 0x2000;
	}
	
	int total_size = part1_size + 8 + sizeof(stateheader);
	
	if (sramSize > 0)
	{
		int sramMaxSize = sramSize + sramSize / 16 + 67;
		int remainingSpace = 0x10000 - part1_size;
		lzo_uint compressedSize2;
		int part2_size;

		lzo1x_1_compress(XGB_SRAM, sramSize, out2, &compressedSize2, workspace);
		part2_size = ((compressedSize2 - 1) | 3) + 1;

		/*
		if (sramMaxSize <= remainingSpace)
		{
			lzo1x_1_compress(XGB_SRAM, sramSize, out2, &compressedSize2, workspace);
			part2_size = ((compressedSize2 - 1) | 3) + 1;
		}
		else
		{
			lzo1x_1_compress(XGB_SRAM, sramSize, buffer2, &compressedSize2, workspace);
			part2_size = ((compressedSize2 - 1) | 3) + 1;
			//if compressed state is bigger than 64K, reject it
			if (total_size + part2_size + 8 >= 0x10000)
			{
				goto fail;
			}
			memcpy32(out2, buffer2, part2_size);
		}
		*/
		*((u32*)(out2 - 4)) = part2_size;
		*((u32*)(out2 - 8)) = sramSize;
		total_size += part2_size + 8;
	}
	
	//setup header:
	stateheader* sh = current_save_file;
	sh->size=total_size;	//size of compressed state+header, word aligned
	sh->type=STATESAVE;
	sh->uncompressed_size=stateSize + sramSize;	//size of compressed state
	sh->framecount=frametotal;
	sh->checksum=checksum_this();	//checksum
#if POGOSHELL
    if(pogoshell)
    {
		strcpy(sh->title,pogoshell_romname);
    }
    else
#endif
    {
		strncpy(sh->title,(char*)findrom(romnum)+0x134,15);
    }
	uncompressed_save = NULL;
	lzo_workspace = NULL;
	cleanup_ewram();
	return total_size;
fail:  //does not happen for now
	uncompressed_save = NULL;
	compressed_save = NULL;
	current_save_file = NULL;
	lzo_workspace = NULL;
	cleanup_ewram();
	return 0;
}

void cleanup_ewram()
{
	//check EWRAM canaries and rebuild tables
	if (ewram_canary_1 != 0xDEADBEEF)
	{
		//NOTE: must update this if equates.h changes
		breakpoint();

		extern u8 vram_packets_dirty[];
		extern u8 vram_packets_registered_bank0[];
		extern u8 vram_packets_registered_bank1[];
		extern u8 vram_packets_incoming[];
		extern u8 RECENT_TILENUM[];
		extern u8 dirty_map_words[];
		extern u8 DIRTY_TILE_BITS[];
		
		//rebuild instant_pages
		make_instant_pages(romstart);
		//zero out vram_packets and stuff (for shantae)
		memset32(vram_packets_dirty, 0, 0xC4);
		memset32(vram_packets_registered_bank0, 0, 0xC0);
		memset32(vram_packets_registered_bank1, 0, 0xC0);
		memset32(vram_packets_incoming, 0, 0xC0);
		memset32(RECENT_TILENUM, 0, 0x80);
		memset32(dirty_map_words, -1, 0x40);
		memset32(DIRTY_TILE_BITS, -1, 0x30);
		ewram_canary_1 = 0xDEADBEEF;
	}
	if (ewram_canary_2 != 0xDEADBEEF)
	{
		breakpoint();
		
		extern u8 TEXTMEM[];
		
		memset32(TEXTMEM, 0x20202020, 0x278);
		//TODO: make up stuff for buffers and stuff
		
		ewram_canary_2 = 0xDEADBEEF;
	}
}

int loadstate2(int romNumber, stateheader *sh)
{
	//sh will be of inside sram_copy
	
	if (sram_copy == NULL)
	{
		getsram();
	}
	
	if (romNumber != romnum)
	{
		//Don't load SRAM while loading state
		doNotLoadSram = true;
		//Do not switch to SGB mode (for initial border) while loading state
		int old_auto_border = auto_border;
		auto_border = 0;
		loadcart(romNumber, g_emuflags);
		//restore auto_border setting
		auto_border = old_auto_border;
		doNotLoadSram = false;
	}
	
	u8 *src = (u8*)(sh + 1);
	u32 *src32 = (u32*)src;
	u32 uncompressedStateSize = *src32++;
	u32 compressedStateSize = *src32++;
	src = (u8*)src32;
	
	u8 *src2 = src + compressedStateSize;
	u32 uncompressedSramSize = 0;
	u32 compressedSramSize = 0;
	
	if (g_sramsize == 0 && sh->size != compressedStateSize + 8 + sizeof(stateheader))
	{
		return 0;
	}
	
	if (g_sramsize != 0)
	{
		//u8 *src2 = src + compressedStateSize;
		src32 = (u32*)src2;
		uncompressedSramSize = *src32++;
		compressedSramSize = *src32++;
		src2 = (u8*)src32;
		
		if (sh->size != compressedStateSize + compressedSramSize + 16 + sizeof(stateheader))
		{
			return 0;
		}
	}
	
	if (uncompressedStateSize > 0x10000 || 0 != (uncompressedStateSize & 3) ||
		compressedStateSize > 0x10000 || 0 != (compressedStateSize & 3) ||
		uncompressedSramSize > 0x8000 || 0 != (uncompressedSramSize & 3) ||
		compressedSramSize > 0x8900 || 0 != (compressedSramSize & 3) )
	{
		return 0;
	}
	
	uncompressed_save = sram_copy + 0xE000;
	
	lzo_uint bytesDecompressed = compressedStateSize;
	lzo1x_decompress(src, compressedStateSize, uncompressed_save, &bytesDecompressed, NULL);
	
	if (uncompressedSramSize > 0)
	{
		lzo_uint bytesDecompressed2 = compressedStateSize;
		lzo1x_decompress(src2, compressedSramSize, XGB_SRAM, &bytesDecompressed2, NULL);
	}
	
	int result = LoadState(uncompressed_save, uncompressedStateSize);
	
	uncompressed_save = NULL;
	
	
	if (result != 0)
	{
		return 0;
	}
	
	frametotal=sh->framecount;		//restore global frame counter
	setup_sram_after_loadstate();		//handle sram packing
	
	return sh->size;
}




#endif


