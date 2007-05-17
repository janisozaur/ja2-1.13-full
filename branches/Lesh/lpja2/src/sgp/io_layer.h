#ifndef __IO_LAYER_H_
#define __IO_LAYER_H_

// Author: Lesh		May 2007

//===================================================================
//	io_layer.h - I/O layer header
//
//	This file is intended for I/O access portability
//	  
//===================================================================

#define IO_ACCESS_READ	    0x01
#define IO_ACCESS_WRITE	    0x02
#define IO_ACCESS_READWRITE	0x03

#define IO_CREATE_NEW		0x0010	// create new file. fail if exists
#define IO_CREATE_ALWAYS	0x0020	// create new file. overwrite existing
#define IO_OPEN_EXISTING	0x0040	// open a file. fail if doesn't exist
#define IO_OPEN_ALWAYS		0x0080	// open a file, create if doesn't exist

#define IO_SEEK_FROM_START		0x01
#define IO_SEEK_FROM_END		0x02
#define IO_SEEK_FROM_CURRENT	0x04

BOOLEAN IO_File_Delete( const STR8 path );

BOOLEAN IO_IsDirectory(const STR8 path);
BOOLEAN IO_IsRegularFile(const STR8 path);

#endif
