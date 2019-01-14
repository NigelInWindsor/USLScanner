// CArchive64.cpp: implementation of the CArchive6464 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxtempl.h>
#include "USLScanner.h"
#include "CArchive64.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DELETE_EXCEPTION(e) do { e->Delete(); } while (0)


CArchive64::CArchive64(CFile64* pFile, UINT nMode, int nBufSize, void* lpBuf) 
{
	m_pFile = pFile;
	m_nMode = nMode;
	m_bNoFlushOnDelete = FALSE;
}

CArchive64::~CArchive64()
{
	if (m_pFile != NULL)
		Close();
}

void CArchive64::Close()
{
	ASSERT_VALID(m_pFile);

	Flush();
	m_pFile = NULL;
}

void CArchive64::Flush()
{
	ASSERT_VALID(m_pFile);

	if(m_nMode & CArchive::store) m_pFile->Flush();
}

void CArchive64::WriteString(LPCTSTR lpsz)
{
	ASSERT(AfxIsValidString(lpsz));
	m_pFile->Write(lpsz, lstrlen(lpsz) * sizeof(TCHAR));
}

void CArchive64::WriteStringChar(LPCTSTR lpsz)
{
	ASSERT(AfxIsValidString(lpsz));
	char *Array = new char[wcslen(lpsz)+2];
	int ii;

	for(ii=0;ii<lstrlen(lpsz);ii++) {
		Array[ii] = (char)lpsz[ii];
	}
	Array[ii]=0;
	m_pFile->Write(Array, lstrlen(lpsz));
	delete Array;
}

LPTSTR CArchive64::ReadString(LPTSTR lpsz, UINT nMax)
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
			m_pFile->Read(&ch,sizeof ch);

			// stop and end-of-line (trailing '\n' is ignored)
			if (ch == '\n' || ch == '\r')
			{
				if (ch == '\r')
					m_pFile->Read(&ch,sizeof ch);
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

BOOL CArchive64::ReadString(CString& rString)
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

/////////////////////////////////////////////////////////////////////////////
