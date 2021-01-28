#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include <stdlib.h>
#include <gba_base.h>
#include <gba_dma.h>
#include <string.h>


#include "ezkernel.h"
#include "draw.h"
#include "Newest_FW_ver.h"
extern u32 FAT_table_buffer[FAT_table_size/4]EWRAM_BSS;
u32 crc32(unsigned char *buf, u32 size);

extern FIL gfile;
// --------------------------------------------------------------------
#define FlashBase_S71		0x08000000

#define NOR_info_offset 0x7A0000
#define SET_info_offset 0x7B0000


// --------------------------------------------------------------------
void IWRAM_CODE SetSDControl(u16  control)
{
	*(u16 *)0x9fe0000 = 0xd200;
	*(u16 *)0x8000000 = 0x1500;
	*(u16 *)0x8020000 = 0xd200;
	*(u16 *)0x8040000 = 0x1500;
	*(u16 *)0x9400000 = control;
	*(u16 *)0x9fc0000 = 0x1500;
}
// --------------------------------------------------------------------
void IWRAM_CODE SD_Enable(void)
{
	SetSDControl(1);
}
// --------------------------------------------------------------------
void IWRAM_CODE SD_Read_state(void)
{
	SetSDControl(3);
}
// --------------------------------------------------------------------
void IWRAM_CODE SD_Disable(void)
{
	SetSDControl(0);
}
// --------------------------------------------------------------------
u16 IWRAM_CODE SD_Response(void)
{
	return *(vu16 *)0x9E00000; 
}
// --------------------------------------------------------------------
u32 IWRAM_CODE Wait_SD_Response()
{
	vu16 res;
	u32 count=0;
	while(1)
	{
		res = SD_Response();
		if(res != 0xEEE1)
		{
			return 0;
		}
			
		count++;
		if(count>0x100000)
		{
			//DEBUG_printf("time out %x",res);	
			//wait_btn();
			return 1;
		}
	}	
}
// --------------------------------------------------------------------
u32 IWRAM_CODE Read_SD_sectors(u32 address,u16 count,u8* SDbuffer)
{
	SD_Enable();
	
	u16 i;
	u16 blocks;
	u32 res;
	u32 times=2;
	for(i=0;i<count;i+=4)
	{
		blocks = (count-i>4)?4:(count-i);
			
	read_again:
		*(vu16 *)0x9fe0000 = 0xd200;
		*(vu16 *)0x8000000 = 0x1500;
		*(vu16 *)0x8020000 = 0xd200;
		*(vu16 *)0x8040000 = 0x1500;
		*(vu16 *)0x9600000 = ((address+i)&0x0000FFFF) ;
		*(vu16 *)0x9620000 = ((address+i)&0xFFFF0000) >>16;
		*(vu16 *)0x9640000 = blocks;
		*(vu16 *)0x9fc0000 = 0x1500;
		SD_Read_state();
		res = Wait_SD_Response();	
		SD_Enable();
		if(res==1)
		{
			times--;
			if(times) 
			{
				delay(5000);
				goto read_again;
			}			
		}
		dmaCopy((void*)0x9E00000, SDbuffer+i*512, blocks*512);
	}
	SD_Disable();
	return 0;
}
// --------------------------------------------------------------------
u32 IWRAM_CODE Write_SD_sectors(u32 address,u16 count, u8* SDbuffer)
{
	SD_Enable();
	SD_Read_state();
	u16 i;
	u16 blocks;
	u32 res;
	for(i=0;i<count;i+=4)
	{
		blocks = (count-i>4)?4:(count-i);
			
		dmaCopy( SDbuffer+i*512,(void*)0x9E00000, blocks*512);	
		*(vu16 *)0x9fe0000 = 0xd200;
		*(vu16 *)0x8000000 = 0x1500;
		*(vu16 *)0x8020000 = 0xd200;
		*(vu16 *)0x8040000 = 0x1500;
		*(vu16 *)0x9600000 = ((address+i)&0x0000FFFF);
		*(vu16 *)0x9620000 = ((address+i)&0xFFFF0000) >>16;
		*(vu16 *)0x9640000 = 0x8000+blocks;
		*(vu16 *)0x9fc0000 = 0x1500;
				
		res = Wait_SD_Response();						
	}
	delay(3000);
	SD_Disable();
	return 0;
}
// --------------------------------------------------------------------
u16 IWRAM_CODE Read_S71NOR_ID()
{
	u16 ID1;
	*((vu16 *)(FlashBase_S71)) = 0xF0;	
	*((vu16 *)(FlashBase_S71+0x555*2)) = 0xAA;
	*((vu16 *)(FlashBase_S71+0x2AA*2)) = 0x55;
	*((vu16 *)(FlashBase_S71+0x555*2)) = 0x90;
	ID1 = *((vu16 *)(FlashBase_S71+0xE*2));
	*((vu16 *)(FlashBase_S71)) = 0xF0;
	return ID1;
}	
// --------------------------------------------------------------------
u16 Read_S98NOR_ID()
{
	u16 ID1;
	*((vu16 *)(FlashBase_S98)) = 0xF0 ;	
	*((vu16 *)(FlashBase_S98+0x555*2)) = 0xAA;
	*((vu16 *)(FlashBase_S98+0x2AA*2)) = 0x55;
	*((vu16 *)(FlashBase_S98+0x555*2)) = 0x90;
	ID1 = *((vu16 *)(FlashBase_S98+0xE*2));
	return ID1;
}	
// --------------------------------------------------------------------
void IWRAM_CODE SetRompage(u16 page)
{
	*(vu16 *)0x9fe0000 = 0xd200;
	*(vu16 *)0x8000000 = 0x1500;
	*(vu16 *)0x8020000 = 0xd200;
	*(vu16 *)0x8040000 = 0x1500;
	*(vu16 *)0x9880000 = page;//C4
	*(vu16 *)0x9fc0000 = 0x1500;
}
// --------------------------------------------------------------------
void  IWRAM_CODE SetbufferControl(u16  control)
{
	*(u16 *)0x9fe0000 = 0xd200;
	*(u16 *)0x8000000 = 0x1500;
	*(u16 *)0x8020000 = 0xd200;
	*(u16 *)0x8040000 = 0x1500;
	*(u16 *)0x9420000 = control;//A1
	*(u16 *)0x9fc0000 = 0x1500;
}
// --------------------------------------------------------------------
void IWRAM_CODE SetPSRampage(u16 page)
{
	*(vu16 *)0x9fe0000 = 0xd200;
	*(vu16 *)0x8000000 = 0x1500;
	*(vu16 *)0x8020000 = 0xd200;
	*(vu16 *)0x8040000 = 0x1500;
	*(vu16 *)0x9860000 = page;//C3
	*(vu16 *)0x9fc0000 = 0x1500;
}
// --------------------------------------------------------------------
void IWRAM_CODE SetRampage(u16 page)
{
	*(vu16 *)0x9fe0000 = 0xd200;
	*(vu16 *)0x8000000 = 0x1500;
	*(vu16 *)0x8020000 = 0xd200;
	*(vu16 *)0x8040000 = 0x1500;
	*(vu16 *)0x9c00000 = page;//E0
	*(vu16 *)0x9fc0000 = 0x1500;
}
// --------------------------------------------------------------------
// --------------------------------------------------------------------
void IWRAM_CODE Send_FATbuffer(u32*buffer,u32 mode)
{	
	SetbufferControl(1);
	dmaCopy(buffer,(void*)0x9E00000, 0x400);
	if(mode==2)
	{
		SetbufferControl(0);
		return;
	}
	SetbufferControl(3);	
	if(mode==1)
	{
		SetbufferControl(0);
		return;
	}
			
	u16 res;
	while(1)
	{
		res = SD_Response();
		if(res != 0x0000)
			break;
	}
	
	while(1)
	{
		res = SD_Response();	
		if(res != 0x0001)
			break;
	}
	SetbufferControl(0);		
}
// --------------------------------------------------------------------
#define	RESET_EWRAM		  (1<<0)	/*!< Clear 256K on-board WRAM			*/
#define	RESET_IWRAM		  (1<<1)	/*!< Clear 32K in-chip WRAM				*/
#define	RESET_PALETTE		(1<<2)	/*!< Clear Palette						*/
#define	RESET_VRAM 	    (1<<3)	/*!< Clear VRAM							*/
#define	RESET_OAM		 	  (1<<4)	/*!< Clear OAM							*/
#define	RESET_SIO		 	  (1<<5)	/*!< Switches to general purpose mode	*/
#define	RESET_SOUND		 	(1<<6)	/*!< Reset Sound registers				*/
#define	RESET_OTHER		 	(1<<7)	/*!< all other registers				*/
// --------------------------------------------------------------------
extern u16 gl_ingame_RTC_open_status;

