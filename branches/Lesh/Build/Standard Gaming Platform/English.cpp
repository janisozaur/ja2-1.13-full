#ifdef JA2_PRECOMPILED_HEADERS
	#include "JA2 SGP ALL.H"
#elif defined( WIZ8_PRECOMPILED_HEADERS )
	#include "WIZ8 SGP ALL.H"
#else
	#include "english.h"
#endif

UINT16 gsStandardKeyTable[] =
{
		//typedef enum {
		//	/* The keyboard syms have been cleverly chosen to map to ASCII */
		//	SDLK_UNKNOWN		= 0,
	0,	//	SDLK_FIRST		= 0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	BACKSPACE,	//	SDLK_BACKSPACE		= 8,
	TAB,	//	SDLK_TAB		= 9,
	10,
	11,
	12,	//	SDLK_CLEAR		= 12,
	ENTER,	//	SDLK_RETURN		= 13,
	14,
	15,
	SHIFT,
	CTRL,
	ALT,
	PAUSE,	//	SDLK_PAUSE		= 19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	ESC,	//	SDLK_ESCAPE		= 27,
	28,
	29,
	30,
	31,
	SPACE,	//	SDLK_SPACE		= 32,
	33,	//	SDLK_EXCLAIM		= 33,
	34,	//	SDLK_QUOTEDBL		= 34,
	35,	//	SDLK_HASH		= 35,
	36,	//	SDLK_DOLLAR		= 36,
	37,
	38,	//	SDLK_AMPERSAND		= 38,
	39,	//	SDLK_QUOTE		= 39,
	40,	//	SDLK_LEFTPAREN		= 40,
	41,	//	SDLK_RIGHTPAREN		= 41,
	42,	//	SDLK_ASTERISK		= 42,
	43,	//	SDLK_PLUS		= 43,
	44,	//	SDLK_COMMA		= 44,
	45,	//	SDLK_MINUS		= 45,
	46,	//	SDLK_PERIOD		= 46,
	47,	//	SDLK_SLASH		= 47,
	48,	//	SDLK_0			= 48,
	49,	//	SDLK_1			= 49,
	50,	//	SDLK_2			= 50,
	51,	//	SDLK_3			= 51,
	52,	//	SDLK_4			= 52,
	53,	//	SDLK_5			= 53,
	54,	//	SDLK_6			= 54,
	55,	//	SDLK_7			= 55,
	56,	//	SDLK_8			= 56,
	57,	//	SDLK_9			= 57,
	58,	//	SDLK_COLON		= 58,
	59,	//	SDLK_SEMICOLON		= 59,
	60,	//	SDLK_LESS		= 60,
	61,	//	SDLK_EQUALS		= 61,
	62,	//	SDLK_GREATER		= 62,
	63,	//	SDLK_QUESTION		= 63,
	64,	//	SDLK_AT			= 64,

	97,	//	
	98,	//	  Replace uppercase letters with lowercase
	99,	//	 
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	91,	//	SDLK_LEFTBRACKET	= 91,
	92,	//	SDLK_BACKSLASH		= 92,
	93,	//	SDLK_RIGHTBRACKET	= 93,
	94,	//	SDLK_CARET		= 94,
	95,	//	SDLK_UNDERSCORE		= 95,
	96,	//	SDLK_BACKQUOTE		= 96,
	97,	//	SDLK_a			= 97,
	98,	//	SDLK_b			= 98,
	99,	//	SDLK_c			= 99,
	100,	//	SDLK_d			= 100,
	101,	//	SDLK_e			= 101,
	102,	//	SDLK_f			= 102,
	103,	//	SDLK_g			= 103,
	104,	//	SDLK_h			= 104,
	105,	//	SDLK_i			= 105,
	106,	//	SDLK_j			= 106,
	107,	//	SDLK_k			= 107,
	108,	//	SDLK_l			= 108,
	109,	//	SDLK_m			= 109,
	110,	//	SDLK_n			= 110,
	111,	//	SDLK_o			= 111,
	112,	//	SDLK_p			= 112,
	113,	//	SDLK_q			= 113,
	114,	//	SDLK_r			= 114,
	115,	//	SDLK_s			= 115,
	116,	//	SDLK_t			= 116,
	117,	//	SDLK_u			= 117,
	118,	//	SDLK_v			= 118,
	119,	//	SDLK_w			= 119,
	120,	//	SDLK_x			= 120,
	121,	//	SDLK_y			= 121,
	122,	//	SDLK_z			= 122,
	123,
	124,
	125,
	126,
	DEL		//	SDLK_DELETE		= 127,
};

UINT16 gsExtendedKeyTable[] =
{
		//	/* Numeric keypad */
	INSERT,	//	SDLK_KP0		= 256,
	END,	//	SDLK_KP1		= 257,
	DNARROW,	//	SDLK_KP2		= 258,
	PGDN,	//	SDLK_KP3		= 259,
	LEFTARROW,	//	SDLK_KP4		= 260,
	261,	//	SDLK_KP5		= 261,
	RIGHTARROW,	//	SDLK_KP6		= 262,
	HOME,	//	SDLK_KP7		= 263,
	UPARROW,	//	SDLK_KP8		= 264,
	PGUP,	//	SDLK_KP9		= 265,
	DEL,	//	SDLK_KP_PERIOD		= 266,
	'/',	//	SDLK_KP_DIVIDE		= 267,
	'*',	//	SDLK_KP_MULTIPLY	= 268,
	'-',	//	SDLK_KP_MINUS		= 269,
	'+',	//	SDLK_KP_PLUS		= 270,
	ENTER,	//	SDLK_KP_ENTER		= 271,
	'=',	//	SDLK_KP_EQUALS		= 272,
		//
		//	/* Arrows + Home/End pad */
	UPARROW,	//	SDLK_UP			= 273,
	DNARROW,	//	SDLK_DOWN		= 274,
	RIGHTARROW,	//	SDLK_RIGHT		= 275,
	LEFTARROW,	//	SDLK_LEFT		= 276,
	INSERT,	//	SDLK_INSERT		= 277,
	HOME,	//	SDLK_HOME		= 278,
	END,	//	SDLK_END		= 279,
	PGUP,	//	SDLK_PAGEUP		= 280,
	PGDN,	//	SDLK_PAGEDOWN		= 281,
		//
		//	/* Function keys */
	F1,	//	SDLK_F1			= 282,
	F2,	//	SDLK_F2			= 283,
	F3,	//	SDLK_F3			= 284,
	F4,	//	SDLK_F4			= 285,
	F5,	//	SDLK_F5			= 286,
	F6,	//	SDLK_F6			= 287,
	F7,	//	SDLK_F7			= 288,
	F8,	//	SDLK_F8			= 289,
	F9,	//	SDLK_F9			= 290,
	F10,	//	SDLK_F10		= 291,
	F11,	//	SDLK_F11		= 292,
	F12,	//	SDLK_F12		= 293,
	294,	//	SDLK_F13		= 294,
	295,	//	SDLK_F14		= 295,
	296,	//	SDLK_F15		= 296,
	297,
	298,
	299,
		//
		//	/* Key state modifier keys */
	NUM_LOCK,	//	SDLK_NUMLOCK		= 300,
	CAPS,	//	SDLK_CAPSLOCK		= 301,
	SCRL_LOCK,	//	SDLK_SCROLLOCK		= 302,
	SHIFT,	//	SDLK_RSHIFT		= 303,
	SHIFT,	//	SDLK_LSHIFT		= 304,
	CTRL,	//	SDLK_RCTRL		= 305,
	CTRL,	//	SDLK_LCTRL		= 306,
	ALT,	//	SDLK_RALT		= 307,
	ALT,	//	SDLK_LALT		= 308,
	309,	//	SDLK_RMETA		= 309,
	310,	//	SDLK_LMETA		= 310,
	311,	//	SDLK_LSUPER		= 311,		/* Left "Windows" key */
	312,	//	SDLK_RSUPER		= 312,		/* Right "Windows" key */
	313,	//	SDLK_MODE		= 313,		/* "Alt Gr" key */
	314,	//	SDLK_COMPOSE		= 314,		/* Multi-key compose key */
		//
		//	/* Miscellaneous function keys */
	315,	//	SDLK_HELP		= 315,
	316,	//	SDLK_PRINT		= 316,
	317,	//	SDLK_SYSREQ		= 317,
	318,	//	SDLK_BREAK		= 318,
	319,	//	SDLK_MENU		= 319,
	320,	//	SDLK_POWER		= 320,		/* Power Macintosh power key */
	321,	//	SDLK_EURO		= 321,		/* Some european keyboards */
	322		//	SDLK_UNDO		= 322,		/* Atari keyboard has Undo */
		//
		//	/* Add any other keys here */
		//
		//	SDLK_LAST
		//} SDLKey;
};

