#include "includes.h"

#if MULTIBOOT


//based on Jeff Frohwein's slave boot demo:
//http://www.devrs.com/gba/files/mbclient.txt

#include <stdio.h>
#include "gba.h"

u8 *findrom(int);


#if GCC

extern u8 __load_stop_iwram9[]; //using this instead of __end__, because it's also cart compatible
extern u8 __iwram_lma[];

#define EMUSIZE1 ( ( (u32)(&__iwram_lma) ) & 0x3FFFF )
#define EMUSIZE2 ( ( ( (u32)(&__load_stop_iwram9) ) - EMUSIZE1 ) & 0x7FFF )

#else

extern u8 Image$$RO$$Limit[];
extern u8 Image$$ZI$$Base[];

#define EMUSIZE1 (((u32)(&Image$$RO$$Limit)&0x3ffff))
#define EMUSIZE2 (((u32)(&Image$$ZI$$Base)&0x7fff))

#endif
extern u32 romnum;	//from cart.s
extern u8 *textstart;	//from main.c


//extern char pogoshell;

typedef struct {
  u32 reserve1[5];      //
  u8 hs_data;           // 20 ($14) Needed by BIOS
  u8 reserve2;          // 21 ($15)
  u16 reserve3;         // 22 ($16)
  u8 pc;                // 24 ($18) Needed by BIOS
  u8 cd[3];             // 25 ($19)
  u8 palette;           // 28 ($1c) Needed by BIOS - Palette flash while load
  u8 reserve4;          // 29 ($1d) rb
  u8 cb;                // 30 ($1e) Needed by BIOS
  u8 reserve5;          // 31 ($1f)
  u8 *startp;           // 32 ($20) Needed by BIOS
  u8 *endp;             // 36 ($24) Needed by BIOS
  u8 *reserve6;         // 40 ($28)
  u8 *reserve7[3];      // 44 ($2c)
  u32 reserve8[4];      // 56 ($38)
  u8 reserve9;          // 72 ($48)
  u8 reserve10;         // 73 ($49)
  u8 reserve11;         // 74 ($4a)
  u8 reserve12;         // 75 ($4b)
} MBStruct;

const
#include "client.h"

void delay() {
	int i=32768;
	while(--i);	//(we're running from EWRAM)
}

