// stdafx.h : Include-Datei f�r Standard-System-Include-Dateien,
//  oder projektspezifische Include-Dateien, die h�ufig benutzt, aber
//      in unregelm��igen Abst�nden ge�ndert werden.
//

#if !defined(AFX_STDAFX_H__4D631555_259F_4508_A81E_5744C6D36DE9__INCLUDED_)
#define AFX_STDAFX_H__4D631555_259F_4508_A81E_5744C6D36DE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// ZU ERLEDIGEN: Verweisen Sie hier auf zus�tzliche Header-Dateien, die Ihr Programm ben�tigt

#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>

#define MIN(a,b) ((a<b)?(a):(b))
#define MAX(a,b) ((a>b)?(a):(b))
#define DIM(a)   ( sizeof((a))/sizeof((a[0])) )


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt zus�tzliche Deklarationen unmittelbar vor der vorherigen Zeile ein.

#endif // !defined(AFX_STDAFX_H__4D631555_259F_4508_A81E_5744C6D36DE9__INCLUDED_)
