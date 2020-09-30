//MODIFIED VERSION of libgba's interrupt add/remove code
//instead of using a key/value pair list, this uses a flat array with 14 elements.
//There's really nothing to it, but the old copyright notice stays on anyway

/*

	libgba interrupt support routines

	Copyright 2003-2004 by Dave Murphy.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
	USA.

	Please report all bugs and problems through the bug tracker at
	"http://sourceforge.net/tracker/?group_id=114505&atid=668551".

*/

//we redefined this, so the old "extern" declaration found in libgba's headers should be made invalid
#define IntrTable __IntrTable

#include "gba_interrupt.h"
#include "gba_video.h"

#undef IntrTable

IntFn IntrTable[14];

//---------------------------------------------------------------------------------
//struct IntTable IntrTable[MAX_INTS];
void dummy(void) {};


//---------------------------------------------------------------------------------
void InitInterrupt(void) {
//---------------------------------------------------------------------------------
	irqInit();
}

//---------------------------------------------------------------------------------
void irqInit() {
//---------------------------------------------------------------------------------
	int i;
	
	// Set all interrupts to dummy functions.
	for(i = 0; i < 14; i ++)
	{
		IntrTable[i] = dummy;
		//IntrTable[i].mask = 0;
	}
	
	INT_VECTOR = IntrMain;
}

//---------------------------------------------------------------------------------
IntFn* SetInterrupt(irqMASK mask, IntFn function) {
//---------------------------------------------------------------------------------
	return irqSet(mask,function);
}

static __inline int log2_int(int value)
{
	int outputValue = 0;
	if (value & 0xFF00)
	{
		value >>= 8;
		outputValue+=8;
	}
	if (value & 0xF0)
	{
		value >>=4;
		outputValue+=4;
	}
	if (value & 0x0C)
	{
		value >>=2;
		outputValue+=2;
	}
	if (value & 2)
	{
		outputValue++;
	}
	return outputValue;
}


//---------------------------------------------------------------------------------
IntFn* irqSet(irqMASK mask, IntFn function) {
//---------------------------------------------------------------------------------
	int i = log2_int(mask);
	if (i >= 14) return NULL;
	IntrTable[i]=function;
	return &IntrTable[i];
/*	
	int i;

	for	(i=0;;i++) {
		if	(!IntrTable[i].mask || IntrTable[i].mask == mask) break;
	}

	if ( i >= MAX_INTS) return NULL;

	IntrTable[i].handler	= function;
	IntrTable[i].mask		= mask;

	return &IntrTable[i].handler;
*/
}

//---------------------------------------------------------------------------------
void EnableInterrupt(irqMASK mask) {
//---------------------------------------------------------------------------------
	irqEnable(mask);
}

//---------------------------------------------------------------------------------
void irqEnable ( int mask ) {
//---------------------------------------------------------------------------------
	REG_IME	= 0;

	if (mask & IRQ_VBLANK) REG_DISPSTAT |= LCDC_VBL;
	if (mask & IRQ_HBLANK) REG_DISPSTAT |= LCDC_HBL;
	if (mask & IRQ_VCOUNT) REG_DISPSTAT |= LCDC_VCNT;
	REG_IE |= mask;
	REG_IME	= 1;
}

//---------------------------------------------------------------------------------
void DisableInterrupt(irqMASK mask) {
//---------------------------------------------------------------------------------
	irqDisable(mask);
}

//---------------------------------------------------------------------------------
void irqDisable(int mask) {
//---------------------------------------------------------------------------------
	REG_IME	= 0;

	if (mask & IRQ_VBLANK) REG_DISPSTAT &= ~LCDC_VBL;
	if (mask & IRQ_HBLANK) REG_DISPSTAT &= ~LCDC_HBL;
	if (mask & IRQ_VCOUNT) REG_DISPSTAT &= ~LCDC_VCNT;
	REG_IE &= ~mask;

	REG_IME	= 1;

}
