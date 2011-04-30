# Microsoft Developer Studio Project File - Name="nebu" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=nebu - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "nebu.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "nebu.mak" CFG="nebu - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "nebu - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "nebu - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "nebu - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./include" /I "../lua5/include" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D VERSION=\"0.71\" /D PATH_SEPARATOR='\\' /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Debug\nebu.lib nebu.lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "nebu - Win32 Release"
# Name "nebu - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\video\2d.c
# End Source File
# Begin Source File

SOURCE=.\audio\audio_system.c
# End Source File
# Begin Source File

SOURCE=.\video\camera.c
# End Source File
# Begin Source File

SOURCE=.\video\console.c

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\filesystem\directory-win32.c"

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\filesystem\file_io.c

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\filesystem\filesystem.c

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\filesystem\findpath.c

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\video\font.c
# End Source File
# Begin Source File

SOURCE=.\input\input_system.c

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

# ADD CPP /I "../lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\base\matrix.c

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include" /I "./include/base"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

# ADD CPP /I "./include/base"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\video\mesh.c
# End Source File
# Begin Source File

SOURCE=.\video\mesh_3ds.c
# End Source File
# Begin Source File

SOURCE=.\video\pixels.c

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\base\png_texture.c
# End Source File
# Begin Source File

SOURCE=.\base\quat.c
# End Source File
# Begin Source File

SOURCE=.\base\random.c

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

# ADD CPP /I "./include/base"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\video\scene.c
# End Source File
# Begin Source File

SOURCE=.\scripting\scripting.c

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

# ADD CPP /I "./include/base" /I "./include/scripting" /I "../lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\audio\SoundSystem.cpp

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\audio\Source.cpp

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\audio\Source3D.cpp

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\audio\SourceCopy.cpp

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\audio\SourceEngine.cpp

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\audio\SourceMusic.cpp

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\audio\SourceSample.cpp

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\base\spline.c
# End Source File
# Begin Source File

SOURCE=.\base\surface.c
# End Source File
# Begin Source File

SOURCE=.\base\system.c

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include" /I "./include/base"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

# ADD CPP /I "./include/base"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\input\system_keynames.c

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

# ADD CPP /I "../lua/include"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\base\util.c

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include" /I "./include/base"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

# ADD CPP /I "./include/base"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\base\vector.c

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include" /I "./include/base"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

# ADD CPP /I "./include/base"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\video\video_system.c

!IF  "$(CFG)" == "nebu - Win32 Release"

# ADD CPP /I "./include" /I "../lua/include"

!ELSEIF  "$(CFG)" == "nebu - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\Nebu.h
# End Source File
# Begin Source File

SOURCE=.\include\audio\nebu_audio_system.h
# End Source File
# Begin Source File

SOURCE=.\include\Nebu_base.h
# End Source File
# Begin Source File

SOURCE=.\include\base\nebu_callbacks.h
# End Source File
# Begin Source File

SOURCE=.\include\Nebu_configuration.h
# End Source File
# Begin Source File

SOURCE=.\include\video\nebu_console.h
# End Source File
# Begin Source File

SOURCE=.\include\video\nebu_extgl.h
# End Source File
# Begin Source File

SOURCE=.\include\filesystem\nebu_file_io.h
# End Source File
# Begin Source File

SOURCE=.\include\filesystem\nebu_filesystem.h
# End Source File
# Begin Source File

SOURCE=.\include\Nebu_filesystem.h
# End Source File
# Begin Source File

SOURCE=.\include\base\nebu_geom.h
# End Source File
# Begin Source File

SOURCE=.\include\Nebu_input.h
# End Source File
# Begin Source File

SOURCE=.\include\input\nebu_input_system.h
# End Source File
# Begin Source File

SOURCE=.\include\video\nebu_light.h
# End Source File
# Begin Source File

SOURCE=.\include\base\nebu_matrix.h
# End Source File
# Begin Source File

SOURCE=.\include\video\nebu_png_texture.h
# End Source File
# Begin Source File

SOURCE=.\include\video\nebu_quad.h
# End Source File
# Begin Source File

SOURCE=.\include\base\nebu_random.h
# End Source File
# Begin Source File

SOURCE=.\include\video\nebu_renderer_gl.h
# End Source File
# Begin Source File

SOURCE=.\include\Nebu_scripting.h
# End Source File
# Begin Source File

SOURCE=.\include\scripting\nebu_scripting.h
# End Source File
# Begin Source File

SOURCE=.\include\audio\nebu_Sound.h
# End Source File
# Begin Source File

SOURCE=.\include\audio\nebu_SoundSystem.h
# End Source File
# Begin Source File

SOURCE=.\include\audio\nebu_Source.h
# End Source File
# Begin Source File

SOURCE=.\include\audio\nebu_Source3D.h
# End Source File
# Begin Source File

SOURCE=.\include\audio\nebu_SourceCopy.h
# End Source File
# Begin Source File

SOURCE=.\include\audio\nebu_SourceEngine.h
# End Source File
# Begin Source File

SOURCE=.\include\audio\nebu_SourceMusic.h
# End Source File
# Begin Source File

SOURCE=.\include\audio\nebu_SourceSample.h
# End Source File
# Begin Source File

SOURCE=.\include\base\nebu_system.h
# End Source File
# Begin Source File

SOURCE=.\include\input\nebu_system_keynames.h
# End Source File
# Begin Source File

SOURCE=.\include\base\nebu_types.h
# End Source File
# Begin Source File

SOURCE=.\include\base\nebu_util.h
# End Source File
# Begin Source File

SOURCE=.\include\base\nebu_vector.h
# End Source File
# Begin Source File

SOURCE=.\include\base\nebu_Vector3.h
# End Source File
# Begin Source File

SOURCE=.\include\Nebu_video.h
# End Source File
# Begin Source File

SOURCE=.\include\video\nebu_video_system.h
# End Source File
# Begin Source File

SOURCE=.\include\video\nebu_video_types.h
# End Source File
# End Group
# End Target
# End Project
