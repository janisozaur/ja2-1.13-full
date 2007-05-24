#ifndef __VFS_TYPES_H_
#define __VFS_TYPES_H_

// Author: Lesh		May 2007

//===================================================================
//	vfs_types.hpp - virtual file system types header
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
//	- replace/add some resources, using directories or containers;
//	  
//===================================================================

#include "types.h"
#include "platform.h"
#include <map>
#include <string>
#include <vector>

typedef std::string							vfsString;
typedef std::map <vfsString, vfsString>	vfsFileMap;
typedef std::vector <vfsString>				vfsStringArray;
typedef std::vector <vfsString>::iterator	vfsStringArrayIterator;

#endif
