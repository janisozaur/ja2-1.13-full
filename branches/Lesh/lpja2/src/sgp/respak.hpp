#ifndef __RESPAK_H_
#define __RESPAK_H_

// Author: Lesh		Nov 2007

//===================================================================
//	  Add description here
//===================================================================

#include "types.h"
#include "platform.h"
#include <vector>
#include "io_layer.h"

typedef	INT32		pakFile;
#define				NO_PAK_FILE		-1


// Abstract resource pak class goes first
// ==============================================================

class sgpResourcePak
{
private:
protected:
	sgpString		pak_path;
	BOOLEAN			is_opened;

	virtual BOOLEAN		load_pak( void ) = 0;
	virtual void		unload_pak( void ) = 0;
	virtual INT32		get_index_from_name( const sgpString& name ) = 0;
	virtual BOOLEAN		is_already_opened( UINT32 index ) = 0;
	virtual INT32		get_free_opened_slot( void ) = 0;
	
public:
	sgpResourcePak();
	sgpResourcePak( const sgpResourcePak& obj );

	virtual ~sgpResourcePak() = 0;
	
			BOOLEAN		open_pak( sgpString path );
			BOOLEAN		close_pak( void );
	
	virtual	void		print_pak( void ) = 0;
	virtual void		get_resource_names( sgpStringArray& contents ) = 0;
	virtual pakFile		file_open ( INT32 file_index ) = 0;
	virtual BOOLEAN		file_close( pakFile file ) = 0;
	virtual INT32		file_read ( pakFile file, PTR data, INT32 bytes_to_read ) = 0;
//	virtual INT32		file_write( pakFile file, PTR data, INT32 bytes_to_write ) = 0;
	virtual BOOLEAN		file_seek ( pakFile file, INT32 distance, UINT8 seek_mode ) = 0;
	virtual INT32		file_pos  ( pakFile file ) = 0;
	virtual INT32		file_size_by_handler ( pakFile file ) = 0;
	virtual INT32		file_size_by_index   ( INT32 index  ) = 0;
};

typedef std::vector<sgpResourcePak*>	sgpPakArray;





// SLF resource pak class goes below
// ==============================================================

typedef struct
{
	sgpString	file_name;				// file name
	UINT32		file_length;			// file lenght
	UINT32		file_offset;			// file offset
} slfEntry;

typedef std::vector<slfEntry>	slfResourceArray;

typedef struct	
{
	UINT32		file_id;				// id of the file
	UINT32		file_pos;				// current position in the file
} slfOpenedFileEntry;

typedef std::vector<slfOpenedFileEntry>	slfOpenedFiles;

#define			FREE_FILE_ID			-1




class sgpSLFResourcePak : public sgpResourcePak
{
private:
	IOFILE			pak_handler;

protected:
	slfResourceArray	catalog;
	slfOpenedFiles		opened_file;

	virtual BOOLEAN		load_pak( void );
	virtual void		unload_pak( void );
	virtual INT32		get_index_from_name( const sgpString& name );
	virtual BOOLEAN		is_already_opened( UINT32 index );
	virtual INT32		get_free_opened_slot( void );
	
public:
	sgpSLFResourcePak();
	sgpSLFResourcePak( const sgpSLFResourcePak& obj );

	virtual ~sgpSLFResourcePak();
	
	virtual void		print_pak( void );
	virtual void		get_resource_names( sgpStringArray& contents );
	virtual pakFile		file_open ( INT32 file_index );
	virtual BOOLEAN		file_close( pakFile file );
	virtual INT32		file_read ( pakFile file, PTR data, INT32 bytes_to_read );
//	virtual INT32		file_write( pakFile file, PTR data, INT32 bytes_to_write );
	virtual BOOLEAN		file_seek ( pakFile file, INT32 distance, UINT8 seek_mode );
	virtual INT32		file_pos  ( pakFile file );
	virtual INT32		file_size_by_handler ( pakFile file );
	virtual INT32		file_size_by_index   ( INT32 index  );
};






// Directory resource pak class goes below
// ==============================================================

typedef IOFILE							dirOpenedFileEntry;
typedef std::vector<dirOpenedFileEntry>	dirOpenedFiles;


class sgpDirResourcePak : public sgpResourcePak
{
private:
			BOOLEAN		get_directory_listing( const sgpString& DirToLook, sgpStringArray& FileList );
	
protected:
	sgpStringArray		catalog;
	dirOpenedFiles		opened_file;

	virtual BOOLEAN		load_pak( void );
	virtual void		unload_pak( void );
	virtual INT32		get_index_from_name( const sgpString& name );
	virtual BOOLEAN		is_already_opened( UINT32 index );
	virtual INT32		get_free_opened_slot( void );
	
public:
	sgpDirResourcePak();
	sgpDirResourcePak( const sgpDirResourcePak& obj );

	virtual ~sgpDirResourcePak();
	
	virtual void		print_pak( void );
	virtual void		get_resource_names( sgpStringArray& contents );
	virtual pakFile		file_open ( INT32 file_index );
	virtual BOOLEAN		file_close( pakFile file );
	virtual INT32		file_read ( pakFile file, PTR data, INT32 bytes_to_read );
//	virtual INT32		file_write( pakFile file, PTR data, INT32 bytes_to_write );
	virtual BOOLEAN		file_seek ( pakFile file, INT32 distance, UINT8 seek_mode );
	virtual INT32		file_pos  ( pakFile file );
	virtual INT32		file_size_by_handler ( pakFile file );
	virtual INT32		file_size_by_index   ( INT32 index  );
};

#endif
