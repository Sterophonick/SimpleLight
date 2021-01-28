#include <gba_systemcalls.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gba_base.h>
#include <gba_input.h>
#include <gba_dma.h>

#include "ezkernel.h"
#include "showcht.h"
#include "draw.h"


FM_CHT_LINE tmpCHTFS ;

u8 *pCHTbuffer = (u8*)(pReadCache + 0x2000); //patchbuffer


ST_entry pCHEAT[3000]EWRAM_BSS;
u32 gl_cheat_count;

extern u16 gl_select_lang;

u16 gl_color_chtBG    = RGB(4,8,0xC);
//------------------------------------------------------------------


extern FIL gfile;
u8 buf[MAX_BUF_LEN]EWRAM_BSS;
char _paramv[MAX_BUF_LEN] EWRAM_BSS;
//------------------------------------------------------------------
//------------------------------------------------------------------
void Trim(char s[])
{
	int n;
	for(n = strlen(s) - 1; n >= 0; n--)
	{
		if(s[n]!=' ' && s[n]!='\t' && s[n]!='\n')
			break;
		s[n] = '\0';
	}
}
//------------------------------------------------------------------
void Get_KEY_val(FIL* file,char*KEY_section,char*KEY_secval,char getbuff[])
{
	int text_comment = 0;

	int in_section=0;
	int keyval_count=0;

	char section[MAX_KEY_LEN] = {0};

	f_lseek(&gfile, 0x0);
	while(f_gets(buf, MAX_KEY_LEN, &gfile) != NULL)
	{
		Trim(buf);
    // to skip text comment with flags /* ...*/
    if (buf[0] != '#' && (buf[0] != '/' || buf[1] != '/'))
    {
			if (strstr(buf, "/*") != NULL)
			{
				text_comment = 1;
				continue;
			}
			else if (strstr(buf, "*/") != NULL)
			{
				text_comment = 0;
				continue;
			}
    }
    if (text_comment == 1)
	  {
			continue;
    }

		int buf_len = strlen(buf);
          
    // ignore and skip the line with first chracter '#', '=' or '/'
    if (buf_len <= 1 || buf[0] == '#' || buf[0] == '=' || buf[0] == '/')
    {
    	in_section =0;
        continue;
    }

		char _paramk[MAX_KEY_LEN] = {0}; 
		char _paramv[/*MAX_VAL_LEN*/MAX_KEY_LEN] = {0};

    int _kv=0, _klen=0, _vlen=0;
    int i = 0;

    int is_section=0;
    int section_len=0;
    int _val_len=0;

    for (i=0; i<buf_len; ++i)
    {
      if (buf[i] == ' ')
          continue;

			if (buf[i] == '[')
			{
				is_section = 1;
				in_section = 0;
				
				memset(section,0,MAX_KEY_LEN);
				continue;
			}

			if(is_section == 1 && buf[i] != ']')
			{
				section[section_len++] = buf[i];
				continue;
			}
      else if (buf[i] == ']')
      {
          is_section = 0;
          in_section = 1;
          _val_len = 0;
          break;
      }

			if(in_section ==1)
			{				
				// scan param key name
        if (_kv == 0 && buf[i] != '=')
        {
            if (_klen >= MAX_KEY_LEN)
                break;
            _paramk[_klen++] = buf[i];
            continue;
        }
        else if (buf[i] == '=')
        {
            _kv = 1;
            continue;
        }
	      	
	      // scan param key value
	      if (_vlen >= MAX_KEY_LEN || buf[i] == '#')
					break;
	                
	      _paramv[_vlen++] = buf[i];
	    }
	          
	  }
     //DEBUG_printf("KEY_section %s, section %s",KEY_section,section);	     
		if( strcmp(KEY_section,section)== 0)
		{
			if( strcmp(KEY_secval,_paramk) == 0)
			{
				strcpy(getbuff,_paramv); 

				return 0;	
			}				
		}
		if (strcmp(_paramk, "")==0 || strcmp(_paramv, "")==0)
			continue;

		memset(buf,0,MAX_KEY_LEN) ;
  }
}
//------------------------------------------------------------------
u32 Get_CHT_val(FIL* file,char*KEY_section,char*KEY_secval/*,char getbuff[]*/)
{
	int text_comment = 0;

	int in_section=0;
	int keyval_count=0;

	char section[MAX_KEY_LEN] = {0};

	f_lseek(&gfile, 0x0);
	while(f_gets(buf, MAX_BUF_LEN, &gfile) != NULL)
	{
		Trim(buf);
    // to skip text comment with flags /* ...*/
    if (buf[0] != '#' && (buf[0] != '/' || buf[1] != '/'))
    {
			if (strstr(buf, "/*") != NULL)
			{
				text_comment = 1;
				continue;
			}
			else if (strstr(buf, "*/") != NULL)
			{
				text_comment = 0;
				continue;
			}
    }
    if (text_comment == 1)
	  {
			continue;
    }

		int buf_len = strlen(buf);
		//DEBUG_printf("cht buf_len %x",buf_len);	    
          
    // ignore and skip the line with first chracter '#', '=' or '/'
    if (buf_len <= 1 || buf[0] == '#' || buf[0] == '=' || buf[0] == '/')
    {
    	in_section =0;
        continue;
    }

		char _paramk[MAX_KEY_LEN] = {0}; 
		//char _paramv[/*MAX_VAL_LEN*/MAX_KEY_LEN] = {0};

    int _kv=0, _klen=0, _vlen=0;
    int i = 0;

    int is_section=0;
    int section_len=0;
    int _val_len=0;

    for (i=0; i<buf_len; ++i)
    {
      if (buf[i] == ' ')
          continue;

			if (buf[i] == '[')
			{
				is_section = 1;
				in_section = 0;
				
				memset(section,0,MAX_KEY_LEN);
				continue;
			}

			if(is_section == 1 && buf[i] != ']')
			{
				section[section_len++] = buf[i];
				continue;
			}
      else if (buf[i] == ']')
      {
          is_section = 0;
          in_section = 1;
          _val_len = 0;
          break;
      }

			if(in_section ==1)
			{				
				// scan param key name
        if (_kv == 0 && buf[i] != '=')
        {
            if (_klen >= MAX_KEY_LEN)
                break;
            _paramk[_klen++] = buf[i];
            continue;
        }
        else if (buf[i] == '=')
        {
            _kv = 1;
            continue;
        }	      	
	      // scan param key value
	      if (_vlen >= MAX_BUF_LEN || buf[i] == '#')
					break;
	                
	      _paramv[_vlen++] = buf[i];
	    }
	          
	  }
	  _vlen--; //remove 0xd
	    
     //DEBUG_printf("KEY_section %s, section %s",KEY_section,section);	     
		if( strcmp(KEY_section,section)== 0)
		{
			if( strcmp(KEY_secval,_paramk) == 0)
			{
			  //0111 Multi-line
			  while(f_gets(buf, MAX_BUF_LEN, &gfile) != NULL)
				{
					Trim(buf);
			    // to skip text comment with flags /* ...*/
			    if (buf[0] != '#' && (buf[0] != '/' || buf[1] != '/'))
			    {
						if (strstr(buf, "/*") != NULL)
						{
							text_comment = 1;
							break;
						}
						else if (strstr(buf, "*/") != NULL)
						{
							text_comment = 0;
							break;
						}
			    }
			    if (text_comment == 1)
				  {
						break;
			    }

					int buf_len = strlen(buf);
					//DEBUG_printf("cht buf_len %x",buf_len);	    
			          
			    // ignore and skip the line with first chracter '#', '=' or '/'
			    if (buf_len <= 1 || buf[0] == '#' || buf[0] == '=' || buf[0] == '/')
			    {	    	
			        break;
			    }
			    for (i=0; i<buf_len; ++i)
		    	{
		     	 	if (buf[i] == '=')
		          goto endcht;
		      }
			    for (i=0; i<buf_len; ++i)
		    	{
						_paramv[_vlen++] = buf[i];
		      }
		       _vlen--; //remove 0xd
		      //DEBUG_printf("%x %x %x %x %x %x %x %x", buf[0],buf[1], buf[2],buf[3], buf[4],buf[5], buf[6],buf[7]);	
		      //wait_btn();
			    
			  }			  			  
			  endcht:
				return _vlen;	
			}				
		}
		if (strcmp(_paramk, "")==0 || strcmp(_paramv, "")==0)
			continue;

		memset(buf,0,MAX_KEY_LEN) ;
  }
  return 0;
}
//------------------------------------------------------------------
u32 Get_all_Section_val(FIL* file)
{
	char buf[MAX_sectionVAL_LEN];
	int text_comment = 0;

	int in_section=0;
	//char keyval[10][25];
	int keyval_count=0;
	u32 Line = 0;


	char section[MAX_KEY_LEN] = {0};

	//fseek(file,0x0,SEEK_SET);
	f_lseek(&gfile, 0x0);
	while(f_gets(buf, MAX_sectionVAL_LEN, &gfile) != NULL)
	{
		memset(tmpCHTFS.LINEname,0x00,MAX_KEY_LEN);
		Trim(buf);
		// to skip text comment with flags /* ...*/
		if(buf[0] == '-'  && buf[1] == '-')
			break;
		if (buf[0] != '#' && (buf[0] != '/' || buf[1] != '/'))
		{
		    if (strstr(buf, "/*") != NULL)
		    {
		        text_comment = 1;
		        continue;
		    }
		    else if (strstr(buf, "*/") != NULL)
		    {
		        text_comment = 0;
		        continue;
		    }
		}
		if (text_comment == 1)
		{
		    continue;
		}

		int buf_len = strlen(buf);            
		
		// ignore and skip the line with first chracter '#', '=' or '/'
		if (buf_len <= 1 || buf[0] == '#' || buf[0] == '=' || buf[0] == '/')
		{
			in_section =0;
		    continue;
		}
		char _paramk[MAX_KEY_LEN] = {0}; 
		//char _paramv[MAX_sectionVAL_LEN] = {0};

		int _kv=0, _klen=0, _vlen=0;
		int i = 0;

		int is_section=0;
		int section_len=0;
		//int _val_len=0;

		for (i=0; i<buf_len; ++i)
		{
			if (buf[i] == ' ')
				continue;

			if (buf[i] == '[')
			{
				is_section = 1;
				in_section = 0;
				
				memset(section,0,MAX_KEY_LEN);
				continue;
			}

			if(is_section == 1 && buf[i] != ']')
			{
				section[section_len++] = buf[i];
				continue;
			}
      else if (buf[i] == ']')
      {      	
      	memcpy(tmpCHTFS.LINEname,section,section_len);      	
      	tmpCHTFS.is_section = 1;
      	tmpCHTFS.len = section_len;
      	tmpCHTFS.select =0;
      	
      	dmaCopy(&tmpCHTFS,&((FM_CHT_LINE*)pCHTbuffer)[Line], sizeof(FM_CHT_LINE));
      	
      	Line++;
      	//if(Line==10)return;
        is_section = 0;
        in_section = 1;
        //_val_len = 0;
        break;
      }
					
			if(in_section ==1)
			{								
        // scan param key name
        if (_kv == 0 && buf[i] != '=')
        {
            if (_klen >= MAX_KEY_LEN)
                break;
            _paramk[_klen++] = buf[i];
            continue;
        }
        else if (buf[i] == '=')
        {
      		//DEBUG_printf(":%s ",_paramk);
      		memcpy(tmpCHTFS.LINEname,_paramk,_klen);      		
      		tmpCHTFS.is_section = 0; 
      		
      		tmpCHTFS.select =0;  
      			     		        		
          _kv = 1;
          continue;
        }
        else{
		      /*
		      // scan param key value
		      if (_vlen >= MAX_VAL_LEN || buf[i] == '#')
		                break;
		                
		      _paramv[_vlen++] = buf[i];
		      */
	    	}
      }
            
		}	
		if(_kv ==1)
		{	
			tmpCHTFS.len = _vlen;
			//memcpy(tmpCHTFS.KEY_val,_paramv,_vlen);    
			dmaCopy(&tmpCHTFS,&((FM_CHT_LINE*)pCHTbuffer)[Line], sizeof(FM_CHT_LINE));
			Line++;
		}
  }
	return Line;
}
//------------------------------------------------------------------
void Show_KEY_val(u32 total,u32 Select,u32 showoffset)
{
	u32 need_show;	
	u32 line;
	//char buffer_temp[128]={0};
	
	char msg[256];
	
	u32 X_offset=15;
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
			
		u8 select	= ((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].select;
		
		if( ((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].is_section==1)
		{
			Clear(X_offset+3, Y_offset+line*line_x+4, 4, 4, gl_color_text, 1);//section flag
			
			sprintf(msg,"%s",((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].LINEname);		
			
			//int res = utf8_check(((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].LINEname,strlen(((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].LINEname));
			//DEBUG_printf(" %x",res);
		
			//DEBUG_printf(" %x %x %x %x ",((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].LINEname[0],((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].LINEname[1],((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].LINEname[2],((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].LINEname[3]);
	
			//if(res==1)
			//{
				//Utf8ToGb2312(((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].LINEname,strlen(((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].LINEname),buffer_temp);
				//sprintf(msg,"%s ",buffer_temp);
			//}
			
			DrawHZText12(msg,30,X_offset+13,Y_offset+line*line_x, name_color,1);					
		}
		else
		{
			VBlankIntrWait();	
			Draw_select_icon(X_offset+13,Y_offset+line*line_x,select)	;
				
			sprintf(msg,"%s",((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].LINEname);	
			
			/*int res = utf8_check(((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].LINEname,strlen(((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].LINEname));
			if(res==1)
			{
				Utf8ToGb2312(((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].LINEname,strlen(((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].LINEname),buffer_temp);
				sprintf(msg,"%s ",buffer_temp);
			}*/
			
			DrawHZText12(msg,30,X_offset+15+13,Y_offset+line*line_x,name_color,1);	
			//sprintf(msg,"%s",((FM_CHT_LINE*)pCHTbuffer)[showoffset+line].KEY_val);	
			//DrawHZText12(msg,20,X_offset+15+13+60,Y_offset+line*line_x,name_color,1);				
		}
			
	}		
}
//------------------------------------------------------------------
unsigned long str2hex(unsigned char*str)
{
	unsigned long sum=0;
	unsigned long i;
	int len = strlen(str);
	
	if(len >8) return 0;
	for(i=0;i<len;i++)
	{
		if(str[i] >= '0' && str[i] <= '9')
			sum = sum*16 + str[i]-'0';
		else if(str[i] >= 'a' && str[i] <= 'f')
			sum = sum*16 + str[i]-0x57;
		else if(str[i] >= 'A' && str[i] <= 'F')
			sum = sum*16 + str[i]-0x37;										
	}
	return sum;
}
//------------------------------------------------------------------
void Analyze_KEYVAL(FIL* file,u32 total)
{
	u32 tol,i;
	u32 buflen;
	//u32 entry;
	//u32 address;
	u32 address_len;
	u32 val_len;
	u8 address_buf[8];
	u8 val_buf[3];
	u32 is_val;
	u32 is_address;
	u32 address_add;
	//char BUF_val[256];
	
	//DEBUG_printf("total %x  ",total);
	gl_cheat_count=0;
		
	if(total)
	{		
	}
	for(tol=0;tol<total;tol++)
	{
		
		if( ((FM_CHT_LINE*)pCHTbuffer)[tol].select == 1)
		{						
			u32 current_select = tol-1;
			while(((FM_CHT_LINE*)pCHTbuffer)[current_select].is_section != 1)						
			{
				current_select--;
			}
			//DEBUG_printf("section %s  ", ((FM_CHT_LINE*)pCHTbuffer)[current_select].LINEname);										
			buflen= Get_CHT_val(&gfile,((FM_CHT_LINE*)pCHTbuffer)[current_select].LINEname,((FM_CHT_LINE*)pCHTbuffer)[tol].LINEname);
																																				
			address_len=0;
			is_val = 0;
			is_address = 1;
			address_add = 0;
			memset(address_buf,0x00,8);
			for(i=0;i<buflen;i++)
			{
	      if (_paramv[i] == ' '){
	          continue;
				}
				else if (_paramv[i] == ','){						
					if(is_address==1)	//first ','
					{					
						is_address = 0;											 
					}
					else{	//next ','
						//DEBUG_printf(",0x%x =%x", str2hex(address_buf)+address_add,str2hex(val_buf));
						ST_entry cheat = { str2hex(address_buf)+address_add,str2hex(val_buf) };
						pCHEAT[gl_cheat_count++] = cheat;
		
						address_add++;
					}
					val_len=0;
					memset(val_buf,0x00,3);	
					is_val = 1;
					continue;
				}
				else if(_paramv[i] == ';'){
					ST_entry cheat = { str2hex(address_buf)+address_add,str2hex(val_buf) };
					pCHEAT[gl_cheat_count++] = cheat;	
					
					
					is_val = 0;
					is_address = 1;
					address_add=0;
					memset(address_buf,0x00,8);
					address_len=0;				
					continue;
				}
				
				if(is_val){
					val_buf[val_len++] = _paramv[i];
				}
				else{
					address_buf[address_len++] = _paramv[i];
				}							
			}
			ST_entry cheat = { str2hex(address_buf)+address_add,str2hex(val_buf) };
			pCHEAT[gl_cheat_count++] = cheat;
						
		}
	}
}
//------------------------------------------------------------------
u32 Check_count(u32 all_count)
{
	u32 count=0;
	u32 Line;
	for(Line=0;Line<all_count;Line++)
	{
		if(strcmp( ((FM_CHT_LINE*)pCHTbuffer)[Line].LINEname,"GameInfo")==0)
			break;
			
		count++;	
		//if(count>512)
			//break;
	}
	return count;
}
//------------------------------------------------------------------
unsigned char HexToChar(unsigned char bChar)  
{  
    if((bChar>=0x30)&&(bChar<=0x39))  
    {  
        bChar -= 0x30;  
    }  
    else if((bChar>=0x41)&&(bChar<=0x46)) // Capital  
    {  
        bChar -= 0x37;  
    }  
    else if((bChar>=0x61)&&(bChar<=0x66)) //littlecase  
    {  
        bChar -= 0x57;  
    }  
    else   
    {  
        bChar = 0xff;  
    }  
    return bChar;  
}  
//------------------------------------------------------------------
u32 Change2cht_folder(u32 chtname)
{
	TCHAR chtnamebuf[100];
	u32 res;
	TCHAR* folder_name;
	TCHAR currentpath[256];
	memset(currentpath,00,256);
	
	memset(chtnamebuf,0x00,100);
	sprintf(chtnamebuf,"%d%d%d%d",HexToChar(((u8*)&chtname)[0]),HexToChar(((u8*)&chtname)[1]),HexToChar(((u8*)&chtname)[2]),HexToChar(  ((u8*)&chtname)[3] )  );
	u32 num=atoi(chtnamebuf);
	//DEBUG_printf("num =%d", num);					
	if(num < 200){
		folder_name = (TCHAR*)"0000";
	}
	else if(num < 400){
		folder_name = (TCHAR*)"0200";
	}
	else if(num < 600){
		folder_name = (TCHAR*)"0400";
	}			
	else if(num < 800){
		folder_name = (TCHAR*)"0600";
	}
	else if(num < 1000){
		folder_name = (TCHAR*)"0800";
	}
	else if(num < 1200){
		folder_name = (TCHAR*)"1000";
	}			
	else if(num < 1400){
		folder_name = (TCHAR*)"1200";
	}	
	else if(num < 1600){
		folder_name = (TCHAR*)"1400";
	}
	else if(num < 1800){
		folder_name = (TCHAR*)"1600";
	}			
	else if(num < 2000){
		folder_name = (TCHAR*)"1800";
	}				
	else if(num < 2200){
		folder_name = (TCHAR*)"2000";
	}
	else if(num < 2400){
		folder_name = (TCHAR*)"2200";
	}			
	else if(num < 2600){
		folder_name = (TCHAR*)"2400";
	}	
	else if(num < 2800){
		folder_name = (TCHAR*)"2600";
	}				
	else {
		folder_name = (TCHAR*)"2800";
	}						
	
	
	if(gl_select_lang == 0xE1E1)//english
	{
		sprintf(currentpath,"/CHEAT/Eng/%s",folder_name);
	}
	else{		
		sprintf(currentpath,"/CHEAT/Chn/%s",folder_name);
	}
	res=f_chdir(currentpath);		
	return res;
}
//------------------------------------------------------------------
u32 Check_cheat_file(TCHAR *gamefilename)
{
	u32 res;
	UINT ret;
	TCHAR chtnamebuf[100];	
	u32 filesize;
	u32 GAMEID=0;
	u32 i;

	res = f_open(&gfile, gamefilename, FA_READ);
	if(res == FR_OK)
	{
		f_lseek(&gfile, 0xAC);
		f_read(&gfile, &GAMEID, 4, (UINT *)&ret);
		f_close(&gfile);
		if(GAMEID==0) return 0;
	}
	
	memcpy(chtnamebuf,gamefilename,100);
	u32 len=strlen(chtnamebuf);
	chtnamebuf[len-3] = 'c';
	chtnamebuf[len-2] = 'h';
	chtnamebuf[len-1] = 't';	
	
	res=f_chdir("/CHEAT");
	if(res != FR_OK){
		return 0;
	}
	
	res = f_open(&gfile,chtnamebuf, FA_OPEN_EXISTING);	
	//f_chdir(currentpath);
	if(res == FR_OK)//have a cht file
	{
		f_close(&gfile);
		return 0x0000FFFF;
	}					
	else
	{			
		res = f_open(&gfile,"GameID2cht.bin", FA_READ);
		u32* tempbuff = (u32*)(pReadCache);
		if(res == FR_OK)//have a file
		{
			filesize = f_size(&gfile);
			if(filesize > 0x10000) filesize=0x10000;
			f_lseek(&gfile, 0x0);
			f_read(&gfile, tempbuff, filesize, &ret);//pReadCache max 0x20000 Byte

			for(i=0;i<filesize/4;i+=2)
			{
				
				if(tempbuff[i]== GAMEID)
				{
					f_close(&gfile); 
					
					u32 chtname= ((u32*)tempbuff)[i+1];

					res=Change2cht_folder(chtname);
					if(res!=0)return 0;
					memset(chtnamebuf,0x00,100);
					sprintf(chtnamebuf,"%d%d%d%d.cht",HexToChar(((u8*)&chtname)[0]),HexToChar(((u8*)&chtname)[1]),HexToChar(((u8*)&chtname)[2]),HexToChar(  ((u8*)&chtname)[3] )  );			
					res = f_open(&gfile,chtnamebuf, FA_OPEN_EXISTING);	

					if(res == FR_OK)//have a cht file
					{					
						f_close(&gfile);							
						return chtname;
					}
					else{
						return 0;
					}
				}
			}			
		}
		return 0;
	}	
}
//---------------------------------------------------------------------------------
void Show_num(u32 totalcount,u32 select)
{
	char msg[20];
	Clear(186, 3, 7*6, 15, gl_color_chtBG, 1);
	sprintf(msg,"[%03lu/%03lu]",select,totalcount);

	DrawHZText12(msg,0,182,3, gl_color_text,1);
}
//------------------------------------------------------------------
void Open_cht_file(TCHAR *gamefilename,u32 havecht)
{
	u32 res;
	char msg[128];
	TCHAR chtnamebuf[100];	

	char buffer[128]={0};
		
	if(havecht == 0x0000FFFF)
	{
		res=f_chdir("/CHEAT");
		if(res != FR_OK){
			return;
		}	
		memcpy(chtnamebuf,gamefilename,100);	
		u32 len=strlen(chtnamebuf);
		chtnamebuf[len-3] = 'c';
		chtnamebuf[len-2] = 'h';
		chtnamebuf[len-1] = 't';			
	}
	else
	{
		Change2cht_folder(havecht);
		u8* chtmode;
		chtmode = &havecht;
		sprintf(chtnamebuf,"%d%d%d%d.cht",HexToChar(chtmode[0]),HexToChar(chtmode[1]),HexToChar(chtmode[2]),HexToChar(chtmode[3]));
	}	
	res = f_open(&gfile,chtnamebuf, FA_READ);	

	if(res == FR_OK)//have a cht file
	{		
		Clear(0, 0, 240, 160, gl_color_chtBG, 1);
		Clear(0, 18, 240, 1, gl_color_selected, 1);

		Get_KEY_val(&gfile,"GameInfo","Name",buffer);
		sprintf(msg,"%s ",buffer);
		
		DrawHZText12(msg,30,2,4, gl_color_text,1);
		
		u32 all_count = Get_all_Section_val(&gfile);
		u32 Select = 1;
		u32 showoffset = 0;
		u32 re_show = 2;
			
		if(all_count)
		{
			all_count = Check_count(all_count);//cut from "GameInfo"
			setRepeat(15,1);		
			while(1)//3
			{
				VBlankIntrWait();
				VBlankIntrWait();	
				if(re_show)
				{
					if(re_show>1)
					{
						Clear(0, 19, 240, 160-19, gl_color_chtBG, 1);
					}
					Show_KEY_val(all_count,Select,showoffset);
					Show_num(all_count,Select+showoffset+1);
					re_show = 0;
				}								
				scanKeys();
				u16 keysdown  = keysDown();
				u16 keysup = keysUp();
				u16 keysrepeat = keysDownRepeat();	
								
				if (keysrepeat & KEY_DOWN) {
					if (Select + showoffset+1 < (all_count )) {
		        if ( Select > 8 ){
		          if ( Select == 9 ) {
		            showoffset++;
          			if( ((FM_CHT_LINE*)pCHTbuffer)[showoffset+Select].is_section==1){
          				showoffset++;
          			}		            
		            re_show=2;
		          }
		        }else{
		          Select++;
		          re_show=1;
		          if( ((FM_CHT_LINE*)pCHTbuffer)[showoffset+Select].is_section==1){
		          	Select++;
				        if ( Select > 9 ){
				        		Select--;
				            showoffset++;

				            
				            re_show=2;
				        }
		          }		          
		        }

					}
					
				}
				else if(keysrepeat & KEY_UP)
				{
					if (Select ) {
						Select--;
  					if((showoffset==0)&& (Select==0)){
  						if( ((FM_CHT_LINE*)pCHTbuffer)[Select].is_section==1){
  							Select=1;
  						}
  					}	
  					
						if( ((FM_CHT_LINE*)pCHTbuffer)[showoffset+Select].is_section==1){
							if(Select)
	          		Select--;
	          }						
						re_show=1;
					}else{
						if (showoffset){
							showoffset--;
    					if(showoffset==0){
    						if( ((FM_CHT_LINE*)pCHTbuffer)[0].is_section==1){
    							Select=1;
    						}
    					}						
							
        			if( ((FM_CHT_LINE*)pCHTbuffer)[showoffset+Select].is_section==1){
        				if(showoffset){
        					showoffset--;
        					if(showoffset==0){
        						if( ((FM_CHT_LINE*)pCHTbuffer)[0].is_section==1){
        							Select=1;
        						}
        					}
        				}
        			}	
							re_show=2;
						}
					}
				}
				else if(keysrepeat & KEY_LEFT)
				{
			    if ( showoffset )
			    {
			      if ( showoffset > 9 )
			        showoffset -= 10;
			      else
			        showoffset = 0;

						if( ((FM_CHT_LINE*)pCHTbuffer)[showoffset+Select].is_section==1){
							Select--;
						}
			      re_show=2;
			    }
			    else{
			    	if(Select>1){
			    		Select=1;
			    		re_show=1;
			   	 	}
			    }
				}
				else if(keysrepeat & KEY_RIGHT)
				{
		      if ( showoffset + 10 < all_count )
		      {
		        if ( showoffset + 20 <= all_count )
		          showoffset += 10;
		        else
		          showoffset = all_count - 10;
		          
						if( ((FM_CHT_LINE*)pCHTbuffer)[showoffset+Select].is_section==1){
							showoffset--;
						}
						re_show=2;
		      }
				}
				else if(keysdown & KEY_A)
				{
					if(((FM_CHT_LINE*)pCHTbuffer)[showoffset+Select].is_section != 1)
					{
						u32 current_select = showoffset+Select-1;
						while(((FM_CHT_LINE*)pCHTbuffer)[current_select].is_section != 1)						
						{
							((FM_CHT_LINE*)pCHTbuffer)[current_select--].select = 0;
						}
						current_select = showoffset+Select+1;
						while(((FM_CHT_LINE*)pCHTbuffer)[current_select].is_section != 1)						
						{
							((FM_CHT_LINE*)pCHTbuffer)[current_select++].select = 0;
						}		
						
						u8 select	= ((FM_CHT_LINE*)pCHTbuffer)[showoffset+Select].select;
						((FM_CHT_LINE*)pCHTbuffer)[showoffset+Select].select = !select;
					
						re_show=1;
					}
				}
				else if(keysup & KEY_B)
				{
					Analyze_KEYVAL(&gfile,all_count);
					break;
				}				
			}
		}
	}					
	f_close(&gfile);	
}
