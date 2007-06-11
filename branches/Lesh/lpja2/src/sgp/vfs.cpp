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
}

//===================================================================
//
//	AddContainerByIndex
//
//===================================================================
BOOLEAN	sgpVFS::AddContainerByIndex( UINT32 uiContainerID )
{
	UINT32	uiResCounter, uiContCounter;
	vfsString	strContainerDir, strResourceName;

	// enumerate all container resources

	if ( uiContainerID >= gFileDataBase.usNumberOfLibraries )
		return FALSE;

	for ( uiContCounter = 0; uiContCounter < gFileDataBase.usNumberOfLibraries; uiContCounter++ )
	{
		// print directory first, and it's contents then
		strContainerDir = gFileDataBase.pLibraries[ uiContCounter ].sLibraryPath;
		AddResourceEntry( strContainerDir, strContainerDir, TRUE, uiContCounter, FALSE );

		for ( uiResCounter = 0; uiResCounter < gFileDataBase.pLibraries[ uiContCounter ].usNumberOfEntries; uiResCounter++ )
		{
			strResourceName = strContainerDir + gFileDataBase.pLibraries[ uiContCounter ].pFileHeader[uiResCounter].pFileName;
			AddResourceEntry( strResourceName, strResourceName, FALSE, uiContCounter, FALSE );
		}
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
	Entry.LibraryID   = LibraryID;

	ResourceMap[ ConvertToApplicationName( ResourceName ) ] = Entry;

	return TRUE;
}

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

BOOLEAN	sgpVFS::GetDirectoryEntries( const vfsString& DirToLook, vfsStringArray& FileList )
{
	STRING512	entry;
	vfsString	dirPattern = DirToLook + "*";

	FileList.clear();

	if ( IO_File_GetFirst( dirPattern.c_str(), entry, 512 ) )
	{
		do
		{
			// add filename in file list
			// additionally, in case of directory, add an ending slash to it
			FileList.push_back( entry );
			if ( IO_IsDirectory( entry ) )
				FileList.back() += "/";
				
		} while ( IO_File_GetNext( entry, 512 ) );
		IO_File_GetClose();
	}

	return TRUE;
}

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
		fprintf(file, "%-32s ==>> %s, %c, %s, %s\n",
			FilesIterator->first.c_str(),
			FilesIterator->second.IsDirectory ? "Dir " : "File",
			FilesIterator->second.IsWriteable ? 'W' : 'R',
			FilesIterator->second.LibraryID == LIB_REAL_FILE ? "FS " : "SLF",
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

BOOLEAN sgpVFS::FindResource( const CHAR8 *pResourceName, vfsEntry& Entry )
{
	vfsFileMapIterator	FoundEntry;

	FoundEntry = ResourceMap.find( ConvertToApplicationName( pResourceName ) );

	if ( FoundEntry == ResourceMap.end() )
		return FALSE;

	Entry = FoundEntry->second;

	return TRUE;
}

UINT32	sgpVFS::StartFilePatternMatch( const CHAR8 *pPattern )
{
	vfsFileMapIterator	ResourceIterator;
	vfsString	pattern = ConvertToApplicationName( pPattern );

	FileMatchResults.clear();
	FileMatchIndex = 0;

	for ( ResourceIterator = ResourceMap.begin(); ResourceIterator != ResourceMap.end(); ResourceIterator++ )
	{
		if ( IO_DoesFilenameMatchesPattern( pattern.c_str(), ResourceIterator->first.c_str() ) )
			FileMatchResults.push_back( ResourceIterator->first );
	}

	return( FileMatchResults.size() );
}

BOOLEAN	sgpVFS::GetNextMatch( CHAR8 *pFilename, UINT32 uiMaxLen )
{
	if ( FileMatchIndex >= FileMatchResults.size() )
		return FALSE;

	strncpy( pFilename, FileMatchResults.at( FileMatchIndex++ ).c_str(), uiMaxLen );
	return TRUE;
}

void sgpVFS::FinishFilePatternMatch( void )
{
	FileMatchResults.clear();
	FileMatchIndex = 0;
}

BOOLEAN	sgpVFS::GetResourceFilename( const CHAR8 *pResourceName, CHAR8 *pFilename, UINT32 uiMaxLen )
{
	vfsEntry	entry;

	if ( !FindResource( pResourceName, entry ) )
		return FALSE;

	strncpy( pFilename, entry.RealName.c_str(), uiMaxLen );
	return TRUE;
}
