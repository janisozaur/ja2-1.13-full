#ifdef JA2_PRECOMPILED_HEADERS
	#include "JA2 SGP ALL.H"
#else
	#include "Platform.h"
	#include "DEBUG.H"
	#include "video.h"
	#include "himage.h"
	#include "vsurface.h"
	#include "vsurface_private.h"
	#include "WCheck.h"
	#include "vobject_blitters.h"
#endif

extern void SetClippingRect(SGPRect *clip);
extern void GetClippingRect(SGPRect *clip);

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Video Surface SGP Module
//
// Second Revision: Dec 10, 1996, Andrew Emmons
//
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Defines
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//
// This define is sent to CreateList SGP function. It dynamically re-sizes if
// the list gets larger
//

#define DEFAULT_NUM_REGIONS					5
#define DEFAULT_VIDEO_SURFACE_LIST_SIZE		10

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// LOCAL functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN UpdateBackupSurface( HVSURFACE hVSurface );
BOOLEAN ClipReleatedSrcAndDestRectangles( HVSURFACE hDestVSurface, HVSURFACE hSrcVSurface, SGPRect *DestRect, SGPRect *SrcRect );
BOOLEAN FillSurface( HVSURFACE hDestVSurface, blt_vs_fx *pBltFx );
BOOLEAN FillSurfaceRect( HVSURFACE hDestVSurface, blt_vs_fx *pBltFx );
BOOLEAN BltVSurfaceUsingDD( HVSURFACE hDestVSurface, HVSURFACE hSrcVSurface, UINT32 fBltFlags, INT32 iDestX, INT32 iDestY, SGPRect *SrcRect );
BOOLEAN BltVSurfaceUsingDDBlt( HVSURFACE hDestVSurface, HVSURFACE hSrcVSurface, UINT32 fBltFlags, INT32 iDestX, INT32 iDestY, SGPRect *SrcRect, SGPRect *DestRect );

void DeletePrimaryVideoSurfaces( );

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// LOCAL global variables
//
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct VSURFACE_NODE
{
	HVSURFACE	hVSurface;
	UINT32		uiIndex;
  struct VSURFACE_NODE *next, *prev;

	#ifdef SGP_VIDEO_DEBUGGING
		UINT8	*pName;
		UINT8	*pCode;
	#endif

}VSURFACE_NODE;

VSURFACE_NODE	*gpVSurfaceHead = NULL;
VSURFACE_NODE	*gpVSurfaceTail = NULL;
UINT32			guiVSurfaceIndex      = 0;
UINT32			guiVSurfaceSize       = 0;
UINT32			guiVSurfaceTotalAdded = 0;

#ifdef _DEBUG
enum
{
	DEBUGSTR_NONE,
	DEBUGSTR_SETVIDEOSURFACETRANSPARENCY,
	DEBUGSTR_ADDVIDEOSURFACEREGION,
	DEBUGSTR_GETVIDEOSURFACEDESCRIPTION,
	DEBUGSTR_BLTVIDEOSURFACE_DST,
	DEBUGSTR_BLTVIDEOSURFACE_SRC,
	DEBUGSTR_COLORFILLVIDEOSURFACEAREA,
	DEBUGSTR_SHADOWVIDEOSURFACERECT,
	DEBUGSTR_BLTSTRETCHVIDEOSURFACE_DST,
	DEBUGSTR_BLTSTRETCHVIDEOSURFACE_SRC,
	DEBUGSTR_DELETEVIDEOSURFACEFROMINDEX
};

UINT8 gubVSDebugCode = 0;

void CheckValidVSurfaceIndex( UINT32 uiIndex );
#endif

INT32				giMemUsedInSurfaces;


//OBSOLETE!!!!!!!!!
HLIST				ghVideoSurfaces = NULL;
//OBSOLETE!!!!!!!!!


HVSURFACE	ghPrimary     = NULL;
HVSURFACE	ghBackBuffer  = NULL;
HVSURFACE   ghFrameBuffer = NULL;
HVSURFACE   ghMouseBuffer = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Video Surface Manager functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// **************************************************************
// Initialization of video surface manager
//  [ret] TRUE if successful, FALSE if not
// **************************************************************
BOOLEAN InitializeVideoSurfaceManager( )
{
	//Shouldn't be calling this if the video surface manager already exists.
	//Call shutdown first...
	Assert( !gpVSurfaceHead );
	Assert( !gpVSurfaceTail );
	RegisterDebugTopic(TOPIC_VIDEOSURFACE, "Video Surface Manager");
	gpVSurfaceHead = gpVSurfaceTail = NULL;

	giMemUsedInSurfaces = 0;

	// Create primary and backbuffer from globals
	if ( !SetPrimaryVideoSurfaces( ) )
	{
		DbgMessage(TOPIC_VIDEOSURFACE, DBG_LEVEL_1, String( "Could not create primary surfaces" ) );
		return FALSE;
	}

	return TRUE ;
}

// **************************************************************
// Shutdown of video surface manager
//  [ret] TRUE always
// **************************************************************
BOOLEAN ShutdownVideoSurfaceManager( )
{
	VSURFACE_NODE *curr;

  DbgMessage(TOPIC_VIDEOSURFACE, DBG_LEVEL_0, "Shutting down the Video Surface manager");

	// Delete primary viedeo surfaces
	DeletePrimaryVideoSurfaces( );

	while( gpVSurfaceHead )
	{
		curr = gpVSurfaceHead;
		gpVSurfaceHead = gpVSurfaceHead->next;
		DeleteVideoSurface( curr->hVSurface );
		#ifdef SGP_VIDEO_DEBUGGING
			if( curr->pName )
				MemFree( curr->pName );
			if( curr->pCode )
				MemFree( curr->pCode );
		#endif
		MemFree( curr );
	}
	gpVSurfaceHead = NULL;
	gpVSurfaceTail = NULL;
	guiVSurfaceIndex = 0;
	guiVSurfaceSize = 0;
	guiVSurfaceTotalAdded = 0;
	UnRegisterDebugTopic(TOPIC_VIDEOSURFACE, "Video Objects");
	return TRUE;
}

// **************************************************************
// Restore video surfaces
//  [ret] TRUE always
// **************************************************************
BOOLEAN RestoreVideoSurfaces( )
{
	VSURFACE_NODE *curr;

	//
	// Loop through Video Surfaces and Restore
	//
	curr = gpVSurfaceTail;
	while( curr )
	{
		if( !RestoreVideoSurface( curr->hVSurface ) )
		{
			return FALSE;
		}
		curr = curr->prev;
	}
	return TRUE;
}

// **************************************************************
// Adds a video surface to queue
//	[in]  pVSurfaceDesc - video surface description
//	[out] puiIndex - index of the new surface
//  [ret] TRUE if successful, FALSE if not
// **************************************************************
BOOLEAN AddStandardVideoSurface( VSURFACE_DESC *pVSurfaceDesc, UINT32 *puiIndex )
{
	HVSURFACE hVSurface;

	// Assertions
	Assert( puiIndex );
	Assert( pVSurfaceDesc );

	// Create video object
	hVSurface = CreateVideoSurface( pVSurfaceDesc );

	if( !hVSurface )
	{
		// Video Object will set error condition.
		return FALSE ;
	}

	// Set transparency to default
	SetVideoSurfaceTransparencyColor( hVSurface, FROMRGB( 0, 0, 0 ) );

	// Set into video object list
	if( gpVSurfaceHead )
	{ //Add node after tail
		gpVSurfaceTail->next = (VSURFACE_NODE*)MemAlloc( sizeof( VSURFACE_NODE ) );
		Assert( gpVSurfaceTail->next ); //out of memory?
		gpVSurfaceTail->next->prev = gpVSurfaceTail;
		gpVSurfaceTail->next->next = NULL;
		gpVSurfaceTail             = gpVSurfaceTail->next;
	}
	else
	{ //new list
		gpVSurfaceHead = (VSURFACE_NODE*)MemAlloc( sizeof( VSURFACE_NODE ) );
		Assert( gpVSurfaceHead ); //out of memory?
		gpVSurfaceHead->prev = gpVSurfaceHead->next = NULL;
		gpVSurfaceTail       = gpVSurfaceHead;
	}
	#ifdef SGP_VIDEO_DEBUGGING
		gpVSurfaceTail->pName = NULL;
		gpVSurfaceTail->pCode = NULL;
	#endif
	//Set the hVSurface into the node.
	gpVSurfaceTail->hVSurface = hVSurface;
	gpVSurfaceTail->uiIndex   = guiVSurfaceIndex+=2;
	*puiIndex                 = gpVSurfaceTail->uiIndex;
	Assert( guiVSurfaceIndex < 0xfffffff0 ); //unlikely that we will ever use 2 billion VSurfaces!
	//We would have to create about 70 VSurfaces per second for 1 year straight to achieve this...
	guiVSurfaceSize++;
	guiVSurfaceTotalAdded++;

	return TRUE ;
}