UINT16 gsStandardShiftKeyTable[] =
{
		//typedef enum {
		//	/* The keyboard syms have been cleverly chosen to map to ASCII */
		//	SDLK_UNKNOWN		= 0,
	0,	//	SDLK_FIRST		= 0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	BACKSPACE,	//	SDLK_BACKSPACE		= 8,
	SHIFT_TAB,	//	SDLK_TAB		= 9,
	10,
	11,
	12,	//	SDLK_CLEAR		= 12,
	ENTER,	//	SDLK_RETURN		= 13,
	14,
	15,
	SHIFT,
	CTRL,
	ALT,
	PAUSE,	//	SDLK_PAUSE		= 19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	ESC,	//	SDLK_ESCAPE		= 27,
	28,
	29,
	30,
	31,
	SPACE,	//	SDLK_SPACE		= 32,
	33,	//	SDLK_EXCLAIM		= 33,
	34,	//	SDLK_QUOTEDBL		= 34,
	35,	//	SDLK_HASH		= 35,
	36,	//	SDLK_DOLLAR		= 36,
	37,
	38,	//	SDLK_AMPERSAND		= 38,
	39,	//	SDLK_QUOTE		= 39,
	40,	//	SDLK_LEFTPAREN		= 40,
	41,	//	SDLK_RIGHTPAREN		= 41,
	42,	//	SDLK_ASTERISK		= 42,
	43,	//	SDLK_PLUS		= 43,
	44,	//	SDLK_COMMA		= 44,
	45,	//	SDLK_MINUS		= 45,
	46,	//	SDLK_PERIOD		= 46,
	47,	//	SDLK_SLASH		= 47,
	')',	//	SDLK_0			= 48,
	'!',	//	SDLK_1			= 49,
	'@',	//	SDLK_2			= 50,
	'#',	//	SDLK_3			= 51,
	'$',	//	SDLK_4			= 52,
	'%',	//	SDLK_5			= 53,
	'^',	//	SDLK_6			= 54,
	'&',	//	SDLK_7			= 55,
	'*',	//	SDLK_8			= 56,
	'(',	//	SDLK_9			= 57,
	58,	//	SDLK_COLON		= 58,
	59,	//	SDLK_SEMICOLON		= 59,
	60,	//	SDLK_LESS		= 60,
	61,	//	SDLK_EQUALS		= 61,
	62,	//	SDLK_GREATER		= 62,
	63,	//	SDLK_QUESTION		= 63,
	64,	//	SDLK_AT			= 64,

	65,	//	/* 
	66,	//	   Skip uppercase letters
	67,	//	 */
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	83,
	84,
	85,
	86,
	87,
	88,
	89,
	90,
	91,	//	SDLK_LEFTBRACKET	= 91,
	92,	//	SDLK_BACKSLASH		= 92,
	93,	//	SDLK_RIGHTBRACKET	= 93,
	94,	//	SDLK_CARET		= 94,
	95,	//	SDLK_UNDERSCORE		= 95,
	96,	//	SDLK_BACKQUOTE		= 96,
	65,	//	SDLK_a			= 97,
	66,	//	SDLK_b			= 98,
	67,	//	SDLK_c			= 99,
	68,	//	SDLK_d			= 100,
	69,	//	SDLK_e			= 101,
	70,	//	SDLK_f			= 102,
	71,	//	SDLK_g			= 103,
	72,	//	SDLK_h			= 104,
	73,	//	SDLK_i			= 105,
	74,	//	SDLK_j			= 106,
	75,	//	SDLK_k			= 107,
	76,	//	SDLK_l			= 108,
	77,	//	SDLK_m			= 109,
	78,	//	SDLK_n			= 110,
	79,	//	SDLK_o			= 111,
	80,	//	SDLK_p			= 112,
	81,	//	SDLK_q			= 113,
	82,	//	SDLK_r			= 114,
	83,	//	SDLK_s			= 115,
	84,	//	SDLK_t			= 116,
	85,	//	SDLK_u			= 117,
	86,	//	SDLK_v			= 118,
	87,	//	SDLK_w			= 119,
	88,	//	SDLK_x			= 120,
	89,	//	SDLK_y			= 121,
	90,	//	SDLK_z			= 122,
	123,
	124,
	125,
	126,
	SHIFT_DELETE		//	SDLK_DELETE		= 127,
};

