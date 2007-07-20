#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
#else
#include "Types.h"
#include "vsurface.h"
#include "mainmenuscreen.h"
#include "Video.h"
#include "Timer Control.h"
#endif

UINT32 guiSplashFrameFade = 10;
UINT32 guiSplashStartTime = 0;
extern HVSURFACE ghFrameBuffer;

//Simply create videosurface, load image, and draw it to the screen.
void InitJA2SplashScreen()
{
#ifdef ENGLISH
	ClearMainMenu();

#else
	UINT32 uiLogoID = 0;
	HVSURFACE hVSurface; // unused jonathanl  // lalien reenabled for international versions
	VSURFACE_DESC VSurfaceDesc; //unused jonathanl // lalien reenabled for international versions
 #ifdef JA2TESTVERSION
	INT32 i = 0;

	memset( &VSurfaceDesc, 0, sizeof( VSURFACE_DESC ) );
	VSurfaceDesc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;
	sprintf( VSurfaceDesc.ImageFile, "LOADSCREENS\\Notification.sti" );
	if( !AddVideoSurface( &VSurfaceDesc, &uiLogoID ) )
	{	
		//AssertMsg( 0, String( "Failed to load %s", VSurfaceDesc.ImageFile ) );
		return;
	}
	GetVideoSurface(&hVSurface, uiLogoID );
			BltVideoSurfaceToVideoSurface( ghFrameBuffer, hVSurface, 0, 0, 0, 0, NULL );
	DeleteVideoSurfaceFromIndex( uiLogoID );


	InvalidateScreen();
	RefreshScreen( NULL );

	guiSplashStartTime = GetJA2Clock();
	while( i < 60 * 15 )//guiSplashStartTime + 15000 > GetJA2Clock() )
	{
		//Allow the user to pick his bum.
		InvalidateScreen();
		RefreshScreen( NULL );
		i++;
	}
#endif
	
			memset( &VSurfaceDesc, 0, sizeof( VSURFACE_DESC ) );
			VSurfaceDesc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;
			GetMLGFilename( VSurfaceDesc.ImageFile, MLG_SPLASH );
			if( !AddVideoSurface( &VSurfaceDesc, &uiLogoID ) )
			{	
				AssertMsg( 0, String( "Failed to load %s", VSurfaceDesc.ImageFile ) );
				return;
			}

			GetVideoSurface( &hVSurface, uiLogoID );
			BltVideoSurfaceToVideoSurface( ghFrameBuffer, hVSurface, 0, iScreenWidthOffset, iScreenHeightOffset, 0, NULL );
			DeleteVideoSurfaceFromIndex( uiLogoID );
	#endif

	InvalidateScreen();
	RefreshScreen( NULL );

	guiSplashStartTime = GetJA2Clock();
}