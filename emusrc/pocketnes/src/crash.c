#include "includes.h"

#if CRASH

const char *const regtxt[]=
{
	"R4 ",
	"R5 ",
	"R6 ",
	"R7 ",
	"R8 ",
	"R9 ",
	"R10",
	"R11",
	"R12",
	"   ",
	"LR "
};

const char *const regtxt2[]=
{
	"CPSR",
	"    ",
	"    ",
	"    ",
	" R0 ",
	" R1 ",
	" R2 ",
	" R3 ",
	" R12",
	" PC "
};

const char *const regtxt3[]=
{
	"PC  ",
	"LR  ",
	"R0  ",
	"R1  ",
	"R2  ",
	"R12 "
};

const char reg3offsets[]=
{
	9,128+10,4,5,6,7
};


void crash(u32 *stackpointer, u32 *irqstackpointer)
{
	int i;
	bool proceed = false;
	ui_x=0;
	ui_y=0;
	move_ui();
	cls(1);
	setdarknessgs(7);
	
	REG_IME = 0;
	stop_dma_interrupts();
	get_ready_to_display_text();
	REG_DISPCNT = 0x0400;
	
	drawtext(0,"Oh no! PocketNES crashed!",0);
	drawtext(1,"Press SELECT to reboot.",0);
	drawtext(2,"Register contents:",0);
	drawtext(3,"(send me these)",0);
	
	for (i=0; i < 6; i++)
	{
		int offset;
		str[0]=0;
		strcat(str,regtxt3[i]);
		offset = reg3offsets[i];
		if (offset < 128)
		{
			strcat(str,hex8(irqstackpointer[offset]));
		}
		else
		{
			strcat(str,hex8(stackpointer[offset & 127]));
		}
		drawtext(i+5,str,0);
	}
	
	drawtext(12, "Press A for stack trace",0);
	
	proceed = false;
	{
		u32 key=~REG_P1;
		u32 oldkey=key;
		u32 pressed=0;
		while (pressed != SELECT)
		{
			oldkey = key;
			key = ~REG_P1;
			pressed = (key^oldkey) & key;
			if (pressed == SELECT)
			{
				break;
			}
			if (pressed == A_BTN)
			{
				proceed = true;
				break;
			}
		}
	}
	if (proceed)
	{
		cls(1);
		drawtext(0,"Stack trace-SELECT to reset",0);

		for (i=0; i < 19; i++)
		{
			str[0]=0;

			if (i<ARRSIZE(regtxt))
			{
				strcat(str,regtxt[i]);
				strcat(str," ");
			}
			else
			{
				strcat(str,"    ");
			}
			strcat(str,hex8(stackpointer[i]));
			strcat(str," ");
			if (i<ARRSIZE(regtxt2))
			{
				strcat(str,regtxt2[i]);
				strcat(str," ");
			}
			else
			{
				strcat(str,"     ");
			}
			strcat(str,hex8(irqstackpointer[i]));
			drawtext(i+1,str,0);
		}

		{
			u32 key=~REG_P1;
			u32 oldkey=key;
			u32 pressed=0;
			while (pressed != SELECT)
			{
				oldkey = key;
				key = ~REG_P1;
				pressed = (key^oldkey) & key;
			}
		}
	}
	doReset();
}

#endif
