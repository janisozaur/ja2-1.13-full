# Microsoft Developer Studio Generated NMAKE File, Based on 30frames.dsp
!IF "$(CFG)" == ""
CFG=30frames - Win32 Debug
!MESSAGE Keine Konfiguration angegeben. 30frames - Win32 Debug wird als Standard verwendet.
!ENDIF 

!IF "$(CFG)" != "30frames - Win32 Release" && "$(CFG)" != "30frames - Win32 Debug"
!MESSAGE UngÅltige Konfiguration "$(CFG)" angegeben.
!MESSAGE Sie kînnen beim AusfÅhren von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "30frames.mak" CFG="30frames - Win32 Debug"
!MESSAGE 
!MESSAGE FÅr die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "30frames - Win32 Release" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE "30frames - Win32 Debug" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE 
!ERROR Eine ungÅltige Konfiguration wurde angegeben.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "30frames - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\30frames.exe"


CLEAN :
	-@erase "$(INTDIR)\30frames.obj"
	-@erase "$(INTDIR)\30frames.pch"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\30frames.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\30frames.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\30frames.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\30frames.pdb" /machine:I386 /out:"$(OUTDIR)\30frames.exe" 
LINK32_OBJS= \
	"$(INTDIR)\30frames.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\30frames.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "30frames - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\30frames.exe" "$(OUTDIR)\30frames.pch"


CLEAN :
	-@erase "$(INTDIR)\30frames.obj"
	-@erase "$(INTDIR)\30frames.pch"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\30frames.exe"
	-@erase "$(OUTDIR)\30frames.ilk"
	-@erase "$(OUTDIR)\30frames.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\30frames.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\30frames.pdb" /debug /machine:I386 /out:"$(OUTDIR)\30frames.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\30frames.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\30frames.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

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


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("30frames.dep")
!INCLUDE "30frames.dep"
!ELSE 
!MESSAGE Warning: cannot find "30frames.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "30frames - Win32 Release" || "$(CFG)" == "30frames - Win32 Debug"
SOURCE=.\30frames.cpp

"$(INTDIR)\30frames.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\30frames.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "30frames - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\30frames.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\30frames.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "30frames - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\30frames.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\30frames.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

