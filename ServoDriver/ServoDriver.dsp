# Microsoft Developer Studio Project File - Name="ServoDriver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=ServoDriver - Win32 Free
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ServoDriver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ServoDriver.mak" CFG="ServoDriver - Win32 Free"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ServoDriver - Win32 Checked" (based on "Win32 (x86) External Target")
!MESSAGE "ServoDriver - Win32 Free" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "ServoDriver - Win32 Checked"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Checked"
# PROP BASE Intermediate_Dir "Checked"
# PROP BASE Cmd_Line "nmake /f "ServoDriver.mak""
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "ServoDriver.exe"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Checked"
# PROP Intermediate_Dir "Checked"
# PROP Cmd_Line "MakeDrvr %DDKROOT% c: c:\WindowsProjects\GenericScanner\ServoDriver\sys checked"
# PROP Rebuild_Opt "-nmake /a"
# PROP Target_File "ServoDriver.exe"
# PROP Bsc_Name "ServoDriver.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "ServoDriver - Win32 Free"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Free"
# PROP BASE Intermediate_Dir "Free"
# PROP BASE Cmd_Line "nmake /f "ServoDriver.mak""
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "ServoDriver.exe"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Free"
# PROP Intermediate_Dir "Free"
# PROP Cmd_Line "C:\WDMBook\MakeDrvr %DDKROOT% c: c:\WindowsProjects\USLScanner\ServoDriver\sys free"
# PROP Rebuild_Opt "-nmake /a"
# PROP Target_File "USLServo.sys"
# PROP Bsc_Name "obj\i386\free\ServoDriver.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "ServoDriver - Win32 Checked"
# Name "ServoDriver - Win32 Free"

!IF  "$(CFG)" == "ServoDriver - Win32 Checked"

!ELSEIF  "$(CFG)" == "ServoDriver - Win32 Free"

!ENDIF 

# Begin Group "Build"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sys\Sources
# End Source File
# End Group
# Begin Group "Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sys\DebugPrint.c
# End Source File
# Begin Source File

SOURCE=.\sys\DebugPrint.h
# End Source File
# Begin Source File

SOURCE=.\sys\DeviceIo.cpp
# End Source File
# Begin Source File

SOURCE=.\sys\Dispatch.cpp
# End Source File
# Begin Source File

SOURCE=.\sys\Init.cpp
# End Source File
# Begin Source File

SOURCE=.\sys\ServoDriver.h
# End Source File
# Begin Source File

SOURCE=.\sys\ServoDriver.rc
# End Source File
# Begin Source File

SOURCE=.\sys\ServoIoctl.h
# End Source File
# End Group
# End Target
# End Project
