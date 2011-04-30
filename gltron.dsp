# Microsoft Developer Studio Project File - Name="gltron" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=gltron - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gltron.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gltron.mak" CFG="gltron - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gltron - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "gltron - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gltron - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D PREF_DIR=\".\" /D SNAP_DIR=\".\" /D DATA_DIR=\".\" /D SEPARATOR='\\' /D VERSION=\"0.70alpha1\" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib sdlmain.lib sdl.lib zlib.lib libpng.lib sdl_sound.lib msvcrt.lib /nologo /subsystem:windows /incremental:yes /machine:I386 /nodefaultlib
# SUBTRACT LINK32 /pdb:none /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Release\gltron.exe gltron.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./src/include" /I "./nebu/include" /I "./lua5/include" /D "_WINDOWS" /D PREF_DIR=\".\" /D SNAP_DIR=\".\" /D DATA_DIR=\".\" /D LOCAL_DATA=\"\" /D SEPARATOR='\\' /D "WIN32" /D "_DEBUG" /D "_MBCS" /D VERSION=\"0.63alpha1\" /D SEPERATOR='\\' /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib glew32.lib opengl32.lib SDLmain.lib sdl.lib libpng.lib libz.lib sdl_sound.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Debug\gltron.exe gltron.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "gltron - Win32 Release"
# Name "gltron - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\video\artpack.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\camera.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\computer.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\computer_utilities.c
# End Source File
# Begin Source File

SOURCE=.\src\game\credits.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\src\filesystem\dirsetup-win32.c"

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\engine.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\event.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\explosion.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\fonts.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\game.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\game_level.c
# End Source File
# Begin Source File

SOURCE=.\src\video\gamegraphics.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\globals.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\gltron.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\graphics_fx.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\graphics_hud.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\graphics_lights.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\graphics_utility.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\graphics_world.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\gui.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\init.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\input\input.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\level.c
# End Source File
# Begin Source File

SOURCE=.\src\video\load_texture.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\material.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\menu.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\model.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\filesystem\path.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\pause.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\recognizer.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\screenshot.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\scripting_interface.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\configuration\settings.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\skybox.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\audio\sound.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\audio\sound_glue.cpp

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\switchCallbacks.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\texture.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\game\timedemo.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\trail.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\trail_geometry.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\trail_render.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\base\util.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\video.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\video\visuals_2d.c

!IF  "$(CFG)" == "gltron - Win32 Release"

# ADD CPP /I "./src/include" /I "./nebu/include" /I "./lua/include"

!ELSEIF  "$(CFG)" == "gltron - Win32 Debug"

# ADD CPP /I "./lua/include"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\include\audio\audio.h
# End Source File
# Begin Source File

SOURCE=.\src\include\game\camera.h
# End Source File
# Begin Source File

SOURCE=.\src\include\game\client_data.h
# End Source File
# Begin Source File

SOURCE=.\src\include\configuration\configuration.h
# End Source File
# Begin Source File

SOURCE=.\src\include\game\data.h
# End Source File
# Begin Source File

SOURCE=.\src\include\filesystem\dirsetup.h
# End Source File
# Begin Source File

SOURCE=.\src\include\game\engine.h
# End Source File
# Begin Source File

SOURCE=.\src\include\game\event.h
# End Source File
# Begin Source File

SOURCE=.\src\include\video\explosion.h
# End Source File
# Begin Source File

SOURCE=.\src\include\video\fonttex.h
# End Source File
# Begin Source File

SOURCE=.\src\include\game\game.h
# End Source File
# Begin Source File

SOURCE=.\src\include\game\game_data.h
# End Source File
# Begin Source File

SOURCE=.\src\include\game\gltron.h
# End Source File
# Begin Source File

SOURCE=.\src\include\video\graphics_fx.h
# End Source File
# Begin Source File

SOURCE=.\src\include\video\graphics_hud.h
# End Source File
# Begin Source File

SOURCE=.\src\include\video\graphics_lights.h
# End Source File
# Begin Source File

SOURCE=.\src\include\video\graphics_utility.h
# End Source File
# Begin Source File

SOURCE=.\src\include\video\graphics_world.h
# End Source File
# Begin Source File

SOURCE=.\src\include\game\init.h
# End Source File
# Begin Source File

SOURCE=.\src\include\input\input.h
# End Source File
# Begin Source File

SOURCE=.\src\include\game\menu.h
# End Source File
# Begin Source File

SOURCE=.\src\include\video\model.h
# End Source File
# Begin Source File

SOURCE=.\src\include\filesystem\path.h
# End Source File
# Begin Source File

SOURCE=.\src\include\video\recognizer.h
# End Source File
# Begin Source File

SOURCE=.\src\include\scripting\scripting.h
# End Source File
# Begin Source File

SOURCE=.\src\include\configuration\settings.h
# End Source File
# Begin Source File

SOURCE=.\src\include\video\skybox.h
# End Source File
# Begin Source File

SOURCE=.\src\include\audio\sound_glue.h
# End Source File
# Begin Source File

SOURCE=.\src\include\base\switchCallbacks.h
# End Source File
# Begin Source File

SOURCE=.\src\include\video\texture.h
# End Source File
# Begin Source File

SOURCE=.\src\include\game\timedemo.h
# End Source File
# Begin Source File

SOURCE=.\src\include\game\timesystem.h
# End Source File
# Begin Source File

SOURCE=.\src\include\video\trail_geometry.h
# End Source File
# Begin Source File

SOURCE=.\src\include\base\util.h
# End Source File
# Begin Source File

SOURCE=.\src\include\video\video.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
