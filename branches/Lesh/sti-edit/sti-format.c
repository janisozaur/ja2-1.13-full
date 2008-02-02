#include "sti-format.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

// gimp parasite names for sti non-image data
#define STI_AUX_DATA_FLAG				"aux-data"
#define STI_AUX_DATA_SUBIMAGE			"aux-data-subimage"
#define STI_USE_CENTER_POINT			"center-point"

// RLE compressor contsants
#define COMPRESS_TRANSPARENT			0x80
#define COMPRESS_NON_TRANSPARENT		0x00
#define COMPRESS_RUN_LIMIT				0x7F
#define TCI								0x00

// RLE compressor functions
gboolean	is_transparent_pixel( guchar *pixel, gint channels );
UINT32		ETRLECompress( guchar * pDest, UINT32 uiDestLen, guchar * pSource, UINT32 uiSourceLen, gint channels );
UINT32		ETRLECompressSubImage( gint32 drawable_ID, UINT8 * pDest, UINT32 uiDestLen, STCISubImage * pSubImage );

// STI load and save functions
gint	sti_load_image_rgb( FILE *file, STCIImage *image );
gint	sti_save_image_rgb( FILE *file, STCIImage *image );
gint	sti_load_image_indexed( FILE *file, STCIImage *image );
gint	sti_save_image_indexed( FILE *file, STCIImage *image );

// miscellaneous utility functions
gint		does_gimp_image_contain_incompatible_layers( gint32 image_id );
void		count_layer_types_in_gimp_image( gint32 image_id, gint *rgb, gint *gray, gint *indexed );

// GIMP to STI images conversion functions
gboolean 	sti_create_rgb_image( gint32 image_id, STCIImage *image );
gboolean 	sti_create_indexed_image( gint32 image_id, STCIImage *image );





/*****************************************************************
**
**  STI loading functions
**
******************************************************************/

// ============================================
// sti_load_image function - loads sti file
// ============================================
//	in	filename	: filename to read
//	in	image		: pointer to image data
//	ret	gint		: non-zero if error, zero if successful

gint	sti_load_image( const char *filename, STCIImage *image )
{
	FILE *file;
	
	// clear image
	memset( image, 0, sizeof(STCIImage) );
	DEBUG("request to open file %s\n", filename);
	
	// try to open
	file = fopen( filename, "r" );
	if ( !file )
		return 0;
	DEBUG("opened successfully\n");
	
	// read header
	if ( fread( &image->header, 1, STCI_HEADER_SIZE, file ) != STCI_HEADER_SIZE )
	{
		fclose( file );
		return 0;
	}
	DEBUG("header has been read\n");
	
	// need stci check
	
	// use 8-bit sti reader, or 16-bit?
	image->is_8bit_image = ( image->header.flags & STCI_INDEXED ) ? (1) : (0);
	if ( image->is_8bit_image )
	{
		// read 8-bit sti
		DEBUG("image is 8 bit\n");
		image->pages_count   = image->header.indexed.number_of_subimages;
		image->rle_data_size = image->header.stored_size;
		DEBUG("subimages = %d\n", image->pages_count);
		DEBUG("rle data  = %d\n", image->rle_data_size);
		if ( !sti_load_image_indexed( file, image ) )
		{
			fclose( file );
			return 0;
		}
	}
	else
	{
		// read 16-bit sti
		DEBUG("image is 16 bit\n");
		image->rgb_data_size = image->header.stored_size;
		if ( !sti_load_image_rgb( file, image ) )
		{
			fclose( file );
			return 0;
		}
	}

	fclose( file );
	DEBUG("reading completed with success\n");
	return 1;
}


// ============================================
// sti_load_image_rgb function - loads 16-bit
// sti file data
// ============================================
//	in	file		: file to read
//	in	image		: pointer to image data
//	ret	gint		: non-zero if error, zero if successful

gint sti_load_image_rgb( FILE *file, STCIImage *image )
{
	// allocate memory
	image->rgb_data = (UINT16*) malloc( image->rgb_data_size );
	if ( !image->rgb_data )
		return 0;
	
	// read rgb data
	if ( fread( image->rgb_data, 1, image->rgb_data_size, file ) != image->rgb_data_size )
		return 0;

	DEBUG("rgb image data reading completed with success\n");
	return 1;
}


// ============================================
// sti_load_image_indexed function - loads 8-bit
// sti file data
// ============================================
//	in	file		: file to read
//	in	image		: pointer to image data
//	ret	gint		: non-zero if error, zero if successful