UINT16 gsStandardCtrlKeyTable[] =
{
		//typedef enum {
		//	/* The keyboard syms have been cleverly chosen to map to ASCII */
		//	SDLK_UNKNOWN		= 0,
	0,	//	SDLK_FIRST		= 0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	BACKSPACE,	//	SDLK_BACKSPACE		= 8,
	CTRL_TAB,	//	SDLK_TAB		= 9,
	10,
	11,
	12,	//	SDLK_CLEAR		= 12,
	ENTER,	//	SDLK_RETURN		= 13,
	14,
	15,
	SHIFT,
	CTRL,
	ALT,
	PAUSE,	//	SDLK_PAUSE		= 19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	ESC,	//	SDLK_ESCAPE		= 27,
	28,
	29,
	30,
	31,
	SPACE,	//	SDLK_SPACE		= 32,
	33,	//	SDLK_EXCLAIM		= 33,
	34,	//	SDLK_QUOTEDBL		= 34,
	35,	//	SDLK_HASH		= 35,
	36,	//	SDLK_DOLLAR		= 36,
	37,
	38,	//	SDLK_AMPERSAND		= 38,
	39,	//	SDLK_QUOTE		= 39,
	40,	//	SDLK_LEFTPAREN		= 40,
	41,	//	SDLK_RIGHTPAREN		= 41,
	42,	//	SDLK_ASTERISK		= 42,
	43,	//	SDLK_PLUS		= 43,
	44,	//	SDLK_COMMA		= 44,
	45,	//	SDLK_MINUS		= 45,
	46,	//	SDLK_PERIOD		= 46,
	47,	//	SDLK_SLASH		= 47,
	48,	//	SDLK_0			= 48,
	49,	//	SDLK_1			= 49,
	50,	//	SDLK_2			= 50,
	51,	//	SDLK_3			= 51,
	52,	//	SDLK_4			= 52,
	53,	//	SDLK_5			= 53,
	54,	//	SDLK_6			= 54,
	55,	//	SDLK_7			= 55,
	56,	//	SDLK_8			= 56,
	57,	//	SDLK_9			= 57,
	58,	//	SDLK_COLON		= 58,
	59,	//	SDLK_SEMICOLON		= 59,
	60,	//	SDLK_LESS		= 60,
	61,	//	SDLK_EQUALS		= 61,
	62,	//	SDLK_GREATER		= 62,
	63,	//	SDLK_QUESTION		= 63,
	64,	//	SDLK_AT			= 64,

	97,	//	/* 
	98,	//	   Skip uppercase letters
	99,	//	 */
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	91,	//	SDLK_LEFTBRACKET	= 91,
	92,	//	SDLK_BACKSLASH		= 92,
	93,	//	SDLK_RIGHTBRACKET	= 93,
	94,	//	SDLK_CARET		= 94,
	95,	//	SDLK_UNDERSCORE		= 95,
	96,	//	SDLK_BACKQUOTE		= 96,
	97,	//	SDLK_a			= 97,
	98,	//	SDLK_b			= 98,
	99,	//	SDLK_c			= 99,
	100,	//	SDLK_d			= 100,
	101,	//	SDLK_e			= 101,
	102,	//	SDLK_f			= 102,
	103,	//	SDLK_g			= 103,
	104,	//	SDLK_h			= 104,
	105,	//	SDLK_i			= 105,
	106,	//	SDLK_j			= 106,
	107,	//	SDLK_k			= 107,
	108,	//	SDLK_l			= 108,
	109,	//	SDLK_m			= 109,
	110,	//	SDLK_n			= 110,
	111,	//	SDLK_o			= 111,
	112,	//	SDLK_p			= 112,
	113,	//	SDLK_q			= 113,
	114,	//	SDLK_r			= 114,
	115,	//	SDLK_s			= 115,
	116,	//	SDLK_t			= 116,
	117,	//	SDLK_u			= 117,
	118,	//	SDLK_v			= 118,
	119,	//	SDLK_w			= 119,
	120,	//	SDLK_x			= 120,
	121,	//	SDLK_y			= 121,
	122,	//	SDLK_z			= 122,
	123,
	124,
	125,
	126,
	CTRL_DELETE		//	SDLK_DELETE		= 127,
};

UINT16 gsStandardAltKeyTable[] =
{
		//typedef enum {
		//	/* The keyboard syms have been cleverly chosen to map to ASCII */
		//	SDLK_UNKNOWN		= 0,
	0,	//	SDLK_FIRST		= 0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	BACKSPACE,	//	SDLK_BACKSPACE		= 8,
	ALT_TAB,	//	SDLK_TAB		= 9,
	10,
	11,
	12,	//	SDLK_CLEAR		= 12,
	ENTER,	//	SDLK_RETURN		= 13,
	14,
	15,
	SHIFT,
	CTRL,
	ALT,
	PAUSE,	//	SDLK_PAUSE		= 19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	ESC,	//	SDLK_ESCAPE		= 27,
	28,
	29,
	30,
	31,
	SPACE,	//	SDLK_SPACE		= 32,
	33,	//	SDLK_EXCLAIM		= 33,
	34,	//	SDLK_QUOTEDBL		= 34,
	35,	//	SDLK_HASH		= 35,
	36,	//	SDLK_DOLLAR		= 36,
	37,
	38,	//	SDLK_AMPERSAND		= 38,
	39,	//	SDLK_QUOTE		= 39,
	40,	//	SDLK_LEFTPAREN		= 40,
	41,	//	SDLK_RIGHTPAREN		= 41,
	42,	//	SDLK_ASTERISK		= 42,
	43,	//	SDLK_PLUS		= 43,
	44,	//	SDLK_COMMA		= 44,
	45,	//	SDLK_MINUS		= 45,
	46,	//	SDLK_PERIOD		= 46,
	47,	//	SDLK_SLASH		= 47,
	48,	//	SDLK_0			= 48,
	49,	//	SDLK_1			= 49,
	50,	//	SDLK_2			= 50,
	51,	//	SDLK_3			= 51,
	52,	//	SDLK_4			= 52,
	53,	//	SDLK_5			= 53,
	54,	//	SDLK_6			= 54,
	55,	//	SDLK_7			= 55,
	56,	//	SDLK_8			= 56,
	57,	//	SDLK_9			= 57,
	58,	//	SDLK_COLON		= 58,
	59,	//	SDLK_SEMICOLON		= 59,
	60,	//	SDLK_LESS		= 60,
	61,	//	SDLK_EQUALS		= 61,
	62,	//	SDLK_GREATER		= 62,
	63,	//	SDLK_QUESTION		= 63,
	64,	//	SDLK_AT			= 64,

	97,	//	/* 
	98,	//	   Skip uppercase letters
	99,	//	 */
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	91,	//	SDLK_LEFTBRACKET	= 91,
	92,	//	SDLK_BACKSLASH		= 92,
	93,	//	SDLK_RIGHTBRACKET	= 93,
	94,	//	SDLK_CARET		= 94,
	95,	//	SDLK_UNDERSCORE		= 95,
	96,	//	SDLK_BACKQUOTE		= 96,
	97,	//	SDLK_a			= 97,
	98,	//	SDLK_b			= 98,
	99,	//	SDLK_c			= 99,
	100,	//	SDLK_d			= 100,
	101,	//	SDLK_e			= 101,
	102,	//	SDLK_f			= 102,
	103,	//	SDLK_g			= 103,
	104,	//	SDLK_h			= 104,
	105,	//	SDLK_i			= 105,
	106,	//	SDLK_j			= 106,
	107,	//	SDLK_k			= 107,
	108,	//	SDLK_l			= 108,
	109,	//	SDLK_m			= 109,
	110,	//	SDLK_n			= 110,
	111,	//	SDLK_o			= 111,
	112,	//	SDLK_p			= 112,
	113,	//	SDLK_q			= 113,
	114,	//	SDLK_r			= 114,
	115,	//	SDLK_s			= 115,
	116,	//	SDLK_t			= 116,
	117,	//	SDLK_u			= 117,
	118,	//	SDLK_v			= 118,
	119,	//	SDLK_w			= 119,
	120,	//	SDLK_x			= 120,
	121,	//	SDLK_y			= 121,
	122,	//	SDLK_z			= 122,
	123,
	124,
	125,
	126,
	ALT_DELETE		//	SDLK_DELETE		= 127,
};

