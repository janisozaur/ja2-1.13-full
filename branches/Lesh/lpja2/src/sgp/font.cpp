// font.c
#ifdef JA2_PRECOMPILED_HEADERS
	#include "ja2_sgp_all.h"
#else
	#include "platform.h"
	#include "types.h"
	#include "sgp.h"
	#include "pcx.h"
	#include "mem_man.h"
	#include "file_man.h"
	#include "font.h"
	#include "debug.h"
	#include "video.h"
	#include "image.h"
	#include "vobject.h"
	#include "vobject_blitters.h"
	#include "sgp_str.h"
	
#endif
//*******************************************************
//
//   Defines
//
//*******************************************************

#define PALETTE_SIZE     768
#define STRING_DELIMITER 0
#define ID_BLACK         0
#define MAX_FONTS				 25

//*******************************************************
//
//   Typedefs
//
//*******************************************************

extern SGPPaletteEntry gSgpPalette[256]; // symbol already declared globally in video.cpp (jonathanl)

typedef struct
{
	UINT16 usDefaultPixelDepth;
	FontTranslationTable *pTranslationTable;
} FontManager;

FontManager *pFManager;
HVOBJECT	FontObjs[MAX_FONTS];
INT32		FontsLoaded=0;

// Destination printing parameters
INT32			FontDefault=(-1);
UINT32		FontDestBuffer=BACKBUFFER;

UINT32		FontDestPitch=640*2;

UINT32		FontDestBPP=16;
SGPRect		FontDestRegion={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};


BOOLEAN		FontDestWrap=FALSE;
UINT16		FontForeground16=0;
UINT16		FontBackground16=0;
UINT16		FontShadow16=DEFAULT_SHADOW;
UINT8			FontForeground8=0;
UINT8			FontBackground8=0;

// Temp, for saving printing parameters
INT32			SaveFontDefault=(-1);
UINT32		SaveFontDestBuffer=BACKBUFFER;

UINT32		SaveFontDestPitch=640*2;

UINT32		SaveFontDestBPP=16;
SGPRect		SaveFontDestRegion={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

BOOLEAN		SaveFontDestWrap=FALSE;
UINT16		SaveFontForeground16=0;
UINT16		SaveFontShadow16=0;
UINT16		SaveFontBackground16=0;
UINT8			SaveFontForeground8=0;
UINT8			SaveFontBackground8=0;

//*****************************************************************************
// SetFontColors
//
//	Sets both the foreground and the background colors of the current font. The
// top byte of the parameter word is the background color, and the bottom byte
// is the foreground.
//
//*****************************************************************************
void SetFontColors(UINT16 usColors)
{
UINT8 ubForeground, ubBackground;

	ubForeground=(UINT8)(usColors&0xff);
	ubBackground=(UINT8)((usColors&0xff00)>>8);

	SetFontForeground(ubForeground);
	SetFontBackground(ubBackground);

}

//*****************************************************************************
// SetFontForeground
//
//	Sets the foreground color of the currently selected font. The parameter is
// the index into the 8-bit palette. In 8BPP mode, that index number is used
// for the pixel value to be drawn for nontransparent pixels. In 16BPP mode,
// the RGB values from the palette are used to create the pixel color. Note
// that if you change fonts, the selected foreground/background colors will
// stay at what they are currently set to.
//
//*****************************************************************************
void SetFontForeground(UINT8 ubForeground)
{
UINT32 uiRed, uiGreen, uiBlue;

	if((FontDefault < 0) || (FontDefault > MAX_FONTS))
		return;

	FontForeground8=ubForeground;

	uiRed=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubForeground].peRed;
	uiGreen=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubForeground].peGreen;
	uiBlue=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubForeground].peBlue;

	FontForeground16=Get16BPPColor(FROMRGB(uiRed, uiGreen, uiBlue));

}

void SetFontShadow(UINT8 ubShadow )
{
UINT32 uiRed, uiGreen, uiBlue;

	if((FontDefault < 0) || (FontDefault > MAX_FONTS))
		return;

	//FontForeground8=ubForeground;

	uiRed=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubShadow].peRed;
	uiGreen=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubShadow].peGreen;
	uiBlue=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubShadow].peBlue;

	FontShadow16=Get16BPPColor(FROMRGB(uiRed, uiGreen, uiBlue));

	if ( ubShadow != 0 )
	{
		if ( FontShadow16 == 0 )
		{
			FontShadow16 = 1;
		}
	}

}

//*****************************************************************************
// SetFontBackground
//
//	Sets the Background color of the currently selected font. The parameter is
// the index into the 8-bit palette. In 8BPP mode, that index number is used
// for the pixel value to be drawn for nontransparent pixels. In 16BPP mode,
// the RGB values from the palette are used to create the pixel color. If the
// background value is zero, the background of the font will be transparent.
// Note that if you change fonts, the selected foreground/background colors will
// stay at what they are currently set to.
//
//*****************************************************************************
void SetFontBackground(UINT8 ubBackground)
{
UINT32 uiRed, uiGreen, uiBlue;

	if((FontDefault < 0) || (FontDefault > MAX_FONTS))
		return;

	FontBackground8=ubBackground;

	uiRed=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubBackground].peRed;
	uiGreen=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubBackground].peGreen;
	uiBlue=(UINT32)FontObjs[FontDefault]->pPaletteEntry[ubBackground].peBlue;

	FontBackground16=Get16BPPColor(FROMRGB(uiRed, uiGreen, uiBlue));
}


//Kris:  These are new counterparts to the above functions.  They won't
//			 effect an 8BPP font, only 16.
void SetRGBFontForeground( UINT32 uiRed, UINT32 uiGreen, UINT32 uiBlue )
{
	if((FontDefault < 0) || (FontDefault > MAX_FONTS))
		return;
	FontForeground16 = Get16BPPColor( FROMRGB( uiRed, uiGreen, uiBlue ) );
}

void SetRGBFontBackground( UINT32 uiRed, UINT32 uiGreen, UINT32 uiBlue )
{
	if((FontDefault < 0) || (FontDefault > MAX_FONTS))
		return;
	FontBackground16 = Get16BPPColor( FROMRGB( uiRed, uiGreen, uiBlue ) );
}

void SetRGBFontShadow( UINT32 uiRed, UINT32 uiGreen, UINT32 uiBlue )
{
	if((FontDefault < 0) || (FontDefault > MAX_FONTS))
		return;
	FontShadow16 = Get16BPPColor( FROMRGB( uiRed, uiGreen, uiBlue ) );
}
//end Kris

//*****************************************************************************
// ResetFontObjectPalette
//
//	Sets the palette of a font, using an 8 bit palette (which is converted to
// the appropriate 16-bit palette, and assigned to the HVOBJECT).
//
//*****************************************************************************
BOOLEAN ResetFontObjectPalette(INT32 iFont)
{
	Assert(iFont >= 0);
	Assert(iFont <= MAX_FONTS);
	Assert(FontObjs[iFont] !=NULL);

	SetFontObjectPalette8BPP(iFont, FontObjs[iFont]->pPaletteEntry);

	return(TRUE);
}


