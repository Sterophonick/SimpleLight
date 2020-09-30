#include "includes.h"

#if CHEATFINDER

#define cf_getbit(i) bits[i>>5]&(1<<(31-(i&31)))
#define cf_togglebit(i) bits[i>>5]^=1<<(31-(i&31))

EWRAM_BSS u32 *cheatfinder_bits=NULL; //size 320
EWRAM_BSS u8 *cheatfinder_values=NULL; //size 10240
EWRAM_BSS u8 *cheatfinder_cheats=NULL; //size 900?

EWRAM_BSS int cheatfinderstate;
EWRAM_BSS int num_cheats=0;
//static const int MAX_CHEATS=50; // 900/18
EWRAM_BSS u8 compare_value;
EWRAM_BSS short found_for[7];

int print_cheatfinder_line_func(int row, const char *oper, int value)
{
	char *compare=&str[32-4];
	char *dest=str;
	dest=strcpy(dest,"Value");
	dest=strcat(dest,oper);
	dest=strcat(dest,compare);
	dest=strcat(dest," - ");
	dest=strcat(dest,number(value));
	return text2_str(row);
}
int print_cheatfinder_line_newold_func(int row, const char *oper, int value)
{
//	char *compare=&str[32-4];
	char *dest=str;
	dest=strcpy(dest,"New");
	dest=strcat(dest,oper);
	dest=strcat(dest,"Old - ");
	dest=strcat(dest,number(value));
	return text2_str(row);
}

/*
#define print_1(xxxx,yyyy) row=print_1_func(row,(xxxx),(yyyy))
#define print_2(xxxx,yyyy) row=print_2_func(row,(xxxx),(yyyy))
#define print_1_1(xxxx) row=text(row,(xxxx));
#define print_2_1(xxxx) row=text2(row,(xxxx));

#define print_cheatfinder_line(xxxx,yyyy) row=print_cheatfinder_line_func(row,(xxxx),(yyyy))
#define print_cheatfinder_line_newold(xxxx,yyyy) row=print_cheatfinder_line_newold_func(row,(xxxx),(yyyy))
*/

void ui5()
{
	if (cheatfinder_bits==NULL || cheatfinder_values==NULL || cheatfinder_cheats==NULL)
	{
		return;
	}

/*	if (sprite_vram_in_use)
	{
		cheatfinderstate=0;
		return;
	}
*/	
	if (cheatfinderstate==0)
	{
		cf_newsearch();
	}
	else
	{
		update_cheatfinder_tally();
	}
	subui(5);
}

void drawui5()
{
	int row=0;
	cls(2);
	drawtext(32,"       Cheat Finder",0);
	
	print_2_1("Edit Cheats...");
	print_2("Search Results - ",number(found_for[6]));

	if (cheatfinderstate==1)
	{
//		looks like:
//		"New==Old - 65535"
		print_cheatfinder_line_newold("==",found_for[0]);
		print_cheatfinder_line_newold("!=",found_for[1]);
		print_cheatfinder_line_newold(" >",found_for[2]);
		print_cheatfinder_line_newold(" <",found_for[3]);
		print_cheatfinder_line_newold(">=",found_for[4]);
		print_cheatfinder_line_newold("<=",found_for[5]);
		print_2("Compare with number - ","Off");
		print_2_1("Update Values");
	}
	else
	{
//		char compare[4];
		char *compare=&str[32-4];
//		looks like:
//		"Value==FF - 65535"
		strcpy(compare,hexn(compare_value,2));
		print_cheatfinder_line("==",found_for[0]);
		print_cheatfinder_line("!=",found_for[1]);
		print_cheatfinder_line(" >",found_for[2]);
		print_cheatfinder_line(" <",found_for[3]);
		print_cheatfinder_line(">=",found_for[4]);
		print_cheatfinder_line("<=",found_for[5]);
		
		
//		print_2_4("Value==",compare," - ",number(found_for[0]));
//		print_2_4("Value!=",compare," - ",number(found_for[1]));
//		print_2_4("Value >",compare," - ",number(found_for[2]));
//		print_2_4("Value <",compare," - ",number(found_for[3]));
//		print_2_4("Value>=",compare," - ",number(found_for[4]));
//		print_2_4("Value<=",compare," - ",number(found_for[5]));
		print_2("Compare with number - ","On");
		print_2("Number to compare to: ",compare);
	}
	print_2_1("New Search");
}

