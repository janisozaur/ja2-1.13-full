/*
 * ChangeLog:
 *  10.12.2005 Lesh ripped out everything that refers to MSS
 *  10.12.2005 Lesh added fmod.h
 */ 
#ifndef __JA2_SGP_ALL_H
#define __JA2_SGP_ALL_H

#pragma message("GENERATED PCH FOR JA2 SGP PROJECT.")

//#ifndef INITGUID
//	#define INITGUID
//#endif


#include "word_wrap.h"
#include "video.h"
#include "button_sound_control.h"
#include "sound_control.h"
#ifdef _JA2_RENDER_DIRTY
	#include "font_control.h"
	#include "render_dirty.h"
	#include "utilities.h"
#endif
#include "input.h"
#include "memman.h"
#include "english.h"
#include "vobject.h"
#include "vobject_blitters.h"
#include "sound_man.h"
#include "button_system.h"
#include "line.h"
#include <stdarg.h>
#include "debug.h"
#include "types.h"
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#if _MSC_VER < 1300 //(iostream.h was removed from VC.NET2003)
#include <iostream.h>
#endif
#include "cursor_control.h"
#include "wcheck.h"
#include "file_man.h"
#include "db_man.h"
#include "topic_ids.h"
#include "topic_ops.h"
#include "screen_ids.h"
#include "sys_globals.h"
#include "ja_screens.h"
#include "game_loop.h"
#include <direct.h>
#include "library_database.h"
#include "io.h"
#include <wchar.h>
#include "sgp.h"
#include "pcx.h"
#include "font.h"
#include "image.h"
#include <math.h>
#include <string.h>
#include "imp_tga.h"
#include "stci.h"
#include <memory.h>
#include "local.h"
#include <tchar.h>
#include <assert.h>
#include	"game_settings.h"
#ifdef _DEBUG
	#include <crtdbg.h>
#endif
#include "mouse_system.h"
#include "Random.h"
#include "vobject_private.h"
#include "shading.h"
#include "img_fmt.h"
#include "render_world.h"
#include "Isometric utils.h"
#include "fade screen.h"
#include "timer control.h"
#include "vsurface.h"
#include "vsurface_private.h"
#include "timer_control.h"

// Lesh modifications
#include "fmod.h"
#include "fmod_errors.h"
#include "SDL.h"

#endif
