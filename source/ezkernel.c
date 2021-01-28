#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include <stdlib.h>
#include <gba_base.h>
#include <gba_dma.h>
#include <string.h>
#include <stdarg.h>
#include <gba_timers.h>

#include "ff.h"
#include "draw.h"
#include "ezkernel.h"
#include "Ezcard_OP.h"
#include "saveMODE.h"
#include "RTC.h"
#include "NORflash_OP.h"
#include "lang.h"
#include "GBApatch.h"
#include "showcht.h"

#include "images/splash.h"
#include "images/SD.h"
#include "images/NOR.h" 
#include "images/SET.h"
#include "images/HELP.h"
#include "images/RECENTLY.h"

#include "images/MENU.h"
#include "images/icons.h"
#include "images/nor_icon.h"
#include "images/icon_FC.h"
#include "images/icon_GB.h"
#include "images/NOTFOUND.h"

#include "images/Chinese_manual.h"
#include "images/English_manual.h"

#include "goomba.h"
#include "pocketnes.h"



FM_FILE_FS pFilename_buffer[MAX_files]EWRAM_BSS;
FM_NOR_FS pNorFS[MAX_NOR]EWRAM_BSS;
FM_Folder_FS pFolder[MAX_folder]EWRAM_BSS;

FM_FILE_FS pFilename_temp;

u32 FAT_table_buffer[FAT_table_size/4]EWRAM_BSS;
u8 pReadCache [MAX_pReadCache_size]EWRAM_BSS;

u8 p_recently_play[10][512]EWRAM_BSS;
TCHAR currentpath_temp[MAX_path_len];
TCHAR current_filename[200];

u8 p_folder_select_show_offset[100]EWRAM_BSS;
u8 p_folder_select_file_select[100]EWRAM_BSS;
u32 folder_select;

u32 game_total_SD;
u32 game_total_NOR;
u32 folder_total;

u32 gl_currentpage;
u32 gl_norOffset;
u16 gl_select_lang;
u16 gl_engine_sel;

u16 gl_show_Thumbnail;
u16 gl_ingame_RTC_open_status;


u8 __attribute__((aligned(4)))GAMECODE[4];

FATFS EZcardFs;
FILINFO fileinfo;
DIR dir;
FIL gfile;
u8 dwName;

u16 gl_reset_on;
u16 gl_rts_on;
u16 gl_sleep_on;
u16 gl_cheat_on;