#if GCC
void DelayCycles (u32 cycles) __attribute__((noinline));
void DelayCycles (u32 cycles)
{
    __asm__ volatile (
		"mov r11,r11\n\t"
		"adr r1,delaycycles_enter_arm\n\t"
		"bx r1\n\t"
		".code 32\n"
		"delaycycles_enter_arm:\n\t"
		"mov r0, %0\n\t"
		"mov r2, pc\n\t"
		"mov r1, #12\n\t"
		"cmp r2, #0x02000000\n\t"
		"beq MultiBootWaitCyclesLoop\n\t"
    
    // ROM 4/2 wait
    	"mov r1, #14\n\t"
    	"cmp r2, #0x08000000\n\t"
    	"beq MultiBootWaitCyclesLoop\n\t"
    
    // IWRAM
    	"mov r1, #4\n"
    
    	"MultiBootWaitCyclesLoop:\n\t"
    	"subs r0, r0, r1\n\t"
    	"bgt MultiBootWaitCyclesLoop\n\t"
    	"adr r1,delaycycles_enter_thumb+1\n\t"
    	"bx r1\n\t"
    	".code 16\n"
    	"delaycycles_enter_thumb:"
    	: :"r"(cycles) : "r0","r1","r2"
    	);
    return;
}
#else
void DelayCycles (u32 cycles)
{
    __asm{mov r2, pc}
    
    // EWRAM
    __asm{mov r1, #12}
    __asm{cmp r2, #0x02000000}
    __asm{beq MultiBootWaitCyclesLoop}
    
    // ROM 4/2 wait
    __asm{mov r1, #14}
    __asm{cmp r2, #0x08000000}
    __asm{beq MultiBootWaitCyclesLoop}
    
    // IWRAM
    __asm{mov r1, #4}
    
    __asm{MultiBootWaitCyclesLoop:}
    __asm{sub r0, r0, r1}
    __asm{bgt MultiBootWaitCyclesLoop}
}
#endif

u16 xfer(u16 send) {
    u32 i;
	
    i=1000;
	
	REG_SIOMLT_SEND = send;
	REG_SIOCNT = 0x2083;
	while((REG_SIOCNT & 0x80) && --i) {DelayCycles(10);}
	return (REG_SIOMULTI1);
}

#if GCC
int swi25(void *p) {
    int i;
    __asm__ volatile (
		"mov r11,r11\n\t"
		"mov r0,%1\n\t"
		"mov r1,#1\n\t"
		"swi 0x25\n\t"
		"mov %0,r0"
	: "=r" (i) : "r"(p) : "r0","r1","r2"
	);
	return i;
}
#else
int swi25(void *p) {
	__asm{mov r1,#1}
	__asm{swi 0x25, {r0-r1}, {}, {r0-r2} }
}
#endif
//returns error code:  1=no link, 2=bad send, 3=too big
#define TIMEOUT 40
int SendMBImageToClient(void) {
	MBStruct mp;
	u8 palette;
	u32 i,j;
	u16 key;
	u16 *p;
	u16 ie;
	u32 emusize1,emusize2,romsize;
	u32 totalsize;
	u16 *src_ewram;
	u16 *src_iwram;
	u32 bytepos;
	
#if ROMVERSION
	src_ewram=(u16*)goomba_mb_gba;
	src_iwram=NULL;
	emusize1=GOOMBA_MB_GBA_SIZE;
	emusize2=0;
#else
	src_iwram=(u16*)0x03000000;
	src_ewram=(u16*)0x02000000;
	emusize1=EMUSIZE1;
	emusize2=EMUSIZE2;
#endif

//	if(pogoshell) romsize=48+16+(*(u8*)(findrom(romnum)+48+4))*16*1024+(*(u8*)(findrom(romnum)+48+5))*8*1024;  //need to read this from ROM
//	else romsize=48+*(u32*)(findrom(romnum)+32);
	romsize = (0x8000 << (*(findrom(romnum)+0x148)));
	if(emusize1+romsize>192*1024) return 3;
	
	totalsize = emusize1+emusize2+romsize;

#if 0
    //this check frequently causes hangs, and is not necessary
	REG_RCNT=0x8003;		//general purpose comms - sc/sd inputs
	i=TIMEOUT;
	while(--i && (REG_RCNT&3)==3) delay();
	if(!i) return 1;

	i=TIMEOUT;
	while(--i && (REG_RCNT&3)!=3) delay();
	if(!i) return 1;
#endif

	REG_RCNT=0;			//non-general purpose comms

	i=250;
	do {
		DelayCycles(10);
		j=xfer(0x6202);
	} while(--i && j!=0x7202);
	if(!i) return 2;

	xfer (0x6100);
	p=(u16*)src_ewram;
	for(bytepos=0;bytepos<96; bytepos++) {		//send header
		xfer(*p);
		p++;
	}

	xfer(0x6202);
	mp.cb = 2;
	mp.pc = 0xd1;
	mp.startp=(u8*)Client;
	i=sizeof(Client);
	i=(i+15)&~15;		//16 byte units
	mp.endp=(u8*)Client+i;

	palette = 0xef;
//8x=purple->blue
//9x=blue->emerald
//ax=emerald->green
//bx=green->yellow
//cx=yellow->red
//dx=red->purple
//ex=purple->white
	mp.palette = palette;

	xfer(0x6300+palette);
	i=xfer(0x6300+palette);

	mp.cd[0] = i;
	mp.cd[1] = 0xff;
	mp.cd[2] = 0xff;

	key = (0x11 + (i & 0xff) + 0xff + 0xff) & 0xff;
	mp.hs_data = key;

	xfer(0x6400 | (key & 0xff));

	ie=REG_IE;
	REG_IE=0;		//don't interrupt
	REG_DM0CNT_H=0;		//DMA stop
	REG_DM1CNT_H=0;
	REG_DM2CNT_H=0;
	REG_DM3CNT_H=0;

	if(swi25(&mp)){	//Execute BIOS routine to transfer client binary to slave unit
		i=2;
		goto transferEnd;
	}
	//now send everything else

	REG_RCNT=0;			//non-general purpose comms
    i=200;
	do {
		delay();
		j=xfer(0x99);
	} while(--i && j!=0x99); //wait til client is ready
	if(!i){ //mbclient not responding
		i=2;
		goto transferEnd;
	}
	xfer(totalsize&0xFFFF);		//transmission size..
	xfer(totalsize>>16);

	p=(u16*)(src_ewram);	//(from ewram.)
	for(bytepos=0;bytepos<emusize1;bytepos+=2)		//send first part of emu
		xfer(*(p++));
	p=(u16*)(src_iwram);			//(from iwram)
	for(bytepos=0;bytepos<emusize2;bytepos+=2)		//send second part of emu
		xfer(*(p++));

	p=(u16*)findrom(romnum);	//send ROM
	for(bytepos=0;bytepos<romsize;bytepos+=2)
		xfer(*(p++));
	i=0;
transferEnd:
	REG_IE=ie;
	return i;
}

#endif
