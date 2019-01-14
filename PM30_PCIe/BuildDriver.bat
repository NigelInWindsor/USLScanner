@echo off


rem **************************************************************
rem *
rem * File       :  BuildDriver.bat
rem *
rem * Description:  Used to build PLX device drivers
rem *
rem **************************************************************


rem ====================================================================
rem  Clean environment from any previous failed builds
rem ====================================================================
SET PLX_CHIP=
IF NOT "%PLX_BUILD_ALT_DIR%" == "" SET BUILD_ALT_DIR=%PLX_BUILD_ALT_DIR%
SET PLX_BUILD_ALT_DIR=


rem ====================================================================
rem  Perform any error checks
rem ====================================================================
IF "%1" == "9050" SET PLX_CHIP=9050
IF "%1" == "9030" SET PLX_CHIP=9030
IF "%1" == "9080" SET PLX_CHIP=9080
IF "%1" == "9054" SET PLX_CHIP=9054
IF "%1" == "9056" SET PLX_CHIP=9056
IF "%1" == "9656" SET PLX_CHIP=9656
IF "%1" == "8311" SET PLX_CHIP=8311

rem Flag that a 9000-series device is building
IF NOT "%PLX_CHIP%" == "" SET b9000=%PLX_CHIP%

IF "%1" == "6000" SET PLX_CHIP=6000
IF "%1" == "8000" SET PLX_CHIP=8000
IF "%1" == "Svc"  SET PLX_CHIP=Svc
IF NOT "%PLX_CHIP%" == "" GOTO EnvOk

echo.
echo PLX Windows driver build batch file
echo Copyright (c) PLX Technology, Inc.
echo.
echo   Usage: builddriver {PLX_Chip} [CleanOption]
echo.
echo      PLX_Chip    = 6000       : PLX 6254/6540/6466 NT-mode PnP driver
echo                    8000       : PLX 8000 NT-mode PnP driver
echo                    9050       : PLX 9050/9052 PnP driver
echo                    9030       : PLX 9030 PnP driver
echo                    9080       : PLX 9080 PnP driver
echo                    9054       : PLX 9054 PnP driver
echo                    9056       : PLX 9056 PnP driver
echo                    9656       : PLX 9656 PnP driver
echo                    8311       : PLX 8311 PnP driver
echo                    Svc        : PLX PCI/PCIe Service driver
echo.
echo      CleanOption = {none}     : Build the driver
echo                    'clean'    : Remove intermediate build files
echo                    'cleanall' : Remove all build files
GOTO Exit


:ErrorSourceDir
echo.
echo ERROR: Source directory "Source.%CMN%" does not exist, unable to continue
GOTO Exit


:EnvOk


rem ====================================================================
rem  Set target architecture
rem ====================================================================
SET ARCH=%_BUILDARCH%
if "%_BUILDARCH%" == "x86" SET ARCH=i386


rem ====================================================================
rem  Override object folder for 9000 devices
rem ====================================================================
IF "%b9000%" == "" Goto _SkipObj9000
SET PLX_BUILD_ALT_DIR=%BUILD_ALT_DIR%
rem TOM CHNAGED TO BELOW 12/09/08 SET BUILD_ALT_DIR=_Plx%PLX_CHIP%_%BUILD_ALT_DIR%
SET BUILD_ALT_DIR=%PLX_CHIP%_%BUILD_ALT_DIR%
:_SkipObj9000


rem ====================================================================
rem  Switch to correct source directory
rem ====================================================================
IF "%b9000%" == "" (SET CMN=Plx%PLX_CHIP%) ELSE (SET CMN=Plx9000)
rem IF NOT EXIST Source.%CMN% GOTO ErrorSourceDir
rem CD Source.%CMN%
SET CMN=


rem ====================================================================
rem  Display parameters
rem ====================================================================
IF "%PLX_AUTO_BLD%" == "" CLS
IF NOT "%2" == "" goto _SkipNotice

echo.
echo   ********************************************************************
echo   * NOTE:  Building of PLX drivers has been tested with various
echo   *        versions of the Windows DDK/WDK, 32-bit and 64-bit (AMD64)
echo   *        environments. IA64 has not been tested and is not supported.
echo   ********************************************************************

SET ARCH=i386

:_SkipNotice
echo.
IF     "%PLX_CHIP%" == "Svc" echo    - TYP: PLX PCI/PCIe Service Driver
IF NOT "%PLX_CHIP%" == "Svc" echo    - TYP: WDM Driver
echo    - CPU: %ARCH%
echo    - CFG: %DDKBUILDENV%
echo    - PLX: %PLX_CHIP%
echo.


