#include <gba_base.h>



void Clear(u16 x, u16 y, u16 w, u16 h, u16 c, u8 isDrawDirect);
void ClearWithBG(u16* pbg,u16 x, u16 y, u16 w, u16 h, u8 isDrawDirect);
void DrawPic(u16 *GFX, u16 x, u16 y, u16 w, u16 h, u8 isTrans, u16 tcolor, u8 isDrawDirect);
void DrawHZText12(char *str, u16 len, u16 x, u16 y, u16 c, u8 isDrawDirect);
void DEBUG_printf(const char *format, ...);
void ShowbootProgress(char *str);