#include "respak.hpp"
#include "io_layer.h"

#define	FILENAME_SIZE			256

#define FILE_OK					0


typedef struct 
{
	CHAR8		sLibName[ FILENAME_SIZE ];
	CHAR8		sPathToLibrary[ FILENAME_SIZE ];
	INT32		iEntries;
	INT32		iUsed;
	UINT16		iSort;
	UINT16		iVersion;
	BOOLEAN		fContainsSubDirectories;
	INT32		iReserved;
} LIBHEADER;


typedef struct 
{
	CHAR8			sFileName[ FILENAME_SIZE ];
	UINT32			uiOffset;
	UINT32			uiLength;
	UINT8			ubState;
	UINT8			ubReserved;
	SGP_FILETIME	sFileTime;
	UINT16			usReserved2;
} DIRENTRY;



// Abstract resource pak class goes first
// ==============================================================




//*****************************************************************************
// Default constructor
//*****************************************************************************
sgpResourcePak::sgpResourcePak() :
	pak_path(""),
	is_opened( FALSE )
{
}

//*****************************************************************************
// Copy constructor
//*****************************************************************************
sgpResourcePak::sgpResourcePak( const sgpResourcePak& obj ) :
	pak_path( obj.pak_path ),
	is_opened( obj.is_opened )
{
}

//*****************************************************************************
// Destructor
//*****************************************************************************
sgpResourcePak::~sgpResourcePak()
{
}

//*****************************************************************************
// open_pak - wrapper for open pak-file procedure
//	in	path - path and filename of pak-file
//	ret - TRUE, if open is successful, FALSE, if not
//*****************************************************************************
BOOLEAN	sgpResourcePak::open_pak( sgpString path )
{
	if ( is_opened )
		return FALSE;
		
	pak_path  = path;
	is_opened = load_pak();
	return( is_opened );
}

//*****************************************************************************
// close_pak - wrapper for close pak-file procedure
//	ret - TRUE, if close is successful, FALSE, if not
//*****************************************************************************
BOOLEAN sgpResourcePak::close_pak( void )
{
	if ( !is_opened )
		return FALSE;
	
	unload_pak();
	pak_path = "";
	is_opened = FALSE;
	return( TRUE );
}




// SLF resource pak class goes below
// ==============================================================





//*****************************************************************************
// Default constructor
//*****************************************************************************
sgpSLFResourcePak::sgpSLFResourcePak() :
	pak_handler( -1 )
{
	catalog.clear();
	opened_file.clear();
}

//*****************************************************************************
// Copy constructor
//*****************************************************************************
sgpSLFResourcePak::sgpSLFResourcePak( const sgpSLFResourcePak& obj ) :
	sgpResourcePak( obj ),
	catalog( obj.catalog ),
	opened_file( obj.opened_file ),
	pak_handler( obj.pak_handler )
{
}

//*****************************************************************************
// Destructor
//*****************************************************************************
sgpSLFResourcePak::~sgpSLFResourcePak()
{
	close_pak();
}

//*****************************************************************************
// print_pak - method for debugging
//*****************************************************************************
void sgpSLFResourcePak::print_pak( void )
{
	INT32	cnt;
	
	printf("pak = %s\n", pak_path.c_str() );
	
	for ( cnt = 0; cnt < catalog.size(); cnt++ )
	{
		printf("[%d] = %s, ( offset %, size %d )\n", cnt, catalog[cnt].file_name.c_str(),
			catalog[cnt].file_offset, catalog[cnt].file_length);

	}
}

