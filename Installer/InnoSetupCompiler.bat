@ECHO OFF


REM ===============================================================================
SET ISS_FILE=%1
REM ===============================================================================

IF EXIST "%ProgramFiles%\Inno Setup 5\compil32.exe" "%ProgramFiles%\Inno Setup 5\compil32.exe" /cc %ISS_FILE%
IF %ERRORLEVEL% NEQ 0 GOTO ERROR

IF EXIST "%ProgramFiles(x86)%\Inno Setup 5\compil32.exe" "%ProgramFiles(x86)%\Inno Setup 5\compil32.exe" /cc %ISS_FILE%
IF %ERRORLEVEL% NEQ 0 GOTO ERROR
  
REM ===============================================================================
REM - Finish the script in ok status
:END
EXIT /B 0
REM ===============================================================================


REM ===============================================================================
REM - Finish the script in error status
:ERROR
EXIT /B 1
REM ===============================================================================