@Echo off
set PATH=C:\devkitPro\msys2\usr\bin;C:\devkitPro\devkitARM\bin;%PATH%
set DEVKITARM=/c/devkitPro/devkitARM
set DEVKITPRO=/c/devkitPro
set LIBGBA=/c/devkitPro/libgba

make  
rem >error.txt 2>&1
pause
REM build.bat