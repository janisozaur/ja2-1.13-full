#ifndef __VSURFACE_PRIVATE_
#define __VSURFACE_PRIVATE_

#include "SDL.h"

// ***********************************************************************
// 
// PRIVATE, INTERNAL Header used by other SGP Internal modules
//
// Allows direct access to underlying Direct Draw Implementation
//
// ***********************************************************************

LPDIRECTDRAWSURFACE2 GetVideoSurfaceDDSurface( HVSURFACE hVSurface );
LPDIRECTDRAWSURFACE  GetVideoSurfaceDDSurfaceOne( HVSURFACE hVSurface );
LPDIRECTDRAWPALETTE  GetVideoSurfaceDDPalette( HVSURFACE hVSurface );

HVSURFACE CreateVideoSurfaceFromSDLSurface( SDL_Surface *pSurface );

#endif