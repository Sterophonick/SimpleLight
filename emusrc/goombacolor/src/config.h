#ifndef __CONFIG_H__
#define __CONFIG_H__

#define GCC 1

#ifndef SRAM_SIZE
#define SRAM_SIZE 64
#endif

#define VERSION "05-04-19"

//#define LITTLESOUNDDJ 0
//Little Sound DJ Hack requires M3/G6/Supercard

//DEFAULT settings
#define RTCSUPPORT 1
#define ROMVERSION 1
#define SPLASH 1
#define MULTIBOOT 0
#define CARTSRAM 1
#define USETRIM 1
#define MOVIEPLAYER 0
#define MOVIEPLAYERDEBUG 0
#define RUMBLE 1
#define RESIZABLE 0
#define GOMULTIBOOT 0
#define POGOSHELL 1
#define VISOLY 1
#define SHANTAE_HACK 1
#define REDUCED_FONT 1
#define FAST_LDI_HL_A 1
#define EARLY_LINE_0 1
#define SPEEDHACKS_NEW 1
#define LCD_HACKS 1
#define LCD_HACKS_ACCURATE 1
#define LCD_HACKS_ACCURATE_DIV 1
#define JOYSTICK_READ_HACKS 0
#define EZFLASH_OMEGA_BUILD 1

#ifdef _MB_VERSION
	#define RTCSUPPORT 0
	#define ROMVERSION 0
	#define SPLASH 0
	#define MULTIBOOT 1
	#define GOMULTIBOOT 1
	#define CARTSRAM 0
	#define SHANTAE_HACK 0
#endif
#ifdef _GBAMP_VERSION
	#define RTCSUPPORT 0
	#define CARTSRAM 0
	#define MOVIEPLAYER 1
	#define MOVIEPLAYERDEBUG 1
	#define RUMBLE 0
	#define RESIZABLE 1
	#define GOMULTIBOOT 0
	#define POGOSHELL 0
	#define VISOLY 0
	#define SHANTAE_HACK 0
#endif

#define SPEEDHACKS_OLD 0


#endif
