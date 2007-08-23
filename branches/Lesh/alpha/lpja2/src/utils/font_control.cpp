#ifdef PRECOMPILEDHEADERS
	#include "utils_all.h"
  #include "winfont.h"
#else
	#include "platform.h"
	#include "sgp.h"
	#include "image.h"
	#include "vsurface.h"
	#include "vsurface_private.h"
	#include "wcheck.h"
	#include "font_control.h"
#endif

INT32          giCurWinFont = 0;
BOOLEAN        gfUseWinFonts = FALSE;


// Global variables for video objects
INT32						gpLargeFontType1;
HVOBJECT				gvoLargeFontType1;

INT32						gpSmallFontType1;
HVOBJECT				gvoSmallFontType1;

INT32						gpTinyFontType1;
HVOBJECT				gvoTinyFontType1;

INT32						gp12PointFont1;
HVOBJECT				gvo12PointFont1;

INT32           gpClockFont;
HVOBJECT        gvoClockFont;

INT32           gpCompFont;
HVOBJECT        gvoCompFont;

INT32           gpSmallCompFont;
HVOBJECT        gvoSmallCompFont;

INT32						gp10PointRoman;
HVOBJECT				gvo10PointRoman;

INT32						gp12PointRoman;
HVOBJECT				gvo12PointRoman;

INT32						gp14PointSansSerif;
HVOBJECT				gvo14PointSansSerif;

//INT32						gpMilitaryFont1;
//HVOBJECT				gvoMilitaryFont1;

INT32						gp10PointArial;
HVOBJECT				gvo10PointArial;

INT32						gp10PointArialBold;
HVOBJECT				gvo10PointArialBold;

INT32						gp14PointArial;
HVOBJECT				gvo14PointArial;

INT32						gp12PointArial;
HVOBJECT				gvo12PointArial;

INT32           gpBlockyFont;
HVOBJECT				gvoBlockyFont;

INT32           gpBlockyFont2;
HVOBJECT				gvoBlockyFont2;

INT32						gp12PointArialFixedFont;
HVOBJECT				gvo12PointArialFixedFont;

INT32						gp16PointArial;
HVOBJECT				gvo16PointArial;

INT32						gpBlockFontNarrow;
HVOBJECT				gvoBlockFontNarrow;

INT32						gp14PointHumanist;
HVOBJECT				gvo14PointHumanist;

#if defined( JA2EDITOR ) && defined( ENGLISH )
	INT32					gpHugeFont;
	HVOBJECT			gvoHugeFont;
#endif

INT32          giSubTitleWinFont;



BOOLEAN					gfFontsInit = FALSE;

UINT16 CreateFontPaletteTables(HVOBJECT pObj );


extern UINT16 gzFontName[32];