void update_cheatfinder_tally(void)
{
	found_for[0]=cheat_test(0,0);
	found_for[1]=cheat_test(4,0);
	found_for[2]=cheat_test(8,0);
	found_for[3]=cheat_test(12,0);
	found_for[4]=cheat_test(16,0);
	found_for[5]=cheat_test(20,0);
	found_for[6]=cheat_test(24,0);
/*
	found_for[0]=cheat_test(is_equal,0);
	found_for[1]=cheat_test(is_not_equal,0);
	found_for[2]=cheat_test(is_greater,0);
	found_for[3]=cheat_test(is_less,0);
	found_for[4]=cheat_test(is_greater_equal,0);
	found_for[5]=cheat_test(is_less_equal,0);
	found_for[6]=cheat_test(is_always_true,0);
*/
}

void reset_cheatfinder(void)
{
	u32 *const bits=cheatfinder_bits;
//	u8 *const values=cheatfinder_values;
//	u8 *const cheats=cheatfinder_cheats;
//	int i;
	memset32(bits,0xFFFFFFFF,1280*sizeof(char));
//	for (i=0;i<1280;i++)
//	{
//		bits[i]=0xFF;
//	}
//	for (i=0;i<10240;i++)
//	{
//		values[i]=0xFF;
//	}
//	update_cheatfinder_tally();
}
void cheat_memcopy(void)
{
	u8 *const values=cheatfinder_values;
	memcpy32(values,NES_RAM,10240);
//	memcpy32(values+2048,NES_SRAM,8192);
}

void write_byte(u8 *address, u8 data)
{

	u16 *addr2;
	//if not hw aligned
	if ( (int)address & 1)
	{
		addr2=(u16*)((int)address-1);
		*addr2 &= 0xFF;
		*addr2 |= (data << 8);
	}
	else
	{
		addr2=(u16*)address;
		*addr2 &= 0xFF00;
		*addr2 |= data;
	}
}

int add_cheat(u16 address, u8 value)
{
	u8* cheats=cheatfinder_cheats;
	u32 element=num_cheats*3;
	int i;
	if (num_cheats<MAX_CHEATS)
	{
		//'012abcdefghijklmno' to
		//'012345abcdefghijklmno               '
		//'012345678abcdefghijklmnoABCDEFGHIJKLMNO               '
		for(i=num_cheats*18+3-1;i>=(element+3);i--)
		   write_byte(&cheats[i],cheats[i-3]);
		for(i=num_cheats*18+3;i<(num_cheats+1)*18;i++)
		   write_byte(&cheats[i],' ');
		write_byte(&cheats[element],address&255);
		write_byte(&cheats[element+1],address>>8);
		write_byte(&cheats[element+2],value);
		num_cheats++;
		return 1;
	}
	else
	{
		return 0;
	}
}

char* real_address(u16 addr)
{
	addr&=0x7FFF;
	if (addr>=0x800)
	{
		addr-=0x800;
		addr+=0x6000;
	}
	return hex4(addr);
}

void do_cheats(void)
{
	int i;
	u8* cheats=cheatfinder_cheats;
	u8 data;
	u16 addr;
	u32 max=num_cheats*3;
	
	if (cheats==NULL) return;
	
	for (i=0;i<max;i+=3)
	{
		addr=cheats[i]+(cheats[i+1]<<8);
		data=cheats[i+2];
		if (addr<10240)
		{
//			if (addr<0x800)
//			{
				(NES_RAM)[addr]=data;
//			}
//			else
//			{
//				(NES_SRAM)[addr-0x800]=data;
//			}
		}
	}
}
void do_cheat_test(u32 testfunc)
{
	cheat_test(testfunc,1);
	cheat_memcopy();
	update_cheatfinder_tally();
}
//void do_cheat_test(cheattestfunc testfunc)
//{
//	cheat_test(testfunc,1);
//	cheat_memcopy();
//	update_cheatfinder_tally();
//}

void cf_equal(void)
{
	do_cheat_test(0);
//	cheat_memcopy();
//	update_cheatfinder_tally();
}
void cf_notequal(void)
{
	do_cheat_test(4);
//	cheat_memcopy();
//	update_cheatfinder_tally();
}
void cf_greater(void)
{
	do_cheat_test(8);
//	cheat_memcopy();
//	update_cheatfinder_tally();
}
void cf_less(void)
{
	do_cheat_test(12);
//	cheat_memcopy();
//	update_cheatfinder_tally();
}
void cf_greaterequal(void)
{
	do_cheat_test(16);
//	cheat_memcopy();
//	update_cheatfinder_tally();
}
void cf_lessequal(void)
{
	do_cheat_test(20);
//	cheat_memcopy();
//	update_cheatfinder_tally();
}
void cf_comparewith(void)
{
	cheatfinderstate^=3;
	update_cheatfinder_tally();
}
void cf_update(void)
{
	if (cheatfinderstate==1)
	{
		cheat_memcopy();
		update_cheatfinder_tally();
	}
	else
	{
		compare_value=inputhex(9,22,compare_value,2);
		update_cheatfinder_tally();
	}

}
void cf_newsearch(void)
{
	if (cheatfinder_bits==NULL || cheatfinder_values==NULL || cheatfinder_cheats==NULL)
	{
		return;
	}


	reset_cheatfinder();
	cheat_memcopy();
	if (cheatfinderstate==0)
	{
		cheatfinderstate=1;
	}
	update_cheatfinder_tally();
}
int cf_next_result(int i)
{
	u32 *const bits=cheatfinder_bits;
	do

	{
		if (i<10240)
		{
			if (cf_getbit(i))
			{
				return i;
			}
			i++;
		}
		else
		{
			return -1;
		}
	} while(1);
}
int cf_result(int n)
{
	int i=0;
	do
	{
		i=cf_next_result(i);
		if (i!=-1)
		{
			if (n==0)
				return i;
			n--;
			i++;
		}
		else
		{
			return -1;
		}
	} while(1);
}

