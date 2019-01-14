# Microsoft Developer Studio Project File - Name="PM30_PCIe" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=PM30_PCIe - Win32 Free
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PM30_PCIe.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PM30_PCIe.mak" CFG="PM30_PCIe - Win32 Free"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PM30_PCIe - Win32 Checked" (based on "Win32 (x86) External Target")
!MESSAGE "PM30_PCIe - Win32 Free" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "PM30_PCIe - Win32 Checked"

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
# PROP Cmd_Line "BuildSI10.bat %DDKROOT% c: c:\WindowsProjects\USLScanner\PM30_PCIe checked"
# PROP Rebuild_Opt "/a"
# PROP Target_File "PM30_PCIe.sys"
# PROP Bsc_Name "PM30_PCIe.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PM30_PCIe - Win32 Free"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Free"
# PROP BASE Intermediate_Dir "Free"
# PROP BASE Cmd_Line "buildDriver.bat 9030"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "PM30_PCIe.sys"
# PROP BASE Bsc_Name "PM30_PCIe.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Free"
# PROP Intermediate_Dir "Free"
# PROP Cmd_Line "C:\WDMBook\MakeDrvr %DDKROOT% c: c:\WindowsProjects\USLScannerV4.37\PM30_PCIe\ free"
# PROP Rebuild_Opt "-nmake /a"
# PROP Target_File "PM30_PCIe.sys"
# PROP Bsc_Name "PM30_PCIe.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "PM30_PCIe - Win32 Checked"
# Name "PM30_PCIe - Win32 Free"

!IF  "$(CFG)" == "PM30_PCIe - Win32 Checked"

!ELSEIF  "$(CFG)" == "PM30_PCIe - Win32 Free"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\pnp.c
# End Source File
# Begin Source File

SOURCE=.\PM30_PCIe.c
# End Source File
# Begin Source File

SOURCE=.\PM30_PCIe.h
# End Source File
# Begin Source File

SOURCE=.\PM30_PCIe.rc
# End Source File
# End Group
# Begin Group "Build"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Makefile
# End Source File
# Begin Source File

SOURCE=.\Sources
# End Source File
# End Group
# End Target
# End Project