UINT16 gsExtendedShiftKeyTable[] =
{
		//	/* Numeric keypad */
	256,	//	SDLK_KP0		= 256,
	257,	//	SDLK_KP1		= 257,
	258,	//	SDLK_KP2		= 258,
	259,	//	SDLK_KP3		= 259,
	260,	//	SDLK_KP4		= 260,
	261,	//	SDLK_KP5		= 261,
	262,	//	SDLK_KP6		= 262,
	263,	//	SDLK_KP7		= 263,
	264,	//	SDLK_KP8		= 264,
	265,	//	SDLK_KP9		= 265,
	266,	//	SDLK_KP_PERIOD		= 266,
	267,	//	SDLK_KP_DIVIDE		= 267,
	268,	//	SDLK_KP_MULTIPLY	= 268,
	269,	//	SDLK_KP_MINUS		= 269,
	270,	//	SDLK_KP_PLUS		= 270,
	271,	//	SDLK_KP_ENTER		= 271,
	272,	//	SDLK_KP_EQUALS		= 272,
		//
		//	/* Arrows + Home/End pad */
	SHIFT_UPARROW,	//	SDLK_UP			= 273,
	SHIFT_DNARROW,	//	SDLK_DOWN		= 274,
	SHIFT_RIGHTARROW,	//	SDLK_RIGHT		= 275,
	SHIFT_LEFTARROW,	//	SDLK_LEFT		= 276,
	SHIFT_INSERT,	//	SDLK_INSERT		= 277,
	SHIFT_HOME,	//	SDLK_HOME		= 278,
	SHIFT_END,	//	SDLK_END		= 279,
	SHIFT_PGUP,	//	SDLK_PAGEUP		= 280,
	SHIFT_PGDN,	//	SDLK_PAGEDOWN		= 281,
		//
		//	/* Function keys */
	SHIFT_F1,	//	SDLK_F1			= 282,
	SHIFT_F2,	//	SDLK_F2			= 283,
	SHIFT_F3,	//	SDLK_F3			= 284,
	SHIFT_F4,	//	SDLK_F4			= 285,
	SHIFT_F5,	//	SDLK_F5			= 286,
	SHIFT_F6,	//	SDLK_F6			= 287,
	SHIFT_F7,	//	SDLK_F7			= 288,
	SHIFT_F8,	//	SDLK_F8			= 289,
	SHIFT_F9,	//	SDLK_F9			= 290,
	SHIFT_F10,	//	SDLK_F10		= 291,
	SHIFT_F11,	//	SDLK_F11		= 292,
	SHIFT_F12,	//	SDLK_F12		= 293,
	294,	//	SDLK_F13		= 294,
	295,	//	SDLK_F14		= 295,
	296,	//	SDLK_F15		= 296,
	297,
	298,
	299,
		//
		//	/* Key state modifier keys */
	NUM_LOCK,	//	SDLK_NUMLOCK		= 300,
	CAPS,	//	SDLK_CAPSLOCK		= 301,
	SCRL_LOCK,	//	SDLK_SCROLLOCK		= 302,
	SHIFT,	//	SDLK_RSHIFT		= 303,
	SHIFT,	//	SDLK_LSHIFT		= 304,
	CTRL,	//	SDLK_RCTRL		= 305,
	CTRL,	//	SDLK_LCTRL		= 306,
	ALT,	//	SDLK_RALT		= 307,
	ALT,	//	SDLK_LALT		= 308,
	309,	//	SDLK_RMETA		= 309,
	310,	//	SDLK_LMETA		= 310,
	311,	//	SDLK_LSUPER		= 311,		/* Left "Windows" key */
	312,	//	SDLK_RSUPER		= 312,		/* Right "Windows" key */
	313,	//	SDLK_MODE		= 313,		/* "Alt Gr" key */
	314,	//	SDLK_COMPOSE		= 314,		/* Multi-key compose key */
		//
		//	/* Miscellaneous function keys */
	315,	//	SDLK_HELP		= 315,
	316,	//	SDLK_PRINT		= 316,
	317,	//	SDLK_SYSREQ		= 317,
	318,	//	SDLK_BREAK		= 318,
	319,	//	SDLK_MENU		= 319,
	320,	//	SDLK_POWER		= 320,		/* Power Macintosh power key */
	321,	//	SDLK_EURO		= 321,		/* Some european keyboards */
	322		//	SDLK_UNDO		= 322,		/* Atari keyboard has Undo */
		//
		//	/* Add any other keys here */
		//
		//	SDLK_LAST
		//} SDLKey;
};

