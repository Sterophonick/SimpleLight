#include "includes.h"

//extern int SaveState(u8 *dest);
//extern int LoadState(u8 *source, int maxLength);


void AfterLoadState(void);

typedef int(*saveFuncPtr)(u8*);
typedef bool(*loadFuncPtr)(u8*, int);

int SaveVers(u8* dest);
int SaveRam(u8 *dest);
int SaveRam2(u8 *dest);
int SaveVram(u8 *dest);
int SaveIo(u8 *dest);
int SaveRegs(u8* dest);
int SaveMapper(u8 *dest);
int SavePalette(u8 *dest);
int SaveEmu(u8 *dest);
int SaveOam(u8 *dest);
int SaveSgb(u8 *dest);

static const int SAVE_VERSION = 1;
EWRAM_BSS int saveVersion;

bool LoadVers(u8 *src, int size);
bool LoadRam(u8 *src, int size);
bool LoadRam2(u8 *src, int size);
bool LoadVram(u8 *src, int size);
bool LoadIo(u8 *src, int size);
bool LoadRegs(u8 *src, int size);
bool LoadMapper(u8 *src, int size);
bool LoadPalette(u8 *src, int size);
bool LoadEmu(u8 *src, int size);
bool LoadOam(u8 *src, int size);
bool LoadSgb(u8 *src, int size);

const char tags[][4] =
{
	{'V','E','R','S'},
	{'R','A','M',' '},
	{'R','A','M','2'},
	{'V','R','A','M'},
	{'I','O',' ',' '},
	{'R','E','G','S'},
	{'M','A','P','R'},
	{'P','A','L',' '},
	{'E','M','U',' '},
	{'O','A','M',' '},
	{'S','G','B',' '},
};

const saveFuncPtr saveFunc[] =
{
	SaveVers,
	SaveRam,
	SaveRam2,
	SaveVram,
	SaveIo,
	SaveRegs,
	SaveMapper,
	SavePalette,
	SaveEmu,
	SaveOam,
	SaveSgb
};

const loadFuncPtr loadFunc[] =
{
	LoadVers,
	LoadRam,
	LoadRam2,
	LoadVram,
	LoadIo,
	LoadRegs,
	LoadMapper,
	LoadPalette,
	LoadEmu,
	LoadOam,
	LoadSgb
};

typedef enum 
{
	_Success,
	_OutOfBoundsTag,
	_UnknownTag,
	_Failed
} LoadStateError;

static u32 GetTagName(int tagId)
{
	if (tagId < 0 || tagId >= ARRSIZE(tags))
	{
		return 0;
	}
	else
	{
		const char *tagNameChar = tags[tagId];
		u32 tagNameInt = *((const u32*)tagNameChar);
		return tagNameInt;
	}
}

static int GetTagId(u32 tagName)
{
	for (int i = 0; i < ARRSIZE(tags); i++)
	{
		u32 otherTag = GetTagName(i);
		if (otherTag == tagName)
		{
			return i;
		}
	}
	return -1;
}


LoadStateError LoadState(u8 *source, int maxLength)
{
	u8 *ptr = source;
	u8 *limit = source + maxLength;
	LoadStateError status = _Success;
	while (ptr < limit)
	{
		//Get Tag Name
		u32 tagName = *((u32*)(ptr + 0));
		u32 tagLength = *((u32*)(ptr + 4));
		
		ptr += 8;
		u8 *nextPtr = ptr + (((tagLength - 1) | 3) + 1);
		if (nextPtr > limit)
		{
			return _OutOfBoundsTag;
		}
		
		int tagId = GetTagId(tagName);
		if (tagId == -1)
		{
			status = _UnknownTag;
			return status;
		}
		else
		{
			bool value = loadFunc[tagId](ptr, tagLength);
			if (value == false)
			{
				status = _Failed;
				return status;
			}
		}
		ptr = nextPtr;
	}
	AfterLoadState();
	return status;
}

int SaveState(u8 *dest)
{
	u8 *startPosition = dest;
	for (int i=0; i < ARRSIZE(tags); i++)
	{
		u32 tagName = GetTagName(i);
		int size = saveFunc[i](dest + 8);
		if (size > 0)
		{
			*((u32*)dest) = tagName;
			dest += 4;
			*((u32*)dest) = size;
			dest += 4;
			dest += size;
		}
	}
	return dest - startPosition;
}


int SaveVers(u8 *dest)
{
	*((u32*)(dest)) = SAVE_VERSION;
	return 4;
}
//in state.s
//int SaveRegs(u8* dest)
//{
//	return 0;
//}
int SaveRam(u8 *dest)
{
	memcpy32(dest, XGB_RAM, 0x2000);
	return 0x2000;
}
int SaveRam2(u8 *dest)
{
	if (gbc_mode)
	{
		memcpy32(dest, GBC_EXRAM, 0x6000);
		return 0x6000;
	}
	return 0;
}
int SaveVram(u8 *dest)
{
	if (!gbc_mode)
	{
		memcpy32(dest, XGB_VRAM, 0x2000);
		return 0x2000;
	}
	else
	{
		memcpy32(dest, XGB_VRAM, 0x4000);
		return 0x4000;
	}
	return 0;
}
//In state.s
//int SaveIo(u8 *dest)
//{
//	return 0;
//}
int SaveMapper(u8 *dest)
{
	memcpy32(dest, g_banks, 44);
	return 44;
}
int SavePalette(u8 *dest)
{
	if (gbc_mode)
	{
		memcpy32(dest, gbc_palette, 128);
		return 128;
	}
	return 0;
}
int SaveEmu(u8 *dest)
{
	return 0;
}
int SaveOam(u8 *dest)
{
	memcpy32(dest, _gb_oam_buffer_writing, 160);
	return 160;
}
int SaveSgb(u8 *dest)
{
	return 0;
}

bool LoadVers(u8 *src, int size)
{
	if (size == 4)
	{
		saveVersion = *((u32*)(src));
		return true;
	}
	return false;
}
//bool LoadRegs(u8 *src, int size)
//{
//	return false;
//}
bool LoadRam(u8 *src, int size)
{
	if (size == 0x2000)
	{
		memcpy32(XGB_RAM, src, size);
		return true;
	}
	return false;
}
bool LoadRam2(u8 *src, int size)
{
	if (size == 0x6000)
	{
		memcpy32(GBC_EXRAM, src, size);
		return true;
	}
	return false;
}
bool LoadVram(u8 *src, int size)
{
	if (size == 0x2000 || size == 0x4000)
	{
		memcpy32(XGB_VRAM, src, size);
		return true;
	}
	return false;
}
//In state.s
//bool LoadIo(u8 *src, int size)
//{
//	return false;
//}
bool LoadMapper(u8 *src, int size)
{
	if (size == 44)
	{
		memcpy32(g_banks, src, size);
		return true;
	}
	return false;
}
bool LoadPalette(u8 *src, int size)
{
	if (size == 128)
	{
		memcpy32(gbc_palette, src, size);
		return true;
	}
	return false;
}
bool LoadEmu(u8 *src, int size)
{
	return false;
}
bool LoadOam(u8 *src, int size)
{
	if (size == 160)
	{
		memcpy32(_gb_oam_buffer_writing, src, size);
		memcpy32(_gb_oam_buffer_screen, src, size);
		memcpy32(_gb_oam_buffer_alt, src, size);
		return true;
	}
	return false;
}
bool LoadSgb(u8 *src, int size)
{
	return false;
}
