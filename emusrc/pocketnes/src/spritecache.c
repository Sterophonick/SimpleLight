#include "includes.h"

int sprite_cache_cursor=0;
int sprite_cache_size=0;

#define FIRST 8

#define MAX 8
#define MAX2 16

void init_sprite_cache()
{
	int i;
	
	memset32(spr_cache_map,0xFFFFFFFF,256);
	memset32(spr_cache,0xFFFFFFFF,MAX2);
	memset32(spr_cache_disp,0xFFFFFFFF,MAX2);
	
	if (vrompages==0)
	{
		for (i=0;i<8;i++)
		{
			spr_cache_map[i]=i+FIRST;
			spr_cache_disp[i]=i;
			spr_cache[i]=i;
		}
	}
	
	sprite_cache_cursor=0;
	sprite_cache_size=0;

}

//called from IRQ code
int add_if_needed(int count,u8 *base,int addthis)
{
	int i;
	for (i=0;i<count;i++)
	{
		if (base[i]==addthis) return count;
	}
	if (i<MAX)
	{
		base[i]=addthis;
		return i+1;
	}
	return count;
}

static bool search(int count, u8 *base, int lookforthis)
{
	int i;
	for (i=0;i<count;i++)
	{
		if (base[i]==lookforthis) return true;
	}
	return false;
}

//called from IRQ code
void recache_sprites()
{
//	int sanity=0;

	//PART 1: build keep and request tables
	u8 keep[MAX];
	u8 request[MAX];
	int keepcount=0;
	int requestcount=0;
	int requestbase=0;
	
	//bankbuffer: for each row (height 8px), 8 bytes, each byte is a vrom bank number
	u8 *bankbuffer=(u8*)dmabankbuffer;
	//oambuff = nes OAM buffer.  Y,T,X,A
	u8 *oambuff=(u8*)dmanesoambuff;
	//scrollbuffer, high bit of first 16 bit value is left/right pattern table selection
	u8 *scrollbuff=((u8*)dmascrollbuff)+1;
	s8 *const map=(s8*)spr_cache_map;
	
	int spr_y;
	int spr_t;
	int spr_ptable;
//	int spr_low_t;
	int spr_high_t;
	int spr_bank;
	s8 cache_bank;
	
	int i;
	
	if (0==(ppuctrl0frame&0x20))
	{
		//8x8
		for (i=0;i<64;i++)
		{
			spr_y=oambuff[i*4];
			if (spr_y<240)
			{
				spr_ptable=(scrollbuff[spr_y*4]&0x80)/0x20;
				spr_t=oambuff[i*4+1];
//				spr_low_t=spr_t&0x3F;
				spr_high_t=spr_t>>6;
//				spr_bank=bankbuffer[(spr_y/8)*8+spr_ptable+spr_high_t];
				spr_bank=bankbuffer[(spr_y&0xF8)+spr_ptable+spr_high_t];
				cache_bank=map[spr_bank];
				if (cache_bank<0)
				{
					requestcount=add_if_needed(requestcount,request,spr_bank);
				}
				else
				{
					keepcount=add_if_needed(keepcount,keep,spr_bank);
				}
			}
		}
	}
	else
	{
		//8x16
		for (i=0;i<64;i++)
		{
			spr_y=oambuff[i*4];
			if (spr_y<240)
			{
				spr_t=oambuff[i*4+1];
				spr_ptable=(spr_t&1)*4;
				spr_high_t=spr_t>>6;
//				spr_bank=bankbuffer[(spr_y/8)*8+spr_ptable+spr_high_t];
				spr_bank=bankbuffer[(spr_y&0xF8)+spr_ptable+spr_high_t];
				cache_bank=map[spr_bank];
				if (cache_bank<0)
				{
					requestcount=add_if_needed(requestcount,request,spr_bank);
				}
				else
				{
					keepcount=add_if_needed(keepcount,keep,spr_bank);
				}
			}
		}
	}
	//PART 2: fill spr_cache table, and rebuild spr_cache_map
	//first check if requesting more pages than can hold
	if (keepcount+requestcount>MAX)
	{
		requestcount=MAX-keepcount;
	}
	while (requestcount)
	{
		//is page at cursor discardable?
		int currentpage;
		bool notdiscardable;
		if (sprite_cache_size<MAX)
		{
			notdiscardable=false;
		}
		else
		{
			currentpage=spr_cache[sprite_cache_cursor];
			notdiscardable=search(keepcount,keep,currentpage);
		}
		
		if (notdiscardable)
		{
			//not discardable
			//skip that page
			sprite_cache_cursor++;
			if (sprite_cache_cursor>=MAX)
			{
				sprite_cache_cursor-=MAX;
			}
		}
		else
		{
			int newpage;
			//discardable
			//discard it!
			
			if (sprite_cache_size<MAX)
			{
				sprite_cache_size++;
			}
			else
			{
				map[currentpage]=-1;
			}
			
			//get requested page
			newpage=request[requestbase];
			//set cache page to requested page
			spr_cache[sprite_cache_cursor]=newpage;
			map[newpage]=sprite_cache_cursor+FIRST;
			
			//pop off request list
			requestbase++;
			requestcount--;
			//next page
			sprite_cache_cursor++;
			if (sprite_cache_cursor>=MAX)
			{
				sprite_cache_cursor-=MAX;
			}
		}
//		sanity++;
//		if (sanity==MAX)
//		{
//			return;
//			//bad thing
//		}
	}
}