__inline u8 cf_readmem(int i)
{
	return (NES_RAM)[i];
//	if (i<2048)
//		return (NES_RAM)[i];
//	else if (i<10240)
//		return (NES_SRAM)[i-2048];
//	return 0;
}

void cf_drawresults()
{
	int bottom=found_for[6];
	u8 *const values=cheatfinder_values;
	int i;
	u8 value;
	int line=0;
	char str[30];
	int top,sel;
	sel=selected;
	top=selected-5;
	if (top<0) top=0;
	selected=sel-top;
	
	i=cf_result(top);
	cls(2);
	drawtext(32,"    Cheat Search Results",0);
	drawtext(33,"Press A to add cheat",0);
	while (line<10)
	{
		if (i==-1)
			break;
		value=cf_readmem(i);
		strmerge(str,real_address(i),": ");
		strmerge(str,str,hexn(value,2));
		strmerge3(str,str," was ",hexn(values[i],2));
		text2(line,str);
		line++;
		if (line>=bottom-top || line>10)
			break;
		i=cf_next_result(i+1);
	}
	selected=sel;
}


void cf_results(void)
{
	int bottom;
	int key;
	
	bool any_cheats_added=false;
	
	bottom=found_for[6];  // 0<=selected<bottom
	if (bottom==0) return;
	selected=0;
	
	cf_drawresults();
	oldkey=~REG_P1;			//reset key input
	do {
		key=getmenuinput(bottom);
		if(key&(A_BTN)) {
			int add_address=cf_result(selected);
			u8 add_value=cf_readmem(add_address);
			if (add_cheat(add_address,add_value))
			{
				edit_cheat(num_cheats-1);
				any_cheats_added=true;
			}
		}
		if(key&(A_BTN+UP+DOWN+LEFT+RIGHT))
			cf_drawresults();
	} while(!(key&(B_BTN+R_BTN+L_BTN)));
	while(key&(B_BTN)) {
		waitframe();		//(polling REG_P1 too fast seems to cause problems)
		key=~REG_P1;
	}
	#if SAVE
	if (any_cheats_added)
	{
		cheatsave();
	}
	#endif
}

void cf_drawedit(int center)
{
	int bottom=num_cheats+1;

	u8 *cheats=cheatfinder_cheats;
	u16 address;
	u8 data;
	int i;
	int max;
	int line=0;
	char str[30];
	char buffer[18];
	int top,sel;
	
	sel=selected;
	top=center-5;
	if (top<0) top=0;
	selected=sel-top;
	if (bottom>top+10) bottom=top+10;
	max=bottom*3;
	
	cls(2);
	//          "                             "
	drawtext(32,"     Cheat List Editor",0);
	drawtext(33,"A to Edit, Start to Toggle",0);
	drawtext(34,"Select to delete",0);
//	drawtext(35,"R to poke",0);  //disabled

	buffer[16]=buffer[0]=' ';
	buffer[17]='\0';
	for (i=top*3;i<max;i+=3)
	{
		if (i==num_cheats*3)
		{
			strcpy(str,"Add New Cheat");
		}
		else
		{
//entries look like 6543: 21 (currently FF) Off
//entries now look like 6543:21 Infinite Health Off
			address=cheats[i]+(cheats[i+1]<<8);
			data=cheats[i+2];
			//strmerge(str,real_address(address),": ");
			strmerge(str,real_address(address),":");
			strmerge(str,str,hexn(data,2));
			//15 character long cheat desc
			memcpy(&buffer[1],&cheats[num_cheats*3+(i*15/3)],15);
			strmerge(str,str,buffer);
			//strmerge4(str,str," (currently ",hexn(cf_readmem(address&0x7FFF),2),") ");
			strmerge(str,str,(address&0x8000)?"Off":"On");
		}
		text2(line,str);
		line++;
	}
	selected=sel;
}


