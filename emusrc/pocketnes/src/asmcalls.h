#ifndef __ASMCALLS_H__
#define __ASMCALLS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "speedhack.h"

#define PRG_BANK_BITS 3
#define PRG_BANK_SIZE (1 << (6 - PRG_BANK_BITS))
#define PRG_16 (16 / PRG_BANK_SIZE)
#define PRG_BANK_COUNT (64 / PRG_BANK_SIZE)
#define PRG_BANK_MASK (((-1) << (16 - PRG_BANK_BITS)) & 0xFFFF)
#define PRG_BANK_SHIFT (16 - PRG_BANK_BITS)


#if !GCC
extern u32 Image$$RO$$Limit;
#endif

extern u8 *chr_rom_table[256];
extern u8 spr_cache[16];
extern u8 spr_cache_disp[16];
extern u8 spr_cache_map[256];

extern u32 *_dmabankbuffer;

extern u32 volatile *_scrollbuff;
extern u32 volatile *_dmascrollbuff;
extern u8 volatile *_nesoambuff;
extern u8 volatile *_dmanesoambuff;
extern u16 volatile *_bg0cntbuff;
extern u16 volatile *_dmabg0cntbuff;
extern u16 volatile *_dispcntbuff;
extern u16 volatile *_dmadispcntbuff;

extern u32 volatile *_dma0buff;
extern u16 volatile *_dma1buff;
extern u16 volatile *_dma3buff;

#define dmabankbuffer _dmabankbuffer
#define dmascrollbuff _dmascrollbuff

#define scrollbuff _scrollbuff
#define dmascrollbuff _dmascrollbuff
#define nesoambuff _nesoambuff
#define dmanesoambuff _dmanesoambuff
#define bg0cntbuff _bg0cntbuff
#define dmabg0cntbuff _dmabg0cntbuff
#define dispcntbuff _dispcntbuff
#define dmadispcntbuff _dmadispcntbuff

#define dma0buff _dma0buff
#define dma1buff _dma1buff
#define dma3buff _dma3buff

extern u8 _ppuctrl0frame;

extern u32 _BGmirror;
extern u8 _nes_palette[32];
#define nes_palette _nes_palette

#if GCC
static __inline void breakpoint()
{
	__asm volatile ("mov r11,r11");
}
#else
void breakpoint(void);
#endif

extern u8 _has_vram;
extern u8 _bankable_vrom;
extern u8 _vram_page_mask;
extern u8 _vram_page_base;

extern u8 ppustat_;
extern u8 ppustat_savestate;

extern u8 dirty_rows[32];
extern u8 dirty_tiles[512];
extern u8 _bg_cache_full;

extern u8 _fourscreen;

//6502.s
void CPU_reset(void);
void ntsc_pal_reset(void);
void cpuhack_reset(void);
void run(int dont_stop);
extern void* op_table[256];
extern void default_scanlinehook(void);
extern void pcm_scanlinehook(void);
extern void CheckI(void);
extern u8 PAL60;
extern u32 cpustate[16];

extern u8 _wantirq;

extern u8** _instant_prg_banks;
extern u8** _instant_chr_banks;
extern u8 *rommap[4];
extern u8 *_memmap_tbl[PRG_BANK_COUNT];
extern u8 *_memmap_0;
extern u8 *_memmap_2;
extern u8 *_memmap_4;
extern u8 *_memmap_6;
extern u8 *_memmap_8;
extern u8 *_memmap_A;
extern u8 *_memmap_C;
extern u8 *_memmap_E;
#if PRG_BANK_SIZE == 4
extern u8 *_memmap_1;
extern u8 *_memmap_3;
extern u8 *_memmap_5;
extern u8 *_memmap_7;
extern u8 *_memmap_9;
extern u8 *_memmap_B;
extern u8 *_memmap_D;
extern u8 *_memmap_F;
#endif

extern u8 *_vram_map[8];
#define vram_map _vram_map
extern u8 _bank6[5];
extern u8 _bank8[4];
extern u8 _Cbank0[8];
extern u8 _nes_chr_map[8];
extern u8 _vrompages;
extern u8 _rompages;
extern u8 NES_VRAM[8192];
extern void* _readmem_tbl[PRG_BANK_COUNT];
extern void* _writemem_tbl[PRG_BANK_COUNT];
extern void *_writemem_0;
extern void *_writemem_2;
extern void *_writemem_4;
extern void *_writemem_6;
extern void *_writemem_8;
extern void *_writemem_A;
extern void *_writemem_C;
extern void *_writemem_E;
#if PRG_BANK_SIZE == 4
extern void *_writemem_1;
extern void *_writemem_3;
extern void *_writemem_5;
extern void *_writemem_7;
extern void *_writemem_9;
extern void *_writemem_B;
extern void *_writemem_D;
extern void *_writemem_F;
#endif

