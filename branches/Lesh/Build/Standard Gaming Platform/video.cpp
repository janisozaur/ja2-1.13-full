#ifdef JA2_PRECOMPILED_HEADERS
	#include "JA2 SGP ALL.H"
	#include "resource.h"
#else
	#include "types.h"
	#include "Video.h"
	#include "vobject_blitters.h"
	#include "sgp.h"
	#include <stdio.h>
	#include "renderworld.h"
	#include "Render Dirty.h"
	#include "Isometric utils.h"
	#include "fade screen.h"
	#include "impTGA.h"
	#include "timer control.h"
	#include "Fileman.h"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Local Defines
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_DIRTY_REGIONS     128

#define VIDEO_OFF             0x00
#define VIDEO_ON              0x01
#define VIDEO_SHUTTING_DOWN   0x02
#define VIDEO_SUSPENDED       0x04

#define THREAD_OFF            0x00
#define THREAD_ON             0x01
#define THREAD_SUSPENDED      0x02

#define CURRENT_MOUSE_DATA		0
#define PREVIOUS_MOUSE_DATA		1


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Local Typedefs
//
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
  BOOLEAN           fRestore;
  UINT16            usMouseXPos, usMouseYPos;
  UINT16            usLeft, usTop, usRight, usBottom;
  SDL_Rect			Region;
  SDL_Surface		*pSurface;

} MouseCursorBackground;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// LOCAL globals
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//
// Video state variables
//

BOOLEAN		gfFullScreen = FALSE;

static UINT16                 gusScreenWidth;
static UINT16                 gusScreenHeight;
static UINT8                  gubScreenPixelDepth;

static RECT    gScrollRegion;  

#define			MAX_NUM_FRAMES			25

BOOLEAN			gfVideoCapture = FALSE;
UINT32			guiFramePeriod = (1000 / 15 );
UINT32			guiLastFrame;
UINT16			*gpFrameData[ MAX_NUM_FRAMES ];
INT32			giNumFrames = 0;

//
// Direct Draw objects for both the Primary and Backbuffer surfaces
//

static LPDIRECTDRAW2          gpDirectDrawObject = NULL;

static LPDIRECTDRAWSURFACE    _gpPrimarySurface = NULL;
static LPDIRECTDRAWSURFACE2   gpPrimarySurface = NULL;
static LPDIRECTDRAWSURFACE2   gpBackBuffer = NULL;

//
// Direct Draw Objects for the frame buffer
//

static LPDIRECTDRAWSURFACE    _gpFrameBuffer = NULL;
static LPDIRECTDRAWSURFACE2   gpFrameBuffer = NULL;

#ifdef WINDOWED_MODE

static LPDIRECTDRAWSURFACE    _gpBackBuffer = NULL;


#endif

//
// SDL Surfaces
//

static SDL_Surface			*gpSDLFrameBuffer   = NULL;
static SDL_Surface			*gpSDLPrimaryBuffer = NULL;
static SDL_Surface			*gpSDLBackBuffer    = NULL;

//
// Globals for mouse cursor
//

static UINT16                 gusMouseCursorWidth;
static UINT16                 gusMouseCursorHeight;
static INT16                  gsMouseCursorXOffset;
static INT16                  gsMouseCursorYOffset;

static LPDIRECTDRAWSURFACE2   gpMouseCursor = NULL;

static LPDIRECTDRAWSURFACE    _gpMouseCursorOriginal = NULL;
static LPDIRECTDRAWSURFACE2   gpMouseCursorOriginal = NULL;

static MouseCursorBackground  gMouseCursorBackground[2];

static SDL_Surface			*gpSDLMouseCursor            = NULL;
static SDL_Surface			*gpSDLMouseCursorOriginal    = NULL;
static SDL_Surface			*gpSDLMouseCursorBackground[2];

static HVOBJECT               gpCursorStore;

BOOLEAN				gfFatalError = FALSE;
char				gFatalErrorString[ 512 ];

// 8-bit palette stuff

SGPPaletteEntry							gSgpPalette[256];
LPDIRECTDRAWPALETTE						gpDirectDrawPalette;

//
// Make sure we record the value of the hWindow (main window frame for the application)
//

HWND                          ghWindow;

//
// Refresh thread based variables
//

UINT32                        guiFrameBufferState;    // BUFFER_READY, BUFFER_DIRTY
UINT32                        guiMouseBufferState;    // BUFFER_READY, BUFFER_DIRTY, BUFFER_DISABLED
UINT32						  guiVideoManagerState;   // VIDEO_ON, VIDEO_OFF, VIDEO_SUSPENDED, VIDEO_SHUTTING_DOWN
UINT32                        guiRefreshThreadState;  // THREAD_ON, THREAD_OFF, THREAD_SUSPENDED

//
// Dirty rectangle management variables
//

void                          (*gpFrameBufferRefreshOverride)(void);
SGPRect                       gListOfDirtyRegions[MAX_DIRTY_REGIONS];
UINT32                        guiDirtyRegionCount;
BOOLEAN                       gfForceFullScreenRefresh;

SGPRect                       gDirtyRegionsEx[MAX_DIRTY_REGIONS];
UINT32                        guiDirtyRegionExCount;

//
// Screen output stuff
//

BOOLEAN                       gfPrintFrameBuffer;
UINT32                        guiPrintFrameBufferIndex;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// External Variables
//
///////////////////////////////////////////////////////////////////////////////////////////////////

extern UINT16 gusRedMask;
extern UINT16 gusGreenMask;
extern UINT16 gusBlueMask;
extern INT16  gusRedShift;
extern INT16  gusBlueShift;
extern INT16  gusGreenShift;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Local Function Prototypes
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void AddRegionEx(INT32 iLeft, INT32 iTop, INT32 iRight, INT32 iBottom, UINT32 uiFlags );
void SnapshotSmall( void );
void VideoMovieCapture( BOOLEAN fEnable );
void RefreshMovieCache( );



