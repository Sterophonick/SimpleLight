#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include <stdlib.h>
#include <gba_base.h>
#include <gba_dma.h>
#include <string.h>
#include <stdarg.h>
#include <gba_timers.h>

#include "draw.h"
#include "ezkernel.h"
#include "Ezcard_OP.h"
#include "lang.h"

#include "ff.h"

#include "images/TextBG.h"

int viewText(TCHAR *filename)
{
	u32 linenumbuf;
	u32 linenum;
	char linebuf[38];
	u32 filesize;
	u32 res;
	FIL txtdoc;
	res = f_open(&txtdoc, filename, FA_READ);
	if(res != FR_OK)
		return 1;
	filesize = f_size(&txtdoc);
	f_lseek(&txtdoc, 0x0000);
	DrawPic((u16*)gImage_TextBG, 0, 0, 240, 160, 0, 0, 1);
	for(linenumbuf = 0; linenumbuf < 12; linenumbuf++)
	{
		
	}
	return 0;
}