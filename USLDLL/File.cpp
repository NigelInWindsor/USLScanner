#include "Stdafx.h"
#include "USLDLL.h"
#include "File.h"
#include <iostream>


CFile::CFile()
{
	m_hFile = (UINT) hFileNull;
	m_bCloseOnDelete = FALSE;
}

CFile::CFile(int hFile)
{
	m_hFile = hFile;
	m_bCloseOnDelete = FALSE;
}

CFile::CFile(LPCTSTR lpszFileName, UINT nOpenFlags)
{

	Open(lpszFileName, nOpenFlags);
}

CFile::~CFile()
{
	if (m_hFile != (UINT)hFileNull && m_bCloseOnDelete)
		Close();
}


BOOL CFile::Open(LPCTSTR lpszFileName, UINT nOpenFlags)
{

	// CFile objects are always binary and CreateFile does not need flag
	nOpenFlags &= ~(UINT)typeBinary;

	m_bCloseOnDelete = FALSE;
	m_hFile = (UINT)hFileNull;

	DWORD dwAccess = 0;
	switch (nOpenFlags & 3)
	{
	case modeRead:
		dwAccess = GENERIC_READ;
		break;
	case modeWrite:
		dwAccess = GENERIC_WRITE;
		break;
	case modeReadWrite:
		dwAccess = GENERIC_READ|GENERIC_WRITE;
		break;
	default:
		;
	}

	// map share mode
	DWORD dwShareMode = 0;
	switch (nOpenFlags & 0x70)    // map compatibility mode to exclusive
	{
	default:
		;
	case shareCompat:
	case shareExclusive:
		dwShareMode = 0;
		break;
	case shareDenyWrite:
		dwShareMode = FILE_SHARE_READ;
		break;
	case shareDenyRead:
		dwShareMode = FILE_SHARE_WRITE;
		break;
	case shareDenyNone:
		dwShareMode = FILE_SHARE_WRITE|FILE_SHARE_READ;
		break;
	}

	// Note: typeText and typeBinary are used in derived classes only.

	// map modeNoInherit flag
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = (nOpenFlags & modeNoInherit) == 0;

	// map creation flags
	DWORD dwCreateFlag;
	if (nOpenFlags & modeCreate)
	{
		if (nOpenFlags & modeNoTruncate)
			dwCreateFlag = OPEN_ALWAYS;
		else
			dwCreateFlag = CREATE_ALWAYS;
	}
	else
		dwCreateFlag = OPEN_EXISTING;

	// attempt file creation
	HANDLE hFile = ::CreateFile(lpszFileName, dwAccess, dwShareMode, &sa,
		dwCreateFlag, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	m_hFile = (HFILE)hFile;
	m_bCloseOnDelete = TRUE;

	return TRUE;
}

UINT CFile::Read(void* lpBuf, UINT nCount)
{

	if (nCount == 0)
		return 0;   // avoid Win32 "null-read"


	DWORD dwRead;
	::ReadFile((HANDLE)m_hFile, lpBuf, nCount, &dwRead, NULL);

	return (UINT)dwRead;
}

void CFile::Write(const void* lpBuf, UINT nCount)
{

	if (nCount == 0)
		return;     // avoid Win32 "null-write" option


	DWORD nWritten;
	::WriteFile((HANDLE)m_hFile, lpBuf, nCount, &nWritten, NULL);

}

LONG CFile::Seek(LONG lOff, UINT nFrom)
{

	DWORD dwNew = ::SetFilePointer((HANDLE)m_hFile, lOff, NULL, (DWORD)nFrom);

	return dwNew;
}

DWORD CFile::GetPosition() const
{

	DWORD dwPos = ::SetFilePointer((HANDLE)m_hFile, 0, NULL, FILE_CURRENT);

	return dwPos;
}

void CFile::Flush()
{

	if (m_hFile == (UINT)hFileNull)
		return;

	::FlushFileBuffers((HANDLE)m_hFile);
}

void CFile::Close()
{

	BOOL bError = FALSE;
	if (m_hFile != (UINT)hFileNull)
		bError = !::CloseHandle((HANDLE)m_hFile);

	m_hFile = (UINT) hFileNull;
	m_bCloseOnDelete = FALSE;

}

void CFile::Abort()
{
	if (m_hFile != (UINT)hFileNull)
	{
		// close but ignore errors
		::CloseHandle((HANDLE)m_hFile);
		m_hFile = (UINT)hFileNull;
	}
}

void CFile::LockRange(DWORD dwPos, DWORD dwCount)
{
	::LockFile((HANDLE)m_hFile, dwPos, 0, dwCount, 0);
}

void CFile::UnlockRange(DWORD dwPos, DWORD dwCount)
{

	::UnlockFile((HANDLE)m_hFile, dwPos, 0, dwCount, 0);
}

void CFile::SetLength(DWORD dwNewLen)
{
	Seek((LONG)dwNewLen, (UINT)begin);

	::SetEndOfFile((HANDLE)m_hFile);
}

DWORD CFile::GetLength() const
{

	DWORD dwLen, dwCur;

	// Seek is a non const operation
	CFile* pFile = (CFile*)this;
	dwCur = pFile->Seek(0L, current);
	dwLen = pFile->Seek(0L,CFile::end);
	dwCur = (DWORD)pFile->Seek(dwCur, begin);

	return dwLen;
}

// CFile does not support direct buffering (CMemFile does)
UINT CFile::GetBufferPtr(UINT nCommand, UINT /*nCount*/,
	void** /*ppBufStart*/, void** /*ppBufMax*/)
{

	return 0;   // no support
}

void PASCAL CFile::Rename(LPCTSTR lpszOldName, LPCTSTR lpszNewName)
{
	::MoveFile((LPTSTR)lpszOldName, (LPTSTR)lpszNewName);
}