BOOLEAN	InitializeFonts( )
{
	//INT16   zWinFontName[128]; // unused (jonathanl)
	//COLORVAL Color;			 // usused (jonathanl)

	// Initialize fonts
//	gpLargeFontType1  = LoadFontFile( "FONTS\\lfont1.STI" );
	gpLargeFontType1  = LoadFontFile( "FONTS\\LARGEFONT1.STI" );
	gvoLargeFontType1 = GetFontObject( gpLargeFontType1 );
	CHECKF( gvoLargeFontType1 );
	CHECKF( CreateFontPaletteTables( gvoLargeFontType1 ) );

//	gpSmallFontType1  = LoadFontFile( "FONTS\\6b-font.STI" );
	gpSmallFontType1  = LoadFontFile( "FONTS\\SMALLFONT1.STI" );
	gvoSmallFontType1 = GetFontObject( gpSmallFontType1 );
	CHECKF( gvoSmallFontType1 );
	CHECKF( CreateFontPaletteTables( gvoSmallFontType1 ) );

//	gpTinyFontType1  = LoadFontFile( "FONTS\\tfont1.STI" );
	gpTinyFontType1  = LoadFontFile( "FONTS\\TINYFONT1.STI" );
	gvoTinyFontType1 = GetFontObject( gpTinyFontType1 );
	CHECKF( gvoTinyFontType1 );
	CHECKF( CreateFontPaletteTables( gvoTinyFontType1 ) );

//	gp12PointFont1	= LoadFontFile( "FONTS\\font-12.STI" );
	gp12PointFont1	= LoadFontFile( "FONTS\\FONT12POINT1.STI" );
	gvo12PointFont1	= GetFontObject( gp12PointFont1 );
	CHECKF( gvo12PointFont1 );
	CHECKF( CreateFontPaletteTables( gvo12PointFont1 ) );


//  gpClockFont  = LoadFontFile( "FONTS\\DIGI.STI" );
  gpClockFont  = LoadFontFile( "FONTS\\CLOCKFONT.STI" );
  gvoClockFont = GetFontObject( gpClockFont );
  CHECKF( gvoClockFont );
  CHECKF( CreateFontPaletteTables( gvoClockFont ) );

//  gpCompFont  = LoadFontFile( "FONTS\\compfont.STI" );
  gpCompFont  = LoadFontFile( "FONTS\\COMPFONT.STI" );
  gvoCompFont = GetFontObject( gpCompFont );
  CHECKF( gvoCompFont );
  CHECKF( CreateFontPaletteTables( gvoCompFont ) );

//  gpSmallCompFont  = LoadFontFile( "FONTS\\scfont.STI" );
  gpSmallCompFont  = LoadFontFile( "FONTS\\SMALLCOMPFONT.STI" );
  gvoSmallCompFont = GetFontObject( gpSmallCompFont );
  CHECKF( gvoSmallCompFont );
  CHECKF( CreateFontPaletteTables( gvoSmallCompFont ) );

//  gp10PointRoman  = LoadFontFile( "FONTS\\Roman10.STI" );
  gp10PointRoman  = LoadFontFile( "FONTS\\FONT10ROMAN.STI" );
  gvo10PointRoman = GetFontObject( gp10PointRoman );
  CHECKF( gvo10PointRoman );
  CHECKF( CreateFontPaletteTables( gvo10PointRoman ) );

//  gp12PointRoman  = LoadFontFile( "FONTS\\Roman12.STI" );
  gp12PointRoman  = LoadFontFile( "FONTS\\FONT12ROMAN.STI" );
  gvo12PointRoman = GetFontObject( gp12PointRoman );
  CHECKF( gvo12PointRoman );
  CHECKF( CreateFontPaletteTables( gvo12PointRoman ) );

//  gp14PointSansSerif  = LoadFontFile( "FONTS\\SansSerif14.STI" );
  gp14PointSansSerif  = LoadFontFile( "FONTS\\FONT14SANSERIF.STI" );
  gvo14PointSansSerif = GetFontObject( gp14PointSansSerif);
  CHECKF( gvo14PointSansSerif );
  CHECKF( CreateFontPaletteTables( gvo14PointSansSerif) );

//	DEF:	Removed.  Replaced with BLOCKFONT
//  gpMilitaryFont1  = LoadFontFile( "FONTS\\milfont.STI" );
//  gvoMilitaryFont1 = GetFontObject( gpMilitaryFont1);
//  CHECKF( CreateFontPaletteTables( gvoMilitaryFont1) );


//  gp10PointArial  = LoadFontFile( "FONTS\\Arial10.STI" );
  gp10PointArial  = LoadFontFile( "FONTS\\FONT10ARIAL.STI" );
  gvo10PointArial = GetFontObject( gp10PointArial);
  CHECKF( gvo10PointArial );
  CHECKF( CreateFontPaletteTables( gvo10PointArial) );

//  gp14PointArial  = LoadFontFile( "FONTS\\Arial14.STI" );
  gp14PointArial  = LoadFontFile( "FONTS\\FONT14ARIAL.STI" );
  gvo14PointArial = GetFontObject( gp14PointArial);
  CHECKF( gvo14PointArial );
  CHECKF( CreateFontPaletteTables( gvo14PointArial) );

//  gp10PointArialBold  = LoadFontFile( "FONTS\\Arial10Bold2.STI" );
  gp10PointArialBold  = LoadFontFile( "FONTS\\FONT10ARIALBOLD.STI" );
  gvo10PointArialBold  = GetFontObject( gp10PointArialBold);
  CHECKF( gvo10PointArialBold );
  CHECKF( CreateFontPaletteTables( gvo10PointArialBold) );

//  gp12PointArial  = LoadFontFile( "FONTS\\Arial12.STI" );
  gp12PointArial  = LoadFontFile( "FONTS\\FONT12ARIAL.STI" );
  gvo12PointArial = GetFontObject( gp12PointArial);
  CHECKF( gvo12PointArial );
  CHECKF( CreateFontPaletteTables( gvo12PointArial) );
  
//	gpBlockyFont  = LoadFontFile( "FONTS\\FONT2.STI" );
	gpBlockyFont  = LoadFontFile( "FONTS\\BLOCKFONT.STI" );
  gvoBlockyFont = GetFontObject( gpBlockyFont);
  CHECKF( gvoBlockyFont );
  CHECKF( CreateFontPaletteTables( gvoBlockyFont) );
 
//	gpBlockyFont2  = LoadFontFile( "FONTS\\interface_font.STI" );
	gpBlockyFont2  = LoadFontFile( "FONTS\\BLOCKFONT2.STI" );
  gvoBlockyFont2 = GetFontObject( gpBlockyFont2);
  CHECKF( gvoBlockyFont2 );
  CHECKF( CreateFontPaletteTables( gvoBlockyFont2) );

//	gp12PointArialFixedFont = LoadFontFile( "FONTS\\Arial12FixedWidth.STI" );
	gp12PointArialFixedFont = LoadFontFile( "FONTS\\FONT12ARIALFIXEDWIDTH.STI" );
	gvo12PointArialFixedFont = GetFontObject( gp12PointArialFixedFont );
	CHECKF( gvo12PointArialFixedFont );
	CHECKF( CreateFontPaletteTables( gvo12PointArialFixedFont ) );
	
	gp16PointArial = LoadFontFile( "FONTS\\FONT16ARIAL.STI" );
	gvo16PointArial = GetFontObject( gp16PointArial );
	CHECKF( gvo16PointArial );
	CHECKF( CreateFontPaletteTables( gvo16PointArial ) );
	
	gpBlockFontNarrow = LoadFontFile( "FONTS\\BLOCKFONTNARROW.STI" );
	gvoBlockFontNarrow = GetFontObject( gpBlockFontNarrow );
	CHECKF( gvoBlockFontNarrow );
	CHECKF( CreateFontPaletteTables( gvoBlockFontNarrow ) );

	gp14PointHumanist = LoadFontFile( "FONTS\\FONT14HUMANIST.STI" );
	gvo14PointHumanist = GetFontObject( gp14PointHumanist );
	CHECKF( gvo14PointHumanist );
	CHECKF( CreateFontPaletteTables( gvo14PointHumanist ) );

	#if defined( JA2EDITOR ) && defined( ENGLISH )
		gpHugeFont = LoadFontFile( "FONTS\\HUGEFONT.STI" );
		gvoHugeFont = GetFontObject( gpHugeFont );
		CHECKF( gvoHugeFont );
		CHECKF( CreateFontPaletteTables( gvoHugeFont ) );
	#endif
	
	// Set default for font system
	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	gfFontsInit = TRUE;

  // ATE: Init WinFont System and any winfonts we wish...
#ifdef WINFONTS

	InitWinFonts( );

  //giSubTitleWinFont = CreateWinFont( -16, 0, 0,  0, FALSE, FALSE, FALSE, L"標楷體", CHINESEBIG5_CHARSET );
	giSubTitleWinFont = CreateWinFont( -16, 0, 0,  0, FALSE, FALSE, FALSE, L"新細明體", CHINESEBIG5_CHARSET );

  SET_USE_WINFONTS( TRUE );
  SET_WINFONT( giSubTitleWinFont ); 
  Color = FROMRGB( 255, 255, 255 );
  SetWinFontForeColor( giSubTitleWinFont, &Color );
	PrintWinFont( FRAME_BUFFER, giSubTitleWinFont, 10, 100, L"Font %ls initialized", gzFontName );
	InvalidateScreen();
	RefreshScreen( NULL );
  SET_USE_WINFONTS( FALSE );

#endif

	return( TRUE );
}