//----------------------------------------
u16 gl_color_selected 		= RGB(00,20,26);
u16 gl_color_text 				= RGB(31,31,31);
u16 gl_color_selectBG_sd 	= RGB(00,00,31);
u16 gl_color_selectBG_nor = RGB(10,10,10);
u16 gl_color_MENU_btn			= RGB(20,20,20);
u16 gl_color_cheat_count  = RGB(00,31,00);
u16 gl_color_cheat_black  = RGB(00,00,00);
u16 gl_color_NORFULL      = RGB(31,00,00);
u16 gl_color_btn_clean    = RGB(00,00,31);
//******************************************************************************
void delay(u32 R0)
{
  int volatile i;

  for ( i = R0; i; --i );
  return;
}
//---------------------------------------------------------------------------------
void wait_btn()
{
	while(1)
	{
		scanKeys();
		u16 keys = keysUp();
		if (keys & KEY_B) {
			break;
		}
	}
	//while(*(vu16*)0x04000130 == 0x3FF );
	//while(*(vu16*)0x04000130 != 0x3FF );
}
//---------------------------------------------------------------------------------
void Show_help_window()
{
	if(gl_select_lang == 0xE1E1)//english
	{
		DrawPic((u16*)gImage_English_manual, 240-70, 160-70, 70, 70, 0, 0, 1);//
	}
	else{
		DrawPic((u16*)gImage_Chinese_manual, 240-70, 160-70, 70, 70, 0, 0, 1);//
	}
	DrawHZText12("START  :",0,3,20, gl_color_selected,1);
		DrawHZText12(gl_START_help,0,52,20, gl_color_text,1);
		
	DrawHZText12("SELECT :",0,3,35, gl_color_selected,1);
		DrawHZText12(gl_SELECT_help,0,52,35, gl_color_text,1);
		
	DrawHZText12("L + A  :",0,3,50, gl_color_selected,1);
		DrawHZText12(gl_L_A_help,0,52,50, gl_color_text,1);
		
	DrawHZText12("L+START:",0,3,65, gl_color_selected,1);
		DrawHZText12(gl_LSTART_help,0,52,65, gl_color_text,1);	
		
	DrawHZText12(gl_online_manual,0,240-70-7,77, gl_color_text,1);
	while(1)
	{
		VBlankIntrWait(); 	
		scanKeys();
		u16 keys = keysDown();
		if (keys & KEY_L) {//return
			return;
		}		
	}
}
//---------------------------------------------------------------------------------
void Get_file_size(u32 num,char*str)
{
		u32 filesize;

		filesize = (pFilename_buffer[num].filesize) >>20 ;//M
		sprintf(str,"%4luM",filesize);
		if(filesize ==0)
		{
			filesize = (pFilename_buffer[num].filesize) /1024 ;//K
			sprintf(str,"%4luK",filesize);
		}
		if(filesize ==0)
		{
			filesize = pFilename_buffer[num].filesize  ;
			sprintf(str,"%4luB",filesize);
		}
}
//---------------------------------------------------------------------------------
void Show_ICON_filename(u32 show_offset,u32 file_select,u32 haveThumbnail)
{
	u32 need_show_game;
	u32 need_show_folder;
	u32 line;
	u32 char_num;

	if(show_offset >= folder_total)
	{
		need_show_folder = 0;
	}
	else
	{
		need_show_folder = folder_total-show_offset;
		if(need_show_folder > 10)
			need_show_folder = 10;
	}
	need_show_game = 10-need_show_folder;
	if(need_show_game > game_total_SD)
		need_show_game = game_total_SD;


	u32 y_offset= 20;
	u16 name_color = gl_color_text;

	for(line=0;line<need_show_folder;line++)
	{
		if(haveThumbnail)
		{
			if(line>3){
				char_num = 17;
			}
			else{
				char_num = 32;
			}
		}
		else{
			char_num = 32;
		}
		
		if(line== file_select)
		{			
			Clear(17,20 + file_select*14,(char_num == 17)?(17*6+1):(240-17),13,gl_color_selectBG_sd,1);
		}

		DrawPic((u16*)(gImage_icons+0*16*14*2),
			0,
			y_offset + line*14,
			16,
			14,
			1,
			gl_color_text,
			1);

		DrawHZText12(pFolder[show_offset+line].filename, char_num, 1+16, y_offset + line*14, name_color,1);
					
		if((haveThumbnail==1)&&(line>3))
		{}
		else
		{
			char msg[20];
			sprintf(msg,"%s","DIR");
			DrawHZText12(msg,0,221,y_offset + line*14, name_color,1);
		}
	}


	u32 offset=0;
	u32 strlen8;
	TCHAR *pfilename;
	if(show_offset >= folder_total)
		offset = show_offset - folder_total;
									
	for(line=need_show_folder;line < need_show_folder+need_show_game;line++)
	{
		if(haveThumbnail)
		{
			if(line>3){
				char_num = 17;
			}
			else{
				char_num = 32;
			}
		}
		else{
			char_num = 32;
		}
		
		if(line== file_select)
		{
			Clear(17,20 + file_select*14,(char_num == 17)?(17*6+1):(240-17),13,gl_color_selectBG_sd,1);
		}

		u32 showy = y_offset +(line)*14;
		pfilename = pFilename_buffer[offset+line-need_show_folder].filename;
		strlen8 = strlen(pfilename) ;
		u16* icon; 
		if(!strcasecmp(&(pfilename[strlen8-3]), "gba"))
		{
			icon = (u16*)(gImage_icons+1*16*14*2);
		}	
		else if(!strcasecmp(&(pfilename[strlen8-3]), "gbc"))
		{
			icon = (u16*)(gImage_icon_GB);
		}
		else if(!strcasecmp(&(pfilename[strlen8-2]), "gb"))
		{
			icon = (u16*)(gImage_icon_GB);
		}
		else if(!strcasecmp(&(pfilename[strlen8-3]), "nes"))
		{
			icon = (u16*)(gImage_icon_FC);
		}
		else 
		{
			icon = (u16*)(gImage_icons+2*16*14*2);
		}	
		DrawPic(icon,
			0,
			showy,
			16,
			14,
			1,
			gl_color_text,
			1);

		DrawHZText12(pFilename_buffer[offset+line-need_show_folder].filename, char_num, 1+16, showy, name_color,1);
		if((haveThumbnail==1)&&(line>3))
		{}
		else
		{		
			char msg[20];
			Get_file_size(offset+line-need_show_folder,msg);
			DrawHZText12(msg,0,208,showy, name_color,1);
		}
	}
}
//---------------------------------------------------------------------------------
void IWRAM_CODE Refresh_filename(u32 show_offset,u32 file_select,u32 updown,u32 haveThumbnail)
{
	u32 need_show_game;
	u32 need_show_folder;
	char msg[20];
	u32 y_offset= 20;	
		
	u32 char_num1;
	u32 char_num2;
	u32 clean_len1;
	u32 clean_len2;
	
	if(show_offset >= folder_total)
	{
		need_show_folder = 0;
	}
	else
	{
		need_show_folder = folder_total-show_offset;
		if(need_show_folder > 10)
			need_show_folder = 10;
	}
	need_show_game = 10-need_show_folder;
	if(need_show_game > game_total_SD)
		need_show_game = game_total_SD;

	u32 offset=0;
	if(show_offset >= folder_total)
		offset = show_offset - folder_total;

	u16 name_color1;
	//u16 name_color2;
	u32 xx1;
	u32 xx2;
	u32 showy1;
	u32 showy2;

	if(haveThumbnail)
	{
		switch(file_select)
		{
			case 0:
			case 1:
			case 2:
				char_num1 = 32;
				char_num2 = 32;
				clean_len1 = 240-17;
				clean_len2 = 240-17;
				break;
			case 3:
				if(updown ==3){
					char_num1 = 32;
					char_num2 = 17;
					clean_len1 = 240-17;
					clean_len2 = 17*6+1;
				}
				else{
					char_num1 = 32;
					char_num2 = 32;
					clean_len1 = 240-17;
					clean_len2 = 240-17;
				}
				break;	
			case 4:
				if(updown ==2){
					char_num1 = 32;
					char_num2 = 17;
					clean_len1 = 240-17;
					clean_len2 = 17*6+1;
				}
				else{
					char_num1 = 17;
					char_num2 = 17;
					clean_len1 = 17*6+1;
					clean_len2 = 17*6+1;
				}	
				break;
			case 5:
				if(updown ==2){
					char_num1 = 17;
					char_num2 = 17;
					clean_len1 = 240-17;
					clean_len2 = 17*6+1;
				}
				else{
					char_num1 = 17;
					char_num2 = 17;
					clean_len1 = 17*6+1;
					clean_len2 = 17*6+1;
				}	
				break;
			default:
				char_num1 = 17;
				char_num2 = 17;
				clean_len1 = 17*6+1;
				clean_len2 = 17*6+1;	
				break;
		}
	}
	else{
		char_num1 = 32;
		char_num2 = 32;
		clean_len1 = 240-17;
		clean_len2 = 240-17;
	}
		

	name_color1 = gl_color_text;
	//name_color2 = 0x7FFF;
	if(updown ==2) //down
	{
		xx1 = file_select-1;
		xx2 = file_select;
		showy1 = y_offset +(file_select-1)*14;
		showy2 = y_offset +(file_select)*14;
		ClearWithBG((u16*)gImage_SD,17, 20 + xx1*14, clean_len1, 13, 1);
		Clear(17,20 + xx2*14,clean_len2,13,gl_color_selectBG_sd,1);
	}
	else// if(updown ==3)//up
	{
		xx1 = file_select;
		xx2 = file_select+1;
		showy1 = y_offset +(file_select)*14;
		showy2 = y_offset +(file_select+1)*14;	
		Clear(17,20 + xx1*14,clean_len1,13,gl_color_selectBG_sd,1);	
		ClearWithBG((u16*)gImage_SD,17, 20 + xx2*14,clean_len2, 13, 1);	
	}

	if((file_select == (need_show_folder-1)) && (updown ==3))
	{
		DrawHZText12(pFolder[show_offset+xx1].filename, char_num1, 1+16, showy1, name_color1,1);
		DrawHZText12(pFilename_buffer[0].filename, char_num2, 1+16, showy2, name_color1,1);
		
		if(char_num1==32){
			sprintf(msg,"%s","DIR");
			DrawHZText12(msg,0,221,showy1, name_color1,1);
		}
		if(char_num2==32){
			Get_file_size(0,msg);
			DrawHZText12(msg,0,208,showy2, name_color1,1);			
		}
	}
	else if(file_select < need_show_folder)
	{
		DrawHZText12(pFolder[show_offset+xx1].filename, char_num1, 1+16, showy1, name_color1,1);
		DrawHZText12(pFolder[show_offset+xx2].filename, char_num2, 1+16, showy2, name_color1,1);

		sprintf(msg,"%s","DIR");
		if(char_num1==32){
			DrawHZText12(msg,0,221,showy1, name_color1,1);			
		}
		if(char_num2==32){
			DrawHZText12(msg,0,221,showy2, name_color1,1);
		}
	}
	else if((file_select == need_show_folder)&& (updown ==2))
	{
		DrawHZText12(pFolder[show_offset+xx1].filename,char_num1, 1+16, showy1, name_color1,1);
		DrawHZText12(pFilename_buffer[0].filename, char_num2, 1+16, showy2, name_color1,1);
		
		if(char_num1==32){
			sprintf(msg,"%s","DIR");			
			DrawHZText12(msg,0,221,showy1, name_color1,1);			
		}
		if(char_num2==32){
			Get_file_size(0,msg);
			DrawHZText12(msg,0,208,showy2, name_color1,1);
		}
	}
	else
	{
		DrawHZText12(pFilename_buffer[offset+xx1-need_show_folder].filename, char_num1, 1+16, showy1, name_color1,1);
		DrawHZText12(pFilename_buffer[offset+xx2-need_show_folder].filename, char_num2, 1+16, showy2, name_color1,1);

		if(char_num1==32){
			Get_file_size(offset+xx1-need_show_folder,msg);
			DrawHZText12(msg,0,208,showy1, name_color1,1);		
		}
		if(char_num2==32){
			Get_file_size(offset+xx2-need_show_folder,msg);
			DrawHZText12(msg,0,208,showy2, name_color1,1);
		}
	}
}
//---------------------------------------------------------------------------------
void Show_ICON_filename_NOR(u32 show_offset,u32 file_select)
{
	int need_show;
	int line;
	char msg[20];
	u16 name_color = gl_color_text;	
	u32 y_offset= 20;	
	u32 char_num=32;
	
	if(game_total_NOR<10)
		need_show = game_total_NOR;
	else
		need_show = 10;

	for(line=0;line<need_show;line++)
	{
		if(line== file_select){
			Clear(17,20 + file_select*14,240-17,13,gl_color_selectBG_nor,1);
		}		

		DrawPic((u16*)gImage_nor_icon/*(gImage_icons+2*16*14*2)*/,
			0,
			y_offset + line*14,
			16,
			14,
			1,
			gl_color_text,
			1);

		DrawHZText12(pNorFS[show_offset+line].filename, char_num, 1+16, y_offset + line*14, name_color,1);	
		sprintf(msg,"%4luM",pNorFS[show_offset+line].filesize >>20 );
		DrawHZText12(msg,0,208,y_offset + line*14, name_color,1);

	}
}
//---------------------------------------------------------------------------------
void Refresh_filename_NOR(u32 show_offset,u32 file_select,u32 updown)
{
	char msg[20];
	u16 name_color1;
	u32 xx1;
	u32 xx2;
	u32 showy1;
	u32 showy2;
	u32 y_offset= 20;
	u32 char_num;
	u32 clean_len;
	
	char_num = 32;
	clean_len = 240-17;
	
	name_color1 = gl_color_text;

	if(updown ==2) //down
	{
		xx1 = file_select-1;
		xx2 = file_select;
		showy1 = y_offset +(file_select-1)*14;
		showy2 = y_offset +(file_select)*14;
		ClearWithBG((u16*)gImage_NOR,17, 20 + xx1*14, clean_len, 13, 1);
		Clear(17,20 + xx2*14,clean_len,13,gl_color_selectBG_nor,1);
	}
	else //if(updown ==3)//up
	{
		xx1 = file_select;
		xx2 = file_select+1;
		showy1 = y_offset +(file_select)*14;
		showy2 = y_offset +(file_select+1)*14;
		Clear(17,20 + xx1*14,clean_len,13,gl_color_selectBG_nor,1);
		ClearWithBG((u16*)gImage_NOR,17, 20 + xx2*14,clean_len, 13, 1);
	}

	DrawHZText12(pNorFS[show_offset+xx1].filename, char_num, 1+16, showy1, name_color1,1);
	DrawHZText12(pNorFS[show_offset+xx2].filename, char_num, 1+16, showy2, name_color1,1);

	sprintf(msg,"%4luM",(pNorFS[show_offset+xx1].filesize) >>20 );
	DrawHZText12(msg,0,208,showy1, name_color1,1);
	sprintf(msg,"%4luM",(pNorFS[show_offset+xx2].filesize) >>20 );
	DrawHZText12(msg,0,208,showy2, name_color1,1);

}
//---------------------------------------------------------------------------------
void Show_game_num(u32 count,u32 list)
{
	char msg[20];
	if(list==0){
		if(game_total_SD+folder_total==0)
			count = 0;
		sprintf(msg,"[%03lu/%03lu]",count,game_total_SD+folder_total);
	}
	else{
		if(game_total_NOR==0)
			count = 0;
		sprintf(msg,"[%03lu/%03lu]",count,game_total_NOR);
	}
	DrawHZText12(msg,0,185,3, gl_color_text,1);
}
//---------------------------------------------------------------------------------
void Filename_loop(u32 shift,u32 show_offset,u32 file_select,u32 haveThumbnail)
{
	u32 need_show_folder;
	//u32 line;
	u32 char_num;	
	u32 y_offset= 20;	
	int namelen;
	static u32 orgtt = 123455;
	u32 timeout = 20;
	//u8 dwName=0;	
	u8 msg[128];
	u8 temp_filename[100];
	
	if(shift > timeout)
	{
		if(show_offset >= folder_total)
		{
			need_show_folder = 0;
		}
		else
		{
			need_show_folder = folder_total-show_offset;
			if(need_show_folder > 10)
				need_show_folder = 10;
		}

		if(haveThumbnail)
		{
			if(file_select>3){
				char_num = 17;
			}
			else{
				char_num = 33;
			}
		}
		else{
			char_num = 33;
		}

		
		u32 offset=0;
		if(show_offset >= folder_total)
			offset = show_offset - folder_total;

		if(file_select < need_show_folder)
		{
			strncpy(temp_filename,pFolder[show_offset+file_select].filename , 100 );
		
		}
		else
		{
			strncpy(temp_filename,pFilename_buffer[offset+file_select-need_show_folder].filename , 100 );		
		}
		
		namelen = strlen(temp_filename);
		if(namelen >(char_num-1) ) 
		{
			u32  tt = ((shift-timeout)/8)% (namelen);
			if(orgtt!= tt )
			{	
				orgtt = tt ;
				sprintf(msg,"%s   ",temp_filename + tt);
				strncpy(msg+strlen(msg) ,temp_filename , 128 - strlen(msg) );
				if(temp_filename[tt] > 0x80)
				{						
					if(dwName)
					{
						msg[0] = 0x20;
						dwName = 0;
					}
					else
						dwName = 1;
				}
				else
					dwName = 0;
					
				Clear(17,20 + file_select*14,(char_num)*6,13,gl_color_selectBG_sd,1);	
				DrawHZText12(msg, char_num-1, 1+16, y_offset + file_select*14, gl_color_text,1);
			}	
		}
	}		
}
//---------------------------------------------------------------------------------
void Show_MENU_btn()
{
	char msg[30];
	Clear(60,118-1,55,14,gl_color_MENU_btn,1);
	Clear(125,118-1,55,14,gl_color_MENU_btn,1);
	sprintf(msg,"%s",gl_menu_btn);
	DrawHZText12(msg,0,60,118, gl_color_text,1);
}
//---------------------------------------------------------------------------------
void Show_MENU(u32 menu_select,PAGE_NUM page,u32 havecht,u32 Save_num,u32 is_menu)
{
	int line;
	u32 y_offset= 30;
	u16 name_color;
	char msg[30];
	
	u32 linemax = (page==NOR_list)?3:(5+havecht);
	if(is_menu){
		linemax = 1;
	}
		
	for(line=0;line<linemax;line++)
	{		
		if(line== menu_select){
			name_color = gl_color_selected;
		}
		else if(line == 5)
		{
			if(havecht==1 && gl_cheat_on==0)
			{
				name_color = gl_color_MENU_btn;
			}
			else if(gl_cheat_count)
			{
				name_color = gl_color_cheat_count;
			}
			else{
				name_color = gl_color_text;
			}			
		}				
		else{
			name_color = gl_color_text;
		}

		if(page==NOR_list)
			DrawHZText12(gl_nor_op[line], 32, 60, y_offset + line*14, name_color,1);
		else
		{
			if(line == 5)//cheat
			{
				sprintf(msg,"%s(%d)",gl_rom_menu[line],gl_cheat_count);
				DrawHZText12(msg, 32, 60, y_offset + line*14, name_color,1);
			}
			else{
				DrawHZText12(gl_rom_menu[line], 32, 60, y_offset + line*14, name_color,1);
							
				if(line == 4)//save tpye
				{				
					switch(Save_num)
					{
						case 1:sprintf(msg,"%s","<  SRAM  >");//0x11
							break;
						case 2:sprintf(msg,"%s","<EEPROM8K>");//0x22
							break;
						case 3:sprintf(msg,"%s","<EEPROM512>");//0x23
							break;				
						case 4:sprintf(msg,"%s","<FLASH64 >");//0x32
							break;
						case 5:sprintf(msg,"%s","<FLASH128>");//0x31
							break;	
						case 0:	
						default:	
							sprintf(msg,"%s",     "<  AUTO  >");	
							break;			
						
					}
					//ClearWithBG((u16*)gImage_MENU -64,60+60, y_offset + line*14, 10*6, 13, 1);
					DrawHZText12(msg, 32, 60+54, y_offset + line*14, name_color,1);					
				}
			}
		}
	}
}
//------------------------------------------------------------------
void Show_game_name(u32 total,u32 Select)
{
	u32 need_show;	
	u32 line;
	
	char msg[256];
	
	u32 X_offset=1;
	u32 Y_offset=20;
	u32 line_x = 14;
	//u32 str_len;
	u16 name_color;
	
	if(total<10)
		need_show = total;
	else
		need_show = 10;
	for(line=0;line<need_show;line++)
	{
		if(line== Select)
			name_color = gl_color_selected;
		else
			name_color = gl_color_text;
			
		sprintf(msg,"%s",&(p_recently_play[line]) );				
		DrawHZText12(msg,39,X_offset,Y_offset+line*line_x, name_color,1);					
			
	}		
}
//---------------------------------------------------------------------------------
u32  get_count(void)
{
	u32 res;
	u32 count=0;
	u8 buf[512];	
	res = f_open(&gfile,"/SAVER/Recently play.txt", FA_READ);	
	if(res == FR_OK)//have a play file
	{
		f_lseek(&gfile, 0x0);
		memset(buf,0x00,512);
		while(f_gets(buf, 512, &gfile) != NULL)
		{		
			//DrawHZText12(buf, 32, 1+16, showy, name_color,1);
			Trim(buf);
			if(buf[0] != '/') break;
			memset(p_recently_play[count],0x00,512);
			dmaCopy(buf,&(p_recently_play[count]), 512);		
			memset(buf,0x00,512);
			count++;
			if(count==10)break;
		}
	}
	f_close(&gfile);
	return count;
}
//---------------------------------------------------------------------------------
u32 show_recently_play(void)
{
	u32 res;
	u32 all_count=0;
	u32 Select = 0;
	u32 re_show = 1;
	u32 return_val=0xBB;
	u32 firsttime = 1;
	
	DrawPic((u16*)gImage_RECENTLY, 0, 0, 240, 160, 0, 0, 1);
	DrawHZText12(gl_recently_play,0,(240-strlen(gl_recently_play)*6)/2,4, gl_color_text,1);//TITLE
	
	all_count = get_count();	
	if(all_count)
	{				
		setRepeat(15,1);		
		while(1)
		{
			VBlankIntrWait();
			VBlankIntrWait();	
					
			if(re_show)
			{
				Show_game_name(all_count,Select);
				re_show = 0;
			}						
			scanKeys();
			u16 keysrepeat = keysDownRepeat();	
			u16 keysup = keysUp();
			if (keysrepeat & KEY_DOWN) {
				if(Select < (all_count-1)){
					Select++;
					re_show=1;
				}		
			}
			else if(keysrepeat & KEY_UP){					
				if(Select){
					Select--;
					re_show=1;
				}
			}
			else if(keysup & KEY_B){	
				return_val = 0xBB;				
				break;
			}
			else if(keysup & KEY_A){					
	 			return_val = Select;	 				
	 			break;
			}						
		}
	}
	else{
		
		DrawHZText12(gl_no_game_played,0,1,20, gl_color_text,1);		
		while(1)
		{
			VBlankIntrWait();
			VBlankIntrWait();	
			scanKeys();
			u16 keysup = keysUp();
			if(keysup & KEY_B){	
				return_val = 0xBB;				
				break;
			}
		}
		
	}
	return return_val;
}
//------------------------------------------------------------------
void Make_recently_play_file(TCHAR* path,TCHAR* gamefilename)
{
	u32 res;
	u32 i;
	u32  count;
	int get=1;
	u8 buf[512];	
	
	//res=f_chdir("/SAVER");
	//is in SAVER
	count = get_count();
		
	memset(buf,0x00,512);
	if(strcmp(path,"/") ==0){		
		sprintf(buf,"%s%s",path,gamefilename);	
	}
	else{
		sprintf(buf,"%s/%s",path,gamefilename);	
	}	

	for(i=0;i<count;i++)
	{
		get = strcmp(buf,p_recently_play[i]) ;
		if(get==0)
		{
			u32 j;
			for(j=i;j>0;j--){
				memset(p_recently_play[j],0x00,512);
				dmaCopy(&(p_recently_play[j-1]),&(p_recently_play[j]), 512);					
			}
			break;			
		}
	}	
	
	if(get != 0){
		if(count==10){
			for(i=9;i>0;i--){
				memset(p_recently_play[i],0x00,512);
				dmaCopy(&(p_recently_play[i-1]),&(p_recently_play[i]), 512);	
			}		
		}
		else if(count){
			for(i=count;i>0;i--){
				memset(p_recently_play[i],0x00,512);
				dmaCopy(&(p_recently_play[i-1]),&(p_recently_play[i]), 512);	
			}
		}	
	}
	dmaCopy(buf,&(p_recently_play[0]), 512);	//write first one
		
	res = f_open(&gfile,"Recently play.txt", FA_WRITE | FA_OPEN_ALWAYS);
	if(res == FR_OK)
	{	
		f_lseek(&gfile, 0x0000);
		for(i=0;i<count+1;i++){			
			res=f_printf(&gfile, "%s\n", p_recently_play[i]);
		}
		f_close(&gfile);
	}
}
//---------------------------------------------------------------------------------
void init_FAT_table(void)
{
	//memset(FAT_table_buffer,0,0x200);
	FAT_table_buffer[0] = 0x00000000;
	CpuFastSet( FAT_table_buffer, FAT_table_buffer, FILL | (FAT_table_size/4));
	FAT_table_buffer[2] = 0xFFFFFFFF;
}
//---------------------------------------------------------------------------------
u32 Check_game_save_FAT(TCHAR *filename,u32 game_save_rts)
{
	u32 res;
	//u32 ret;
	FIL file;
	u32 *FAT_table_P;
	u32 getcluster;
	u32 getcluster_old;
	u32 cluster_num = 0;
	u32 lastest_cluster;

	res = f_open(&file, filename, FA_READ);

	if(res != FR_OK)
			return 0xffffffff;


	#ifdef DEBUG
		//DEBUG_printf("first clust %x;  sec=%x ",(&file)->obj.sclust,	ClustToSect(&EZcardFs,(&file)->obj.sclust)	);
		//DEBUG_printf("fs->fs_type %x",(&EZcardFs)->fs_type);
	#endif
	if((&EZcardFs)->fs_type == FS_FAT16)
	{
		lastest_cluster = 0xFFFF;
	}
	else{
		lastest_cluster = 0xFFFFFF7;
	}
	getcluster =  (&file)->obj.sclust;

	if(game_save_rts == 1)
	{
		FAT_table_P = FAT_table_buffer;
	}
	else if	(game_save_rts == 2)
	{
		FAT_table_P = FAT_table_buffer+ FAT_table_SAV_offset/4;
	}
	else
	{
		FAT_table_P = FAT_table_buffer+ FAT_table_RTS_offset/4;
	}	


	*FAT_table_P = 0x00000000;
	FAT_table_P++;
	*FAT_table_P = (ClustToSect(&EZcardFs,getcluster));
	FAT_table_P++;

	getcluster_old = getcluster;
	do {
		getcluster =  Get_NextCluster(&(&file)->obj,getcluster);
		cluster_num++;
		if(getcluster != (getcluster_old+1)) {
			#ifdef DEBUG
				//DEBUG_printf("getcluster = %x",getcluster);
			#endif
			*FAT_table_P = (cluster_num * (&EZcardFs)->csize);//sector_per_cluster
			FAT_table_P++;
			*FAT_table_P = (ClustToSect(&EZcardFs,getcluster));//getcluster;
			FAT_table_P++;
		}
		getcluster_old = getcluster;
	} while(getcluster < lastest_cluster);
	*--FAT_table_P = 0x0;
	*--FAT_table_P = 0xffffffff;

	f_close(&file);
	return 0;
}
//---------------------------------------------------------------------------------
u32 IWRAM_CODE Loadsavefile(TCHAR *filename)
{
	UINT ret;
	UINT filesize;
	UINT left;
	FIL file;

	switch(f_open(&file, filename, FA_READ))
	{
		case FR_OK:
		{
			filesize = f_size(&file);
			if(filesize > 128*1024)
				filesize = 128*1024;

			SetRampage(0x0);
			
			if(filesize>64*1024)
			{
		      f_read(&file, pReadCache, 64*1024, (UINT *)&ret);
					WriteSram(SRAMSaver, pReadCache , 64*1024 );
					SetRampage(0x10);
					left = filesize - 64*1024 ;
		      f_read(&file, pReadCache, left, (UINT *)&ret);
					WriteSram(SRAMSaver, pReadCache , left );
			}
			else
			{
				f_read(&file, pReadCache, filesize, (UINT *)&ret);
				WriteSram(SRAMSaver,pReadCache,filesize);
			}
	    f_close(&file);
	    SetRampage(0x0);
	    return 1;
    }
    default:
			return false;
  }
}
//---------------------------------------------------------------------------------
u32 IWRAM_CODE LoadRTSfile(TCHAR *filename)
{
	UINT ret;
	UINT filesize;
	FIL file;
	u32 page;

	switch(f_open(&file, filename, FA_READ))
	{
		case FR_OK:
		{
			filesize = f_size(&file);
			if(filesize > 0x70000)
				filesize = 0x70000;

			for(page = 0x40;page<0xB0;page += 0x10)
			{
				SetRampage(page);
				f_read(&file, pReadCache, 64*1024, (UINT *)&ret);
				WriteSram(SRAMSaver, pReadCache , 64*1024 );	
			}
	    f_close(&file);
	    SetRampage(0x0);
	    return 1;
    }
    default:
			return false;
  }
}
//---------------------------------------------------------------------------------
u32 SavefileWrite(TCHAR *filename,u32 savesize)
{
	FIL file;
	if(savesize==0) return 0xff;
	u32 ret=f_open(&file, filename, FA_WRITE | FA_CREATE_ALWAYS);
	switch(ret)
	{
		case FR_OK:
		{
			int i;
			unsigned int written;
			memset(pReadCache,0xFF,0x200*4);

			if(savesize < 0x800)
			{
				for(i=0;i<(savesize+0x1FF)/0x200 ;i++)
				{
		      f_write(&file, pReadCache, 0x200, &written);
		      if(written != 0x200) break;
		    }	
			}
			else
			{
				for(i=0;i<(savesize+0x1FF)/0x800 ;i++)
				{
		      f_write(&file, pReadCache, 0x200*4, &written);
		      if(written != 0x200*4) break;
		    }
		  }
	    
	    f_close(&file);

	     return 1;
    }
    break;
    default:
			return false;
  }
}
//---------------------------------------------------------------
u8 Check_saveMODE(u8 gamecode[])
{
	u32 i;
	BYTE savemode = 0xFF;
	dmaCopy((void*)saveMODE_table, (void*)pReadCache, sizeof(saveMODE_table));
	for(i=0;i<3000;i++)
	{
		if( memcmp( ((SAVE_MODE_*)pReadCache)[i].gamecode,"FFFF",4) ==0)
		{			
			break;
		}	
		else if(  memcmp( ((SAVE_MODE_*)pReadCache)[i].gamecode,gamecode,4) ==0 )
		{
			savemode = ((SAVE_MODE_*)pReadCache)[i].savemode;
			break;
		}
	}
	return savemode;
}
//---------------------------------------------------------------
u32 IWRAM_CODE Loadfile2PSRAM(TCHAR *filename)
{
	UINT  ret;
	u32 filesize;
	u32 res;
	u32 blocknum;
	char msg[20];
	
	u32 Address;
	vu16 page=0;
	SetPSRampage(page);
	
	res = f_open(&gfile, filename, FA_READ);
	if(res == FR_OK)
	{
		filesize = f_size(&gfile);		
		Clear(60,160-15,120,15,gl_color_cheat_black,1);	
		DrawHZText12(gl_writing,0,78,160-15,gl_color_text,1);	

		f_lseek(&gfile, 0x0000);
		for(blocknum=0x0000;blocknum<filesize;blocknum+=0x20000)
		{		
			sprintf(msg,"%luMb",(blocknum)/0x20000);
			Clear(78+54,160-15,110,15,gl_color_cheat_black,1);
			DrawHZText12(msg,0,78+54,160-15,gl_color_text,1);
			f_read(&gfile, pReadCache, 0x20000, &ret);//pReadCache max 0x20000 Byte
			
			if((gl_reset_on==1) || (gl_rts_on==1) || (gl_sleep_on==1) || (gl_cheat_on==1))		    
			{
				PatchInternal((u32*)pReadCache,0x20000,blocknum);
			}
						
			Address=blocknum;
			while(Address>=0x800000)
			{
				Address-=0x800000;
				page+=0x1000;
			}

			SetPSRampage(page);
			dmaCopy((void*)pReadCache,PSRAMBase_S98+Address, 0x20000);
			page = 0;
		}
		f_close(&gfile);
		SetPSRampage(0);
		return 0;
	}
	else
	{
		return 1;
	}	
	
}
//---------------------------------------------------------------------------------
void CheckLanguage(void)
{
	//read setting
	gl_select_lang =  Read_SET_info(0);
	if( (gl_select_lang != 0xE1E1) && (gl_select_lang != 0xE2E2))
	{
		gl_select_lang = 0xE1E1;
	}
	
	if(gl_select_lang == 0xE1E1)//english
	{
		LoadEnglish();
	}
	else//ÖÐÎÄ
	{
		LoadChinese();
	}
}
//---------------------------------------------------------------------------------
void CheckSwitch(void)
{
	gl_reset_on = Read_SET_info(1);
	gl_rts_on = Read_SET_info(2);
	gl_sleep_on = Read_SET_info(3);
	gl_cheat_on = Read_SET_info(4);
	if( (gl_reset_on != 0x0) && (gl_reset_on != 0x1))
	{
		gl_reset_on = 0x0;
	}
	if( (gl_rts_on != 0x0) && (gl_rts_on != 0x1))
	{
		gl_rts_on = 0x0;
	}
	if( (gl_sleep_on != 0x0) && (gl_sleep_on != 0x1))
	{
		gl_sleep_on = 0x0;
	}
	if( (gl_cheat_on != 0x0) && (gl_cheat_on != 0x1))
	{
		gl_cheat_on = 0x0;
	}	
	
	gl_engine_sel = Read_SET_info(11);
	if( (gl_engine_sel != 0x0) && (gl_engine_sel != 0x1))
	{
		gl_engine_sel = 0x1;
	}
	
	gl_show_Thumbnail = Read_SET_info(12);
	if( (gl_show_Thumbnail != 0x0) && (gl_show_Thumbnail != 0x1))
	{
		gl_show_Thumbnail = 0x0;
	}
	
	gl_ingame_RTC_open_status = Read_SET_info(13);
	if( (gl_ingame_RTC_open_status != 0x0) && (gl_ingame_RTC_open_status != 0x1))
	{
		gl_ingame_RTC_open_status = 0x1;
	}
}
//---------------------------------------------------------------------------------
void ShowTime(u32 page_num ,u32 page_mode)
{
	u8 datetime[3];
	char msgtime[50];
	//get time
	rtc_enable();
	rtc_gettime(datetime);
	rtc_disenable();
	delay(5);
	
	if(page_mode==0x1)
		ClearWithBG((u16*)gImage_RECENTLY,80, 3, 80, 13, 1);	
	else if(page_num==SD_list)
		ClearWithBG((u16*)gImage_SD,100, 3, 50, 13, 1);
	else if (page_num==NOR_list)
		ClearWithBG((u16*)gImage_NOR,100, 3, 50, 13, 1);
	 
	u8 HH = UNBCD(datetime[0]&0x3F);
	u8 MM = UNBCD(datetime[1]&0x7F);
	u8 SS = UNBCD(datetime[2]&0x7F);
	if(HH >23)HH=0;
	if(MM >59)MM=0;
	if(SS >59)SS=0;
	
	sprintf(msgtime,"%02u:%02u:%02u",HH,MM,SS);
	DrawHZText12(msgtime,0,100,3,gl_color_text,1);
}
//---------------------------------------------------------------
u32 IWRAM_CODE LoadEMU2PSRAM(TCHAR *filename,u32 is_EMU)
{
	UINT  ret;
	u32 filesize;
	u32 res;
	u32 blocknum;
	char msg[20];
	
	u32 Address;
	vu16 page=0;
	SetPSRampage(page);
	
	u32 rom_start_address=0;
	switch(is_EMU)
	{
		case 1://gbc
		case 2://gb	
			dmaCopy((void*)goomba_gba,pReadCache, goomba_gba_size);
			dmaCopy((void*)pReadCache,PSRAMBase_S98, goomba_gba_size);
			rom_start_address = goomba_gba_size;
			break;
		case 3://nes
			dmaCopy((void*)pocketnes_gba,pReadCache, pocketnes_gba_size);
			dmaCopy((void*)pReadCache,PSRAMBase_S98, pocketnes_gba_size);
			rom_start_address = pocketnes_gba_size+0x30;
			break;
		default:
			break;	
	}
	
	res = f_open(&gfile, filename, FA_READ);
	if(res == FR_OK)
	{
		filesize = f_size(&gfile);	
		
		if(is_EMU==3){//nes pocketnes_2013_07_01
			*(vu32*)pReadCache = 0x45564153;
			*((vu32*)pReadCache+1) = 0x0;
			dmaCopy((void*)pReadCache,PSRAMBase_S98 + rom_start_address -0x30, 0x8);				
			*(vu32*)pReadCache = filesize;
			dmaCopy((void*)pReadCache,PSRAMBase_S98 + rom_start_address -0x10, 0x4);
			
			*(vu32*)pReadCache = 0x709346c0;//usr rtc
			dmaCopy((void*)pReadCache,PSRAMBase_S98 + 0x1EA0, 0x4);	
			
		}
		else{
			*(vu32*)pReadCache = 0x46c046c0;
			dmaCopy((void*)pReadCache,PSRAMBase_S98 + 0x3AA0, 0x4);	//exit no sram write
			dmaCopy((void*)pReadCache,PSRAMBase_S98 + 0x39F8, 0x4);	//L R no write
			*(vu32*)pReadCache = 0x1C2246c0;//usr rtc
			dmaCopy((void*)pReadCache,PSRAMBase_S98 + 0x830, 0x4);				
		}
			
		Clear(60,160-15,120,15,gl_color_cheat_black,1);	
		DrawHZText12(gl_writing,0,78,160-15,gl_color_text,1);	

		f_lseek(&gfile, 0x0000);
		for(blocknum=0x0000;blocknum<filesize;blocknum+=0x20000)
		{		
			sprintf(msg,"%luMb",(blocknum)/0x20000);
			Clear(78+54,160-15,110,15,gl_color_cheat_black,1);
			DrawHZText12(msg,0,78+54,160-15,gl_color_text,1);
			//f_lseek(&gfile, blocknum);
			f_read(&gfile, pReadCache, 0x20000, &ret);//pReadCache max 0x20000 Byte
						
			Address=blocknum;
			while(Address>=0x800000)
			{
				Address-=0x800000;
				page+=0x1000;
			}
			SetPSRampage(page);
			dmaCopy((void*)pReadCache,PSRAMBase_S98 + rom_start_address + Address, 0x20000);
			
			page = 0;
		}
		f_close(&gfile);
		SetPSRampage(0);
		return 0;
	}
	else
	{
		return 1;
	}	
	
	return 0;
}
//---------------------------------------------------------------------------------
extern u16 SET_info_buffer [0x200]EWRAM_BSS;
void save_set_info_SELECT(void)
{
	u32 address;
	for(address=0;address < 12;address++)
	{
		SET_info_buffer[address] = Read_SET_info(address);
	}
	SET_info_buffer[12] = gl_show_Thumbnail;
	//save to nor 
	Save_SET_info(SET_info_buffer,0x200);
}
//---------------------------------------------------------------------------------
//Sort folder
void Sort_folder(folder_total)
{
	u32 ret;
	u32 i;
	int get;
	if(folder_total>1)
	{
		for(ret=0;ret<folder_total-1;ret++)
		{
			for(i=0;i<folder_total-ret-1;i++)
			{
				get = strcmp(pFolder[i].filename,pFolder[i+1].filename) ;
				if(get>0)
				{
					dmaCopy(&pFolder[i+1],&(pFilename_temp.filename),sizeof(FM_Folder_FS));
					dmaCopy(&pFolder[i],&pFolder[i+1],sizeof(FM_Folder_FS));
					dmaCopy(&(pFilename_temp.filename),&pFolder[i],sizeof(FM_Folder_FS));					
				}
			}
		}
	}
}
//---------------------------------------------------------------------------------
//Sort file 
void Sort_file(game_total_SD)
{
	u32 ret;
	u32 i;
	int get;
	if(game_total_SD>1)
	{
		for(ret=0;ret<game_total_SD-1;ret++)
		{
			for(i=0;i<game_total_SD-ret-1;i++)
			{
				get = strcmp(pFilename_buffer[i].filename,pFilename_buffer[i+1].filename) ;
				if(get>0)
				{
					dmaCopy(&pFilename_buffer[i+1],&pFilename_temp,sizeof(FM_FILE_FS));
					dmaCopy(&pFilename_buffer[i],&pFilename_buffer[i+1],sizeof(FM_FILE_FS));
					dmaCopy(&pFilename_temp,&pFilename_buffer[i],sizeof(FM_FILE_FS));					
				}
			}
		}
	}	
}	
//---------------------------------------------------------------------------------
u32 Load_Thumbnail(TCHAR *pfilename_pic)
{
  u32 rett;
  u32 res;
  TCHAR picpath[30];
	
	res = f_open(&gfile, pfilename_pic, FA_READ);
	if(res == FR_OK)
	{
		f_lseek(&gfile, 0xAC);
		f_read(&gfile, GAMECODE, 4, (UINT *)&rett);
		f_close(&gfile);
					
		memset(picpath,00,30);
		sprintf(picpath,"/IMGS/%c/%c/%c%c%c%c.bmp",GAMECODE[0],GAMECODE[1],GAMECODE[0],GAMECODE[1],GAMECODE[2],GAMECODE[3]);						
		res = f_open(&gfile,picpath, FA_READ);
		if(res == FR_OK)
		{
			f_read(&gfile, pReadCache+0x10000, 0x4B38, &rett);			
			f_close(&gfile);	
			return 1;												
		}		
				
	}			
	return 0;	
}
//---------------------------------------------------------------------------------
//Delete file
void SD_list_L_START(show_offset,file_select,folder_total)
{
	u32 res;
	
	DrawPic((u16*)gImage_MENU, 56, 25, 128, 110, 0, 0, 1);//show menu pic		
	Show_MENU_btn();

	DrawHZText12(gl_LSTART_help,0,60,60,gl_color_text,1);//use sure?gl_LSTART_help
	DrawHZText12(pFilename_buffer[show_offset+file_select-folder_total].filename,20,60,75,0x7fff,1);//file name
	DrawHZText12(gl_formatnor_info,5,60,90,gl_color_text,1);//use sure?
	while(1){
		VBlankIntrWait();
		scanKeys();
		u16 keysdown  = keysDown();
		if (keysdown & KEY_A) {
			TCHAR *pdelfilename;			
			pdelfilename = pFilename_buffer[show_offset+file_select-folder_total].filename;	
			res = f_unlink(pdelfilename);	
			break;
		}
		else if(keysdown & KEY_B){
			break;
		}
	}	
}
//---------------------------------------------------------------------------------
u32 Check_file_type(TCHAR *pfilename)
{
	u32 strlen8 = strlen(pfilename) ;
	//u32 is_EMU;
	if(!strcasecmp(&(pfilename[strlen8-3]), "gba"))
	{
		return 0;
	}	
	else if(!strcasecmp(&(pfilename[strlen8-3]), "gbc"))
	{
    return 1;
	}
	else if(!strcasecmp(&(pfilename[strlen8-2]), "gb"))
	{
    return 2;
	}
	else if(!strcasecmp(&(pfilename[strlen8-3]), "nes"))
	{
    return 3;
	}
	else 
	{
		return 0xff;
	}	
}
//---------------------------------------------------------------------------------
void Show_error_num(u8 error_num)
{
	char msg[50];

	ClearWithBG((u16*)gImage_SD,90, 2, 90, 13, 1);
	switch(error_num)
	{
		case 0x0:
			sprintf(msg,"%s",gl_error_0);
			break;
		case 0x1:
			sprintf(msg,"%s",gl_error_1);
			break;
		case 0x2:
			sprintf(msg,"%s",gl_error_2);
			break;
		case 0x3:
			sprintf(msg,"%s",gl_error_3);
			break;
		case 0x4:
			sprintf(msg,"%s",gl_error_4);
			break;
		case 0x5:
			sprintf(msg,"%s",gl_error_5);
			break;
		case 0x6:
			sprintf(msg,"%s",gl_error_6);
			break;
		default:
			sprintf(msg,"%s","error?");
			break;				
	}

	DrawHZText12(msg,0,90,2, RGB(31,00,00),1);
	wait_btn();
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void) {

	irqInit();
	irqEnable(IRQ_VBLANK);

	REG_IME = 1;

	u32 res;
	u32 game_folder_total;
	u32 file_select;
	u32 show_offset;
	u32 updata;
	u32 continue_MENU;
	PAGE_NUM page_num=SD_list;
	u32 page_mode;
	u32 shift;
	u32 short_filename=0;
	
	u8 error_num;

	gl_currentpage = 0x8002 ;//kernel mode

	SetMode (MODE_3 | BG2_ENABLE );
	
	SD_Disable();	
	Set_RTC_status(1);
		
	//check FW
	u16 Built_in_ver = 7;   //Newest_FW_ver
	u16 Current_FW_ver = Read_FPGA_ver();

	if((Current_FW_ver < Built_in_ver) || (Current_FW_ver == 99))//99 is test ver
	{
		Check_FW_update(Current_FW_ver,Built_in_ver);
	}
	
	DrawPic((u16*)gImage_splash, 0, 0, 240, 160, 0, 0, 1);	
	CheckLanguage();	
	CheckSwitch();

	res = f_mount(&EZcardFs, "", 1);
	if( res != FR_OK)
	{
		DrawHZText12(gl_init_error,0,2,20, gl_color_text,1);
		DrawHZText12(gl_power_off,0,2,33, gl_color_text,1);
		while(1);
	}
	else
	{
		DrawHZText12(gl_init_ok,0,2,20, gl_color_text,1);
		DrawHZText12(gl_Loading,0,2,33, gl_color_text,1);
	}
	VBlankIntrWait();	

	f_chdir("/");
	TCHAR currentpath[MAX_path_len];
	memset(currentpath,00,MAX_path_len);
	memset(currentpath_temp,0x00,MAX_path_len);
	folder_select = 1;
	memset(p_folder_select_show_offset,0x00,100);
	memset(p_folder_select_file_select,0x00,100);
	
	res = f_getcwd(currentpath, sizeof currentpath / sizeof *currentpath);

	Read_NOR_info();	
	gl_norOffset = 0x000000;
	game_total_NOR = GetFileListFromNor();//initialize to prevent direct writes to NOR without page turning
	if(game_total_NOR==0)
	{
		memset(pNorFS,00,sizeof(FM_NOR_FS)*MAX_NOR);
		Save_NOR_info(pNorFS,sizeof(FM_NOR_FS)*MAX_NOR);
	}

refind_file:
	

	if(page_num== SD_list)
	{
		folder_total = 0;
		game_total_SD = 0; 

		res = f_opendir(&dir,currentpath);
		if (res == FR_OK)
		{
			while(1)
			{
				res = f_readdir(&dir, &fileinfo);                   //read next
				//DEBUG_printf("=%x %s %x %x",res, fileinfo.fname,fileinfo.fname[0],fileinfo.fattrib);
				//wait_btn();								
				if (res != FR_OK || fileinfo.fname[0] == 0) break;

				if(	(fileinfo.fattrib == AM_DIR) || (fileinfo.fattrib == 0x30))//DIR and exFAT dir
				{
					memcpy(pFolder[folder_total].filename,fileinfo.fname,100);
					pFolder[folder_total++].filename[99] = 0;
					if ( folder_total > MAX_folder )//cut
	      		break;
				}
				else if(	fileinfo.fattrib == AM_ARC)
				{
					memcpy(pFilename_buffer[game_total_SD].filename,fileinfo.fname,100);
					pFilename_buffer[game_total_SD].filename[99] = 0;
					pFilename_buffer[game_total_SD++].filesize = fileinfo.fsize;
	    		if ( game_total_SD > MAX_files )//cut
	      		break;
				}
			}
		}
		f_closedir(&dir);
		
		game_folder_total = folder_total + game_total_SD;
		
		Sort_folder(folder_total);//folder
		Sort_file(game_total_SD);//file
  }
  else
  {
  	Read_NOR_info();
 		gl_norOffset = 0x000000;
		game_total_NOR = GetFileListFromNor();
  }
  
  if(folder_select){
		file_select = p_folder_select_file_select[folder_select];
		show_offset = p_folder_select_show_offset[folder_select];
  }
  else{	
		file_select = 0;
		show_offset = 0;
	}
	continue_MENU = 0;
	
	u32 haveThumbnail;
	u32 is_GBA_old=0;
	u32 is_GBA;
	
	u32 play_re;
	play_re = 0xBB;
//NOR_list:
//SD_list:
re_showfile:
	
	shift =0;
	page_mode=0;
  updata=1;
  u32 key_L=0;
	setRepeat(5,1);
	
	if(page_num==SD_list)
	{
		DrawPic((u16*)gImage_SD, 0, 0, 240, 160, 0, 0, 1);	
	}
	while(1)
	{
		while(1)//2
		{
			VBlankIntrWait();
			VBlankIntrWait();			
			if((shift==0) || (gl_show_Thumbnail==0)){
				short_filename = 0;				
			}
			if(shift==0){
				dwName =0;
			}			
			shift++;
			
			haveThumbnail = 0;
			is_GBA = 0;
			
			if(updata && gl_show_Thumbnail)
			{
		  	u32 rett;
		  	TCHAR picpath[30];
	
				TCHAR *pfilename_pic;
				
				if(page_num==SD_list){
					pfilename_pic = pFilename_buffer[show_offset+file_select-folder_total].filename;
				}
				else{
					pfilename_pic = pNorFS[show_offset+file_select].filename;
				}

				u32 strlengba = strlen(pfilename_pic) ;
				if(!strcasecmp(&(pfilename_pic[strlengba-3]), "gba"))
				{
					is_GBA = 1;		
					haveThumbnail = Load_Thumbnail(pfilename_pic);	
					short_filename = 1;			
				}
				else{
					if((is_GBA_old==1) && (is_GBA==0)){
						updata = 1;
					}
				}	
				
				is_GBA_old = is_GBA;
			}
	    if(updata==1){//reshow all
	    	if(page_num==SD_list)
	    	{
	    		//DrawPic((u16*)gImage_SD, 0, 0, 240, 160, 0, 0, 1);	
	    		ClearWithBG((u16*)gImage_SD,0, 0, 90, 20, 1);  //  		
	    		ClearWithBG((u16*)gImage_SD,185+6, 3, 6*3, 16, 1);//Show_game_num
	    		ClearWithBG((u16*)gImage_SD,0, 20, 240, 160-20, 1);
	    		Show_ICON_filename(show_offset,file_select,gl_show_Thumbnail&&is_GBA);
	    		    		
	    	}
	    	else if(page_num==SET_win)//set windows
	    	{
					DrawPic((u16*)gImage_SET, 0, 0, 240, 160, 0, 0, 1);
					res =Setting_window();
					if(res==0){
						DrawPic((u16*)gImage_NOR, 0, 0, 240, 160, 0, 0, 1);
						page_num = NOR_list;//NOR
					}
					else{
						DrawPic((u16*)gImage_HELP, 0, 0, 240, 160, 0, 0, 1);
						page_num = HELP;//HELP
					}
					goto re_showfile;
	    	}
	    	else if(page_num==HELP)//HELP windows
	    	{
					DrawPic((u16*)gImage_HELP, 0, 0, 240, 160, 0, 0, 1);
					Show_help_window();
					DrawPic((u16*)gImage_SET, 0, 0, 240, 160, 0, 0, 1);
					page_num = SET_win;//	
					goto re_showfile;
	    	}
	    	else
	    	{    		
	      	DrawPic((u16*)gImage_NOR, 0, 0, 240, 160, 0, 0, 1);
	    		//ClearWithBG((u16*)gImage_NOR,0, 0, 90, 20, 1);  //  		
	    		//ClearWithBG((u16*)gImage_NOR,185+6, 3, 6*7, 16, 1);
	    		//ClearWithBG((u16*)gImage_NOR,0, 20, 240, 160-20, 1);
					Show_ICON_filename_NOR(show_offset,file_select);			    		
	    	}
	    	Show_game_num(file_select+show_offset+1,page_num);
	  	}
	  	else if(updata >1){
	    	if(page_num==NOR_list)
	    	{
					Refresh_filename_NOR(show_offset,file_select,updata);
					ClearWithBG(gImage_NOR,185, 0, 30, 18, 1);
	    	}
	    	else
	    	{
	    		Refresh_filename(show_offset,file_select,updata,gl_show_Thumbnail&&is_GBA);
	    		ClearWithBG((u16*)gImage_SD,185, 0, 30, 18, 1);
	    	}
	    	Show_game_num(file_select+show_offset+1,page_num );
	  	}
	  	
	  	if( updata && gl_show_Thumbnail && is_GBA && (page_num==SD_list) )
			{
	  		if(haveThumbnail){				
	  			DrawPic((u16*)(pReadCache+0x10036), 120, 80, 120, 80, 0, 0, 1);//show game pic				
				}
				else{
					DrawPic((u16*)(gImage_NOTFOUND), 120, 80, 120, 80, 0, 0, 1);//show game pic	
				}
			}
			if(continue_MENU) break;
			if(page_num==SD_list){
				if(game_folder_total)
					Filename_loop(shift,show_offset,file_select,short_filename);
			}
				
	    updata=0;
			scanKeys();
			u16 keysdown  = keysDown();
			u16 keys_released = keysUp();
			u16 keysrepeat = keysDownRepeat();

			u32 list_game_total;
			if(page_num==NOR_list)
			{
				list_game_total = game_total_NOR;
			}
			else
			{
				list_game_total = game_folder_total;
			}

			if (keysrepeat  & KEY_DOWN) {
				if (file_select + show_offset+1 < (list_game_total )) {
	        if ( file_select > 8 ){
	          if ( file_select == 9 ) {
	            show_offset++;
	            updata=1;
	          }
	        }else{
	          file_select++;
	          updata=2;
	        }
					shift = 0;
				}
			}
			else if(keysrepeat & KEY_UP)
			{
				if (file_select ) {
					file_select--;
					updata=3;
				}else{
					if (show_offset){
						show_offset--;
						updata=1;
					}
				}
				shift = 0;
			}
			else if(keysrepeat & KEY_LEFT)
			{
		    if ( show_offset )
		    {
		      if ( show_offset > 9 )
		        show_offset -= 10;
		      else
		        show_offset = 0;

		      updata=1;
		    }
		    else{
		    	if(file_select){
		    		file_select=0;
		    		updata=1;
		   	 	}
		    }
		    shift = 0;
			}
			else if(keysrepeat & KEY_RIGHT)
			{
	      if ( show_offset + 10 < list_game_total )
	      {
	        if ( show_offset + 20 <= list_game_total )
	          show_offset += 10;
	        else
	          show_offset = list_game_total - 10;

					updata=1;
	      }
	      shift = 0;
			}
			else if(keysdown & KEY_L)
			{
				key_L = 1;
				if(page_num)
				{
	      	file_select = 0;
	      	show_offset = 0;
	      	updata=1;
	      	DrawPic((u16*)gImage_SD, 0, 0, 240, 160, 0, 0, 1);
	      	folder_select=1;
	    	}
				page_num = SD_list;	
				shift = 0;			
			}
			else if(keys_released & KEY_L)
			{
				key_L = 0;
			}
			else if(keysdown & KEY_R)
			{			
	      if(page_num==HELP)continue;
				page_num ++;
				if(page_num==NOR_list)DrawPic((u16*)gImage_NOR, 0, 0, 240, 160, 0, 0, 1);
				updata=1;
				folder_select=0;
				shift = 0;
				goto refind_file;
			}   
			else if(keysdown & KEY_B)//return
			{
				if(page_num == SD_list)
				{
	   			//res = f_getcwd(currentpath, sizeof currentpath / sizeof *currentpath);
	   			if(strcmp(currentpath,"/") !=0 ){		
		    		dmaCopy(currentpath, currentpath_temp, MAX_path_len);
		    		TCHAR *p=strrchr(currentpath_temp, '/');
		    		memset(currentpath,0x00,MAX_path_len);
		    		strncpy(currentpath, currentpath_temp, p-currentpath_temp);
		    		if(currentpath[0]==0) currentpath[0]='/';
		    		
						res=f_chdir(currentpath);
						if(res != FR_OK){
							error_num = 10;
							Show_error_num(error_num);
							goto re_showfile;
						}						
						
						p_folder_select_show_offset[folder_select] = 0;//clean
						p_folder_select_file_select[folder_select] = 0;//clean
						if(folder_select){
							folder_select--;
						}												
				    goto refind_file;
			    }
		  	}
			}
			else if(keysdown & KEY_SELECT)
			{
				gl_show_Thumbnail = !gl_show_Thumbnail;
				save_set_info_SELECT();
				updata=1;
			}	
			else if(keysdown & KEY_A)
			{
				if(page_num==SD_list){
					//res = f_getcwd(currentpath, sizeof currentpath / sizeof *currentpath);		
		      if( show_offset+file_select <  folder_total)
		      {	   				
	   				if(strcmp(currentpath,"/") !=0){	
	   					sprintf(currentpath,"%s%s",currentpath,"/");
						}
		      	sprintf(currentpath,"%s%s",currentpath,pFolder[show_offset+file_select].filename);
		      	
						res=f_chdir(currentpath);
						if(res != FR_OK){
							error_num = 0;
							Show_error_num(error_num);
							goto re_showfile;
						}	
											
						p_folder_select_show_offset[folder_select] = show_offset;
						p_folder_select_file_select[folder_select] = file_select;
						folder_select++;

			      goto refind_file;
			    }
		      else{   //SD_list file
						break;
					}
				}
				else{   //NOR gba file
					if(game_total_NOR){
						break;
					}
				} 
					
			}		
			else if(keysdown & (KEY_START)  ) 
			{
				if(page_num==SD_list){//only work on sd list								
					if(key_L)
					{
						if(show_offset+file_select >= folder_total){
							SD_list_L_START(show_offset,file_select,folder_total);
							goto refind_file;	
						}				
					}
					else{//only START //Recently played							
						play_re=show_recently_play();
						if(play_re==0xBB){
							goto refind_file;//KEY B
						}
						else{
							page_mode = 0x1;
							break;
						}
					}
				}
			}
				
			ShowTime(page_num,page_mode);

		}	//2

		continue_MENU = 0;
		//press A, show boot MENU;
		TCHAR *pfilename;
		
		if(play_re==0xBB){
			if(page_num==SD_list){
				pfilename = pFilename_buffer[show_offset+file_select-folder_total].filename;
			}
			else{
				pfilename = pNorFS[show_offset+file_select].filename;
			}
		}
		else{		
			u8 *p=strrchr(p_recently_play[play_re], '/');
			strncpy(currentpath_temp, currentpath, 256);//old
			memset(currentpath,00,256);
			strncpy(currentpath, p_recently_play[play_re], p-p_recently_play[play_re]);
			if(currentpath[0]==0){
				currentpath[0] = '/';
			}			
			memset(current_filename,00,200);
			strncpy(current_filename, p+1, 100);//remove directory path	
			pfilename = current_filename;						
		}		

		u8 Save_num=0;//save tpye: auto
		u8 old_Save_num=0;		
		u32 havecht;	
		u32 MENU_line=0   ;
		u32 re_menu=1;
		u32 MENU_max;		
		u32 is_EMU=Check_file_type(pfilename);
		if(is_EMU == 0xff) 
		{
			goto re_showfile;
		}							
		else if(is_EMU)
		{
			havecht = 0;
			Save_num = 0xF;
			MENU_max = 0;
		}
		else{
			res=f_chdir(currentpath);//can open  re list game
			havecht = Check_cheat_file(pfilename);	
			old_Save_num = Check_mde_file(pfilename);	
			Save_num	= old_Save_num;
			MENU_max=(page_num==NOR_list)?2:(4+ ((gl_cheat_on==1)? ((havecht>0)?1:0):0) );
		}
		
		re_show_menu:
		DrawPic((u16*)gImage_MENU, 56, 25, 128, 110, 0, 0, 1);//show menu pic		
		Show_MENU_btn();			
		while(1)//3
		{
			if(re_menu)
			{				
				Show_MENU(MENU_line,page_num, ((havecht>0)?1:0),Save_num,is_EMU);								
			}
			VBlankIntrWait();
			
	    re_menu=0;
			scanKeys();
			u16 keysdown  = keysDown();
			u16 keysup  = keysUp();
			u16 keys_released = keysUp();

			if (keysdown & KEY_DOWN) {
				if (MENU_line < MENU_max) {
	       	MENU_line++;
	        re_menu=1;
				}
				else if(MENU_line == MENU_max){
	        MENU_line=0;
	        re_menu=1;	
				}
			}
			else if(keysdown & KEY_UP)
			{
				if (MENU_line ) {
					MENU_line--;
					re_menu=1;
				}
				else if(MENU_line == 0){
					MENU_line=MENU_max;
					re_menu=1;
				}
			}
			else if(keysup & KEY_B)
			{
				gl_cheat_count = 0;
				if(play_re!=0xBB){
					strncpy(currentpath, currentpath_temp, 256);//
				}
				f_chdir(currentpath);//return to old folder
				goto re_showfile;
			}
			else if(keysdown & KEY_LEFT)
			{
				if(MENU_line==4){//save type
					if(Save_num){
						Save_num--;
						re_menu=1;
						DrawPic((u16*)gImage_MENU, 56, 25, 128, 110, 0, 0, 1);//show menu pic
						Show_MENU_btn();
					}
				}
			}
			else if(keysdown & KEY_RIGHT)
			{
				if(MENU_line==4){//save type
					if(Save_num<5){
						Save_num++;	
						re_menu=1;
						DrawPic((u16*)gImage_MENU, 56, 25, 128, 110, 0, 0, 1);//show menu pic
						Show_MENU_btn();
					}
				}
			}	
			else if(keysdown & KEY_L)
			{
				key_L = 1;		
			}
			else if(keys_released & KEY_L)
			{
				key_L = 0;
			}
			else if(keysdown & KEY_A)
			{
				if(page_num==NOR_list){
	      	if(MENU_line==0){//boot to NOR.page
						break;
					}
					else if(MENU_line==1){
						//delete lastest geme
						if(show_offset+file_select+1 == game_total_NOR){
							Block_Erase(gl_norOffset-pNorFS[show_offset+file_select].filesize);
						}
						else{
							DrawHZText12(gl_lastest_game,0,66,118-15,gl_color_text,1);	
							wait_btn();
						}
						page_num = NOR_list;
						goto refind_file;
					}
					else{ //MENU_line==2
						//format all
						FormatNor();
						page_num = NOR_list;
						goto refind_file;				
					}
				}
				else{//page_num==SD_list
					if(MENU_line==5){
						//open cht file
						Open_cht_file(pfilename,havecht);	
						re_menu=1;
						MENU_line=1;
						goto re_show_menu;	
					}
					else if(MENU_line==4){
						// do nothing
					}
					else{ //boot
						break;
					}
				}
			}
			ShowTime(page_num,page_mode);
		}	//3

		Clear(0, 0, 240, 160, gl_color_cheat_black, 1);
		DrawHZText12(gl_Loading,0,(240-strlen(gl_Loading)*6)/2,74, gl_color_text,1);

		u32 gamefilesize=0;
		u32 savefilesize=0;		
		u32 ret;

		TCHAR savfilename[100];		
		BYTE saveMODE;
		u32 have_pat=0;
		
		init_FAT_table();		
		if(page_num==SD_list){	//Load to PSRAM or NOR 

			f_chdir(currentpath);//return to game folder	
			res = f_open(&gfile, pfilename, FA_READ);
			if(res == FR_OK)
			{
				f_lseek(&gfile, 0xAC);
				f_read(&gfile, GAMECODE, 4, (UINT *)&ret);
				
				gamefilesize = f_size(&gfile);
				f_close(&gfile);
			}
			else
			{
				memset(GAMECODE,'F',4);
			}	
			
			if(gamefilesize > 0x2000000){
				ShowbootProgress(gl_file_overflow);	
				wait_btn();
				goto refind_file;	
			}
			
			if(MENU_line<2) //PSRAM DirectPSRAM or soft reset   or run NOR game 
			{
				res = Check_game_save_FAT(pfilename,1);//game FAT
				if(res == 0xffffffff){
					error_num = 1;
					Show_error_num(error_num);
					goto re_showfile;
				}
			}								
		}
		else //run nor game 
		{
			pfilename = pNorFS[show_offset+file_select].filename;
			gamefilesize = pNorFS[show_offset+file_select].filesize;
			memcpy(GAMECODE,&pNorFS[show_offset+file_select].gamename[0xC],4);
		}
		
		ShowbootProgress(gl_check_sav);				
		memcpy(savfilename,pfilename,100);
		u32 strlen8 = strlen(savfilename) ;
		if(is_EMU){
			if(is_EMU ==2){//gb
				(savfilename)[strlen8-2] = 'e';
				(savfilename)[strlen8-1] = 's';
				(savfilename)[strlen8-0] = 'v';		
				(savfilename)[strlen8+1] = 0;	
			}		
			else{
				(savfilename)[strlen8-3] = 'e';
				(savfilename)[strlen8-2] = 's';
				(savfilename)[strlen8-1] = 'v';
			}	
		}
		else{//gba		
			(savfilename)[strlen8-3] = 's';
			(savfilename)[strlen8-2] = 'a';
			(savfilename)[strlen8-1] = 'v';
		}
		#ifdef DEBUG
			//DEBUG_printf("sav %s",savfilename);			
		#endif
		if(is_EMU ==0){//gba
			if(old_Save_num != Save_num){
				Make_mde_file(pfilename,Save_num);
			}
		}	
				
		res = f_mkdir("/SAVER");
		res=f_chdir("/SAVER");
		if(res != FR_OK){
			error_num = 2;
			Show_error_num(error_num);
			goto re_showfile;;
		}


		if(page_num==SD_list){	
			if(MENU_line<2){//PSRAM DirectPSRAM or soft reset
				Make_recently_play_file(currentpath,pfilename);	
			}
		}	

		if(Save_num==0)//auto
		{
			saveMODE = Check_saveMODE(GAMECODE);
		}
		else 
		{
			switch(Save_num)
			{
				case 0x1:saveMODE=0x11;break;//SRAM
				case 0x2:
					if(gamefilesize> 0x1200000){//some eeprom modify rom 
						saveMODE=0x23;//32M //EEPROM8K
					}
					else{ 
						saveMODE=0x22;//EEPROM8K
					}
					break;
				case 0x3:saveMODE=0x21;break;//EEPROM512
				case 0x4:saveMODE=0x32;break;//FLASH64
				case 0x5:saveMODE=0x31;break;//FLASH128
				case 0xf:saveMODE=0xee;break;	
				default:saveMODE=0x00;break;					
			}
		}

		switch(saveMODE)
		{
			case 0x00:savefilesize=0x0;break;//no save
			case 0x11:savefilesize=0x8000;break;//SRAM_TYPE 32k
			case 0x21:savefilesize=0x200;break;//EEPROM_TYPE 512
			case 0x22:savefilesize=0x2000;break;//EEPROM_TYPE 8k	
			case 0x23:savefilesize=0x2000;break;//EEPROM_TYPE v125 v126 must use 8k
			case 0x32:savefilesize=0x10000;break;//FLASH_TYPE 64k
			case 0x33:savefilesize=0x10000;break;//FLASH512_TYPE 64k	
			case 0x31:savefilesize=0x20000;break;//FLASH1M_TYPE 128k
			case 0xee:savefilesize=0x10000;break;//EMU 64k	
			default:	savefilesize=0x10000;break;//UNKNOW,FF  for homebrew SRAM_TYPE	//2018-4-23 some emu homebrew need 64kByte	
		}		
		
		res = f_open(&gfile,savfilename, FA_OPEN_EXISTING);
		
		if(res == FR_OK)//have a old save file
		{
			savefilesize = f_size(&gfile);		
			f_close(&gfile);				
		}					
		else //make a new one
		{	
			//new_save:
			ShowbootProgress(gl_make_sav);
			res = SavefileWrite(savfilename, savefilesize);
			if(res == 0)
			{
				error_num = 5;
				Show_error_num(error_num);
				goto re_showfile;
			}
		}
		#ifdef DEBUG
			//DEBUG_printf("saveMODE %x",saveMODE);
			//DEBUG_printf("savefilesize %x",savefilesize);
		#endif	
		if(MENU_line<2)//PSRAM DirectPSRAM or soft reset   or run NOR game 
		{
			if(savefilesize)
			{
				res = Check_game_save_FAT(savfilename,2);//save FAT
				if(res == 0xffffffff)//   save file error
				{
					error_num = 4;
					Show_error_num(error_num);
					goto re_showfile;
				}
				if(FAT_table_buffer[(FAT_table_SAV_offset+4)/4] == 0)//save fat
				{
					error_num = 3;
					Show_error_num(error_num);
					goto re_showfile;
				}			
				
				Bank_Switching(0);
				res = Loadsavefile(savfilename);							
			}					

			FAT_table_buffer[0x1F0/4] = gamefilesize;//size
			FAT_table_buffer[0x1F4/4] = 0x1;  //rom copy to psram
			FAT_table_buffer[0x1F8/4] = (&EZcardFs)->csize;//0x40;  //secort of cluster
			FAT_table_buffer[0x1FC/4] = (saveMODE<<24) | savefilesize;  //save mode and save file size
			//DEBUG_printf(" %08X %08X ", FAT_table_buffer[0],FAT_table_buffer[1]);
			//DEBUG_printf(" %08X %08X ", FAT_table_buffer[2],FAT_table_buffer[3]);
			//DEBUG_printf(" %08X %08X ", FAT_table_buffer[4],FAT_table_buffer[5]);
			//DEBUG_printf(" %08X %08X ", FAT_table_buffer[0x200/4],FAT_table_buffer[0x204/4]);		
			//DEBUG_printf(" %08X %08X ", FAT_table_buffer[0x208/4],FAT_table_buffer[0x20C/4]);
			//DEBUG_printf(" %08X %08X ", FAT_table_buffer[0x1F0/4],FAT_table_buffer[0x1F4/4]);
			//DEBUG_printf(" %08X %08X ", FAT_table_buffer[0x1F8/4],FAT_table_buffer[0x1FC/4]);					
		}
		
		if(is_EMU)
		{			
			ShowbootProgress(gl_loading_game);	
			f_chdir(currentpath);//return to game folder
			
		  FAT_table_buffer[0x1F4/4] = 0x2;  	//copy mode
			Send_FATbuffer(FAT_table_buffer,1); //only save FAT
								
  		res=LoadEMU2PSRAM(pfilename,is_EMU);
  		SetRompageWithHardReset(0x200,key_L);
  		while(1);
		}
			
		if(page_num==NOR_list){//boot nor game
			
			if(pNorFS[show_offset+file_select].have_patch && pNorFS[show_offset+file_select].have_RTS)
			{	
				ShowbootProgress(gl_check_RTS);		
				u32 size = Check_RTS(pfilename);
				if(size ==0)
				{
					error_num = 6;
					Show_error_num(error_num);
					goto re_showfile;
				}
			}
					
			FAT_table_buffer[0x1F4/4] = 0x2;  //copy mode
			Send_FATbuffer(FAT_table_buffer,1); //only save FAT
			//wait_btn();
			SetRompageWithHardReset(pNorFS[show_offset+file_select].rompage,key_L);
			while(1);
		}
		else {			
	  	switch(MENU_line){
	  		case 0://DirectPSRAM CLEAN BOOT
	  			ShowbootProgress(gl_loading_game);
					Send_FATbuffer(FAT_table_buffer,0);
					GBApatch_Cleanrom(PSRAMBase_S98,gamefilesize);
					//wait_btn();
					SetRompageWithHardReset(0x200,key_L);
		  		break;
		    case 1://PSRAM BOOT WITH ADDON
		    	gl_reset_on = Read_SET_info(1);
					gl_rts_on = Read_SET_info(2);
					gl_sleep_on = Read_SET_info(3);
					gl_cheat_on = Read_SET_info(4);
					
					if(gl_rts_on==1)
					{		
						ShowbootProgress(gl_check_RTS);		
						u32 size = Check_RTS(pfilename);
						if(size ==0)
						{
							error_num = 6;
							Show_error_num(error_num);
							goto re_showfile;
						}
					}		
						
					ShowbootProgress(gl_check_pat);						
					have_pat = Check_pat(pfilename);	
								
					f_chdir(currentpath);//return to game folder	
					ShowbootProgress(gl_loading_game);
					u32 make_pat=0;
					if(have_pat==1)
					{			    		
			    		Send_FATbuffer(FAT_table_buffer,0);//Loading rom	
					}	
					else //(have_pat==0)
					{
						//get the location of the patch
						res = f_open(&gfile,pfilename, FA_READ);	
						f_lseek(&gfile, (gamefilesize-1)&0xFFFE0000);
						f_read(&gfile, pReadCache, 0x20000, &ret);
						f_close(&gfile);
						SetTrimSize(pReadCache,gamefilesize,0x20000,0x0,saveMODE);						
						
						if((gl_engine_sel==0) || (gl_select_lang == 0xE2E2))
						{				
			    		FAT_table_buffer[0x1F4/4] = 0x2;  // copy mode
							Send_FATbuffer(FAT_table_buffer,1); //only save FAT													
			    		res=Loadfile2PSRAM(pfilename);
			    		make_pat = 1;
						}
						else 
						{
			    		use_internal_engine(GAMECODE);	
			    		Send_FATbuffer(FAT_table_buffer,0);//Loading rom	
						}
					}									
		    	
		    	if((gl_reset_on==1) || (gl_rts_on==1) || (gl_sleep_on==1) || (gl_cheat_on==1))		    	
		    	{
		    		Patch_SpecialROM_sleepmode();//
		    		GBApatch_PSRAM(PSRAMBase_S98,gamefilesize);		    		
		    	}
		    	//
		    	if(make_pat==1)
		    	{
						ShowbootProgress(gl_make_pat);
						Make_pat_file(pfilename);
					}
					//wait_btn();	
		    	SetRompageWithHardReset(0x200,key_L);
		    	break;
		    case 2://WRITE TO NOR CLEAN    	
		    	f_chdir(currentpath);//return to game folder
					res = Loadfile2NOR(pfilename, gl_norOffset,0x0);
					if(res==0)
					{
						page_num = NOR_list;
						goto refind_file;
					}
					else if(res==2)
					{
						DrawHZText12("NOR FULL!",0,0,160-15, gl_color_NORFULL,1);
						wait_btn();	
						goto refind_file;
					}
		    	break;
		    case 3://WRITE TO NOR ADDON
		    	gl_reset_on = Read_SET_info(1);
					gl_rts_on = Read_SET_info(2);
					gl_sleep_on = Read_SET_info(3);
					gl_cheat_on = Read_SET_info(4);
															
					f_chdir(currentpath);//return to game folder	
					u32 needpatch = 0;
					if((gl_reset_on==1) || (gl_rts_on==1) || (gl_sleep_on==1) || (gl_cheat_on==1))		    	
		    	{
		    		Patch_SpecialROM_sleepmode();//
	      				
						//get the location of the patch
						res = f_open(&gfile,pfilename, FA_READ);	
						if(res==FR_OK){
							f_lseek(&gfile, (gamefilesize-1)&0xFFFE0000);
							f_read(&gfile, pReadCache, 0x20000, &ret);
							f_close(&gfile);
							SetTrimSize(pReadCache,gamefilesize,0x20000,0x1,saveMODE);
						}
						needpatch = 1;
					}
					res = Loadfile2NOR(pfilename, gl_norOffset,needpatch);
					//wait_btn();	
					if(res==0)
					{
						page_num = NOR_list;
						goto refind_file;
					}	
					else if(res==2)
					{
				    //ClearWithBG((u16*)gImage_SD,0, 160-15, 60, 13, 1);
						DrawHZText12("NOR FULL!",0,0,160-15, gl_color_NORFULL,1);
						wait_btn();	
						goto refind_file;
					}		
		    	break;
		    default:
		    	break;
		  }
		}
	}
}
//---------------------------------------------------------------