void edit_cheat(int cheatnum)
{
	int bottom=num_cheats+1;

	u8 *const cheats=cheatfinder_cheats;
	u32 address;
	u8 data;
	int i;
//	int line=0;
	int top,sel;
	int row,column;
	int enabled;
	char buffer[16];
	
	u8* activecheat=cheats+cheatnum*3;
	u8* activecheatdesc=cheats+num_cheats*3+cheatnum*15;

	sel=selected;
	top=cheatnum-5;
	selected=-1;
	if (top<0) top=0;
	row=cheatnum-top;
	column=0;
	if (bottom>top+10) bottom=top+10;
	
	cf_drawedit(cheatnum);

	address=activecheat[0]+(activecheat[1]<<8);
	enabled=address&0x8000;
	data=activecheat[2];

	address&=0x7FFF;

	if (address>=0x800) address+=0x5800;
	address=inputhex(row,column,address,4);
	address&=0x7FFF;
	if (address<0x6000)
		address&=0x7FF;
	else
	{
		address-=0x5800;
	}
	address|=enabled;
	write_byte(&activecheat[0],address&255);
	write_byte(&activecheat[1],address>>8);
	cf_drawedit(cheatnum);
	
	column=5;
	data=inputhex(row,column,data,2);

	write_byte(&activecheat[2],data);
	cf_drawedit(cheatnum);
	
	column=8;
	buffer[15]='\0';
	memcpy(buffer,activecheatdesc,15);
	inputtext(row,column,buffer,15);
	for (i=0;i<15;i++)
	    write_byte(&activecheatdesc[i],buffer[i]);
	
	selected=sel;
}

void delete_cheat(int i)
{
	u8* cheats=cheatfinder_cheats;
	u8* cheatsdesc;
	int j;
//	u32 element=num_cheats*3;
	if (i<num_cheats)
	{
		//'012012abcdefghijklmnoABCDEFGHIJKLMNO' to
		//'012ABCDEFGHIJKLMNO'
		num_cheats--;
		for (j=i;j<num_cheats;j++)

		{
			write_byte(&cheats[j*3+0],cheats[j*3+3]);
			write_byte(&cheats[j*3+1],cheats[j*3+4]);
			write_byte(&cheats[j*3+2],cheats[j*3+5]);
		}
		cheatsdesc=cheats+num_cheats*3;
		for (j=0;j<i*15;j++)
			write_byte(&cheatsdesc[j],cheatsdesc[j+3]);
		for (j=i*15;j<num_cheats*15;j++)
			write_byte(&cheatsdesc[j],cheatsdesc[j+15+3]);
	}
}


void cf_editcheats(void)
{
	u8* cheats=cheatfinder_cheats;
	bool cheats_dirty=false;

	int key;//,oldsel;
	selected=0;
	
	cf_drawedit(selected);
//	scrolll(0);
	oldkey=~REG_P1;			//reset key input
	do {
		key=getmenuinput(num_cheats+1);
//		if(key==(R_BTN)) {  //poke feature disabled
//			u16 addr=(cheats[selected*3+0]+(cheats[selected*3+1]<<8))&0x7FFF;
//			u8 data=cheats[selected*3+2];
//			if (addr<0x800)
//			{
//				(NES_RAM)[addr]=data;
//			}
//			else
//			{
//				(NES_SRAM)[addr-0x800]=data;
//			}
//		}
		if(key&(START))
		{
			if (selected==num_cheats)
			{
				if(add_cheat(0,0))
					edit_cheat(selected);
				cheats_dirty=true;
			}
			else
			{
				write_byte(&cheats[selected*3+1],cheats[selected*3+1]^0x80);
				cheats_dirty=true;
			}
		}
		if(key&(A_BTN))
		{
			if (selected<num_cheats)
			{
				edit_cheat(selected);
				cheats_dirty=true;
			}
			if (selected==num_cheats)
			{
				if (add_cheat(0,0))
					edit_cheat(selected);
				cheats_dirty=true;
			}
		}
		if (key&(SELECT))
		{
			delete_cheat(selected);
			cheats_dirty=true;
		}
		if(key&(A_BTN+UP+DOWN+LEFT+RIGHT+START+SELECT))
			cf_drawedit(selected);
	} while(!(key&(B_BTN+L_BTN+R_BTN)));
//	scrollr();
	while(key&(B_BTN)) {
		waitframe();		//(polling REG_P1 too fast seems to cause problems)
		key=~REG_P1;
	}
	#if SAVE
	if (cheats_dirty)
	{
		cheatsave();
	}
	#endif
}

#endif