UINT16 gsExtendedCtrlKeyTable[] =
{
		//	/* Numeric keypad */
	256,	//	SDLK_KP0		= 256,
	257,	//	SDLK_KP1		= 257,
	258,	//	SDLK_KP2		= 258,
	259,	//	SDLK_KP3		= 259,
	260,	//	SDLK_KP4		= 260,
	261,	//	SDLK_KP5		= 261,
	262,	//	SDLK_KP6		= 262,
	263,	//	SDLK_KP7		= 263,
	264,	//	SDLK_KP8		= 264,
	265,	//	SDLK_KP9		= 265,
	266,	//	SDLK_KP_PERIOD		= 266,
	267,	//	SDLK_KP_DIVIDE		= 267,
	268,	//	SDLK_KP_MULTIPLY	= 268,
	269,	//	SDLK_KP_MINUS		= 269,
	270,	//	SDLK_KP_PLUS		= 270,
	271,	//	SDLK_KP_ENTER		= 271,
	272,	//	SDLK_KP_EQUALS		= 272,
		//
		//	/* Arrows + Home/End pad */
	CTRL_UPARROW,	//	SDLK_UP			= 273,
	CTRL_DNARROW,	//	SDLK_DOWN		= 274,
	CTRL_RIGHTARROW,	//	SDLK_RIGHT		= 275,
	CTRL_LEFTARROW,	//	SDLK_LEFT		= 276,
	CTRL_INSERT,	//	SDLK_INSERT		= 277,
	CTRL_HOME,	//	SDLK_HOME		= 278,
	CTRL_END,	//	SDLK_END		= 279,
	CTRL_PGUP,	//	SDLK_PAGEUP		= 280,
	CTRL_PGDN,	//	SDLK_PAGEDOWN		= 281,
		//
		//	/* Function keys */
	CTRL_F1,	//	SDLK_F1			= 282,
	CTRL_F2,	//	SDLK_F2			= 283,
	CTRL_F3,	//	SDLK_F3			= 284,
	CTRL_F4,	//	SDLK_F4			= 285,
	CTRL_F5,	//	SDLK_F5			= 286,
	CTRL_F6,	//	SDLK_F6			= 287,
	CTRL_F7,	//	SDLK_F7			= 288,
	CTRL_F8,	//	SDLK_F8			= 289,
	CTRL_F9,	//	SDLK_F9			= 290,
	CTRL_F10,	//	SDLK_F10		= 291,
	CTRL_F11,	//	SDLK_F11		= 292,
	CTRL_F12,	//	SDLK_F12		= 293,
	294,	//	SDLK_F13		= 294,
	295,	//	SDLK_F14		= 295,
	296,	//	SDLK_F15		= 296,
	297,
	298,
	299,
		//
		//	/* Key state modifier keys */
	NUM_LOCK,	//	SDLK_NUMLOCK		= 300,
	CAPS,	//	SDLK_CAPSLOCK		= 301,
	SCRL_LOCK,	//	SDLK_SCROLLOCK		= 302,
	SHIFT,	//	SDLK_RSHIFT		= 303,
	SHIFT,	//	SDLK_LSHIFT		= 304,
	CTRL,	//	SDLK_RCTRL		= 305,
	CTRL,	//	SDLK_LCTRL		= 306,
	ALT,	//	SDLK_RALT		= 307,
	ALT,	//	SDLK_LALT		= 308,
	309,	//	SDLK_RMETA		= 309,
	310,	//	SDLK_LMETA		= 310,
	311,	//	SDLK_LSUPER		= 311,		/* Left "Windows" key */
	312,	//	SDLK_RSUPER		= 312,		/* Right "Windows" key */
	313,	//	SDLK_MODE		= 313,		/* "Alt Gr" key */
	314,	//	SDLK_COMPOSE		= 314,		/* Multi-key compose key */
		//
		//	/* Miscellaneous function keys */
	315,	//	SDLK_HELP		= 315,
	316,	//	SDLK_PRINT		= 316,
	317,	//	SDLK_SYSREQ		= 317,
	318,	//	SDLK_BREAK		= 318,
	319,	//	SDLK_MENU		= 319,
	320,	//	SDLK_POWER		= 320,		/* Power Macintosh power key */
	321,	//	SDLK_EURO		= 321,		/* Some european keyboards */
	322		//	SDLK_UNDO		= 322,		/* Atari keyboard has Undo */
		//
		//	/* Add any other keys here */
		//
		//	SDLK_LAST
		//} SDLKey;
};

UINT16 gsExtendedAltKeyTable[] =
{
		//	/* Numeric keypad */
	256,	//	SDLK_KP0		= 256,
	257,	//	SDLK_KP1		= 257,
	258,	//	SDLK_KP2		= 258,
	259,	//	SDLK_KP3		= 259,
	260,	//	SDLK_KP4		= 260,
	261,	//	SDLK_KP5		= 261,
	262,	//	SDLK_KP6		= 262,
	263,	//	SDLK_KP7		= 263,
	264,	//	SDLK_KP8		= 264,
	265,	//	SDLK_KP9		= 265,
	266,	//	SDLK_KP_PERIOD		= 266,
	267,	//	SDLK_KP_DIVIDE		= 267,
	268,	//	SDLK_KP_MULTIPLY	= 268,
	269,	//	SDLK_KP_MINUS		= 269,
	270,	//	SDLK_KP_PLUS		= 270,
	271,	//	SDLK_KP_ENTER		= 271,
	272,	//	SDLK_KP_EQUALS		= 272,
		//
		//	/* Arrows + Home/End pad */
	ALT_UPARROW,	//	SDLK_UP			= 273,
	ALT_DNARROW,	//	SDLK_DOWN		= 274,
	ALT_RIGHTARROW,	//	SDLK_RIGHT		= 275,
	ALT_LEFTARROW,	//	SDLK_LEFT		= 276,
	ALT_INSERT,	//	SDLK_INSERT		= 277,
	ALT_HOME,	//	SDLK_HOME		= 278,
	ALT_END,	//	SDLK_END		= 279,
	ALT_PGUP,	//	SDLK_PAGEUP		= 280,
	ALT_PGDN,	//	SDLK_PAGEDOWN		= 281,
		//
		//	/* Function keys */
	ALT_F1,	//	SDLK_F1			= 282,
	ALT_F2,	//	SDLK_F2			= 283,
	ALT_F3,	//	SDLK_F3			= 284,
	ALT_F4,	//	SDLK_F4			= 285,
	ALT_F5,	//	SDLK_F5			= 286,
	ALT_F6,	//	SDLK_F6			= 287,
	ALT_F7,	//	SDLK_F7			= 288,
	ALT_F8,	//	SDLK_F8			= 289,
	ALT_F9,	//	SDLK_F9			= 290,
	ALT_F10,	//	SDLK_F10		= 291,
	ALT_F11,	//	SDLK_F11		= 292,
	ALT_F12,	//	SDLK_F12		= 293,
	294,	//	SDLK_F13		= 294,
	295,	//	SDLK_F14		= 295,
	296,	//	SDLK_F15		= 296,
	297,
	298,
	299,
		//
		//	/* Key state modifier keys */
	NUM_LOCK,	//	SDLK_NUMLOCK		= 300,
	CAPS,	//	SDLK_CAPSLOCK		= 301,
	SCRL_LOCK,	//	SDLK_SCROLLOCK		= 302,
	SHIFT,	//	SDLK_RSHIFT		= 303,
	SHIFT,	//	SDLK_LSHIFT		= 304,
	CTRL,	//	SDLK_RCTRL		= 305,
	CTRL,	//	SDLK_LCTRL		= 306,
	ALT,	//	SDLK_RALT		= 307,
	ALT,	//	SDLK_LALT		= 308,
	309,	//	SDLK_RMETA		= 309,
	310,	//	SDLK_LMETA		= 310,
	311,	//	SDLK_LSUPER		= 311,		/* Left "Windows" key */
	312,	//	SDLK_RSUPER		= 312,		/* Right "Windows" key */
	313,	//	SDLK_MODE		= 313,		/* "Alt Gr" key */
	314,	//	SDLK_COMPOSE		= 314,		/* Multi-key compose key */
		//
		//	/* Miscellaneous function keys */
	315,	//	SDLK_HELP		= 315,
	316,	//	SDLK_PRINT		= 316,
	317,	//	SDLK_SYSREQ		= 317,
	318,	//	SDLK_BREAK		= 318,
	319,	//	SDLK_MENU		= 319,
	320,	//	SDLK_POWER		= 320,		/* Power Macintosh power key */
	321,	//	SDLK_EURO		= 321,		/* Some european keyboards */
	322		//	SDLK_UNDO		= 322,		/* Atari keyboard has Undo */
		//
		//	/* Add any other keys here */
		//
		//	SDLK_LAST
		//} SDLKey;
};

