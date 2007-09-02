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

HVSURFACE CreateVideoSurfaceFromSDLSurface( SDL_Surface *pSurface );

#endif
