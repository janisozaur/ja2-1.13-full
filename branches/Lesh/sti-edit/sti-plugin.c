#include <libgimp/gimp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sti-format.h"

// *************************************************************
// FUNCTION DEFINITIONS
// *************************************************************

static void query (void);
static void run   (const gchar      *name,
                   gint              nparams,
                   const GimpParam  *param,
                   gint             *nreturn_vals,
                   GimpParam       **return_vals);

static gint32 load_image( const char *filename );
static gint32 save_image( const char *filename, gint32 gimp_image );

// *************************************************************
// TYPE DEFINITIONS
// *************************************************************

GimpPlugInInfo PLUG_IN_INFO =
{
    NULL,
    NULL,
    query,
    run
};

// *************************************************************
// PREDEFINED VALUES DEFINITIONS
// *************************************************************

#define STI_PLUGIN_VERSION		"v0.9 beta-1"
#define	STI_LOAD_PROCEDURE_NAME	"file-sti-load"
#define	STI_SAVE_PROCEDURE_NAME	"file-sti-save"

MAIN()





// ============================================
// GIMP 'query' function
// ============================================

static void query (void)
{
	// Register load handler
	// ---------------------
    static GimpParamDef load_vals[] =
    {
        { GIMP_PDB_INT32,   "run-mode", 	"Run mode" },
        { GIMP_PDB_STRING,  "filename", 	"The name of the file to load"},
        { GIMP_PDB_STRING,  "raw-filename",	"The name of the file to load"}
    };

	static GimpParamDef load_return_vals[] = {
		{ GIMP_PDB_IMAGE,   "image",        "Output image" }
	};
	
    gimp_install_procedure (
        STI_LOAD_PROCEDURE_NAME,
        "STI loader " STI_PLUGIN_VERSION,
        "Loads STI graphics, used in Jagged Alliance 2 game",
        "Lesh",
        "Copyright",
        "2007-2008",
        "<Load>/Jagged Alliance 2 STI graphics",
        NULL,
        GIMP_PLUGIN,
        G_N_ELEMENTS (load_vals), G_N_ELEMENTS (load_return_vals),
        load_vals, load_return_vals);
        
    gimp_register_load_handler(STI_LOAD_PROCEDURE_NAME, "sti", "");




	// Register save handler
	// ---------------------
    static GimpParamDef save_vals[] =
    {
        { GIMP_PDB_INT32,   "run-mode", 	"Run mode" },
	    { GIMP_PDB_IMAGE,	"image",		"Input image" },
	    { GIMP_PDB_DRAWABLE,"drawable",		"Drawable to save" },
        { GIMP_PDB_STRING,  "filename", 	"The name of the file to save"},
        { GIMP_PDB_STRING,  "raw-filename",	"The name of the file to save"}
    };

    gimp_install_procedure (
        STI_SAVE_PROCEDURE_NAME,
        "STI saver " STI_PLUGIN_VERSION,
        "Saves STI graphics, used in Jagged Alliance 2 game",
        "Lesh",
        "Copyright",
        "2007-2008",
        "<Save>/Jagged Alliance 2 STI graphics",
        "RGB,INDEXED",
        GIMP_PLUGIN,
        G_N_ELEMENTS (save_vals), 0,
        save_vals, NULL);
        
    gimp_register_save_handler(STI_SAVE_PROCEDURE_NAME, "sti", "");
}



// ============================================
// GIMP 'run' function
// ============================================

static void
run (const gchar      *name,
     gint              nparams,
     const GimpParam  *param,
     gint             *nreturn_vals,
     GimpParam       **return_vals)
{
    static GimpParam  	values[2];
    GimpRunMode       	run_mode;
	gint32				imageID;
	int					use_loader, use_saver;
	
    /* Setting mandatory output values */
    *nreturn_vals = 2;
    *return_vals  = values;

    values[0].type 			= GIMP_PDB_STATUS;
    values[0].data.d_status = GIMP_PDB_SUCCESS;
    values[1].type          = GIMP_PDB_IMAGE;
    values[1].data.d_image  = -1;

    /* Getting run_mode - we won't display a dialog if 
     * we are in NONINTERACTIVE mode */
    run_mode = param[0].data.d_int32;
    
    use_loader = 0;
    use_saver  = 0;
    
    /* What we are doing ? */
    if ( strcmp(name, STI_LOAD_PROCEDURE_NAME) == 0 )
    	use_loader = 1;
    
    if ( strcmp(name, STI_SAVE_PROCEDURE_NAME) == 0 )
    	use_saver = 1;

	// loader section
	if ( use_loader )
	{
	    imageID = load_image( param[1].data.d_string );
	    if ( imageID == -1 )
	    	values[0].data.d_status = GIMP_PDB_CALLING_ERROR;
		else
	    	values[1].data.d_image  = imageID;
	}

	// saver section
	if ( use_saver )
	{
	    if ( save_image( param[3].data.d_string, param[1].data.d_int32 ) == -1 )
	    	values[0].data.d_status = GIMP_PDB_CALLING_ERROR;
	}
}



// ============================================
// load_image function
// ============================================
//	in	filename	: file to read
//	ret	gint32		: index of image, that was read

static gint32 load_image( const char *filename )
{
	STCIImage	image;
	gint32		new_image;

	if ( !sti_load_image( filename, &image ) )
	{
		DEBUG("failed to load file\n");
		return( -1 );
	}

	if ( image.is_8bit_image )
	{
		DEBUG("Image %s is 8 bit rle compressed\n", filename);
		DEBUG("pages count is %d\n", image.pages_count);
		DEBUG("rle data size is %d\n", image.rle_data_size);
		DEBUG("aux data %s\n", (image.aux_data) ? ("yes"):("no") );

		new_image = create_gimp_image_indexed( &image );
		if ( new_image == -1 )
		{
			DEBUG("can't allocate new image\n");
			sti_free_image( &image );
			return( -1 );
		}
	}
	else
	{
		DEBUG("Image %s is 16 bit rgb\n", filename);
		DEBUG("width, height, depth: %d, %d, %d\n", image.header.width, image.header.height, image.header.depth );
		DEBUG("rgb data size is %d\n", image.rgb_data_size);
		DEBUG("mask red   : %08X\n", image.header.rgb.mask_red);
		DEBUG("mask green : %08X\n", image.header.rgb.mask_green);
		DEBUG("mask blue  : %08X\n", image.header.rgb.mask_blue);
		DEBUG("mask alpha : %08X\n", image.header.rgb.mask_alpha);
		DEBUG("depth red  : %d\n", image.header.rgb.depth_red);
		DEBUG("depth green: %d\n", image.header.rgb.depth_green);
		DEBUG("depth blue : %d\n", image.header.rgb.depth_blue);
		DEBUG("depth alpha: %d\n", image.header.rgb.depth_alpha);

		new_image = create_gimp_image_rgb( &image );
		if ( new_image == -1 )
		{
			DEBUG("can't allocate new image\n");
			sti_free_image( &image );
			return( -1 );
		}
	};

	gimp_image_set_filename(new_image, filename);
	sti_free_image( &image );
	
	return( new_image );
}



// ============================================
// save_image function
// ============================================
//	in	filename	: file to write
//	in	image		: pointer to image data to save
//	ret	gint32		: non-zero if error, zero if successful

static gint32 save_image( const char *filename, gint32 gimp_image )
{
	STCIImage	image;
	gint		success;

	if ( !sti_create_image_from_gimp_image( gimp_image, &image ) )
		return -1;

	success = sti_save_image( filename, &image );
	sti_free_image( &image );

	if ( success )
		return 0;
	else
		return -1;
}