void ShutdownFonts( )
{
	UnloadFont( gpLargeFontType1 );
	UnloadFont( gpSmallFontType1 );
	UnloadFont( gpTinyFontType1 );
	UnloadFont( gp12PointFont1 );
	UnloadFont( gpClockFont);
  UnloadFont( gpCompFont);
	UnloadFont( gpSmallCompFont);
	UnloadFont( gp10PointRoman);
	UnloadFont( gp12PointRoman);
	UnloadFont( gp14PointSansSerif);
//	UnloadFont( gpMilitaryFont1);
	UnloadFont( gp10PointArial);
  UnloadFont( gp10PointArialBold);
	UnloadFont( gp14PointArial);
  UnloadFont( gpBlockyFont);
	UnloadFont( gp12PointArialFixedFont );
	#if defined( JA2EDITOR ) && defined( ENGLISH )
		UnloadFont( gpHugeFont );
	#endif

  // ATE: Shutdown any win fonts
#ifdef WINFONTS
    DeleteWinFont( giSubTitleWinFont );
#endif
}

// Set shades for fonts
BOOLEAN SetFontShade( UINT32 uiFontID, INT8 bColorID )
{
	HVOBJECT pFont;

	CHECKF( bColorID > 0 );
	CHECKF( bColorID < 16 );

	pFont	= GetFontObject( uiFontID );

	pFont->pShadeCurrent = pFont->pShades[ bColorID ];

	return( TRUE );
}

