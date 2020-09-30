//#if 0

#include "includes.h"

//extern const u8 font[];
//extern const u8 fontpal[];

EWRAM_BSS u32 oldkey;
EWRAM_BSS int selected;
extern u8 TEXTMEM[21][30];

extern int ui_x;
extern int ui_y_real;
EWRAM_BSS int old_ui_x;
EWRAM_BSS int ui_y;

#ifndef _const
#define _const __attribute__((const))
#endif

#if REDUCED_FONT

static const char char_lookup_1 [] = { '*', '|', ';', '@', '\\', '~', '^', '`', '{', '}', 127 };

_const int lookup_character(int character)
{
	if (character < '+')
	{
		if (character < 32)
		{
			character = 32;
		}
		character = char_lookup_1[character - 32];
		return character - 32 + FONT_FIRSTCHAR;
	}
	else
	{
		return character - 32 + FONT_FIRSTCHAR;
	}
}

static __inline int lookup_character_inverse(int character)
{
	character = (character + 32 - FONT_FIRSTCHAR) & 0x7F;
	if (character == '*')
	{
		character = ' ';
	}
	return character;
}

#else

#define lookup_character(xxxx) ((xxxx) - 32 + FONT_FIRSTCHAR)
#define lookup_character_inverse(xxxx) (((xxxx) + 32 - FONT_FIRSTCHAR)&0x7F)

#endif

void swap_column(int col)
{
	int row;
	if (col>=30) return;
	
	for (row=0;row<21;row++)
	{
		u16 from_vram=(SCREENBASE)[row*32+col];
		u8 from_textmem=TEXTMEM[row][col];
		u16 to_vram=(lookup_character(from_textmem & 0x7F) + FONT_PALETTE_NUMBER * 0x1000 + (from_textmem >> 7) * 0x1000);
		u8 to_textmem=( lookup_character_inverse(from_vram) + ((from_vram - FONT_PALETTE_NUMBER * 0x1000) & 0x1000) / 32);
		TEXTMEM[row][col]=to_textmem;
		(SCREENBASE)[row*32+col]=to_vram;
	}
}

void move_ui_expose()
{
	//get exposed area
	{
		u32 current_col=(old_ui_x/8)&0x3F;
		u32 new_col=(ui_x/8)&0x3F;
		u32 col;
		
		if (current_col!=new_col)
		{
			int delta=(new_col-current_col)&0x3F;
			if (delta>32)
			{
				current_col=(current_col-1)&0x3F;
				new_col=(new_col-1)&0x3F;
				for (col=current_col;col!=new_col;col=(col-1)&0x3F)
				{
					swap_column(col&0x1F);
				}
			}
			else
			{
				for (col=current_col;col!=new_col;col=(col+1)&0x3F)
				{
					swap_column(col&0x1F);
				}
			}
		}
	}
	old_ui_x = ui_x;
}

void move_ui_scroll()
{
	ui_y_real = ui_y + UI_Y_BASE;
	move_ui_asm();
}

void move_ui_wait()
{
	move_ui_scroll();
	waitframe();
	move_ui_expose();
}

void move_ui()
{
	move_ui_scroll();
	move_ui_expose();
}

void loadfont()
{
	//memcpy32(FONT_MEM,(const u32*)&font, font_size);
	LZ77UnCompVram((const u32*)&font_lz77,FONT_MEM);
}
void loadfontpal()
{
	memcpy32(FONT_PAL,&fontpal_bin,64);
}
void get_ready_to_display_text()
{
	//this function is used exclusively to display error messages
	setdarkness(0);
	REG_DISPCNT&=~(7 | FORCE_BLANK); //force mode 0, and turn off force blank
	REG_DISPCNT|=BG2_EN; //text on BG2
	REG_BG2CNT= 0 | (FONT_CHAR_PAGE << 2) | (UI_TILEMAP_NUMBER << 8) | (0<<14);
	REG_BG2VOFS = UI_Y_BASE;
	REG_BG2HOFS = 0;
	loadfont();
	loadfontpal();
}

void clearoldkey()
{
	oldkey=~REG_P1;
}

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

