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

//using namespace std;

/*
class sgpAbstractFileResource
{
private:
protected:
	CHAR8*	zApplicationName;
	CHAR8*	zRealName;
	BOOLEAN	fIsReadOnly;

public:
	sgpAbstractFileResource();
	~sgpAbstractFileResource() = 0;

	BOOLEAN	Open ( UINT32 uiOpenFlags ) = 0;
	void		Close( void                   ) = 0;

	BOOLEAN	Read ( void* pReadBuffer,  UINT32 uiSize ) = 0;
	BOOLEAN	Write( void* pWriteBuffer, UINT32 uiSize ) = 0;

	UINT32	Seek ( UINT32 uiPosition, UINT32 uiMethod ) = 0;
	UINT32	Tell ( void                                       ) = 0;

	UINT32	GetSize( void ) = 0;
};
*/

class sgpVFS
{
private:
protected:
	vfsFileMap ResourceMap;

	BOOLEAN		AddResourceFile( const vfsString& RealFileName );
	BOOLEAN		AddDirectory   ( const vfsString& DirAName );
	BOOLEAN		GetDirectoryEntries( const vfsString& DirToLook, vfsStringArray& FileList );

public:
	sgpVFS();
	~sgpVFS();

	BOOLEAN		Initialize	( void );
	void		Shutdown	( void );

	UINT32		Open ( const STR8 pResourceName, UINT32 uiOpenFlags  );
	void		Close( UINT32 uiFileHandle );

	BOOLEAN		Read ( UINT32 uiFileHandle, void* pReadBuffer,  UINT32 uiSize );
	BOOLEAN		Write( UINT32 uiFileHandle, void* pWriteBuffer, UINT32 uiSize );

	UINT32		Seek ( UINT32 uiFileHandle, UINT32 uiPosition, UINT32 uiMethod );
	UINT32		Tell ( UINT32 uiFileHandle                                     );

	UINT32		GetSize( UINT32 uiFileHandle );
	BOOLEAN		IsEOF  ( UINT32 uiFileHandle );
	BOOLEAN		IsFileExist( const STR* pResourceName   );
	BOOLEAN		IsDirExist ( const STR* pDirectoryName  );

	BOOLEAN		AddContainerByIndex( UINT32 uiContainerID );
	BOOLEAN		AddReadDirectory( const STR8 pDirPath );
};

extern sgpVFS	VFS;

#endif