//*****************************************************************************
// load_pak - method, that will load and parse header and list of files in
// SLF container.
//	ret	(TRUE) - if successful, (FALSE) - error
//*****************************************************************************
BOOLEAN	sgpSLFResourcePak::load_pak( void )
{
	DIRENTRY 	DirEntry;
	LIBHEADER	LibFileHeader;
	UINT32		uiNumBytesRead;
	INT32		iLoop;
	slfEntry	slfResource;

	// check for existance	
	if ( !IO_IsRegularFile( pak_path.c_str() ) )
	{
		fprintf(stderr, "Specified pak is not exist (%s)\n", pak_path.c_str());
		return FALSE;
	}

	// try to open file
	pak_handler = IO_File_Open( pak_path.c_str(), IO_ACCESS_READ );
	if ( pak_handler == -1 )
	{
		fprintf(stderr, "Error opening library %s: errno=%d\n", pak_path.c_str(), errno);
		return( FALSE );
	}
	
	// read header
	uiNumBytesRead = IO_File_Read( pak_handler, &LibFileHeader, sizeof( LIBHEADER ) );
	if( uiNumBytesRead != sizeof( LIBHEADER ) )
	{
		fprintf(stderr, "Error reading library header %d: errno=%d\n",
			pak_handler, errno);
		//Error Reading the file database header.
		return( FALSE );
	}

	// add directory contained in slf
	slfResource.file_name   = sgpString(LibFileHeader.sPathToLibrary);
	slfResource.file_length = 0;
	slfResource.file_offset = 0;
	catalog.push_back( slfResource );

	// seek the table of contents
	if ( !IO_File_Seek( pak_handler, -( LibFileHeader.iEntries * (INT32)sizeof(DIRENTRY) ), IO_SEEK_FROM_END ) )
	{
		fprintf(stderr, "Error seeking library %d: errno=%d\n",
			pak_handler, errno);
		return(FALSE);
	}

	//loop through the library and determine the number of files that are FILE_OK
	//ie.  so we dont load the old or deleted files
	for( iLoop=0; iLoop<LibFileHeader.iEntries; iLoop++ )
	{
		uiNumBytesRead = IO_File_Read( pak_handler, &DirEntry, sizeof( DIRENTRY ) );
		if( uiNumBytesRead != sizeof( DIRENTRY ) )
		{
			fprintf(stderr, "Error reading library entry %d: errno=%d\n",
				pak_handler, errno);
			//Error Reading the file database header.
			return( FALSE );
		}

		if( DirEntry.ubState == FILE_OK )
		{
			slfResource.file_name   = sgpString(LibFileHeader.sPathToLibrary) + DirEntry.sFileName;
			slfResource.file_length = DirEntry.uiLength;
			slfResource.file_offset = DirEntry.uiOffset;
			catalog.push_back( slfResource );
		}
	}

	opened_file.clear();
	
	return TRUE;
}

//*****************************************************************************
// unload_pak - method, that will finish work with SLF container
//*****************************************************************************
void sgpSLFResourcePak::unload_pak( void )
{
	IO_File_Close( pak_handler );
	catalog.clear();
	opened_file.clear();
}

//*****************************************************************************
// get_resource_names - transfer list of resources outside
//	out	contents - string array on resource names
//*****************************************************************************
void sgpSLFResourcePak::get_resource_names( sgpStringArray& contents )
{
	INT32	cnt;

	contents.clear();	
	for ( cnt = 0; cnt < catalog.size(); cnt++ )
	{
		 contents.push_back( catalog[cnt].file_name );
	}
}

//*****************************************************************************
// file_open - open file with name from SLF
//	in	file_index - file index in container
//	ret	NO_PAK_FILE - if error, file handler, if successful
//*****************************************************************************
pakFile	sgpSLFResourcePak::file_open ( INT32 file_index )
{
	INT32	slot_number;
	slfOpenedFileEntry	slot;
	
	if ( !is_opened )
		return NO_PAK_FILE;

	// 1. get index from name
	// not needed already
	
	// 2. check, if it is already opened
	if ( is_already_opened( file_index ) )
		return NO_PAK_FILE;
	
	// 3. open
	slot.file_id  = file_index;
	slot.file_pos = 0;
	slot_number = get_free_opened_slot();
	if ( slot_number == -1 )
	{
		// add new slot
		opened_file.push_back( slot );
		slot_number = opened_file.size() - 1;
	}
	else
	{
		// replace existing slot
		opened_file[ slot_number ] = slot;
	}
	
	return slot_number;
}

//*****************************************************************************
// file_close - close file in SLF container
//	in	file - file handler
//	ret	FALSE - if error, TRUE, if successful
//*****************************************************************************
BOOLEAN	sgpSLFResourcePak::file_close( pakFile file )
{
	if ( !is_opened )
		return FALSE;
	
	if ( file < 0 || file >= opened_file.size() )
		return FALSE;
	
	opened_file[ file ].file_id  = FREE_FILE_ID;
	opened_file[ file ].file_pos = 0;
	
	return TRUE;
}