///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN InitializeVideoManager(void)
{ 
  UINT32        uiIndex;
  UINT32		uiVideoFlags;
  const SDL_VideoInfo	*pVideoInfo;
  char          AppName[] = APPLICATION_NAME;

	//
	// Register debug topics
	//

	RegisterDebugTopic(TOPIC_VIDEO, "Video");
	DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Initializing the video manager");

	if ( SDL_InitSubSystem( SDL_INIT_VIDEO ) < 0 )
	{
		fprintf(stderr, "Couldn't initialize video sybsystem: %s\n", SDL_GetError());
		return(FALSE);
	}

	pVideoInfo = SDL_GetVideoInfo();

	printf("\nVideo information\n");
	printf("Hardware surfaces: %s\n", (pVideoInfo->hw_available) ? "yes":"no" );
	printf("Windows manager  : %s\n", (pVideoInfo->wm_available) ? "yes":"no" );

	printf("Hardware blits accelerated         : %s\n", (pVideoInfo->blit_hw   ) ? "yes":"no" );
	printf("Hardware blits colorkey accelerated: %s\n", (pVideoInfo->blit_hw_CC) ? "yes":"no" );
	printf("Hardware blits alpha accelerated   : %s\n", (pVideoInfo->blit_hw_A ) ? "yes":"no" );
	printf("Software blits accelerated         : %s\n", (pVideoInfo->blit_sw   ) ? "yes":"no" );
	printf("Software blits colorkey accelerated: %s\n", (pVideoInfo->blit_sw_CC) ? "yes":"no" );
	printf("Software blits alpha accelerated   : %s\n", (pVideoInfo->blit_sw_A ) ? "yes":"no" );
	printf("Color fills accelerated            : %s\n", (pVideoInfo->blit_fill ) ? "yes":"no" );

	printf("Video memory = %d\n", pVideoInfo->video_mem );

	printf("\nCurrent video mode\n");
	printf("Color depth = %d\n", pVideoInfo->vfmt->BitsPerPixel );
	printf("alpha       = %d\n", pVideoInfo->vfmt->alpha );
	printf("Rmask  = %08X\n", pVideoInfo->vfmt->Rmask );
	printf("Gmask  = %08X\n", pVideoInfo->vfmt->Gmask );
	printf("Bmask  = %08X\n", pVideoInfo->vfmt->Bmask );
	printf("Amask  = %08X\n", pVideoInfo->vfmt->Amask );
	printf("Rshift = %d\n",   pVideoInfo->vfmt->Rshift );
	printf("Gshift = %d\n",   pVideoInfo->vfmt->Gshift );
	printf("Bshift = %d\n",   pVideoInfo->vfmt->Bshift );
	printf("Ashift = %d\n",   pVideoInfo->vfmt->Ashift );
	printf("Rloss  = %d\n",   pVideoInfo->vfmt->Rloss );
	printf("Gloss  = %d\n",   pVideoInfo->vfmt->Gloss );
	printf("Bloss  = %d\n",   pVideoInfo->vfmt->Bloss );
	printf("Aloss  = %d\n",   pVideoInfo->vfmt->Aloss );

	SDL_WM_SetCaption(AppName, NULL);

	// Apply SDL video flags
	uiVideoFlags = SDL_SWSURFACE;

	if ( pVideoInfo->hw_available )
		uiVideoFlags |= SDL_HWSURFACE | SDL_DOUBLEBUF;

	if ( gfFullScreen || !pVideoInfo->wm_available )
		uiVideoFlags |= SDL_FULLSCREEN;

    
	//
	// Set the display mode
	//
	gpSDLPrimaryBuffer = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, gbPixelDepth, uiVideoFlags);
	//gpSDLFrameBuffer = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
	
	if ( !gpSDLPrimaryBuffer )
	{
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		return FALSE;
	}

	gusScreenWidth      = SCREEN_WIDTH;
	gusScreenHeight     = SCREEN_HEIGHT;
	gubScreenPixelDepth = gbPixelDepth;
    
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Setup all the surfaces
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////

	//
	// Initialize BackBuffer and offscreen FrameBuffer
	//

	gpSDLFrameBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE,
		gpSDLPrimaryBuffer->w,
		gpSDLPrimaryBuffer->h,
		gpSDLPrimaryBuffer->format->BitsPerPixel,
		gpSDLPrimaryBuffer->format->Rmask,
		gpSDLPrimaryBuffer->format->Gmask,
		gpSDLPrimaryBuffer->format->Bmask,
		gpSDLPrimaryBuffer->format->Amask);

	if ( !gpSDLFrameBuffer )
	{
		fprintf(stderr, "Couldn't create frame video buffer: %s\n", SDL_GetError());
		return FALSE;
	}

	gpSDLBackBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE,
		gpSDLPrimaryBuffer->w,
		gpSDLPrimaryBuffer->h,
		gpSDLPrimaryBuffer->format->BitsPerPixel,
		gpSDLPrimaryBuffer->format->Rmask,
		gpSDLPrimaryBuffer->format->Gmask,
		gpSDLPrimaryBuffer->format->Bmask,
		gpSDLPrimaryBuffer->format->Amask);

	if ( !gpSDLBackBuffer )
	{
		fprintf(stderr, "Couldn't create back video buffer: %s\n", SDL_GetError());
		return FALSE;
	}

	//
	// Initialize the main mouse surfaces
	//

	gpSDLMouseCursor = SDL_CreateRGBSurface(SDL_SWSURFACE,
		MAX_CURSOR_WIDTH,
		MAX_CURSOR_HEIGHT,
		gpSDLFrameBuffer->format->BitsPerPixel,
		gpSDLFrameBuffer->format->Rmask,
		gpSDLFrameBuffer->format->Gmask,
		gpSDLFrameBuffer->format->Bmask,
		gpSDLFrameBuffer->format->Amask);

	if ( !gpSDLMouseCursor )
	{
		fprintf(stderr, "Couldn't create mouse cursor: %s\n", SDL_GetError());
		return FALSE;
	}

	if ( SDL_SetColorKey(gpSDLMouseCursor, SDL_SRCCOLORKEY, 0) < 0 )
	{
		fprintf(stderr, "Couldn't set colorkey for mouse cursor: %s\n", SDL_GetError());
		return FALSE;
	}

	//
	// Initialize the main mouse original surface
	//

	gpSDLMouseCursorOriginal = SDL_CreateRGBSurface(SDL_SWSURFACE,
		MAX_CURSOR_WIDTH,
		MAX_CURSOR_HEIGHT,
		gpSDLFrameBuffer->format->BitsPerPixel,
		gpSDLFrameBuffer->format->Rmask,
		gpSDLFrameBuffer->format->Gmask,
		gpSDLFrameBuffer->format->Bmask,
		gpSDLFrameBuffer->format->Amask);

	if ( !gpSDLMouseCursorOriginal )
	{
		fprintf(stderr, "Couldn't create mouse cursor original: %s\n", SDL_GetError());
		return FALSE;
	}

	//
	// Initialize the main mouse background surfaces. There are two of them (one for each of the Primary
	// and Backbuffer surfaces
	//

	for (uiIndex = 0; uiIndex < 1; uiIndex++)
	{
		//
		// Initialize various mouse background variables
		//

		gMouseCursorBackground[uiIndex].fRestore = FALSE;

		//
		// Initialize the direct draw surfaces for the mouse background
		//

		gMouseCursorBackground[uiIndex].pSurface = SDL_CreateRGBSurface(SDL_SWSURFACE,
			MAX_CURSOR_WIDTH,
			MAX_CURSOR_HEIGHT,
			gpSDLFrameBuffer->format->BitsPerPixel,
			gpSDLFrameBuffer->format->Rmask,
			gpSDLFrameBuffer->format->Gmask,
			gpSDLFrameBuffer->format->Bmask,
			gpSDLFrameBuffer->format->Amask);

		if ( !gMouseCursorBackground[uiIndex].pSurface )
		{
			fprintf(stderr, "Couldn't create mouse background %d: %s\n", uiIndex, SDL_GetError());
			return FALSE;
		}
	}

  //
  // Initialize state variables
  //

  guiFrameBufferState          = BUFFER_DIRTY;
  guiMouseBufferState          = BUFFER_DISABLED;
  guiVideoManagerState         = VIDEO_ON;
  guiRefreshThreadState        = THREAD_OFF;
  guiDirtyRegionCount          = 0;
  gfForceFullScreenRefresh     = TRUE;
  gpFrameBufferRefreshOverride = NULL;
  gpCursorStore                = NULL;
  gfPrintFrameBuffer           = FALSE;
  guiPrintFrameBufferIndex     = 0;

  //
  // This function must be called to setup RGB information
  //

  GetRGBDistribution();

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ShutdownVideoManager(void)
{ 
	if ( guiVideoManagerState == VIDEO_OFF )
		return;

	DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Shutting down the video manager"); 

	//
	// Toggle the state of the video manager to indicate to the refresh thread that it needs to shut itself
	// down
	//

	SDL_FreeSurface(gpSDLBackBuffer);
	SDL_FreeSurface(gpSDLFrameBuffer);
	SDL_FreeSurface(gpSDLMouseCursor);
	SDL_FreeSurface(gpSDLMouseCursorOriginal);
	SDL_FreeSurface(gMouseCursorBackground[0].pSurface);

	gpSDLBackBuffer    = NULL;
	gpSDLFrameBuffer   = NULL;
	gpSDLMouseCursor   = NULL;
	gpSDLMouseCursorOriginal           = NULL;
	gMouseCursorBackground[0].pSurface = NULL;

	guiVideoManagerState = VIDEO_OFF;

	if (gpCursorStore != NULL)
	{
		DeleteVideoObject(gpCursorStore);
		gpCursorStore = NULL;
	}

	// ATE: Release mouse cursor!
	FreeMouseCursor( );

	SDL_QuitSubSystem( SDL_INIT_VIDEO );

	UnRegisterDebugTopic(TOPIC_VIDEO, "Video"); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SuspendVideoManager(void)
{
  guiVideoManagerState = VIDEO_SUSPENDED;
}

void DoTester( )
{
	SDL_Rect rect;

	rect.h = 40;
	rect.w = 40;
	rect.x = 10;
	rect.y = 10;

	// try to draw something on screen
	if ( SDL_FillRect( gpSDLFrameBuffer, &rect, SDL_MapRGB(gpSDLFrameBuffer->format, 127, 255, 127) ) )
	{
		fprintf(stderr, "Couldn't fill rect\n");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN RestoreVideoManager(void)
{ 
	HRESULT ReturnCode;
  
	//
	// Make sure the video manager is indeed suspended before moving on
	//

	if (guiVideoManagerState == VIDEO_SUSPENDED)
	{
		//
		// Restore the Primary and Backbuffer
		//

		//ReturnCode = IDirectDrawSurface2_Restore( gpPrimarySurface );
		//if (ReturnCode != DD_OK)
		//{
		//  DirectXAttempt ( ReturnCode, __LINE__, __FILE__ );
		//  return FALSE;
		//}

		//ReturnCode = IDirectDrawSurface2_Restore( gpBackBuffer );
		//if (ReturnCode != DD_OK)
		//{
		//  DirectXAttempt ( ReturnCode, __LINE__, __FILE__ );
		//  return FALSE;
		//}

		//
		// Restore the mouse surfaces and make sure to initialize the gpMouseCursor surface
	    //
    
		//ReturnCode = IDirectDrawSurface2_Restore( gMouseCursorBackground[0].pSurface );
		//if (ReturnCode != DD_OK)
		//{
		//  DirectXAttempt ( ReturnCode, __LINE__, __FILE__ );
		//  return FALSE;
		//}

		//ReturnCode = IDirectDrawSurface2_Restore( gpMouseCursor );
		//if (ReturnCode != DD_OK)
		//{
		//  DirectXAttempt ( ReturnCode, __LINE__, __FILE__ );
		//  return FALSE;
		//} else
		{
		guiMouseBufferState = BUFFER_DIRTY;
		}

		// 
		// Set the video state to VIDEO_ON
		//
    
		guiFrameBufferState			= BUFFER_DIRTY;
		guiMouseBufferState			= BUFFER_DIRTY;
		gfForceFullScreenRefresh	= TRUE;
		guiVideoManagerState		= VIDEO_ON;
		return TRUE;
	}
	else
	{
		return FALSE;  
	}  	
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void GetCurrentVideoSettings( UINT16 *usWidth, UINT16 *usHeight, UINT8 *ubBitDepth )
{
	*usWidth = (UINT16) gusScreenWidth;
	*usHeight = (UINT16) gusScreenHeight;
	*ubBitDepth = (UINT8) gubScreenPixelDepth;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void InvalidateRegion(INT32 iLeft, INT32 iTop, INT32 iRight, INT32 iBottom)
{
	if (gfForceFullScreenRefresh == TRUE)
	{
		//
		// There's no point in going on since we are forcing a full screen refresh
		//

		return;
	}

	if (guiDirtyRegionCount < MAX_DIRTY_REGIONS)
	{
		//
		// Well we haven't broken the MAX_DIRTY_REGIONS limit yet, so we register the new region
		//

		// DO SOME PREMIMARY CHECKS FOR VALID RECTS
		if ( iLeft < 0 )
				iLeft = 0;

		if ( iTop < 0 )
				iTop = 0;

		if ( iRight > SCREEN_WIDTH )
				iRight = SCREEN_WIDTH;

		if ( iBottom > SCREEN_HEIGHT )
				iBottom = SCREEN_HEIGHT;

		if (  ( iRight - iLeft ) <= 0 )
				return;

		if (  ( iBottom - iTop ) <= 0 )
				return;

		gListOfDirtyRegions[guiDirtyRegionCount].iLeft   = iLeft;
		gListOfDirtyRegions[guiDirtyRegionCount].iTop    = iTop;
		gListOfDirtyRegions[guiDirtyRegionCount].iRight  = iRight;
		gListOfDirtyRegions[guiDirtyRegionCount].iBottom = iBottom;

//		gDirtyRegionFlags[ guiDirtyRegionCount ] = TRUE;

		guiDirtyRegionCount++;

	}
	else
	{
		//
		// The MAX_DIRTY_REGIONS limit has been exceeded. Therefore we arbitrarely invalidate the entire
		// screen and force a full screen refresh
		//
		guiDirtyRegionExCount = 0;
		guiDirtyRegionCount = 0;
		gfForceFullScreenRefresh = TRUE;
	}
}


void InvalidateRegionEx(INT32 iLeft, INT32 iTop, INT32 iRight, INT32 iBottom, UINT32 uiFlags )
{
	INT32 iOldBottom;

	iOldBottom = iBottom;

	// Check if we are spanning the rectangle - if so slit it up!
	if ( iTop <= gsVIEWPORT_WINDOW_END_Y && iBottom > gsVIEWPORT_WINDOW_END_Y )
	{
		// Add new top region
		iBottom				= gsVIEWPORT_WINDOW_END_Y;
		AddRegionEx( iLeft, iTop, iRight, iBottom, uiFlags );
		
		// Add new bottom region
		iTop   = gsVIEWPORT_WINDOW_END_Y;
		iBottom	= iOldBottom;
		AddRegionEx( iLeft, iTop, iRight, iBottom, uiFlags );
													
	}
	else
	{
		 AddRegionEx( iLeft, iTop, iRight, iBottom, uiFlags );
	}
}


void AddRegionEx(INT32 iLeft, INT32 iTop, INT32 iRight, INT32 iBottom, UINT32 uiFlags )
{
	if (guiDirtyRegionExCount < MAX_DIRTY_REGIONS)
	{
		// DO SOME PREMIMARY CHECKS FOR VALID RECTS
		if ( iLeft < 0 )
				iLeft = 0;

		if ( iTop < 0 )
				iTop = 0;

		if ( iRight > SCREEN_WIDTH )
				iRight = SCREEN_WIDTH;

		if ( iBottom > SCREEN_HEIGHT )
				iBottom = SCREEN_HEIGHT;

		if (  ( iRight - iLeft ) <= 0 )
				return;

		if (  ( iBottom - iTop ) <= 0 )
				return;

		gDirtyRegionsEx[ guiDirtyRegionExCount ].iLeft   = iLeft;
		gDirtyRegionsEx[ guiDirtyRegionExCount ].iTop    = iTop;
		gDirtyRegionsEx[ guiDirtyRegionExCount ].iRight  = iRight;
		gDirtyRegionsEx[ guiDirtyRegionExCount ].iBottom = iBottom;

		//gDirtyRegionsFlagsEx[ guiDirtyRegionExCount ] = uiFlags;

		guiDirtyRegionExCount++;
	}
	else
	{
		guiDirtyRegionExCount = 0;
		guiDirtyRegionCount = 0;
		gfForceFullScreenRefresh = TRUE;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////

void InvalidateRegions(SGPRect *pArrayOfRegions, UINT32 uiRegionCount)
{
	if (gfForceFullScreenRefresh == TRUE)
	{
		//
		// There's no point in going on since we are forcing a full screen refresh
		//

		return;
	}

	if ((guiDirtyRegionCount + uiRegionCount) < MAX_DIRTY_REGIONS)
	{
		UINT32 uiIndex;

		for (uiIndex = 0; uiIndex < uiRegionCount; uiIndex++)
		{
			//
			// Well we haven't broken the MAX_DIRTY_REGIONS limit yet, so we register the new region
			//

			gListOfDirtyRegions[guiDirtyRegionCount].iLeft   = pArrayOfRegions[uiIndex].iLeft;
			gListOfDirtyRegions[guiDirtyRegionCount].iTop    = pArrayOfRegions[uiIndex].iTop;
			gListOfDirtyRegions[guiDirtyRegionCount].iRight  = pArrayOfRegions[uiIndex].iRight;
			gListOfDirtyRegions[guiDirtyRegionCount].iBottom = pArrayOfRegions[uiIndex].iBottom;

			guiDirtyRegionCount++; 
		}    
	}
	else
	{
		guiDirtyRegionCount = 0;
		gfForceFullScreenRefresh = TRUE;    
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void InvalidateScreen(void)
{
	guiDirtyRegionCount = 0;
	guiDirtyRegionExCount = 0;
	gfForceFullScreenRefresh = TRUE;
	guiFrameBufferState = BUFFER_DIRTY;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SetFrameBufferRefreshOverride(PTR pFrameBufferRefreshOverride)
{
	gpFrameBufferRefreshOverride = (void (__cdecl *)(void))pFrameBufferRefreshOverride;
}

//#define SCROLL_TEST

///////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollJA2Background(UINT32 uiDirection, INT16 sScrollXIncrement, INT16 sScrollYIncrement, SDL_Surface *pSource, SDL_Surface *pDest, BOOLEAN fRenderStrip, UINT32 uiCurrentMouseBackbuffer )
{
	UINT16			usWidth, usHeight;
	UINT8			ubBitDepth;
	static SDL_Rect	Region, dest;  
	static UINT16	usMouseXPos, usMouseYPos;
	static SDL_Rect	StripRegions[ 2 ], MouseRegion;
	UINT16			usNumStrips = 0;
	INT32			cnt;
	INT16			sShiftX, sShiftY;
	INT32			uiCountY;


 	GetCurrentVideoSettings( &usWidth, &usHeight, &ubBitDepth );
	usHeight=(gsVIEWPORT_WINDOW_END_Y - gsVIEWPORT_WINDOW_START_Y );

///zmiany
	StripRegions[ 0 ].x = gsVIEWPORT_START_X;
	StripRegions[ 0 ].w = gsVIEWPORT_END_X - gsVIEWPORT_START_X;
	StripRegions[ 0 ].y = gsVIEWPORT_WINDOW_START_Y;
	StripRegions[ 0 ].h = gsVIEWPORT_WINDOW_END_Y - gsVIEWPORT_WINDOW_START_Y;
	StripRegions[ 1 ].x = gsVIEWPORT_START_X;
	StripRegions[ 1 ].w = gsVIEWPORT_END_X - gsVIEWPORT_START_X;
	StripRegions[ 1 ].y = gsVIEWPORT_WINDOW_START_Y;
	StripRegions[ 1 ].h = gsVIEWPORT_WINDOW_END_Y - gsVIEWPORT_WINDOW_START_Y;

	MouseRegion.x = gMouseCursorBackground[ uiCurrentMouseBackbuffer ].usLeft;
	MouseRegion.y = gMouseCursorBackground[ uiCurrentMouseBackbuffer ].usTop;
	MouseRegion.w = gMouseCursorBackground[ uiCurrentMouseBackbuffer ].usRight - gMouseCursorBackground[ uiCurrentMouseBackbuffer ].usLeft;
	MouseRegion.h = gMouseCursorBackground[ uiCurrentMouseBackbuffer ].usBottom - gMouseCursorBackground[ uiCurrentMouseBackbuffer ].usTop;

	usMouseXPos	  = gMouseCursorBackground[ uiCurrentMouseBackbuffer ].usMouseXPos;
	usMouseYPos	  = gMouseCursorBackground[ uiCurrentMouseBackbuffer ].usMouseYPos;

	switch (uiDirection)
	{
		case SCROLL_LEFT:

			Region.x = 0;
			Region.y = gsVIEWPORT_WINDOW_START_Y;
			Region.w = usWidth-(sScrollXIncrement);
			Region.h = usHeight;
			dest.x   = sScrollXIncrement;
			dest.y   = gsVIEWPORT_WINDOW_START_Y;

			SDL_BlitSurface(pSource, &Region, pDest, &dest);

			// memset z-buffer
			for(uiCountY = gsVIEWPORT_WINDOW_START_Y; uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++)
			{
				memset((UINT8 *)gpZBuffer+(uiCountY*2280), 0, sScrollXIncrement*2); 
			}	

			StripRegions[ 0 ].w =(INT16)(sScrollXIncrement);
			usMouseXPos += sScrollXIncrement;

			usNumStrips = 1;
			break;

		case SCROLL_RIGHT:

			Region.x = sScrollXIncrement;
			Region.y = gsVIEWPORT_WINDOW_START_Y;
			Region.w = usWidth - sScrollXIncrement;
			Region.h = usHeight;
			dest.x   = 0;
			dest.y   = gsVIEWPORT_WINDOW_START_Y;

			SDL_BlitSurface(pSource, &Region, pDest, &dest);

			// memset z-buffer
			for(uiCountY= gsVIEWPORT_WINDOW_START_Y; uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++)
			{
				memset((UINT8 *)gpZBuffer+(uiCountY*2280) + ( ( gsVIEWPORT_END_X - sScrollXIncrement ) * 2 ), 0, 
								sScrollXIncrement*2); 
			}	

			StripRegions[ 0 ].x =(INT16)(gsVIEWPORT_END_X-sScrollXIncrement);
			StripRegions[ 0 ].w =(INT16)(sScrollXIncrement);
			usMouseXPos -= sScrollXIncrement;

			usNumStrips = 1;
			break;

		case SCROLL_UP:

			Region.x = 0;
			Region.y = gsVIEWPORT_WINDOW_START_Y;
			Region.w = usWidth;
			Region.h = usHeight - sScrollYIncrement;
			dest.x   = 0;
			dest.y   = gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement;

			SDL_BlitSurface(pSource, &Region, pDest, &dest);

			for(uiCountY=sScrollYIncrement-1+gsVIEWPORT_WINDOW_START_Y; uiCountY >= gsVIEWPORT_WINDOW_START_Y; uiCountY--)
			{
				memset((UINT8 *)gpZBuffer+(uiCountY*2280), 0, 2280);
			}	

			StripRegions[ 0 ].h =(INT16)(sScrollYIncrement);
			usNumStrips = 1;

			usMouseYPos += sScrollYIncrement;

			break;

		case SCROLL_DOWN:

			Region.x = 0;
			Region.y = gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement;
			Region.w = usWidth;
			Region.h = usHeight - sScrollYIncrement;
			dest.x   = 0;
			dest.y   = gsVIEWPORT_WINDOW_START_Y;

			SDL_BlitSurface(pSource, &Region, pDest, &dest);

			// Zero out z
			for(uiCountY=(gsVIEWPORT_WINDOW_END_Y - sScrollYIncrement ); uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++)
			{
				memset((UINT8 *)gpZBuffer+(uiCountY*2280), 0, 2280);
			}

			StripRegions[ 0 ].y = (INT16)(gsVIEWPORT_WINDOW_END_Y-sScrollYIncrement);
			StripRegions[ 0 ].h = (INT16)(sScrollYIncrement);
			usNumStrips = 1;

			usMouseYPos -= sScrollYIncrement;

			break;

		case SCROLL_UPLEFT:

			Region.x = 0;
			Region.y = gsVIEWPORT_WINDOW_START_Y;
			Region.w = usWidth - sScrollXIncrement;
			Region.h = usHeight - sScrollYIncrement;
			dest.x   = sScrollXIncrement;
			dest.y   = gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement;

			SDL_BlitSurface(pSource, &Region, pDest, &dest);

			// memset z-buffer
			for(uiCountY=gsVIEWPORT_WINDOW_START_Y; uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++)
			{
				memset((UINT8 *)gpZBuffer+(uiCountY*2280), 0, sScrollXIncrement*2); 

			}	
			for(uiCountY=gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement-1; uiCountY >= gsVIEWPORT_WINDOW_START_Y; uiCountY--)
			{
				memset((UINT8 *)gpZBuffer+(uiCountY*2280), 0, 2280);
			}	

			StripRegions[ 0 ].w = (INT16)(sScrollXIncrement);
			StripRegions[ 1 ].x = (INT16)(sScrollXIncrement);
			StripRegions[ 1 ].h = (INT16)(sScrollYIncrement);
			StripRegions[ 1 ].w = (INT16)(usWidth - sScrollXIncrement);
			usNumStrips = 2;

			usMouseYPos += sScrollYIncrement;
			usMouseXPos += sScrollXIncrement;

			break;

		case SCROLL_UPRIGHT:

			Region.x = sScrollXIncrement;
			Region.y = gsVIEWPORT_WINDOW_START_Y;
			Region.w = usWidth - sScrollXIncrement;
			Region.h = usHeight - sScrollYIncrement;
			dest.x   = 0;
			dest.y   = gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement;

			SDL_BlitSurface(pSource, &Region, pDest, &dest);

			// memset z-buffer
			for(uiCountY=gsVIEWPORT_WINDOW_START_Y; uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++)
			{
				memset((UINT8 *)gpZBuffer+(uiCountY*2280) + ( ( gsVIEWPORT_END_X - sScrollXIncrement ) * 2 ), 0, 
								sScrollXIncrement*2); 
			}	
			for(uiCountY=gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement-1; uiCountY >= gsVIEWPORT_WINDOW_START_Y; uiCountY--)
			{
				memset((UINT8 *)gpZBuffer+(uiCountY*2280), 0, 2280);
			}	

			StripRegions[ 0 ].x = (INT16)(gsVIEWPORT_END_X-sScrollXIncrement);
			StripRegions[ 0 ].w = (INT16)(sScrollXIncrement);
			StripRegions[ 1 ].h = (INT16)(sScrollYIncrement);
			StripRegions[ 1 ].w = (INT16)(usWidth - sScrollXIncrement);
			usNumStrips = 2;

			usMouseYPos += sScrollYIncrement;
			usMouseXPos -= sScrollXIncrement;

			break;

		case SCROLL_DOWNLEFT:

			Region.x = 0;
			Region.y = gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement;
			Region.w = usWidth - sScrollXIncrement;
			Region.h = usHeight - sScrollYIncrement;
			dest.x   = sScrollXIncrement;
			dest.y   = gsVIEWPORT_WINDOW_START_Y;

			SDL_BlitSurface(pSource, &Region, pDest, &dest);

			// memset z-buffer
			for(uiCountY=gsVIEWPORT_WINDOW_START_Y; uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++)
			{
				memset((UINT8 *)gpZBuffer+(uiCountY*2280), 0, sScrollXIncrement*2); 

			}	
			for(uiCountY=(gsVIEWPORT_WINDOW_END_Y - sScrollYIncrement); uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++)
			{
				memset((UINT8 *)gpZBuffer+(uiCountY*2280), 0, 2280);
			}


			StripRegions[ 0 ].w = (INT16)(sScrollXIncrement);
			StripRegions[ 1 ].y	= (INT16)(gsVIEWPORT_WINDOW_END_Y-sScrollYIncrement);
			StripRegions[ 1 ].x = (INT16)(sScrollXIncrement);
			StripRegions[ 1 ].h = (INT16)(sScrollYIncrement);
			usNumStrips = 2;

			usMouseYPos -= sScrollYIncrement;
			usMouseXPos += sScrollXIncrement;

			break;

		case SCROLL_DOWNRIGHT:

			Region.x = sScrollXIncrement;
			Region.y = gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement;
			Region.w = usWidth - sScrollXIncrement;
			Region.h = usHeight - sScrollYIncrement;
			dest.x   = 0;
			dest.y   = gsVIEWPORT_WINDOW_START_Y;

			SDL_BlitSurface(pSource, &Region, pDest, &dest);

			// memset z-buffer
			for(uiCountY=gsVIEWPORT_WINDOW_START_Y; uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++)
			{
				memset((UINT8 *)gpZBuffer+(uiCountY*2280) + ( ( gsVIEWPORT_END_X - sScrollXIncrement ) * 2 ), 0, 
								sScrollXIncrement*2); 
			}	
			for(uiCountY=(gsVIEWPORT_WINDOW_END_Y - sScrollYIncrement); uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++)
			{
				memset((UINT8 *)gpZBuffer+(uiCountY*2280), 0, 2280);
			}

			StripRegions[ 0 ].x = (INT16)(gsVIEWPORT_END_X-sScrollXIncrement);
			StripRegions[ 0 ].w = (INT16)(sScrollXIncrement);
			StripRegions[ 1 ].y = (INT16)(gsVIEWPORT_WINDOW_END_Y-sScrollYIncrement);
			StripRegions[ 1 ].w = (INT16)(usWidth-sScrollXIncrement);
			usNumStrips = 2;

			usMouseYPos -= sScrollYIncrement;
			usMouseXPos -= sScrollXIncrement;

			break;

	}

	if ( fRenderStrip )
	{

		// Memset to 0
#ifdef SCROLL_TEST
		{
			SDL_FillRect(pDest, NULL, 0);
		}
#endif


		for ( cnt = 0; cnt < usNumStrips; cnt++ )
		{
			//RenderStaticWorld();
			//RenderDynamicWorld();
			RenderStaticWorldRect(
				(INT16) ( StripRegions[ cnt ].x ),
				(INT16) ( StripRegions[ cnt ].y ),
				(INT16) ( StripRegions[ cnt ].x + StripRegions[ cnt ].w ),
				(INT16) ( StripRegions[ cnt ].y + StripRegions[ cnt ].h ),
				TRUE );
			// Optimize Redundent tiles too!
			//ExamineZBufferRect( (INT16)StripRegions[ cnt ].left, (INT16)StripRegions[ cnt ].top, (INT16)StripRegions[ cnt ].right, (INT16)StripRegions[ cnt ].bottom );

			dest.x = StripRegions[ cnt ].x;
			dest.y = StripRegions[ cnt ].y;

			SDL_BlitSurface(gpSDLFrameBuffer, &( StripRegions[ cnt ] ), pDest, &dest);
		}

		sShiftX = 0;
		sShiftY = 0;

		switch (uiDirection)
		{
			case SCROLL_LEFT:

				sShiftX = sScrollXIncrement;
				sShiftY = 0;
				break;

			case SCROLL_RIGHT:

				sShiftX = -sScrollXIncrement;
				sShiftY = 0;
				break;

			case SCROLL_UP:

				sShiftX = 0;
				sShiftY = sScrollYIncrement;
				break;

			case SCROLL_DOWN:

				sShiftX = 0;
				sShiftY = -sScrollYIncrement;
				break;

			case SCROLL_UPLEFT:

				sShiftX = sScrollXIncrement;
				sShiftY = sScrollYIncrement;
				break;

			case SCROLL_UPRIGHT:

				sShiftX = -sScrollXIncrement;
				sShiftY = sScrollYIncrement;
				break;

			case SCROLL_DOWNLEFT:

				sShiftX = sScrollXIncrement;
				sShiftY = -sScrollYIncrement;
				break;

			case SCROLL_DOWNRIGHT:

				sShiftX = -sScrollXIncrement;
				sShiftY = -sScrollYIncrement;
				break;
		}

		// RESTORE SHIFTED
		RestoreShiftedVideoOverlays( sShiftX, sShiftY );

		// SAVE NEW
		SaveVideoOverlaysArea( BACKBUFFER );

		// BLIT NEW
		ExecuteVideoOverlaysToAlternateBuffer( BACKBUFFER );


//#if 0
//
//		// Erase mouse from old position
//		if (gMouseCursorBackground[ uiCurrentMouseBackbuffer ].fRestore == TRUE )
//		{
//
//			do
//			{
//				ReturnCode = IDirectDrawSurface2_SGPBltFast(gpBackBuffer, usMouseXPos, usMouseYPos, gMouseCursorBackground[uiCurrentMouseBackbuffer].pSurface, (LPRECT)&MouseRegion, DDBLTFAST_NOCOLORKEY);
//				if ((ReturnCode != DD_OK)&&(ReturnCode != DDERR_WASSTILLDRAWING))
//				{
//					DirectXAttempt ( ReturnCode, __LINE__, __FILE__ );
//
//					if (ReturnCode == DDERR_SURFACELOST)
//					{
//
//					}
//				}
//			} while (ReturnCode != DD_OK);
//		}
//
//#endif
//
	}


	//InvalidateRegion( sLeftDraw, sTopDraw, sRightDraw, sBottomDraw );

	//UpdateSaveBuffer();
	//SaveBackgroundRects();
}		


//rain
//extern BOOLEAN gfVSync;

BOOLEAN IsItAllowedToRenderRain();
extern UINT32 guiRainRenderSurface;

BOOLEAN gfNextRefreshFullScreen = FALSE;
//end rain

void RefreshScreen(void *DummyVariable)
{
	static UINT32	uiRefreshThreadState, uiIndex;
	UINT16  usScreenWidth, usScreenHeight;
	static BOOLEAN	fShowMouse;
	static SDL_Rect	Rect, dstRect;
	static SGPPos	MousePos;	// to be retyped
	static BOOLEAN	fFirstTime = TRUE;
	//UINT32			uiTime;

	usScreenWidth = usScreenHeight = 0;

	if ( fFirstTime )
	{
		fShowMouse = FALSE;
	}

	if( gfNextRefreshFullScreen )
	{
		if( guiCurrentScreen == GAME_SCREEN )
		{
			InvalidateScreen();
			gfRenderScroll = FALSE;
			//			gfForceFullScreenRefresh = TRUE;
			//			guiFrameBufferState == BUFFER_DIRTY;
		}
		gfNextRefreshFullScreen = FALSE;
	}

	//DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Looping in refresh");


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// REFRESH_THREAD_MUTEX 
	//
	///////////////////////////////////////////////////////////////////////////////////////////////

	switch (guiVideoManagerState)
	{
	case VIDEO_ON:
		//
		// Excellent, everything is cosher, we continue on
		//
		uiRefreshThreadState = guiRefreshThreadState = THREAD_ON;        
		usScreenWidth = gusScreenWidth;
		usScreenHeight = gusScreenHeight;
		break;
	case VIDEO_OFF:
		//
		// Hot damn, the video manager is suddenly off. We have to bugger out of here. Don't forget to
		// leave the critical section
		//
		guiRefreshThreadState = THREAD_OFF;
		return;
	case VIDEO_SUSPENDED:
		//
		// This are suspended. Make sure the refresh function does try to access any of the direct
		// draw surfaces
		//
		uiRefreshThreadState = guiRefreshThreadState = THREAD_SUSPENDED;
		break;
	case VIDEO_SHUTTING_DOWN:
		//
		// Well things are shutting down. So we need to bugger out of there. Don't forget to leave the
		// critical section before returning
		//
		guiRefreshThreadState = THREAD_OFF;
		return;
	}


	//
	// Get the current mouse position
	//

	GetMousePos(&MousePos);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// FRAME_BUFFER_MUTEX 
	//
	/////////////////////////////////////////////////////////////////////////////////////////////


	// RESTORE OLD POSITION OF MOUSE
	if (gMouseCursorBackground[CURRENT_MOUSE_DATA].fRestore == TRUE )
	{
		Rect.x = gMouseCursorBackground[CURRENT_MOUSE_DATA].usLeft;
		Rect.y = gMouseCursorBackground[CURRENT_MOUSE_DATA].usTop;
		Rect.w = gMouseCursorBackground[CURRENT_MOUSE_DATA].usRight;
		Rect.h = gMouseCursorBackground[CURRENT_MOUSE_DATA].usBottom;
		dstRect = gMouseCursorBackground[CURRENT_MOUSE_DATA].Region;
		
		SDL_BlitSurface(gMouseCursorBackground[CURRENT_MOUSE_DATA].pSurface, &Rect, gpSDLBackBuffer, &dstRect);

		// Save position into other background region
		memcpy( &(gMouseCursorBackground[PREVIOUS_MOUSE_DATA] ), &(gMouseCursorBackground[CURRENT_MOUSE_DATA] ), sizeof( MouseCursorBackground ) );
	}

	//
	// Ok we were able to get a hold of the frame buffer stuff. Check to see if it needs updating
	// if not, release the frame buffer stuff right away
	//
	if (guiFrameBufferState == BUFFER_DIRTY)
	{
		
		// Well the frame buffer is dirty.
		//

		if (gpFrameBufferRefreshOverride != NULL)
		{
			//
			// Method (3) - We are using a function override to refresh the frame buffer. First we
			// call the override function then we must set the override pointer to NULL
			//
    
			(*gpFrameBufferRefreshOverride)();
			gpFrameBufferRefreshOverride = NULL;

		}


		if ( gfFadeInitialized && gfFadeInVideo )
		{
			gFadeFunction( );
		}
		else
		//
		// Either Method (1) or (2)
		//
		{
			if (gfForceFullScreenRefresh == TRUE)
			{
				//
				// Method (1) - We will be refreshing the entire screen
				//

				SDL_BlitSurface(gpSDLFrameBuffer, NULL, gpSDLBackBuffer, NULL);
			}
			else
			{
				for (uiIndex = 0; uiIndex < guiDirtyRegionCount; uiIndex++)
				{
					Rect.x = gListOfDirtyRegions[uiIndex].iLeft;
					Rect.y = gListOfDirtyRegions[uiIndex].iTop;
					Rect.w = gListOfDirtyRegions[uiIndex].iRight - gListOfDirtyRegions[uiIndex].iLeft;
					Rect.h = gListOfDirtyRegions[uiIndex].iBottom - gListOfDirtyRegions[uiIndex].iTop;

					dstRect.x = gListOfDirtyRegions[uiIndex].iLeft;
					dstRect.y = gListOfDirtyRegions[uiIndex].iTop;

					SDL_BlitSurface(gpSDLFrameBuffer, &Rect, gpSDLBackBuffer, &dstRect);
				}

				// Now do new, extended dirty regions
				for (uiIndex = 0; uiIndex < guiDirtyRegionExCount; uiIndex++)
				{
					Rect.x = gDirtyRegionsEx[uiIndex].iLeft;
					Rect.y = gDirtyRegionsEx[uiIndex].iTop;
					Rect.w = gDirtyRegionsEx[uiIndex].iRight - gDirtyRegionsEx[uiIndex].iLeft;
					Rect.h = gDirtyRegionsEx[uiIndex].iBottom - gDirtyRegionsEx[uiIndex].iTop;

					// Do some checks if we are in the process of scrolling!	
					if ( gfRenderScroll )
					{
						// Check if we are completely out of bounds
						if ( gDirtyRegionsEx[uiIndex].iTop <= gsVIEWPORT_WINDOW_END_Y  && gDirtyRegionsEx[uiIndex].iBottom <= gsVIEWPORT_WINDOW_END_Y )
						{
							continue;
						}
					}

					dstRect.x = gDirtyRegionsEx[uiIndex].iLeft;
					dstRect.y = gDirtyRegionsEx[uiIndex].iTop;

					SDL_BlitSurface(gpSDLFrameBuffer, &Rect, gpSDLBackBuffer, &dstRect);
				}

			}
						
		}
		if ( gfRenderScroll )
		{
			ScrollJA2Background( guiScrollDirection, gsScrollXIncrement, gsScrollYIncrement, gpSDLPrimaryBuffer, gpSDLBackBuffer, TRUE, PREVIOUS_MOUSE_DATA );
		}
		gfIgnoreScrollDueToCenterAdjust = FALSE;
	
 
		//
		// Update the guiFrameBufferState variable to reflect that the frame buffer can now be handled
		//

		guiFrameBufferState = BUFFER_READY;
	}

//  //
//  // Do we want to print the frame stuff ??
//  //
//
//	if( gfVideoCapture )
//	{
//		uiTime=GetTickCount();
//		if((uiTime < guiLastFrame) || (uiTime > (guiLastFrame+guiFramePeriod)))
//		{
//			SnapshotSmall( );		
//			guiLastFrame=uiTime;
//		}
//	}
//
//
//  if (gfPrintFrameBuffer == TRUE)
//  {
//    LPDIRECTDRAWSURFACE    _pTmpBuffer;
//    LPDIRECTDRAWSURFACE2   pTmpBuffer;
//    DDSURFACEDESC          SurfaceDescription;
//    FILE                  *OutputFile;
//    UINT8                  FileName[64];
//    INT32                  iIndex;
//		STRING512			DataDir;
//		STRING512			         ExecDir;
//    UINT16                 *p16BPPData;
//
//		// Snap: save current directory
//		GetFileManCurrentDirectory( DataDir );
//		GetExecutableDirectory( ExecDir );
//		SetFileManCurrentDirectory( ExecDir );
//
//    //
//    // Create temporary system memory surface. This is used to correct problems with the backbuffer
//    // surface which can be interlaced or have a funky pitch
//    //
//
//    ZEROMEM(SurfaceDescription);
//    SurfaceDescription.dwSize         = sizeof(DDSURFACEDESC);
//    SurfaceDescription.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
//    SurfaceDescription.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
//    SurfaceDescription.dwWidth        = SCREEN_WIDTH;
//    SurfaceDescription.dwHeight       = SCREEN_HEIGHT;
//    ReturnCode = IDirectDraw2_CreateSurface ( gpDirectDrawObject, &SurfaceDescription, &_pTmpBuffer, NULL );
//		if ((ReturnCode != DD_OK)&&(ReturnCode != DDERR_WASSTILLDRAWING))
//    { 
//      DirectXAttempt ( ReturnCode, __LINE__, __FILE__ );
//    }
//
//    ReturnCode = IDirectDrawSurface_QueryInterface(_pTmpBuffer, /*&*/IID_IDirectDrawSurface2, (LPVOID *)&pTmpBuffer); // (jonathanl)
//		if ((ReturnCode != DD_OK)&&(ReturnCode != DDERR_WASSTILLDRAWING))
//    { 
//      DirectXAttempt ( ReturnCode, __LINE__, __FILE__ );
//    }
//    
//    //
//    // Copy the primary surface to the temporary surface
//    //
//
//    Region.left = 0;
//    Region.top = 0;
//    Region.right = SCREEN_WIDTH;
//    Region.bottom = SCREEN_HEIGHT;
//
//    do
//    {            
//      ReturnCode = IDirectDrawSurface2_SGPBltFast(pTmpBuffer, 0, 0, gpPrimarySurface, &Region, DDBLTFAST_NOCOLORKEY);
//		  if ((ReturnCode != DD_OK)&&(ReturnCode != DDERR_WASSTILLDRAWING))
//      {
//        DirectXAttempt ( ReturnCode, __LINE__, __FILE__ );
//      }
//    } while (ReturnCode != DD_OK);
//
//    //
//    // Ok now that temp surface has contents of backbuffer, copy temp surface to disk
//    //
//
//	do
//	{
//		sprintf((char *) FileName, "SCREEN%03d.TGA", guiPrintFrameBufferIndex++);
//	}
//	while( (_access( (const char *)FileName, 0 )) != -1 );
//
//    if ((OutputFile = fopen((const char *) FileName, "wb")) != NULL)
//    {
//      fprintf(OutputFile, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, LOBYTE(SCREEN_WIDTH), HIBYTE(SCREEN_WIDTH), LOBYTE(SCREEN_HEIGHT), HIBYTE(SCREEN_HEIGHT), 0x10, 0);
//
//
//      //
//      // Lock temp surface
//      //
//
//      ZEROMEM(SurfaceDescription);
//	    SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
//      ReturnCode = IDirectDrawSurface2_Lock(pTmpBuffer, NULL, &SurfaceDescription, 0, NULL);
//			if ((ReturnCode != DD_OK)&&(ReturnCode != DDERR_WASSTILLDRAWING))
//      {
//        DirectXAttempt ( ReturnCode, __LINE__, __FILE__ );
//      }
//
//      //
//      // Copy 16 bit buffer to file
//      //
//
//      // 5/6/5.. create buffer...
//			if (gusRedMask == 0xF800 && gusGreenMask == 0x07E0 && gusBlueMask == 0x001F)
//      {
//        p16BPPData = (UINT16 *)MemAlloc( SCREEN_WIDTH * 2 );
//      }
//
//      for (iIndex = SCREEN_HEIGHT - 1; iIndex >= 0; iIndex--)
//      { 
//        // ATE: OK, fix this such that it converts pixel format to 5/5/5
//        // if current settings are 5/6/5....
//				if (gusRedMask == 0xF800 && gusGreenMask == 0x07E0 && gusBlueMask == 0x001F)
//        {
//          // Read into a buffer...
//          memcpy( p16BPPData, ( ((UINT8 *)SurfaceDescription.lpSurface) + (iIndex * SCREEN_WIDTH * 2) ), SCREEN_WIDTH * 2 ); 
//
//          // Convert....
//          ConvertRGBDistribution565To555( p16BPPData, SCREEN_WIDTH );
//
//          // Write
//          fwrite( p16BPPData, SCREEN_WIDTH * 2, 1, OutputFile);
//        }
//        else
//        {
//          fwrite((void *)(((UINT8 *)SurfaceDescription.lpSurface) + (iIndex * SCREEN_WIDTH * 2)), SCREEN_WIDTH * 2, 1, OutputFile);
//        }
//      }
//
//      // 5/6/5.. Delete buffer...
//			if (gusRedMask == 0xF800 && gusGreenMask == 0x07E0 && gusBlueMask == 0x001F)
//      {
//        MemFree( p16BPPData );
//      }
//      
//      fclose(OutputFile);
//
//      //
//      // Unlock temp surface
//      //
//
//      ZEROMEM(SurfaceDescription);
//      SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
//      ReturnCode = IDirectDrawSurface2_Unlock(pTmpBuffer, &SurfaceDescription);
//			if ((ReturnCode != DD_OK)&&(ReturnCode != DDERR_WASSTILLDRAWING))
//      {
//        DirectXAttempt ( ReturnCode, __LINE__, __FILE__ );    
//      }
//    }
//
//    //
//    // Release temp surface
//    //
//
//    gfPrintFrameBuffer = FALSE;          
//    IDirectDrawSurface2_Release(pTmpBuffer);  
//
//		// Snap: Restore the data directory once we are finished.
//		SetFileManCurrentDirectory( DataDir );
//		//strcat( ExecDir, "\\Data" );
//		//SetFileManCurrentDirectory( ExecDir );
//  }
//
//  //
//  // Ok we were able to get a hold of the frame buffer stuff. Check to see if it needs updating
//  // if not, release the frame buffer stuff right away
//  //
//
	if (guiMouseBufferState == BUFFER_DIRTY)
	{
		//
		// Well the mouse buffer is dirty. Upload the whole thing
		//

		SDL_BlitSurface(gpSDLMouseCursorOriginal, NULL, gpSDLMouseCursor, NULL);

		guiMouseBufferState = BUFFER_READY;
	}
  
	//
	// Check current state of the mouse cursor
	//

	if (fShowMouse == FALSE)
	{
		if (guiMouseBufferState == BUFFER_READY)
		{
			fShowMouse = TRUE;             
		}
		else
		{
			fShowMouse = FALSE;
		}
	}
	else
	{
		if (guiMouseBufferState == BUFFER_DISABLED)
		{
			fShowMouse = FALSE;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// End of MOUSE_BUFFER_MUTEX
	//
	///////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// If fMouseState == TRUE
	//
	// (1) Save mouse background from gpBackBuffer to gpMouseCursorBackground
	// (2) If step (1) is successfull blit mouse cursor onto gpBackBuffer
	//
	///////////////////////////////////////////////////////////////////////////////////////////////

	if (fShowMouse == TRUE )
	{      
		//
		// Step (1) - Save mouse background
		//                      

		Rect.x = MousePos.x - gsMouseCursorXOffset;
		Rect.y = MousePos.y - gsMouseCursorYOffset;
		Rect.w = gusMouseCursorWidth;
		Rect.h = gusMouseCursorHeight;        

		if (Rect.x + Rect.w > usScreenWidth)
		{
			Rect.w = usScreenWidth - Rect.x;          
		}

		if (Rect.y + Rect.h > usScreenHeight)
		{
			Rect.h = usScreenHeight - Rect.y;
		}        

		//if ((Region.right > Region.left)&&(Region.bottom > Region.top))
		{
			//
			// Make sure the mouse background is marked for restore and coordinates are saved for the
			// future restore
			//

			gMouseCursorBackground[CURRENT_MOUSE_DATA].fRestore    = TRUE;                    
			gMouseCursorBackground[CURRENT_MOUSE_DATA].usRight     = Rect.w;
			gMouseCursorBackground[CURRENT_MOUSE_DATA].usBottom    = Rect.h;
			if (Rect.x < 0)
			{
				gMouseCursorBackground[CURRENT_MOUSE_DATA].usLeft = (UINT16) (0 - Rect.x);
				gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseXPos = 0;
				Rect.x = 0;
			}
			else
			{
				gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseXPos = (UINT16) MousePos.x - gsMouseCursorXOffset;
				gMouseCursorBackground[CURRENT_MOUSE_DATA].usLeft = 0;
			}
			if (Rect.y < 0)
			{
				gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseYPos = 0;
				gMouseCursorBackground[CURRENT_MOUSE_DATA].usTop = (UINT16) (0 - Rect.y);
				Rect.y = 0;
			}
			else
			{
				gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseYPos = (UINT16) MousePos.y - gsMouseCursorYOffset;
				gMouseCursorBackground[CURRENT_MOUSE_DATA].usTop = 0;
			}          

			//if ((Region.right > Region.left)&&(Region.bottom > Region.top))
			{
				// Save clipped region
				gMouseCursorBackground[CURRENT_MOUSE_DATA].Region = Rect;

				//
				// Ok, do the actual data save to the mouse background
				//
				dstRect.x = gMouseCursorBackground[CURRENT_MOUSE_DATA].usLeft;
				dstRect.y = gMouseCursorBackground[CURRENT_MOUSE_DATA].usTop;
				dstRect.w = gMouseCursorBackground[CURRENT_MOUSE_DATA].usRight;
				dstRect.h = gMouseCursorBackground[CURRENT_MOUSE_DATA].usBottom;

				SDL_BlitSurface(gpSDLBackBuffer, &Rect, gMouseCursorBackground[CURRENT_MOUSE_DATA].pSurface, &dstRect);

				//
				// Step (2) - Blit mouse cursor to back buffer
				//

				Rect.x = 0;
				Rect.y = 0;
				Rect.w = gusMouseCursorWidth;
				Rect.h = gusMouseCursorHeight;        
				dstRect.x = gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseXPos;
				dstRect.y = gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseYPos;

				SDL_BlitSurface(gpSDLMouseCursor, &Rect, gpSDLBackBuffer, &dstRect);
			}
			//else
			//{
			//	//
			//	// Hum, the mouse was not blitted this round. Henceforth we will flag fRestore as FALSE
			//	//

			//	gMouseCursorBackground[CURRENT_MOUSE_DATA].fRestore = FALSE;
			//}

		}
		//else
		//{
		//	//
		//	// Hum, the mouse was not blitted this round. Henceforth we will flag fRestore as FALSE
		//	//

		//	gMouseCursorBackground[CURRENT_MOUSE_DATA].fRestore = FALSE;
		//}
	}
	else
	{
		//
		// Well since there was no mouse handling this round, we disable the mouse restore
		//        

		gMouseCursorBackground[CURRENT_MOUSE_DATA].fRestore = FALSE;

	}



	///////////////////////////////////////////////////////////////////////////////////////////////
	// Rain                                                                                      //
	///////////////////////////////////////////////////////////////////////////////////////////////

//  if( IsItAllowedToRenderRain() && gfProgramIsRunning )
//  {
//	  BltVideoSurface( BACKBUFFER, guiRainRenderSurface, 0, 0, 0, VS_BLT_FAST | VS_BLT_USECOLORKEY, NULL );
//	  gfNextRefreshFullScreen = TRUE;
//  }
//
//
//
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// (1) Flip Pages
	// (2) If the page flipping worked, then we copy the contents of the primary surface back
	//     to the backbuffer
	// (3) If step (2) was successfull we then restore the mouse background onto the backbuffer
	//     if fShowMouse is TRUE
	//
	///////////////////////////////////////////////////////////////////////////////////////////////

	//
	// Step (1) - Flip pages
	//
//#ifdef WINDOWED_MODE

	SDL_BlitSurface(gpSDLBackBuffer, NULL, gpSDLPrimaryBuffer, NULL);
	SDL_UpdateRect(gpSDLPrimaryBuffer, 0, 0, 0, 0);

//#else
//
//  do
//  {
//	  ReturnCode = IDirectDrawSurface_Flip(_gpPrimarySurface, NULL, gGameExternalOptions.gfVSync ? DDFLIP_WAIT : 0x00000008l );//DDFLIP_WAIT ); 
////    if ((ReturnCode != DD_OK)&&(ReturnCode != DDERR_WASSTILLDRAWING))
//		if ((ReturnCode != DD_OK)&&(ReturnCode != DDERR_WASSTILLDRAWING))
//    {
//      DirectXAttempt ( ReturnCode, __LINE__, __FILE__ );
//
//      if (ReturnCode == DDERR_SURFACELOST)
//      {
//        goto ENDOFLOOP;
//      }
//    }
//
//  } while (ReturnCode != DD_OK);
//
//#endif

  //
  // Step (2) - Copy Primary Surface to the Back Buffer
  //
	if ( gfRenderScroll )
	{
		//Rect.x = 0;
		//Rect.y = 0;
		//Rect.w = gsVIEWPORT_END_X; //ods1 640;
		//Rect.h = gsVIEWPORT_END_Y;

		SDL_BlitSurface(gpSDLPrimaryBuffer, NULL, gpSDLBackBuffer, NULL);

		//Get new background for mouse
		//
		// Ok, do the actual data save to the mouse background

		gfRenderScroll = FALSE;
		gfScrollStart  = FALSE;
	}

	// COPY MOUSE AREAS FROM PRIMARY BACK!

	// FIRST OLD ERASED POSITION
	//if (gMouseCursorBackground[PREVIOUS_MOUSE_DATA].fRestore == TRUE )
	//{
	//	Rect = gMouseCursorBackground[PREVIOUS_MOUSE_DATA].Region;
	//	//dstRect.x = gMouseCursorBackground[PREVIOUS_MOUSE_DATA].usMouseXPos;
	//	//dstRect.y = gMouseCursorBackground[PREVIOUS_MOUSE_DATA].usMouseYPos;
	//	dstRect = Rect;

	//	SDL_BlitSurface(gpSDLPrimaryBuffer, &Rect, gpSDLBackBuffer, &dstRect);
	//}

	// NOW NEW MOUSE AREA
	//if (gMouseCursorBackground[CURRENT_MOUSE_DATA].fRestore == TRUE )
	//{
	//	Rect = gMouseCursorBackground[CURRENT_MOUSE_DATA].Region;
	//	//dstRect.x = gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseXPos;
	//	//dstRect.y = gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseYPos;
	//	dstRect = Rect;

	//	SDL_BlitSurface(gpSDLPrimaryBuffer, &Rect, gpSDLBackBuffer, &dstRect);
	//}


	if (gfForceFullScreenRefresh == TRUE )
	{
		//
		// Method (1) - We will be refreshing the entire screen
		//
		SDL_BlitSurface(gpSDLPrimaryBuffer, NULL, gpSDLBackBuffer, NULL);

		guiDirtyRegionCount = 0; 
		guiDirtyRegionExCount = 0; 
		gfForceFullScreenRefresh = FALSE;
	}
	else
	{
		for (uiIndex = 0; uiIndex < guiDirtyRegionCount; uiIndex++)
		{
			Rect.x = gListOfDirtyRegions[uiIndex].iLeft;
			Rect.y = gListOfDirtyRegions[uiIndex].iTop;
			Rect.w = gListOfDirtyRegions[uiIndex].iRight - gListOfDirtyRegions[uiIndex].iLeft;
			Rect.h = gListOfDirtyRegions[uiIndex].iBottom - gListOfDirtyRegions[uiIndex].iTop;
			dstRect.x = gListOfDirtyRegions[uiIndex].iLeft;
			dstRect.y = gListOfDirtyRegions[uiIndex].iTop;

			SDL_BlitSurface(gpSDLPrimaryBuffer, &Rect, gpSDLBackBuffer, &dstRect);
		}

		for (uiIndex = 0; uiIndex < guiDirtyRegionExCount; uiIndex++)
		{
			Rect.x = gDirtyRegionsEx[uiIndex].iLeft;
			Rect.y = gDirtyRegionsEx[uiIndex].iTop;
			Rect.w = gDirtyRegionsEx[uiIndex].iRight - gDirtyRegionsEx[uiIndex].iLeft;
			Rect.h = gDirtyRegionsEx[uiIndex].iBottom - gDirtyRegionsEx[uiIndex].iTop;

			if ( ( Rect.y < gsVIEWPORT_WINDOW_END_Y ) && gfRenderScroll )
			{
				continue;
			}

			dstRect.x = gDirtyRegionsEx[uiIndex].iLeft;
			dstRect.y = gDirtyRegionsEx[uiIndex].iTop;

			SDL_BlitSurface(gpSDLPrimaryBuffer, &Rect, gpSDLBackBuffer, &dstRect);
		}

		guiDirtyRegionExCount = 0;
		guiDirtyRegionCount = 0;
		gfForceFullScreenRefresh = FALSE;
	}

	// Do extended dirty regions!

//ENDOFLOOP:

	fFirstTime = FALSE;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Object access functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////

SDL_Surface* GetPrimarySurfaceObject(void)
{
	Assert( gpSDLPrimaryBuffer != NULL );

	return gpSDLPrimaryBuffer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

SDL_Surface* GetBackBufferObject(void)
{
	Assert( gpSDLBackBuffer != NULL );

	return gpSDLBackBuffer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

SDL_Surface* GetFrameBufferObject(void)
{
	Assert( gpSDLFrameBuffer != NULL );

	return gpSDLFrameBuffer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

SDL_Surface* GetMouseBufferObject(void)
{
	Assert( gpSDLMouseCursor != NULL );

	return gpSDLMouseCursor;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Buffer access functions   
//
///////////////////////////////////////////////////////////////////////////////////////////////////

PTR LockPrimarySurface(UINT32 *uiPitch)
{
	if ( SDL_LockSurface( gpSDLPrimaryBuffer ) < 0 )
	{
		fprintf(stderr, "Failed to lock primary buffer surface: %s\n", SDL_GetError());
		return NULL;
	}

	*uiPitch = gpSDLPrimaryBuffer->pitch;

	return gpSDLPrimaryBuffer->pixels;

}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UnlockPrimarySurface(void)
{
	SDL_UnlockSurface( gpSDLPrimaryBuffer );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PTR LockBackBuffer(UINT32 *uiPitch)
{
	if ( SDL_LockSurface( gpSDLBackBuffer ) < 0 )
	{
		fprintf(stderr, "Failed to lock back buffer surface: %s\n", SDL_GetError());
		return NULL;
	}

	*uiPitch = gpSDLBackBuffer->pitch;

	return gpSDLBackBuffer->pixels;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UnlockBackBuffer(void)
{
	SDL_UnlockSurface( gpSDLBackBuffer );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PTR LockFrameBuffer(UINT32 *uiPitch)
{
	if ( SDL_LockSurface( gpSDLFrameBuffer ) < 0 )
	{
		fprintf(stderr, "Failed to lock frame buffer surface\n", SDL_GetError());
		return NULL;
	}

	*uiPitch = gpSDLFrameBuffer->pitch;

	return gpSDLFrameBuffer->pixels;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UnlockFrameBuffer(void)
{
	SDL_UnlockSurface( gpSDLFrameBuffer );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PTR LockMouseBuffer(UINT32 *uiPitch)
{
	if ( SDL_LockSurface( gpSDLMouseCursorOriginal ) < 0 )
	{
		fprintf(stderr, "Failed to lock mouse cursor surface: %s\n", SDL_GetError());
		return NULL;
	}

	*uiPitch = gpSDLMouseCursorOriginal->pitch;

	return gpSDLMouseCursorOriginal->pixels;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UnlockMouseBuffer(void)
{
	SDL_UnlockSurface( gpSDLMouseCursorOriginal );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// RGB color management functions   
//
///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN GetRGBDistribution(void)
{
//UINT16        usBit;
//
//	// ONLY DO IF WE ARE IN 16BIT MODE
//	if ( gbPixelDepth == 8 )
//	{
//		return( TRUE );
//	}
//
	gusRedMask    = (UINT16) gpSDLFrameBuffer->format->Rmask;
	gusGreenMask  = (UINT16) gpSDLFrameBuffer->format->Gmask;
	gusBlueMask   = (UINT16) gpSDLFrameBuffer->format->Bmask;

	gusRedShift   = (UINT16) (gpSDLFrameBuffer->format->Rshift - gpSDLFrameBuffer->format->Rloss);
	gusGreenShift = (UINT16) (gpSDLFrameBuffer->format->Gshift - gpSDLFrameBuffer->format->Gloss);
	gusBlueShift  = (UINT16) (gpSDLFrameBuffer->format->Bshift - gpSDLFrameBuffer->format->Bloss);


	// RGB 5,5,5
	if((gusRedMask==0x7c00) && (gusGreenMask==0x03e0) && (gusBlueMask==0x1f))
		guiTranslucentMask=0x3def;
	// RGB 5,6,5
	else if((gusRedMask==0xf800) && (gusGreenMask==0x03e0) && (gusBlueMask==0x1f))
		guiTranslucentMask=0x7bef;


  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN GetPrimaryRGBDistributionMasks(UINT32 *RedBitMask, UINT32 *GreenBitMask, UINT32 *BlueBitMask)
{
	*RedBitMask   = gusRedMask;
	*GreenBitMask = gusGreenMask;
	*BlueBitMask  = gusBlueMask;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN SetMouseCursorFromObject(UINT32 uiVideoObjectHandle, UINT16 usVideoObjectSubIndex, UINT16 usOffsetX, UINT16 usOffsetY )
{
	BOOLEAN      ReturnValue;
	ETRLEObject  pETRLEPointer;

	//
	// Erase cursor background
	//

	EraseMouseCursor( );

	//
	// Get new cursor data
	//

	ReturnValue = BltVideoObjectFromIndex(MOUSE_BUFFER, uiVideoObjectHandle, usVideoObjectSubIndex, 0, 0, VO_BLT_SRCTRANSPARENCY, NULL);
	guiMouseBufferState = BUFFER_DIRTY;

	if (GetVideoObjectETRLEPropertiesFromIndex(uiVideoObjectHandle, &pETRLEPointer, usVideoObjectSubIndex))
	{
		gsMouseCursorXOffset = usOffsetX;
		gsMouseCursorYOffset = usOffsetY;
		gusMouseCursorWidth  = pETRLEPointer.usWidth + pETRLEPointer.sOffsetX;
		gusMouseCursorHeight = pETRLEPointer.usHeight + pETRLEPointer.sOffsetY;

		DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "=================================================");
		DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, String("Mouse Create with [ %d. %d ] [ %d, %d]", pETRLEPointer.sOffsetX, pETRLEPointer.sOffsetY, pETRLEPointer.usWidth, pETRLEPointer.usHeight));
		DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "=================================================");

	}
	else
	{
		DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Failed to get mouse info");
	}

	return ReturnValue;
}

BOOLEAN EraseMouseCursor( )
{
	SDL_Rect	rect;

	//
	// Erase cursor background
	//

	rect.x = rect.y = 0;
	rect.w = MAX_CURSOR_WIDTH;
	rect.h = MAX_CURSOR_HEIGHT;
	SDL_FillRect(gpSDLMouseCursorOriginal, &rect, 0);

	// Don't set dirty
	return( TRUE );
}

BOOLEAN SetMouseCursorProperties( INT16 sOffsetX, INT16 sOffsetY, UINT16 usCursorHeight, UINT16 usCursorWidth )
{
	gsMouseCursorXOffset = sOffsetX;
	gsMouseCursorYOffset = sOffsetY;
	gusMouseCursorWidth  = usCursorWidth;
	gusMouseCursorHeight = usCursorHeight;
	return( TRUE );
}

BOOLEAN BltToMouseCursor(UINT32 uiVideoObjectHandle, UINT16 usVideoObjectSubIndex, UINT16 usXPos, UINT16 usYPos )
{
	BOOLEAN      ReturnValue;

	ReturnValue = BltVideoObjectFromIndex(MOUSE_BUFFER, uiVideoObjectHandle, usVideoObjectSubIndex, usXPos, usYPos, VO_BLT_SRCTRANSPARENCY, NULL);

	return ReturnValue;
}

void DirtyCursor( )
{
	guiMouseBufferState = BUFFER_DIRTY;
}

void EnableCursor( BOOLEAN fEnable )
{
	if ( fEnable )
	{
		guiMouseBufferState = BUFFER_DISABLED;
	}
	else
	{
		guiMouseBufferState = BUFFER_READY;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN HideMouseCursor(void)
{
  guiMouseBufferState = BUFFER_DISABLED;

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN LoadCursorFile(PTR pFilename)
{
	VOBJECT_DESC VideoObjectDescription;

	//
	// Make sure the old cursor store is destroyed
	//

	if (gpCursorStore != NULL)
	{
		DeleteVideoObject(gpCursorStore);
		gpCursorStore = NULL;
	}

	//
	// Get the source file with all the cursors inside
	//

	VideoObjectDescription.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	strcpy(VideoObjectDescription.ImageFile, pFilename);
	gpCursorStore = CreateVideoObject(&VideoObjectDescription);

	//
	// Were we successful in creating the cursor store ? 
	//

	if (gpCursorStore == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN SetCurrentCursor(UINT16 usVideoObjectSubIndex,  UINT16 usOffsetX, UINT16 usOffsetY )
{
	BOOLEAN      ReturnValue;
	ETRLEObject  pETRLEPointer;

	//
	// Make sure we have a cursor store
	//

	if (gpCursorStore == NULL)
	{
		DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "ERROR : Cursor store is not loaded");
		return FALSE;
	}

	//
	// Ok, then blit the mouse cursor to the MOUSE_BUFFER (which is really gpMouseBufferOriginal)
	//
	//
	// Erase cursor background
	//

	EraseMouseCursor( );

	//
	// Get new cursor data
	//

	ReturnValue = BltVideoObject(MOUSE_BUFFER, gpCursorStore, usVideoObjectSubIndex, 0, 0, VO_BLT_SRCTRANSPARENCY, NULL);
	guiMouseBufferState = BUFFER_DIRTY;

	if (GetVideoObjectETRLEProperties(gpCursorStore, &pETRLEPointer, usVideoObjectSubIndex))
	{
		gsMouseCursorXOffset = usOffsetX;
		gsMouseCursorYOffset = usOffsetY;
		gusMouseCursorWidth = pETRLEPointer.usWidth + pETRLEPointer.sOffsetX;
		gusMouseCursorHeight = pETRLEPointer.usHeight + pETRLEPointer.sOffsetY;

		DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "=================================================");
		DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, String("Mouse Create with [ %d. %d ] [ %d, %d]", pETRLEPointer.sOffsetX, pETRLEPointer.sOffsetY, pETRLEPointer.usWidth, pETRLEPointer.usHeight));
		DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "=================================================");
	}
	else
	{
		DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Failed to get mouse info");
	}

	return ReturnValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void StartFrameBufferRender(void)
{
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void EndFrameBufferRender(void)
{
	guiFrameBufferState = BUFFER_DIRTY;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void PrintScreen(void)
{
  gfPrintFrameBuffer = TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//Kaiden - Added for VC6 Compatibility
#if _MSC_VER <= 1200
	template void FatalError<char const *>(char *, ...);
#endif
 
template void FatalError<char const *>(char const *, ...);
template <typename string1>
void FatalError( string1 pError, ...)
{
	va_list argptr;

	va_start(argptr, pError);       	// Set up variable argument pointer
	vsprintf(gFatalErrorString, pError, argptr);	
	va_end(argptr);


	gfFatalError = TRUE;

	SDL_Quit();

	gfProgramIsRunning = FALSE;
}


/*********************************************************************************
* SnapshotSmall
*
*		Grabs a screen from the [rimary surface, and stuffs it into a 16-bit (RGB 5,5,5), 
* uncompressed Targa file. Each time the routine is called, it increments the 
* file number by one. The files are create in the current directory, usually the
* EXE directory. This routine produces 1/4 sized images.
*
*********************************************************************************/

#pragma pack (push, 1)

typedef struct {

		UINT8		ubIDLength;
		UINT8		ubColorMapType;
		UINT8		ubTargaType;
		UINT16	usColorMapOrigin;
		UINT16	usColorMapLength;
		UINT8		ubColorMapEntrySize;
		UINT16	usOriginX;
		UINT16	usOriginY;
		UINT16	usImageWidth;
		UINT16	usImageHeight;
		UINT8		ubBitsPerPixel;
		UINT8		ubImageDescriptor;
		
} TARGA_HEADER;

#pragma pack (pop)


void SnapshotSmall(void)
{
	INT32 iCountX, iCountY;
  DDSURFACEDESC SurfaceDescription;
	UINT16 *pVideo, *pDest;

  HRESULT       ReturnCode;


  ZEROMEM(SurfaceDescription);
	SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  ReturnCode = IDirectDrawSurface2_Lock( gpPrimarySurface, NULL, &SurfaceDescription, 0, NULL);
	if ((ReturnCode != DD_OK)&&(ReturnCode != DDERR_WASSTILLDRAWING))
  {
		return;
  }

//	sprintf( cFilename, "JA%5.5d.TGA", uiPicNum++ );
	
//	if( ( disk = fopen(cFilename, "wb"))==NULL )
//		return;

//	memset(&Header, 0, sizeof(TARGA_HEADER));
	
//	Header.ubTargaType=2;			// Uncompressed 16/24/32 bit
//	Header.usImageWidth=320;
//	Header.usImageHeight=240;
//	Header.ubBitsPerPixel=16;

//	fwrite(&Header, sizeof(TARGA_HEADER), 1, disk);

		// Get the write pointer
	pVideo = (UINT16*)SurfaceDescription.lpSurface;

	pDest = gpFrameData[ giNumFrames ];

	for(iCountY=SCREEN_HEIGHT-1; iCountY >=0 ; iCountY-=1)
	{
		for(iCountX=0; iCountX < SCREEN_WIDTH; iCountX+= 1)
		{
	//		uiData=(UINT16)*(pVideo+(iCountY*SCREEN_WIDTH*2)+ ( iCountX * 2 ) );

//				1111 1111 1100 0000
//				f		 f		c
	//		usPixel555=	(UINT16)(uiData&0xffff);
//			usPixel555= ((usPixel555 & 0xffc0) >> 1) | (usPixel555 & 0x1f);

	//		usPixel555=	(UINT16)(uiData);
			
		//	fwrite( &usPixel555, sizeof(UINT16), 1, disk);
	//		fwrite(	(void *)(((UINT8 *)SurfaceDescription.lpSurface) + ( iCountY * SCREEN_WIDTH * 2) + ( iCountX * 2 ) ), 2 * sizeof( BYTE ), 1, disk );

		 *( pDest + ( iCountY * SCREEN_WIDTH ) + ( iCountX ) ) = *( pVideo + ( iCountY * SCREEN_WIDTH ) + ( iCountX ) );
		}

	}
	
	giNumFrames++;

	if ( giNumFrames == MAX_NUM_FRAMES )
	{
		RefreshMovieCache( );
	}


  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  ReturnCode = IDirectDrawSurface2_Unlock(gpPrimarySurface, &SurfaceDescription);
	if ((ReturnCode != DD_OK)&&(ReturnCode != DDERR_WASSTILLDRAWING))
  {
    DirectXAttempt ( ReturnCode, __LINE__, __FILE__ );    
  }

//	fclose(disk);

}


void VideoCaptureToggle(void)
{
#ifdef JA2TESTVERSION
	VideoMovieCapture( (BOOLEAN)!gfVideoCapture);
#endif
}

void VideoMovieCapture( BOOLEAN fEnable )
{
	INT32 cnt;

	gfVideoCapture=fEnable;
	if(fEnable)
	{
		for ( cnt = 0; cnt < MAX_NUM_FRAMES; cnt++ )
		{
			gpFrameData[ cnt ] = (UINT16 *)MemAlloc( SCREEN_WIDTH * SCREEN_HEIGHT * 2 );
		}

		giNumFrames = 0;

		guiLastFrame=GetTickCount();
	}
	else
	{
		RefreshMovieCache( );

		for ( cnt = 0; cnt < MAX_NUM_FRAMES; cnt++ )
		{
			if ( gpFrameData[ cnt ] != NULL )
			{
				MemFree( gpFrameData[ cnt ] );
			}
		}
		giNumFrames = 0;
	}
}

void RefreshMovieCache( )
{
	TARGA_HEADER Header;
	INT32 iCountX, iCountY;
	FILE *disk;
	CHAR8 cFilename[_MAX_PATH];
	static UINT32 uiPicNum=0;
	UINT16 *pDest;
	INT32	cnt;
	STRING512			DataDir;
	STRING512			ExecDir;

	PauseTime( TRUE );

	// Snap: save current directory
	GetFileManCurrentDirectory( DataDir );
	
	GetExecutableDirectory( ExecDir );
	SetFileManCurrentDirectory( ExecDir );

	for ( cnt = 0; cnt < giNumFrames; cnt++ )
	{
		sprintf( cFilename, "JA%5.5d.TGA", uiPicNum++ );

		if( ( disk = fopen(cFilename, "wb"))==NULL )
			return;

		memset(&Header, 0, sizeof(TARGA_HEADER));

		Header.ubTargaType=2;			// Uncompressed 16/24/32 bit
		Header.usImageWidth=SCREEN_WIDTH;
		Header.usImageHeight=SCREEN_HEIGHT;
		Header.ubBitsPerPixel=16;

		fwrite(&Header, sizeof(TARGA_HEADER), 1, disk);

		pDest = gpFrameData[ cnt ];

		for(iCountY=SCREEN_HEIGHT-1; iCountY >=0 ; iCountY-=1)
		{
			for(iCountX=0; iCountX < SCREEN_WIDTH; iCountX ++ )
			{
				 fwrite( ( pDest + ( iCountY * SCREEN_WIDTH ) + iCountX ), sizeof(UINT16), 1, disk);
			}

		}

		fclose(disk);
	}

	PauseTime( FALSE );

	giNumFrames = 0;

	// Snap: Restore the data directory once we are finished.
	SetFileManCurrentDirectory( DataDir );
	//strcat( ExecDir, "\\Data" );
	//SetFileManCurrentDirectory( ExecDir );
}
