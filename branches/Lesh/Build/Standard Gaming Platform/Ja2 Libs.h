#ifndef _JA2_LIBS_H_
#define _JA2_LIBS_H_


	//enums used for accessing the libraries
	enum
	{
		LIBRARY_DATA,
		LIBRARY_AMBIENT,
		LIBRARY_ANIMS,
		LIBRARY_BATTLESNDS,
		LIBRARY_BIGITEMS,
		LIBRARY_BINARY_DATA,
		LIBRARY_CURSORS,
		LIBRARY_FACES,
		LIBRARY_FONTS,
		LIBRARY_INTERFACE,
		LIBRARY_LAPTOP,
		LIBRARY_MAPS,
		LIBRARY_MERCEDT,
		LIBRARY_MUSIC,
		LIBRARY_NPC_SPEECH,
		LIBRARY_NPC_DATA,
		LIBRARY_RADAR_MAPS,
		LIBRARY_SOUNDS,
		LIBRARY_SPEECH,
//		LIBRARY_TILE_CACHE,
		LIBRARY_TILESETS,
		LIBRARY_LOADSCREENS,
		LIBRARY_INTRO,

#ifdef GERMAN
		LIBRARY_GERMAN_DATA,
#endif

#ifdef DUTCH
		LIBRARY_DUTCH_DATA,
#endif

#ifdef POLISH
		LIBRARY_POLISH_DATA,
#endif

#ifdef ITALIAN
		LIBRARY_ITALIAN_DATA,
#endif

#ifdef RUSSIAN
		LIBRARY_RUSSIAN_DATA,
#endif

		NUMBER_OF_LIBRARIES
	};


#endif