//*****************************************************************************
// file_read - read from file in SLF container
//	in	file - file handler
//	out	data - pointer to buffer
//	in	bytes_to_read - number of bytes to read
//	ret	(-1) - if error, (0), if end of file, (>0) - if read is successful
//*****************************************************************************
INT32 sgpSLFResourcePak::file_read ( pakFile file, PTR data, INT32 bytes_to_read )
{
	INT32	file_index;
	INT32	cur_pos;
	INT32	has_been_read;
	INT32	file_length;
	INT32	file_offset;
	
	if ( !is_opened )
		return -1;
	
	if ( file < 0 || file >= opened_file.size() )
		return -1;
	
	file_index  = opened_file[ file ].file_id;
	cur_pos     = opened_file[ file ].file_pos;
	file_length = catalog[ file_index ].file_length;
	file_offset = catalog[ file_index ].file_offset;

	//if we are trying to read more data then the size of the file, correct read size
	if( bytes_to_read + cur_pos > file_length + file_offset )
	{
		bytes_to_read = file_length + file_offset - cur_pos;
	}

	// seek file position within pak-file
	if ( !IO_File_Seek( pak_handler, file_offset + cur_pos, IO_SEEK_FROM_START ) )
	{
		fprintf(stderr, "Error seeking library (load) %d: errno=%d\n",
			pak_handler, errno);
		return(-1);
	}

	// read data
	has_been_read = IO_File_Read( pak_handler, data, bytes_to_read );
	if( bytes_to_read != has_been_read )
	{
		fprintf(stderr, "Error reading library entry %d: errno=%d\n",
			pak_handler, errno);
	}

	// advance file position
	opened_file[ file ].file_pos += has_been_read;
	return has_been_read;
}

//*****************************************************************************
// file_seek - seek in file in SLF container
//	in	file - file handler
//	in	distance - offset in bytes
//	in	seek_mode - mode of seek. Can be:
//			IO_SEEK_FROM_START	 - offset from start of file 
//			IO_SEEK_FROM_END	 - offset from end of file
//			IO_SEEK_FROM_CURRENT - offset from current position in file
//	ret	FALSE - if error, TRUE, if successful
//*****************************************************************************
BOOLEAN	sgpSLFResourcePak::file_seek ( pakFile file, INT32 distance, UINT8 seek_mode )
{
	if ( !is_opened )
		return FALSE;
	
	if ( file < 0 || file >= opened_file.size() )
		return FALSE;

	if ( seek_mode == IO_SEEK_FROM_START )
		opened_file[ file ].file_pos = distance;
	else if ( seek_mode == IO_SEEK_FROM_END )
		opened_file[ file ].file_pos = catalog[ opened_file[ file ].file_id ].file_length - distance;
	else if ( seek_mode == IO_SEEK_FROM_CURRENT )
		opened_file[ file ].file_pos += distance;
	else
		return(FALSE);

	return TRUE;
}

//*****************************************************************************
// file_size - get current position of file in SLF container
//	in	file - file handler
//	ret	(-1) - if error, (>=0) - if successful
//*****************************************************************************
INT32 sgpSLFResourcePak::file_pos  ( pakFile file )
{
	if ( !is_opened )
		return -1;
	
	if ( file < 0 || file >= opened_file.size() )
		return -1;

	return( opened_file[ file ].file_pos );
}

//*****************************************************************************
// file_size_by_handler - get size of file in SLF container using file handler
//	in	file - file handler
//	ret	(-1) - if error, (>=0) - if successful
//*****************************************************************************
INT32 sgpSLFResourcePak::file_size_by_handler( pakFile file )
{
	if ( !is_opened )
		return -1;
	
	if ( file < 0 || file >= opened_file.size() )
		return -1;
	
	return( catalog[ opened_file[ file ].file_id ].file_length );
}

