# Microsoft Developer Studio Project File - Name="TaiwaneseResources" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TaiwaneseResources - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TaiwaneseResources.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TaiwaneseResources.mak" CFG="TaiwaneseResources - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TaiwaneseResources - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TaiwaneseResources - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TaiwaneseResources - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TaiwaneseResources_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TaiwaneseResources_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "TaiwaneseResources - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TaiwaneseResources_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TaiwaneseResources_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\Program Files\USL\USLScanner\TaiwaneseResources.dll" /pdbtype:sept /NOENTRY
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "TaiwaneseResources - Win32 Release"
# Name "TaiwaneseResources - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\USLScanner.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00014.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00016.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00017.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00018.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00019.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00020.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00021.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00022.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00023.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00024.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00025.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00026.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00027.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00028.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00029.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00030.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00031.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00032.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00033.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00034.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00035.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00036.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00037.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00038.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00039.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00040.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00041.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00042.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00043.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00044.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00045.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00046.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00047.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00048.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00049.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00050.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00051.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00052.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00053.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00054.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00055.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00056.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00057.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp3_axi.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00004.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00005.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00006.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00007.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00008.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00009.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00010.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00011.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00012.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00013.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00014.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\flat_sca.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hnwse.cur
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\operator.bmp
# End Source File
# Begin Source File

SOURCE=.\res\palette_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PlateScanner.ico
# End Source File
# Begin Source File

SOURCE=.\res\polycoor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\profile_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\profile_3D.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\usl.ico
# End Source File
# Begin Source File

SOURCE=.\res\USL_ICON1.ico
# End Source File
# Begin Source File

SOURCE=.\res\USLScanner.ico
# End Source File
# Begin Source File

SOURCE=.\res\vertical.bmp
# End Source File
# Begin Source File

SOURCE=.\res\view_arc.bmp
# End Source File
# End Group
# End Target
# End Project
