// Author: Lesh		May 2007

//===================================================================
//	vfs.cpp - virtual file system
//
//		VFS provides unified access to file resources for the
//	application. VFS hides real file system, where application
//	is running, and gives to application freedom from particular
//	file system.
//		VFS also has such features, like:
//	- attach a container file (file with application resources,
//	  e.g. ".slf" or ".zip");
//	- attach a directory on a file system as an application
//	  resources;
//	- replace/add some resources, using directories or containers
//	  multiple times (!);
//	  
//===================================================================

//===================================================================
//
//	Little bit of theory, how VFS is working:
//
//	Roughly speaking, VFS is a file system in memory, an abstraction
//	layer over real file system and container files (*.slf). VFS is
//	represented by data structure, called map. The map has array of
//	objects with two entities inside: <Key, Association>.
//		<Key1, Association1>
//		<Key2, Association2>
//		.....
//
//	Note, that "Key" entity is unique in map.
//
//	So far, I'll be using "Key" entity for application resource name
//	(App-name). App-name is the file name of some application (read,
//	game) resource. Ex.: "ANIMS\ANIMALS\CT_HIT.STI" or
//	"AMBIENT\BIRD9.WAV".
//
//	On the other hand, "Association" entity is the real file name on
//	file system - Real-name. Thus we can have such mappings:
//
//		App-name				 ||| Real-name
//		--------------------------------------------------------
//		AMBIENT\BIRD9.WAV        --> /home/lesh/ja2_113/Data-1.13/Ambient/Bird9.wav
//		ANIMS\ANIMALS\CT_HIT.STI --> Anims\ANIMALS\CT_HIT.STI (from slf-container)
//		and so on...
//
//	Of course, if App-name already present in map and we're adding
//	it again, it's Real-name will be overwritten with the new value.
//
//	Be sure, that App-name is stored in unified form. This means,
//	that all letters are upper-cased and all slashes are '\' (as this
//	was from the beginning). Real-names are stored as given, i.e.
//	full real path with slashes, used in current file system
//	(if it is external file), or relative path with '\' slashes
//	( in case of slf-container resources).
//
//	The process of filling such resource map consist of two phases:
//		1. Add resources from default application containers.
//		2. Add additional resource containers or directories,
//		   overriding previosly mapped resources, if any. We can
//		   override one resource multiple times during second phase.
//
//	When adding resources from directory (let's call it Root-dir),
//	we have relative path (Rel-path) to resource from Root-dir.
//	For example, Root-dir is "/home/lesh/ja2_113/Data-1.13/" and
//	"Ambient/Bird9.wav" is the Rel-path. Ok, let's continue.
//	At first, let's unify Rel-path. We'll get "AMBIENT\BIRD9.WAV".
//	This will be our App-name. And our Real-name will be concatenation
//	of Root-dir and Rel-path, i.e. "/home/lesh/ja2_113/Data-1.13/Ambient/Bird9.wav".
//	Thus, we'll have no problem with case-sensitive file systems.
//
// 	To increase versality of this technique, it is better to replace
//	R-name with an object, incapsulating common file operations.
//	This means object-oriented approach to be used and objects to
//	be created, that will represent real files and files in containers.
//	Also wrapper-objects for containers are possible (slf, zip).
//	But probably, I'll be happy to do only structure instead of objects.
//	Objects seems to me much of work to redo.
//
//===================================================================

#include "vfs.hpp"
#include "library_database.h"
#include "io_layer.h"


sgpVFS	VFS;

//===================================================================
//
//	Constructor of the VFS.
//
//===================================================================
sgpVFS::sgpVFS()
{
	ResourceMap.clear();
	FileMatchResults.clear();
	OpenedFiles.clear();
	ResourceLib.clear();
	FileMatchIndex = 0;
}

//===================================================================
//
//	Destructor of the VFS.
//
//===================================================================
sgpVFS::~sgpVFS()
{
	ResourceMap.clear();
	FileMatchResults.clear();
	OpenedFiles.clear();
	FreeAllContainers();
	ResourceLib.clear();
}

