#include <gba_base.h>

#define UNBCD(x) (((x) & 0xF) + (((x) >> 4) * 10))
#define _BCD(x) ((((x) / 10)<<4) + ((x) % 10))
#define RTC_DATA ((vu16 *)0x080000C4)
#define RTC_RW ((vu16 *)0x080000C6)
#define RTC_ENABLE ((vu16 *)0x080000C8)
#define CART_NAME ((vu8 *)0x080000A0)
#define RTC_CMD_READ(x) (((x)<<1) | 0x61)
#define RTC_CMD_WRITE(x) (((x)<<1) | 0x60)

// --------------------------------------------------------------------
void rtc_enable(void);
void rtc_disenable(void);
void rtc_cmd(int v);
void rtc_data(int v);
int rtc_read(void);
int rtc_get(u8 *data);
int rtc_gettime(u8 *data);
void rtc_set(u8 *data);