rem ====================================================================
rem  Check for command-line parameters
rem ====================================================================
IF "%2" == "c"        goto Clean
IF "%2" == "C"        goto Clean
IF "%2" == "clean"    goto Clean
IF "%2" == "a"        goto CleanAll
IF "%2" == "A"        goto CleanAll
IF "%2" == "cleanall" goto CleanAll


rem ====================================================================
rem  Create necessary directories
rem ====================================================================
IF NOT EXIST Driver_Plx%PLX_CHIP%  Md Driver_Plx%PLX_CHIP%
IF NOT EXIST %ARCH%                Md %ARCH%


rem ====================================================================
rem  Copy chip-specific files for 9000 devices
rem ====================================================================
IF "%b9000%" == "" Goto _SkipCopy9000
echo Copying chip-specific files...
IF EXIST Chip\%PLX_CHIP%\*.c  XCopy Chip\%PLX_CHIP%\*.c  %ARCH% /D /Q >NUL
echo.
:_SkipCopy9000

echo build alt dir is %BUILD_ALT_DIR%
rem ====================================================================
rem  Build driver
rem ====================================================================
build  -ZbwegI %1
IF ERRORLEVEL == 1 GOTO Cleanup



rem ====================================================================
rem  Remove log files if error-free build
rem ====================================================================
IF EXIST Build%BUILD_ALT_DIR%.*  Del Build%BUILD_ALT_DIR%.*


rem ====================================================================
rem  Digitally sign the driver if PLX internal build
rem ====================================================================
IF NOT EXIST ..\..\..\..\PlxDriverSign goto _DriverSign_Skip

echo      ______________________________________________________________
echo.
echo DRIVER SIGN: Driver_Plx%PLX_CHIP%\%DDKBUILDENV%\%_BUILDARCH%\Plx%PLX_CHIP%.sys



rem Build command-line in environment variable
SET PLX_SIGN=/v /ac ..\..\..\..\PlxDriverSign\PLX_Tech.cer /n "PLX Technology, Inc."
SET PLX_SIGN=%PLX_SIGN% /d "PLX SDK driver" /t http://timestamp.verisign.com/scripts/timestamp.dll 
SET PLX_SIGN=%PLX_SIGN% Driver_Plx%PLX_CHIP%\%DDKBUILDENV%\%ARCH%\Plx%PLX_CHIP%.sys

Signtool sign %PLX_SIGN% > NUL
IF ERRORLEVEL == 0 SET PLX_SIGN=Complete
IF ERRORLEVEL == 1 SET PLX_SIGN=ERROR - Unable to sign driver
IF ERRORLEVEL == 2 SET PLX_SIGN=WARNING - Unable to sign driver
echo DRIVER SIGN: %PLX_SIGN%

IF "%PLX_SIGN%" == "Complete" goto _DriverSign_Skip
echo DRIVER SIGN: Verify PLX certificate (PFX file) is added to certificate store

rem Add error information to log file
echo. >> Build%BUILD_ALT_DIR%.err
echo ============================================== >> Build%BUILD_ALT_DIR%.err
echo  PLX DRIVER SIGN: %PLX_SIGN%                   >> Build%BUILD_ALT_DIR%.err
echo ============================================== >> Build%BUILD_ALT_DIR%.err
echo. >> Build%BUILD_ALT_DIR%.err

:_DriverSign_Skip
echo      ______________________________________________________________
SET PLX_SIGN=


rem ====================================================================
rem  Build complete, goto cleanup
rem ====================================================================
goto Cleanup


:CleanAll
rem ====================================================================
rem  Delete all build files if cleaning
rem ====================================================================
echo Deleting: All build files
IF EXIST Driver_Plx%PLX_CHIP%  Rd /q /s Driver_Plx%PLX_CHIP%
echo Deleting: ...finished!
echo.


:Clean
rem ====================================================================
rem  Delete intermediate build files if cleaning
rem ====================================================================
echo Deleting: Intermediate build files
IF EXIST Obj                    Rd /q /s Obj
IF EXIST Obj%BUILD_ALT_DIR%     Rd /q /s Obj%BUILD_ALT_DIR%
IF EXIST Build%BUILD_ALT_DIR%.* Del Build%BUILD_ALT_DIR%.*
echo Deleting: ...finished!


:Cleanup
rem Remove common files to avoid multiple versions
IF EXIST %ARCH%  Rd /q /s %ARCH%

rem Go back to original directory
CD ..


:Exit
rem ====================================================================
rem  Cleanup environment
rem ====================================================================
IF NOT "%PLX_BUILD_ALT_DIR%" == "" SET BUILD_ALT_DIR=%PLX_BUILD_ALT_DIR%
SET PLX_BUILD_ALT_DIR=
SET ARCH=
SET b9000=
SET PLX_CHIP=
echo.
