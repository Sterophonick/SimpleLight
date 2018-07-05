#include <gba_base.h>

#include "ff.h"

void  SetSDControl(u16  control);
u16 IWRAM_CODE SD_Response(void);
u32 Read_SD_sectors(u32 address,u16 count,u8* SDbuffer);
u32 Write_SD_sectors(u32 address,u16 count,const u8* SDbuffer);
u16 IWRAM_CODE Read_S71NOR_ID();
u16 Read_S98NOR_ID();
void IWRAM_CODE SetRompage(u16 page);
void  IWRAM_CODE SetbufferControl(u16  control);
void SetPSRampage(u16 page);
void SetRampage(u16 page);
void IWRAM_CODE Progress(u16 x, u16 y, u16 w, u16 h, u16 c, u8 isDrawDirect);
void IWRAM_CODE Send_FATbuffer(u32*buffer,u32 mode);
void IWRAM_CODE SetRompageWithHardReset(u16 page,u32 bootmode);
void ReadSram(u32 address, u8* data , u32 size );
void WriteSram(u32 address, u8* data , u32 size );
void IWRAM_CODE Save_NOR_info(u8 * NOR_info_buffer,u32 buffersize);
void IWRAM_CODE Save_SET_info(u16 * SET_info_buffer,u32 buffersize);
void IWRAM_CODE Read_NOR_info();
u16 IWRAM_CODE Read_SET_info(u32 offset);
u32 Loadfile2PSRAM(TCHAR *filename);
u16 IWRAM_CODE Read_FPGA_ver(void);