void IWRAM_CODE SetRompageWithHardReset(u16 page,u32 bootmode)
{
	Set_RTC_status(gl_ingame_RTC_open_status);
	SetRompage(page);
	RegisterRamReset(RESET_EWRAM|RESET_PALETTE| RESET_VRAM|RESET_OAM |RESET_SIO | RESET_SOUND | RESET_OTHER);
	if(bootmode==1){
		HardReset();
	}
	else{
		SoftReset_now();
	}
}
// --------------------------------------------------------------------
void IWRAM_CODE ReadSram(u32 address, u8* data , u32 size )
{
	register int i ;
	for(i=0;i<size;i++)
	{
		data[i]=*(u8*)(address+i);
	}
}
// --------------------------------------------------------------------
void IWRAM_CODE WriteSram(u32 address, u8* data , u32 size )
{
	register int i ;
	for(i=0;i<size;i++)
		*(vu8*)(address+i)=data[i];
}
// --------------------------------------------------------------------
void IWRAM_CODE Bank_Switching(u8 bank)
{
	*((vu8 *)(SAVE_sram_base+0x5555)) = 0xAA ;
	*((vu8 *)(SAVE_sram_base+0x2AAA)) = 0x55 ;
	*((vu8 *)(SAVE_sram_base+0x5555)) = 0xB0 ;
	*((vu8 *)(SAVE_sram_base+0x0000)) = bank ;	
}
// --------------------------------------------------------------------
void IWRAM_CODE Save_info(u32 info_offset, u8 * info_buffer,u32 buffersize)
{
	u32 offset;
	vu16* buf = (vu16*)info_buffer ;
	register u32 loopwrite ;
	vu16 v1,v2;
	
	*((vu16 *)(FlashBase_S71)) = 0xF0 ;	
	
	offset= info_offset;//0x7A0000/0x7B0000 ;
	
	*((vu16 *)(FlashBase_S71+0x555*2)) = 0xAA ;
	*((vu16 *)(FlashBase_S71+0x2AA*2)) = 0x55 ;
	*((vu16 *)(FlashBase_S71+0x555*2)) = 0x80 ;
	*((vu16 *)(FlashBase_S71+0x555*2)) = 0xAA ;
	*((vu16 *)(FlashBase_S71+0x2AA*2)) = 0x55 ;	
	*((vu16 *)(FlashBase_S71+offset)) = 0x30 ;//erase
	do
	{
		v1 = *((vu16 *)(FlashBase_S71+offset)) ;
		v2 = *((vu16 *)(FlashBase_S71+offset)) ;
	}while(v1!=v2);		
	//erase finish
	u32 i;
	for(loopwrite=0;loopwrite<(buffersize/32);loopwrite++)
	{
		*((vu16 *)(FlashBase_S71+0x555*2)) = 0xAA;
		*((vu16 *)(FlashBase_S71+0x2AA*2)) = 0x55;
		*((vu16 *)(FlashBase_S71+offset+loopwrite*32)) = 0x25;
		*((vu16 *)(FlashBase_S71+offset+loopwrite*32)) = 15;
		for(i=0;i<=15;i++)
		{
			*((vu16 *)(FlashBase_S71+offset+loopwrite*32 +2*i )) = buf[loopwrite*16+i];
		}	
		*((vu16 *)(FlashBase_S71+offset+loopwrite*32)) = 0x29;
		
		do
		{
			v1 = *((vu16 *)(FlashBase_S71+offset+loopwrite*32));
			v2 = *((vu16 *)(FlashBase_S71+offset+loopwrite*32));
		}while(v1!=v2);
	}

	*((vu16 *)(FlashBase_S71)) = 0xF0;	
}
// --------------------------------------------------------------------
void IWRAM_CODE Save_NOR_info(u8 * NOR_info_buffer,u32 buffersize)
{
	Save_info(NOR_info_offset, NOR_info_buffer,buffersize);
}
// --------------------------------------------------------------------
void IWRAM_CODE Save_SET_info(u16 * SET_info_buffer,u32 buffersize)
{
	Save_info(SET_info_offset, SET_info_buffer,buffersize);
}
// --------------------------------------------------------------------
void IWRAM_CODE Read_NOR_info()
{
	register u32 loopwrite ;
	for(loopwrite=0;loopwrite<sizeof(FM_NOR_FS)*0x40;loopwrite++)
	{
		((u16*)pNorFS)[loopwrite] = *((vu16 *)(FlashBase_S71+NOR_info_offset+loopwrite*2));
	}
}
// --------------------------------------------------------------------
u16 IWRAM_CODE Read_SET_info(u32 offset)
{
	return *((vu16 *)(FlashBase_S71+SET_info_offset+offset*2));
}
// --------------------------------------------------------------------
void IWRAM_CODE SetSPIControl(u16  control)
{
	*(u16 *)0x9fe0000 = 0xd200;
	*(u16 *)0x8000000 = 0x1500;
	*(u16 *)0x8020000 = 0xd200;
	*(u16 *)0x8040000 = 0x1500;
	*(u16 *)0x9660000 = control;
	*(u16 *)0x9fc0000 = 0x1500;
}
// --------------------------------------------------------------------
void IWRAM_CODE SPI_Enable(void)
{
	SetSPIControl(1);
}
// --------------------------------------------------------------------
void IWRAM_CODE SPI_Disable(void)
{
	SetSPIControl(0);
}
// --------------------------------------------------------------------
u16 IWRAM_CODE Read_FPGA_ver(void)
{
	u16 Read_SPI;
	SPI_Enable();	
	Read_SPI =  *(vu16 *)0x9E00000; 
	SPI_Disable();
	return Read_SPI;
}
// --------------------------------------------------------------------
void IWRAM_CODE SetSPIWrite(u16  control)
{
	*(u16 *)0x9fe0000 = 0xd200;
	*(u16 *)0x8000000 = 0x1500;
	*(u16 *)0x8020000 = 0xd200;
	*(u16 *)0x8040000 = 0x1500;
	*(u16 *)0x9680000 = control;
	*(u16 *)0x9fc0000 = 0x1500;
}
// --------------------------------------------------------------------
void IWRAM_CODE SPI_Write_Enable(void)
{
	SetSPIWrite(1);
}
// --------------------------------------------------------------------
void IWRAM_CODE SPI_Write_Disable(void)
{
	SetSPIWrite(0);
}
// --------------------------------------------------------------------
void IWRAM_CODE Set_RTC_status(u16  status)
{
	*(u16 *)0x9fe0000 = 0xd200;
	*(u16 *)0x8000000 = 0x1500;
	*(u16 *)0x8020000 = 0xd200;
	*(u16 *)0x8040000 = 0x1500;
	*(u16 *)0x96A0000 = status;
	*(u16 *)0x9fc0000 = 0x1500;
}
// --------------------------------------------------------------------
void IWRAM_CODE Set_AUTO_save(u16  mode)
{
	*(u16 *)0x9fe0000 = 0xd200;
	*(u16 *)0x8000000 = 0x1500;
	*(u16 *)0x8020000 = 0xd200;
	*(u16 *)0x8040000 = 0x1500;
	*(u16 *)0x96C0000 = mode;
	*(u16 *)0x9fc0000 = 0x1500;
}
// --------------------------------------------------------------------

