#ifndef __VFS_H_
#define __VFS_H_

// Author: Lesh		May 2007

//===================================================================
//	vfs.hpp - virtual file system header
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

#include "vfs_types.hpp"
#include "respak.hpp"

class sgpVFS
{
private:
protected:
	vfsFileMap 		ResourceMap;
	vfsStringArray	FileMatchResults;
	UINT32			FileMatchIndex;
	vfsOpenFiles	OpenedFiles;
	sgpPakArray		ResourceLib;

	BOOLEAN		AddResourceEntry( const vfsString& ResourceName, const vfsString& RealName, BOOLEAN IsDirectory, UINT32 LibraryID, BOOLEAN Writeable );
	BOOLEAN		GetDirectoryEntries( const vfsString& DirToLook, vfsStringArray& FileList );
	vfsString	ConvertToApplicationName( const vfsString& FileName, BOOLEAN IsDirectory = FALSE );
	INT32		GetFreeOpenedSlot( void );

	void		FreeAllContainers( void );
	BOOLEAN		InitContainer( sgpResourcePak* lib, const CHAR8 *pPath );
	BOOLEAN		LoadContainerDirectory( const CHAR8 *pPath );
	BOOLEAN		LoadContainerSLF( const CHAR8 *pPath );
	BOOLEAN		AddContainer( UINT32 uiContainerID );
	BOOLEAN		AddResource( const vfsString& RealName, INT32 LibIndex, INT32 FileIndex );
	
public:
	sgpVFS();
	~sgpVFS();

	BOOLEAN		Initialize	( void );
	void		Shutdown	( void );

	void		BuildResourceMap( sgpStringArray& SrcList );
	BOOLEAN		FindResource( const CHAR8 *pResourceName, vfsEntry& Entry );
	BOOLEAN		GetResourceFilename( const CHAR8 *pResourceName, CHAR8 *pFilename, UINT32 uiMaxLen );
	UINT32		StartFilePatternMatch ( const CHAR8 *pPattern );
	BOOLEAN		GetNextMatch          ( CHAR8 *pFilename, UINT32 uiMaxLen );
	void		FinishFilePatternMatch( void );

	INT32		Open ( const STR8 pResourceName );
	void		Close( INT32 iFileHandle );
	INT32		Read ( INT32 iFileHandle, void *pReadBuffer,  INT32 iSize );
	INT32		Write( INT32 iFileHandle, void *pWriteBuffer, INT32 iSize );
	BOOLEAN		Seek ( INT32 iFileHandle, INT32 iPosition, INT32 iMethod );
	INT32		Tell ( INT32 iFileHandle                                   );
	INT32		GetSize( INT32 iFileHandle );
	INT32		GetSize( const CHAR8 *pResourceName );
	BOOLEAN		IsEOF  ( INT32 iFileHandle );

	BOOLEAN		IsFileExist( const CHAR8 *pResourceName   );
	BOOLEAN		IsDirExist ( const CHAR8 *pDirectoryName  );

	BOOLEAN		AddContainerByIndex ( UINT32 uiContainerID );
	BOOLEAN		AddDirectoryContents( const CHAR8 *pDirPath, BOOLEAN fWriteable, const CHAR8 *pOptionalDirectoryName = NULL );

	BOOLEAN		LoadContainer( const CHAR8 *pPath );

	void		DebugDumpResources( const CHAR8 *pDumpFileName );
};

extern sgpVFS	VFS;

#endif