// **************************************************************
// Lock the video surface data (to make draw operations)
// Must be unlocked after
//  [in]  uiVSurface - index of surface
//  [out] puiPitch - 
//  [ret] pointer to surface content (video data)
// **************************************************************
BYTE *LockVideoSurface( UINT32 uiVSurface, UINT32 *puiPitch )
{
	VSURFACE_NODE *curr;

	//
	// Check if given backbuffer or primary buffer
	//
#ifdef JA2
	if ( uiVSurface == PRIMARY_SURFACE )
	{
		return (BYTE *)LockPrimarySurface( puiPitch );
	}

	if ( uiVSurface == BACKBUFFER )
	{
		return (BYTE *)LockBackBuffer( puiPitch );
	}
#endif

	if ( uiVSurface == FRAME_BUFFER )
	{
		return (BYTE *)LockFrameBuffer( puiPitch );
	}

	if ( uiVSurface == MOUSE_BUFFER )
	{
		return (BYTE *)LockMouseBuffer( puiPitch );
	}

	//
	// Otherwise, use list
	//

	curr = gpVSurfaceHead;
	while( curr )
	{
		if( curr->uiIndex == uiVSurface )
		{
			break;
		}
		curr = curr->next;
	}
	if( !curr )
	{
		return FALSE;
	}

	//
	// Lock buffer
	//

	return LockVideoSurfaceBuffer( curr->hVSurface, puiPitch );

}

// **************************************************************
// Unlock the video surface data (end of draw operations)
//  [in]  uiVSurface - index of surface
// **************************************************************
void UnLockVideoSurface( UINT32 uiVSurface )
{
	VSURFACE_NODE *curr;

	//
	// Check if given backbuffer or primary buffer
	//
#ifdef JA2
	if ( uiVSurface == PRIMARY_SURFACE )
	{
		UnlockPrimarySurface();
		return;
	}

	if ( uiVSurface == BACKBUFFER )
	{
		UnlockBackBuffer();
		return;
	}
#endif

	if ( uiVSurface == FRAME_BUFFER )
	{
		UnlockFrameBuffer();
		return;
	}

	if ( uiVSurface == MOUSE_BUFFER )
	{
		UnlockMouseBuffer();
		return;
	}

	curr = gpVSurfaceHead;
	while( curr )
	{
		if( curr->uiIndex == uiVSurface )
		{
			break;
		}
		curr = curr->next;
	}
	if( !curr )
	{
		return;
	}

	//
	// unlock buffer
	//

	UnLockVideoSurfaceBuffer( curr->hVSurface );
}

// **************************************************************
// Set the transparent color value
//  [in]  uiIndex - index of surface
//  [in]  TransColor - color value to be transparent
//  [ret] TRUE always
// **************************************************************
BOOLEAN SetVideoSurfaceTransparency( UINT32 uiIndex, COLORVAL TransColor )
{
	HVSURFACE hVSurface;

	//
	// Get Video Surface
	//

	#ifdef _DEBUG
		gubVSDebugCode = DEBUGSTR_SETVIDEOSURFACETRANSPARENCY;
	#endif
	CHECKF( GetVideoSurface( &hVSurface, uiIndex ) );

	//
	// Set transparency
	//

	SetVideoSurfaceTransparencyColor( hVSurface, TransColor );

	return( TRUE );
}

// **************************************************************
// Get video surface width, height and bit depth
//  [in]  uiIndex - index of surface
//  [out] usWidth - pointer to width var
//  [out] usHeight - pointer to height var
//  [out] ubBitDepth - pointer to bit depth var
//  [ret] TRUE always
// **************************************************************
BOOLEAN GetVideoSurfaceDescription( UINT32 uiIndex, UINT16 *usWidth, UINT16 *usHeight, UINT8 *ubBitDepth )
{
	HVSURFACE hVSurface;

	Assert( usWidth != NULL );
	Assert( usHeight != NULL );
	Assert( ubBitDepth != NULL );

	//
	// Get Video Surface
	//

	#ifdef _DEBUG
		gubVSDebugCode = DEBUGSTR_GETVIDEOSURFACEDESCRIPTION;
	#endif
	CHECKF( GetVideoSurface( &hVSurface, uiIndex ) );

	*usWidth    = hVSurface->usWidth;
	*usHeight   = hVSurface->usHeight;
	*ubBitDepth = hVSurface->ubBitDepth;

	return TRUE;
}

// **************************************************************
// Get video surface object by specified index
//  [out] hVSurface - surface object
//  [in]  uiIndex - index of surface
//  [ret] TRUE if successful, FALSE if not
// **************************************************************
BOOLEAN GetVideoSurface( HVSURFACE *hVSurface, UINT32 uiIndex )
{
	VSURFACE_NODE *curr;

	#ifdef _DEBUG
		CheckValidVSurfaceIndex( uiIndex );
	#endif

	if ( uiIndex == PRIMARY_SURFACE )
	{
		*hVSurface = ghPrimary;
		return TRUE;
	}

	if ( uiIndex == BACKBUFFER )
	{
		*hVSurface = ghBackBuffer;
		return TRUE;
	}

	if ( uiIndex == FRAME_BUFFER )
	{
		*hVSurface = ghFrameBuffer;
		return TRUE;
	}

	if ( uiIndex == MOUSE_BUFFER )
	{
		*hVSurface = ghMouseBuffer;
		return TRUE;
	}

	curr = gpVSurfaceHead;
	while( curr )
	{
		if( curr->uiIndex == uiIndex )
		{
			*hVSurface = curr->hVSurface;
			return TRUE;
		}
		curr = curr->next;
	}
	return FALSE;
}

// **************************************************************
// Create primary surfaces, like frame buffer surface, primary,
// backbuffer and mouse surfaces
//  [ret] TRUE if successful, FALSE if not
// **************************************************************
BOOLEAN SetPrimaryVideoSurfaces( )
{
	SDL_Surface *pSurface;

	// Delete surfaces if they exist
	DeletePrimaryVideoSurfaces( );

	//
	// Get Primary surface
	//
	pSurface = GetPrimarySurfaceObject();
	CHECKF( pSurface != NULL );

	ghPrimary = CreateVideoSurfaceFromSDLSurface( pSurface );
	CHECKF( ghPrimary != NULL );

	//
	// Get Backbuffer surface
	//

	pSurface = GetBackBufferObject( );
	CHECKF( pSurface != NULL );

	ghBackBuffer = CreateVideoSurfaceFromSDLSurface( pSurface );
	CHECKF( ghBackBuffer != NULL );

	//
	// Get mouse buffer surface
	//
	pSurface = GetMouseBufferObject( );
	CHECKF( pSurface != NULL );

	ghMouseBuffer = CreateVideoSurfaceFromSDLSurface( pSurface );
	CHECKF( ghMouseBuffer != NULL );

	//
	// Get frame buffer surface
	//

	pSurface = GetFrameBufferObject( );
	CHECKF( pSurface != NULL );

	ghFrameBuffer = CreateVideoSurfaceFromSDLSurface( pSurface );
	CHECKF( ghFrameBuffer != NULL );

	return( TRUE );
}

