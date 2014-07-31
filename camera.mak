# Microsoft Developer Studio Generated NMAKE File, Based on camera.dsp
!IF $(CFG)" == "
CFG=camera - Win32 Debug
!MESSAGE No configuration specified. Defaulting to camera - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "camera - Win32 Release" && "$(CFG)" != "camera - Win32 Debug"
!MESSAGE 指定的配置 "$(CFG)" 无效.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "camera.mak" CFG="camera - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "camera - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "camera - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF $(OS)" == "Windows_NT
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "camera - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# 开始自定义宏
OutDir=.\Release
# 结束自定义宏

ALL : "$(OUTDIR)\camera.exe"


CLEAN :
	-@erase "$(INTDIR)\camera.obj"
	-@erase "$(INTDIR)\camera.pch"
	-@erase "$(INTDIR)\camera.res"
	-@erase "$(INTDIR)\cameraDlg.obj"
	-@erase "$(INTDIR)\hand.obj"
	-@erase "$(INTDIR)\HelpDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\camera.exe"
	-@erase "$(OUTDIR)\camera.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\camera.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\camera.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\camera.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=vfw32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\camera.pdb" /debug /machine:I386 /out:"$(OUTDIR)\camera.exe" 
LINK32_OBJS= \
	"$(INTDIR)\camera.obj" \
	"$(INTDIR)\cameraDlg.obj" \
	"$(INTDIR)\hand.obj" \
	"$(INTDIR)\HelpDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\camera.res"

"$(OUTDIR)\camera.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "camera - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# 开始自定义宏
OutDir=.\Debug
# 结束自定义宏

ALL : "$(OUTDIR)\camera.exe"


CLEAN :
	-@erase "$(INTDIR)\camera.obj"
	-@erase "$(INTDIR)\camera.pch"
	-@erase "$(INTDIR)\camera.res"
	-@erase "$(INTDIR)\cameraDlg.obj"
	-@erase "$(INTDIR)\hand.obj"
	-@erase "$(INTDIR)\HelpDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\camera.exe"
	-@erase "$(OUTDIR)\camera.ilk"
	-@erase "$(OUTDIR)\camera.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /O2 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\camera.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\camera.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\camera.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=vfw32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\camera.pdb" /debug /machine:I386 /out:"$(OUTDIR)\camera.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\camera.obj" \
	"$(INTDIR)\cameraDlg.obj" \
	"$(INTDIR)\hand.obj" \
	"$(INTDIR)\HelpDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\camera.res"

"$(OUTDIR)\camera.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("camera.dep")
!INCLUDE "camera.dep"
!ELSE 
!MESSAGE Warning: cannot find "camera.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "camera - Win32 Release" || "$(CFG)" == "camera - Win32 Debug"
SOURCE=.\camera.cpp

"$(INTDIR)\camera.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\camera.pch"


SOURCE=.\camera.rc

"$(INTDIR)\camera.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\cameraDlg.cpp

"$(INTDIR)\cameraDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\camera.pch"


SOURCE=.\hand.cpp

"$(INTDIR)\hand.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\camera.pch"


SOURCE=.\HelpDlg.cpp

"$(INTDIR)\HelpDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\camera.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "camera - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\camera.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\camera.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "camera - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /O2 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\camera.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\camera.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

