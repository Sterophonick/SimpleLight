#ifndef __MAIN_H__
#define __MAIN_H__

#define NORETURN __attribute__ ((noreturn))

#include "includes.h"
#if MOVIEPLAYER
extern int usinggbamp;
extern int usingcache;
extern File rom_file;
extern char save_slot;

extern int rom_filesize;
#endif

#define TRIM 0x4D495254

extern u32 max_multiboot_size;

extern u32 oldinput;
extern u8 *textstart;//points to first GB rom (initialized by boot.s)
extern u8 *ewram_start;//points to first NES rom (initialized by boot.s)
extern int roms;//total number of roms
extern int selectedrom;
extern int ui_x;
extern int ui_y;
extern int ui_y_real;
extern u32 max_multiboot_size;
#if POGOSHELL
extern char pogoshell_romname[32];	//keep track of rom name (for state saving, etc)
extern char pogoshell;
#endif
extern char rtc;
extern char gameboyplayer;
extern char gbaversion;

//#define WORSTCASE ((82048)+(82048)/64+16+4+64)

void C_entry(void);
void splash(const u16* splashImage);
#if MOVIEPLAYER
int get_saved_sram_CF(char* sramname);
int save_sram_CF(char* sramname);
#endif
void jump_to_rommenu(void) NORETURN;
void rommenu(void);
u8 *findrom2(int n);
u8 *findrom(int n);
int drawmenu(int sel);
int getinput(void);
//void cls(int chrmap);
//void drawtext(int row,char *str,int hilite);
//void drawtextl(int row,char *str,int hilite,int len);
//void setdarkness(int dark);
//void setbrightnessall(int light);

#endif