//*****************************************************************************
// SetFontObjectPalette8BPP
//
//	Sets the palette of a font, using an 8 bit palette (which is converted to
// the appropriate 16-bit palette, and assigned to the HVOBJECT).
//
//*****************************************************************************
UINT16 *SetFontObjectPalette8BPP(INT32 iFont, SGPPaletteEntry *pPal8)
{
UINT16 *pPal16;

	Assert(iFont >= 0);
	Assert(iFont <= MAX_FONTS);
	Assert(FontObjs[iFont] !=NULL);

	if((pPal16=Create16BPPPalette(pPal8))==NULL)
		return(NULL);

	FontObjs[iFont]->p16BPPPalette=pPal16;
	FontObjs[iFont]->pShadeCurrent=pPal16;

	return(pPal16);
}

//*****************************************************************************
// SetFontObjectPalette16BPP
//
//	Sets the palette of a font, using a 16 bit palette.
//
//*****************************************************************************
UINT16 *SetFontObjectPalette16BPP(INT32 iFont, UINT16 *pPal16)
{
	Assert(iFont >= 0);
	Assert(iFont <= MAX_FONTS);
	Assert(FontObjs[iFont] !=NULL);

	FontObjs[iFont]->p16BPPPalette=pPal16;
	FontObjs[iFont]->pShadeCurrent=pPal16;

	return(pPal16);

}

//*****************************************************************************
// GetFontObjectPalette16BPP
//
//	Sets the palette of a font, using a 16 bit palette.
//
//*****************************************************************************
UINT16 *GetFontObjectPalette16BPP(INT32 iFont)
{
	Assert(iFont >= 0);
	Assert(iFont <= MAX_FONTS);
	Assert(FontObjs[iFont] !=NULL);

	return(FontObjs[iFont]->p16BPPPalette);
}

//*****************************************************************************
// GetFontObject
//
//	Returns the VOBJECT pointer of a font.
//
//*****************************************************************************
HVOBJECT GetFontObject(INT32 iFont)
{
	Assert(iFont >= 0);
	Assert(iFont <= MAX_FONTS);
	Assert(FontObjs[iFont] !=NULL);

	return(FontObjs[iFont]);
}

//*****************************************************************************
// FindFreeFont
//
//	Locates an empty slot in the font table.
//
//*****************************************************************************
INT32 FindFreeFont(void)
{
int count;

	for(count=0; count < MAX_FONTS; count++)
		if(FontObjs[count]==NULL)
			return(count);

	return(-1);

}

//*****************************************************************************
// LoadFontFile
//
//	Loads a font from an ETRLE file, and inserts it into one of the font slots.
//  This function returns (-1) if it fails, and debug msgs for a reason.
//  Otherwise the font number is returned.
//*****************************************************************************
template INT32 LoadFontFile<char *>(char *);
template INT32 LoadFontFile<char const *>(char const *);
template <typename string1>
INT32 LoadFontFile(string1 filename)
{
VOBJECT_DESC		vo_desc;
UINT32					LoadIndex;

	Assert(filename!=NULL);
	Assert(strlen(filename));

	if((LoadIndex=FindFreeFont())==(-1))
	{
		fprintf(stderr, "FONT: Out of font slots (%s)\n", filename);
		return(-1);
	}

	vo_desc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	strcpy(vo_desc.ImageFile, filename);

	if((FontObjs[LoadIndex]=CreateVideoObject(&vo_desc))==NULL)
	{
		fprintf(stderr, "FONT: Error creating VOBJECT (%s)\n", filename);
		return(-1);
	}

	if(FontDefault==(-1))
		FontDefault=LoadIndex;

	return(LoadIndex);
}

//*****************************************************************************
// UnloadFont - Delete the font structure
//
//	Deletes the video object of a particular font. Frees up the memory and
// resources allocated for it.
//
//*****************************************************************************
void UnloadFont(UINT32 FontIndex)
{
	Assert(FontIndex >= 0);
	Assert(FontIndex <= MAX_FONTS);
	Assert(FontObjs[FontIndex]!=NULL);

	DeleteVideoObject(FontObjs[FontIndex]);
	FontObjs[FontIndex]=NULL;
}

//*****************************************************************************
// GetWidth
//
//	Returns the width of a given character in the font.
//
//*****************************************************************************
UINT32 GetWidth(HVOBJECT hSrcVObject, INT16 ssIndex)
{
  ETRLEObject *pTrav;

	// Assertions
	Assert( hSrcVObject != NULL );

	if ( ssIndex < 0 || ssIndex > 92 )
	{
		int i=0;
	}

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[ ssIndex ] );
	return((UINT32)(pTrav->usWidth+pTrav->sOffsetX));
}

//*****************************************************************************
// StringPixLengthArg
//
//		Returns the length of a string with a variable number of arguments, in
// pixels, using the current font. Maximum length in characters the string can
// evaluate to is 512.
//    'uiCharCount' specifies how many characters of the string are counted.
//*****************************************************************************
INT16 StringPixLengthArg(INT32 usUseFont, UINT32 uiCharCount, STR16 pFontString, ...)
{
va_list argptr;
wchar_t	string[512];

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);       	// Set up variable argument pointer
	WSTR_VSPrintf(string, 512, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

  // make sure the character count is legal
  if (uiCharCount > wcslen(string))
  {
    uiCharCount = wcslen(string);
  }
  else
  {
    if (uiCharCount < wcslen(string))
    {
      // less than the full string, so whack off the end of it (it's temporary anyway)
      string[uiCharCount] = '\0';
    }
  }

	return(StringPixLength(string, usUseFont));
}

//*****************************************************************************
// StringPixLengthArg
//
// Returns the length of a string with a variable number of arguments, in
// pixels, using the current font. Maximum length in characters the string can
// evaluate to is 512.  Because this is for fast help text, all '|' characters are ignored for the 
// width calculation.
// 'uiCharCount' specifies how many characters of the string are counted.
// YOU HAVE TO PREBUILD THE FAST HELP STRING!
//*****************************************************************************
INT16 StringPixLengthArgFastHelp(INT32 usUseFont, INT32 usBoldFont, UINT32 uiCharCount, STR16 pFontString )
{
	wchar_t	string[512];
	UINT32 i, index;
	INT16 sBoldDiff = 0;
	UINT16 str[2];

	Assert(pFontString!=NULL);

	wcscpy( string, (wchar_t*) pFontString );

  // make sure the character count is legal
  if (uiCharCount > wcslen(string))
  {
    uiCharCount = wcslen(string);
  }
  else
  {
    if (uiCharCount < wcslen(string))
    {
      // less than the full string, so whack off the end of it (it's temporary anyway)
      string[uiCharCount] = '\0';
    }
  }
	//now eliminate all '|' characters from the string.
	i = 0;
	while( i < uiCharCount )
	{
		if( string[ i ] == '|' )
		{
			for( index = i; index < uiCharCount; index++ )
			{
				string[ index ] = string[ index + 1 ];
			}
			uiCharCount--;
			//now we have eliminated the '|' character, so now calculate the size difference of the
			//bolded character.
			str[ 0 ] = string[ i ];
			str[ 1 ] = 0;
			sBoldDiff += StringPixLength( str, usBoldFont ) - StringPixLength( str, usUseFont );
		}
		i++;
	}
	return StringPixLength(string, usUseFont) + sBoldDiff;
}



