# Microsoft Developer Studio Generated NMAKE File, Based on PseudoMIPS.dsp
!IF "$(CFG)" == ""
CFG=PseudoMIPS - Win32 Debug
!MESSAGE Keine Konfiguration angegeben. PseudoMIPS - Win32 Debug wird als Standard verwendet.
!ENDIF 

!IF "$(CFG)" != "PseudoMIPS - Win32 Release" && "$(CFG)" != "PseudoMIPS - Win32 Debug"
!MESSAGE UngÅltige Konfiguration "$(CFG)" angegeben.
!MESSAGE Sie kînnen beim AusfÅhren von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "PseudoMIPS.mak" CFG="PseudoMIPS - Win32 Debug"
!MESSAGE 
!MESSAGE FÅr die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "PseudoMIPS - Win32 Release" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE "PseudoMIPS - Win32 Debug" (basierend auf  "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "PseudoMIPS - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\PseudoMIPS.exe"


CLEAN :
	-@erase "$(INTDIR)\cpuclock.obj"
	-@erase "$(INTDIR)\PseudoMIPS.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\PseudoMIPS.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\PseudoMIPS.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PseudoMIPS.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\PseudoMIPS.pdb" /machine:I386 /out:"$(OUTDIR)\PseudoMIPS.exe" 
LINK32_OBJS= \
	"$(INTDIR)\PseudoMIPS.obj" \
	"$(INTDIR)\cpuclock.obj"

"$(OUTDIR)\PseudoMIPS.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PseudoMIPS - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\PseudoMIPS.exe"


CLEAN :
	-@erase "$(INTDIR)\cpuclock.obj"
	-@erase "$(INTDIR)\PseudoMIPS.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\PseudoMIPS.exe"
	-@erase "$(OUTDIR)\PseudoMIPS.ilk"
	-@erase "$(OUTDIR)\PseudoMIPS.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "UNICODE" /D "_UNICODE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ  /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PseudoMIPS.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\PseudoMIPS.pdb" /debug /machine:I386 /out:"$(OUTDIR)\PseudoMIPS.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\PseudoMIPS.obj" \
	"$(INTDIR)\cpuclock.obj"

"$(OUTDIR)\PseudoMIPS.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("PseudoMIPS.dep")
!INCLUDE "PseudoMIPS.dep"
!ELSE 
!MESSAGE Warning: cannot find "PseudoMIPS.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "PseudoMIPS - Win32 Release" || "$(CFG)" == "PseudoMIPS - Win32 Debug"
SOURCE=.\cpuclock.cpp

"$(INTDIR)\cpuclock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PseudoMIPS.pch"


SOURCE=.\PseudoMIPS.cpp

"$(INTDIR)\PseudoMIPS.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\PseudoMIPS.pch"



!ENDIF 