extern void *_readmem_0;
extern void *_readmem_2;
extern void *_readmem_4;
extern void *_readmem_6;
extern void *_readmem_8;
extern void *_readmem_A;
extern void *_readmem_C;
extern void *_readmem_E;
#if PRG_BANK_SIZE == 4
extern void *_readmem_1;
extern void *_readmem_3;
extern void *_readmem_5;
extern void *_readmem_7;
extern void *_readmem_9;
extern void *_readmem_B;
extern void *_readmem_D;
extern void *_readmem_F;
#endif



extern u32 frametotal;
extern u32 sleeptime;
extern u8 novblankwait;
extern u8 dontstop;

#if OLDSPEEDHACKS
extern u32 SPEEDHACK_FIND_BPL_BUF[32],SPEEDHACK_FIND_BMI_BUF[32],SPEEDHACK_FIND_BVC_BUF[32],
           SPEEDHACK_FIND_BVS_BUF[32],SPEEDHACK_FIND_BCC_BUF[32],SPEEDHACK_FIND_BCS_BUF[32],
           SPEEDHACK_FIND_BNE_BUF[32],SPEEDHACK_FIND_BEQ_BUF[32],SPEEDHACK_FIND_JMP_BUF[32];
#else

extern u16 SPEEDHACK_TEMP_BUF[48];
extern u16 SPEEDHACK_INCS[64];
extern speedhack_T speedhacks[4];

void set_cpu_hack(int new_hacknumber);


#endif

extern u8* _m6502_pc;
extern u8* _m6502_s;
extern u8* _lastbank;
extern int _cpu_cycles;

//apack.s
void depack(u8 *source, u8 *destination);

//boot.s
extern const u8 font[];				//from boot.s
extern const u8 fontpal[];				//from boot.s

//cart.s
void reset_buffers(void);
void reset_all(void);
void loadcart_asm(void);			//from cart.s
void map67_(int page);
void map89_(int page);
void mapAB_(int page);
void mapCD_(int page);
void mapEF_(int page);
void map89AB_(int page);
void mapCDEF_(int page);
void map89ABCDEF_(int page);
void chr0_(int page);
void chr1_(int page);
void chr2_(int page);
void chr3_(int page);
void chr4_(int page);
void chr5_(int page);
void chr6_(int page);
void chr7_(int page);
void chr01_(int page);
void chr23_(int page);
void chr45_(int page);
void chr67_(int page);
void chr0123_(int page);
void chr4567_(int page);
void chr01234567_(int page);
extern void *writeCHRTBL[8];
void updateBGCHR_(void);
void updateOBJCHR(void);
void mirror1_(void);
void mirror2V_(void);
void mirror2H_(void);
void mirror4_(void);
void mirrorKonami_(void);
void chrfinish(void);

//int savestate(void*);
//void loadstate(int,void*);

typedef enum
{
	EMUFLAGS_PALTIMING = 0x04,
	EMUFLAGS_FPS50 = 0x08,
	EMUFLAGS_DENDY = 0x10
} EMUFLAGS;

typedef enum
{
	REGION_NTSC = 0,
	REGION_PAL = EMUFLAGS_PALTIMING | EMUFLAGS_FPS50,
	REGION_DENDY = EMUFLAGS_FPS50 | EMUFLAGS_DENDY,
	REGION_OVERCLOCKED = EMUFLAGS_DENDY
} REGION;

extern u32 _emuflags;
extern u8* romstart;
extern u32 romnum;
extern u8 _scaling;
extern u8 _cartflags;
//extern u8 _hackflags;
//extern u8 _hackflags2;
//extern u8 _hackflags3;
extern u8 _mapper_number;
extern u8* _rombase;
extern u8* _vrombase;
extern u32 _vrommask;
extern u32 _rommask;
extern u8 _rompages;
extern u8 _vrompages;

