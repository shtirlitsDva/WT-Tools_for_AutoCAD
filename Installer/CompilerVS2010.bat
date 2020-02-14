@ECHO OFF

REM ===============================================================================
REM - Variables Declaration
SET SOLUTION_PATH=%1
SET CONFIGURATION=%2
REM ===============================================================================


REM ===============================================================================
REM - Entering the VS Prompt
ECHO ===============================================================================
ECHO Entering the VS Prompt ...
ECHO ===============================================================================
IF EXIST "%ProgramFiles(x86)%\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" CALL "%ProgramFiles(x86)%\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
IF %ERRORLEVEL% NEQ 0 GOTO ERROR
IF EXIST "%ProgramFiles%\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" CALL "%ProgramFiles%\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
IF %ERRORLEVEL% NEQ 0 GOTO ERROR
ECHO.
REM ===============================================================================


REM ===============================================================================
REM - Cleaning Projects
ECHO ===============================================================================
ECHO Rebuilding Projects ...
ECHO ===============================================================================
MSBuild %SOLUTION_PATH% /t:Rebuild /p:Configuration=%CONFIGURATION%;PlatformTarget=anycpu
IF %ERRORLEVEL% NEQ 0 GOTO ERROR
REM ===============================================================================


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