gint sti_load_image_indexed( FILE *file, STCIImage *image )
{
	gint pages_size = sizeof(STCISubImage ) * image->pages_count;
	gint aux_size   = sizeof(AuxObjectData) * image->pages_count;
	gint size;
	
	// read palette
	size = fread( image->palette, 1, STCI_8BIT_PALETTE_SIZE, file );
	if ( size != STCI_8BIT_PALETTE_SIZE )
		return 0;
	DEBUG("reading palette completed with success\n");
	
	// read page headers
	image->page = (STCISubImage*) malloc( pages_size );
	if ( !image->page )
		return 0;
	
	size = fread( image->page, 1, pages_size, file );
	if ( size != pages_size )
		return 0;
	DEBUG("reading %d page headers completed with success\n", image->pages_count);
	
	// read rle data
	image->rle_data = (UINT8*) malloc( image->rle_data_size );
	if ( !image->rle_data )
		return 0;
	
	size = fread( image->rle_data, 1, image->rle_data_size, file );
	if ( size != image->rle_data_size )
		return 0;
	DEBUG("reading %d bytes of rle data completed with success\n", image->rle_data_size);
	
	// read aux data
	if ( image->header.aux_data_size > 0 )
	{
		DEBUG("aux data is present\n");
		image->aux_data = (AuxObjectData*) malloc( aux_size );
		if ( !image->aux_data )
			return 0;
		
		if ( fread( image->aux_data, 1, aux_size, file ) != aux_size )
			return 0;
		DEBUG("reading aux data completed with success\n");
	}

	return 1;
}








/*****************************************************************
**
**  STI saving functions
**
******************************************************************/

// ============================================
// sti_save_image function -
// saves sti image
// ============================================
//	in	filename	: file name
//	in  image		: sti image
//	ret	gint		: 1, if successful, 0, if not

gint	sti_save_image( const char *filename, STCIImage *image )
{
	FILE	*file;
	gint	success;

	DEBUG("sti_save_image()\n");
	DEBUG("filename = %s\n", filename);

	// open for write
	file = fopen( filename, "w" );
	if ( !file )
		return 0;
	
	// write header
	if ( fwrite( &image->header, 1, STCI_HEADER_SIZE, file ) != STCI_HEADER_SIZE )
	{
		fclose( file );
		return 0;
	}
	
	// choose 8-bit or 16-bit sti saver
	if ( image->is_8bit_image )
	{
		success = sti_save_image_indexed( file, image );
	}
	else
	{
		success = sti_save_image_rgb( file, image );
	}

	fclose( file );

	// if failed, try to remove file
	if ( !success )
	{
		remove( filename );
		return 0;
	}
	
	return 1;
}


// ============================================
// sti_save_image_rgb function -
// saves 16-bit sti image
// ============================================
//	in	file		: file to save
//	in  image		: sti image
//	ret	gint		: 1, if successful, 0, if not

gint	sti_save_image_rgb( FILE *file, STCIImage *image )
{
	DEBUG("sti_save_image_rgb()\n");

	if ( fwrite( image->rgb_data, 1, image->rgb_data_size, file ) != image->rgb_data_size )
		return 0;

	DEBUG("rgb image was saved successfully\n");
	return 1;
}


// ============================================
// sti_save_image_indexed function -
// saves 8-bit sti image
// ============================================
//	in	file		: file to save
//	in  image		: sti image
//	ret	gint		: 1, if successful, 0, if not

gint	sti_save_image_indexed( FILE *file, STCIImage *image )
{
	gint size;
	gint pages_size, aux_size;

	pages_size = image->pages_count * sizeof(STCISubImage);
	aux_size   = image->pages_count * sizeof(AuxObjectData);
	
	DEBUG("sti_save_image_indexed()\n");

	// write palette
	size = fwrite( image->palette, 1, STCI_8BIT_PALETTE_SIZE, file );
	if ( size != STCI_8BIT_PALETTE_SIZE )
		return 0;
	
	// write page headers
	size = fwrite( image->page, 1, pages_size, file );
	if ( size != pages_size )
		return 0;

	// write rle data
	size = fwrite( image->rle_data, 1, image->rle_data_size, file );
	if ( size != image->rle_data_size )
		return 0;

	// write aux data
	if ( image->header.aux_data_size > 0 )
	{
		if ( fwrite( image->aux_data, 1, aux_size, file ) != aux_size )
			return 0;
	}

	DEBUG("indexed image was saved successfully\n");
	return 1;
}







/*****************************************************************
**
**  'Finish work' functions
**
******************************************************************/

// ============================================
// sti_free_image function - free memory, used
// to store sti image data
// ============================================
//	in	image		: pointer to image data
//	ret	gint		: non-zero if error, zero if successful

gint sti_free_image( STCIImage *image )
{
	if ( !image )
		return 1;	// nothing to free
		
	if ( image->rgb_data )
		free( image->rgb_data );

	if ( image->page )
		free( image->page );

	if ( image->rle_data )
		free( image->rle_data );

	if ( image->aux_data )
		free( image->aux_data );

	memset( image, 0, sizeof(STCIImage) );
	
	return 1;
}








/*****************************************************************
**
**  GIMP image creating functions and related
**
******************************************************************/


