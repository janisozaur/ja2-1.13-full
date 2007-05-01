//
// Snap: Implementation of the TFileCat class
//

#include "platform.h"
#include "file_cat.h"
#include "read_dir.h"
#include "file_man.h"

bool IsDirectory(const char *path)
{
	struct stat dir_stat;

    if (stat(path, &dir_stat) == -1)
	{
		printf("IsDirectory() fails: path %s\n", path);
		return false;
	}

	return( S_ISDIR(dir_stat.st_mode) );
}

bool IsRegularFile(const char *path)
{
	struct stat file_stat;

    if (stat(path, &file_stat) == -1)
	{
		printf("IsRegularFile() fails: path %s\n", path);
		return false;
	}

	return( S_ISREG(file_stat.st_mode) );
}

// Remove a slash or backslash (if any) from the end of a string
void ChompSlash(std::string& s)
{
	if ( s.empty() )
		return;

	if ( *s.rbegin() == '\\' || *s.rbegin() == '/' )
	{
		s.erase( s.length() - 1 );
	}
}


// Build a new file catalogue by recursively traversing the root directory
void TFileCat::NewCat(std::string root)
{
	char tmpRoot[512];

	strncpy(tmpRoot, root.c_str(), 512);
	BACKSLASH(tmpRoot);

	fRootDir = tmpRoot;
	ChompSlash(fRootDir);

	fFileCat.clear();

	TraverseDir(fRootDir);
}


// Look for a given file in the catalogue
// Unless pathIncludesRoot == true, will prepend the root directory to path
bool TFileCat::FindFile(std::string path, bool pathIncludesRoot) const
{
	if (pathIncludesRoot)
		return fFileCat.find(path) != fFileCat.end();
	else
		return fFileCat.find(fRootDir + SLASH + path) != fFileCat.end();
}


// Delete a given file from the catalogue
// Unless pathIncludesRoot == true, will prepend the root directory to path
size_t TFileCat::RemoveFile(std::string path, bool pathIncludesRoot)
{
	if (pathIncludesRoot)
		return fFileCat.erase(path);
	else
		return fFileCat.erase(fRootDir + SLASH + path);
}


// Delete all files from a given directory in the catalogue
// Unless pathIncludesRoot == true, will prepend the root directory to path
size_t TFileCat::RemoveDir(std::string dir, bool pathIncludesRoot)
{
	if ( !pathIncludesRoot )
		dir = fRootDir + SLASH + dir;

	ChompSlash(dir);
	std::string dirlower = dir + SLASH;
	std::string dirupper = dir + char(SLASH+1);

	TCatalogue::iterator upper = fFileCat.upper_bound(dirupper);
	TCatalogue::iterator lower;

	int deleted = 0;

	while ( ( lower = fFileCat.lower_bound(dirlower) ) != upper)
	{
		fFileCat.erase(lower);
		deleted++;
	}

	return deleted;
}


// Recursively traverse a directory, adding regular files to the catalogue
void TFileCat::TraverseDir(std::string dir, int depth)
{
	using std::string;
	SGPFILENAME fileName;

	if (!dir.empty())
		dir += SLASH;

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	TReadDir readDir((dir + "*").c_str());

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	TReadDir readDir( dir.c_str() );

// -------------------- End of Linux-specific stuff ------------------------
#endif

	while ( readDir.NextFile(fileName) )
	{
		if (string(".") == fileName || string("..") == fileName)
			continue;

		string fullPath = dir + fileName;

		if ( IsDirectory( fullPath.c_str() ) )
		{
			if (depth < 0) TraverseDir(fullPath);
			else
				if (depth > 0) TraverseDir(fullPath, depth-1);
		}
		else
		{
			fFileCat.insert(fullPath);
		}
	}
	readDir.Close();
}
