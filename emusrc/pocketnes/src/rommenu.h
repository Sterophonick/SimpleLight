#ifndef __ROMMENU_H__
#define __ROMMENU_H__

#ifdef __cplusplus
extern "C" {
#endif

extern u32 oldinput;
//extern const unsigned int __fp_status_arm;
//extern u8 *textstart;//points to first NES rom (initialized by boot.s)
//extern u8 *ewram_start;
//extern u8 *end_of_exram;
extern int roms;//total number of roms
extern volatile int selectedrom;
extern volatile int selected_rom_options;
extern volatile int rommenu_state;
//extern char pogoshell_romname[32];	//keep track of rom name (for state saving, etc)
//extern char pogoshell;
//extern char rtc;
//extern char gameboyplayer;
//extern char gbaversion;
extern const int ne;

//#define PALTIMING 4

//void C_entry(void);
//void splash(void);
void rommenu(void);
u8 *find_nes_header(u8 *base);
u8 *findrom(int n);
int drawmenu(int sel);
int getinput(void);
//void cls(int chrmap);
//void drawtext(int row,char *str,int hilite);
//void setdarknessgs(int dark);
//void setbrightnessall(int light);

#ifdef __cplusplus
}
#endif

#endif
