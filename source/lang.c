#include "lang.h"

char* gl_init_error;
char* gl_power_off;
char* gl_init_ok;
char* gl_Loading;
char* gl_file_overflow;

char* gl_menu_btn;
char* gl_lastest_game;

char* gl_writing;

char* gl_time;
char* gl_Mon;
char* gl_Tues;
char* gl_Wed;
char* gl_Thur;
char* gl_Fri;
char* gl_Sat;
char* gl_Sun;

char* gl_addon;
char* gl_reset;
char* gl_rts;
char* gl_sleep;
char* gl_cheat;

char* gl_hot_key;
char* gl_hot_key2;

char* gl_language;
char* gl_en_lang;
char* gl_zh_lang;
char* gl_set_btn;
char* gl_ok_btn;

char* gl_formatnor_info;

char* gl_check_sav;
char* gl_make_sav;

char* gl_check_RTS;
char* gl_make_RTS;

char* gl_check_pat;
char* gl_make_pat;

char* gl_loading_game;

char* gl_engine;
char* gl_use_engine;

char*  gl_recently_play;

char* gl_START_help;
char* gl_SELECT_help;
char* gl_L_A_help;
char* gl_LSTART_help;
char* gl_online_manual;

char* gl_no_game_played;

char* gl_ingameRTC;
char* gl_offRTC_powersave;
//--
char**  gl_rom_menu;
char**  gl_nor_op;


//中文
const char zh_init_error[]="TF卡初始化失败";
const char zh_power_off[]="关机";
const char zh_init_ok[]="TF卡初始化成功";
const char zh_Loading[]="加载中...";
const char zh_file_overflow[]="文件太大,不能加载";

const char zh_menu_btn[]=" [B]取消    [A]确定";
const char zh_writing[]="正在写...";
const char zh_lastest_game[]="请选择最后一个游戏";

const char zh_time[] ="     时间";
const char zh_Mon[]="一";
const char zh_Tues[]="二";
const char zh_Wed[]="三";
const char zh_Thur[]="四";
const char zh_Fri[]="五";
const char zh_Sat[]="六";
const char zh_Sun[]="日";

const char zh_addon[]="     功能";
const char zh_reset[]="软复位";
const char zh_rts[]="即时存档";
const char zh_sleep[]="睡眠";
const char zh_cheat[]="金手指";

const char zh_hot_key[]=" 睡眠热键";
const char zh_hot_key2[]=" 菜单热键";

const char zh_language[]=" LANGUAGE";
const char zh_lang[]=" 中文";

const char zh_set_btn[]="设置";
const char zh_ok_btn[]="保存";
const char zh_formatnor_info[]="确定?大约4分钟";

const char zh_check_sav[]="检查SAV文件";
const char zh_make_sav[]="创建SAV文件";

const char zh_check_RTS[]="检查RTS文件";
const char zh_make_RTS[]="创建RTS文件";

const char zh_check_pat[]="检查PAT文件";
const char zh_make_pat[]="创建PAT文件";

const char zh_loading_game[]="加载游戏";

const char zh_engine[]="     引擎";
const char zh_use_engine[]="快速补丁引擎";

const char zh_recently_play[]="最近游戏列表";

const char zh_START_help[]="打开最近游戏列表";
const char zh_SELECT_help[]="缩略图开关";
const char zh_L_A_help[]="冷启动";
const char zh_LSTART_help[]="删除文件";
const char zh_online_manual[]="  在线说明书";

const char zh_no_game_played[]="还没玩过游戏";

const char zh_ingameRTC[]=" 游戏时钟";
const char zh_offRTC_powersave[]="关闭可以节能";


const char *zh_rom_menu[]={
	"直接启动",
	"启动带辅助",
	"烧录到NOR",
	"烧录到NOR带辅助",
	"存档类型",
	"金手指",
};
const char *zh_nor_op[3]={
	"直接运行",
	"删除",
	"全部格式化",
};