//*****************************************************************************
// file_size_by_index - get size of file in SLF container using file index
//	in	index - file index
//	ret	(-1) - if error, (>=0) - if successful
//*****************************************************************************
INT32 sgpSLFResourcePak::file_size_by_index  ( INT32 index  )
{
	if ( !is_opened )
		return -1;
	
	if ( index < 0 || index >= catalog.size() )
		return -1;

	return( catalog[ index ].file_length );
}

//*****************************************************************************
// get_index_from_name - gives index of the file name in catalog
//	in	name - file name
//	ret	(-1) - if name not found, (>=0) - if successful
//*****************************************************************************
INT32 sgpSLFResourcePak::get_index_from_name( const sgpString& name )
{
	INT32	cnt;
	
	for ( cnt = 0; cnt < catalog.size(); cnt++ )
	{
		if ( catalog[cnt].file_name == name )
			return cnt;
	}
	
	return -1;	// failed to find
}

//*****************************************************************************
// is_already_opened - check, if file with index is already opened
//	in	index - file index in catalog
//	ret	(FALSE) - if file is not opened, (TRUE) - if opened currently
//*****************************************************************************
BOOLEAN sgpSLFResourcePak::is_already_opened( UINT32 index )
{
	INT32	cnt;
	
	for ( cnt = 0; cnt < opened_file.size(); cnt++ )
	{
		if ( opened_file[cnt].file_id == index )
			return TRUE;
	}
	
	return FALSE;	// failed to find
}

//*****************************************************************************
// get_free_opened_slot - gives index of the first free slot in opened files array
//	ret	(-1) - if no free slots left, (>=0) - if successful
//*****************************************************************************
INT32 sgpSLFResourcePak::get_free_opened_slot( void )
{
	INT32	cnt;
	
	for ( cnt = 0; cnt < opened_file.size(); cnt++ )
	{
		if ( opened_file[cnt].file_id == FREE_FILE_ID )
			return cnt;
	}
	
	return -1;	// failed to find
}








// Directory resource pak class goes below
// ==============================================================








//*****************************************************************************
// Default constructor
//*****************************************************************************
sgpDirResourcePak::sgpDirResourcePak()
{
	catalog.clear();
	opened_file.clear();
}

//*****************************************************************************
// Copy constructor
//*****************************************************************************
sgpDirResourcePak::sgpDirResourcePak( const sgpDirResourcePak& obj ) :
	sgpResourcePak( obj ),
	catalog( obj.catalog ),
	opened_file( obj.opened_file )
{
}

//*****************************************************************************
// Destructor
//*****************************************************************************
sgpDirResourcePak::~sgpDirResourcePak()
{
	close_pak();
}

//*****************************************************************************
// print_pak - method for debugging
//*****************************************************************************
void sgpDirResourcePak::print_pak( void )
{
	INT32	cnt;
	
	printf("pak = %s\n", pak_path.c_str() );
	
	for ( cnt = 0; cnt < catalog.size(); cnt++ )
	{
		printf("[%d] = %s\n", cnt, catalog[cnt].c_str() );
	
	}
}

//*****************************************************************************
// load_pak - method, that will gather list of files in directory container.
//	ret	(TRUE) - if successful, (FALSE) - error
//*****************************************************************************
BOOLEAN	sgpDirResourcePak::load_pak( void )
{
	STRING512		zCurDirSave;
	sgpStringArray	dirToLook, foundFiles;
	sgpString		dirToBeLooked;
	sgpString		OptionalDirectoryName, RootDirectory;
	sgpStringArrayIterator	foundFilesIterator;
	BOOLEAN			fIsDirectory;

	// check for existance	
	if ( !IO_IsDirectory( pak_path.c_str() ) )
	{
		fprintf(stderr, "Specified pak is not exist (%s)\n", pak_path.c_str());
		return FALSE;
	}

	// add slash at end, if it absent
	if ( pak_path[ pak_path.length() - 1 ] != SLASH )
		pak_path += SLASH;

	IO_Dir_GetCurrentDirectory( zCurDirSave, 512 );
	IO_Dir_SetCurrentDirectory( pak_path.c_str() );
	dirToLook.push_back("");

	while ( !dirToLook.empty() )
	{
		get_directory_listing( dirToLook.back(), foundFiles );
		dirToLook.pop_back();

		for ( foundFilesIterator = foundFiles.begin(); foundFilesIterator != foundFiles.end(); foundFilesIterator++ )
		{
			fIsDirectory = IO_IsDirectory( foundFilesIterator->c_str() );
			if ( fIsDirectory )
				dirToLook.push_back( *foundFilesIterator );

			catalog.push_back( *foundFilesIterator );
		}
	}

	IO_Dir_SetCurrentDirectory( zCurDirSave );

	opened_file.clear();
	
	return TRUE;
}

