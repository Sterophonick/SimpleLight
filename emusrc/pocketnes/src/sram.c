/*
sram.c entry points:

backup_nes_sram
cheatload
cheatsave
get_saved_sram
quickload
quicksave
readconfig
writeconfig
*/

extern char rom_is_compressed;
extern char ewram_owner_is_sram;

#include "includes.h"

#if SAVE

/*
#include "gba.h"
#include <string.h>
#include "sram.h"
#include "main.h"
#include "asmcalls.h"
#include "ui.h"
#include "minilzo.107/minilzo.h"
#include "cheat.h"
*/

#define SAVE_START_32K 0x6000
#define SAVE_START_64K 0xE000

#if SAVE32
	#define SAVE_START SAVE_START_32K
#else
	#define SAVE_START SAVE_START_64K
#endif

EWRAM_DATA u32 save_start = SAVE_START;


#define STATEID 0x57a731d7
#define STATEID2 0x57a731d8

#define STATESAVE 0
#define SRAMSAVE 1
#define CONFIGSAVE 2

#if CHEATFINDER
	#define CHEATLIST 3
#endif

EWRAM_BSS int totalstatesize;		//how much SRAM is used
EWRAM_BSS u32 sram_owner=0;
//u32 save_start=0xE000;
EWRAM_DATA int config_position=-1;


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
	char displaytype;
	char misc;
	char reserved2;
	char reserved3;
	u32 sram_checksum;	//checksum of rom using SRAM e000-ffff
	u32 zero;	//=0
	char reserved4[32];  //="CFG"
} configdata;

extern configdata CachedConfig;


void flush_end_sram(void);
void getsram(void);
void writeerror(void);
int updatestates(int index,int erase,int type);
int twodigits(int n,char *s);
void getstatetimeandsize(char *s,int time,u32 size,u32 totalsize);
stateheader* drawstates(int menutype,int *menuitems,int *menuoffset, int needed_size);
void compressstate(lzo_uint size,u16 type,u8 *src,void *workspace);
int findstate(u32 checksum,int type,stateheader **stateptr);
void uncompressstate(int rom,stateheader *sh);
int using_flashcart(void);
int save_new_sram(u8* SRAM_SOURCE);
void register_sram_owner(void);
void no_sram_owner(void);
void setup_sram_after_loadstate(void);
void findconfig(void);


//we have a big chunk of memory starting at Image$$RO$$Limit free to use
EWRAM_BSS u8* BUFFER1;
EWRAM_BSS u8* BUFFER2;
EWRAM_BSS u8* BUFFER3;

#define buffer1 BUFFER1
#define buffer2 BUFFER2
#define buffer3 BUFFER3

#define ASSERT(xxxx)
/* \
	if (!(xxxx)) \
	{ \
		display_assert_error(#xxxx); \
	}
void display_assert_error(char *s)
{
	int i;
	drawtext(32+7,"ASSERT ERROR",0);

	drawtext(32+9,s,0);
	for(i=30;i;--i)
		waitframe();
	cls(2);
}*/
bool sram_matches()
{
	u8 *p1, *p2;
	int i;
	p1=MEM_SRAM+save_start;
	p2=NES_SRAM;
	for (i=0;i<8192;i++)
	{
		if (p1[i] != p2[i])
		{
			return false;
		}
	}
	return true;
}


void bytecopy(u8 *dst,const u8 *src,int count) {
	int i=0;
	do {
		dst[i]=src[i];
		i++;
	} while(--count);
}

