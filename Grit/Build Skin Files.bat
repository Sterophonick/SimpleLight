@Echo off & SetLocal EnableExtensions DisableDelayedExpansion
CD "%~dp0"
if not exist "Config.ini" goto :make_config
For /f "tokens=2,* delims==" %%a in ('findstr /b /i /l "Skin_Folder" "Config.ini"') do Set "source=%%a\"
For /f "tokens=2,* delims==" %%a in ('findstr /b /i /l "Image_Folder" "Config.ini"') do Set "image_sizes=%%a\image_sizes.h"
Del /Q "%image_sizes%" >NUL
For /f "tokens=*" %%a in ('Dir /b /n "%source%\*.bmp"') do (
	Set input=%%~na
	SetLocal EnableDelayedExpansion
		Echo Input: %%a
		grit.exe "!source!\!input!.bmp" -gu8 -gb -gB16 -ftc -s "gImage_!input!___"
		Del /Q !input!.h
		Call repl.bat "___Bitmap" "" L < "!input!.c" >"!input!1" & Del "!input!.c" & rename "!input!1" "!input!.h"
		Call repl.bat "___" "" L < "!input!.h" >"!input!1" & Del "!input!.h" & rename "!input!1" "!input!.h"
		For /f "tokens=2 delims=[]" %%b in ('findstr /i /l "gImage_!input!" "!input!.h"') do (
			Echo extern const unsigned char __attribute__^(^(aligned^(4^)^)^)gImage_!input![%%b]^; >>"!image_sizes!"
		)
		Move /y "!input!.h" "!source!\" >NUL
	endlocal
)
pause
exit
:make_config
(
Echo Skin_Folder=..\source\images\dark
Echo Image_Folder=..\source\images
)>Config.ini