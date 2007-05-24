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

sgpVFS::sgpVFS()
{
	ResourceMap.clear();
}

sgpVFS::~sgpVFS()
{
	ResourceMap.clear();
}

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
		AddDirectory( strContainerDir );

		for ( uiResCounter = 0; uiResCounter < gFileDataBase.pLibraries[ uiContCounter ].usNumberOfEntries; uiResCounter++ )
		{
			strResourceName = strContainerDir + gFileDataBase.pLibraries[ uiContCounter ].pFileHeader[uiResCounter].pFileName;
			AddResourceFile( strResourceName );
		}
	}

	FILE	*file;
	vfsFileMap::iterator	FilesIterator;
	if ( (file = fopen("add.log", "w+t")) == NULL )
		return FALSE;
	fprintf(file, "%d\n", ResourceMap.size() );
	for ( FilesIterator = ResourceMap.begin(); FilesIterator != ResourceMap.end(); FilesIterator++ )
		fprintf(file, "%s\n", FilesIterator->second.c_str() );
	fclose( file );

	return TRUE;
}

BOOLEAN	sgpVFS::AddResourceFile( const vfsString& RealFileName )
{
	// FileAppName is an application name of resource, transformed from RealFileName. It is unified.
	// todo: $$$ - more work needed
	vfsString FileAppName = RealFileName;

	ResourceMap[ FileAppName ] = RealFileName;

	return TRUE;
}

BOOLEAN	sgpVFS::AddDirectory   ( const vfsString& RealDirName )
{
	// DirAppName is an application name of resource, transformed from ReadDirName. It is unified.
	// todo: $$$ - more work needed
	vfsString DirAppName = RealDirName;

	ResourceMap[ DirAppName ] = RealDirName;

	return TRUE;
}

BOOLEAN	sgpVFS::AddReadDirectory( const STR8 pDirPath )
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
	vfsStringArrayIterator	foundFilesIterator;

	IO_Dir_GetCurrentDirectory( zCurDirSave, 512 );
	IO_Dir_SetCurrentDirectory( pDirPath );
	dirToLook.push_back("");

	while ( !dirToLook.empty() )
	{
		GetDirectoryEntries( dirToLook.back(), foundFiles );
		dirToLook.pop_back();

		for ( 	foundFilesIterator = foundFiles.begin();
				foundFilesIterator != foundFiles.end();
				foundFilesIterator++ )
		{
			if ( IO_IsDirectory( foundFilesIterator->c_str() ) )
			{
				dirToLook.push_back( *foundFilesIterator );
				AddDirectory( *foundFilesIterator );
			}
			else
			{
				AddResourceFile( *foundFilesIterator );
			}
		}
	}

	IO_Dir_SetCurrentDirectory( zCurDirSave );

	FILE	*file;
	vfsFileMap::iterator	FilesIterator;
	if ( (file = fopen("add1.log", "w+t")) == NULL )
		return FALSE;
	fprintf(file, "%d\n", ResourceMap.size() );
	for ( FilesIterator = ResourceMap.begin(); FilesIterator != ResourceMap.end(); FilesIterator++ )
		fprintf(file, "%s\n", FilesIterator->second.c_str() );
	fclose( file );

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
	}

	return TRUE;
}
