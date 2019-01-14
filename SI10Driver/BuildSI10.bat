rem @echo off

rem Structure
rem Param 1 DDK Root
rem Param 2 root
rem Param 3 Driver Folder
rem Param 4 free or checked

echo ---
echo *******************************************************
echo *	This Process will:                                  *
echo *		Build the Driver                                   *
echo *		Copy the Sys File to the Drivers Directory         *
echo *                                                     *
echo *******************************************************

echo ---
echo *** Setting up environment variables ***
echo ---
if "%1"=="" goto usage
if %3=="" goto usage
if not exist %1\bin\setenv.bat goto usage

copy /Y BuildDriver.bat  %3\BuildDriver.bat
call %1\bin\setenv %1 %4 %5
%2
cd %3

echo ---
echo *** Building Driver ***
echo ---

Call BuildDriver.bat 9030 clean

%2
cd %3

Call BuildDriver.bat 9030

echo ---
echo *** Copying Driver to Directory Folder ***
echo ---

if "%4"=="checked" goto checked
if "%4"=="free" goto free

:checked
 echo *** Copying Checked driver ***
 copy /Y %3\Driver_Plx9030\chk\i386\plx9030.sys  %WinDir%\system32\drivers\SI10.sys
 Exit


:free
 echo *** Copying Free driver ***
 copy /Y %3\Driver_Plx9030\fre\i386\plx9030.sys  %WinDir%\system32\drivers\SI10.sys
 Exit


:usage
echo usage   BuildSI10 DDK_dir Root_Dir Driver_Dir free/checked [build_options]
echo eg      BuildSI10 %%DDKROOT%% C: %%WDMBOOK%% free -cef
:exit
Exit