// ============================================
// sti_get_max_image_dimensions function - 
// calculate bounding rect for subimages in sti image.
//   If all offsets are equal to zero, then bounding rect
// will be equal to dimensions of the most large subimage.
// I call it, offset point is in the left-upper corner of
// the image.
//   In case non-zero offsets, offset point is placed in
// center of image. This will help further to calculate
// offsets, when user requests to save the image.
// ============================================
//	in	image		: pointer to image data
//	out	left		: number of pixels to the left from the offset point
//	out	right		: number of pixels to the right from the offset point
//	out	up			: number of pixels to the up from the offset point
//	out	down		: number of pixels to the down from the offset point
//	ret	gint		: always 1

gint sti_get_max_image_dimensions( STCIImage *image, gint *left, gint *right, gint *up, gint *down )
{
	gint	max_left, max_right, max_up, max_down;
	gint	l, r, u, d;
	int		i;
	
	max_left = max_right = max_up = max_down = 0;

	// check, if image isn't 8-bit
	if ( !image->is_8bit_image )
	{
		max_left  = 0;
		max_up    = 0;
		max_right = image->header.width;
		max_down  = image->header.height;
	}
	else
	{
		// look in every subimage
		// calculate pixels for all 4 directions from offset point
		for ( i = 0; i < image->pages_count; i++ )
		{
			l = -image->page[i].offset_x;
			r =  image->page[i].width - l;
			u = -image->page[i].offset_y;
			d =  image->page[i].height - u;
			
			if ( l > max_left )
				max_left = l;
			if ( r > max_right )
				max_right = r;
			if ( u > max_up )
				max_up = u;
			if ( d > max_down )
				max_down = d;
		}
	}

	// check, if offset point should be in center of image	
	if ( max_left != 0 || max_up != 0 )
	{
		if ( max_left > max_right )
			max_right = max_left;
		else
			max_left = max_right;

		if ( max_up > max_down )
			max_down = max_up;
		else
			max_up = max_down;
	}
		
	*left  = max_left;
	*right = max_right;
	*up    = max_up;
	*down  = max_down;
	
	return 1;
}


// ============================================
// sti_unpack_rle_page function - decode rle-encoded
// subimage to gimp layer
// ============================================
//	in	image		: pointer to image data
//	in	page_index	: index of subimage to decompress
//	in	layer		: layer ID to store decoded image
//	ret	gint		: always 1

gint sti_unpack_rle_page( STCIImage *image, gint page_index, gint32 layer )
{
	GimpDrawable	*drawable;
	GimpPixelRgn	rgn_out;
	gint			width, height, channels;
	guchar			*outrow;
	int				rle_index, data_left, rows_left, column, pixels_to_skip, pixels_to_copy, row;
	UINT8			*rle_data;

	// get subimage dimensions
	width    = image->page[ page_index ].width;
	height   = image->page[ page_index ].height;
	
	// get drawable and fill area with transparent color
	drawable = gimp_drawable_get(layer);
	gimp_drawable_fill(drawable->drawable_id, GIMP_TRANSPARENT_FILL);

	// obtain bytes per pixel (channels), init pixel region and memory for one row of subimage	
	channels = gimp_drawable_bpp (drawable->drawable_id);
	gimp_pixel_rgn_init (&rgn_out, drawable, 0, 0, width, height, TRUE, FALSE);
	outrow   = g_new (guchar, channels * width);

	// prepare for loop
	rows_left = height;
	data_left = image->page[ page_index ].data_length;
	rle_data  = image->rle_data;
	rle_index = image->page[ page_index ].data_offset;
	row       = 0;
	
	// decompress rle
	while ( rows_left-- && data_left )
	{
		// clear row
		memset( outrow, 0, channels * width * sizeof(guchar) );
		column = 0;

		// decompress one row of subimage
		while ( data_left && (column < width) && (rle_data[ rle_index ] != 0) )
		{
			if ( rle_data[ rle_index ] & 0x80 )
			{
				// decompress transparent pixels
				pixels_to_skip = rle_data[ rle_index ] & 0x7F;
				while ( (column < width) && pixels_to_skip-- )
					column++;
				data_left--;
				rle_index++;
			}
			else
			{
				// decompress non-transparetn pixels
				pixels_to_copy = rle_data[ rle_index++ ];
				while ( data_left-- && (column < width) && pixels_to_copy-- )
				{
					outrow[ channels * column     ] = rle_data[ rle_index++ ];
					outrow[ channels * column + 1 ] = 255;
					column++;
				}
			}
		}
		rle_index++;
		
		// update gimp layer with a new row of data
		gimp_pixel_rgn_set_row (&rgn_out, outrow, 0, row, width);
		row++;
	}

	g_free( outrow );

	// tell gimp to update image
	gimp_drawable_flush(drawable);
	gimp_drawable_update (drawable->drawable_id, 0, 0, width, height);
	gimp_drawable_detach(drawable);

	return 1;
}


// ============================================
// create_gimp_image_rgb function - create a
// 16-bit image (actually RGBA) with one layer.
// ============================================
//	in	image		: pointer to image data
//	ret	gint32		: return new image ID, (-1) if error.