//extern char lfnName[256];
//extern unsigned char globalBuffer[BYTE_PER_READ];
//extern unsigned char fatWriteBuffer[BYTE_PER_READ];
//extern unsigned char fatBuffer[BYTE_PER_READ];
//extern FAT_FILE openFiles[MAX_FILES_OPEN];

extern u8 NES_RAM[2048];
extern u8 NES_SRAM[8192];
extern u8 END_OF_EXRAM;
extern char SramName[256];
extern u8 NES_VRAM[8192];
extern u8 NES_VRAM2[2048];
extern u8 NES_VRAM4[4096];
//extern u8* cache_location[MAX_CACHE_SIZE];
extern u16 DISPCNTBUFF[240];
extern u16 DMA1BUFF[164];

extern u8 mapperstate[32];

void loadstate_gfx(void);

extern u8 AGB_BG[8192];


//cf.s
int cheat_test(u32 operation, int changeok);

//io.s
extern u32 _joycfg;				//from io.s
#define joycfg _joycfg
void resetSIO(u32);				//io.s
void vbaprint(const char *text);		//io.s
//disabled because LibGBA includes this already
//void LZ77UnCompVram(const u32 *source,u16 *destination);		//io.s
void waitframe(void);			//io.s
int CheckGBAVersion(void);		//io.s
void doReset(void);			//io.s
void suspend(void);			//io.s
void waitframe(void);		//io.s
int gettime(void);			//io.s
void spriteinit(void);		//io.s

void serialinterrupt(void);


//memory.s
extern u32 sram_R[];
extern u32 sram_W[];
extern u32 rom_R60[];
extern u32 empty_W[];
#if SAVE
extern u32 sram_W2[];
#else
#define sram_W2 sram_W
#endif

u32* memcpy32(void* dest, const void* src, int bytesToCopy);
u32* memmove32(void* dest, const void* src, int bytesToMove);
u32* memset32(void* dest, u32 value, int bytesToSet);
void memset8(void* dest, u8 value, int bytesToSet);  //halfword aligned only

/*
memory.s(7): EXPORT void
memory.s(8): EXPORT empty_R
memory.s(9): EXPORT empty_W
memory.s(10): EXPORT ram_R
memory.s(11): EXPORT ram_W
memory.s(12): EXPORT sram_R
memory.s(13): EXPORT sram_W
memory.s(14): EXPORT sram_W2
memory.s(15): EXPORT rom_R60
memory.s(16): EXPORT rom_R80
memory.s(17): EXPORT rom_RA0
memory.s(18): EXPORT rom_RC0
memory.s(19): EXPORT rom_RE0
*/

//ppu.s
//extern u32 *vblankfptr;			//from ppu.s
//extern u32 vbldummy;			//from ppu.s
void vblankinterrupt(void);		//from ppu.s
void hblankinterrupt(void);		//from ppu.s
void vcountinterrupt(void);		//from ppu.s
extern u32 AGBinput;			//from ppu.s
extern u32 EMUinput;

extern u32 _vramaddr;
extern u32 _vramaddr_dummy;

void build_chr_decode(void);
//void debug_(int,int);		//ppu.s
void paletteinit(void);		//ppu.s
void PaletteTxAll(void);	//ppu.s
void Update_Palette(void);

void PPU_reset(void);
//void PPU_init(void);

extern u32 FPSValue;		//from ppu.s
extern char fpsenabled;		//from ppu.s
extern char gammavalue;		//from ppu.s
extern char _twitch;			//from ppu.s
extern char _flicker;		//from ppu.s
extern char wtop;			//from ppu.s

extern u32 ppustate[10];
extern u16 _agb_pal[32];

//extern u32 agb_nt_map[4];

extern u8 _frameready;
extern u8 _firstframeready;

extern u32 _nextx;
extern u32 _scrollX;

extern volatile u8 _okay_to_run_hdma;

extern u16 DISPCNTBUFF2[240];

