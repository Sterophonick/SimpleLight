//#ifndef	EZKERNEL_HEADER
//#define EZKERNEL_HEADER

#include "ff.h"

#define MAX_pReadCache_size 0x20000
#define MAX_files     0x200
#define MAX_folder    0x100
#define MAX_NOR				0x40

#define MAX_path_len 0x100

#define FAT_table_size 0x400
#define FAT_table_SAV_offset 0x200
#define FAT_table_RTS_offset 0x300

#define DEBUG


#define VideoBuffer    (u16*)0x6000000
#define Vcache         (u16*)pReadCache
#define RGB(r,g,b) ((r)+(g<<5)+(b<<10))

#define PSRAMBase_S98			(u32)0x08800000
#define FlashBase_S98 		(u32)0x09000000
#define FlashBase_S98_end (u32)0x09800000

#define SAVE_sram_base (u32)0x0E000000
#define SRAMSaver		   (u32)0x0E000000


#define UNBCD(x) (((x) & 0xF) + (((x) >> 4) * 10))
#define _BCD(x) ((((x) / 10)<<4) + ((x) % 10))
#define _YEAR	0
#define _MONTH	1
#define _DAY	2
#define _WKD	3
#define _HOUR	4
#define _MIN	5
#define _SEC	6


typedef struct FM_NOR_FILE_SECT{////save to nor
	unsigned char filename[100];	
	u16 rompage ;
	u16 have_patch ;
	u16	have_RTS;
	u16 reserved;
	u32 filesize;
	u32 reserved2 ;
	char gamename[0x10];
} FM_NOR_FS;

typedef struct FM_Folder_SECT{
	unsigned char filename[100];	
} FM_Folder_FS;

typedef struct FM_FILE_SECT{
	unsigned char filename[100];
	u32 filesize;	
} FM_FILE_FS;


typedef enum {
	SD_list=0,
	NOR_list=1,
	SET_win=2,
	HELP=3,
}PAGE_NUM ;
//----------------------------
extern DWORD Get_NextCluster(	FFOBJID* obj,	DWORD clst);
extern DWORD ClustToSect(FATFS* fs,DWORD clst);
extern const unsigned char __attribute__((aligned(4)))gImage_SD[76800];
extern const unsigned char __attribute__((aligned(4)))gImage_NOR[76800];
extern const unsigned char __attribute__((aligned(4)))gImage_LOGO[76800];
extern const unsigned char __attribute__((aligned(4)))gImage_icons[1344];
extern const unsigned char __attribute__((aligned(4)))gImage_MENU[28160];

extern FM_NOR_FS pNorFS[MAX_NOR]EWRAM_BSS;
extern u8 pReadCache [MAX_pReadCache_size]EWRAM_BSS;
extern u8 __attribute__((aligned(4)))GAMECODE[4];


extern u16 gl_reset_on;
extern u16 gl_rts_on;
extern u16 gl_sleep_on;
extern u16 gl_cheat_on;


extern u16 gl_color_selected;
extern u16 gl_color_text;
extern u16 gl_color_selectBG_sd;
extern u16 gl_color_selectBG_nor;
extern u16 gl_color_MENU_btn;
extern u16 gl_color_cheat_count;
extern u16 gl_color_cheat_black;
extern u16 gl_color_NORFULL;
extern u16 gl_color_btn_clean;

u32 Setting_window(void);


u32 LoadRTSfile(TCHAR *filename);
void ShowTime(u32 page_num ,u32 page_mode);


//#endif