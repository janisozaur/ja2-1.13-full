#if !defined( STCI_H )

#define STCI_H

// Sir-Tech's Crazy Image (STCI) file format specifications.  Each file is composed of:
// 1		ImageFileHeader, uncompressed
// *		Palette (STCI_INDEXED, size = uiNumberOfColours * PALETTE_ELEMENT_SIZE), uncompressed
// *		SubRectInfo's (usNumberOfRects > 0, size = usNumberOfSubRects * sizeof(SubRectInfo) ), uncompressed
// *		Bytes of image data, possibly compressed

#include <libgimp/gimp.h>
#include "types.h"

// Uncomment this, if you want to see debug output
//#define	DEBUG_PLUGIN

#ifdef DEBUG_PLUGIN
#define DEBUG(fmt, args...)	printf(fmt, ## args)
#else
#define DEBUG(fmt, args...)
#endif

#define STCI_ID_STRING       "STCI"
#define STCI_ID_LEN          4

#define STCI_ETRLE_COMPRESSED		0x0020
#define STCI_ZLIB_COMPRESSED		0x0010
#define STCI_INDEXED				0x0008
#define STCI_RGB					0x0004
#define STCI_ALPHA					0x0002
#define STCI_TRANSPARENT			0x0001

// ETRLE defines
#define COMPRESS_TRANSPARENT		0x80
#define COMPRESS_NON_TRANSPARENT	0x00
#define COMPRESS_RUN_LIMIT			0x7F

// RGB 16-bit color masks and depths
#define MASK_RED		0xF800
#define MASK_GREEN		0x07E0
#define MASK_BLUE		0x001F
#define MASK_ALPHA		0x0000
#define DEPTH_RED		5
#define DEPTH_GREEN		6
#define DEPTH_BLUE		5
#define DEPTH_ALPHA		0

// NB if you're going to change the header definition:
// - make sure that everything in this header is nicely aligned
// - don't exceed the 64-byte maximum
typedef struct
{
	CHAR8		id[STCI_ID_LEN];		// stci identifier
	UINT32		original_size;			// size of original image in bytes, if image is rgb
	UINT32		stored_size; 			// equal to original_size if data uncompressed
	UINT32		transparent_value;		// transparent color index (actually not used in game)
	UINT32		flags;					// image flags STCI_* ( only ETRLE_COMPRESSED, INDEXED and RGB are used)
	UINT16		height;					// image height (valid, if rgb image)
	UINT16		width;					// image width  (valid, if rgb image)
	union
	{
		struct 							// rgb image color info
		{
			UINT32		mask_red;		// red color mask
			UINT32		mask_green;		// green color mask
			UINT32		mask_blue;		// blue color mask
			UINT32		mask_alpha;		// alpha mask (not used in game)
			UINT8		depth_red;		// depth of red color (number of bits per color value)
			UINT8		depth_green;	// depth of green color (number of bits per color value)
			UINT8		depth_blue;		// depth of blue color (number of bits per color value)
			UINT8		depth_alpha;	// depth of alpha channel (number of bits per color value)
		} rgb;
		struct 							// indexed color image info
		{
							// For indexed files, the palette will contain 3 separate bytes for red, green, and blue
			UINT32		number_of_colours;		// number of colors in palette (seems to be always 256)
			UINT16		number_of_subimages;	// number of frames (subimages) in image
			UINT8		depth_red;				// depth of red color (always 8)
			UINT8		depth_green;			// depth of green color (always 8)
			UINT8		depth_blue;				// depth of blue color (always 8)
			UINT8		indexed_unused[11];
		} indexed;
	};
	UINT8		depth;					// size in bits of one pixel as stored in the file (can be 8 or 16)
	UINT32		aux_data_size;			// size of aux data in bytes
	UINT8		unused[15];
} STCIHeader;

#define STCI_HEADER_SIZE 64

typedef struct 							// this struct is used only when image is indexed
{
	UINT32			data_offset;		// start of subimage data in RLE data
	UINT32			data_length;		// length of compressed subimage data
	INT16			offset_x;			// x offset of subimage relative to anchor (center) point
	INT16			offset_y;			// y offset of subimage relative to anchor (center) point
	UINT16			height;				// subimage height
	UINT16			width;				// subimage width
} STCISubImage;

#define STCI_SUBIMAGE_SIZE 16

typedef struct
{
	UINT8			red;				// palette's red color value
	UINT8			green;				// palette's green color value
	UINT8			blue;				// palette's blue color value
} STCIPaletteElement;

#define STCI_PALETTE_ELEMENT_SIZE 3
#define STCI_8BIT_PALETTE_SIZE 768

typedef struct 							// not yet investigated :(
{
	UINT8		wall_orientation;
	UINT8		number_of_tiles;
	UINT16		tile_loc_index;
	UINT8		unused1[3];
	UINT8		current_frame;
	UINT8		number_of_frames;
	UINT8		flags;
	UINT8		unused[6];
} AuxObjectData;

typedef struct
{
	STCIHeader				header;			// sti header
	STCIPaletteElement		palette[256];	// sti palette
	BOOLEAN					is_8bit_image;	// 8 bit image flag
	UINT32					rgb_data_size;	// size of rgb data
	UINT16					*rgb_data;		// rgb data itself
	UINT32					pages_count;	// number of pages in indexed sti
	STCISubImage			*page;			// page headers
	UINT32					rle_data_size;	// size of rle compressed 8 bit data of page images
	UINT8					*rle_data;		// rle compressed 8 bit data of page images itself
	AuxObjectData			*aux_data;		// aux sti data, that is shipped sometimes with multipage sti
} STCIImage;


gint		sti_load_image( const char *filename, STCIImage *image );
gint		sti_save_image( const char *filename, STCIImage *image );
gint 		sti_free_image( STCIImage *image );
gint		sti_get_max_image_dimensions( STCIImage *image, gint *left, gint *right, gint *up, gint *down );
gint		sti_unpack_rle_page( STCIImage *image, gint page_index, gint32 layer );
gint32		create_gimp_image_rgb( STCIImage *image );
gint32		create_gimp_image_indexed( STCIImage *image );
gboolean	sti_create_image_from_gimp_image( gint32 image_id, STCIImage *image );

#endif