/*
ppu.s(9): EXPORT PPU_init
ppu.s(10): EXPORT PPU_reset
ppu.s(11): EXPORT PPU_R
ppu.s(12): EXPORT PPU_W
ppu.s(13): EXPORT agb_nt_map
ppu.s(14): EXPORT vram_map
ppu.s(15): EXPORT _vram_write_tbl
ppu.s(16): EXPORT VRAM_chr
ppu.s(17): EXPORT VRAM_chr2
ppu.s(18): EXPORT debug_
ppu.s(19): EXPORT AGBinput
ppu.s(20): EXPORT EMUinput
ppu.s(21): EXPORT paletteinit
ppu.s(22): EXPORT PaletteTxAll
ppu.s(23): EXPORT newframe
ppu.s(24): EXPORT agb_pal
ppu.s(25): EXPORT ppustate
ppu.s(26): EXPORT writeBG
ppu.s(27): EXPORT wtop
ppu.s(28): EXPORT gammavalue
ppu.s(29): EXPORT oambuffer
ppu.s(30): EXPORT ctrl1_W
ppu.s(31): EXPORT newX
ppu.s(32): EXPORT twitch
ppu.s(33): EXPORT flicker
ppu.s(34): EXPORT fpsenabled
ppu.s(35): EXPORT FPSValue
ppu.s(36): EXPORT vbldummy
ppu.s(37): EXPORT vblankfptr
ppu.s(38): EXPORT vblankinterrupt
ppu.s(39): EXPORT NES_VRAM
*/

//sound.s
//void make_freq_table(void);
//extern u16* _freqtbl;
//extern u16 FREQTBL2[2048];

extern u8 sound_state[56];

extern u8 PCMWAV[128];

extern u8 *_pcmstart, *_pcmcurrentaddr;

void timer1interrupt(void);
void timer_2_interrupt(void);
void timer_3_interrupt(void);

void Sound_hardware_reset(void);
void reset_sound_after_loadstate(void);

extern u8 _frame_mode; 
extern u8 _channel_enables;

extern u8 _reg_4000;
extern u8 _reg_4001;
extern u8 _reg_4002;
extern u8 _reg_4003;
extern u8 _reg_4004;
extern u8 _reg_4005;
extern u8 _reg_4006;
extern u8 _reg_4007;
extern u8 _reg_4008;

extern u8 _reg_400A;
extern u8 _reg_400B;
extern u8 _reg_400C;

extern u8 _reg_400E;
extern u8 _reg_400F;

extern u8 _frame_sequencer_step;
extern u8 _sq1_length;
extern u8 _reg_4001_written;
extern u8 _reg_4003_written;

extern u8 _frame_mode; 
extern u8 _sq2_length;
extern u8 _reg_4005_written;
extern u8 _reg_4007_written;

extern u8 _tri_linear;
extern u8 _tri_length;

extern u8 _reg_400B_written;

extern u8 _channel_enables;
extern u8 _noise_length;

extern u8 _reg_400F_written;

extern u8 _sq1_env;
extern u8 _sq1_env_delay;
extern u8 _sq1_sweep_delay;
extern u8 _sq1_neg_offset;

extern u8 _sq2_env;
extern u8 _sq2_env_delay;
extern u8 _sq2_sweep_delay;
extern u8 _sq2_neg_offset;

extern u8 _noise_env;
extern u8 _noise_env_delay;

extern int _sq1_period;
extern int _sq2_period;
extern int _tri_period;

extern int _pcmctrl;
extern int _pcmlength;
extern int _pcmcount;
extern int _pcmlevel;

//extern int _pcmstart;
//extern int _pcmcurrentaddr;

extern int _gba_sound1cnt_x;
extern int _gba_sound2cnt_h;
extern int _gba_timer_2;
extern int _gba_timer_3;
extern int _gba_sound4cnt_h;

extern int _freq_mult;
extern int _frame_sequencer_interval;

extern int _dmc_last_timestamp;
extern int _dmc_tick_counter;
extern int _dmc_period;
extern int _dmc_period_reciprocal;

extern int _dmc_length_counter;

extern u8 _dmc_remaining_bits;


/*
sound.s(4): EXPORT timer1interrupt
sound.s(5): EXPORT Sound_reset
sound.s(6): EXPORT updatesound
sound.s(7): EXPORT make_freq_table
sound.s(8): EXPORT _4000w
sound.s(9): EXPORT _4001w
sound.s(10): EXPORT _4002w
sound.s(11): EXPORT _4003w
sound.s(12): EXPORT _4004w
sound.s(13): EXPORT _4005w
sound.s(14): EXPORT _4006w
sound.s(15): EXPORT _4007w
sound.s(16): EXPORT _4008w
sound.s(17): EXPORT _400aw
sound.s(18): EXPORT _400bw
sound.s(19): EXPORT _400cw
sound.s(20): EXPORT _400ew
sound.s(21): EXPORT _400fw
sound.s(22): EXPORT _4010w
sound.s(23): EXPORT _4011w
sound.s(24): EXPORT _4012w
sound.s(25): EXPORT _4013w
sound.s(26): EXPORT _4015w
sound.s(27): EXPORT _4015r
sound.s(28): EXPORT pcmctrl
*/

