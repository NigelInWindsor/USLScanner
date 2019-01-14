#include "stdafx.h"
#include "cfile64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////// Implementation //////////////////////////////

IMPLEMENT_DYNAMIC(CFile64, CObject)

CFile64::CFile64()
{
  m_hFile = INVALID_HANDLE_VALUE;
  m_sFilename.Empty();
  m_bAutoClose = FALSE;   
}

CFile64::CFile64(HANDLE hFile, BOOL bAutoClose)
{
  Attach(hFile, bAutoClose);
}

CFile64::~CFile64()
{
  Detach();
}

CFile64::operator HANDLE()
{
  return m_hFile;
}

BOOL CFile64::Open(LPCTSTR lpFileName,
                   DWORD dwDesiredAccess,
                   DWORD dwShareMode,
                   DWORD dwCreationDistribution,
                   CFileException* pError,
                   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                   DWORD dwFlagsAndAttributes,
                   HANDLE hTemplateFile)
{
	ASSERT(AfxIsValidString(lpFileName));
	ASSERT(pError == NULL || AfxIsValidAddress(pError, sizeof(CFileException)));


  HANDLE hFile = CreateFile(lpFileName,	dwDesiredAccess, dwShareMode,	
                            lpSecurityAttributes,	dwCreationDistribution,	
                            dwFlagsAndAttributes,	hTemplateFile);

  BOOL bSuccess = FALSE;
  if (hFile != INVALID_HANDLE_VALUE)
  {
    bSuccess = TRUE;
    m_hFile = hFile;
    m_sFilename = lpFileName;
    m_bAutoClose = TRUE;   
  }
  else
  {
		if (pError != NULL)
		{
			pError->m_lOsError = ::GetLastError();
			pError->m_cause =	CFileException::OsErrorToException(pError->m_lOsError);

			// use passed file name (not expanded vesion) when reporting
			// an error while opening
			pError->m_strFileName = lpFileName;
    }
  }

  return bSuccess;
}

inline BOOL CFile64::IsOpen() const
{
  return (m_hFile != INVALID_HANDLE_VALUE);
}

inline BOOL CFile64::IsClosed() const
{
  return (m_hFile == INVALID_HANDLE_VALUE);
}

void CFile64::Attach(HANDLE hFile, BOOL bAutoClose)
{
  m_hFile = hFile;
  m_sFilename.Empty(); //Currently Win32 provides no means of
                       //going from a File Handle back to a filename
  m_bAutoClose = bAutoClose;   
}

void CFile64::Detach()
{
  if (IsOpen() && m_bAutoClose)
    Close();
  else
  {
  	m_hFile = INVALID_HANDLE_VALUE;
	  m_bAutoClose = FALSE;
  	m_sFilename.Empty();
  }
}

void CFile64::Close()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	BOOL bError = FALSE;
	if (IsOpen())
		bError = !::CloseHandle(m_hFile);

	m_hFile = INVALID_HANDLE_VALUE;
	m_bAutoClose = FALSE;
	m_sFilename.Empty();

	if (bError)
		CFileException::ThrowOsError((LONG)::GetLastError());
}

void CFile64::Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite)
{
 	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lpBuffer != NULL);
	ASSERT(AfxIsValidAddress(lpBuffer, dwNumberOfBytesToWrite, FALSE));

	DWORD nWritten;
	if (!::WriteFile(m_hFile, lpBuffer, dwNumberOfBytesToWrite, &nWritten, NULL))
		CFileException::ThrowOsError((LONG)::GetLastError(), m_sFilename);

	if (nWritten != dwNumberOfBytesToWrite)
		AfxThrowFileException(CFileException::diskFull, -1, m_sFilename);
}

DWORD CFile64::Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lpBuffer != NULL);
	ASSERT(AfxIsValidAddress(lpBuffer, dwNumberOfBytesToRead, TRUE));

	DWORD dwRead;
	if (!::ReadFile(m_hFile, lpBuffer, dwNumberOfBytesToRead, &dwRead, NULL))
		CFileException::ThrowOsError((LONG)::GetLastError());

	return dwRead;
}

UINT64 CFile64::MakeUnsignedInt64(DWORD nHigh, DWORD nLow)
{
  return ((((UINT64) nHigh) << 32) | nLow);
}