//*****************************************************************************
// unload_pak - method, that will finish work with directory container
//*****************************************************************************
void sgpDirResourcePak::unload_pak( void )
{
	catalog.clear();
	opened_file.clear();
}

//*****************************************************************************
// get_resource_names - transfer list of resources outside
//	out	contents - string array on resource names
//*****************************************************************************
void sgpDirResourcePak::get_resource_names( sgpStringArray& contents )
{
	INT32	cnt;

	contents.clear();	
	for ( cnt = 0; cnt < catalog.size(); cnt++ )
	{
		 contents.push_back( catalog[cnt] );
	}
}

//*****************************************************************************
// file_open - open file with name from directory
//	in	name - file name
//	ret	NO_PAK_FILE - if error, file handler, if successful
//*****************************************************************************
pakFile	sgpDirResourcePak::file_open ( INT32 file_index )
{
	INT32	slot_number;
	IOFILE	handler;
	sgpString	real_name;
	
	if ( !is_opened )
		return NO_PAK_FILE;

	// 1. get index from name
	// not needed already
	
	// 2. check, if it is already opened
	if ( is_already_opened( file_index ) )
		return NO_PAK_FILE;
	
	// 3. open
	real_name = pak_path + catalog[ file_index ];
	handler = IO_File_Open( real_name.c_str(), IO_ACCESS_READ );
	if ( handler == -1 )
	{
		fprintf(stderr, "Error opening file %s: errno=%d\n", real_name.c_str(), errno);
		return( NO_PAK_FILE );
	}

	slot_number = get_free_opened_slot();
	if ( slot_number == -1 )
	{
		// add new slot
		opened_file.push_back( handler );
		slot_number = opened_file.size() - 1;
	}
	else
	{
		// replace existing slot
		opened_file[ slot_number ] = handler;
	}

	return slot_number;
}

//*****************************************************************************
// file_close - close file in directory container
//	in	file - file handler
//	ret	FALSE - if error, TRUE, if successful
//*****************************************************************************
BOOLEAN	sgpDirResourcePak::file_close( pakFile file )
{
	if ( !is_opened )
		return FALSE;
	
	if ( file < 0 || file >= opened_file.size() )
		return FALSE;
	
	IO_File_Close( opened_file[ file ] );
	opened_file[ file ] = FREE_FILE_ID;

	return TRUE;
}

//*****************************************************************************
// file_read - read from file in directory container
//	in	file - file handler
//	out	data - pointer to buffer
//	in	bytes_to_read - number of bytes to read
//	ret	(-1) - if error, (0), if end of file, (>0) - if read is successful
//*****************************************************************************
INT32 sgpDirResourcePak::file_read ( pakFile file, PTR data, INT32 bytes_to_read )
{
	INT32	file_index;
	INT32	cur_pos;
	INT32	has_been_read;
	
	if ( !is_opened )
		return -1;
	
	if ( file < 0 || file >= opened_file.size() )
		return -1;
	
	has_been_read = IO_File_Read( opened_file[ file ], data, bytes_to_read );
	
	return has_been_read;
}

//*****************************************************************************
// file_seek - seek in file in directory container
//	in	file - file handler
//	in	distance - offset in bytes
//	in	seek_mode - mode of seek. Can be:
//			IO_SEEK_FROM_START	 - offset from start of file 
//			IO_SEEK_FROM_END	 - offset from end of file
//			IO_SEEK_FROM_CURRENT - offset from current position in file
//	ret	FALSE - if error, TRUE, if successful
//*****************************************************************************
BOOLEAN	sgpDirResourcePak::file_seek ( pakFile file, INT32 distance, UINT8 seek_mode )
{
	if ( !is_opened )
		return FALSE;
	
	if ( file < 0 || file >= opened_file.size() )
		return FALSE;

	return IO_File_Seek( opened_file[ file ], distance, seek_mode );
}