//visoly.s
void doReset(void);			//io.s

//timeout.s
extern int TimeoutState[52];
extern int HandlersTable[31];
void decode_timeouts(void);
void encode_timeouts(void);
extern int _cycles_to_run;
extern volatile u8 _crash_disabled;

//aliases without the prefix
#define ppuctrl0frame	_ppuctrl0frame
#define BGmirror	_BGmirror
#define nes_palette	_nes_palette
#define agb_pal	_agb_pal
#define has_vram	_has_vram
#define bankable_vrom	_bankable_vrom
#define vram_page_mask	_vram_page_mask
#define vram_page_base	_vram_page_base
#define bg_cache_full	_bg_cache_full
#define fourscreen	_fourscreen
#define instant_prg_banks	_instant_prg_banks
#define instant_chr_banks	_instant_chr_banks
#define bank6	_bank6
#define bank8	_bank8
#define Cbank0	_Cbank0
#define nes_chr_map	_nes_chr_map
#define vrompages	_vrompages
#define rompages	_rompages
#define readmem_tbl	_readmem_tbl
#define writemem_tbl	_writemem_tbl
#define m6502_pc	_m6502_pc
#define m6502_s	_m6502_s
#define lastbank	_lastbank
#define emuflags	_emuflags
#define scaling	_scaling
#define cartflags	_cartflags
#define hackflags	_hackflags
#define hackflags2	_hackflags2
#define hackflags3	_hackflags3
#define mapper_number	_mapper_number
#define rombase	_rombase
#define vrombase	_vrombase
#define vrommask	_vrommask
#define rommask	_rommask
#define rompages	_rompages
#define vrompages	_vrompages
#define joycfg	_joycfg
#define frameready	_frameready
#define firstframeready	_firstframeready
#define memmap_tbl _memmap_tbl
#define nextx _nextx
#define scrollX _scrollX
#define twitch _twitch
#define flicker _flicker

#define memmap_0 _memmap_0
#define memmap_1 _memmap_1
#define memmap_2 _memmap_2
#define memmap_3 _memmap_3
#define memmap_4 _memmap_4
#define memmap_5 _memmap_5
#define memmap_6 _memmap_6
#define memmap_7 _memmap_7
#define memmap_8 _memmap_8
#define memmap_9 _memmap_9
#define memmap_A _memmap_A
#define memmap_B _memmap_B
#define memmap_C _memmap_C
#define memmap_D _memmap_D
#define memmap_E _memmap_E
#define memmap_F _memmap_F

#define readmem_0 _readmem_0
#define readmem_1 _readmem_1
#define readmem_2 _readmem_2
#define readmem_3 _readmem_3
#define readmem_4 _readmem_4
#define readmem_5 _readmem_5
#define readmem_6 _readmem_6
#define readmem_7 _readmem_7
#define readmem_8 _readmem_8
#define readmem_9 _readmem_9
#define readmem_A _readmem_A
#define readmem_B _readmem_B
#define readmem_C _readmem_C
#define readmem_D _readmem_D
#define readmem_E _readmem_E
#define readmem_F _readmem_F

#define writemem_0 _writemem_0
#define writemem_1 _writemem_1
#define writemem_2 _writemem_2
#define writemem_3 _writemem_3
#define writemem_4 _writemem_4
#define writemem_5 _writemem_5
#define writemem_6 _writemem_6
#define writemem_7 _writemem_7
#define writemem_8 _writemem_8
#define writemem_9 _writemem_9
#define writemem_A _writemem_A
#define writemem_B _writemem_B
#define writemem_C _writemem_C
#define writemem_D _writemem_D
#define writemem_E _writemem_E
#define writemem_F _writemem_F

#define romstart rombase
#define romnumber romnum
#define mapper mapper_number

#define vramaddr _vramaddr
#define vramaddr_dummy _vramaddr_dummy
#define okay_to_run_hdma _okay_to_run_hdma

#define crash_disabled _crash_disabled

#ifdef __cplusplus
}
#endif


#endif