/*
int bytecmp(const void *dst,const void *src,int count) {
	int i=0;
	int v=0;
	const u8 *d = (const u8*)dst;
	const u8 *s = (const u8*)src;
	
	do {
		v=d[i]-s[i];
		if (v!=0) return v;
		i++;
	} while(--count);
	return 0;
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
		
	if ((val1 == STATEID || val1 == STATEID2) && val2 == val1)
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
}


void getsram() {		//copy GBA sram to BUFFER1
	u8 *sram=MEM_SRAM;
	u8 *buff1=buffer1;
	u32 *p;
	
//	if (ewram_owner_is_sram==1)
//	{
//		if (bytecmp(buff1,sram,save_start)!=0)
//		{
//			drawtext(1,"SANITY CHECK FAIL",1);
//			breakpoint();
//			while (1);
//		}
//	}
//	else
	{
		probe_sram_size();  //stop NO$GBA from copying out-of-range data
		bytecopy(buff1,sram,save_start);	//copy everything to buffer1
		p=(u32*)buff1;
		if(!(*p == STATEID || *p == STATEID2)) {	//valid savestate data?
			*p=STATEID;	//nope.  initialize
			*(p+1)=0;
			*(p+2)=0xFFFFFFFF;
			
			bytecopy(sram,buff1,12);
		}
		ewram_owner_is_sram=1;
	}
}

void writeerror() {
	int i;
	cls(2);
	ui_x=256;
	move_ui();
	drawtext(32+9, "  Write error! Memory full.",0);
	drawtext(32+10,"     Delete some saves.",0);
	for(i=90;i;--i)
		waitframe();
	cls(2);
}

//(BUFFER1=copy of GBA SRAM, BUFFER3=new data)
//overwrite:  index=state#, erase=0
//new:  index=big number (anything >=total saves), erase=0
//erase:  index=state#, erase=1
//returns TRUE if successful
//IMPORTANT!!! totalstatesize is assumed to be current
int updatestates(int index,int erase,int type) {
	int i;
	int srcsize;
	int srctype;
	int total=totalstatesize;
	u8 *dst=BUFFER1;
	u8 *src=BUFFER2;
	stateheader *newdata=(stateheader*)BUFFER3;
	
	getsram();

	memcpy(src,dst,total);			//copy buffer1 to buffer2
						//(buffer1=new, buffer2=old)
	src+=4;//skip STATEID
	dst+=4;

	//skip ahead to where we want to write
	srcsize=((stateheader*)src)->size;
	srctype=((stateheader*)src)->type;
	
	i=(srctype==type || (type==-1 && srctype!=CONFIGSAVE)  )?0:-1;
	while(i<index && srcsize) {	//while (looking for state) && (not out of data)
		dst+=srcsize;
		src+=srcsize;
		srcsize=((stateheader*)src)->size;
		srctype=((stateheader*)src)->type;
		if(srctype==type || (type==-1 && srctype!=CONFIGSAVE))
			i++;
	}

	//write new data

	total-=srcsize;
	if(!erase) {
		i=newdata->size;
		total+=i;
		if(total>save_start) //**OUT OF MEMORY**
			return 0;
		memcpy(dst,newdata,i);	//overwrite
		dst+=i;
	}
	src+=srcsize;
	srcsize=((stateheader*)src)->size;

	//get trailing data

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
	bytecopy(MEM_SRAM,BUFFER1,total);	//copy to sram
	
	findconfig(); //config may move after altering the filesystem
	
	return 1;
}


//more dumb stuff so we don't waste space by using sprintf
int twodigits(int n,char *s) {
	int mod=n%10;
	n=n/10;
	*(s++)=(n+'0');
	*s=(mod+'0');
	return n;
}

void getstatetimeandsize(char *s,int time,u32 size,u32 freespace) {
	
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
}

#define LOADMENU 0
#define SAVEMENU 1
#define SRAMMENU 2
#define DELETEMENU 3
#define FIRSTLINE 2
#define LASTLINE 16

//BUFFER1 holds copy of SRAM
//draw save/loadstate menu and update global totalstatesize
//returns a pointer to current selected state
//update *states on exit
stateheader* drawstates(int menutype,int *menuitems,int *menuoffset, int needed_size) {
	int type;
	int offset=*menuoffset;
	int sel=selected;
	int startline;
	int size;
	int statecount;
	int total;
	int freespace;
	char *s=str;
//	char s[30];
	stateheader *selectedstate;
	int time;
	int selectedstatesize;
	stateheader *sh=(stateheader*)(BUFFER1+4);

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
		sh=(stateheader*)((u8*)BUFFER3);
		time=sh->framecount;
		selectedstatesize=sh->size;
		
		getstatetimeandsize(s,time,selectedstatesize,freespace);
		drawtext(32+18,s,0);
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
			strmerge3(s,"Please free up ",number(freethis)," bytes");
			drawtext(32,s,0);
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

//compress src into BUFFER3 (adding header), using 64k of workspace
void compressstate(lzo_uint size,u16 type,u8 *src,void *workspace) {
	u8* rom_start;
	lzo_uint compressedsize;
	stateheader *sh;

	//workspace needs to be 64k
	lzo1x_1_compress(src,size,BUFFER3+sizeof(stateheader),&compressedsize,workspace);

	//setup header:
	sh=(stateheader*)BUFFER3;
	sh->size=(compressedsize+sizeof(stateheader)+3)&~3;	//size of compressed state+header, word aligned
	sh->type=type;
	sh->uncompressed_size=size;	//size of compressed state
	sh->framecount=frametotal;
	rom_start=findrom(romnum);
	sh->checksum=checksum((u8*)rom_start+sizeof(romheader)+16);	//checksum
    if(pogoshell)
    {
		strcpy(sh->title,pogoshell_romname);
    }
    else
    {
		strcpy(sh->title,(char*)findrom(romnum));
    }
}

void managesram() {
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
	scrollr();
}

void deletemenu(int statesize)
{
	int i;
	int menuitems;
	int offset=0;

	getsram();

	selected=0;
	drawstates(DELETEMENU,&menuitems,&offset,statesize);
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
}



void savestatemenu() {
	int i;
	int menuitems;
	int offset=0;

	//	ewram_owner_is_sram=1;  //getsram sets it
	
	i=savestate(BUFFER2);
	compressstate(i,STATESAVE,BUFFER2,BUFFER1);

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
	scrollr();
}

//locate last save by checksum
//returns save index (-1 if not found) and updates stateptr
//updates totalstatesize (so quicksave can use updatestates)
int findstate(u32 checksum,int type,stateheader **stateptr) {
	int state,size,foundstate,total;
	stateheader *sh;

	getsram();
	sh=(stateheader*)(BUFFER1+4);

	state=-1;
	foundstate=-1;
	total=8;
	size=sh->size;
	while(size) {
		if(sh->type==type) {
			state++;
			if(sh->checksum==checksum) {
				foundstate=state;
				if (stateptr!=NULL)
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

void uncompressstate(int rom,stateheader *sh)
{
	lzo_uint statesize=sh->size-sizeof(stateheader);
	lzo1x_decompress((u8*)(sh+1),statesize,BUFFER2,&statesize,NULL);
	loadstate(rom,BUFFER2,statesize);
#if CHEATFINDER
	cheatload();
	//loadstate destroys cheats
#endif
	frametotal=sh->framecount;		//restore global frame counter
	setup_sram_after_loadstate();	//handle sram packing
}

#if CHEATFINDER
void cheatload()
{
	stateheader *sh;
	int i;
	u16 *dst, *src;
	lzo_uint cheatsize;

	if(!using_flashcart())
		return;
	
	if (cheatfinder_cheats==NULL)
	{
		num_cheats=0;
		return;
	}
	
//	ewram_owner_is_sram=1;  //findstate sets it

	i=findstate(checksum((u8*)romstart),CHEATLIST,&sh);
	if(i>=0) {
		/* The cheat list is in videoram, so it's necessary
		 * to write a byte at a time. */
		cheatsize=sh->size-sizeof(stateheader);
		lzo1x_decompress((u8*)(sh+1),cheatsize,BUFFER2,&cheatsize,NULL);
		src=(u16*)BUFFER2;
		dst=(u16*)(cheatfinder_cheats);
		
		// Write up to an extra byte.
		for (i = 0; i < cheatsize/2+(cheatsize&1); i++)
		{
			*dst = *src;
			src++;
			dst++;
		}
		num_cheats=cheatsize/18;
	}
	else
	{
		num_cheats=0;
		//TEST TEST
		//cf_newsearch();
	}
}