//英文
const char en_init_error[]="Failed to initialize Micro SD card.";
const char en_power_off[]="Power off";
const char en_init_ok[]="Micro SD card initial OK";
const char en_Loading[]="Loading...";
const char en_file_overflow[]="The file is too big.";

const char en_menu_btn[]="(B) No      (A) OK";
const char en_writing[]="Writing...";
const char en_lastest_game[]="Select the lastest";

const char en_time[]="     Time";
const char en_Mon[]="Mon";
const char en_Tues[]="Tue";
const char en_Wed[]="Wed";
const char en_Thur[]="Thu";
const char en_Fri[]="Fri";
const char en_Sat[]="Sat";
const char en_Sun[]="Sun";

const char en_addon[]="    Addon";
const char en_reset[]="Reset";
const char en_rts[]="Savestate";
const char en_sleep[]="Sleep";
const char en_cheat[]="Cheat";

const char en_hot_key[] ="Sleep key";
const char en_hot_key2[]=" Menu key";

const char en_language[]=" Language";
const char en_lang[]="English";
const char en_set_btn[]="Set";
const char en_ok_btn[]=" OK";
const char en_formatnor_info[]="Sure?about 4 mins";

const char en_check_sav[]="Checking SAV file";
const char en_make_sav[] ="Creating SAV file";

const char en_check_RTS[]="Checking RTS file";
const char en_make_RTS[] ="Creating RTS file";

const char en_check_pat[]="Checking PAT file";
const char en_make_pat[] ="Creating PAT file";

const char en_loading_game[]="Loading Game";

const char en_engine[]="   Engine";
const char en_use_engine[]="Fast Patch Engine";

const char en_recently_play[]="Recent Played";

const char en_START_help[]="Open recently played list";
const char en_SELECT_help[]="Toggle thumbnail";
const char en_L_A_help[]="Multiboot";
const char en_LSTART_help[]="Delete file";
const char en_online_manual[]="Online manual";

const char en_no_game_played[]="No game played yet";

const char en_ingameRTC[]=" Game RTC";
const char en_offRTC_powersave[]="Turn off to Powersave";

const char *en_rom_menu[] = {
	"Clean boot",
	"Boot with addon",
	"Write to NOR clean",
	"Write to NOR addon",
	"Save type",
	"Cheat",
};
const char *en_nor_op[3]={
	"Direct boot",
	"Delete",
	"Format all",
};	

