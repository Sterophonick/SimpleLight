#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include <stdlib.h>
#include <gba_base.h>
#include <gba_dma.h>
#include <string.h>

#include "RTC.h"
// --------------------------------------------------------------------
void rtc_enable(void)
{
	*RTC_ENABLE = 1;
}
// --------------------------------------------------------------------
void rtc_disenable(void)
{
	*RTC_ENABLE = 0;
}
// --------------------------------------------------------------------
void rtc_cmd(int v)
{
	int l;
	u16 b;
	v = v<<1;
	for(l=7; l>=0; l--)
	{
		b = (v>>l) & 0x2;
		*RTC_DATA = b | 4;
		*RTC_DATA = b | 4;
		*RTC_DATA = b | 4;
		*RTC_DATA = b | 5;
	}
}
// --------------------------------------------------------------------
void rtc_data(int v)
{
	int l;
	u16 b;
	v = v<<1;
	for(l=0; l<8; l++)
	{
		b = (v>>l) & 0x2;
		*RTC_DATA = b | 4;
		*RTC_DATA = b | 4;
		*RTC_DATA = b | 4;
		*RTC_DATA = b | 5;
	}
}
// --------------------------------------------------------------------
int rtc_read(void)
{
	int j,l;
	u16 b;
	int v = 0;
	for(l=0; l<8; l++)
	{
		for(j=0;j<5; j++)
			*RTC_DATA = 4;
		*RTC_DATA = 5;
		b = *RTC_DATA;
		v = v | ((b & 2)<<l);
	}
	v = v>>1;
	return v;
}
// --------------------------------------------------------------------
int rtc_get(u8 *data)
{
	int i;
	*RTC_DATA = 1;
	*RTC_RW = 7;
	*RTC_DATA = 1;
	*RTC_DATA = 5;
	rtc_cmd(RTC_CMD_READ(2));
	*RTC_RW = 5;
	for(i=0; i<4; i++)
		data[i] = (u8)rtc_read();
	*RTC_RW = 5;
	for(i=4; i<7; i++)
		data[i] = (u8)rtc_read();
	return 0;
}
// --------------------------------------------------------------------
int rtc_gettime(u8 *data)
{
	int i;
	*RTC_DATA = 1;
	*RTC_RW = 7;
	*RTC_DATA = 1;
	*RTC_DATA = 5;
	rtc_cmd(RTC_CMD_READ(3));
	*RTC_RW = 5;
	for(i=0; i<3; i++)
		data[i] = (u8)rtc_read();
	return 0;
}
// --------------------------------------------------------------------
void rtc_set(u8 *data) 
{
	int i; 
	u8 newdata[7];
	
	for(i=0;i<7;i++) {
		newdata[i] = _BCD(data[i]);
	}
	
	*RTC_ENABLE = 1;
	*RTC_DATA = 1;
	*RTC_DATA = 5;
	*RTC_RW = 7;
	rtc_cmd(RTC_CMD_WRITE(2));
	for(i=0;i<4;i++) {
		rtc_data(newdata[i]);
	}
	for(i=4;i<7;i++) {
		rtc_data(newdata[i]);
	}
}