//*****************************************************************************************
//
//  StringNPixLength
//
//  Return the length of the of the string or count characters in the 
//  string, which ever comes first.
//
//  Returns INT16
//
//  Created by:     Gilles Beauparlant
//  Created on:     12/1/99
//
//*****************************************************************************************
INT16 StringNPixLength(UINT16 *string, UINT32 uiMaxCount, INT32 UseFont)
{
	UINT32 Cur, uiCharCount;
	UINT16 *curletter,transletter;

	Cur = 0;
	uiCharCount = 0;
	curletter = string;

	while((*curletter) != L'\0' && uiCharCount < uiMaxCount )
	{
		transletter=GetIndex(*curletter++);
		Cur+=GetWidth(FontObjs[UseFont], transletter);
		uiCharCount++;
	}
	return((INT16)Cur);
}

//*****************************************************************************
//
// StringPixLength
//
//	Returns the length of a string in pixels, depending on the font given.
//
//*****************************************************************************
template INT16 StringPixLength<short *>(short *, INT32);
template INT16 StringPixLength<wchar_t *>(wchar_t *, INT32);
template INT16 StringPixLength<wchar_t const *>(wchar_t const*, INT32);
template INT16 StringPixLength<unsigned short const *>(unsigned short const *, INT32);
template <typename type1>
INT16 StringPixLength(type1 string, INT32 UseFont)
{
	UINT32 Cur;
	UINT16 *curletter,transletter;

	if (string == NULL)
	{
		return(0);
	}

	Cur=0;
	curletter = (UINT16 *)string;

	while((*curletter) != L'\0')
	{
		transletter=GetIndex(*curletter++);
		Cur+=GetWidth(FontObjs[UseFont], transletter);
	}
	return((INT16)Cur);
}


//*****************************************************************************
//
// SaveFontSettings
//
//	Saves the current font printing settings into temporary locations.
//
//*****************************************************************************
void SaveFontSettings(void)
{
	SaveFontDefault=FontDefault;
	SaveFontDestBuffer=FontDestBuffer;
	SaveFontDestPitch=FontDestPitch;
	SaveFontDestBPP=FontDestBPP;
	SaveFontDestRegion=FontDestRegion;
	SaveFontDestWrap=FontDestWrap;
	SaveFontForeground16 = FontForeground16;
	SaveFontShadow16 = FontShadow16;
	SaveFontBackground16 = FontBackground16;
	SaveFontForeground8 = FontForeground8;
	SaveFontBackground8 = FontBackground8;
}


//*****************************************************************************
//
// RestoreFontSettings
//
//	Restores the last saved font printing settings from the temporary lactions
//
//*****************************************************************************
void RestoreFontSettings(void)
{
	FontDefault=SaveFontDefault;
	FontDestBuffer=SaveFontDestBuffer;
	FontDestPitch=SaveFontDestPitch;
	FontDestBPP=SaveFontDestBPP;
	FontDestRegion=SaveFontDestRegion;
	FontDestWrap=SaveFontDestWrap;
	FontForeground16 = SaveFontForeground16;
	FontShadow16 = SaveFontShadow16;
	FontBackground16 = SaveFontBackground16;
	FontForeground8 = SaveFontForeground8;
	FontBackground8 = SaveFontBackground8;
}


//*****************************************************************************
// GetHeight
//
//	Returns the height of a given character in the font.
//
//*****************************************************************************
UINT32 GetHeight(HVOBJECT hSrcVObject, INT16 ssIndex)
{
  ETRLEObject *pTrav;

	// Assertions
	Assert( hSrcVObject != NULL );

	// Get Offsets from Index into structure
	pTrav = &(hSrcVObject->pETRLEObject[ ssIndex ] );
	return((UINT32)(pTrav->usHeight+pTrav->sOffsetY));
}

//*****************************************************************************
//
// GetFontHeight
//
//	Returns the height of the first character in a font.
//
//*****************************************************************************
UINT16 GetFontHeight(INT32 FontNum)
{
	Assert(FontNum >= 0);
	Assert(FontNum <= MAX_FONTS);
	Assert(FontObjs[FontNum]!=NULL);

	return((UINT16)GetHeight(FontObjs[FontNum], 0));
}

//*****************************************************************************
// GetIndex
//
//		Given a word-sized character, this function returns the index of the
//	cell in the font to print to the screen. The conversion table is built by
//	CreateEnglishTransTable()
//
//*****************************************************************************
INT16 GetIndex(UINT16 siChar)
{
	UINT16 *pTrav;
	UINT16 ssCount=0;
	UINT16	usNumberOfSymbols = pFManager->pTranslationTable->usNumberOfSymbols;

	// search the Translation Table and return the index for the font
	pTrav = pFManager->pTranslationTable->DynamicArrayOf16BitValues;
	while (ssCount < usNumberOfSymbols )
	{
		if (siChar == *pTrav)
		{
      return ssCount;
    }
		ssCount++;
		pTrav++;
	}

	// If here, present warning and give the first index
	DbgMessage(TOPIC_FONT_HANDLER, DBG_LEVEL_0, String("Error: Invalid character given %d", siChar));

	// Return 0 here, NOT -1 - we should see A's here now...
	return 0;
}


//*****************************************************************************
// SetFont
//
//	Sets the current font number.
//
//*****************************************************************************
BOOLEAN SetFont(INT32 iFontIndex)
{
	Assert(iFontIndex >= 0);
	Assert(iFontIndex <= MAX_FONTS);
	Assert(FontObjs[iFontIndex]!=NULL);

	FontDefault=iFontIndex;
	return(TRUE);
}

//*****************************************************************************
// SetFontDestBuffer
//
//	Sets the destination buffer for printing to, the clipping rectangle, and
// sets the line wrap on/off. DestBuffer is a VOBJECT handle, not a pointer.
//
//*****************************************************************************
BOOLEAN SetFontDestBuffer(UINT32 DestBuffer, INT32 x1, INT32 y1, INT32 x2, INT32 y2, BOOLEAN wrap)
{
	Assert(x2 > x1);
	Assert(y2 > y1);

	FontDestBuffer=DestBuffer;

	FontDestRegion.iLeft=x1;
	FontDestRegion.iTop=y1;
	FontDestRegion.iRight=x2;
	FontDestRegion.iBottom=y2;

	FontDestWrap=wrap;

	return(TRUE);
}

