@REM [[ Change dir to input directory and link to grit ]]
@%~d1
@cd %~dp1
@set GRIT=%~dp0grit.exe

@REM [[ run grit ]]
%GRIT% %1

@pause