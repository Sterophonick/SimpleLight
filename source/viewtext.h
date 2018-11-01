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

void GetLinebuf(char *str, u32 len, u16 w, u16 *linenumbuf, u32 *totalLineNum);

int viewText(u8 *txtp,u32 size,u32 startline,u32 pi);