gint32 create_gimp_image_rgb( STCIImage *image )
{
	gint32			new_image, layer;
	gint			width, height, image_type, layer_type, channels;
	GimpDrawable	*drawable;
	GimpPixelRgn	rgn_out;
	gint			x, y;
	guchar			*outrow;
	gint32			color;

	// define new image characteristics - sizes and bpp
	width      = image->header.width;
	height     = image->header.height;
	image_type = GIMP_RGB;
	layer_type = GIMP_RGBA_IMAGE;
	
	// try to create new image. return -1, if call is fails.
	new_image = gimp_image_new(width, height, image_type);
	if ( new_image == -1)
	{
		return( -1 );
	}

	// then add layer to image
	layer = gimp_layer_new(new_image, "Main page", width, height, layer_type, 100, GIMP_NORMAL_MODE);
	gimp_image_add_layer(new_image, layer, 0);

	// get drawable and channels, init pixel region and memory for one row of image	
	drawable = gimp_drawable_get(layer);
	channels = gimp_drawable_bpp (drawable->drawable_id);
	gimp_pixel_rgn_init (&rgn_out, drawable, 0, 0, width, height, TRUE, FALSE);
	outrow   = g_new (guchar, channels * width);
	
	// row by row convert pixels from 16-bit to 24-bit
	for ( y = 0; y < height; y++ )
	{
		for ( x = 0; x < width; x++ )
		{
			color = image->rgb_data[ y * width + x ];
			outrow[ channels * x + 0] = (guchar)( (color & image->header.rgb.mask_red  ) >> 8);
			outrow[ channels * x + 1] = (guchar)( (color & image->header.rgb.mask_green) >> 3);
			outrow[ channels * x + 2] = (guchar)( (color & image->header.rgb.mask_blue ) << 3);
			outrow[ channels * x + 3] = 255;	// it is alpha channel, 255 means totally visible (0% transparency)
		}
		// push row to gimp layer
		gimp_pixel_rgn_set_row (&rgn_out, outrow, 0, y, width);
	}
	
	g_free( outrow );
	
	// tell gimp to update image
	gimp_drawable_flush(drawable);
	gimp_drawable_update (drawable->drawable_id, 0, 0, width, height);
	gimp_drawable_detach(drawable);
	
	return( new_image );
}


// ============================================
// create_gimp_image_indexed function - create a
// 8-bit image (actually INDEXEDA).
// ============================================
//	in	image		: pointer to image data
//	ret	gint32		: return new image ID, (-1) if error.

gint32 create_gimp_image_indexed( STCIImage *image )
{
	gint32			new_image, layer;
	gint			image_type, layer_type;
	gint			max_width, max_height, image_index, center_x, center_y;
	gint			left, right, up, down;
	char			name[32];
	gint32			aux_data_presence, use_center_point;

	// define image parameters - sizes and layer type	
	sti_get_max_image_dimensions( image, &left, &right, &up, &down );
	max_width  = left + right;
	max_height = up + down;
	image_type = GIMP_INDEXED;
	layer_type = GIMP_INDEXEDA_IMAGE;
	
	// define offset point
	center_x   = left;
	center_y   = up;

	DEBUG("image has %d pages\n", image->pages_count);
	DEBUG("max (W, H) = (%d, %d)\n", max_width, max_height);
	DEBUG("(left, right) = (%d, %d)\n", left, right);
	DEBUG("(up  , down ) = (%d, %d)\n", up, down);
	DEBUG("image aux data: %s\n", (image->header.aux_data_size > 0) ? "yes":"no");

	// create image
	new_image = gimp_image_new(max_width, max_height, image_type);
	if ( new_image == -1)
	{
		return( -1 );
	}

	// install colormap (palette) to image
	if ( !gimp_image_set_colormap( new_image, (guchar*)image->palette, 256) )
	{
		DEBUG("failed to set colormap\n");
		return( -1 );
	}
	
	// install gimp parasite to indicate sti aux data
	aux_data_presence = (image->header.aux_data_size > 0);
	gimp_image_attach_new_parasite( new_image, STI_AUX_DATA_FLAG, GIMP_PARASITE_PERSISTENT, sizeof(gint32), &aux_data_presence );

	// install gimp parasite to	indicate using of center point
	use_center_point = (center_x != 0 ) || (center_y != 0 );
	gimp_image_attach_new_parasite( new_image, STI_USE_CENTER_POINT, GIMP_PARASITE_PERSISTENT, sizeof(gint32), &use_center_point );
	
	// for each subimage
	for ( image_index = 0; image_index < image->pages_count; image_index++ )
	{
		// make name for layer, create a new layer and push it to stack
		sprintf(name, "page%03d", image_index);
		layer = gimp_layer_new(new_image, name,
								image->page[image_index].width,
								image->page[image_index].height,
								layer_type, 100, GIMP_NORMAL_MODE);
		gimp_image_add_layer(new_image, layer, image_index+1);

		// then decompress subimage data in a new layer
		sti_unpack_rle_page( image, image_index, layer );
		
		// attach a parasite with aux data per frame
		if ( aux_data_presence )
		{
			gimp_image_attach_new_parasite( layer, STI_AUX_DATA_SUBIMAGE, GIMP_PARASITE_PERSISTENT, sizeof(AuxObjectData), &image->aux_data[image_index] );
		}
		
		// and finally shift layer to its place
		gimp_layer_set_offsets( layer, center_x + image->page[image_index].offset_x, center_y + image->page[image_index].offset_y );
	}

	// add guides to show offset point
	gimp_image_add_hguide( new_image, center_y );
	gimp_image_add_vguide( new_image, center_x );
	
	return( new_image );
}