void CFile64::SplitUnsignedInt64(const UINT64& nBigInt, DWORD& nHigh, DWORD& nLow)
{
  nHigh = (DWORD) ((nBigInt & 0xFFFFFFFF00000000) >> 32);
  nLow = (DWORD) (nBigInt & 0x00000000FFFFFFFF);
}

UINT64 CFile64::Seek(const UINT64& lDistanceToMove, SeekPosition MoveMethod, BOOL bForward)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

  UINT64 nNewPos;
  switch (MoveMethod)
  {
    case begin:   
    {
      nNewPos = lDistanceToMove; 
      break;
    }
    case current: //deliberate fall through
    {
      if (bForward)
        nNewPos = GetPosition() + lDistanceToMove; 
      else
        nNewPos = GetPosition() - lDistanceToMove;
      break;
    }
    case end:     
    {
      if (bForward)
        nNewPos = GetLength() + lDistanceToMove; 
      else
        nNewPos = GetLength() - lDistanceToMove;
      break;
    }
    default:      
    {
      ASSERT(FALSE);        
      break;
    }
  }
  DWORD dwDistanceToMoveLow;
  DWORD dwDistanceToMoveHigh;
  SplitUnsignedInt64(nNewPos, dwDistanceToMoveHigh, dwDistanceToMoveLow);

  //WINBUG: What if dwDistanceToMoveLow is > LONG_MAX, then SetFilePointer 
  //will interpret that as a negative value
	DWORD dwNewLow = ::SetFilePointer(m_hFile, dwDistanceToMoveLow, (LONG*) &dwDistanceToMoveHigh, FILE_BEGIN);
	if (dwNewLow  == (DWORD)-1)
		CFileException::ThrowOsError((LONG)::GetLastError());

	return MakeUnsignedInt64(dwDistanceToMoveHigh, dwNewLow);
}

UINT64 CFile64::SeekToEnd()
{
  return Seek(0, end, FALSE);
}

void CFile64::SeekToBegin()
{
  Seek(0, begin, TRUE);
}

void CFile64::SetLength(const UINT64& lNewLen)
{
 	ASSERT_VALID(this);
	ASSERT(IsOpen());

	Seek(lNewLen, begin, TRUE);

	if (!::SetEndOfFile(m_hFile))
		CFileException::ThrowOsError((LONG)::GetLastError());
}

UINT64 CFile64::GetLength() const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

  DWORD dwHighLength;
  DWORD dwLowLength = GetFileSize(m_hFile, &dwHighLength);
	if (dwLowLength  == (DWORD)-1)
		CFileException::ThrowOsError((LONG)::GetLastError());  

	return MakeUnsignedInt64(dwHighLength, dwLowLength);
}

UINT64 CFile64::GetPosition() const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

  DWORD dwHighPos = 0;
	DWORD dwLowPos = ::SetFilePointer(m_hFile, 0, (LONG*) &dwHighPos, FILE_CURRENT);
	if (dwLowPos  == (DWORD)-1)
		CFileException::ThrowOsError((LONG)::GetLastError());

	return MakeUnsignedInt64(dwHighPos, dwLowPos);
}

void CFile64::Flush()
{
	ASSERT_VALID(this);

	if (IsClosed())
		return;

	if (!::FlushFileBuffers(m_hFile))
		CFileException::ThrowOsError((LONG)::GetLastError());
}

CFile64* CFile64::Duplicate() const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	CFile64* pFile = new CFile64();
	HANDLE hFile;
  HANDLE hProcess = GetCurrentProcess();
	if (!::DuplicateHandle(hProcess, m_hFile, hProcess, &hFile, 0, FALSE, DUPLICATE_SAME_ACCESS))
	{
		delete pFile;
		CFileException::ThrowOsError((LONG)::GetLastError());
	}
	pFile->m_hFile = hFile;
  pFile->m_sFilename = m_sFilename;
	ASSERT(pFile->IsOpen());
	pFile->m_bAutoClose = m_bAutoClose;
	return pFile;
}

void CFile64::Abort()
{
 	ASSERT_VALID(this);
	if (IsOpen())
	{
		// close but ignore errors
		::CloseHandle(m_hFile);
  	m_hFile = INVALID_HANDLE_VALUE;
	  m_bAutoClose = FALSE;
	  m_sFilename.Empty();
	}
}