void cheatsave() {
	stateheader *sh;
	int i;

	if(!using_flashcart())
		return;

	setdarknessgs(7);	//darken
	drawtext(32+9,"       Saving cheat.",0);

	i=findstate(checksum((u8*)romstart),CHEATLIST,&sh);
	if(i<0) i=65536;	//make new save if one doesn't exist
	if (num_cheats) {
		compressstate(num_cheats*18,CHEATLIST,cheatfinder_cheats,BUFFER2);
		if(!updatestates(i,0,CHEATLIST))
			writeerror();
	} else if (i < 65536) {
		if(!updatestates(i,1,CHEATLIST))
			writeerror();
	}
	cls(2);
}
#endif

bool can_quickload()
{
	int i;
	i=findstate(checksum((u8*)romstart),STATESAVE,NULL);
	return i>=0;
}

bool quickload() {
	stateheader *sh;
	int i;

	if(!using_flashcart())
		return false;

//	ewram_owner_is_sram=1;  //findstate sets it

	i=findstate(checksum((u8*)romstart),STATESAVE,&sh);
	if(i>=0)
	{
		uncompressstate(romnum,sh);
		return true;
	}
	return false;
}

bool quicksave() {
	stateheader *sh;
	int i;

	if(!using_flashcart())
		return false;

//	ewram_owner_is_sram=1;	//findstate appears later, sets it to 1

	setdarknessgs(7);	//darken
	drawtext(32+9,"           Saving.",0);

	i=savestate(BUFFER2);
	compressstate(i,STATESAVE,BUFFER2,BUFFER1);
	i=findstate(checksum((u8*)romstart),STATESAVE,&sh);
	if(i<0) i=65536;	//make new save if one doesn't exist
	if(!updatestates(i,0,STATESAVE))
	{
		writeerror();
		return false;
	}
	cls(2);
	return true;
}