/*****************************************************************
**
**  STI image creating functions (from GIMP images) and related
**
******************************************************************/

// ============================================
// sti_create_image_from_gimp_image function -
// create sti image in memory from gimp image.
// ============================================
//	in	image_id	: gimp image, needed to convert to sti image
//	out	image		: pointer to image data
//	ret	gboolean	: TRUE, if successful, FALSE, if not

gboolean sti_create_image_from_gimp_image( gint32 image_id, STCIImage *image )
{
	gint		rgb_count, gray_count, indexed_count;
	gboolean	is_8bit_image;
	
	// check for incompatible layers
	if ( does_gimp_image_contain_incompatible_layers(image_id) )
	{
		DEBUG("incompatible layers detected!\n");
		return FALSE;
	}

	// count number of layers of each type	
	count_layer_types_in_gimp_image( image_id, &rgb_count, &gray_count, &indexed_count );
	
	if ( gray_count != 0 )
	{
		DEBUG("Can't save gray layers!\n");
		return FALSE;
	}
	
	if ( indexed_count == 0 && rgb_count > 1 )
	{
		DEBUG("Can't save more than one rgb layers!\n");
		return FALSE;
	}

	if ( indexed_count != 0 && rgb_count != 0 )
	{
		DEBUG("Can't mix rgb and indexed layers!\n");
		return FALSE;
	}
	
	// define sti image type (8 or 16 bit)
	is_8bit_image = indexed_count > 0;
	
	if ( is_8bit_image )
		return sti_create_indexed_image( image_id, image );
	else
		return sti_create_rgb_image( image_id, image );
}


// ============================================
// does_gimp_image_contain_incompatible_layers function -
// checks, that gimp image contains only compatible
// with sti-format layers, e.g. no gray layers, etc...
// ============================================
//	in	image_id	: gimp image, needed to check
//	ret	gint		: TRUE, if incompatible layers present, FALSE, if not

gint does_gimp_image_contain_incompatible_layers( gint32 image_id )
{
	gint		*layers;
	gint		num_layers, i;
	gboolean	incompatible_layers_present = FALSE;

	layers = gimp_image_get_layers( image_id, &num_layers );

	for ( i = 0; (i < num_layers) && !incompatible_layers_present; i++ )
	{
		if ( !gimp_drawable_is_layer( layers[i] ) )
			incompatible_layers_present = TRUE;
	}

	return incompatible_layers_present;
}


// ============================================
// count_layer_types_in_gimp_image function -
// count number of layers of each type in gimp image
// ============================================
//	in	image_id	: gimp image
//	out	rgb			: number of rgb layers
//	out gray		: number of gray layers
//	out indexed		: number of indexed layers

void count_layer_types_in_gimp_image( gint32 image_id, gint *rgb, gint *gray, gint *indexed )
{
	gint		*layers;
	gint		num_layers, i;
	gint		rgb_count, gray_count, indexed_count;

	rgb_count     = 0;
	gray_count    = 0;
	indexed_count = 0;
	
	layers = gimp_image_get_layers( image_id, &num_layers );

	for ( i = 0; i < num_layers; i++ )
	{
		if ( gimp_drawable_is_rgb( layers[i] ) )
			rgb_count++;

		if ( gimp_drawable_is_gray( layers[i] ) )
			gray_count++;

		if ( gimp_drawable_is_indexed( layers[i] ) )
			indexed_count++;
	}

	*rgb     = rgb_count;
	*gray    = gray_count;
	*indexed = indexed_count;
}


// ============================================
// sti_create_rgb_image function -
// create rgb sti image from gimp image
// ============================================
//	in	image_id	: gimp image
//	out image		: rgb sti image
//	ret	gboolean	: TRUE, if successful, FALSE, if not

