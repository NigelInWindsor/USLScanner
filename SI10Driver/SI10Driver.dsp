# Microsoft Developer Studio Project File - Name="SI10Driver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=SI10Driver - Win32 Free
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SI10Driver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SI10Driver.mak" CFG="SI10Driver - Win32 Free"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SI10Driver - Win32 Checked" (based on "Win32 (x86) External Target")
!MESSAGE "SI10Driver - Win32 Free" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "SI10Driver - Win32 Checked"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Checked"
# PROP BASE Intermediate_Dir "Checked"
# PROP BASE Cmd_Line "buildDriver.bat 9030"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Checked"
# PROP Intermediate_Dir "Checked"
# PROP Cmd_Line "BuildSI10.bat %DDKROOT% c: c:\WindowsProjects\USLScanner\SI10Driver checked"
# PROP Rebuild_Opt "/a"
# PROP Target_File "SI10Driver.sys"
# PROP Bsc_Name "SI10Driver.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "SI10Driver - Win32 Free"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Free"
# PROP BASE Intermediate_Dir "Free"
# PROP BASE Cmd_Line "buildDriver.bat 9030"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "SI10Driver.sys"
# PROP BASE Bsc_Name "SI10Driver.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Free"
# PROP Intermediate_Dir "Free"
# PROP Cmd_Line "BuildSI10.bat %DDKROOT% c: c:\WindowsProjects\USLScannerV4.63\SI10Driver free"
# PROP Rebuild_Opt "/a"
# PROP Target_File "SI10Driver.sys"
# PROP Bsc_Name "SI10Driver.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "SI10Driver - Win32 Checked"
# Name "SI10Driver - Win32 Free"

!IF  "$(CFG)" == "SI10Driver - Win32 Checked"

!ELSEIF  "$(CFG)" == "SI10Driver - Win32 Free"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ApiFunc.c
# End Source File
# Begin Source File

SOURCE=.\ApiFunc.h
# End Source File
# Begin Source File

SOURCE=.\DebugPrint.c
# End Source File
# Begin Source File

SOURCE=.\DebugPrint.h
# End Source File
# Begin Source File

SOURCE=.\Dispatch.c
# End Source File
# Begin Source File

SOURCE=.\Dispatch.h
# End Source File
# Begin Source File

SOURCE=.\Driver.c
# End Source File
# Begin Source File

SOURCE=.\Driver.h
# End Source File
# Begin Source File

SOURCE=.\DriverVer.rc
# End Source File
# Begin Source File

SOURCE=.\DrvDefs.h
# End Source File
# Begin Source File

SOURCE=.\Eep_9000.c
# End Source File
# Begin Source File

SOURCE=.\Eep_9000.h
# End Source File
# Begin Source File

SOURCE=.\GlobalVars.c
# End Source File
# Begin Source File

SOURCE=.\GlobalVars.h
# End Source File
# Begin Source File

SOURCE=.\PciFunc.c
# End Source File
# Begin Source File

SOURCE=.\PciFunc.h
# End Source File
# Begin Source File

SOURCE=.\PlugPlay.c
# End Source File
# Begin Source File

SOURCE=.\PlugPlay.h
# End Source File
# Begin Source File

SOURCE=.\PlxChip.h
# End Source File
# Begin Source File

SOURCE=.\PlxChipApi.c
# End Source File
# Begin Source File

SOURCE=.\PlxChipApi.h
# End Source File
# Begin Source File

SOURCE=.\PlxChipFn.c
# End Source File
# Begin Source File

SOURCE=.\PlxChipFn.h
# End Source File
# Begin Source File

SOURCE=.\PlxInterrupt.c
# End Source File
# Begin Source File

SOURCE=.\PlxInterrupt.h
# End Source File
# Begin Source File

SOURCE=..\PolyCoordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Power.c
# End Source File
# Begin Source File

SOURCE=.\Power.h
# End Source File
# Begin Source File

SOURCE=.\Si10Ioctl.h
# End Source File
# Begin Source File

SOURCE=.\SuppFunc.c
# End Source File
# Begin Source File

SOURCE=.\SuppFunc.h
# End Source File
# End Group
# Begin Group "Build"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BuildDriver.bat
# End Source File
# Begin Source File

SOURCE=.\BuildSI10.bat
# End Source File
# Begin Source File

SOURCE=.\Makefile
# End Source File
# Begin Source File

SOURCE=..\..\..\WXPDDK\bin\setenv.bat
# End Source File
# Begin Source File

SOURCE=.\Sources
# End Source File
# End Group
# Begin Source File

SOURCE=..\2DPrimitives.h
# End Source File
# Begin Source File

SOURCE=.\Include\PlxIoctl.h
# End Source File
# Begin Source File

SOURCE=..\PolyCoordDlg.h
# End Source File
# Begin Source File

SOURCE=..\TubeScanPage.h
# End Source File
# End Target
# End Project
