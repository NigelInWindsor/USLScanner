# Microsoft Developer Studio Project File - Name="USLDriver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=USLDriver - Win32 Free
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "USLDriver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "USLDriver.mak" CFG="USLDriver - Win32 Free"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "USLDriver - Win32 Checked" (based on "Win32 (x86) External Target")
!MESSAGE "USLDriver - Win32 Free" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "USLDriver - Win32 Checked"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "USLDriver___Win32_Checked"
# PROP BASE Intermediate_Dir "USLDriver___Win32_Checked"
# PROP BASE Cmd_Line "nmake /f "USLDriver.mak""
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "USLDriver.exe"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "USLDriver___Win32_Checked"
# PROP Intermediate_Dir "USLDriver___Win32_Checked"
# PROP Cmd_Line "MakeDrvr %DDKROOT% c: c:\WindowsProjects\GenericScanner\USLDriver\sys checked"
# PROP Rebuild_Opt "-nmake /a"
# PROP Target_File "USLDriver.sys"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "USLDriver - Win32 Free"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "USLDriver___Win32_Free"
# PROP BASE Intermediate_Dir "USLDriver___Win32_Free"
# PROP BASE Cmd_Line "nmake /f "USLDriver.mak""
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "USLDriver.exe"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "USLDriver___Win32_Free"
# PROP Intermediate_Dir "USLDriver___Win32_Free"
# PROP Cmd_Line "C:\WDMBook\MakeDrvr %DDKROOT% c: c:\WindowsProjects\USLScanner\USLDriver\sys free"
# PROP Rebuild_Opt "-nmake /a"
# PROP Target_File "USL.sys"
# PROP Bsc_Name "obj\i386\free\USLDriver.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "USLDriver - Win32 Checked"
# Name "USLDriver - Win32 Free"

!IF  "$(CFG)" == "USLDriver - Win32 Checked"

!ELSEIF  "$(CFG)" == "USLDriver - Win32 Free"

!ENDIF 

# Begin Group "Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DebugPrint.c
# End Source File
# Begin Source File

SOURCE=.\DebugPrint.h
# End Source File
# Begin Source File

SOURCE=.\DeviceIo.cpp
# End Source File
# Begin Source File

SOURCE=.\Dispatch.cpp
# End Source File
# Begin Source File

SOURCE=.\Init.cpp
# End Source File
# Begin Source File

SOURCE=.\Pnp.cpp
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\USLDriver.h
# End Source File
# Begin Source File

SOURCE=.\USLDriver.rc
# End Source File
# Begin Source File

SOURCE=.\USLIoctl.h
# End Source File
# End Group
# Begin Group "Build"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MAKEFILE
# End Source File
# Begin Source File

SOURCE=.\Makefile.inc
# End Source File
# Begin Source File

SOURCE=.\Sources
# End Source File
# End Group
# End Target
# End Project