//===================================================================
//
//	AddContainerByIndex - adds contents of slf-container to resource
//	map using it's index.
//
//	in	uiContainerID: container's index
//
//	return: TRUE, if contents were added, otherwise FALSE
//
//===================================================================
BOOLEAN	sgpVFS::AddContainerByIndex( UINT32 uiContainerID )
{
	UINT32	uiResCounter;
	vfsString	strContainerDir, strResourceName;

	// enumerate all container resources

	if ( uiContainerID >= gFileDataBase.usNumberOfLibraries )
		return FALSE;

	// print directory first, and it's contents then
	strContainerDir = gFileDataBase.pLibraries[ uiContainerID ].sLibraryPath;
	AddResourceEntry( strContainerDir, strContainerDir, TRUE, uiContainerID, FALSE );

	for ( uiResCounter = 0; uiResCounter < gFileDataBase.pLibraries[ uiContainerID ].usNumberOfEntries; uiResCounter++ )
	{
		strResourceName = strContainerDir + gFileDataBase.pLibraries[ uiContainerID ].pFileHeader[uiResCounter].pFileName;
		AddResourceEntry( strResourceName, strResourceName, FALSE, uiContainerID, FALSE );
	}

	return TRUE;
}

//===================================================================
//
//	AddResourceEntry - adds entry (file or dir) to resource map
//
//	in	ResourceName: name of the game resource
//	in	RealName: filename with full path of this resource
//	in	IsDirectory: specifies, it is directory entry or not
//	in	LibraryID: holds library ID, where resource in contained
//	in	Writeable: specifies, it is writeable entry or not
//
//	return:	TRUE always
//
//===================================================================
BOOLEAN	sgpVFS::AddResourceEntry( const vfsString& ResourceName, const vfsString& RealName, BOOLEAN IsDirectory, UINT32 LibraryID, BOOLEAN Writeable )
{
	vfsEntry	Entry;

	Entry.RealName    = RealName;
	Entry.IsDirectory = IsDirectory;
	Entry.IsWriteable = Writeable;
	Entry.LibIndex    = LibraryID;
	Entry.FileIndex   = 0;

	ResourceMap[ ConvertToApplicationName( ResourceName ) ] = Entry;

	return TRUE;
}

//===================================================================
//
//	AddDirectoryContents - adds contents of directory to resource
//	map using it's index. Allows setting of writeable status and
//	optional real directory to redirect application directory to
//	other real directory. However, this redirection is not used
//	in project.
//
//	in	pDirPath: root directory, where recursive scan for resources begins
//	in	fWriteable: writeable status of currently adding directory
//	in	pOptionalDirectoryName: other directory to redirect resources
//
//	return: TRUE, if contents were added, otherwise FALSE
//
//===================================================================
BOOLEAN	sgpVFS::AddDirectoryContents( const CHAR8 *pDirPath, BOOLEAN fWriteable, const CHAR8 *pOptionalDirectoryName )
{
	// Theory
	// We need to enumerate all files and subdirectoies under pDirPath.
	// So we'll use supplied path and set it as current directory.
	// For our help we'll use vector dirToLook. There we will store
	// all subdirectories, we need to search in.
	//
	// Each iteration last element is taken from dirToLook (and deleted
	// from vector) and concatenated with pattern ("*"). Then search
	// process goes and result is in foundFiles (again vector).
	//
	// For each element in foundFiles a check is performed: if element is
	// a directory, then it is added to dirToLook and added to resource map.
	// If element is an ordinary file, it is simply added to resource map.
	//
	// Initially dirToLook holds only one element "" (empty string).

	STRING512		zCurDirSave;
	vfsStringArray	dirToLook, foundFiles;
	vfsString		dirToBeLooked;
	vfsString		OptionalDirectoryName, RootDirectory;
	vfsStringArrayIterator	foundFilesIterator;
	BOOLEAN			fIsDirectory;

	RootDirectory = pDirPath;

	if ( pOptionalDirectoryName == NULL )
		OptionalDirectoryName = "";
	else
	{
		OptionalDirectoryName = ConvertToApplicationName( pOptionalDirectoryName, TRUE );
		AddResourceEntry( OptionalDirectoryName, RootDirectory, TRUE, LIB_REAL_FILE, fWriteable );
	}

	IO_Dir_GetCurrentDirectory( zCurDirSave, 512 );
	IO_Dir_SetCurrentDirectory( pDirPath );
	dirToLook.push_back("");

	while ( !dirToLook.empty() )
	{
		GetDirectoryEntries( dirToLook.back(), foundFiles );
		dirToLook.pop_back();

		for ( foundFilesIterator = foundFiles.begin(); foundFilesIterator != foundFiles.end(); foundFilesIterator++ )
		{
			fIsDirectory = IO_IsDirectory( foundFilesIterator->c_str() );
			if ( fIsDirectory )
				dirToLook.push_back( *foundFilesIterator );
			AddResourceEntry( OptionalDirectoryName + *foundFilesIterator, RootDirectory + *foundFilesIterator, fIsDirectory, LIB_REAL_FILE, fWriteable );
		}
	}

	IO_Dir_SetCurrentDirectory( zCurDirSave );

	return TRUE;
}

