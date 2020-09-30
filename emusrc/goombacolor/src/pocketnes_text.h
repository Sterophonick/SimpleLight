#ifndef __POCKETNES_TEXT_H__
#define __POCKETNES_TEXT_H__

#if REDUCED_FONT
#define FONT_FIRSTCHAR (0 - 10)
#define FONT_MEM_FIRSTCHAR 0
#else
#define FONT_FIRSTCHAR 0
#define FONT_MEM_FIRSTCHAR 0
#endif

#define FONT_CHAR_PAGE 1
#define FONT_PALETTE_NUMBER 5
#define UI_TILEMAP_NUMBER 9
#define SCREEN_ROW_OFFSET 11
#define UI_Y_BASE (SCREEN_ROW_OFFSET * 8)
#define SCREENBASE (u16*)((u8*)MEM_VRAM + UI_TILEMAP_NUMBER*2048 + SCREEN_ROW_OFFSET * 64 )
#define FONT_MEM (u16*)((u8*)MEM_VRAM + FONT_CHAR_PAGE*16384 + FONT_MEM_FIRSTCHAR*32 )
#define FONT_PAL (u16*)((u8*)MEM_PALETTE + FONT_PALETTE_NUMBER*16*2 )

#ifdef __cplusplus
extern "C" {
#endif

extern u32 oldkey;
extern int selected;

extern int ui_x;
extern int ui_y;

void move_ui_expose(void);
void move_ui_scroll(void);
void move_ui_wait(void);
void move_ui(void);

void loadfont(void);
void loadfontpal(void);
void get_ready_to_display_text(void);
u32 getmenuinput(int menuitems);
void cls(int chrmap);
void drawtext(int row,const char *str,int hilite);
void drawtextl(int row,const char *str,int hilite, int len);
void setdarkness(int dark);
void setbrightnessall(int light);
void waitkey(void);
void clearoldkey(void);

int lookup_character(int character);

void scrolll(int doFade);
void scrollr(int doFade);

static __inline void cls_primary()
{
	cls(8);
}

static __inline void cls_secondary()
{
	cls(4);
}

void drawtext_primary(int row, const char *str, int hilite);
void drawtext_secondary(int row, const char *str, int hilite);

void make_ui_visible(void);
void make_ui_invisible(void);


#ifdef __cplusplus
}
#endif

#endif
