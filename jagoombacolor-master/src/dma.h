#ifndef __DMA_H__
#define __DMA_H__

extern u16 _dma_src;
extern u16 _dma_dest;
extern u8 _vrambank;
extern u8 _doing_hdma;
extern u8 _dma_blocks_remaining;

void UpdateTiles1(u8 *sourceAddress, int byteCount, int vramAddress1);
void UpdateTiles2(u8 *sourceAddress, int byteCount, int vramAddress1);
void UpdateTiles3(u8 *sourceAddress, int byteCount, int vramAddress1);

void DoDma(int byteCountRemaining);

/*
extern u8* _vram_packet_dest;
extern u8* _vram_packet_source;

extern u8 *vram_packet_first_dest;
extern u8 *vram_packet_first_source;
*/

/*
typedef struct
{
	u8 *dest;
	u8 *source;
	u16 length;
	u16 dirty;
} VramPacketData;

static const int MAX_PACKETS = 8;
extern VramPacketData vram_packets[];
*/

//void register_packet(u8* dest, u8* source, int size);
//void new_dma_packet(u8 *newDestAddress, u8* newSourceAddress);

#endif