UINT16 CreateFontPaletteTables(HVOBJECT pObj )
{
	UINT32 count;
	SGPPaletteEntry Pal[256];

	for( count = 0; count < 16; count++ )
	{
		if ( (count == 4) && (pObj->p16BPPPalette == pObj->pShades[ count ]) )
			pObj->pShades[ count ] = NULL;
		else if ( pObj->pShades[ count ] != NULL )
		{
			MemFree( pObj->pShades[ count ] );
			pObj->pShades[ count ] = NULL;
		}
	}

	// Build white palette
	for(count=0; count < 256; count++)
	{
		Pal[count].peRed=(UINT8)255;
		Pal[count].peGreen=(UINT8)255;
		Pal[count].peBlue=(UINT8)255;
	}

	pObj->pShades[ FONT_SHADE_RED ]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 255, 0, 0, TRUE);
	pObj->pShades[ FONT_SHADE_BLUE ]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 0, 0, 255, TRUE);
	pObj->pShades[ FONT_SHADE_GREEN ]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 0, 255, 0, TRUE);
	pObj->pShades[ FONT_SHADE_YELLOW ]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 255, 255, 0, TRUE);
	pObj->pShades[ FONT_SHADE_NEUTRAL ]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 255, 255, 255, FALSE);

	pObj->pShades[ FONT_SHADE_WHITE ]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 255, 255, 255, TRUE);

		
	// the rest are darkening tables, right down to all-black.
	pObj->pShades[0]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 165, 165, 165, FALSE);
	pObj->pShades[7]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 135, 135, 135, FALSE);
	pObj->pShades[8]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 105, 105, 105, FALSE);
	pObj->pShades[9]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 75, 75, 75, FALSE);
	pObj->pShades[10]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 45, 45, 45, FALSE);
	pObj->pShades[11]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 36, 36, 36, FALSE);
	pObj->pShades[12]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 27, 27, 27, FALSE);
	pObj->pShades[13]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 18, 18, 18, FALSE);
	pObj->pShades[14]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 9, 9, 9, FALSE);
	pObj->pShades[15]=Create16BPPPaletteShaded( pObj->pPaletteEntry, 0, 0, 0, FALSE);

	// Set current shade table to neutral color
	pObj->pShadeCurrent=pObj->pShades[4];

	// check to make sure every table got a palette
	//for(count=0; (count < HVOBJECT_SHADE_TABLES) && (pObj->pShades[count]!=NULL); count++);
	
	// return the result of the check
	//return(count==HVOBJECT_SHADE_TABLES);
	return(TRUE);
}

UINT16    WFGetFontHeight( INT32 FontNum )
{
  if ( USE_WINFONTS( ) )
  {
		// return how many Y pixels we used
//	  return( GetWinFontHeight( L"a\0", GET_WINFONT( ) ) );
		fprintf(stderr, "Tried to use winfont WFGetFontHeight(): line %d\n", __LINE__);
  }
  else
  {
		// return how many Y pixels we used
	  return( GetFontHeight( FontNum ) );
  }
}


INT16 WFStringPixLength( wchar_t *string,INT32 UseFont )
{
  if ( USE_WINFONTS( ) )
  {
		// return how many Y pixels we used
//	  return( WinFontStringPixLength( string, GET_WINFONT( ) ) );
		fprintf(stderr, "Tried to use winfont WFStringPixLength(): line %d\n", __LINE__);
  }
  else
  {
		// return how many Y pixels we used
	  return( StringPixLength( string, UseFont ) );
  }
}


