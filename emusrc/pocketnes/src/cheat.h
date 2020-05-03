#ifndef __CHEAT_H__
#define __CHEAT_H__

#if CHEATFINDER

#ifdef __cplusplus
extern "C" {
#endif

extern int cheatfinderstate;
extern int num_cheats;
static const int MAX_CHEATS = 50; // 900/18
extern u8 compare_value;
extern short found_for[7];

extern u32 *cheatfinder_bits; //size 320
extern u8 *cheatfinder_values; //size 10240
extern u8 *cheatfinder_cheats; //size 900?

#define print_cheatfinder_line(xxxx,yyyy) row=print_cheatfinder_line_func(row,(xxxx),(yyyy))
#define print_cheatfinder_line_newold(xxxx,yyyy) row=print_cheatfinder_line_newold_func(row,(xxxx),(yyyy))

extern const fptr fnlist5[];

int print_cheatfinder_line_func(int row, const char *oper, int value);
int print_cheatfinder_line_newold_func(int row, const char *oper, int value);
void drawui5(void);
void ui5(void);
void update_cheatfinder_tally(void);
void reset_cheatfinder(void);
void cheat_memcopy(void);
void write_byte(u8 *address, u8 data);
int add_cheat(u16 address, u8 value);
char* real_address(u16 addr);
void do_cheats(void);
void do_cheat_test(u32 testfunc);
void cf_equal(void);
void cf_notequal(void);
void cf_greater(void);
void cf_less(void);
void cf_greaterequal(void);
void cf_lessequal(void);
void cf_comparewith(void);
void cf_update(void);
void cf_newsearch(void);
int cf_next_result(int i);
int cf_result(int n);
void cf_drawresults(void);
void cf_results(void);
void cf_drawedit(int center);
void edit_cheat(int cheatnum);
void delete_cheat(int i);
void cf_editcheats(void);







#endif

#ifdef __cplusplus
}
#endif

#endif
