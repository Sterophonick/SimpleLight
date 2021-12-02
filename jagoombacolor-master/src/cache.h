#ifndef __CACHE_H__
#define __CACHE_H__

#define MAX_CACHE_SIZE 16

extern u8 *const bank_0;
extern u8 *const bank_1;

extern u8* cache_location[MAX_CACHE_SIZE];
extern int cache_queue_cursor;
extern u16 prgcache[MAX_CACHE_SIZE];
extern u8* cachebase;
extern u8 cachepages;
extern u32* pageoffsets;

u8 *make_instant_pages(u8* rom_base);
void init_cache(void);
void flushcache(void);
void clear_instant_prg(void);
void regenerate_instant_prg(void);
void loadcachepage(int i,int bank);
void getbank(int kilobyte);
void get_rom_map(void);
void update_cache(void);

void reload_vram_page1(void);

#endif
