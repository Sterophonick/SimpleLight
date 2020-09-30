#ifndef __CACHE_H__
#define __CACHE_H__

#ifdef __cplusplus
extern "C" {
#endif

extern char rom_is_compressed;
extern char ewram_owner_is_sram;
extern char sprite_vram_in_use;
extern char do_not_decompress;

void rebankswitch(void);
void* memcpy_if_okay(void *dest, const void *src, size_t size);
void simpleswap_if_okay(void *a_in, void* b_in, u32 size);
void simpleswap (void* a_in, void* b_in, u32 size);

void assign_chr_pages(u8* base, int start, int end);
void assign_prg_pages(u8* base, int start, int end);
void assign_prg_pages2(u8* base, int start, int numpages);
void assign_chr_pages2(u8* base, int start, int numpages);
void swap_prg_pages(int page1, int page2, int numpages);
void make_contiguous(u8* cachebase, int page1, int page2);


#ifdef __cplusplus
}
#endif

#endif
