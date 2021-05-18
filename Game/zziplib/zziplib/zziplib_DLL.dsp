# Microsoft Developer Studio Project File - Name="zziplib_DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=zziplib_DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "zziplib_DLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "zziplib_DLL.mak" CFG="zziplib_DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "zziplib_DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zziplib_DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "zziplib_DLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release/zziplib_DLL"
# PROP Intermediate_Dir "Release/zziplib_DLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ZZIPLIB_DLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ZZIP_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib /nologo /dll /machine:I386 /out:"Release/zziplib_DLL/zziplib.dll"

!ELSEIF  "$(CFG)" == "zziplib_DLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug/zziplib_DLL"
# PROP Intermediate_Dir "Debug/zziplib_DLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ZZIPLIB_DLL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ZZIP_DLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib /nologo /dll /debug /machine:I386 /out:"Debug/zziplib_DLL/zziplib.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "zziplib_DLL - Win32 Release"
# Name "zziplib_DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "c;cpp"
# Begin Source File

SOURCE=".\zzip-dir.c"
# End Source File
# Begin Source File

SOURCE=".\zzip-err.c"
# End Source File
# Begin Source File

SOURCE=".\zzip-file.c"
# End Source File
# Begin Source File

SOURCE=".\zzip-info.c"
# End Source File
# Begin Source File

SOURCE=".\zzip-io.c"
# End Source File
# Begin Source File

SOURCE=".\zzip-stat.c"
# End Source File
# Begin Source File

SOURCE=".\zzip-zip.c"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=".\zzip-conf.h"
# End Source File
# Begin Source File

SOURCE=".\zzip-config.h"
# End Source File
# Begin Source File

SOURCE=".\zzip-file.h"
# End Source File
# Begin Source File

SOURCE=".\zzip-io.h"
# End Source File
# Begin Source File

SOURCE=".\zzip-msvc.h"
# End Source File
# Begin Source File

SOURCE=".\zzip-stdint.h"
# End Source File
# Begin Source File

SOURCE=.\zzip.h
# End Source File
# Begin Source File

SOURCE=.\zzipformat.h
# End Source File
# Begin Source File

SOURCE=.\zziplib.h
# End Source File
# End Group
# End Target
# End Project
