#ifndef _JA2_SDL_H
#define _JA2_SDL_H

#include <SDL.h>

/*
** Additional macros and functions for loading/saving data through SDL_RWops
*/

Uint8	SDLCALL SDL_Read8(SDL_RWops *src);
int		SDLCALL SDL_Save8(SDL_RWops *dst, Uint8 value);

#endif
