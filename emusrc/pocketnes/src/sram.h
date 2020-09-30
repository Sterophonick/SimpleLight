#ifndef __SRAM_H__
#define __SRAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cheat.h"

#if SAVE

extern u32 sram_owner;
extern u32 my_checksum;

bool can_quickload(void);

void loadstatemenu(void);
void writeconfig(void);
void readconfig(void);
void bytecopy(u8 *dst,const u8 *src,int count);
void managesram(void);
void savestatemenu(void);
bool quickload(void);
bool quicksave(void);
int backup_nes_sram(int prompt_delete_menu);
void get_saved_sram(void);
u32 get_sram_owner(void);
void deletemenu(int statesize);

void getsram(void);

#if CHEATFINDER
void cheatload(void);
void cheatsave(void);
#endif

int find_rom_number_by_checksum(u32 sum);

extern u8* BUFFER1;
extern u8* BUFFER2;
extern u8* BUFFER3;

#define buffer1 BUFFER1
#define buffer2 BUFFER2
#define buffer3 BUFFER3

#endif

u32 checksum(u8 *p);
int using_flashcart(void);

#ifdef __cplusplus
}
#endif

#endif