//*****************************************************************************
// mprintf
//
//	Prints to the currently selected destination buffer, at the X/Y coordinates
// specified, using the currently selected font. Other than the X/Y coordinates,
// the parameters are identical to printf. The resulting string may be no longer
// than 512 word-characters. Uses monochrome font color settings
//*****************************************************************************
template UINT32 mprintf<short *>(INT32, INT32, short *, ...);
template UINT32 mprintf<wchar_t *>(INT32, INT32, wchar_t *, ...);
template UINT32 mprintf<wchar_t const *>(INT32, INT32, wchar_t const *, ...);
template UINT32 mprintf<unsigned short *>(INT32, INT32, unsigned short *, ...);
template UINT32 mprintf<unsigned short const *>(INT32, INT32, unsigned short const *, ...);
template <typename type3>
UINT32 mprintf(INT32 x, INT32 y, type3 pFontString, ...)
{
INT32		destx, desty;
wchar_t	*curletter, transletter;
va_list argptr;
wchar_t	string[512];
UINT32			uiDestPitchBYTES;
UINT8				*pDestBuf;

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);       	// Set up variable argument pointer
	WSTR_VSPrintf(string, 512, (wchar_t *)pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	curletter=string;

	destx=x;
	desty=y;

	// Lock the dest buffer
	pDestBuf = LockVideoSurface( FontDestBuffer, &uiDestPitchBYTES );

	while((*curletter)!=0)
	{
		transletter=GetIndex(*curletter++);

		if(FontDestWrap && BltIsClipped(FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion))
		{
			destx=x;
			desty+=GetHeight(FontObjs[FontDefault], transletter);
		}

		// Blit directly
		if ( gbPixelDepth == 8 )
		{
			Blt8BPPDataTo8BPPBufferMonoShadowClip(pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground8, FontBackground8);
		}
		else
		{
			Blt8BPPDataTo16BPPBufferMonoShadowClip((UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground16, FontBackground16, FontShadow16 );
		}
		destx+=GetWidth(FontObjs[FontDefault], transletter);
	}

	// Unlock buffer
	UnLockVideoSurface( FontDestBuffer );

	return(0);
}

template void VarFindFontRightCoordinates<short *>(INT16, INT16, INT16, INT16, INT32, INT16 *, INT16 *, short *, ...);
template void VarFindFontRightCoordinates<wchar_t *>(INT16, INT16, INT16, INT16, INT32, INT16 *, INT16 *, wchar_t *, ...);
template void VarFindFontRightCoordinates<unsigned short *>(INT16, INT16, INT16, INT16, INT32, INT16 *, INT16 *, unsigned short *, ...);
template <typename type8>
void VarFindFontRightCoordinates( INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY, type8 pFontString, ... )
{
	wchar_t	string[512];
	va_list argptr;

	va_start(argptr, pFontString);       	// Set up variable argument pointer
	WSTR_VSPrintf(string, 512, (wchar_t *)pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	FindFontRightCoordinates( sLeft, sTop, sWidth, sHeight, string, iFontIndex, psNewX, psNewY );
}

template void VarFindFontCenterCoordinates<short *>(INT16, INT16, INT16, INT16, INT32, INT16 *, INT16 *, short *, ...);
template void VarFindFontCenterCoordinates<wchar_t *>(INT16, INT16, INT16, INT16, INT32, INT16 *, INT16 *, wchar_t *, ...);
template void VarFindFontCenterCoordinates<wchar_t const *>(INT16, INT16, INT16, INT16, INT32, INT16 *, INT16 *, wchar_t const *, ...);
template void VarFindFontCenterCoordinates<unsigned short *>(INT16, INT16, INT16, INT16, INT32, INT16 *, INT16 *, unsigned short *, ...);
template void VarFindFontCenterCoordinates<unsigned short const *>(INT16, INT16, INT16, INT16, INT32, INT16 *, INT16 *, unsigned short const*, ...);
template <typename type8>
void VarFindFontCenterCoordinates( INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY, type8 pFontString, ... )
{
	wchar_t	string[512];
	va_list argptr;

	va_start(argptr, pFontString);       	// Set up variable argument pointer
	WSTR_VSPrintf(string, 512, (wchar_t *)pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	FindFontCenterCoordinates( sLeft, sTop, sWidth, sHeight, string, iFontIndex, psNewX, psNewY );
}

template void FindFontRightCoordinates<short *, unsigned short *, unsigned short *>(INT16, INT16, INT16, INT16, short *, INT32, unsigned short *, unsigned short *);
template void FindFontRightCoordinates<short *, short *, unsigned short *>(INT16, INT16, INT16, INT16, short *, INT32, short *, unsigned short *);
template void FindFontRightCoordinates<wchar_t *, short *, short *>(INT16, INT16, INT16, INT16, wchar_t *, INT32, short *, short *);
template void FindFontRightCoordinates<wchar_t *, unsigned short *, unsigned short *>(INT16, INT16, INT16, INT16, wchar_t *, INT32, unsigned short *, unsigned short *);
template void FindFontRightCoordinates<wchar_t *, short *, unsigned short *>(INT16, INT16, INT16, INT16, wchar_t *, INT32, short *, unsigned short *);
template void FindFontRightCoordinates<unsigned short *, short *, short *>(INT16, INT16, INT16, INT16, unsigned short *, INT32, short *, short *);
template void FindFontRightCoordinates<unsigned short *, unsigned short *, unsigned short *>(INT16, INT16, INT16, INT16, unsigned short *, INT32, unsigned short *, unsigned short *);
template <typename string5, typename string7, typename string8>
void FindFontRightCoordinates( INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, string5 pStr, INT32 iFontIndex, string7 psNewX, string8 psNewY )
{
	INT16 xp,yp;

	// Compute the coordinates to right justify the text
	xp = ((sWidth-StringPixLength(pStr,iFontIndex))) + sLeft;
	yp = ((sHeight-GetFontHeight(iFontIndex)) / 2) + sTop;

	*psNewX = xp;
	*psNewY = yp;
}

template void FindFontCenterCoordinates<short *, unsigned short *, unsigned short *>(INT16, INT16, INT16, INT16, short *, INT32, unsigned short *, unsigned short *);
template void FindFontCenterCoordinates<unsigned short *, unsigned short *, unsigned short *>(INT16, INT16, INT16, INT16, unsigned short *, INT32, unsigned short *, unsigned short *);
template void FindFontCenterCoordinates<unsigned short *, short *, short *>(INT16, INT16, INT16, INT16, unsigned short *, INT32, short *, short *);
template void FindFontCenterCoordinates<short *, short *, short *>(INT16, INT16, INT16, INT16, short *, INT32, short *, short *);
template void FindFontCenterCoordinates<wchar_t *, short *, short *>(INT16, INT16, INT16, INT16, wchar_t *, INT32, short *, short *);
template void FindFontCenterCoordinates<wchar_t *, unsigned short *, unsigned short *>(INT16, INT16, INT16, INT16, wchar_t *, INT32, unsigned short *, unsigned short *);
template <typename string5, typename string7, typename string8>
void FindFontCenterCoordinates( INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, string5 pStr, INT32 iFontIndex, string7 psNewX, string8 psNewY )
{
	INT16 xp,yp;

	// Compute the coordinates to center the text
	xp = ((sWidth-StringPixLength(pStr,iFontIndex) + 1) / 2) + sLeft;
	yp = ((sHeight-GetFontHeight(iFontIndex)) / 2) + sTop;

	*psNewX = xp;
	*psNewY = yp;
}

//*****************************************************************************
// gprintf
//
//	Prints to the currently selected destination buffer, at the X/Y coordinates
// specified, using the currently selected font. Other than the X/Y coordinates,
// the parameters are identical to printf. The resulting string may be no longer
// than 512 word-characters.
//*****************************************************************************
UINT32 gprintf(INT32 x, INT32 y, CHAR16 *pFontString, ...)
{
INT32		destx, desty;
wchar_t	*curletter, transletter;
va_list argptr;
wchar_t	string[512];
UINT32			uiDestPitchBYTES;
UINT8				*pDestBuf;

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);       	// Set up variable argument pointer
	WSTR_VSPrintf(string, 512, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	curletter=string;

	destx=x;
	desty=y;

	// Lock the dest buffer
	pDestBuf = LockVideoSurface( FontDestBuffer, &uiDestPitchBYTES );

	while((*curletter)!=0)
	{
		transletter=GetIndex(*curletter++);

		if(FontDestWrap && BltIsClipped(FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion))
		{
			destx=x;
			desty+=GetHeight(FontObjs[FontDefault], transletter);
		}

		// Blit directly
		if ( gbPixelDepth == 8 )
		{
			Blt8BPPDataTo8BPPBufferTransparentClip( (UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion );
		}
		else
		{
			Blt8BPPDataTo16BPPBufferTransparentClip( (UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion );
		}
		destx+=GetWidth(FontObjs[FontDefault], transletter);
	}

	// Unlock buffer
	UnLockVideoSurface( FontDestBuffer );

	return(0);
}

UINT32 gprintfDirty(INT32 x, INT32 y, UINT16 *pFontString, ...)
{
INT32		destx, desty;
wchar_t	*curletter, transletter;
va_list argptr;
wchar_t	string[512];
UINT32			uiDestPitchBYTES;
UINT8				*pDestBuf;

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);       	// Set up variable argument pointer
	WSTR_VSPrintf(string, 512, (wchar_t*) pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	curletter=string;

	destx=x;
	desty=y;

	// Lock the dest buffer
	pDestBuf = LockVideoSurface( FontDestBuffer, &uiDestPitchBYTES );

	while((*curletter)!=0)
	{
		transletter=GetIndex(*curletter++);

		if(FontDestWrap && BltIsClipped(FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion))
		{
			destx=x;
			desty+=GetHeight(FontObjs[FontDefault], transletter);
		}

		// Blit directly
		if ( gbPixelDepth == 8 )
		{
			Blt8BPPDataTo8BPPBufferTransparentClip( (UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion );
		}
		else
		{
			Blt8BPPDataTo16BPPBufferTransparentClip( (UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion );
		}
		destx+=GetWidth(FontObjs[FontDefault], transletter);
	}

	// Unlock buffer
	UnLockVideoSurface( FontDestBuffer );

#if defined ( JA2 ) || defined( UTIL )
	InvalidateRegion(x, y, 
										x + StringPixLength(string, FontDefault),
										y + GetFontHeight(FontDefault));
#else
	InvalidateRegion(x, y, 
										x + StringPixLength(string, FontDefault),
										y + GetFontHeight(FontDefault),
										INVAL_SRC_TRANS);
#endif

	return(0);
}
//*****************************************************************************
// gprintf_buffer
//
//	Prints to the currently selected destination buffer, at the X/Y coordinates
// specified, using the currently selected font. Other than the X/Y coordinates,
// the parameters are identical to printf. The resulting string may be no longer
// than 512 word-characters.
//*****************************************************************************
UINT32 gprintf_buffer( UINT8 *pDestBuf, UINT32 uiDestPitchBYTES, UINT32 FontType, INT32 x, INT32 y, UINT16 *pFontString, ...)
{
INT32		destx, desty;
wchar_t	*curletter, transletter;
va_list argptr;
wchar_t	string[512];

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);       	// Set up variable argument pointer
	WSTR_VSPrintf(string, 512, (wchar_t*) pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	curletter=string;

	destx=x;
	desty=y;

	while((*curletter)!=0)
	{
		transletter=GetIndex(*curletter++);

		if(FontDestWrap && BltIsClipped(FontObjs[FontType], destx, desty, transletter, &FontDestRegion))
		{
			destx=x;
			desty+=GetHeight(FontObjs[FontType], transletter);
		}

		// Blit directly
		if ( gbPixelDepth == 8 )
		{
			Blt8BPPDataTo8BPPBufferTransparentClip( (UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion );
		}
		else
		{
			Blt8BPPDataTo16BPPBufferTransparentClip( (UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion );
		}

		destx+=GetWidth(FontObjs[FontType], transletter);
	}

	return(0);
}

template UINT32	mprintf_buffer<short *>(UINT8 *, UINT32, UINT32, INT32, INT32, short *, ...);
template UINT32	mprintf_buffer<wchar_t *>(UINT8 *, UINT32, UINT32, INT32, INT32, wchar_t *, ...);
template UINT32	mprintf_buffer<wchar_t const *>(UINT8 *, UINT32, UINT32, INT32, INT32, wchar_t const *, ...);
template UINT32	mprintf_buffer<unsigned short *>(UINT8 *, UINT32, UINT32, INT32, INT32, unsigned short *, ...);
template UINT32	mprintf_buffer<unsigned short const*>(UINT8 *, UINT32, UINT32, INT32, INT32, unsigned short const*, ...);
template <typename string6>
UINT32	mprintf_buffer( UINT8 *pDestBuf, UINT32 uiDestPitchBYTES, UINT32 FontType, INT32 x, INT32 y, string6 pFontString, ...)
{
INT32		destx, desty;
wchar_t	*curletter, transletter;
va_list argptr;
wchar_t	string[512];

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);       	// Set up variable argument pointer
	WSTR_VSPrintf(string, 512, (wchar_t *)pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	curletter=string;

	destx=x;
	desty=y;

	while((*curletter)!=0)
	{
		transletter=GetIndex(*curletter++);

		if(FontDestWrap && BltIsClipped(FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion))
		{
			destx=x;
			desty+=GetHeight(FontObjs[FontDefault], transletter);
		}

		// Blit directly
		if ( gbPixelDepth == 8 )
		{
			Blt8BPPDataTo8BPPBufferMonoShadowClip(pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground8, FontBackground8);
		}
		else
		{
			Blt8BPPDataTo16BPPBufferMonoShadowClip((UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground16, FontBackground16, FontShadow16 );
		}
		destx+=GetWidth(FontObjs[FontDefault], transletter);
	}

	return(0);
}

template UINT32 mprintf_buffer_coded<short *>(UINT8 *, UINT32, UINT32, INT32, INT32, short *, ...);
template UINT32 mprintf_buffer_coded<wchar_t *>(UINT8 *, UINT32, UINT32, INT32, INT32, wchar_t *, ...);
template <typename string6>
UINT32 mprintf_buffer_coded( UINT8 *pDestBuf, UINT32 uiDestPitchBYTES, UINT32 FontType, INT32 x, INT32 y, string6 pFontString, ...)
{
INT32		destx, desty;
wchar_t	*curletter, transletter;
va_list argptr;
wchar_t	string[512];
UINT16	usOldForeColor;

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);       	// Set up variable argument pointer
	WSTR_VSPrintf(string, 512, (wchar_t *)pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	curletter=string;

	destx=x;
	desty=y;

	usOldForeColor = FontForeground16;

	while((*curletter)!=0)
	{
		if ( (*curletter) == 180 )
		{
			curletter++;
			SetFontForeground( (UINT8)(*curletter) );
			curletter++;
		}
		else if ( (*curletter) == 181 )
		{
			FontForeground16 = usOldForeColor;
			curletter++;
		}

		transletter=GetIndex(*curletter++);


		if(FontDestWrap && BltIsClipped(FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion))
		{
			destx=x;
			desty+=GetHeight(FontObjs[FontDefault], transletter);
		}

		// Blit directly
		if ( gbPixelDepth == 8 )
		{
			Blt8BPPDataTo8BPPBufferMonoShadowClip(pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground8, FontBackground8);
		}
		else
		{
			Blt8BPPDataTo16BPPBufferMonoShadowClip((UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground16, FontBackground16, FontShadow16 );
		}
		destx+=GetWidth(FontObjs[FontDefault], transletter);
	}

	return(0);
}


UINT32 mprintf_coded( INT32 x, INT32 y, CHAR16 *pFontString, ...)
{
INT32		destx, desty;
wchar_t	*curletter, transletter;
va_list argptr;
wchar_t	string[512];
UINT16	usOldForeColor;
UINT32			uiDestPitchBYTES;
UINT8				*pDestBuf;

	Assert(pFontString!=NULL);

	va_start(argptr, pFontString);       	// Set up variable argument pointer
	WSTR_VSPrintf(string, 512, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	curletter=string;

	destx=x;
	desty=y;

	usOldForeColor = FontForeground16;

	// Lock the dest buffer
	pDestBuf = LockVideoSurface( FontDestBuffer, &uiDestPitchBYTES );

	while((*curletter)!=0)
	{
		if ( (*curletter) == 180 )
		{
			curletter++;
			SetFontForeground( (UINT8)(*curletter) );
			curletter++;
		}
		else if ( (*curletter) == 181 )
		{
			FontForeground16 = usOldForeColor;
			curletter++;
		}

		transletter=GetIndex(*curletter++);


		if(FontDestWrap && BltIsClipped(FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion))
		{
			destx=x;
			desty+=GetHeight(FontObjs[FontDefault], transletter);
		}

		// Blit directly
		if ( gbPixelDepth == 8 )
		{
			Blt8BPPDataTo8BPPBufferMonoShadowClip(pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground8, FontBackground8);
		}
		else
		{
			Blt8BPPDataTo16BPPBufferMonoShadowClip((UINT16*)pDestBuf, uiDestPitchBYTES, FontObjs[FontDefault], destx, desty, transletter, &FontDestRegion, FontForeground16, FontBackground16, FontShadow16 );
		}
		destx+=GetWidth(FontObjs[FontDefault], transletter);
	}

	// Unlock buffer
	UnLockVideoSurface( FontDestBuffer );

	return(0);
}



//*****************************************************************************
// InitializeFontManager
//
//	Starts up the font manager system with the appropriate translation table.
//
//*****************************************************************************
BOOLEAN InitializeFontManager(UINT16 usDefaultPixelDepth, FontTranslationTable *pTransTable)
{
FontTranslationTable *pTransTab;
int count;
UINT16 uiRight, uiBottom;
UINT8 uiPixelDepth;

	FontDefault=(-1);
	FontDestBuffer=BACKBUFFER;
	FontDestPitch=0;

	//	FontDestBPP=0;

	GetCurrentVideoSettings( &uiRight, &uiBottom, &uiPixelDepth );
	FontDestRegion.iLeft = 0;
	FontDestRegion.iTop = 0;
	FontDestRegion.iRight=(INT32)uiRight;
	FontDestRegion.iBottom=(INT32)uiBottom;
	FontDestBPP=(UINT32)uiPixelDepth;


	FontDestWrap=FALSE;

	// register the appropriate debug topics
	if(pTransTable == NULL)
	{
    return FALSE;
  }
	RegisterDebugTopic(TOPIC_FONT_HANDLER, "Font Manager");

	if ((pFManager = (FontManager *)MemAlloc(sizeof(FontManager)))==NULL)
	{
    return FALSE;
  }

	if((pTransTab = (FontTranslationTable *)MemAlloc(sizeof(FontTranslationTable)))==NULL)
	{
    return FALSE;
  }

	pFManager->pTranslationTable = pTransTab;
	pFManager->usDefaultPixelDepth = usDefaultPixelDepth;
	pTransTab->usNumberOfSymbols = pTransTable->usNumberOfSymbols;
  pTransTab->DynamicArrayOf16BitValues = pTransTable->DynamicArrayOf16BitValues;

	// Mark all font slots as empty
	for(count=0; count < MAX_FONTS; count++)
		FontObjs[count]=NULL;

	return TRUE;
}


//*****************************************************************************
// ShutdownFontManager
//
//	Shuts down, and deallocates all fonts.
//*****************************************************************************
void ShutdownFontManager(void)
{
  INT32 count;

	UnRegisterDebugTopic(TOPIC_FONT_HANDLER, "Font Manager");
  if(pFManager)
		MemFree(pFManager);

	for(count=0; count < MAX_FONTS; count++)
	{
		if(FontObjs[count]!=NULL)
			UnloadFont(count);
	}
}


//*****************************************************************************
// DestroyEnglishTransTable
//
// Destroys the English text->font map table.
//*****************************************************************************
void DestroyEnglishTransTable( void )
{
	if(pFManager)
	{
		if (pFManager->pTranslationTable != NULL)
		{
			if (pFManager->pTranslationTable->DynamicArrayOf16BitValues != NULL)
			{
				MemFree( pFManager->pTranslationTable->DynamicArrayOf16BitValues );
			}

			MemFree( pFManager->pTranslationTable );

			pFManager->pTranslationTable = NULL;
		}
	}
}


//*****************************************************************************
// CreateEnglishTransTable
//
// Creates the English text->font map table.
//*****************************************************************************
FontTranslationTable *CreateEnglishTransTable(  )
{
	FontTranslationTable *pTable = NULL;
	UINT16	*temp;

  pTable = (FontTranslationTable *)MemAlloc(sizeof(FontTranslationTable));
	#ifdef JA2
		// ha ha, we have more than Wizardry now (again)
		pTable->usNumberOfSymbols = 255;
	#else
		pTable->usNumberOfSymbols = 255;
	#endif
	pTable->DynamicArrayOf16BitValues = (UINT16 *)MemAlloc(pTable->usNumberOfSymbols * 2);
	temp = pTable->DynamicArrayOf16BitValues;

	*temp = 'A';
	temp++;
	*temp='B';
	temp++;
	*temp ='C';
	temp++;
	*temp ='D';
	temp++;
	*temp = 'E';
	temp++;
	*temp = 'F';
	temp++;
	*temp='G';
	temp++;
	*temp ='H';
	temp++;
	*temp ='I';
	temp++;
	*temp = 'J';
	temp++;
	*temp = 'K';
	temp++;
	*temp='L';
	temp++;
	*temp ='M';
	temp++;
	*temp ='N';
	temp++;
	*temp = 'O';
	temp++;
	*temp = 'P';
	temp++;
	*temp='Q';
	temp++;
	*temp ='R';
	temp++;
	*temp ='S';
	temp++;
	*temp = 'T';
	temp++;
	*temp ='U';
	temp++;
	*temp ='V';
	temp++;
	*temp = 'W';
	temp++;
	*temp = 'X';
	temp++;
	*temp='Y';
	temp++;
	*temp ='Z';
	temp++;
	*temp ='a';
	temp++;
	*temp = 'b';
	temp++;
	*temp ='c';
	temp++;
	*temp = 'd';
	temp++;
	*temp ='e';
	temp++;
	*temp = 'f';
	temp++;
	*temp ='g';
	temp++;
	*temp = 'h';
	temp++;
	*temp ='i';
	temp++;
	*temp = 'j';
	temp++;
	*temp ='k';
	temp++;
	*temp = 'l';
	temp++;
	*temp ='m';
	temp++;
	*temp = 'n';
	temp++;
	*temp ='o';
	temp++;
	*temp = 'p';
	temp++;
	*temp ='q';
	temp++;
	*temp = 'r';
	temp++;
	*temp ='s';
	temp++;
	*temp = 't';
	temp++;
	*temp ='u';
	temp++;
	*temp = 'v';
	temp++;
	*temp ='w';
	temp++;
	*temp = 'x';
	temp++;
	*temp ='y';
	temp++;
	*temp = 'z';
	temp++;
	*temp ='0';
	temp++;
	*temp = '1';
	temp++;
	*temp ='2';
	temp++;
	*temp = '3';
	temp++;
	*temp ='4';
	temp++;
	*temp = '5';
	temp++;
	*temp ='6';
	temp++;
	*temp = '7';
	temp++;
	*temp ='8';
	temp++;
	*temp = '9';
	temp++;
	*temp ='!';
	temp++;
	*temp = '@';
	temp++;
	*temp ='#';
	temp++;
	*temp = '$';
	temp++;
	*temp ='%';
	temp++;
	*temp = '^';
	temp++;
	*temp ='&';
	temp++;
	*temp = '*';
	temp++;
	*temp ='(';
	temp++;
	*temp = ')';
	temp++;
	*temp ='-';
	temp++;
	*temp = '_';
	temp++;
	*temp ='+';
	temp++;
	*temp = '=';
	temp++;
	*temp ='|';
	temp++;
	*temp = '\\';
	temp++;
	*temp ='{';
	temp++;
	*temp = '}';// 80
	temp++;
	*temp = '[';
	temp++;
	*temp =']';
	temp++;
	*temp = ':';
	temp++;
	*temp =';';
	temp++;
	*temp = '"';
	temp++;
	*temp ='\'';
	temp++;
	*temp = '<';
	temp++;
	*temp = '>';
	temp++;
	*temp =',';
	temp++;
	*temp = '.';
	temp++;
	*temp ='?';
	temp++;
	*temp = '/';
	temp++;
	*temp = ' '; //93
	temp++;

#ifdef JA2

#ifdef GERMAN
	*temp = 196; // "A" umlaut
	temp++;
	*temp = 214; // "O" umlaut
	temp++;
	*temp = 220; // "U" umlaut
	temp++;
	*temp = 228; // "a" umlaut
	temp++;
	*temp = 246; // "o" umlaut
	temp++;
	*temp = 252; // "u" umlaut
	temp++;
	*temp = 223; // double-s that looks like a beta/B  // 100
	temp++;
#else
	*temp = 0; // "A" umlaut
	temp++;
	*temp = 0; // "O" umlaut
	temp++;
	*temp = 0; // "U" umlaut
	temp++;
	*temp = 0; // "a" umlaut
	temp++;
	*temp = 0; // "o" umlaut
	temp++;
	*temp = 0; // "u" umlaut
	temp++;
	*temp = 0; // double-s that looks like a beta/B  // 100
	temp++;
#endif

	*temp = 192; // � 
	temp++;
	*temp = 193; // � 
	temp++;
	*temp = 194; // �
	temp++;
	*temp = 195; // �
	temp++;
	*temp = 196; // �
	temp++;
    *temp = 197; // �
	temp++;
	*temp = 198; // �
	temp++;
	*temp = 199; // �
	temp++;
	*temp = 200; // �				140
	temp++;
	*temp = 201; // �
	temp++;
	*temp = 202; // �
	temp++;
	*temp = 203; // �
	temp++;
	*temp = 204; // �
	temp++;
	*temp = 205; // �
	temp++;
	*temp = 206; // �
	temp++;
	*temp = 207; // �
	temp++;
	*temp = 208; // �
	temp++;
	*temp = 209; // �
	temp++;
	*temp = 210; // �				150
	temp++;
	*temp = 211; // �
	temp++;
	*temp = 212; // �
	temp++;
	*temp = 213; // �
	temp++;
	*temp = 214; // �
	temp++;
	*temp = 215; // �
	temp++;
	*temp = 216; // �
	temp++;
	*temp = 217; // �
	temp++;
	*temp = 218; // �
	temp++;
	*temp = 219; // �
	temp++;
	*temp = 220; // �				160
	temp++;
	*temp = 221; // �
	temp++;
	*temp = 222; // �
	temp++;
	*temp = 223; // �
	temp++;
	*temp = 224; // �
	temp++;
	*temp = 225; // �
	temp++;
	*temp = 226; // �
	temp++;
	*temp = 227; // �
	temp++;
	*temp = 228; // �
	temp++;
	*temp = 229; // �
	temp++;
	*temp = 230; // �				170
	temp++;
	*temp = 231; // �
	temp++;
	*temp = 232; // �
	temp++;
	*temp = 233; // �
	temp++;
	*temp = 234; // �
	temp++;
	*temp = 235; // �
	temp++;
	*temp = 236; // �
	temp++;
	*temp = 237; // �
	temp++;
	*temp = 238; // �
	temp++;
	*temp = 239; // �				170
	temp++;
	*temp = 240; // �
	temp++;
	*temp = 241; // �
	temp++;
	*temp = 242; // �
	temp++;
	*temp = 243; // �
	temp++;
	*temp = 244; // �
	temp++;
	*temp = 245; // �
	temp++;
	*temp = 246; // �
	temp++;
	*temp = 247; // �
	temp++;
	*temp = 248; // �
	temp++;
	*temp = 249; // �				170
	temp++;
	*temp = 250; // �
	temp++;
	*temp = 251; // �
	temp++;
	*temp = 252; // �
	temp++;
	*temp = 253; // �				170
	temp++;
	*temp = 254; // �
	temp++;
	*temp = 255; // �
	temp++;
	*temp = 256; // �
	temp++;


	// START OF FUNKY RUSSIAN STUFF
	*temp = 1041;
	temp++;
	*temp = 1042;
	temp++;
	*temp = 1043;
	temp++;
	*temp = 1044;
	temp++;
	*temp = 1045;
	temp++;
	*temp = 1046;
	temp++;
	*temp = 1047;
	temp++;
	*temp = 1048;
	temp++;
	*temp = 1049;				//110
	temp++;
	*temp = 1050;
	temp++;
	*temp = 1051;
	temp++;
	*temp = 1052;
	temp++;
	*temp = 1053;
	temp++;
	*temp = 1054;
	temp++;
	*temp = 1055;
	temp++;
	*temp = 1056;
	temp++;
	*temp = 1057;
	temp++;
	*temp = 1058;
	temp++;
	*temp = 1059;				//120
	temp++;
	*temp = 1060;
	temp++;
	*temp = 1061;
	temp++;
	*temp = 1062;
	temp++;
	*temp = 1063;
	temp++;
	*temp = 1064;
	temp++;
	*temp = 1065;
	temp++;
	*temp = 1066;
	temp++;
	*temp = 1067;
	temp++;
	*temp = 1068;
	temp++;
	*temp = 1069; // 130
	temp++;
	*temp = 1070;
	temp++;
	*temp = 1071;
	temp++;
	*temp = 1072; // � 
    temp++;
	*temp = 1073; // � 
	temp++;
	*temp = 1074; // � 
	temp++;
	*temp = 1075; // �
	temp++;
	*temp = 1076; // �
	temp++;
	*temp = 1077; // �
	temp++;
	*temp = 1078; // �
	temp++;
	*temp = 1079; // �				140
	temp++;
	*temp = 1080; // �
	temp++;
	*temp = 1081; // �
	temp++;
	*temp = 1082; // �				140
	temp++;
	*temp = 1083; // �
	temp++;
	*temp = 1084; // �
	temp++;
	*temp = 1085; // �
	temp++;
	*temp = 1086; // �
	temp++;
	*temp = 1087; // �
	temp++;
	*temp = 1088; // �
	temp++;
	*temp = 1089; // �				150
	temp++;
	*temp = 1090; // �
	temp++;
	*temp = 1091; // �
	temp++;
	*temp = 1092; // �				150
	temp++;
	*temp = 1093; // �
	temp++;
	*temp = 1094; // �
	temp++;
	*temp = 1095; // �
	temp++;
	*temp = 1096; // �
	temp++;
	*temp = 1097; // �
	temp++;
	*temp = 1098; // �
	temp++;
	*temp = 1099; // �				160
	temp++;
	*temp = 1100; // �
	temp++;
	*temp = 1101; // �
	temp++;
	*temp = 1102; // �				160
	temp++;
	*temp = 1103; // �
	temp++;

#else
// Windows Code Page 1252 Western Standard Character Set

	*temp = 193;	// "A" acute
	temp++;
	*temp = 192;	// "A" grave
	temp++;
	*temp = 193;	// "A" circumflex
	temp++;
	*temp = 196;	// "A" umlaut
	temp++;
	*temp = 195;	// "A" tilde
	temp++;
	*temp = 197;	// "A" ring
	temp++;
	*temp = 199;	// "C" cedile
	temp++;
	*temp = 201;	// "E" acute
	temp++;
	*temp = 200;	// "E" grave
	temp++;
	*temp = 202;	// "E" circumflex
	temp++;
	*temp = 203;	// "E" umlaut
	temp++;
	*temp = 205; // "I" acute
	temp++;
	*temp = 204;	// "I" grave
	temp++;
	*temp = 206;	// "I" circumflex
	temp++;
	*temp = 207;	// "I" umlaut
	temp++;
	*temp = 209;	// "N" tilde
	temp++;
	*temp = 211;	// "O" acute
	temp++;
	*temp = 210;	// "O" grave
	temp++;
	*temp = 212;	// "O" circumflex
	temp++;
	*temp = 214;	// "O" umlaut
	temp++;
	*temp = 213;	// "O" tilde
	temp++;
	*temp = 216;	// "0" O strike-through
	temp++;
	*temp = 218;	// "U" acute
	temp++;
	*temp = 217;	// "U" grave
	temp++;
	*temp = 219;	// "U" circumflex
	temp++;
	*temp = 220;	// "U" umlaut
	temp++;
	*temp = 221;	// "Y" acute
	temp++;
	*temp = 225;	// "a" acute
	temp++;
	*temp = 224;	// "a" grave
	temp++;
	*temp = 226;	// "a" circumflex
	temp++;
	*temp = 228;	// "a" umlaut
	temp++;
	*temp = 227;	// "a" tilde
	temp++;
	*temp = 229;	// "a" ring
	temp++;
	*temp = 231;	// "c" cedile
	temp++;
	*temp = 233;	// "e" acute
	temp++;
	*temp = 232;	// "e" grave
	temp++;
	*temp = 234;	// "e" circumflex
	temp++;
	*temp = 235;	// "e" umlaut
	temp++;
	*temp = 237;	// "i" acute
	temp++;
	*temp = 236;	// "i" grave
	temp++;
	*temp = 238;	// "i" circumflex
	temp++;
	*temp = 239;	// "i" umlaut
	temp++;
	*temp = 241;	// "n" tilde
	temp++;
	*temp = 243;	// "o" acute
	temp++;
	*temp = 242;	// "o" grave
	temp++;
	*temp = 244;	// "o" circumflex
	temp++;
	*temp = 246;	// "o" umlaut
	temp++;
	*temp = 245;	// "o" tilde
	temp++;
	*temp = 248;	// "o" strike-through
	temp++;
	*temp = 250;	// "u" acute
	temp++;
	*temp = 249;	// "u" grave
	temp++;
	*temp = 251;	// "u" circumflex
	temp++;
	*temp = 252;	// "u" umlaut
	temp++;
	*temp = 254;	// "y" acute
	temp++;
	*temp = 255;	// "y" umlaut
	temp++;
	*temp = 223;	// beta	

// Font glyphs for spell targeting icons
	//ATE: IMPORTANT! INcreate the array above if you add any new items here...
	temp++;
	*temp = FONT_GLYPH_TARGET_POINT;
	temp++;
	*temp = FONT_GLYPH_TARGET_CONE;
	temp++;
	*temp = FONT_GLYPH_TARGET_SINGLE;
	temp++;
	*temp = FONT_GLYPH_TARGET_GROUP;
	temp++;
	*temp = FONT_GLYPH_TARGET_NONE;

// 154
#endif

	   return pTable;
}