//---------------------------------------------------------------------------------
void LoadChinese(void)
{
	gl_init_error = (char*)zh_init_error;
	gl_power_off = (char*)zh_power_off;
	gl_init_ok = (char*)zh_init_ok;
	gl_Loading = (char*)zh_Loading;
	gl_file_overflow = (char*)zh_file_overflow;

	gl_menu_btn = (char*)zh_menu_btn;
	gl_writing = (char*)zh_writing;
	gl_lastest_game = (char*)zh_lastest_game;
	
	
	gl_time = (char*)zh_time;	
	gl_Mon = (char*)zh_Mon;
	gl_Tues = (char*)zh_Tues;
	gl_Wed = (char*)zh_Wed;
	gl_Thur = (char*)zh_Thur;
	gl_Fri = (char*)zh_Fri;
	gl_Sat = (char*)zh_Sat;
	gl_Sun = (char*)zh_Sun;

	gl_addon = (char*)zh_addon;
	gl_reset = (char*)zh_reset;
	gl_rts = (char*)zh_rts;
	gl_sleep = (char*)zh_sleep;
	gl_cheat = (char*)zh_cheat;	
	
	gl_hot_key = (char*)zh_hot_key;
	gl_hot_key2 = (char*)zh_hot_key2;

	gl_language =  (char*)zh_language;
	gl_en_lang = (char*)en_lang;
	gl_zh_lang = (char*)zh_lang;;
	gl_set_btn = (char*)zh_set_btn;
	gl_ok_btn = (char*)zh_ok_btn;
	gl_formatnor_info = (char*)zh_formatnor_info;

	gl_check_sav = (char*)zh_check_sav;
	gl_make_sav = (char*)zh_make_sav;
		
	gl_check_RTS = (char*)zh_check_RTS;
	gl_make_RTS = (char*)zh_make_RTS;
	
	gl_check_pat = (char*)zh_check_pat;
	gl_make_pat = (char*)zh_make_pat;
	
	gl_loading_game = (char*)zh_loading_game;
	gl_engine = (char*)zh_engine;
	gl_use_engine = (char*)zh_use_engine;
	
	gl_recently_play = (char*)zh_recently_play;

	gl_START_help = (char*)zh_START_help;
	gl_SELECT_help = (char*)zh_SELECT_help;
	gl_L_A_help = (char*)zh_L_A_help;
	gl_LSTART_help = (char*)zh_LSTART_help;
	gl_online_manual = (char*)zh_online_manual;
	
	gl_no_game_played = (char*)zh_no_game_played;
	
	gl_ingameRTC = (char*)zh_ingameRTC;
	gl_offRTC_powersave = (char*)zh_offRTC_powersave;
	//
	gl_rom_menu = (char**)zh_rom_menu;
	gl_nor_op = (char**)zh_nor_op;

}
//---------------------------------------------------------------------------------
void LoadEnglish(void)
{
	gl_init_error = (char*)en_init_error;
	gl_power_off = (char*)en_power_off;
	gl_init_ok = (char*)en_init_ok;
	gl_Loading = (char*)en_Loading;
	gl_file_overflow = (char*)en_file_overflow;

	gl_menu_btn = (char*)en_menu_btn;
	gl_writing = (char*)en_writing;
	gl_lastest_game = (char*)en_lastest_game;
	
	gl_time = (char*)en_time;	
	gl_Mon = (char*)en_Mon;
	gl_Tues = (char*)en_Tues;
	gl_Wed = (char*)en_Wed;
	gl_Thur = (char*)en_Thur;
	gl_Fri = (char*)en_Fri;
	gl_Sat = (char*)en_Sat;
	gl_Sun = (char*)en_Sun;
	gl_addon = (char*)en_addon;
	gl_reset = (char*)en_reset;
	gl_rts = (char*)en_rts;
	gl_sleep = (char*)en_sleep;
	gl_cheat = (char*)en_cheat;	
	
	gl_hot_key = (char*)en_hot_key;
	gl_hot_key2 = (char*)en_hot_key2;
	
	gl_language =  (char*)en_language;
	gl_en_lang = (char*)en_lang;
	gl_zh_lang = (char*)zh_lang;;
	gl_set_btn = (char*)en_set_btn;
	gl_ok_btn = (char*)en_ok_btn;
	gl_formatnor_info = (char*)en_formatnor_info;

	gl_check_sav = (char*)en_check_sav;
	gl_make_sav = (char*)en_make_sav;
		
	gl_check_RTS = (char*)en_check_RTS;
	gl_make_RTS = (char*)en_make_RTS;
	
	gl_check_pat = (char*)en_check_pat;
	gl_make_pat = (char*)en_make_pat;
	
	gl_loading_game = (char*)en_loading_game;
	
	gl_engine = (char*)en_engine;
	gl_use_engine = (char*)en_use_engine;
	
	gl_recently_play = (char*)en_recently_play;
	
	gl_START_help = (char*)en_START_help;
	gl_SELECT_help = (char*)en_SELECT_help;
	gl_L_A_help = (char*)en_L_A_help;
	gl_LSTART_help = (char*)en_LSTART_help;
	gl_online_manual = (char*)en_online_manual;
	
	gl_no_game_played = (char*)en_no_game_played;
	
	gl_ingameRTC = (char*)en_ingameRTC;
	gl_offRTC_powersave = (char*)en_offRTC_powersave;
	//
	gl_rom_menu = (char**)en_rom_menu;
	gl_nor_op = (char**)en_nor_op;
}
