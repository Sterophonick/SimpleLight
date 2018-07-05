#include <gba_base.h>

#include "ff.h"
//---------------------------------------------------------------
void Chip_Reset();
void Block_Erase(u32 blockAdd);
void Chip_Erase();
void FormatNor();
void WriteFlash(u32 address,u8 *buffer,u32 size);
void IWRAM_CODE WriteFlash_with32word(u32 address,u8 *buffer,u32 size);
u32 Loadfile2NOR(TCHAR *filename, u32 NORaddress,u32 have_patch);
u32 GetFileListFromNor(void);