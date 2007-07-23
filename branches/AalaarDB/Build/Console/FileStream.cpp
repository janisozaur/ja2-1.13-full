/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2001-2004 by Marko Bozikovic
// All rights reserved
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc., and
// I'll try to keep a version up to date.  I can be reached as follows:
//    marko.bozikovic@alterbox.net
//    bozho@kset.org
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// FileStream.cpp	- implementation of IStream interface on any file 
//					  (not strictly COM, I'm faking it here a bit :-)
//

#undef CINTERFACE
#include "builddefines.h"

//#include "stdafx.h"
//#include <atlbase.h>
#include <Windows.h>
#include <objidl.h>
#include "FileStream.h"

#include "profiler.h"
//#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#endif

/////////////////////////////////////////////////////////////////////////////
// CreateFileStream	- opens/creates a file and returns an IStream interface

HRESULT CreateFileStream(
  LPCTSTR lpFileName,							// file name
  DWORD dwDesiredAccess,						// access mode
  DWORD dwShareMode,							// share mode
  LPSECURITY_ATTRIBUTES lpSecurityAttributes,	// SD
  DWORD dwCreationDisposition,					// how to create
  DWORD dwFlagsAndAttributes,					// file attributes
  HANDLE hTemplateFile,							// handle to template file
  IStream** ppStream							// pointer to IStream interface
)
{
	PERFORMANCE_MARKER

	HANDLE hFile = ::CreateFile(
						lpFileName,
						dwDesiredAccess, 
						dwShareMode,
						lpSecurityAttributes,
						dwCreationDisposition,
						dwFlagsAndAttributes,
						hTemplateFile);

	if (hFile == INVALID_HANDLE_VALUE) {
		DWORD a = ::GetLastError();
		ppStream = NULL;
		return E_FAIL;
	}

	FileStream* pFileStream = new FileStream(hFile);

	return pFileStream->QueryInterface(IID_IStream, (void**)ppStream);
}

/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// FileStream class

/////////////////////////////////////////////////////////////////////////////
// ctor/dtor

FileStream::FileStream(HANDLE hFile)
	: m_lRefCount(0)
	, m_hFile(hFile)
{
	PERFORMANCE_MARKER
}

FileStream::~FileStream()
{
	PERFORMANCE_MARKER
	if (m_hFile) ::CloseHandle(m_hFile);
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// IStream methods

/////////////////////////////////////////////////////////////////////////////
// Read	- reads a specified number of bytes from the stream object into memory 
//		  starting at the current seek pointer

STDMETHODIMP FileStream::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
	PERFORMANCE_MARKER
	if (!::ReadFile(m_hFile, pv, cb, pcbRead, NULL)) return S_FALSE;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Write	- writes a specified number of bytes into the stream object starting 
//			  at the current seek pointer

STDMETHODIMP FileStream::Write(void const *pv, ULONG cb, ULONG *pcbWritten)
{
	PERFORMANCE_MARKER
	if (!::WriteFile(m_hFile, pv, cb, pcbWritten, NULL)) return S_FALSE;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Seek	- changes the seek pointer to a new location relative to the beginning 
//		  of the stream, to the end of the stream, or to the current seek pointer

STDMETHODIMP FileStream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
{
	PERFORMANCE_MARKER
	DWORD dwFileOrigin;

	DWORD dwMoveLow = dlibMove.LowPart;
	LONG lMoveHigh = dlibMove.HighPart;
	
	switch (dwOrigin) {
		case STREAM_SEEK_SET : dwFileOrigin = FILE_BEGIN; break;
		case STREAM_SEEK_CUR : dwFileOrigin = FILE_CURRENT; break;
		case STREAM_SEEK_END : dwFileOrigin = FILE_END; break;
		default:dwFileOrigin = FILE_BEGIN;
	}

	DWORD dwRet = ::SetFilePointer(m_hFile, dwMoveLow, &lMoveHigh, dwFileOrigin);

	if (::GetLastError() != NO_ERROR) return STG_E_INVALIDPOINTER;

	plibNewPosition->LowPart = dwRet;
	plibNewPosition->HighPart = lMoveHigh;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// SetSize	- not really doing anything here

STDMETHODIMP FileStream::SetSize(ULARGE_INTEGER libNewSize)
{
	PERFORMANCE_MARKER
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CopyTo	- copies a specified number of bytes from the current seek pointer 
//			  in the stream to the current seek pointer in another stream

STDMETHODIMP FileStream::CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten)
{
	PERFORMANCE_MARKER
	// later...
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Commit - no work here...

STDMETHODIMP FileStream::Commit(DWORD grfCommitFlags)
{
	PERFORMANCE_MARKER
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Revert - no work here...

STDMETHODIMP FileStream::Revert(void)
{
	PERFORMANCE_MARKER
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// LockRegion - no work here...

STDMETHODIMP FileStream::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	PERFORMANCE_MARKER
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// UnlockRegion - no work here...

STDMETHODIMP FileStream::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	PERFORMANCE_MARKER
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Stat - no work here...

STDMETHODIMP FileStream::Stat(STATSTG *pstatstg, DWORD grfStatFlag)
{
	PERFORMANCE_MARKER
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Clone - no work here...

STDMETHODIMP FileStream::Clone(IStream **ppstm)
{
	PERFORMANCE_MARKER
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