//*****************************************************************************
// file_size - get current position of file in directory container
//	in	file - file handler
//	ret	(-1) - if error, (>=0) - if successful
//*****************************************************************************
INT32 sgpDirResourcePak::file_pos  ( pakFile file )
{
	if ( !is_opened )
		return -1;
	
	if ( file < 0 || file >= opened_file.size() )
		return -1;

	return( IO_File_GetPosition( opened_file[ file ] ) );
}

//*****************************************************************************
// file_size - get size of file in directory container
//	in	file - file handler
//	ret	(-1) - if error, (>=0) - if successful
//*****************************************************************************
INT32 sgpDirResourcePak::file_size_by_handler ( pakFile file )
{
	if ( !is_opened )
		return -1;
	
	if ( file < 0 || file >= opened_file.size() )
		return -1;

	return( IO_File_GetSize(opened_file[ file ]) );
}

//*****************************************************************************
// file_size_by_index - get size of file in dir container using file index
//	in	index - file index
//	ret	(-1) - if error, (>=0) - if successful
//*****************************************************************************
INT32 sgpDirResourcePak::file_size_by_index  ( INT32 index  )
{
	if ( !is_opened )
		return -1;
	
	if ( index < 0 || index >= catalog.size() )
		return -1;

	return( IO_File_GetSize( catalog[index].c_str() ) );
}

//*****************************************************************************
// get_index_from_name - gives index of the file name in catalog
//	in	name - file name
//	ret	(-1) - if name not found, (>=0) - if successful
//*****************************************************************************
INT32 sgpDirResourcePak::get_index_from_name( const sgpString& name )
{
	INT32	cnt;
	
	for ( cnt = 0; cnt < catalog.size(); cnt++ )
	{
		if ( catalog[cnt] == name )
			return cnt;
	}
	
	return -1;	// failed to find
}

//*****************************************************************************
// is_already_opened - check, if file with index is already opened
//	in	index - file index in catalog
//	ret	(FALSE) - if file is not opened, (TRUE) - if opened currently
//*****************************************************************************
BOOLEAN sgpDirResourcePak::is_already_opened( UINT32 index )
{
	INT32	cnt;
	
	for ( cnt = 0; cnt < opened_file.size(); cnt++ )
	{
		if ( opened_file[cnt] == index )
			return TRUE;
	}
	
	return FALSE;	// failed to find
}

//*****************************************************************************
// get_free_opened_slot - gives index of the first free slot in opened files array
//	ret	(-1) - if no free slots left, (>=0) - if successful
//*****************************************************************************
INT32 sgpDirResourcePak::get_free_opened_slot( void )
{
	INT32	cnt;
	
	for ( cnt = 0; cnt < opened_file.size(); cnt++ )
	{
		if ( opened_file[cnt] == FREE_FILE_ID )
			return cnt;
	}
	
	return -1;	// failed to find
}

//*****************************************************************************
//	get_directory_listing - collect directory listing to array of strings
//	in	DirToLook: root directory, where recursive scan for entries begins
//	out	FileList: array of resulting entries
//	return: always TRUE
//*****************************************************************************
BOOLEAN	sgpDirResourcePak::get_directory_listing( const sgpString& DirToLook, sgpStringArray& FileList )
{
	STRING512	entry;
	sgpString	dirPattern = DirToLook + "*";

	FileList.clear();

	if ( IO_File_GetFirst( dirPattern.c_str(), entry, sizeof(entry) ) )
	{
		do
		{
			// add filename in file list
			// additionally, in case of directory, add an ending slash to it
			if ( !IO_IsHiddenEntry( entry ) )
			{
				FileList.push_back( entry );
				if ( IO_IsDirectory( entry ) )
					FileList.back() += "/";
			}
				
		} while ( IO_File_GetNext( entry, sizeof(entry) ) );
		IO_File_GetClose();
	}

	return TRUE;
}
