#include "ja2_sdl.h"
#include <SDL.h>



/*
** Function to read byte from SDL_RWops
** 
** in	src		input rwops
** ret			result of reading
**
*/
Uint8	SDLCALL SDL_Read8(SDL_RWops *src)
{
	Uint8 value;

	SDL_RWread(src, &value, (sizeof value), 1);
	return(value);
}




/*
** Function to save byte to SDL_RWops
** 
** in	dst		output rwops
** ret			result of writing (number of bytes written)
**
*/

int		SDLCALL SDL_Save8(SDL_RWops *dst, Uint8 value)
{
	return(SDL_RWwrite(dst, &value, (sizeof value), 1));
}

