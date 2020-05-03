#ifndef __CONFIG_H__
#define __CONFIG_H__

#define GCC 1

//#define COMPY 1  //can be defined in the makefile with -D COMPY

#define DEBUG 0				//Set to 1 to have code called at the end of each instruction (such as logging)

//#define VERSION_NUMBER "X alpha 3"
#define VERSION_NUMBER "2013-07-01"
//#define VERSION_NUMBER "DO NOT RELEASE"

#define EZFLASH_OMEGA_BUILD

//default options
//===============
//locked options (don't change):
#define REDUCED_FONT 1		//Use the reduced font to save memory (must change other code to disable this)
#define RESET_ALL 1			//Use the code to zero-fill variables when loading roms (prevents bugs)
#define VERSION_IN_ROM 1 	//When set to 1, jumps between sections are long jumps
#define OLDSPEEDHACKS 0		//Uses the old speed hack system instead of the new speed hack system
#define BRANCHHACKDETAIL 0	//For old speed hacks, displays which speed hack it is using
#define EDITBRANCHHACKS 0	//For old speed hacks, allows editing the speed hack directly
#define APACK 1				//Include APLIB code

//CPU options
#define HAPPY_CPU_TESTER 1	//Makes the CPU emulation more accurate
#define FULL_DMC 0			//Set to 1 to emulate DMC cycle stealing
//#define DEBUG 0				//Set to 1 to have code called at the end of each instruction (such as logging)
#define LESSMAPPERS 0		//Set to 1 to remove certain mappers

//Graphics options
#define DIRTYTILES 1		//Stores changed CHR-RAM tiles into a buffer to update later.  Helps synchronize graphics better and improves speed.
#define USE_BG_CACHE 1		//Use a buffer for background tiles
#define SPRITESCAN 1		//Check for 8 sprites deliberately placed in a scanline, then mask off sprites for that range
#define DRAW_ATTRIBUTE_TABLES 0	//When scroll is set to 'negitive' coordinates, draw the attribute tables as tiles
#define MIXED_VRAM_VROM 1	//Support games that use both CHR-RAM and CHR-ROM, or bankswitchable CHR-RAM (Pinbot, High speed, Metal Max chinese hack, Lagrange Point, etc)
#define SAVE 1				//Allow compressed save games, and saving to the cartridge SRAM
#define SAVE32 0			//SRAM size is limited to 32k by default
#define SAVE_FORBIDDEN 0	//Disallows saving games (for builds where saving doesn't work)
#define SAVESTATES_FORBIDDEN 0	//Disallows saving state (for builds where save states don't work)

//emulator memory options
#define USE_GAME_SPECIFIC_HACKS 1	//Fixes Magic of Scheherazade (rearranges the banks in memory)
#define USE_ACCELERATION 1	//Copies first 128K of ROM into EWRAM, not sure if this helps performance or not, but it does force 256 byte alignment.
#define MULTIBOOT 0			//Allows the Link Transfer feature
#define GOMULTIBOOT 0		//Allows use of the "Go Multiboot" feature (for multiboot builds)

//Other options
#define CHEATFINDER 1		//Enables the cheat finder
#define EDITFOLLOW 1		//Allow editing the information for sprite following
#define LINK 1				//Include code for GBA link cable multiplayer (this is different from link transfer)
#define RTCSUPPORT 1		//Displays the current time for cartridges that support the Real Time Clock
#define PREVIEWBUILD 0		//Shows a warning message before the emulator starts
#define MOVIEPLAYER 0		//This is a GBA Movie Player build
#define CRASH 1				//Enables the crash detector (if it fails to complete running a frame within 5 seconds, shows a stack dump)
#define VISOLY 1			//Include RESET code for the Flash2Advance and EZ4 cartridges

#if defined COMPY

//For PocketNES Compy
#undef USE_ACCELERATION
#define USE_ACCELERATION 0
#undef SAVE
#define SAVE 0
#undef GOMULTIBOOT
#define GOMULTIBOOT 1
#undef MULTIBOOT
#define MULTIBOOT 1
#undef CRASH
#define CRASH 0
#undef USE_GAME_SPECIFIC_HACKS
#define USE_GAME_SPECIFIC_HACKS 0
#undef RTCSUPPORT
#define RTCSUPPORT 0
#undef LESSMAPPERS
#define LESSMAPPERS 1
#undef VISOLY
#define VISOLY 0
#undef CHEATFINDER
#define CHEATFINDER 0

#elif defined GBAMP

//For PocketNES GBAMP
#undef CHEATFINDER
#define CHEATFINDER 0
#undef RTCSUPPORT
#define RTCSUPPORT 1
#undef SAVE
#define SAVE 0
#undef MOVEPLAYER
#define MOVIEPLAYER 1
#undef CRASH
#define CRASH 0

#else

//For regular PocketNES

#endif

#define CARTSAVE SAVE
#define SAVESTATES (SAVE | MOVIEPLAYER)

#ifndef GCC
#define GCC 0
#endif

#endif