gboolean sti_create_rgb_image( gint32 image_id, STCIImage *image )
{
	GimpDrawable	*drawable;
	GimpPixelRgn	rgn;
	gint			x, y;
	guchar			*row;
	gint32			color, r, g, b;
	gint			channels, width, height;
	gint			*layer;
	gint			num_layers;

	// clear and fill sti-image structures
	memset( image, 0, sizeof(STCIImage) );
	strncpy( (char*)&(image->header.id), STCI_ID_STRING, STCI_ID_LEN );
	image->header.width  = gimp_image_width (image_id);
	image->header.height = gimp_image_height(image_id);
	image->header.depth  = 16;
	image->header.original_size = image->header.width * image->header.height * image->header.depth / 8;
	image->header.stored_size   = image->header.original_size;
	image->header.flags = STCI_RGB;
	image->header.rgb.mask_red    = MASK_RED;
	image->header.rgb.mask_green  = MASK_GREEN;
	image->header.rgb.mask_blue   = MASK_BLUE;
	image->header.rgb.mask_alpha  = MASK_ALPHA;
	image->header.rgb.depth_red   = DEPTH_RED;
	image->header.rgb.depth_green = DEPTH_GREEN;
	image->header.rgb.depth_blue  = DEPTH_BLUE;
	image->header.rgb.depth_alpha = DEPTH_ALPHA;
	image->rgb_data_size = image->header.original_size;
	image->rgb_data      = (UINT16*)malloc(image->rgb_data_size);
	image->is_8bit_image = FALSE;

	// get parameters of the image to exported
	layer    = gimp_image_get_layers( image_id, &num_layers );
	drawable = gimp_drawable_get( layer[0] );
	channels = gimp_drawable_bpp(drawable->drawable_id);
	width    = image->header.width;
	height   = image->header.height;

	// init pixel region and row of rgb data
	gimp_pixel_rgn_init (&rgn, drawable, 0, 0, width, height, FALSE, FALSE);
	row   = g_new (guchar, width * channels);
	
	// convert 24-bit rgb to 16-bit color
	for ( y = 0; y < height; y++ )
	{
		gimp_pixel_rgn_get_row( &rgn, row, 0, y, width );
		for ( x = 0; x < width; x++ )
		{
			r = (row[ channels * x + 0] << 8) & MASK_RED;
			g = (row[ channels * x + 1] << 3) & MASK_GREEN;
			b = (row[ channels * x + 2] >> 3) & MASK_BLUE;
			color = r | g | b;
			image->rgb_data[ y * width + x ] = (UINT16)color;
		}
	}
	
	g_free( row );

	return TRUE;
}


// ============================================
// sti_create_indexed_image function -
// create 8-bit sti image from gimp image
// ============================================
//	in	image_id	: gimp image
//	out image		: 8-bit sti image
//	ret	gboolean	: TRUE, if successful, FALSE, if not

