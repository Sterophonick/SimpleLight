#ifndef __UI_H__
#define __UI_H__

#ifdef __cplusplus
extern "C" {
#endif


//#define HEX4 EDITFOLLOW||CHEATFINDER

char *number_at(char *dest, unsigned int n);

extern char str[32];

extern u8 autoA,autoB;				//0=off, 1=on, 2=R
extern u8 stime;
extern u8 autostate;

extern int cheatfinderstate;
extern int num_cheats;
//extern const int MAX_CHEATS; // 900/18
extern u8 compare_value;
extern short found_for[7];
extern int selected;//selected menuitem.  used by all menus.
extern int mainmenuitems;//? or CARTMENUITEMS, depending on whether saving is allowed

extern u32 oldkey;


extern const char MENUXITEMS[];

extern const fptr multifnlist[];
extern const fptr fnlist1[];
extern const fptr fnlist2[];
extern const fptr fnlist2[];

extern const fptr fnlist3[];
extern const fptr fnlist4[];
//extern const fptr fnlist5[];

extern const fptr *const fnlistX[];

extern const fptr drawuiX[];

extern const char *const autotxt[];
extern const char *const vsynctxt[];
extern const char *const sleeptxt[];
extern const char *const brightxt[];
extern const char *const ctrltxt[];
extern const char *const disptxt[];
extern const char *const flicktxt[];
extern const char *const cntrtxt[];
extern const char *const followtxt[];
extern const char *const followtxt2[];
extern const char *const branchtxt[];
extern const char *const emuname[];

extern u32 *const speedhack_buffers[];
extern const u8 hacktypes[];
extern const int num_speedhack_buffers;

void drawui1(void);
void drawui2(void);
void drawui3(void);
void drawui4(void);
//void drawui5(void);

u32 getmenuinput(int menuitems);
void subui(int menunr);
void ui(void);
void ui2(void);
void ui3(void);
void ui4(void);
//void ui5(void);
int text(int row,const char *str);
int text2(int row,const char *str);
void strmerge(char *dst,const char *src1,const char *src2);
void strmerge3(char *dst,const char *src1,const char *src2, const char *src3);
void strmerge4(char *dst,const char *src1,const char *src2, const char *src3, const char *src4);

#if CHEATFINDER | EDITFOLLOW
char *hexn(unsigned int n, int digits);
static __inline char *hex4(u32 n) {	return hexn(n,4); }
static __inline char *hex2(u32 n) {	return hexn(n,2); }
static __inline char *hex8(u32 n) {	return hexn(n,8); }
#endif

void drawclock(void);
void autoAset(void);
void autoBset(void);
void controller(void);
void sleepset(void);
void vblset(void);
void fpsset(void);
void brightset(void);
void multiboot(void);
void restart(void);
void exit_(void);
void sleep_(void);
void fadetowhite(void);
void scrolll(int f);
void scrollr(void);
void swapAB(void);
void display(void);
void flickset(void);
void autostateset(void);
void autostateset2(void);
void nextregion(void);
void cpuhacktoggle(void);
void ppuhacktoggle(void);
void autohacktoggle(void);
void followramtoggle(void);
void draw_input_text(int row, int column, char* str, int hilitedigit);
u32 inputhex(int row, int column, u32 value, u32 digits);
void inputtext(int row, int column, char *text, u32 length);
void selectfollowaddress(void);
void changehackmode(void);
void changebranchlength(void);
int getbranchhacknumber(void);
char *number(unsigned short n);
void autodetect_speedhack(void);
void find_best_speedhack(void);
void go_multiboot(void);

int GetRegion(void);
void SetRegion(int newRegion);

#define print_1(xxxx,yyyy) row=print_1_func(row,(xxxx),(yyyy))
#define print_2(xxxx,yyyy) row=print_2_func(row,(xxxx),(yyyy))
#define print_1_1(xxxx) row=text(row,(xxxx));
#define print_2_1(xxxx) row=text2(row,(xxxx));

int print_2_func(int row, const char *src1, const char *src2);
int print_1_func(int row, const char *src1, const char *src2);
int strmerge_str(int unused, const char *src1, const char *src2);
int text2_str(int row);
int text1_str(int row);

#ifdef __cplusplus
}
#endif

#endif