void cls(int chrmap)
{
	//chrmap is a bitfield
	//1 = left, 2 = right, 4 = whichever is off the screen, 8 = whichever is on the screen
	int i=0,len=0x200;
	u32 *scr=(u32*)SCREENBASE;
	
	const u32 FILL_PATTERN = (FONT_MEM_FIRSTCHAR + FONT_PALETTE_NUMBER*0x1000)*0x10001;
	
	if ( (chrmap & 8) || ((chrmap & 1) && !(ui_x & 256)) || ((chrmap & 2) && (ui_x & 256)) )
	{
		len=0x540/4;
		for(;i<len;i++)				//512x256
		{
			scr[i]=FILL_PATTERN;
		}
	}
	if ( (chrmap & 4) || ((chrmap & 1) && (ui_x & 256)) || ((chrmap & 2) && !(ui_x & 256)) )
	{
		u32 spaces = ' ' | (' ' << 8) | (' ' << 16) | (' ' << 24);
		memset32(TEXTMEM,spaces,sizeof(TEXTMEM));
	}
	ui_y=0;
	move_ui();
}

#if 0
void cls_primary()
{
	cls(8);
}

void cls_secondary()
{
	cls(4);
}
#endif

void drawtext_primary(int row, const char *str, int hilite)
{
	if (ui_x & 0x100)
	{
		row &= 0x1F;
		row |= 0x20;
	}
	else
	{
		row &= 0x1F;
	}
	drawtext(row, str, hilite);
}

void drawtext_secondary(int row, const char *str, int hilite)
{
	if (ui_x & 0x100)
	{
		row &= 0x1F;
	}
	else
	{
		row &= 0x1F;
		row |= 0x20;
	}
	drawtext(row, str, hilite);
}

void make_ui_visible()
{
	ui_border_visible|=1;
	
	loadfont();
	//cls(3);
	move_ui();
}

void make_ui_invisible()
{
	ui_border_visible&=~1;
	move_ui();
}

void scrolll(int doFade)
{
	make_ui_visible();
	int initial_ui_x = ui_x;
	int i;
	for(i = 0; i < 9; i++)
	{
		if (doFade == 1)
		{
			setdarkness(i);	//Darken screen
		}
		else if (doFade == 2)
		{
			setdarkness(8 - i);
		}
		ui_x=initial_ui_x + i * 32;
		move_ui_wait();
	}
}

void scrollr(int doFade)
{
	make_ui_visible();
	int i;
	int initial_ui_x = ui_x;
	for(i = 0; i < 9; i++)
	{
		if (doFade == 1)
		{
			setdarkness(i);	//Darken screen
		}
		else if (doFade == 2)
		{
			setdarkness(8 - i);
		}
		ui_x = initial_ui_x - i * 32;
		move_ui_wait();
	}
	cls_secondary();
}



void drawtext(int row,const char *str,int hilite)
{
	drawtextl(row,str,hilite,29);
}

void drawtextl(int row,const char *str,int hilite, int len)
{
	if ((row >= 32 && (ui_x & 256)) || (row <32 && !(ui_x&256)))
	{
		u16 *here=SCREENBASE+(row&0x1F)*32;
		int i=0;
		u16 asterisk;
		u16 space;
		int map_add = 0x1000 * FONT_PALETTE_NUMBER;
		map_add+= hilite * 0x1000;
		space = lookup_character(' ');
		asterisk = lookup_character('*') + map_add;
		
		if (hilite>=0)
		{
			//leading asterisk
			*here = hilite ? asterisk : space;
			here++;
		}
		else
		{
			hilite=-hilite-1;
		}
		
		while(str[i]>=' ' && i<29 && i <len)
		{
			here[i]=(u16)(lookup_character(str[i])+map_add);
			i++;
		}
		for(;i<29;i++)
		{
			here[i] = space;
		}
	}
	else
	{
		u8 *dest=&TEXTMEM[row&0x1F][0];
		if (hilite)
		{
			hilite=0x80;
			*dest='*'+0x80;
		}
		else
		{
			*dest=' ';
		}
		dest++;
		{
			int i=0;
			while (str[i]>=' ' && i<29)
			{
				*dest++=str[i++]+hilite;
			}
			for (;i<29;i++)
			{
				*dest=' ';
			}
		}
	}
}

void setdarkness(int dark) {
	darkness=dark;
	//REG_BLDCNT=0x01f1;				//darken game screen
	//REG_BLDY=dark;					//Darken screen
	//REG_BLDALPHA=(0x10-dark)<<8;	//set blending for OBJ affected BG0
}

void setbrightnessall(int light) {
	darkness=light|0x80;
	//REG_BLDCNT=0x00bf;				//brightness increase all
	//REG_BLDY=light;
}

void waitkey()
{
	u32 key;
	clearoldkey();
	do
	{
		key=getmenuinput(1);
		waitframe();
	} while (!(key & (A_BTN | B_BTN | START)));
}

//#endif
