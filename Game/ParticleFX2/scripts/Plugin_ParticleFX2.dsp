# Microsoft Developer Studio Project File - Name="Plugin_ParticleFX2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Plugin_ParticleFX2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Plugin_ParticleFX2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Plugin_ParticleFX2.mak" CFG="Plugin_ParticleFX2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Plugin_ParticleFX2 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Plugin_ParticleFX2 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/S&S/DEV/Ogrenew/Plugin_ParticleFX2", OGDAAAAA"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Plugin_ParticleFX2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin\Release"
# PROP Intermediate_Dir "..\obj\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_ParticleFX_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\..\..\OgreMain\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_ParticleFX_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x418 /d "NDEBUG"
# ADD RSC /l 0x418 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib OgreMain.lib /nologo /dll /machine:I386 /libpath:"..\..\..\OgreMain\lib\Release"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy ..\bin\Release\*.dll ..\..\..\Samples\Common\bin\Release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Plugin_ParticleFX2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin\Debug"
# PROP Intermediate_Dir "..\obj\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_ParticleFX_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\..\..\OgreMain\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUGIN_ParticleFX_EXPORTS" /YX /FD /GZ /Zm500 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x418 /d "_DEBUG"
# ADD RSC /l 0x418 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib OgreMain_d.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\OgreMain\lib\Debug"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy ..\bin\Debug\*.dll ..\..\..\Samples\Common\bin\Debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Plugin_ParticleFX2 - Win32 Release"
# Name "Plugin_ParticleFX2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\OgreAreaEmitter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OgreBoxEmitter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OgreColourFaderAffector.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OgreColourFaderAffector2.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OgreColourImageAffector.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OgreColourInterpolatorAffector.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OgreCylinderEmitter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OgreEllipsoidEmitter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OgreHollowEllipsoidEmitter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OgreLinearForceAffector.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OgreParticleFX.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\OgreParticleFXDll.def
# End Source File
# Begin Source File

SOURCE=..\src\OgrePointEmitter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OgreRingEmitter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OgreRotationAffector.cpp
# End Source File
# Begin Source File

SOURCE=..\src\OgreScaleAffector.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\OgreAreaEmitter.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreBoxEmitter.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreBoxEmitterFactory.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreColourFaderAffector.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreColourFaderAffector2.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreColourFaderAffectorFactory.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreColourFaderAffectorFactory2.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreColourImageAffector.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreColourImageAffectorFactory.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreColourInterpolatorAffector.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreColourInterpolatorAffectorFactory.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreCylinderEmitter.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreCylinderEmitterFactory.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreEllipsoidEmitter.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreEllipsoidEmitterFactory.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreHollowEllipsoidEmitter.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreHollowEllipsoidEmitterFactory.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreLinearForceAffector.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreLinearForceAffectorFactory.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreParticleFXPrerequisites.h
# End Source File
# Begin Source File

SOURCE=..\include\OgrePointEmitter.h
# End Source File
# Begin Source File

SOURCE=..\include\OgrePointEmitterFactory.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreRingEmitter.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreRingEmitterFactory.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreRotationAffector.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreRotationAffectorFactory.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreScaleAffector.h
# End Source File
# Begin Source File

SOURCE=..\include\OgreScaleAffectorFactory.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
