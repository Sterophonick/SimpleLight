#include "gba.h"

extern char gameboyplayer;

EWRAM_BSS u32 SerialIn, DoRumble, RumbleCnt;
EWRAM_BSS u16 stage, ind;
EWRAM_BSS u16 SerOut0, SerOut1;
char const GBPData[]={"NINTENDO"};

void RumbleInterrupt(void) {
	u32 OutData=0;
	u16 SerIn0, SerIn1;
	u16 *GBPD2 = (u16*)GBPData;
	
	SerialIn = REG_SIODATA32;
	switch(stage) {
		case 0:
			SerIn0 = SerialIn>>16;
			SerIn1 = SerialIn;
			if(SerIn0 == SerOut1){
				if(ind <=3){
					if( SerialIn == ~(SerOut1 | (SerOut0<<16)) ){
						ind++;
					}
				}else{
					if(SerIn1 == 0x8002){
						OutData = 0x10000010;
						stage=1;
						break;
					}
				}
			}else{
				ind = 0;
			}
			if(ind <=3){
				SerOut0 = GBPD2[ind];
			}else{
				SerOut0 = 0x8000;
			}
			SerOut1 = ~SerIn1;
			OutData = SerOut1 | (SerOut0<<16);
			break;

		case 1:
			if(SerialIn == 0x10000010){
				OutData = 0x20000013;
				stage=2;
			}else{
				stage = 4;
			}
			break;

		case 2:
			if(SerialIn == 0x20000013){
				OutData = 0x40000004;
				stage=3;
			}else{
				stage = 4;
			}
			break;

		case 3:
			if(SerialIn == 0x30000003){
				if(DoRumble) RumbleCnt = 2;
				if(RumbleCnt){
					RumbleCnt--;
					OutData = 0x40000026;
				}else{
					OutData = 0x40000004;
				}
			}else{
				stage = 4;
			}
			break;

		case 4:
			SerialIn = 0;
			DoRumble = 0;
			stage = 0;
			ind = 0;
			SerOut0 = 0;
			SerOut1 = 0;
			return;
	}

	REG_SIODATA32 = OutData;
	REG_SIOCNT |= 0x80;
}

void StartRumbleComs(void) {
	if( (SerialIn != 0x30000003) && gameboyplayer){
		REG_RCNT = 0x0;
		REG_SIOCNT = 0x1008;
		REG_SIOCNT |= 0x4000;
		REG_SIOCNT &=~1;
		REG_SIOCNT |= 0x0080;
	}
}