void CFile64::LockRange(const UINT64& lPos, const UINT64& lCount)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

  DWORD dwFileOffsetLow;
  DWORD dwFileOffsetHigh;
  SplitUnsignedInt64(lPos, dwFileOffsetHigh, dwFileOffsetLow);
  DWORD nNumberOfBytesToLockLow;
  DWORD nNumberOfBytesToLockHigh;
  SplitUnsignedInt64(lCount, nNumberOfBytesToLockHigh, nNumberOfBytesToLockLow);

	if (!::LockFile(m_hFile, dwFileOffsetLow, dwFileOffsetHigh, nNumberOfBytesToLockLow, nNumberOfBytesToLockHigh))
		CFileException::ThrowOsError((LONG)::GetLastError());
}

void CFile64::UnlockRange(const UINT64& lPos, const UINT64& lCount)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

  DWORD dwFileOffsetLow;
  DWORD dwFileOffsetHigh;
  SplitUnsignedInt64(lPos, dwFileOffsetHigh, dwFileOffsetLow);
  DWORD nNumberOfBytesToLockLow;
  DWORD nNumberOfBytesToLockHigh;
  SplitUnsignedInt64(lCount, nNumberOfBytesToLockHigh, nNumberOfBytesToLockLow);

	if (!::UnlockFile(m_hFile, dwFileOffsetLow, dwFileOffsetHigh, nNumberOfBytesToLockLow, nNumberOfBytesToLockHigh))
		CFileException::ThrowOsError((LONG)::GetLastError());
}

DWORD CFile64::GetFileType()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

  return ::GetFileType(m_hFile);
}

#ifdef _DEBUG
void CFile64::AssertValid() const
{
	CObject::AssertValid();
}

void CFile64::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << "with handle " << m_hFile;
	dc << " and name \"" << m_sFilename << "\"";
	dc << "\n";
}
#endif

CString CFile64::GetFileName() const
{
	ASSERT_VALID(this);
	int pos;
	CString Buff = m_sFilename;
	if((pos = m_sFilename.ReverseFind('\\')) > 0) {
		Buff = m_sFilename.Right(m_sFilename.GetLength() - pos - 1);
	}
	return Buff;
}

CString CFile64::GetFilePath() const
{
	ASSERT_VALID(this);
	return m_sFilename;
}

void CFile64::SetFileName(LPCTSTR lpszNewName)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidString(lpszNewName, FALSE));
	m_sFilename = lpszNewName;
}

void CFile64::DecodeFileException(CFileException* pExcep, CString *pBuff,CString FileName)
{

	pBuff->Format(_T("File opening Error message not recognised"));
	if(pExcep->m_cause==CFileException::fileNotFound) pBuff->Format(_T("The file %s could not be located"),FileName);
	if(pExcep->m_cause==CFileException::badPath) pBuff->Format(_T("All or part of the path %s is invalid"),FileName);
	if(pExcep->m_cause==CFileException::tooManyOpenFiles) pBuff->Format(_T("The permitted number of open files was exceeded"));
	if(pExcep->m_cause==CFileException::accessDenied) pBuff->Format(_T("The file %s could not be accessed\r\nProbably read only"),FileName);
	if(pExcep->m_cause==CFileException::invalidFile) pBuff->Format(_T("There was an attempt to use an invalid file handle"));
	if(pExcep->m_cause==CFileException::removeCurrentDir) pBuff->Format(_T("The current working directory cannot be removed"));
	if(pExcep->m_cause==CFileException::directoryFull) pBuff->Format(_T("There are no more directory entries"));
	if(pExcep->m_cause==CFileException::badSeek) pBuff->Format(_T("There was an error trying to set the file pointer"));
	if(pExcep->m_cause==CFileException::hardIO) pBuff->Format(_T("There was a hardware error"));
	if(pExcep->m_cause==CFileException::sharingViolation) pBuff->Format(_T("SHARE.EXE was not loaded, or a shared region was locked"));
	if(pExcep->m_cause==CFileException::lockViolation) pBuff->Format(_T("There was an attempt to lock a region that was already locked"));
	if(pExcep->m_cause==CFileException::diskFull) pBuff->Format(_T("The disk is full"));
	if(pExcep->m_cause==CFileException::endOfFile) pBuff->Format(_T("End of file"));


}