gboolean sti_create_indexed_image( gint32 image_id, STCIImage *image )
{
	gint			*layer;
	guchar			*image_colormap;
	gint			num_layers, num_colors, cnt;
	UINT32			compressed_page_size, bytes_used, bytes_left;
	gint32			use_center_point, aux_data_presence;
	GimpParasite	*parasite;
	gint			center_x, center_y, offset_x, offset_y;

	DEBUG("sti_create_indexed_image()\n");

	memset( image, 0, sizeof(STCIImage) );

	// check for center-point parasite
	parasite = gimp_image_parasite_find( image_id, STI_USE_CENTER_POINT );
	if ( parasite )
	{
		memcpy( &use_center_point, gimp_parasite_data( parasite ), sizeof(gint32) );
	}
	else
	{
		DEBUG("center-point parasite not found\n");
		use_center_point = 0;
	}

	// check for sti-aux-data parasite
	parasite = gimp_image_parasite_find( image_id, STI_AUX_DATA_FLAG );
	if ( parasite )
	{
		memcpy( &aux_data_presence, gimp_parasite_data( parasite ), sizeof(gint32) );
	}
	else
	{
		DEBUG("sti-aux-data parasite not found\n");
		aux_data_presence = 0;
	}
	
	// get layers and colormap
	layer          = gimp_image_get_layers  ( image_id, &num_layers );
	image_colormap = gimp_image_get_colormap( image_id, &num_colors );

	// fill sti image data
	strncpy( (char*)&(image->header.id), STCI_ID_STRING, STCI_ID_LEN );
	image->header.width  = gimp_image_width (image_id);
	image->header.height = gimp_image_height(image_id);
	image->header.depth  = 8;
	image->header.transparent_value = 0;
	image->header.flags = STCI_INDEXED | STCI_ETRLE_COMPRESSED;
	image->header.indexed.number_of_colours   = 256;
	image->header.indexed.number_of_subimages = num_layers;
	image->header.indexed.depth_red   = 8;
	image->header.indexed.depth_green = 8;
	image->header.indexed.depth_blue  = 8;
	image->is_8bit_image = TRUE;
	image->pages_count = num_layers;
	image->page = (STCISubImage*) malloc( num_layers * sizeof(STCISubImage) );
	if ( aux_data_presence )
	{
		image->header.aux_data_size = num_layers * sizeof(AuxObjectData);
		image->aux_data = (AuxObjectData*) malloc(image->header.aux_data_size);
		memset(image->aux_data, 0, image->header.aux_data_size);
	}
	else
	{
		image->aux_data = NULL;
		image->header.aux_data_size = 0;
	}


	bytes_left = image->header.width * image->header.height * num_layers * 2;
	image->rle_data = (UINT8*) malloc( bytes_left ); // this should be enough

	DEBUG("image w,h   = (%d, %d)\n", image->header.width, image->header.height);
	DEBUG("pages count = %d\n", image->pages_count);
	
	memset( image->page, 0, num_layers * sizeof(STCISubImage) );
	memset( image->rle_data, 0, image->header.width * image->header.height * num_layers * 2 );
	memcpy( image->palette, image_colormap, 3 * num_colors );

	// calculate center point
	if ( use_center_point )
	{
		center_x = image->header.width  / 2;
		center_y = image->header.height / 2;
	}
	else
	{
		center_x = center_y = 0;
	}
	
	bytes_used = 0;
	
	// compress layer after layer
	for ( cnt = 0; cnt < num_layers; cnt++ )
	{
		DEBUG("page #%d\n", cnt);
		DEBUG("  offset = %d\n", bytes_used);
		
		// compress layer
		image->page[cnt].data_offset = bytes_used;
		compressed_page_size = ETRLECompressSubImage( layer[cnt], &image->rle_data[bytes_used], bytes_left, &image->page[cnt] );
		if ( !compressed_page_size )
		{
			DEBUG("Out of space\n");
			return FALSE;
		}
		bytes_used += compressed_page_size;
		bytes_left -= compressed_page_size;
		
		// fill subimage info
		image->page[cnt].data_length = compressed_page_size;
		image->page[cnt].width       = gimp_drawable_width ( layer[cnt] );
		image->page[cnt].height      = gimp_drawable_height( layer[cnt] );

		// define subimage offsets
		if ( use_center_point )
		{
			gimp_drawable_offsets( layer[cnt], &offset_x, &offset_y );
			image->page[cnt].offset_x = offset_x - center_x;
			image->page[cnt].offset_y = offset_y - center_y;
		}
		else
		{		
			image->page[cnt].offset_x = 0;
			image->page[cnt].offset_y = 0;
		}

		// define aux data per frame
		if ( aux_data_presence )
		{
			parasite = gimp_image_parasite_find( layer[cnt], STI_AUX_DATA_SUBIMAGE );
			if ( parasite )
			{
				memcpy( &image->aux_data[cnt], gimp_parasite_data( parasite ), sizeof(AuxObjectData) );
			}
			else
			{
				DEBUG("aux data parasite not fount for layer %d\n", cnt);
			}
		}
		
		DEBUG("  lenght = %d\n", compressed_page_size);
		DEBUG("  offset = (%d, %d)\n", image->page[cnt].offset_x, image->page[cnt].offset_y);
		DEBUG("  sizes  = (%d, %d)\n", image->page[cnt].width, image->page[cnt].height);
		DEBUG("\n");
	}
	
	image->header.original_size = 0;
	image->header.stored_size   = bytes_used;
	image->rle_data_size        = bytes_used;
	DEBUG("total used = %d\n", bytes_used);
	DEBUG("total left = %d\n", bytes_left);

	return TRUE;
}









/*****************************************************************
**
**  RLE compression functions and related
**
******************************************************************/

// ============================================
// ETRLECompressSubImage function -
// takes layer from gimp image and compresses it
// using rle algorythm
// ============================================
//	in	drawable_ID	: layer id
//	out	pDest		: output buffer, where compressed image will be stored
//	in  uiDestLen	: length of output buffer
//	out	pSubImage	: sti subimage info
//	ret	UINT32		: compressed image size in bytes, if compress was successful, 0, if failure

UINT32 ETRLECompressSubImage( gint32 drawable_ID, UINT8 * pDest, UINT32 uiDestLen, STCISubImage * pSubImage )
{
	UINT16		usLoop;
	UINT32		uiScanLineCompressedSize;
	UINT32		uiSpaceLeft = uiDestLen;
	gint		width, height, channels;
	GimpDrawable	*drawable;
	GimpPixelRgn	rgn;
	guchar			*row, *rle;
	gboolean		alpha;
	
	DEBUG("  ETRLECompressSubImage()\n");
	
	// get parameters of the layer
	width    = gimp_drawable_width    ( drawable_ID );
	height   = gimp_drawable_height   ( drawable_ID );
	channels = gimp_drawable_bpp      ( drawable_ID );
	drawable = gimp_drawable_get      ( drawable_ID );
	alpha    = gimp_drawable_has_alpha( drawable_ID );

	DEBUG("    sizes    = (%d, %d)\n", width, height);

	// init pixel region for read, allocate memory for source and compressed rows
	gimp_pixel_rgn_init (&rgn, drawable, 0, 0, width, height, FALSE, FALSE);
	row = g_new (guchar, width * channels);
	rle = g_new (guchar, width * 2); // worst case - double width

	for (usLoop = 0; usLoop < height; usLoop++)
	{
		gimp_pixel_rgn_get_row( &rgn, row, 0, usLoop, width );
		
		// compress row and check return code
		DEBUG("Compressing image %d, scanline %d\n", drawable_ID, usLoop);
		uiScanLineCompressedSize = ETRLECompress( rle, width * 2, row, width * channels, channels );
		if (uiScanLineCompressedSize == 0 )
		{
			g_free(rle);
			g_free(row);
			DEBUG("ETRLECompressSubImage: out of buffer!\n");
			// there wasn't enough room to complete the compression!
			return( 0 );
		}
		
		// copy rle data to destination
		memcpy( pDest, rle, uiScanLineCompressedSize );
		
		// reduce the amount of available space
		uiSpaceLeft -= uiScanLineCompressedSize;
		pDest += uiScanLineCompressedSize;
	}
	
	g_free(rle);
	g_free(row);
	
	return( uiDestLen - uiSpaceLeft );
}