int backup_nes_sram(int prompt_delete_menu)
{
	int i=0;
//	configdata *cfg=&CachedConfig;
	stateheader *sh;
	lzo_uint compressedsize;
	u32 sum;
	
//	u32 chk = checksum(findrom(i)+sizeof(romheader)+16);

	if(!using_flashcart())
		return 1;

//	i=findstate(0,CONFIGSAVE,(stateheader**)&cfg);	//find config
	sum=get_sram_owner();
//	sum=cfg->sram_checksum;
	if(sum)	//SRAM is occupied?
	{
		//	ewram_owner_is_sram=1;  //findstate sets it
		i=findstate(sum,SRAMSAVE,&sh);//find out where to save
		if(i>=0)
		{
			memcpy(BUFFER3,sh,sizeof(stateheader));//use old info, in case the rom for this sram is gone and we can't look up its name.
			lzo1x_1_compress(MEM_SRAM+save_start,0x2000,BUFFER3+sizeof(stateheader),&compressedsize,BUFFER2);	//workspace needs to be 64k
			sh=(stateheader*)BUFFER3;
			sh->size=(compressedsize+sizeof(stateheader)+3)&~3;	//size of compressed state+header, word aligned
			sh->uncompressed_size=0x2000;	//size of compressed state
			if (!updatestates(i,0,SRAMSAVE))
			{
				writeerror();
				if (prompt_delete_menu)
				{
					deletemenu(sh->size);
					return backup_nes_sram(0);
				}
				return 0;
			}
			else
			{
//				i=findstate(0,CONFIGSAVE,(stateheader**)&cfg);	//find config
				no_sram_owner();
			}
		}
		else
		{
			//could not find SRAM file, but we still have something to save!
			int r;
			r=find_rom_number_by_checksum(sum);
			if (r>=0)
			{
				int save_success;
				u32 oldromnum=romnum;
				u8* oldromstart=romstart;
				romstart=findrom(r)+sizeof(romheader)+16;
				romnum=r;
				save_success=save_new_sram(MEM_SRAM+save_start);
				romnum=oldromnum;
				romstart=oldromstart;
				if (!save_success)
				{
					writeerror();
					if (prompt_delete_menu)
					{
						sh=(stateheader*)BUFFER3;
						deletemenu(sh->size);
						return backup_nes_sram(0);
					}
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
	return 1;

}

//make new saved sram (using NES_SRAM contents)
//this is to ensure that we have all info for this rom and can save it even after this rom is removed
int save_new_sram(u8* SRAM_SOURCE) {
	u8 *sram_source_copy = BUFFER1+save_start;
	bytecopy(sram_source_copy,SRAM_SOURCE,0x2000);
	compressstate(0x2000,SRAMSAVE,sram_source_copy,BUFFER2);
	return updatestates(65536,0,SRAMSAVE);
}

void get_saved_sram(void)
{
	int j;
	u32 chk;
//	configdata *cfg=&CachedConfig;
	stateheader *sh;
	lzo_uint statesize;

	if(!using_flashcart())
		return;

	if(cartflags&2) {	//if rom uses SRAM
		chk=checksum(romstart);
//		i=findstate(0,CONFIGSAVE,(stateheader**)&cfg);	//find config
		//	ewram_owner_is_sram=1;  //findstate sets it
		j=findstate(chk,SRAMSAVE,&sh);	//see if packed SRAM exists

		//probably shouldn't do this
/*
		if(i>=0) if(chk==cfg->sram_checksum) {	//SRAM is already ours
			bytecopy(NES_SRAM,MEM_SRAM+0xe000,0x2000);
			if(j<0) save_new_sram();	//save it if we need to
			return;
		}
		*/
		if(j>=0) {//packed SRAM exists: unpack into NES_SRAM
			statesize=sh->size-sizeof(stateheader);
			lzo1x_decompress((u8*)(sh+1),statesize,NES_SRAM,&statesize,NULL);
		} else { //pack new sram and save it.
			save_new_sram(NES_SRAM);
		}
		bytecopy(MEM_SRAM+save_start,NES_SRAM,0x2000);
		register_sram_owner();//register new sram owner

	}
}

void register_sram_owner()
{
	sram_owner=checksum(romstart);
	writeconfig();
}

void no_sram_owner()
{
	sram_owner=0;
	writeconfig();
	flush_end_sram();
}

u32 get_sram_owner()
{
	configdata *cfg = &CachedConfig;
	return cfg->sram_checksum;
}


void setup_sram_after_loadstate() {	
	//previous SRAM owner?  Save the old SRAM.
	backup_nes_sram(1);
	
	//does game use SRAM?
	if(cartflags&2)
	{
		//the SRAM we want is in NES_SRAM, the sram stored at E000 doesn't matter
		no_sram_owner();
		
		//copy NES_SRAM to real sram
		bytecopy(MEM_SRAM+save_start,NES_SRAM,0x2000);
		
		//set owner
		register_sram_owner();
		
		//back it up immediately?
		backup_nes_sram(1);

		//set owner again
		get_saved_sram();

	}
/*	int i;
	u32 chk;
	configdata *cfg;

	if(cartflags&2) {	//if rom uses SRAM
		chk=checksum(romstart);
		i=findstate(0,CONFIGSAVE,(stateheader**)&cfg);	//find config
		if(i>=0) if(chk!=cfg->sram_checksum) {//if someone else was using sram, save it
			backup_nes_sram(0);
		}
		bytecopy(MEM_SRAM+save_start,NES_SRAM,0x2000);		//copy nes sram to real sram
		i=findstate(chk,SRAMSAVE,(stateheader**)&cfg);	//does packed SRAM for this rom exist?
		if(i<0)						//if not, create it
			save_new_sram();
		register_sram_owner();//register new sram owner
	}
*/
}

//returns a rom number, or -1
int find_rom_number_by_checksum(u32 sum)
{
	int i;
	for (i=0;i<roms;i++)
	{
		if(sum==checksum(findrom(i)+sizeof(romheader)+16))
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

	//	ewram_owner_is_sram=1;  //getsram sets it
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
			if (sum == my_checksum)
			{
				i = romnumber;
			}
			else
			{
				i=find_rom_number_by_checksum(sum);
			}
			if (i>=0)
			{
				uncompressstate(i,sh);
			}
			else
			{
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
	scrollr();
}

const configdata configtemplate={
	sizeof(configdata),
	CONFIGSAVE,
	0,0,0,0,0,0,
	"CFG"
};

void writeconfig()
{
	configdata *cfg = &CachedConfig;
	configdata *cfg2;
	int i,j;
	int configdirty=0;

	if(!using_flashcart())
		return;
	
//	if (config_position>=4 && config_position<save_start)
//	{
//		configdata *cfg3 = (void*)((u8*)MEM_SRAM + config_position);
//		breakpoint();
//		if (bytecmp(cfg,cfg3,48)!=0)
//		{
//			drawtext(0,"BAKA",0);
//			drawtext(1,"SANITY CHECK FAIL",0);
//			drawtext(32,"SANITY CHECK FAIL",0);
//			breakpoint();
//			while (1);
//		}
//	}
	

	j =(scaling & 0xF);						//store current display type
	j |= (gammavalue & 0x7)<<5;					//store current gamma value
	if (cfg->displaytype !=j)
	{
		cfg->displaytype = j;
		configdirty=1;
	}
	j = stime & 0x3;							//store current autosleep time
	j |= (autostate & 0x3)<<5;					//store current autostate setting
	j |= ((flicker & 0x1)^1)<<4;				//store current flicker setting
	if (cfg->misc !=j)
	{
		cfg->misc = j;
		configdirty=1;
	}
	if (cfg->sram_checksum!=sram_owner)
	{
		cfg->sram_checksum=sram_owner;
		configdirty=1;
	}
	if (configdirty)
	{
		if (config_position>=4 && config_position<save_start)
		{
			//config already exists, update sram directly (faster)
//			breakpoint();
			bytecopy((u8*)MEM_SRAM+config_position,(u8*)cfg,sizeof(configdata));
			if (ewram_owner_is_sram==1)
			{
				bytecopy((u8*)BUFFER1+config_position,(u8*)cfg,sizeof(configdata));
			}
		}
		else
		{
			//	ewram_owner_is_sram=1;  //findstate sets it
			i=findstate(0,CONFIGSAVE,(stateheader**)&cfg2);
			if (i<0)
			{
				//make new config
				cfg2=(configdata*)BUFFER3;
			}
			*cfg2=*cfg;  //copy contents of configuration
			if (i<0)
			{
				//create new config
				updatestates(0,0,CONFIGSAVE);
				i=findstate(0,CONFIGSAVE,(stateheader**)&cfg2);
				config_position=((u8*)cfg2)-((u8*)BUFFER1);
			}
		}
	}
}

void findconfig()
{
	int i;
	configdata *cfg2;
	i=findstate(0,CONFIGSAVE,(stateheader**)&cfg2);
	if (i>=0)
	{
		config_position=((u8*)cfg2)-((u8*)BUFFER1);
	}
	else
	{
		config_position=-1;
	}
}


void readconfig() {
	int i;
//	configdata *cfg;
	configdata *cfg = &CachedConfig;
	configdata *cfg2;

	if(!using_flashcart())
		return;

	//	ewram_owner_is_sram=1;  //findstate sets it

	i=findstate(0,CONFIGSAVE,(stateheader**)&cfg2);
	if(i>=0) {
		config_position=((u8*)cfg2)-((u8*)BUFFER1);
		
		i = cfg2->displaytype;					//restore display type
		scaling = (i & 0xF);
		gammavalue = (i & 0xE0)>>5;				//restore gamma value
		i = cfg2->misc;
		stime = i & 0x3;						//restore autosleep time
		autostate = (i & 0x60)>>5;				//restore autostate setting
		flicker = ((i & 0x10)^0x10)>>4;			//restore flicker setting
		*cfg=*cfg2;
	}
	else
	{
		//create blank configuration
		memcpy(cfg,&configtemplate,sizeof(configdata));
		writeconfig();
	}
}

#if 0
void clean_nes_sram() {
	int i,j;
	u8 *nes_sram_ptr = MEM_SRAM+save_start;
	configdata *cfg;

	if(!using_flashcart())
		return;

	for(i=0;i<0x2000;i++) *nes_sram_ptr++ = 0;

	i=findstate(0,CONFIGSAVE,(stateheader**)&cfg);
	if(i<0) {//make new config
		memcpy(BUFFER3,&configtemplate,sizeof(configdata));
		cfg=(configdata*)BUFFER3;
	}
	j = (scaling & 0xF);						//store current display type
	j |= (gammavalue & 0x7)<<5;					//store current gamma value
	cfg->displaytype = j;
	j = stime & 0xf;							//store current autosleep time
	j |= ((flicker & 0x1)^1)<<4;				//store current flicker setting
	cfg->misc = j;
	cfg->sram_checksum=0;						// we don't want to save the empty sram
	if(i<0) {	//create new config
		updatestates(0,0,CONFIGSAVE);
	} else {		//config already exists, update sram directly (faster)
		bytecopy((u8*)cfg-BUFFER1+MEM_SRAM,(u8*)cfg,sizeof(configdata));
	}
}
#endif

#endif

//quick & dirty rom checksum
u32 checksum(u8 *p) {
	u32 sum=0;
	int i;
	
	//if rom is compressed, use the checksum from the header
	if ( ((u32)(p)>=0x02000004) && (0==memcmp(((char*)p-4),"AP33",4)))
	{
//		if (pogoshell)
//		{
//			//if it's pogoshell, we never computed this, so just return the currently loaded ROM checksum
//			return my_checksum;
//		}
		
		sum= ((*(p))<<0) | ((*(p+1))<<8) | ((*(p+2))<<16) | ((*(p+3))<<24);
		return sum;
	}
	
	for(i=0;i<128;i++) {
		sum+=*p|(*(p+1)<<8)|(*(p+2)<<16)|(*(p+3)<<24);
		p+=128;
	}
	return sum;
}

int using_flashcart() {
	return (u32)textstart&0x8000000;
}

