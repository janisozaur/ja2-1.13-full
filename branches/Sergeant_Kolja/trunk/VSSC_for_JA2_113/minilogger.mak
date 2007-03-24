# Microsoft Developer Studio Generated NMAKE File, Based on minilogger.dsp
!IF "$(CFG)" == ""
CFG=minilogger - Win32 Debug
!MESSAGE Keine Konfiguration angegeben. minilogger - Win32 Debug wird als Standard verwendet.
!ENDIF 

!IF "$(CFG)" != "minilogger - Win32 Release" && "$(CFG)" != "minilogger - Win32 Debug"
!MESSAGE UngÅltige Konfiguration "$(CFG)" angegeben.
!MESSAGE Sie kînnen beim AusfÅhren von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "minilogger.mak" CFG="minilogger - Win32 Debug"
!MESSAGE 
!MESSAGE FÅr die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "minilogger - Win32 Release" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE "minilogger - Win32 Debug" (basierend auf  "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "minilogger - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\minilogger.exe"


CLEAN :
	-@erase "$(INTDIR)\minilogger.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VSSC.obj"
	-@erase "$(OUTDIR)\minilogger.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\minilogger.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib ws2_32.lib advapi32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\minilogger.pdb" /machine:I386 /out:"$(OUTDIR)\minilogger.exe" 
LINK32_OBJS= \
	"$(INTDIR)\minilogger.obj" \
	"$(INTDIR)\VSSC.obj"

"$(OUTDIR)\minilogger.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "minilogger - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\minilogger.exe"


CLEAN :
	-@erase "$(INTDIR)\minilogger.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VSSC.obj"
	-@erase "$(OUTDIR)\minilogger.exe"
	-@erase "$(OUTDIR)\minilogger.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\minilogger.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib shell32.lib ws2_32.lib advapi32.lib /nologo /subsystem:console /profile /map:"$(INTDIR)\minilogger.map" /debug /machine:I386 /out:"$(OUTDIR)\minilogger.exe" 
LINK32_OBJS= \
	"$(INTDIR)\minilogger.obj" \
	"$(INTDIR)\VSSC.obj"

"$(OUTDIR)\minilogger.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("minilogger.dep")
!INCLUDE "minilogger.dep"
!ELSE 
!MESSAGE Warning: cannot find "minilogger.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "minilogger - Win32 Release" || "$(CFG)" == "minilogger - Win32 Debug"
SOURCE=.\minilogger.cpp

"$(INTDIR)\minilogger.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\VSSC.cpp

"$(INTDIR)\VSSC.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