//===================================================================
//
//	GetDirectoryEntries - collect directory listing to array of strings
//
//	in	DirToLook: root directory, where recursive scan for entries begins
//	out	FileList: array of resulting entries
//
//	return: always TRUE
//
//===================================================================
BOOLEAN	sgpVFS::GetDirectoryEntries( const vfsString& DirToLook, vfsStringArray& FileList )
{
	STRING512	entry;
	vfsString	dirPattern = DirToLook + "*";

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

//===================================================================
//
//	ConvertToApplicationName - convert filename or directory name to
//	application name. Conversion produces uppercased name with '\'
//	slashes. If supplied name is a directory name, an ending slash
//	is added.
//
//	in	FileName: filename to convert
//	in	IsDirectory: directory flag
//
//	return: application name
//
//===================================================================
vfsString	sgpVFS::ConvertToApplicationName( const vfsString& FileName, BOOLEAN IsDirectory )
{
	UINT32		uiCharIndex;
	vfsString	ConvertedName;

	for ( uiCharIndex = 0; uiCharIndex < FileName.length(); uiCharIndex++ )
	{
		if ( FileName[ uiCharIndex ] == '/' )
			ConvertedName += '\\';
		else
			ConvertedName += toupper( FileName[ uiCharIndex ] );
	}

	if ( IsDirectory && ConvertedName[ ConvertedName.length() - 1 ] != '\\' )
		ConvertedName += '\\';

	return ConvertedName;
}

//===================================================================
//
//	DebugDumpResources - output contents of resource map to file
//	for debug purposes. The output has format:
//	<AppName> ==> <Type>, <R/W>, <Container>, <RealName>
//	where	<Type> - entry type: file or directory
//			<R/W> - read only or writeable entry
//			<Container> - shows, entry is in container or on file system
//			<AppName> - application name (unified)
//			<RealName> - real name of entry
//
//	in	pDumpFileName: pointer to output file name
//
//===================================================================
void sgpVFS::DebugDumpResources( const CHAR8 *pDumpFileName )
{
	FILE	*file;
	vfsFileMapIterator	FilesIterator;
	if ( (file = fopen( pDumpFileName, "w+t" )) == NULL )
		return;
	fprintf(file, "Resources in map: %d\n", ResourceMap.size() );
	for ( FilesIterator = ResourceMap.begin(); FilesIterator != ResourceMap.end(); FilesIterator++ )
		fprintf(file, "%-32s ==>> %s, %c, ( %d, %d), %s\n",
			FilesIterator->first.c_str(),
			FilesIterator->second.IsDirectory ? "Dir " : "File",
			FilesIterator->second.IsWriteable ? 'W' : 'R',
			FilesIterator->second.LibIndex,
			FilesIterator->second.FileIndex,
			FilesIterator->second.RealName.c_str() );
	fclose( file );
}

//===================================================================
//
//	IsFileExist - checks resource for existance in resource map
//
//	in	pResourceName: pointer to resource name
//
//	return:	TRUE, if specified file exists, FALSE if not
//
//===================================================================
BOOLEAN	sgpVFS::IsFileExist( const CHAR8 *pResourceName   )
{
	// First, convert given name to application name. Then locate it
	// in resource map. There is three possible situations:
	// 1. given name is not found                 ==> FALSE
	// 2. given name is found, but it is not file ==> FALSE
	// 3. given name is found, but it is file     ==> TRUE

	vfsFileMapIterator	FoundEntry;

	FoundEntry = ResourceMap.find( ConvertToApplicationName( pResourceName ) );

	if ( FoundEntry == ResourceMap.end() )
		return FALSE;

	if ( FoundEntry->second.IsDirectory )
		return FALSE;

	return TRUE;
}

//===================================================================
//
//	IsDirExist - checks directory for existance in resource map
//
//	in	pDirectoryName: pointer to directory name
//
//	return:	TRUE, if specified directory exists, FALSE if not
//
//===================================================================
BOOLEAN	sgpVFS::IsDirExist ( const CHAR8 *pDirectoryName  )
{
	// First, convert given name to application name. Then locate it
	// in resource map. There is three possible situations:
	// 1. given name is not found                      ==> FALSE
	// 2. given name is found, but it is not directory ==> FALSE
	// 3. given name is found, but it is directory     ==> TRUE

	vfsFileMapIterator	FoundEntry;

	FoundEntry = ResourceMap.find( ConvertToApplicationName( pDirectoryName, TRUE ) );

	if ( FoundEntry == ResourceMap.end() )
		return FALSE;

	if ( !FoundEntry->second.IsDirectory )
		return FALSE;

	return TRUE;
}

//===================================================================
//
//	FindResource - trying to find given resource in map
//
//	in	pResourceName: resource name
//	out	Entry: result of the search - an entry.
//
//	return:	TRUE, if found, FALSE if not
//
//===================================================================
BOOLEAN sgpVFS::FindResource( const CHAR8 *pResourceName, vfsEntry& Entry )
{
	vfsFileMapIterator	FoundEntry;

	FoundEntry = ResourceMap.find( ConvertToApplicationName( pResourceName ) );

	if ( FoundEntry == ResourceMap.end() )
		return FALSE;

	Entry = FoundEntry->second;

	return TRUE;
}

//===================================================================
//
//	StartFilePatternMatch - perform searches of the application names,
//	that match the given wildcard pattern, inside resource map.
//	This function gathers all found names into array.
//
//	in	pPattern: path and pattern
//
//	return:	number of found names
//
//===================================================================
UINT32	sgpVFS::StartFilePatternMatch( const CHAR8 *pPattern )
{
	// 1. Extract directory name SearchDir from pattern
	// 2. Find the element in ResourceMap with the name of SearchDir
	// 3. Skip this element
	// 4. While element starts with SearchDir, do
	//    4.1. If element is file, then check him for pattern match.
	//         Store him, if matches. Proceed to next element.
	//    4.2. If element is directory, then remember it as SkipDir
	//         and skip elements, that starts from SkipDir.

	vfsFileMapIterator	ResourceIterator;
	vfsStringPos		LastSlashPos;
	vfsString			SearchDir, SkipDir;
	vfsString			pattern = ConvertToApplicationName( pPattern );

	FileMatchResults.clear();
	FileMatchIndex = 0;

	// extract directory name SearchDir from pattern
	LastSlashPos = pattern.rfind( "\\", pattern.size() );
	if ( LastSlashPos == vfsString::npos )
		SearchDir = "";
	else
		SearchDir = pattern.substr( 0, LastSlashPos + 1 );

	// find the element in ResourceMap with the name of SearchDir
	ResourceIterator = ResourceMap.find( SearchDir );
	if ( ResourceIterator == ResourceMap.end() )
		return( 0 );	// nothing was found, because directory in pattern doesn't exist

	ResourceIterator++;

	// while not end and element names starts from SearchDir...
	while ( (ResourceIterator != ResourceMap.end() ) && (ResourceIterator->first.find( SearchDir, 0 ) == 0) )
	{
		if ( !ResourceIterator->second.IsDirectory )
		{
			// check element name for pattern match.
			if ( IO_DoesFilenameMatchesPattern( pattern.c_str(), ResourceIterator->first.c_str() ) )
				FileMatchResults.push_back( ResourceIterator->first );
			ResourceIterator++;
		}
		else
		{
			// skip elements, that starts from SkipDir.
			SkipDir = ResourceIterator->first;
			while ( ResourceIterator->first.find( SkipDir, 0 ) == 0 )
				ResourceIterator++;
		}
	}

	return( FileMatchResults.size() );
}

//===================================================================
//
//	GetNextMatch - give next matched name
//
//	out	pFilename: next name
//	in	uiMaxLen: max size of the pFilename string
//
//	return:	TRUE, if name was given, FALSE if no names left
//
//===================================================================
BOOLEAN	sgpVFS::GetNextMatch( CHAR8 *pFilename, UINT32 uiMaxLen )
{
	if ( FileMatchIndex >= FileMatchResults.size() )
		return FALSE;

	strncpy( pFilename, FileMatchResults.at( FileMatchIndex++ ).c_str(), uiMaxLen );
	return TRUE;
}

//===================================================================
//
//	FinishFilePatternMatch - finish name searching with use of
//	wildcard patterns
//
//===================================================================
void sgpVFS::FinishFilePatternMatch( void )
{
	FileMatchResults.clear();
	FileMatchIndex = 0;
}

//===================================================================
//
//	GetResourceFilename - give full real name of the specified
//	application name
//
//	in	pResourceName: resource name
//	out	pFilename: full real filename
//	in	uiMaxLen: max size of the pFilename string
//
//	return:	TRUE, if name was given, FALSE if not
//
//===================================================================
BOOLEAN	sgpVFS::GetResourceFilename( const CHAR8 *pResourceName, CHAR8 *pFilename, UINT32 uiMaxLen )
{
	vfsEntry	entry;

	if ( !FindResource( pResourceName, entry ) )
		return FALSE;

	strncpy( pFilename, ResourceLib[ entry.LibIndex ]->full_name(entry.FileIndex).c_str(), uiMaxLen );
	return TRUE;
}

//===================================================================
//
//	GetFreeOpenedSlot - obtain a free slot of opened files array
//
//	return:	(>=0), if free slot was found, (-1), if no free slot
//
//===================================================================
INT32 sgpVFS::GetFreeOpenedSlot( void )
{
	INT32	cnt;
	
	for ( cnt = 0; cnt < OpenedFiles.size(); cnt++ )
	{
		if ( OpenedFiles[cnt].IsFree )
			return cnt;
	}
	
	return -1;	// failed to find
}

//===================================================================
//
//	LoadContainer - load resource container into resource library
//
//	in	pPath: container name
//
//	return:	(TRUE), if successful, (FALSE), if not
//
//===================================================================
BOOLEAN sgpVFS::LoadContainer( const CHAR8 *pPath )
{
	STRING512	ContainerName;
	INT32		Len;
	
	strcpy( ContainerName, pPath);
	
	// check for directory container
	if ( IO_IsDirectory(ContainerName) )
	{
		Len = strlen(ContainerName);
		
		// if there is no slash at end, add it
		if (ContainerName[ Len - 1 ] != SLASH)
		{
			ContainerName[ Len     ] = SLASH;
			ContainerName[ Len + 1 ] = 0;
		}
		
		// load directory container
		return LoadContainerDirectory( ContainerName );
	}
	
	if ( !IO_IsRegularFile(ContainerName) )
		return FALSE;

	return LoadContainerSLF( ContainerName );
}

//===================================================================
//
//	FreeAllContainers - close resources and release memory, allocated
//	for containers
//
//===================================================================
void sgpVFS::FreeAllContainers( void )
{
	INT32	i;
	
	for ( i = 0; i < ResourceLib.size(); i++ )
	{
		ResourceLib[i]->close_pak();
		delete ResourceLib[i];
	}
}

//===================================================================
//
//	InitContainer - load and place resource container into
//	resource library
//
//	in	lib: pointer to created resource library
//
//	return:	(TRUE), if successful, (FALSE), if not
//
//===================================================================
BOOLEAN sgpVFS::InitContainer( sgpResourcePak* lib, const CHAR8 *pPath )
{
	if ( !lib )
		return FALSE;
	
	if ( !lib->open_pak( pPath ) )
	{
		delete lib;
		return FALSE;
	}

	ResourceLib.push_back( lib );
	return TRUE;		
}

//===================================================================
//
//	LoadContainerDirectory - load directory resource container into
//	resource library
//
//	in	pPath: container name
//
//	return:	(TRUE), if successful, (FALSE), if not
//
//===================================================================
BOOLEAN sgpVFS::LoadContainerDirectory( const CHAR8 *pPath )
{
	sgpResourcePak*	lib;
	
	lib = new sgpDirResourcePak;
	return InitContainer( lib, pPath );
}

//===================================================================
//
//	LoadContainer - load slf resource container into resource library
//
//	in	pPath: container name
//
//	return:	(TRUE), if successful, (FALSE), if not
//
//===================================================================
BOOLEAN sgpVFS::LoadContainerSLF( const CHAR8 *pPath )
{
	sgpResourcePak*	lib;
	
	lib = new sgpSLFResourcePak;
	return InitContainer( lib, pPath );
}

//===================================================================
//
//	BuildResourceMap - construct resource map from sources in SrcList
//
//	in	SrcList: array of resource sources names (slf-files, directories, ...)
//
//===================================================================
void sgpVFS::BuildResourceMap( sgpStringArray& SrcList )
{
	INT32	i;
	BOOLEAN	result;
	
	// load all containers
	for ( i = 0; i < SrcList.size(); i++ )
	{
		// get source type
		if ( IO_IsDirectory( SrcList[i].c_str() ) )
		{
			result = LoadContainerDirectory( SrcList[i].c_str() );
		}
		else
		{
			result = LoadContainerSLF( SrcList[i].c_str() );
		}
		
		// report error, if any
		if ( !result )
			fprintf(stderr, "Failed to load container %s\n", SrcList[i].c_str() );
	}
	
	//browse through all containers
	for ( i = 0; i < ResourceLib.size(); i++ )
	{
		AddContainer( i );
	}
}

//===================================================================
//
//	AddContainer - adds contents of container to resource
//	map using it's index.
//
//	in	uiContainerID: container's index
//
//	return: TRUE, if contents were added, otherwise FALSE
//
//===================================================================
BOOLEAN	sgpVFS::AddContainer( UINT32 uiContainerID )
{
	UINT32			i;
	sgpStringArray	FileList;

	if ( uiContainerID >= ResourceLib.size() )
		return FALSE;

	// get resource names list
	ResourceLib[ uiContainerID ]->get_resource_names( FileList );

	for ( i = 0; i < FileList.size(); i++ )
	{
		AddResource( FileList[ i ], uiContainerID, i );
	}

	return TRUE;
}

//===================================================================
//
//	AddResource - adds entry to resource map
//
//	in	RealName: real filename of this resource
//	in	LibIndex: holds container index
//	in	FileIndex: holds file index in container
//
//	return:	TRUE always
//
//===================================================================
BOOLEAN	sgpVFS::AddResource( const vfsString& RealName, INT32 LibIndex, INT32 FileIndex )
{
	vfsEntry	Entry;
	vfsString	ResourceName;

	ResourceName      = ConvertToApplicationName( RealName );
	Entry.RealName    = RealName;
	Entry.IsDirectory = FALSE;
	Entry.IsWriteable = FALSE;
	Entry.LibIndex    = LibIndex;
	Entry.FileIndex   = FileIndex;

	ResourceMap[ ResourceName ] = Entry;

	return TRUE;
}

//===================================================================
//
//	Open - open resource file
//
//	in	pResourceName: filename of resource
//
//	return:	(-1) if error, (>=0) if successful
//
//===================================================================
INT32 sgpVFS::Open( const STR8 pResourceName )
{
	INT32			SlotNumber;
	vfsEntry		Resource;
	vfsFileHandler	Descriptor;
	
	if ( !FindResource( pResourceName, Resource ) )
		return -1;

	Descriptor.IsFree = FALSE;
	Descriptor.LibIndex = Resource.LibIndex;
	Descriptor.Handler = ResourceLib[ Resource.LibIndex ]->file_open( Resource.FileIndex );

	if ( Descriptor.Handler == NO_PAK_FILE )
		return -1;
	
	SlotNumber = GetFreeOpenedSlot();
	if ( SlotNumber != -1 )
		OpenedFiles[ SlotNumber ] = Descriptor;
	else
	{
		SlotNumber = OpenedFiles.size();
		OpenedFiles.push_back( Descriptor );
	}
	
	return SlotNumber;
}

//===================================================================
//
//	Close - close resource file
//
//	in	iFileHandle: handler of resource
//
//===================================================================
void sgpVFS::Close( INT32 iFileHandle )
{
	INT32	LibIndex, Handler;

	if ( iFileHandle < 0 || iFileHandle >= OpenedFiles.size() )
		return;
	
	LibIndex = OpenedFiles[ iFileHandle ].LibIndex;
	Handler  = OpenedFiles[ iFileHandle ].Handler;
	
	ResourceLib[ LibIndex ]->file_close( Handler );
	OpenedFiles[ iFileHandle ].IsFree = TRUE;
}

//===================================================================
//
//	Read - read data from resource file
//
//	in	iFileHandle: handler of resource
//	in	pReadBuffer: buffer for read operation
//	in	iSize: number of bytes to read
//
//	return:	(-1) if error, (==0) if end-of-file (>0) if successful read
//
//===================================================================
INT32 sgpVFS::Read( INT32 iFileHandle, void *pReadBuffer, INT32 iSize )
{
	INT32	LibIndex, Handler;

	if ( iFileHandle < 0 || iFileHandle >= OpenedFiles.size() )
		return -1;
	
	LibIndex = OpenedFiles[ iFileHandle ].LibIndex;
	Handler  = OpenedFiles[ iFileHandle ].Handler;
	
	return ResourceLib[ LibIndex ]->file_read( Handler, pReadBuffer, iSize );
}

//===================================================================
//
//	Write - write data to resource file. Unused
//
//	in	iFileHandle: handler of resource
//	in	pReadBuffer: buffer for write operation
//	in	iSize: number of bytes to write
//
//	return:	(-1) always
//
//===================================================================
INT32 sgpVFS::Write( INT32 iFileHandle, void *pWriteBuffer, INT32 iSize )
{
	INT32	LibIndex, Handler;

	if ( iFileHandle < 0 || iFileHandle >= OpenedFiles.size() )
		return -1;
	
	LibIndex = OpenedFiles[ iFileHandle ].LibIndex;
	Handler  = OpenedFiles[ iFileHandle ].Handler;
	
	return -1;
}

//===================================================================
//
//	Seek - set position in resource file
//
//	in	iFileHandle: handler of resource
//	in	iPosition: new position
//	in	iMethod: method of positioning. Possible values:
//			IO_SEEK_FROM_START	 - offset from start of file 
//			IO_SEEK_FROM_END	 - offset from end of file
//			IO_SEEK_FROM_CURRENT - offset from current position in file
//
//	return:	(FALSE) if error, (TRUE) if successful
//
//===================================================================
BOOLEAN sgpVFS::Seek( INT32 iFileHandle, INT32 iPosition, INT32 iMethod )
{
	INT32	LibIndex, Handler;

	if ( iFileHandle < 0 || iFileHandle >= OpenedFiles.size() )
		return FALSE;
	
	LibIndex = OpenedFiles[ iFileHandle ].LibIndex;
	Handler  = OpenedFiles[ iFileHandle ].Handler;
	
	return ResourceLib[ LibIndex ]->file_seek( Handler, iPosition, iMethod );
}

//===================================================================
//
//	Tell - get position in resource file
//
//	in	iFileHandle: handler of resource
//
//	return:	(-1) if error, ( current position ) if successful
//
//===================================================================
INT32 sgpVFS::Tell( INT32 iFileHandle )
{
	INT32	LibIndex, Handler;

	if ( iFileHandle < 0 || iFileHandle >= OpenedFiles.size() )
		return -1;
	
	LibIndex = OpenedFiles[ iFileHandle ].LibIndex;
	Handler  = OpenedFiles[ iFileHandle ].Handler;
	
	return ResourceLib[ LibIndex ]->file_pos( Handler );
}

//===================================================================
//
//	GetSize - get length of the file using its handle
//
//	in	iFileHandle: handler of resource
//
//	return:	(-1) if error, (>=0) if successful
//
//===================================================================
INT32 sgpVFS::GetSize( INT32 iFileHandle )
{
	INT32	LibIndex, Handler;

	if ( iFileHandle < 0 || iFileHandle >= OpenedFiles.size() )
		return -1;
	
	LibIndex = OpenedFiles[ iFileHandle ].LibIndex;
	Handler  = OpenedFiles[ iFileHandle ].Handler;
	
	return ResourceLib[ LibIndex ]->file_size_by_handler( Handler );
}

//===================================================================
//
//	GetSize - get length of the file using its name
//
//	in	iFileHandle: handler of resource
//
//	return:	(-1) if error, (>=0) if successful
//
//===================================================================
INT32 sgpVFS::GetSize( const CHAR8 *pResourceName )
{
	vfsEntry		Resource;
	
	if ( !FindResource( pResourceName, Resource ) )
		return -1;
	
	return ResourceLib[ Resource.LibIndex ]->file_size_by_index( Resource.FileIndex );
}

//===================================================================
//
//	IsEOF - check for end of file
//
//	in	iFileHandle: handler of resource
//
//	return:	(TRUE) if end of file, (FALSE) if not
//
//===================================================================
BOOLEAN sgpVFS::IsEOF  ( INT32 iFileHandle )
{
	if ( iFileHandle < 0 || iFileHandle >= OpenedFiles.size() )
		return TRUE;

	if ( Tell(iFileHandle) < GetSize(iFileHandle) )
		return FALSE;
	
	return TRUE;
}
