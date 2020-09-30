#ifndef __SPRITECACHE_H__
#define __SPRITECACHE_H__

#ifdef __cplusplus
extern "C" {
#endif

extern int sprite_cache_cursor;
void init_sprite_cache(void);
int add_if_needed(int count,u8 *base,int addthis);
void recache_sprites(void);

#ifdef __cplusplus
}
#endif

#endif