UINT16 gsExtendedNumKeyTable[] =
{
			//	/* Numeric keypad */
	48,	//	SDLK_KP0		= 256,
	49,	//	SDLK_KP1		= 257,
	50,	//	SDLK_KP2		= 258,
	51,	//	SDLK_KP3		= 259,
	52,	//	SDLK_KP4		= 260,
	53,	//	SDLK_KP5		= 261,
	54,	//	SDLK_KP6		= 262,
	55,	//	SDLK_KP7		= 263,
	56,	//	SDLK_KP8		= 264,
	57,	//	SDLK_KP9		= 265,
	'.',	//	SDLK_KP_PERIOD		= 266,
	'/',	//	SDLK_KP_DIVIDE		= 267,
	'*',	//	SDLK_KP_MULTIPLY	= 268,
	'-',	//	SDLK_KP_MINUS		= 269,
	'+',	//	SDLK_KP_PLUS		= 270,
	ENTER,	//	SDLK_KP_ENTER		= 271,
	'=',	//	SDLK_KP_EQUALS		= 272,
};

// The gsKeyTranslationTable is used to return KEY values on the basis of the virtual key code and
// SHIFT/ALT/CTRL key states. Range 0-255 is for normal keys, 256-511 is when SHIFT is pressed
// 512-767 is when ALT is pressed and 768-1023 is when CTRL is pressed. This table should be modified
// during translation

