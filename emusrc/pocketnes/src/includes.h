#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include "config.h"

#include <stdio.h>
#include <string.h>
#include "gba.h"

#include <gba_interrupt.h>

#define ARRSIZE(xxxx) (sizeof((xxxx))/sizeof((xxxx)[0]))

//#include "vsprintf.c.h"

#include "asmcalls.h"

#if MOVIEPLAYER
	#include "fs.h"
#endif

#include "main.h"
#include "loadcart.h"
#include "rommenu.h"
#include "pocketnes_text.h"

#include "cache.h"
#include "ui.h"
#include "cheat.h"
#include "speedhack.h"
#include "spritecache.h"

#if MOVIEPLAYER
	#include "savestate.h"
#endif

#include "sram.h"

#if SAVE
	#include "minilzo.107/minilzo.h"
	#include "savestate.h"
#endif

#include "ui.h"

#if MULTIBOOT
	#include "mbclient.h"
#endif

#endif