void IWRAM_CODE Check_FW_update(u16 Current_FW_ver,u16 Built_in_ver)
{
	vu16 busy;
	vu32 offset;
	u32 offset_Y = 5;
	u32 line_x = 17;
	char msg[100];

	//DEBUG_printf("Current_FW_ver %x ",Current_FW_ver);	
	Clear(0, 0, 240, 160, RGB(0,18,24), 1);
	
	u32 get_crc32 = crc32( newomega_top_bin, newomega_top_bin_size);
	//DEBUG_printf("get_crc32 %x ",get_crc32);
	
	//if(	get_crc32 != 0x22475DDC) //fw3
	//if(	get_crc32 != 0xEE2DACE7) //fw4
	//if(	get_crc32 != 0x5B6B5129) //fw5
	//if(	get_crc32 != 0x7E6212AB) //fw6
	if( get_crc32 != 0xEFD03788) //fw7
	{
			sprintf(msg,"check crc32 error!");		
			DrawHZText12(msg,0,2,offset_Y+0*line_x, RGB(31,00,00),1);
			sprintf(msg,"press [B] to return");
			DrawHZText12(msg,0,2,offset_Y+2*line_x, 0x7FFF,1);	
			while(1)
			{
				VBlankIntrWait();	
				
				scanKeys();
				u16 keys = keysDown();
						
				if (keys & KEY_B) {
					return;
				}
			}		
	}

	sprintf(msg,"current firmware version: V%02d",Current_FW_ver);
	DrawHZText12(msg,0,2,offset_Y+1*line_x, 0x7FFF,1);	
	
	sprintf(msg,"will be updated to version: V%02d",Built_in_ver);
	DrawHZText12(msg,0,2,offset_Y+2*line_x, 0x7FFF,1);	

	sprintf(msg,"press [A] to update");
	DrawHZText12(msg,0,2,offset_Y+4*line_x, 0x7FFF,1);	
	sprintf(msg,"press [B] to cancel");
	DrawHZText12(msg,0,2,offset_Y+5*line_x, 0x7FFF,1);	
	
	while(1)
	{
		VBlankIntrWait();	
		
		scanKeys();
		u16 keys = keysDown();
				
		if (keys & KEY_A) {
			SPI_Write_Disable();
			Clear(2, offset_Y+4*line_x,220,15,RGB(0,18,24),1);	
			Clear(2, offset_Y+5*line_x,220,15,RGB(0,18,24),1);	
		
			sprintf(msg,"progress:");		
			DrawHZText12(msg,0,2,offset_Y+6*line_x, 0x7FFF,1);
									
			for(offset = 0x0000;offset<newomega_top_bin_size;offset+=256)
			{
					
				sprintf(msg," %lu%%",(offset*100/newomega_top_bin_size+1));
				Clear(54, offset_Y+6*line_x,120,15,RGB(0,18,24),1);	
				DrawHZText12(msg,0,54,offset_Y+6*line_x, 0x7FFF,1);	
				
				FAT_table_buffer[0] = (0x40000 + offset);
				
				dmaCopy(newomega_top_bin+offset,&FAT_table_buffer[1],256);  
				Send_FATbuffer(FAT_table_buffer,2); 
								   
				SPI_Write_Enable();
				while(1)
				{
					busy = SD_Response();
					if(busy==0) break;
				}
				SPI_Write_Disable();
				//DEBUG_printf("count %x ",count);
				//break;								
			}		
			sprintf(msg,"update finished,power off manual");
			DrawHZText12(msg,0,2,offset_Y+8*line_x, 0x7FFF,1);	
			
			while(1);
			break;
		}	
		else if (keys & KEY_B) {
			break;
		}
	}
}
// --------------------------------------------------------------------
static const u32 crc32tab[] = {
 0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL,
 0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L,
 0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L,
 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
 0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L,
 0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL,
 0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L,
 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L,
 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
 0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L,
 0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L,
 0x26d930acL, 0x51de003aL, 0xc8d75180L, 0xbfd06116L,
 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
 0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL,
 0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL,
 0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L, 0xe8b8d433L,
 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L,
 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
 0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL,
 0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L,
 0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL, 0xfcb9887cL,
 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,
 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
 0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL,
 0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L,
 0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
 0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L,
 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
 0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L,
 0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL,
 0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL,
 0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
 0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
 0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL,
 0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L,
 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL,
 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
 0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L,
 0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
 0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L,
 0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
 0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL,
 0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L,
 0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL,
 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL,
 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
 0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L,
 0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L,
 0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L, 0x1fda836eL,
 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
 0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,
 0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L,
 0xa7672661L, 0xd06016f7L, 0x4969474dL, 0x3e6e77dbL,
 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L,
 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
 0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L,
 0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL,
 0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L,
 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL 
};
u32 crc32(unsigned char *buf, u32 size)
{
	u32 i, crc;
	crc = 0xFFFFFFFF;
	for (i = 0; i < size; i++)
		crc = crc32tab[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
		
	return crc^0xFFFFFFFF;
}