UINT16 gsKeyTranslationTable[1024] =
{
   0, //			// 0
   1, //
   2, //
   3, //
   4, //
   5, //
   6, //
   7, //
   8, // BACK
   9, // TAB
  10, //			// 10
  11, //
  12, //
  13, // ENTER
  14, //
  15, //
  16, // SHIFT
  17, // CTRL
  18, // ALT
  19, // PAUSE
  20, // CAPS		// 20
  21, //
  22, //
  23, //
  24, //
  25, //
  26, //
  27, // ESC
  28, //
  29, //
  30, //			// 30
  31, //
  32, // SPACE
  33, //
  34, //
  35, //
  36, //
  37, //
  38, //
  39, //
  40, //			// 40
  41, //
  42, //
  43, //
  44, //
  45, //
  46, //
  47, //
  48, // 0
  49, // 1
  50, // 2			// 50
  51, // 3
  52, // 4
  53, // 5
  54, // 6
  55, // 7
  56, // 8
  57, // 9
  58, //
  59, //
  60, //			// 60
  61, //
  62, //
  63, //
  64, //
  97, // a
  98, // b
  99, // c
 100, // d
 101, // e
 102, // f			// 70
 103, // g
 104, // h
 105, // i
 106, // j
 107, // k
 108, // l
 109, // m
 110, // n
 111, // o
 112, // p			// 80
 113, // q
 114, // r
 115, // s
 116, // t
 117, // u
 118, // v
 119, // w
 120, // x		
 121, // y
 122, // z			// 90
  91, //
  92, //
  93, //
  94, //
  95, //
  96, //
  97, //
  98, //
  99, //
 100, //			// 100
 101, //
 102, //
 103, //
 104, //
 105, //
  42, // NUM_TIMES
  43, // NUM_PLUS
 108, //
  45, // NUM_MINUS
 110, //			// 110
  47, // NUM_SLASH
 124, // F1
 125, // F2
 126, // F3
 127, // F4
 128, // F5
 129, // F6
 130, // F7
 131, // F8
 132, // F9			// 120
 133, // F10
 134, // F11
 135, // F12
 124, //
 125, //
 126, //
 127, //
 128, //
 129, //
 130, //			// 130
 131, //
 132, //
 133, //
 134, //
 135, //
 136, //
 137, //
 138, //
 139, //
 140, //			// 140
 141, //
 142, //
 143, //
 144, // NUM_LOCK
 145, // SCRL
 146, //
 147, //
 148, //
 149, //
 150, //			// 150
 151, //
 152, //
 153, //
 154, //
 155, //
 156, //
 157, //
 158, //
 159, //
 160, //			// 160
 161, //
 162, //
 163, //
 164, //
 165, //
 166, //
 167, //
 168, //
 169, //
 170, //			// 170
 171, //
 172, //
 173, //
 174, //
 175, //
 176, //
 177, //
 178, //
 179, //
 180, //			// 180
 181, //
 182, //
 183, //
 184, //
 185, //
  59, // ;
  61, // =
  44, // ,
  45, // -
  46, // .			// 190
  47, // slash
  96, // `
 193, //
 194, //
 195, //
 196, //
 197, //
 198, //
 199, //
 200, //			// 200
 201, //
 202, //
 203, //
 204, //
 205, //
 206, //
 207, //
 208, //
 209, //
 210, //			// 210
 211, //
 212, //
 213, //
 214, //
 215, //
 216, //
 217, //
 218, //
  91, // [
  92, // back slash	// 220
  93, // ]
  39, // '
  48, // NUM_0 (ON)
  46, // NUM_PERIOD (ON)
  49, // NUM_1 (ON)
  50, // NUM_2 (ON)
  51, // NUM_3 (ON)
  52, // NUM_4 (ON)
  53, // NUM_5 (ON)
  54, // NUM_6 (ON)	// 230
  55, // NUM_7 (ON)
  56, // NUM_8 (ON)
  57, // NUM_9 (ON)
 245, // NUM_0 (OFF)
 246, // NUM_PERIOD (OFF)
 247, // NUM_1 (OFF)
 248, // NUM_2 (OFF)
 249, // NUM_3 (OFF)
 250, // NUM_4 (OFF)
   0, // NUM_5 (OFF)	// 240
 251, // NUM_6 (OFF)
 252, // NUM_7 (OFF)
 253, // NUM_8 (OFF)
 254, // NUM_9 (OFF)
 245, // INS
 246, // DEL
 247, // END
 248, // DOWN
 249, // PGDN
 250, // LEFT		// 250
 251, // RIGHT
 252, // HOME
 253, // UP
 254, // PGUP
 255, //
 256, //
 257, //
 258, //
 259, //
 260, //			// 260
 261, //
 262, //
 263, //
   8, // BACK
 265, // TAB
 266, //
 267, //
 268, //
  13, // ENTER
 270, //			// 270
 271, //
  16, // SHIFT
  17, // CTRL
  18, // ALT
  19, // PAUSE
  20, // CAPS
 277, //
 278, //
 279, //
 280, //			// 280
 281, //
 282, //
  27, // ESC
 284, //
 285, //
 286, //
 287, //
  32, // SPACE
 289, //
 290, //			// 290
 291, //
 292, //
 293, //
 294, //
 295, //
 296, //
 297, //
 298, //
 299, //
 300, //			// 300
 301, //
 302, //
 303, //
  41, // )
  33, // !
  64, // @
  35, // #
  36, // $
  37, // %
  94, // ^			// 310
  38, // &
  42, // *
  40, // (  
 314, //
 315, //
 316, //
 317, //
 318, //
 319, //
 320, //			// 320
  65, // A
  66, // B
  67, // C
  68, // D
  69, // E
  70, // F
  71, // G
  72, // H
  73, // I
  74, // J			// 330
  75, // K
  76, // L
  77, // M
  78, // N
  79, // O
  80, // P
  81, // Q
  82, // R
  83, // S
  84, // T			// 340
  85, // U
  86, // V
  87, // W
  88, // X
  89, // Y
  90, // Z
 347, //
 348, //
 349, //
 350, //			// 350
 351, //
 352, //
 353, //
 354, //
 355, //
 356, //
 357, //
 358, //
 359, //
 360, //			// 360
 361, //
  42, // NUM_TIMES
  43, // NUM_PLUS
 364, //
  45, // NUM_MINUS
 366, //
  47, // NUM_SLASH
 368, // SHIFT-F1
 369, // SHIFT-F2
 370, // SHIFT-F3	// 370
 371, // SHIFT-F4
 372, // SHIFT-F5
 373, // SHIFT-F6
 374, // SHIFT-F7
 375, // SHIFT-F8
 376, // SHIFT-F9
 377, // SHIFT-F10
 378, // SHIFT-F11
 379, // SHIFT-F12
 380, //			// 380
 381, //
 382, //
 383, //
 384, //
 385, //
 386, //
 387, //
 388, //
 389, //
 390, //			// 390
 391, //
 392, //
 393, //
 394, //
 395, //
 396, //
 397, //
 398, //
 399, //
 144, // NUM_LOCK	// 400
 145, // SCRL
 402, //
 403, //
 404, //
 405, //
 406, //
 407, //
 408, //
 409, //
 410, //			// 410
 411, //
 412, //
 413, //
 414, //
 415, //
 416, //
 417, //
 418, //
 419, //
 420, //			// 420
 421, //
 422, //
 423, //
 424, //
 425, //
 426, //
 427, //
 428, //
 429, //
 430, //			// 430
 431, //
 432, //
 433, //
 434, //
 435, //
 436, //
 437, //
 438, //
 439, //
 440, //			// 440
 441, //
  58, // :
  43, // +
  60, // <
  95, // _
  62, // >
  63, // ?
 126, // ~
 449, //
 450, //			// 450
 451, //
 452, //
 453, //
 454, //
 455, //
 456, //
 457, //
 458, //
 459, //
 460, //			// 460
 461, //
 462, //
 463, //
 464, //
 465, //
 466, //
 467, //
 468, //
 469, //
 470, //			// 470
 471, //
 472, //
 473, //
 474, //
 123, // {
 124, // |
 125, // }
  34, // "
  48, // NUM_0 (ON)
  46, // NUM_PERIOD (ON)	// 480
  49, // NUM_1 (ON)
  50, // NUM_2 (ON)
  51, // NUM_3 (ON)
  52, // NUM_4 (ON)
  53, // NUM_5 (ON)
  54, // NUM_6 (ON)
  55, // NUM_7 (ON)
  56, // NUM_8 (ON)
  57, // NUM_9 (ON)
 501, // SHIFT-NUM_0 (OFF)		// 490
 502, // SHIFT-NUM_PERIOD (OFF)
 503, // SHIFT-NUM_1 (OFF)
 504, // SHIFT-NUM_2 (OFF)
 505, // SHIFT-NUM_3 (OFF)
 506, // SHIFT-NUM_4 (OFF)
   0, // SHIFT-NUM_5 (OFF)
 507, // SHIFT-NUM_6 (OFF)
 508, // SHIFT-NUM_7 (OFF)
 509, // SHIFT-NUM_8 (OFF)
 510, // SHIFT-NUM_9 (OFF)		// 500
 501, // SHIFT-INS
 502, // SHIFT-DEL
 503, // SHIFT-END
 504, // SHIFT-DOWN
 505, // SHIFT-PGDN
 506, // SHIFT-LEFT
 507, // SHIFT-RIGHT
 508, // SHIFT-HOME
 509, // SHIFT-UP
 510, // SHIFT-PGUP			// 510
 511, //
 512, //
 513, //
 514, //
 515, //
 516, //
 517, //
 518, //
 519, //
   8, // BACK				// 520
 521, // TAB
 522, //
 523, //
 524, //
  13, // ENTER
 526, //
 527, //
  16, // SHIFT
  17, // CTRL
  18, // ALT			// 530
  19, // PAUSE
  20, // CAPS
 533, //
 534, //
 535, //
 536, //
 537, //
 538, //
  27, // ESC
 540, //				// 540
 541, //
 542, //
 543, //
  32, // SPACE
 545, //
 546, //
 547, //
 548, //
 549, //
 550, //			// 550
 551, //
 552, //
 553, //
 554, //
 555, //
 556, //
 557, //
 558, //
 559, //
 560, //			// 560
 561, //
 562, //
 563, //
 564, //
 565, //
 566, //
 567, //
 568, //
 569, //
 570, //			// 570
 571, //
 572, //
 573, //
 574, //
 575, //
 576, //
 577, //
 578, //
 579, //
 580, //			// 580
 581, //
 582, //
 583, //
 584, //
 585, //
 586, //
 587, //
 588, //
 589, //
 590, //			// 590
 591, //
 592, //
 593, //
 594, //
 595, //
 596, //
 597, //
 598, //
 599, //
 600, //			// 600
 601, //
 602, //
 603, //
 604, //
 605, //
 606, //
 607, //
 608, //
 609, //
 610, //			// 610
 611, //
 612, //
 613, //
 614, //
 615, //
 616, //
 617, //
  42, // NUM_TIMES
  43, // NUM_PLUS
 620, //			// 620
  45, // NUM_MINUS
 622, //
  47, // NUM_SLASH
 624, // ALT-F1
 625, // ALT-F2
 626, // ALT-F3
 627, // ALT-F4
 628, // ALT-F5
 629, // ALT-F6
 630, // ALT-F7		// 630
 631, // ALT-F8
 632, // ALT-F9
 633, // ALT-F10
 634, // ALT-F11
 635, // ALT-F12
 636, //
 637, //
 638, //
 639, //
 640, //			// 640
 641, //
 642, //
 643, //
 644, //
 645, //
 646, //
 647, //
 648, //
 649, //
 650, //			// 650
 651, //
 652, //
 653, //
 654, //
 655, //
 144, // NUM_LOCK
 145, // SCRL
 658, //
 659, //
 660, //			// 660
 661, //
 662, //
 663, //
 664, //
 665, //
 666, //
 667, //
 668, //
 669, //
 670, //			// 670
 671, //
 672, //
 673, //
 674, //
 675, //
 676, //
 677, //
 678, //
 679, //
 680, //			// 680
 681, //
 682, //
 683, //
 684, //
 685, //
 686, //
 687, //
 688, //
 689, //
 690, //			// 690
 691, //
 692, //
 693, //
 694, //
 695, //
 696, //
 697, //
 698, //
 699, //
 700, //			// 700
 701, //
 702, //
 703, //
 704, //
 705, //
 706, //
 707, //
 708, //
 709, //
 710, //			// 710
 711, //
 712, //
 713, //
 714, //
 715, //
 716, //
 717, //
 718, //
 719, //
 720, //			// 720
 721, //
 722, //
 723, //
 724, //
 725, //
 726, //
 727, //
 728, //
 729, //
 730, //			// 730
 731, //
 732, //
 733, //
 734, //
  48, // NUM_0 (ON)
  46, // NUM_PERIOD (ON)
  49, // NUM_1 (ON)
  50, // NUM_2 (ON)
  51, // NUM_3 (ON)
  52, // NUM_4 (ON)		// 740
  53, // NUM_5 (ON)
  54, // NUM_6 (ON)
  55, // NUM_7 (ON)
  56, // NUM_8 (ON)
  57, // NUM_9 (ON)
 757, // ALT-NUM_0 (OFF)
 758, // ALT-NUM_PERIOD (OFF)
 759, // ALT-NUM_1 (OFF)
 760, // ALT-NUM_2 (OFF)
 761, // ALT-NUM_3 (OFF)	// 750
 762, // ALT-NUM_4 (OFF)
   0, // ALT-NUM_5 (OFF)
 763, // ALT-NUM_6 (OFF)
 764, // ALT-NUM_7 (OFF)
 765, // ALT-NUM_8 (OFF)
 766, // ALT-NUM_9 (OFF)
 757, // ALT-INS
 758, // ALT-DEL
 759, // ALT-END
 760, // ALT-DOWN			// 760
 761, // ALT-PGDN
 762, // ALT-LEFT
 763, // ALT-RIGHT
 764, // ALT-HOME
 765, // ALT-UP
 766, // ALT-PGUP
 767, //
 768, //
 769, //
 770, //			// 770
 771, //
 772, //
 773, //
 774, //
 775, //
   8, // BACK
 777, // TAB
 778, //
 779, //
 780, //			// 780
  13, // ENTER
 782, //
 783, //
  16, // SHIFT
  17, // CTRL
  18, // ALT
  19, // PAUSE
  20, // CAPS
 789, //
 790, //			// 790
 791, //
 792, //
 793, //
 794, //
  27, // ESC
 796, //
 797, //
 798, //
 799, //
  32, // SPACE		// 800
 801, //
 802, //
 803, //
 804, //
 805, //
 806, //
 807, //
 808, //
 809, //
 810, //			// 810
 811, //
 812, //
 813, //
 814, //
 815, //
 816, //
 817, //
 818, //
 819, //
 820, //			// 820
 821, //
 822, //
 823, //
 824, //
 825, //
 826, //
 827, //
 828, //
 829, //
 830, //			// 830
 831, //
 832, //
 833, //
 834, //
 835, //
 836, //
 837, //
 838, //
 839, //
 840, //			// 840
 841, //
 842, //
 843, //
 844, //
 845, //
 846, //
 847, //
 848, //
 849, //
 850, //			// 850
 851, //
 852, //
 853, //
 854, //
 855, //
 856, //
 857, //
 858, //
 859, //
 860, //			// 860
 861, //
 862, //
 863, //
 864, //
 865, //
 866, //
 867, //
 868, //
 869, //
 870, //			// 870
 871, //
 872, //
 873, //
  42, // NUM_TIMES
  43, // NUM_PLUS
 876, //
  45, // NUM_MINUS
 878, //
  47, // NUM_SLASH
 880, // CTRL-F1	// 880
 881, // CTRL-F2
 882, // CTRL-F3
 883, // CTRL-F4
 884, // CTRL-F5
 885, // CTRL-F6
 886, // CTRL-F7
 887, // CTRL-F8
 888, // CTRL-F9
 889, // CTRL-F10
 890, // CTRL-F11	// 890
 891, // CTRL-F12
 892, //
 893, //
 894, //
 895, //
 896, //
 897, //
 898, //
 899, //
 900, //			// 900
 901, //
 902, //
 903, //
 904, //
 905, //
 906, //
 907, //
 908, //
 909, //
 910, //			// 910
 911, //
 144, // NUM_LOCK
 145, // SCRL
 914, //
 915, //
 916, //
 917, //
 918, //
 919, //
 920, //			// 920
 921, //
 922, //
 923, //
 924, //
 925, //
 926, //
 927, //
 928, //
 929, //
 930, //			// 930
 931, //
 932, //
 933, //
 934, //
 935, //
 936, //
 937, //
 938, //
 939, //
 940, //			// 940
 941, //
 942, //
 943, //
 944, //
 945, //
 946, //
 947, //
 948, //
 949, //
 950, //			// 950
 951, //
 952, //
 953, //
 954, //
 955, //
 956, //
 957, //
 958, //
 959, //
 960, //			// 960
 961, //
 962, //
 963, //
 964, //
 965, //
 966, //
 967, //
 968, //
 969, //
 970, //			// 970
 971, //
 972, //
 973, //
 974, //
 975, //
 976, //
 977, //
 978, //
 979, //
 980, //			// 980
 981, //
 982, //
 983, //
 984, //
 985, //
 986, //
 987, //
 988, //
 989, //
 990, //			// 990
  48, // NUM_0 (ON)
  46, // NUM_PERIOD (ON)
  49, // NUM_1 (ON)
  50, // NUM_2 (ON)
  51, // NUM_3 (ON)
  52, // NUM_4 (ON)
  53, // NUM_5 (ON)
  54, // NUM_6 (ON)
  55, // NUM_7 (ON)
  56, // NUM_8 (ON)		// 1000
  57, // NUM_9 (ON)
1013, // CTRL-NUM_0 (OFF)
1014, // CTRL-NUM_PERIOD (OFF)
1015, // CTRL-NUM_1 (OFF)
1016, // CTRL-NUM_2 (OFF)
1017, // CTRL-NUM_3 (OFF)
1018, // CTRL-NUM_4 (OFF)
   0, // CTRL-NUM_5 (OFF)
1019, // CTRL-NUM_6 (OFF)
1020, // CTRL-NUM_7 (OFF)	// 1010
1021, // CTRL-NUM_8 (OFF)
1022, // CTRL-NUM_9 (OFF)
1013, // CTRL-INS
1014, // CTRL-DEL
1015, // CTRL-END
1016, // CTRL-DOWN
1017, // CTRL-PGDN
1018, // CTRL-LEFT
1019, // CTRL-RIGHT
1020, // CTRL-HOME			// 1020
1021, // CTRL-UP
1022, // CTRL-PGUP
1023  // CURSOR
};