// ============================================
// ETRLECompress function -
// takes row of 8-bit graphical data and compresses
// it using rle algorythm
// ============================================
//	out	pDest		: output buffer, where compressed image will be stored
//	in  uiDestLen	: length of output buffer
//	in	pSource		: source graphic data
//	in	uiSourceLen	: size of source data in bytes
//	in	channels	: bytes per pixel for source data
//	ret	UINT32		: compressed image size in bytes, if compress was successful, 0, if failure

UINT32 ETRLECompress( guchar * pDest, UINT32 uiDestLen, guchar * pSource, UINT32 uiSourceLen, gint channels )
{
	// Compress a buffer (a scanline) into ETRLE format, which is a series of runs.
	// Each run starts with a byte whose high bit is 1 if the run is compressed, 0 otherwise.
	// The lower seven bits of that byte indicate the length of the run
	
	// ETRLECompress returns the number of bytes used by the compressed buffer, or 0 if an error
	// occurred

	// uiSourceLoc keeps track of our current position in the source
	UINT32	uiSourceLoc = 0;
	// uiCurrentSourceLoc is used to look ahead in the source to determine the length of runs
	UINT32	uiCurrentSourceLoc = 0;
	UINT32	uiDestLoc = 0;
	UINT8	ubLength = 0;
	
	while (uiSourceLoc < uiSourceLen && uiDestLoc < uiDestLen)
	{
		uiCurrentSourceLoc = uiSourceLoc;
		ubLength = 0;
		if ( is_transparent_pixel( &pSource[uiSourceLoc], channels) )
		{
			// transparent run - determine its length
			do
			{		
				uiCurrentSourceLoc += channels;
				ubLength++;
			}
			while ((uiCurrentSourceLoc < uiSourceLen)
				&& is_transparent_pixel( &pSource[uiCurrentSourceLoc], channels)
				&& (ubLength < COMPRESS_RUN_LIMIT));
			
			// output run-byte
			pDest[uiDestLoc] = ubLength | COMPRESS_TRANSPARENT;
				
			// update location
			uiSourceLoc += ubLength * channels;
			uiDestLoc += 1;
		}
		else
		{
			// non-transparent run - determine its length
			do
			{
				uiCurrentSourceLoc += channels;
				ubLength++;
			}
			while ((uiCurrentSourceLoc < uiSourceLen)
				&& !is_transparent_pixel( &pSource[uiCurrentSourceLoc], channels)
				&& (ubLength < COMPRESS_RUN_LIMIT));
			
			if (uiDestLoc + ubLength < uiDestLen)
			{
				// output run-byte
				pDest[uiDestLoc++] = ubLength | COMPRESS_NON_TRANSPARENT;

				// output run (and update location)
				while (ubLength > 0)
				{
					pDest[uiDestLoc] = pSource[uiSourceLoc];
					uiDestLoc++;
					uiSourceLoc += channels;
					ubLength--;
				}
			}
			else
			{
				// not enough room in dest buffer to copy the run!
				return( 0 );
			}			
		}
	}
	if (uiDestLoc >= uiDestLen)
	{
		return( 0 );
	}
	else 
	{
		// end with a run of 0 length (which might as well be non-transparent,
		// giving a 0-byte
		pDest[uiDestLoc++] = 0;
		return( uiDestLoc );
	}
}


// ============================================
// is_transparent_pixel function -
// tests pixel for transparency
// ============================================
//	in	pixel		: test pixel
//	in	channels	: bytes per pixel for source data
//	ret	gboolean	: TRUE, if pixel is transparent, FALSE, if not

gboolean is_transparent_pixel( guchar *pixel, gint channels )
{
	// alpha has range [0, 255].
	// 255 is visible pixel
	// 0 is completenly transparent
	if ( channels == 2 && pixel[1] != 0xFF )
		return TRUE;
	
	if ( channels == 1 && pixel[0] == TCI )
		return TRUE;
	
	return FALSE;
}