int CFile64::ReadStruct(void *pArray)
{
	int	nn=0;
	int	jj=sizeof (int);

	Read(&nn,sizeof (int));
	if(nn > 0) {
		Seek(jj * -1,CFile64::current,TRUE);
		Read(pArray,nn);
	};

	return nn;
}

int CFile64::WriteStruct(void *pArray, int nSize)
{
	Write(pArray,nSize);

	return nSize;
}

void CFile64::WriteString(LPCTSTR lpsz)
{
	ASSERT(AfxIsValidString(lpsz));
	Write(lpsz, lstrlen(lpsz) * sizeof(TCHAR));
}

void CFile64::WriteStringChar(LPCTSTR lpsz)
{
	int nLength=wcslen(lpsz);
	char *str = new char[nLength + 2];
	
	int ii,jj;
	for(ii=0, jj=0;ii<nLength;jj++,ii++) {
		str[jj]=(char)lpsz[ii];
	}
	str[jj]=0;
	Write(str,jj);

	delete str;
}


int CFile64::WriteUnicodeStringAsAscii(CString *pBuff)
{
	int nLength=pBuff->GetLength();
	char *str = new char[nLength + 2];
	
	int ii,jj;
	for(ii=0, jj=0;ii<nLength;jj++,ii++) {
		str[jj]=(char)pBuff->GetAt(ii);
	}
	str[jj]=0;
	Write(str,jj);

	delete str;
	return jj;
}

#define DELETE_EXCEPTION(e) do { e->Delete(); } while (0)

LPTSTR CFile64::ReadString(LPTSTR lpsz, UINT nMax)
{
	// if nMax is negative (such a large number doesn't make sense given today's
	// 2gb address space), then assume it to mean "keep the newline".
	int nStop = (int)nMax < 0 ? -(int)nMax : (int)nMax;
	ASSERT(AfxIsValidAddress(lpsz, (nStop+1) * sizeof(TCHAR)));

	_TUCHAR ch;
	int nRead = 0;

	TRY
	{
		while (nRead < nStop)
		{
			Read(&ch,sizeof ch);

			// stop and end-of-line (trailing '\n' is ignored)
			if (ch == '\n' || ch == '\r')
			{
				if (ch == '\r')
					Read(&ch,sizeof ch);
				// store the newline when called with negative nMax
				if ((int)nMax != nStop)
					lpsz[nRead++] = ch;
				break;
			}
			lpsz[nRead++] = ch;
		}
	}
	CATCH(CArchiveException, e)
	{
		if (e->m_cause == CArchiveException::endOfFile)
		{
			DELETE_EXCEPTION(e);
			if (nRead == 0)
				return NULL;
		}
		else
		{
			THROW_LAST();
		}
	}
	END_CATCH

	lpsz[nRead] = '\0';
	return lpsz;
}


BOOL CFile64::ReadString(CString& rString)
{
	rString.Empty();    // empty string without deallocating
	const int nMaxSize = 128;
	LPTSTR lpsz = rString.GetBuffer(nMaxSize);
	LPTSTR lpszResult;
	int nLen;
	for (;;)
	{
		lpszResult = ReadString(lpsz, (UINT)-nMaxSize); // store the newline
		rString.ReleaseBuffer();

		// if string is read completely or EOF
		if (lpszResult == NULL ||
			(nLen = lstrlen(lpsz)) < nMaxSize ||
			lpsz[nLen-1] == '\n')
		{
			break;
		}

		nLen = rString.GetLength();
		lpsz = rString.GetBuffer(nMaxSize + nLen) + nLen;
	}

	// remove '\n' from end of string if present
	lpsz = rString.GetBuffer(0);
	nLen = rString.GetLength();
	if (nLen != 0 && lpsz[nLen-1] == '\n')
		rString.GetBufferSetLength(nLen-1);

	return lpszResult != NULL;
}


bool CFile64::ZeroBlockOfMemory(UINT64 n64StartOffset,unsigned int nSize)
{

	Seek(n64StartOffset,CFile64::begin,TRUE);

	char *pZeroBuffer = new char[nSize];
	ZeroMemory(pZeroBuffer, nSize);
	Write(pZeroBuffer,nSize);
	delete pZeroBuffer;

	Seek(n64StartOffset,CFile64::begin,TRUE);

	return true;
}
