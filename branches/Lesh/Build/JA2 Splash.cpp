#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
#else
	//add includes here!
#endif

UINT32 guiSplashFrameFade = 10;
UINT32 guiSplashStartTime = 0;
extern HVSURFACE ghFrameBuffer;
extern BOOLEAN Unpack8BPPDataTo8BPPBuffer( UINT16 *pBuffer, UINT32 uiDestPitchBYTES, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex );
extern SGPRect	ClippingRect;

//Simply create videosurface, load image, and draw it to the screen.
void InitJA2SplashScreen()
{
//#ifdef ENGLISH
	//ClearMainMenu();

//#else
	UINT32 uiLogoID = 0, uiPic = 0;
	HVSURFACE hVSurface; // unused jonathanl  // lalien reenabled for international versions
	VSURFACE_DESC VSurfaceDesc; //unused jonathanl // lalien reenabled for international versions
	HVOBJECT hHandle;
	VOBJECT_DESC    VObjectDesc; 

// #ifdef JA2TESTVERSION
//	INT32 i = 0;
//
//	memset( &VSurfaceDesc, 0, sizeof( VSURFACE_DESC ) );
//	VSurfaceDesc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;
//	sprintf( VSurfaceDesc.ImageFile, "LOADSCREENS\\Notification.sti" );
//	if( !AddVideoSurface( &VSurfaceDesc, &uiLogoID ) )
//	{	
//		//AssertMsg( 0, String( "Failed to load %s", VSurfaceDesc.ImageFile ) );
//		return;
//	}
//	GetVideoSurface(&hVSurface, uiLogoID );
//	BltVideoSurfaceToVideoSurface( ghFrameBuffer, hVSurface, 0, 0, 0, 0, NULL );
//	DeleteVideoSurfaceFromIndex( uiLogoID );
//
//
//	InvalidateScreen();
//	RefreshScreen( NULL );
//
//	guiSplashStartTime = GetJA2Clock();
//	while( i < 60 * 15 )//guiSplashStartTime + 15000 > GetJA2Clock() )
//	{
//		//Allow the user to pick his bum.
//		InvalidateScreen();
//		RefreshScreen( NULL );
//		i++;
//	}
//#endif
	ClippingRect.iLeft		= 0;
	ClippingRect.iTop		= 0;
	ClippingRect.iRight		= SCREEN_WIDTH;
	ClippingRect.iBottom	= SCREEN_HEIGHT;

			memset( &VSurfaceDesc, 0, sizeof( VSURFACE_DESC ) );
			VSurfaceDesc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;
			//GetMLGFilename( VSurfaceDesc.ImageFile, MLG_SPLASH );
			sprintf( VSurfaceDesc.ImageFile, "LOADSCREENS\\splash.sti" );
			if( !AddVideoSurface( &VSurfaceDesc, &uiLogoID ) )
			{	
				fprintf(stderr,	"Failed to load %s\n", VSurfaceDesc.ImageFile);
				AssertMsg( 0, String( "Failed to load %s", VSurfaceDesc.ImageFile ) );
				return;
			}

			GetVideoSurface( &hVSurface, uiLogoID );
			BltVideoSurfaceToVideoSurface( ghFrameBuffer, hVSurface, 0, iScreenWidthOffset, iScreenHeightOffset, 0, NULL );
			DeleteVideoSurfaceFromIndex( uiLogoID );
	//#endif
			VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
			FilenameForBPP( "INTERFACE\\MBS.sti", VObjectDesc.ImageFile );
			if ( !AddVideoObject( &VObjectDesc, &uiPic ) )
			{
				fprintf(stderr,	"Failed to load %s\n", VObjectDesc.ImageFile);
				AssertMsg( 0, String( "Failed to load %s", VObjectDesc.ImageFile ) );
				return;
			}
			GetVideoObject(&hHandle, uiPic ); 
			BltVideoObject( FRAME_BUFFER , hHandle, 0, 0, 0, VO_BLT_SRCTRANSPARENCY,NULL );

	InvalidateScreen();
	//RefreshScreen( NULL );

	guiSplashStartTime = GetJA2Clock();
}