// **************************************************************
// Deletes all primary surfaces
// **************************************************************
void DeletePrimaryVideoSurfaces( )
{
	//
	// If globals are not null, delete them
	//

	if ( ghPrimary != NULL )
	{
		//If there is a 16bpp palette, free it
		if( ghPrimary->p16BPPPalette != NULL )
		{
			MemFree( ghPrimary->p16BPPPalette );
			ghPrimary->p16BPPPalette = NULL;
		}

		giMemUsedInSurfaces -= ( ghPrimary->usHeight * ghPrimary->usWidth * ( ghPrimary->ubBitDepth / 8 ) );

		// Release object
		MemFree( ghPrimary );
		ghPrimary = NULL;
	}

	if ( ghBackBuffer != NULL )
	{
		//If there is a 16bpp palette, free it
		if( ghBackBuffer->p16BPPPalette != NULL )
		{
			MemFree( ghBackBuffer->p16BPPPalette );
			ghBackBuffer->p16BPPPalette = NULL;
		}

		giMemUsedInSurfaces -= ( ghBackBuffer->usHeight * ghBackBuffer->usWidth * ( ghBackBuffer->ubBitDepth / 8 ) );

		// Release object
		MemFree( ghBackBuffer );
		ghBackBuffer = NULL;
	}

	if ( ghFrameBuffer != NULL )
	{
		//If there is a 16bpp palette, free it
		if( ghFrameBuffer->p16BPPPalette != NULL )
		{
			MemFree( ghFrameBuffer->p16BPPPalette );
			ghFrameBuffer->p16BPPPalette = NULL;
		}

		giMemUsedInSurfaces -= ( ghFrameBuffer->usHeight * ghFrameBuffer->usWidth * ( ghFrameBuffer->ubBitDepth / 8 ) );

		// Release object
		MemFree( ghFrameBuffer );
		ghFrameBuffer = NULL;
	}

	if ( ghMouseBuffer != NULL )
	{
		//If there is a 16bpp palette, free it
		if( ghMouseBuffer->p16BPPPalette != NULL )
		{
			MemFree( ghMouseBuffer->p16BPPPalette );
			ghMouseBuffer->p16BPPPalette = NULL;
		}

		giMemUsedInSurfaces -= ( ghMouseBuffer->usHeight * ghMouseBuffer->usWidth * ( ghMouseBuffer->ubBitDepth / 8 ) );

		// Release object
		MemFree( ghMouseBuffer );
		ghMouseBuffer = NULL;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Given an index to the dest and src vobject contained in our private VSurface list
// Based on flags, blit accordingly
// There are two types, a BltFast and a Blt. BltFast is 10% faster, uses no
// clipping lists
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// **************************************************************
// Blits surface to another surface
//  [in]  uiDestVSurface - index of destination surface
//  [in]  uiSrcVSurface - index of source surface
//  [in]  usRegionIndex -
//  [in]  iDestX - destination surface X
//  [in]  iDestY - destination surface Y
//  [in]  fBltFlags - blit flags
//  [in]  pBltFx - 
//  [ret] TRUE if successful, FALSE if not
// **************************************************************
BOOLEAN BltVideoSurface(UINT32 uiDestVSurface, UINT32 uiSrcVSurface, UINT16 usRegionIndex, INT32 iDestX, INT32 iDestY, UINT32 fBltFlags, blt_vs_fx *pBltFx )
{
	HVSURFACE	hDestVSurface;
	HVSURFACE	hSrcVSurface;

	#ifdef _DEBUG
		gubVSDebugCode = DEBUGSTR_BLTVIDEOSURFACE_DST;
	#endif
	if( !GetVideoSurface( &hDestVSurface, uiDestVSurface ) )
	{
		return FALSE;
	}
	#ifdef _DEBUG
		gubVSDebugCode = DEBUGSTR_BLTVIDEOSURFACE_SRC;
	#endif
	if( !GetVideoSurface( &hSrcVSurface, uiSrcVSurface ) )
	{
		return FALSE;
	}
	if ( !BltVideoSurfaceToVideoSurface( hDestVSurface, hSrcVSurface, usRegionIndex, iDestX, iDestY, fBltFlags, pBltFx ) )
	{ // VO Blitter will set debug messages for error conditions
		return FALSE ;
	}
	return TRUE ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Fills an rectangular area with a specified color value.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// **************************************************************
// Fill surface by specified color
//  [in]  uiDestVSurface - index of destination surface
//  [in]  iDestX1 - left bound
//  [in]  iDestY1 - top bound
//  [in]  iDestX2 - right bound
//  [in]  iDestY2 - bottom bound
//  [in]  Color16BPP - color
//  [ret] TRUE if successful, FALSE if not
// **************************************************************
BOOLEAN ColorFillVideoSurfaceArea(UINT32 uiDestVSurface, INT32 iDestX1, INT32 iDestY1, INT32 iDestX2, INT32 iDestY2, UINT16 Color16BPP)
{
	blt_vs_fx	BltFx;
	HVSURFACE	hDestVSurface;
	SGPRect		Clip;

	#ifdef _DEBUG
		gubVSDebugCode = DEBUGSTR_COLORFILLVIDEOSURFACEAREA;
	#endif
	if( !GetVideoSurface( &hDestVSurface, uiDestVSurface ) )
	{
		return FALSE;
	}

	BltFx.ColorFill = Color16BPP;
	BltFx.DestRegion = 0;

	//
	// Clip fill region coords
	//

	GetClippingRect(&Clip);

	if(iDestX1 < Clip.iLeft)
		iDestX1 = Clip.iLeft;

	if(iDestX1 > Clip.iRight)
		return(FALSE);

	if(iDestX2 > Clip.iRight)
		iDestX2 = Clip.iRight;

	if(iDestX2 < Clip.iLeft)
		return(FALSE);

	if(iDestY1 < Clip.iTop)
		iDestY1 = Clip.iTop;

	if(iDestY1 > Clip.iBottom)
		return(FALSE);

	if(iDestY2 > Clip.iBottom)
		iDestY2 = Clip.iBottom;

	if(iDestY2 < Clip.iTop)
		return(FALSE);

	if((iDestX2 <= iDestX1) || (iDestY2 <= iDestY1))
		return(FALSE);

	BltFx.SrcRect.iLeft   =	BltFx.FillRect.iLeft   = iDestX1;
	BltFx.SrcRect.iTop    = BltFx.FillRect.iTop    = iDestY1;
	BltFx.SrcRect.iRight  = BltFx.FillRect.iRight  = iDestX2;
	BltFx.SrcRect.iBottom = BltFx.FillRect.iBottom = iDestY2;

	return( FillSurfaceRect( hDestVSurface, &BltFx ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Fills an rectangular area with a specified image value.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// **************************************************************
// Fill surface by specified image
//  [in]  uiDestVSurface - index of destination surface
//  [in]  iDestX1 - left bound
//  [in]  iDestY1 - top bound
//  [in]  iDestX2 - right bound
//  [in]  iDestY2 - bottom bound
//  [in]  BkgrndImg - image
//  [in]  Index - 
//  [in]  Ox - 
//  [in]  Oy - 
//  [ret] TRUE if successful, FALSE if not
// **************************************************************
BOOLEAN ImageFillVideoSurfaceArea(UINT32 uiDestVSurface, INT32 iDestX1, INT32 iDestY1, INT32 iDestX2, INT32 iDestY2, HVOBJECT BkgrndImg, UINT16 Index, INT16 Ox, INT16 Oy)
{
	INT16 xc,yc,hblits,wblits,aw,pw,ah,ph,w,h,xo,yo;
	ETRLEObject		*pTrav;
	SGPRect NewClip,OldClip;

	pTrav = &(BkgrndImg->pETRLEObject[Index]);
	ph = (INT16)(pTrav->usHeight+pTrav->sOffsetY);
	pw = (INT16)(pTrav->usWidth+pTrav->sOffsetX);

	ah = (INT16)(iDestY2-iDestY1);
	aw = (INT16)(iDestX2-iDestX1);

	Ox %= pw;
	Oy %= ph;

	if(Ox>0)
		Ox -= pw;
	xo = (-Ox)%pw;

	if(Oy>0)
		Oy -= ph;
	yo = (-Oy)%ph;

	if(Ox<0)
		xo = (-Ox)%pw;
	else
	{
		xo = pw-(Ox%pw);
		Ox -= pw;
	}

	if(Oy<0)
		yo = (-Oy)%ph;
	else
	{
		yo = ph-(Oy%pw);
		Oy -= ph;
	}

	hblits = ((ah+yo)/ph) + (((ah+yo)%ph)?1:0);
	wblits = ((aw+xo)/pw) + (((aw+xo)%pw)?1:0);

	if((hblits==0) || (wblits==0))
		return(FALSE);

	//
	// Clip fill region coords
	//

	GetClippingRect(&OldClip);

	NewClip.iLeft=iDestX1;
	NewClip.iTop=iDestY1;
	NewClip.iRight=iDestX2;
	NewClip.iBottom=iDestY2;

	if(NewClip.iLeft < OldClip.iLeft)
		NewClip.iLeft = OldClip.iLeft;

	if(NewClip.iLeft > OldClip.iRight)
		return(FALSE);

	if(NewClip.iRight > OldClip.iRight)
		NewClip.iRight = OldClip.iRight;

	if(NewClip.iRight < OldClip.iLeft)
		return(FALSE);

	if(NewClip.iTop < OldClip.iTop)
		NewClip.iTop = OldClip.iTop;

	if(NewClip.iTop > OldClip.iBottom)
		return(FALSE);

	if(NewClip.iBottom > OldClip.iBottom)
		NewClip.iBottom = OldClip.iBottom;

	if(NewClip.iBottom < OldClip.iTop)
		return(FALSE);

	if((NewClip.iRight <= NewClip.iLeft) || (NewClip.iBottom <= NewClip.iTop))
		return(FALSE);

	SetClippingRect(&NewClip);

	yc = (INT16)iDestY1;
	for(h=0;h<hblits;h++)
	{
		xc = (INT16)iDestX1;
		for(w=0;w<wblits;w++)
		{
			BltVideoObject(uiDestVSurface, BkgrndImg, Index, xc+Ox, yc+Oy, VO_BLT_SRCTRANSPARENCY, NULL);
			xc += pw;
		}
		yc += ph;
	}

	SetClippingRect(&OldClip);
	return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Video Surface Manipulation Functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// **************************************************************
// Create surface by specified surface description
//  [in]  VSurfaceDesc - surface descriptor
//  [ret] pointer to new surface
// **************************************************************
HVSURFACE CreateVideoSurface( VSURFACE_DESC *VSurfaceDesc )
{
	HVSURFACE				hVSurface;
	HIMAGE					hImage;
	SGPRect					tempRect;
	UINT16					usHeight;
	UINT16					usWidth;
	UINT8					ubBitDepth;
	UINT32					fMemUsage;
	UINT32					uiRBitMask;
	UINT32					uiGBitMask;
	UINT32					uiBBitMask;
	UINT32					uiABitMask;
	SDL_Surface				*pNewSurface;

	//
	// The description structure contains memory usage flag
	//

	fMemUsage = VSurfaceDesc->fCreateFlags;

	//
	// Check creation options
	//

	do
	{
		//
		// Check if creating from file
		//

		if ( VSurfaceDesc->fCreateFlags & VSURFACE_CREATE_FROMFILE )
		{
			//
			// Create himage object from file
			//

			hImage = CreateImage( VSurfaceDesc->ImageFile, IMAGE_ALLIMAGEDATA );

			if ( hImage == NULL )
			{
				DbgMessage( TOPIC_VIDEOSURFACE, DBG_LEVEL_2, "Invalid Image Filename given" );
				return( NULL );
			}

			//
			// Set values from himage
			//
			usHeight   = hImage->usHeight;
			usWidth    = hImage->usWidth;
			ubBitDepth = hImage->ubBitDepth;
			break;
		}


		//
		// If here, no special options given,
		// Set values from given description structure
		//

		usHeight   = VSurfaceDesc->usHeight;
		usWidth    = VSurfaceDesc->usWidth;
		ubBitDepth = VSurfaceDesc->ubBitDepth;

	}	while( FALSE );

	//
	// Assertions
	//

	Assert ( usHeight > 0 );
	Assert ( usWidth > 0 );

	//
	// First do Pixel Format
	//

	switch( ubBitDepth )
	{

		case 8:

			uiRBitMask = uiGBitMask = uiBBitMask = uiABitMask = 0;
			break;

		case 16:

			uiRBitMask = 0xF800;
			uiGBitMask = 0x07E0;
			uiBBitMask = 0x001F;
			uiABitMask = 0;
			break;

		case 24:
			
			uiRBitMask = 0x00FF0000;
			uiGBitMask = 0x0000FF00;
			uiBBitMask = 0x000000FF;
			uiABitMask = 0;
			break;

		case 32:
			
			uiRBitMask = 0xFF000000;
			uiGBitMask = 0x00FF0000;
			uiBBitMask = 0x0000FF00;
			uiABitMask = 0x000000FF;
			break;

		default:

			return( FALSE );
	}

	//
	// Create Surface
	//

	pNewSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, usWidth, usHeight, ubBitDepth,
		uiRBitMask, uiGBitMask, uiBBitMask, uiABitMask);

	if ( !pNewSurface )
	{
		fprintf(stderr, "Failed to create SDL surface in CreateVideoSurface(): %s\n", SDL_GetError());
		return NULL;
	}

	//
	// Allocate memory for Video Surface data and initialize
	//

	hVSurface = (HVSURFACE) MemAlloc( sizeof( SGPVSurface ) );
	memset( hVSurface, 0, sizeof( SGPVSurface ) );
	CHECKF( hVSurface != NULL );

	hVSurface->usHeight				= usHeight;
	hVSurface->usWidth				= usWidth;
	hVSurface->ubBitDepth			= ubBitDepth;
	hVSurface->p16BPPPalette		= NULL;
	hVSurface->pSurface				= pNewSurface;
	hVSurface->TransparentColor		= 0;
	hVSurface->fFlags				= 0;

	//
	// Initialize surface with hImage , if given
	//

	if ( VSurfaceDesc->fCreateFlags & VSURFACE_CREATE_FROMFILE )
	{
		tempRect.iLeft		= 0;
		tempRect.iTop		= 0;
		tempRect.iRight		= hImage->usWidth-1;
		tempRect.iBottom	= hImage->usHeight-1;
		SetVideoSurfaceDataFromHImage( hVSurface, hImage, 0, 0, &tempRect );

		//
		// Set palette from himage
		//

		if ( hImage->ubBitDepth == 8 )
		{
			SetVideoSurfacePalette( hVSurface, hImage->pPalette );
		}

		//
		// Delete himage object
		//

		DestroyImage( hImage );
	}

	//
	// All is well
	//
	giMemUsedInSurfaces += ( hVSurface->usHeight * hVSurface->usWidth * ( hVSurface->ubBitDepth / 8 ) );

	DbgMessage( TOPIC_VIDEOSURFACE, DBG_LEVEL_3, String("Success in Creating Video Surface" ) );

	return( hVSurface );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Called when surface is lost, for the most part called by utility functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// **************************************************************
// !! need convert to SDL
BOOLEAN RestoreVideoSurface( HVSURFACE hVSurface )
{
	//LPDIRECTDRAWSURFACE2		lpDDSurface;
	//LPDIRECTDRAWSURFACE2		lpBackupDDSurface;
	SGPRect										aRect;

	Assert( hVSurface != NULL );

	//
	// Restore is only for VIDEO MEMORY - should check if VIDEO and QUIT if not
	//

	if ( !( hVSurface->fFlags & VSURFACE_VIDEO_MEM_USAGE ) )
	{
		//
		// No second surfaace has been allocated, return failure
		//

		DbgMessage( TOPIC_VIDEOSURFACE, DBG_LEVEL_2, String("Failed to restore Video Surface surface" ) );
		return( FALSE );
	}

	//
	// Check for valid secondary surface
	//

	//if ( hVSurface->pSavedSurfaceData1 == NULL )
	//{
		////
		//// No secondary surface available
		////

	//  DbgMessage( TOPIC_VIDEOSURFACE, DBG_LEVEL_2, String("Failure in retoring- no secondary surface found" ) );
	//	return( FALSE );
	//}

	// Restore primary surface
	//lpDDSurface = ( LPDIRECTDRAWSURFACE2 )hVSurface->pSurfaceData;
	//DDRestoreSurface( lpDDSurface );

	// Blit backup surface data into primary
	//lpBackupDDSurface = ( LPDIRECTDRAWSURFACE2 )hVSurface->pSavedSurfaceData;

	aRect.iTop    = 0;
	aRect.iLeft   = 0;
	aRect.iBottom = hVSurface->usHeight;
	aRect.iRight  = hVSurface->usWidth;

	//DDBltFastSurface( (LPDIRECTDRAWSURFACE2)hVSurface->pSavedSurfaceData, 0, 0, (LPDIRECTDRAWSURFACE2)hVSurface->pSurfaceData, &aRect, DDBLTFAST_NOCOLORKEY );

	return( TRUE );
}


// **************************************************************
// Lock must be followed by release
// Pitch MUST be used for all width calculations ( Pitch is in bytes )
// The time between Locking and unlocking must be minimal
//  [in]  hVSurface - surface to lock
//  [out] pPitch - line pitch
//  [ret] pointer to surface video data
// **************************************************************
BYTE *LockVideoSurfaceBuffer( HVSURFACE hVSurface, UINT32 *pPitch )
{
	Assert( hVSurface != NULL );
	Assert( pPitch != NULL );

	if ( SDL_LockSurface( hVSurface->pSurface ) < 0 )
	{
		fprintf(stderr, "Failed to lock SDL surface in LockVideoSurfaceBuffer(): %s\n", SDL_GetError());
		return NULL;
	}

	*pPitch = hVSurface->pSurface->pitch;

	return (BYTE *)hVSurface->pSurface->pixels;
}

// **************************************************************
// Unlock surface
//  [in]  hVSurface - surface to unlock
// **************************************************************
void UnLockVideoSurfaceBuffer( HVSURFACE hVSurface )
{
	Assert( hVSurface != NULL );

	SDL_UnlockSurface( hVSurface->pSurface );
}

// **************************************************************
// Given an HIMAGE object, blit imagery into existing Video Surface. Can be from 8->16 BPP
//  [in]  hVSurface - surface to fill with image
//  [in]  hImage - an image to send to surface
//  [in]  usX - X coord of image on surface
//  [in]  usY - Y coord of image on surface
//  [in]  pSrcRect - clipping rectange
//  [ret] TRUE if successful, FALSE if not
// **************************************************************
BOOLEAN SetVideoSurfaceDataFromHImage( HVSURFACE hVSurface, HIMAGE hImage, UINT16 usX, UINT16 usY, SGPRect *pSrcRect )
{
	BYTE	*pDest;
	UINT32	fBufferBPP = 0;
	UINT32  uiPitch;
	UINT16  usEffectiveWidth;
	SGPRect	aRect;

	// Assertions
	Assert( hVSurface != NULL );
	Assert( hImage != NULL );

	// Get Size of hImage and determine if it can fit
	CHECKF( hImage->usWidth  >= hVSurface->usWidth );
	CHECKF( hImage->usHeight >= hVSurface->usHeight );

	// Check BPP and see if they are the same
	if ( hImage->ubBitDepth != hVSurface->ubBitDepth )
	{
		// They are not the same, but we can go from 8->16 without much cost
		if ( hImage->ubBitDepth == 8 && hVSurface->ubBitDepth == 16 )
		{
			fBufferBPP = BUFFER_16BPP;
		}
	}
	else
	{
		// Set buffer BPP
		switch ( hImage->ubBitDepth )
		{
			case 8:

				fBufferBPP = BUFFER_8BPP;
				break;

			case 16:

				fBufferBPP = BUFFER_16BPP;
				break;
		}

	}

	Assert( fBufferBPP != 0 );

	// Get surface buffer data
	pDest = LockVideoSurfaceBuffer( hVSurface, &uiPitch );

	// Effective width ( in PIXELS ) is Pitch ( in bytes ) converted to pitch ( IN PIXELS )
	usEffectiveWidth = (UINT16)( uiPitch / ( hVSurface->ubBitDepth / 8 ) );

	CHECKF( pDest != NULL );

	// Blit Surface
	// If rect is NULL, use entrie image size
	if ( pSrcRect == NULL )
	{
		aRect.iLeft   = 0;
		aRect.iTop    = 0;
		aRect.iRight  = hImage->usWidth;
		aRect.iBottom = hImage->usHeight;
	}
	else
	{
		aRect.iLeft   = pSrcRect->iLeft;
		aRect.iTop    = pSrcRect->iTop;
		aRect.iRight  = pSrcRect->iRight;
		aRect.iBottom = pSrcRect->iBottom;
	}

	// This HIMAGE function will transparently copy buffer
	if ( !CopyImageToBuffer( hImage, fBufferBPP, pDest, usEffectiveWidth, hVSurface->usHeight, usX, usY, &aRect ) )
	{
		DbgMessage( TOPIC_VIDEOSURFACE, DBG_LEVEL_2, String( "Error Occured Copying HIMAGE to HVSURFACE" ));
		UnLockVideoSurfaceBuffer( hVSurface );
		return( FALSE );
	}

	// All is OK
	UnLockVideoSurfaceBuffer( hVSurface );

	return( TRUE );
}

// **************************************************************
// Set palettes for video surface
// Palette setting is expensive, need to set both DDPalette and create 16BPP palette
//  [in]  hVSurface - surface to set up a palette
//  [in]  pSrcPalette - palette
//  [ret] TRUE if successful, FALSE if not
// **************************************************************
BOOLEAN SetVideoSurfacePalette( HVSURFACE hVSurface, SGPPaletteEntry *pSrcPalette )
{
	SDL_Color palette[256];
	INT32	cnt;

	Assert( hVSurface != NULL );

	if ( hVSurface->ubBitDepth != 8 )
		return TRUE;

	// to be sure...
	for (cnt=0; cnt < 256; cnt++)
	{
		palette[cnt].r = pSrcPalette[cnt].peRed;
		palette[cnt].g = pSrcPalette[cnt].peGreen;
		palette[cnt].b = pSrcPalette[cnt].peBlue;
	}

	SDL_SetPalette( hVSurface->pSurface, SDL_LOGPAL, palette, 0, 256 );

	// Delete 16BPP Palette if one exists
	if ( hVSurface->p16BPPPalette != NULL )
	{
		MemFree( hVSurface->p16BPPPalette );
		hVSurface->p16BPPPalette = NULL;
	}

	// Create 16BPP Palette
	hVSurface->p16BPPPalette = Create16BPPPalette( pSrcPalette );

	DbgMessage(TOPIC_VIDEOSURFACE, DBG_LEVEL_3, String("Video Surface Palette change successfull" ));
	return( TRUE );
}

// Transparency needs to take RGB value and find best fit and place it into DD Surface
// colorkey value.
// **************************************************************
// !! need convert to SDL
BOOLEAN SetVideoSurfaceTransparencyColor( HVSURFACE hVSurface, COLORVAL TransColor )
{
 // DDCOLORKEY		ColorKey;
 // DWORD					fFlags = CLR_INVALID;
	//LPDIRECTDRAWSURFACE2	lpDDSurface;

	// Assertions
	Assert( hVSurface != NULL );

	//Set trans color into Video Surface
	hVSurface->TransparentColor = TransColor;

	// Get surface pointer
	//if ( hVSurface->pSurface )
	//{
	//	SDL_SetColorKey(hVSurface->pSurface, SDL_SRCCOLORKEY, TransColor);
	//}
	//lpDDSurface = (LPDIRECTDRAWSURFACE2)hVSurface->pSurfaceData;
	//CHECKF( lpDDSurface != NULL );

	// Get right pixel format, based on bit depth

	//switch( hVSurface->ubBitDepth )
	//{
	//		case 8:

	//			// Use color directly
	//			ColorKey.dwColorSpaceLowValue  = TransColor;
	//			ColorKey.dwColorSpaceHighValue = TransColor;
	//			break;

	//		case 16:

	//			fFlags = Get16BPPColor( TransColor );

	//			//fFlags now contains our closest match
	//			ColorKey.dwColorSpaceLowValue  = fFlags;
	//			ColorKey.dwColorSpaceHighValue = ColorKey.dwColorSpaceLowValue;
	//			break;

	//}

	//DDSetSurfaceColorKey( lpDDSurface, DDCKEY_SRCBLT, &ColorKey);

	return( TRUE );
}

// **************************************************************
// !! need convert to SDL
BOOLEAN GetVSurfacePaletteEntries( HVSURFACE hVSurface, SGPPaletteEntry *pPalette )
{
	//CHECKF( hVSurface->pPalette != NULL );

	//DDGetPaletteEntries( (LPDIRECTDRAWPALETTE)hVSurface->pPalette, 0, 0, 256, (PALETTEENTRY*)pPalette );

	return( TRUE );
}

// **************************************************************
// Deletes video surface with given index
//  [in]  uiIndex - surface index
//  [ret] TRUE if successful, FALSE if not
// **************************************************************
BOOLEAN DeleteVideoSurfaceFromIndex( UINT32 uiIndex )
{
	VSURFACE_NODE *curr;

	#ifdef _DEBUG
		gubVSDebugCode = DEBUGSTR_DELETEVIDEOSURFACEFROMINDEX;
		CheckValidVSurfaceIndex( uiIndex );
	#endif

	curr = gpVSurfaceHead;
	while( curr )
	{
		if( curr->uiIndex == uiIndex )
		{ //Found the node, so detach it and delete it.

			//Deallocate the memory for the video surface
			DeleteVideoSurface( curr->hVSurface );

			if( curr == gpVSurfaceHead )
			{ //Advance the head, because we are going to remove the head node.
				gpVSurfaceHead = gpVSurfaceHead->next;
			}
			if( curr == gpVSurfaceTail )
			{ //Back up the tail, because we are going to remove the tail node.
				gpVSurfaceTail = gpVSurfaceTail->prev;
			}
			//Detach the node from the vsurface list
			if( curr->next )
			{ //Make the prev node point to the next
				curr->next->prev = curr->prev;
			}
			if( curr->prev )
			{ //Make the next node point to the prev
				curr->prev->next = curr->next;
			}
			//The node is now detached.  Now deallocate it.

			#ifdef SGP_VIDEO_DEBUGGING
				if( curr->pName )
				{
					MemFree( curr->pName );
				}
				if( curr->pCode )
				{
					MemFree( curr->pCode );
				}
			#endif

			MemFree( curr );
			guiVSurfaceSize--;
			return TRUE;
		}
		curr = curr->next;
	}
	return FALSE;
}


// **************************************************************
// Deletes all palettes and SDL surfaces, free memory
//  [in]  hVSurface - surface to delete
//  [ret] TRUE if successful, FALSE if not
// **************************************************************
BOOLEAN DeleteVideoSurface( HVSURFACE hVSurface )
{
	// Assertions
	CHECKF( hVSurface != NULL );

	//Release surface
	if ( hVSurface->pSurface != NULL )
	{
		SDL_FreeSurface( hVSurface->pSurface );
		hVSurface->pSurface = NULL;
	}

	//If there is a 16bpp palette, free it
	if( hVSurface->p16BPPPalette != NULL )
	{
		MemFree( hVSurface->p16BPPPalette );
		hVSurface->p16BPPPalette = NULL;
	}

	giMemUsedInSurfaces -= ( hVSurface->usHeight * hVSurface->usWidth * ( hVSurface->ubBitDepth / 8 ) );

	// Release object
	MemFree( hVSurface );

	return( TRUE );
}

// *******************************************************************
//
// Blitting Functions
//
// *******************************************************************

// Blt  will use DD Blt or BltFast depending on flags.
// Will drop down into user-defined blitter if 8->16 BPP blitting is being done

// **************************************************************
BOOLEAN BltVideoSurfaceToVideoSurface( HVSURFACE hDestVSurface, HVSURFACE hSrcVSurface, UINT16 usIndex, INT32 iDestX, INT32 iDestY, INT32 fBltFlags, blt_vs_fx *pBltFx )
{
	//VSURFACE_REGION aRegion;
	SGPRect					 SrcRect/*, DestRect*/;
	UINT8					*pSrcSurface8, *pDestSurface8;
	//UINT16				*pDestSurface16, *pSrcSurface16;
	UINT32				uiSrcPitch, uiDestPitch, uiWidth, uiHeight;

	// Assertions
	Assert( hDestVSurface != NULL );

	// Check for fill, if true, fill entire region with color
	if ( fBltFlags & VS_BLT_COLORFILL )
	{
		return( FillSurface( hDestVSurface, pBltFx ) );
	}

	// Check for colorfill rectangle
	if ( fBltFlags & VS_BLT_COLORFILLRECT )
	{
		return( FillSurfaceRect( hDestVSurface, pBltFx ) );
	}

	// Check for source coordinate options - from region, specific rect or full src dimensions
	do
	{
		// Use SUBRECT if specified
		if ( fBltFlags & VS_BLT_SRCSUBRECT )
		{
			//SGPRect aSubRect;

			CHECKF( pBltFx != NULL );

			SrcRect = pBltFx->SrcRect;
			//aSubRect = pBltFx->SrcRect;

			//SrcRect.iTop    = aSubRect.iTop;
			//SrcRect.iLeft   = aSubRect.iLeft;
			//SrcRect.iBottom = aSubRect.iBottom;
			//SrcRect.iRight  = aSubRect.iRight;

			break;
		}

		// Here, use default, which is entire Video Surface
		// Check Sizes, SRC size MUST be <= DEST size
		if ( hDestVSurface->usHeight < hSrcVSurface->usHeight )
		{
				DbgMessage(TOPIC_VIDEOSURFACE, DBG_LEVEL_2, String( "Incompatible height size given in Video Surface blit" ));
				return( FALSE );
		}
		if ( hDestVSurface->usWidth < hSrcVSurface->usWidth )
		{
				DbgMessage(TOPIC_VIDEOSURFACE, DBG_LEVEL_2, String( "Incompatible height size given in Video Surface blit" ));
				return( FALSE );
		}

		SrcRect.iTop    = 0;
		SrcRect.iLeft   = 0;
		SrcRect.iBottom = hSrcVSurface->usHeight;
		SrcRect.iRight  = hSrcVSurface->usWidth;

	} while( FALSE );

	// Once here, assert valid Src
	Assert( hSrcVSurface != NULL );

	uiWidth  = hDestVSurface->usWidth;
	uiHeight = hDestVSurface->usHeight;

	// check for position entirely off the screen
	//if(iDestX >= DestRect.right)
	//	return(FALSE);
	//if(iDestY >= DestRect.bottom)
	//	return(FALSE);
	//if((iDestX+(INT32)uiWidth) < (INT32)DestRect.left)
	//	return(FALSE);
	//if((iDestY+(INT32)uiHeight) < (INT32)DestRect.top)
	//	return(FALSE);

	// Send dest position, rectangle, etc to DD bltfast function
	// First check BPP values for compatibility
	if ( hDestVSurface->ubBitDepth == 16 && hSrcVSurface->ubBitDepth == 16 )
	{
		CHECKF( BltVSurfaceUsingDD( hDestVSurface, hSrcVSurface, fBltFlags, iDestX, iDestY, &SrcRect ) );
	}
	else if ( hDestVSurface->ubBitDepth == 8 && hSrcVSurface->ubBitDepth == 8 )
	{
		if((pSrcSurface8=(UINT8 *)LockVideoSurfaceBuffer(hSrcVSurface, &uiSrcPitch))==NULL)
		{
			DbgMessage(TOPIC_VIDEOSURFACE, DBG_LEVEL_2, String( "Failed on lock of 8BPP surface for blitting" ));
			return(FALSE);
		}

		if((pDestSurface8=(UINT8 *)LockVideoSurfaceBuffer(hDestVSurface, &uiDestPitch))==NULL)
		{
			UnLockVideoSurfaceBuffer(hSrcVSurface);
			DbgMessage(TOPIC_VIDEOSURFACE, DBG_LEVEL_2, String( "Failed on lock of 8BPP dest surface for blitting" ));
			return(FALSE);
		}

		//Blt8BPPDataTo8BPPBuffer( UINT8 *pBuffer, UINT32 uiDestPitchBYTES, HVOBJECT hSrcVObject, INT32 iX, INT32 iY, UINT16 usIndex );
		Blt8BPPTo8BPP(pDestSurface8, uiDestPitch, pSrcSurface8, uiSrcPitch, iDestX, iDestY, SrcRect.iLeft, SrcRect.iTop, uiWidth, uiHeight);
		UnLockVideoSurfaceBuffer(hSrcVSurface);
		UnLockVideoSurfaceBuffer(hDestVSurface);
		return(TRUE);
	}
	else
	{
		DbgMessage(TOPIC_VIDEOSURFACE, DBG_LEVEL_2, String( "Incompatible BPP values with src and dest Video Surfaces for blitting" ));
		return( FALSE );
	}

	return( TRUE );
}



// ******************************************************************************************
//
// UTILITY FUNCTIONS
//
// ******************************************************************************************

// Blt to backup buffer
// **************************************************************
// !! need convert to SDL
BOOLEAN UpdateBackupSurface( HVSURFACE hVSurface )
{
	//SGPRect		aRect;

	//// Assertions
	//Assert( hVSurface != NULL );

	//// Validations
	//CHECKF( hVSurface->pSavedSurfaceData != NULL );

	//aRect.top = (int)0;
	//aRect.left = (int)0;
	//aRect.bottom = (int)hVSurface->usHeight;
	//aRect.right = (int)hVSurface->usWidth;

	//// Copy all contents into backup buffer
	//DDBltFastSurface( (LPDIRECTDRAWSURFACE2)hVSurface->pSurfaceData, 0, 0, (LPDIRECTDRAWSURFACE2)hVSurface->pSavedSurfaceData, &aRect, DDBLTFAST_NOCOLORKEY );

	return( TRUE );

}

// **************************************************************
// **************************************************************
// Create surface object from SDL surface
//  [in]  pSurface - SDL surface
//  [ret] pointer to new video surface
HVSURFACE CreateVideoSurfaceFromSDLSurface( SDL_Surface *pSurface )
{
	// Create Video Surface
	HVSURFACE			hVSurface = NULL;


	// Allocate Video Surface struct
	hVSurface = (HVSURFACE) MemAlloc( sizeof( SGPVSurface ) );

	hVSurface->usHeight				= pSurface->h;
	hVSurface->usWidth				= pSurface->w;
	hVSurface->ubBitDepth			= pSurface->format->BitsPerPixel;
	hVSurface->pSurface				= pSurface;
	hVSurface->fFlags				= 0;
	hVSurface->p16BPPPalette		= NULL;

	// All is well
	DbgMessage( TOPIC_VIDEOSURFACE, DBG_LEVEL_0, String("Success in Creating Video Surface from DD Surface" ) );

	return( hVSurface );
}

// **************************************************************
// Obtain a primary surface
//  [ret] pointer to primary video surface
// **************************************************************
HVSURFACE GetPrimaryVideoSurface( )
{
	return( ghPrimary );
}

// **************************************************************
// Obtain a backbuffer surface
//  [ret] pointer to backbuffer video surface
// **************************************************************
HVSURFACE GetBackBufferVideoSurface( )
{
	return( ghBackBuffer );
}

// **************************************************************
// Obtain a framebuffer surface
//  [ret] pointer to framebuffer video surface
// **************************************************************
HVSURFACE GetFrameBufferVideoSurface( )
{
	return( ghFrameBuffer );
}

// **************************************************************
// Obtain a mouse surface
//  [ret] pointer to mouse video surface
// **************************************************************
HVSURFACE GetMouseBufferVideoSurface( )
{
	return( ghMouseBuffer );
}


// UTILITY FUNCTIONS FOR BLITTING

// **************************************************************
BOOLEAN ClipReleatedSrcAndDestRectangles( HVSURFACE hDestVSurface, HVSURFACE hSrcVSurface, SGPRect *DestRect, SGPRect *SrcRect )
{

	Assert( hDestVSurface != NULL );
	Assert( hSrcVSurface != NULL );

	// Check for invalid start positions and clip by ignoring blit
	if ( DestRect->iLeft >= hDestVSurface->usWidth || DestRect->iTop >= hDestVSurface->usHeight )
	{
		return( FALSE );
	}

	if ( SrcRect->iLeft >= hSrcVSurface->usWidth || SrcRect->iTop >= hSrcVSurface->usHeight )
	{
		return( FALSE );
	}

	// For overruns
	// Clip destination rectangles
	if ( DestRect->iRight > hDestVSurface->usWidth )
	{
		// Both have to be modified or by default streching occurs
		DestRect->iRight = hDestVSurface->usWidth;
		SrcRect->iRight = SrcRect->iLeft + ( DestRect->iRight - DestRect->iLeft );
	}
	if ( DestRect->iBottom > hDestVSurface->usHeight )
	{
		// Both have to be modified or by default streching occurs
		DestRect->iBottom = hDestVSurface->usHeight;
		SrcRect->iBottom = SrcRect->iTop + ( DestRect->iBottom - DestRect->iTop );
	}

	// Clip src rectangles
	if ( SrcRect->iRight > hSrcVSurface->usWidth )
	{
		// Both have to be modified or by default streching occurs
		SrcRect->iRight = hSrcVSurface->usWidth;
		DestRect->iRight = DestRect->iLeft  + ( SrcRect->iRight - SrcRect->iLeft );
	}
	if ( SrcRect->iBottom > hSrcVSurface->usHeight )
	{
		// Both have to be modified or by default streching occurs
		SrcRect->iBottom = hSrcVSurface->usHeight;
		DestRect->iBottom = DestRect->iTop + ( SrcRect->iBottom - SrcRect->iTop );
	}

	// For underruns
	// Clip destination rectangles
	if ( DestRect->iLeft < 0 )
	{
		// Both have to be modified or by default streching occurs
		DestRect->iLeft = 0;
		SrcRect->iLeft = SrcRect->iRight - ( DestRect->iRight - DestRect->iLeft );
	}
	if ( DestRect->iTop < 0 )
	{
		// Both have to be modified or by default streching occurs
		DestRect->iTop = 0;
		SrcRect->iTop = SrcRect->iBottom - ( DestRect->iBottom - DestRect->iTop );
	}

	// Clip src rectangles
	if ( SrcRect->iLeft < 0 )
	{
		// Both have to be modified or by default streching occurs
		SrcRect->iLeft = 0;
		DestRect->iLeft = DestRect->iRight  - ( SrcRect->iRight - SrcRect->iLeft );
	}
	if ( SrcRect->iTop < 0 )
	{
		// Both have to be modified or by default streching occurs
		SrcRect->iTop = 0;
		DestRect->iTop = DestRect->iBottom - ( SrcRect->iBottom - SrcRect->iTop );
	}

	return( TRUE );
}

// **************************************************************
BOOLEAN FillSurface( HVSURFACE hDestVSurface, blt_vs_fx *pBltFx )
{
	Assert( hDestVSurface != NULL );
	CHECKF( pBltFx != NULL );

	SDL_FillRect( hDestVSurface->pSurface, NULL, pBltFx->ColorFill);

	return( TRUE );
}

// **************************************************************
BOOLEAN FillSurfaceRect( HVSURFACE hDestVSurface, blt_vs_fx *pBltFx )
{
	SDL_Rect	rect;

	Assert( hDestVSurface != NULL );
	CHECKF( pBltFx != NULL );

	rect.x = pBltFx->FillRect.iLeft;
	rect.y = pBltFx->FillRect.iTop;
	rect.w = pBltFx->FillRect.iRight - pBltFx->FillRect.iLeft;
	rect.h = pBltFx->FillRect.iBottom - pBltFx->FillRect.iTop;

	SDL_FillRect( hDestVSurface->pSurface, &rect, pBltFx->ColorFill);

	//if ( hDestVSurface->fFlags & VSURFACE_VIDEO_MEM_USAGE && !(hDestVSurface->fFlags & VSURFACE_RESERVED_SURFACE) )
	//{
	//	UpdateBackupSurface( hDestVSurface );
	//}

	return( TRUE );
}

// **************************************************************
// !! need convert to SDL
BOOLEAN BltVSurfaceUsingDD( HVSURFACE hDestVSurface, HVSURFACE hSrcVSurface, UINT32 fBltFlags, INT32 iDestX, INT32 iDestY, SGPRect *SrcRect )
{
	UINT32		uiDDFlags;
	SGPRect		DestRect;
	SDL_Rect	srcRect, dstRect;

	srcRect.x = (UINT16)(SrcRect->iLeft);
	srcRect.y = (UINT16)(SrcRect->iTop);
	srcRect.w = (UINT16)(SrcRect->iRight - SrcRect->iLeft);
	srcRect.h = (UINT16)(SrcRect->iBottom - SrcRect->iTop);
	dstRect.x = iDestX;
	dstRect.y = iDestY;

  // Blit using the correct blitter
	if ( fBltFlags & VS_BLT_FAST )
	{
		// Validations
		CHECKF( iDestX >= 0 );
		CHECKF( iDestY >= 0 );

		// Default flags
		uiDDFlags = 0;

		// Convert flags into DD flags, ( for transparency use, etc )
		//if ( fBltFlags & VS_BLT_USECOLORKEY )
		//{
		//	SDL_SetColorKey( hSrcVSurface->pSurface, SDL_SRCCOLORKEY, hSrcVSurface->TransparentColor );
		//}

		// Convert flags into DD flags, ( for transparency use, etc )
		//if ( fBltFlags & VS_BLT_USEDESTCOLORKEY )
		//{
		//	SDL_SetColorKey( hDestVSurface->pSurface, SDL_SRCCOLORKEY, hDestVSurface->TransparentColor );
		//}

		//if ( uiDDFlags == 0 )
		//{
		//	// Default here is no colorkey
		//	SDL_SetColorKey( hSrcVSurface->pSurface, 0, 0 );
		//	SDL_SetColorKey( hDestVSurface->pSurface, 0, 0 );
		//}

		SDL_BlitSurface(hSrcVSurface->pSurface, &srcRect, hDestVSurface->pSurface, &dstRect);
	}
	else
	{
		// Normal, specialized blit for clipping, etc

		// Default flags
		//uiDDFlags = DDBLT_WAIT;
		uiDDFlags = 0;

		// Convert flags into DD flags, ( for transparency use, etc )
		//if ( fBltFlags & VS_BLT_USECOLORKEY )
		//{
		//	SDL_SetColorKey( hSrcVSurface->pSurface, SDL_SRCCOLORKEY, hSrcVSurface->TransparentColor );
		//}

		// Setup dest rectangle
		DestRect.iTop    = (int)iDestY;
		DestRect.iLeft   = (int)iDestX;
		DestRect.iBottom = (int)iDestY + ( SrcRect->iBottom - SrcRect->iTop  );
		DestRect.iRight  = (int)iDestX + ( SrcRect->iRight  - SrcRect->iLeft );

		// Do Clipping of rectangles
		if ( !ClipReleatedSrcAndDestRectangles( hDestVSurface, hSrcVSurface, &DestRect, SrcRect ) )
		{
			// Returns false because dest start is > dest size
			return( TRUE );
		}

		// Check values for 0 size
		if ( DestRect.iTop == DestRect.iBottom || DestRect.iRight == DestRect.iLeft )
		{
			return( TRUE );
		}

		SDL_BlitSurface(hSrcVSurface->pSurface, &srcRect, hDestVSurface->pSurface, &dstRect);
	}

	// Update backup surface with new data
	//if ( hDestVSurface->fFlags & VSURFACE_VIDEO_MEM_USAGE && !(hDestVSurface->fFlags & VSURFACE_RESERVED_SURFACE) )
	//{
	//	UpdateBackupSurface( hDestVSurface );
	//}

	return( TRUE );
}

BOOLEAN Blt16BPPBufferShadowRectAlternateTable(UINT16 *pBuffer, UINT32 uiDestPitchBYTES, SGPRect *area);


// **************************************************************
BOOLEAN InternalShadowVideoSurfaceRect(  UINT32	uiDestVSurface, INT32 X1, INT32 Y1, INT32 X2, INT32 Y2, BOOLEAN fLowPercentShadeTable )
{
	UINT16 *pBuffer;
	UINT32 uiPitch;
	SGPRect   area;
	HVSURFACE hVSurface;


	// CLIP IT!
	// FIRST GET SURFACE

	//
	// Get Video Surface
	//
	#ifdef _DEBUG
		gubVSDebugCode = DEBUGSTR_SHADOWVIDEOSURFACERECT;
	#endif
	CHECKF( GetVideoSurface( &hVSurface, uiDestVSurface ) );

	if ( X1 < 0 )
			X1 = 0;

	if ( X2 < 0 )
			return( FALSE );

	if ( Y2 < 0 )
			return( FALSE );

	if ( Y1 < 0 )
			Y1 = 0;

	if ( X2 >= hVSurface->usWidth )
			X2 = hVSurface->usWidth-1;

	if ( Y2 >= hVSurface->usHeight )
			Y2 = hVSurface->usHeight-1;

	if ( X1 >= hVSurface->usWidth )
			return( FALSE );

	if ( Y1 >= hVSurface->usHeight )
			return( FALSE );

	if (  ( X2 - X1 ) <= 0 )
			return( FALSE );

	if (  ( Y2 - Y1 ) <= 0 )
			return( FALSE );


	area.iTop=Y1;
	area.iBottom=Y2;
	area.iLeft=X1;
	area.iRight=X2;


	// Lock video surface
	pBuffer = (UINT16*)LockVideoSurface( uiDestVSurface, &uiPitch );
 	//UnLockVideoSurface( uiDestVSurface );

	if ( pBuffer == NULL )
	{
		return( FALSE );
	}

	if ( !fLowPercentShadeTable )
	{
		// Now we have the video object and surface, call the shadow function
		if(!Blt16BPPBufferShadowRect(pBuffer, uiPitch, &area))
		{
			// Blit has failed if false returned
			return( FALSE );
		}
	}
	else
	{
		// Now we have the video object and surface, call the shadow function
		if(!Blt16BPPBufferShadowRectAlternateTable(pBuffer, uiPitch, &area))
		{
			// Blit has failed if false returned
			return( FALSE );
		}
	}

	// Mark as dirty if it's the backbuffer
	//if ( uiDestVSurface == BACKBUFFER )
	//{
	//	InvalidateBackbuffer( );
	//}

	UnLockVideoSurface( uiDestVSurface );
	return( TRUE );
}

// **************************************************************
BOOLEAN ShadowVideoSurfaceRect(  UINT32	uiDestVSurface, INT32 X1, INT32 Y1, INT32 X2, INT32 Y2)
{
 return( InternalShadowVideoSurfaceRect( uiDestVSurface, X1, Y1, X2, Y2, FALSE ) );
}

// **************************************************************
BOOLEAN ShadowVideoSurfaceRectUsingLowPercentTable(  UINT32	uiDestVSurface, INT32 X1, INT32 Y1, INT32 X2, INT32 Y2)
{
 return( InternalShadowVideoSurfaceRect( uiDestVSurface, X1, Y1, X2, Y2, TRUE ) );
}


//
// BltVSurfaceUsingDDBlt will always use Direct Draw Blt,NOT BltFast
// **************************************************************
// !! need convert to SDL
BOOLEAN BltVSurfaceUsingDDBlt( HVSURFACE hDestVSurface, HVSURFACE hSrcVSurface, UINT32 fBltFlags, INT32 iDestX, INT32 iDestY, SGPRect *SrcRect, SGPRect *DestRect )
{
	SDL_Rect	srcRect, dstRect;

	// Convert flags into DD flags, ( for transparency use, etc )
	//if ( fBltFlags & VS_BLT_USECOLORKEY )
	//{
	//	SDL_SetColorKey(hSrcVSurface->pSurface, SDL_SRCCOLORKEY, hSrcVSurface->TransparentColor);
	//}

	srcRect.x = (UINT16) SrcRect->iLeft;
	srcRect.y = (UINT16) SrcRect->iTop;
	srcRect.w = (UINT16) (SrcRect->iRight - SrcRect->iLeft);
	srcRect.h = (UINT16) (SrcRect->iBottom - SrcRect->iTop);

	dstRect.x = (UINT16) DestRect->iLeft;
	dstRect.y = (UINT16) DestRect->iTop;
	dstRect.w = (UINT16) (DestRect->iRight - DestRect->iLeft);
	dstRect.h = (UINT16) (DestRect->iBottom - DestRect->iTop);

	SDL_SoftStretch(hSrcVSurface->pSurface, &srcRect, hDestVSurface->pSurface, &dstRect);

	//if ( fBltFlags & VS_BLT_USECOLORKEY )
	//{
	//	SDL_SetColorKey(hSrcVSurface->pSurface, 0, 0);
	//}

	// Update backup surface with new data
	//if ( hDestVSurface->fFlags & VSURFACE_VIDEO_MEM_USAGE && !(hDestVSurface->fFlags & VSURFACE_RESERVED_SURFACE) )
	//{
	//	UpdateBackupSurface( hDestVSurface );
	//}

	return( TRUE );
}


//
// This function will stretch the source image to the size of the dest rect.
//
// If the 2 images are not 16 Bpp, it returns false.
//
// **************************************************************
BOOLEAN BltStretchVideoSurface(UINT32 uiDestVSurface, UINT32 uiSrcVSurface, INT32 iDestX, INT32 iDestY, UINT32 fBltFlags, SGPRect *SrcRect, SGPRect *DestRect )
{
	HVSURFACE	hDestVSurface;
	HVSURFACE	hSrcVSurface;

	#ifdef _DEBUG
		gubVSDebugCode = DEBUGSTR_BLTSTRETCHVIDEOSURFACE_DST;
	#endif
	if( !GetVideoSurface( &hDestVSurface, uiDestVSurface ) )
	{
		return FALSE;
	}
	#ifdef _DEBUG
		gubVSDebugCode = DEBUGSTR_BLTSTRETCHVIDEOSURFACE_SRC;
	#endif
	if( !GetVideoSurface( &hSrcVSurface, uiSrcVSurface ) )
	{
		return FALSE;
	}

	//if the 2 images are not both 16bpp, return FALSE
	if( (hDestVSurface->ubBitDepth != 16) && (hSrcVSurface->ubBitDepth != 16) )
		return(FALSE);

	if(!BltVSurfaceUsingDDBlt( hDestVSurface, hSrcVSurface, fBltFlags, iDestX, iDestY, (SGPRect*)SrcRect, (SGPRect*)DestRect ) )
	{
		//
		// VO Blitter will set debug messages for error conditions
		//

		return( FALSE );
	}

	return( TRUE );
}

// **************************************************************
BOOLEAN ShadowVideoSurfaceImage( UINT32	uiDestVSurface, HVOBJECT hImageHandle, INT32 iPosX, INT32 iPosY)
{
	//Horizontal shadow
	ShadowVideoSurfaceRect( uiDestVSurface, iPosX+3, iPosY+hImageHandle->pETRLEObject->usHeight, iPosX+hImageHandle->pETRLEObject->usWidth, iPosY+	hImageHandle->pETRLEObject->usHeight+3);

	//vertical shadow
	ShadowVideoSurfaceRect( uiDestVSurface, iPosX+hImageHandle->pETRLEObject->usWidth, iPosY+3, iPosX+hImageHandle->pETRLEObject->usWidth+3, iPosY+	hImageHandle->pETRLEObject->usHeight);
	return( TRUE );
}

// **************************************************************
BOOLEAN MakeVSurfaceFromVObject(UINT32 uiVObject, UINT16 usSubIndex, UINT32 *puiVSurface)
{
	HVOBJECT hSrcVObject;
	UINT32 uiVSurface;
	VSURFACE_DESC hDesc;

	if(GetVideoObject(&hSrcVObject, uiVObject))
	{
		// ATE: Memset
		memset( &hDesc, 0, sizeof( VSURFACE_DESC ) );
		hDesc.fCreateFlags=VSURFACE_CREATE_DEFAULT;
		hDesc.usWidth=hSrcVObject->pETRLEObject[ usSubIndex ].usWidth;
		hDesc.usHeight=hSrcVObject->pETRLEObject[ usSubIndex ].usHeight;
		hDesc.ubBitDepth=PIXEL_DEPTH;

		if(AddVideoSurface(&hDesc, &uiVSurface))
		{
			if(BltVideoObjectFromIndex(uiVSurface, uiVObject, usSubIndex, 0, 0, VO_BLT_SRCTRANSPARENCY, NULL))
			{
				*puiVSurface=uiVSurface;
				return(TRUE);
			}
			else
				DeleteVideoSurfaceFromIndex(uiVSurface);
		}
	}		
	
	return(FALSE);
}


// **************************************************************
BOOLEAN SaveAsBMPVideoSurface(UINT32 uiVSurface, CHAR8 *name)
{
	HVSURFACE	hVSurface;

	if( !GetVideoSurface( &hVSurface, uiVSurface ) )
	{
		return FALSE;
	}

	return ( !SDL_SaveBMP(hVSurface->pSurface, name) );
}


#ifdef _DEBUG
// **************************************************************
void CheckValidVSurfaceIndex( UINT32 uiIndex )
{
	BOOLEAN fAssertError = FALSE;
	if( uiIndex == 0xffffffff )
	{ //-1 index -- deleted
		fAssertError = TRUE;
	}
	else if( uiIndex % 2 && uiIndex < 0xfffffff0  )
	{ //odd numbers are reserved for vobjects
		fAssertError = TRUE;
	}

	if( fAssertError )
	{
		UINT8 str[60];
		switch( gubVSDebugCode )
		{
			case DEBUGSTR_SETVIDEOSURFACETRANSPARENCY:
				sprintf( (char *)str, "SetVideoSurfaceTransparency" );
				break;
			case DEBUGSTR_ADDVIDEOSURFACEREGION:
				sprintf( (char *)str, "AddVideoSurfaceRegion" );
				break;
			case DEBUGSTR_GETVIDEOSURFACEDESCRIPTION:
				sprintf( (char *)str, "GetVideoSurfaceDescription" );
				break;
			case DEBUGSTR_BLTVIDEOSURFACE_DST:
				sprintf( (char *)str, "BltVideoSurface (dest)" );
				break;
			case DEBUGSTR_BLTVIDEOSURFACE_SRC:
				sprintf( (char *)str, "BltVideoSurface (src)" );
				break;
			case DEBUGSTR_COLORFILLVIDEOSURFACEAREA:
				sprintf( (char *)str, "ColorFillVideoSurfaceArea" );
				break;
			case DEBUGSTR_SHADOWVIDEOSURFACERECT:
				sprintf( (char *)str, "ShadowVideoSurfaceRect" );
				break;
			case DEBUGSTR_BLTSTRETCHVIDEOSURFACE_DST:
				sprintf( (char *)str, "BltStretchVideoSurface (dest)" );
				break;
			case DEBUGSTR_BLTSTRETCHVIDEOSURFACE_SRC:
				sprintf( (char *)str, "BltStretchVideoSurface (src)" );
				break;
			case DEBUGSTR_DELETEVIDEOSURFACEFROMINDEX:
				sprintf( (char *)str, "DeleteVideoSurfaceFromIndex" );
				break;
			case DEBUGSTR_NONE:
			default:
				sprintf( (char *)str, "GetVideoSurface" );
				break;
		}
		if( uiIndex == 0xffffffff )
		{
			AssertMsg( 0, String( "Trying to %s with deleted index -1." , str ) );
		}
		else
		{
			AssertMsg( 0, String( "Trying to %s using a VOBJECT ID %d!", str, uiIndex ) );
		}
	}
}
#endif

#ifdef SGP_VIDEO_DEBUGGING
typedef struct DUMPFILENAME
{
	UINT8 str[256];
}DUMPFILENAME;

// **************************************************************
void DumpVSurfaceInfoIntoFile( UINT8 *filename, BOOLEAN fAppend )
{
	VSURFACE_NODE *curr;
	FILE *fp;
	DUMPFILENAME *pName, *pCode;
	UINT32 *puiCounter;
	UINT8 tempName[ 256 ];
	UINT8 tempCode[ 256 ];
	UINT32 i, uiUniqueID;
	BOOLEAN fFound;
	if( !guiVSurfaceSize )
	{
		return;
	}

	if( fAppend )
	{
		fp = fopen( (const char *)filename, "a" );
	}
	else
	{
		fp = fopen( (const char *)filename, "w" );
	}
	Assert( fp );

	//Allocate enough strings and counters for each node.
	pName = (DUMPFILENAME*)MemAlloc( sizeof( DUMPFILENAME ) * guiVSurfaceSize );
	pCode = (DUMPFILENAME*)MemAlloc( sizeof( DUMPFILENAME ) * guiVSurfaceSize );
	memset( pName, 0, sizeof( DUMPFILENAME ) * guiVSurfaceSize );
	memset( pCode, 0, sizeof( DUMPFILENAME ) * guiVSurfaceSize );
	puiCounter = (UINT32*)MemAlloc( 4 * guiVSurfaceSize );
	memset( puiCounter, 0, 4 * guiVSurfaceSize );

	//Loop through the list and record every unique filename and count them
	uiUniqueID = 0;
	curr = gpVSurfaceHead;
	while( curr )
	{
		strcpy( tempName, curr->pName );
		strcpy( tempCode, curr->pCode );
		fFound = FALSE;
		for( i = 0; i < uiUniqueID; i++ )
		{
			if( !_stricmp( tempName, pName[i].str ) && !_stricmp( tempCode, pCode[i].str ) )
			{ //same string
				fFound = TRUE;
				(puiCounter[ i ])++;
				break;
			}
		}
		if( !fFound )
		{
			strcpy( pName[i].str, tempName );
			strcpy( pCode[i].str, tempCode );
			(puiCounter[ i ])++;
			uiUniqueID++;
		}
		curr = curr->next;
	}

	//Now dump the info.
	fprintf( fp, "-----------------------------------------------\n" );
	fprintf( fp, "%d unique vSurface names exist in %d VSurfaces\n", uiUniqueID, guiVSurfaceSize );
	fprintf( fp, "-----------------------------------------------\n\n" );
	for( i = 0; i < uiUniqueID; i++ )
	{
		fprintf( fp, "%d occurrences of %s\n", puiCounter[i], pName[i].str );
		fprintf( fp, "%s\n\n", pCode[i].str );
	}
	fprintf( fp, "\n-----------------------------------------------\n\n" );

	//Free all memory associated with this operation.
	MemFree( pName );
	MemFree( pCode );
	MemFree( puiCounter );
	fclose( fp );
}

//Debug wrapper for adding vsurfaces
// **************************************************************
template <typename type4>
BOOLEAN _AddAndRecordVSurface( VSURFACE_DESC *VSurfaceDesc, UINT32 *uiIndex, UINT32 uiLineNum, type4 pSourceFile )
{
	UINT16 usLength;
	UINT8 str[256];
	if( !AddStandardVideoSurface( VSurfaceDesc, uiIndex ) )
	{
		return FALSE;
	}

	//record the filename of the vsurface (some are created via memory though)
	usLength = strlen( VSurfaceDesc->ImageFile ) + 1;
	gpVSurfaceTail->pName = (UINT8*)MemAlloc( usLength );
	memset( gpVSurfaceTail->pName, 0, usLength );
	strcpy( gpVSurfaceTail->pName, VSurfaceDesc->ImageFile );

	//record the code location of the calling creating function.
	sprintf( (char *) str, "%s -- line(%d)", pSourceFile, uiLineNum );
	usLength = strlen( str ) + 1;
	gpVSurfaceTail->pCode = (UINT8*)MemAlloc( usLength );
	memset( gpVSurfaceTail->pCode, 0, usLength );
	strcpy( gpVSurfaceTail->pCode, str );

	return TRUE;
}

#endif

