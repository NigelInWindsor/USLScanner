// USLFile.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "USLFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUSLFile

IMPLEMENT_DYNAMIC(CUSLFile, CFile)

CUSLFile::CUSLFile(LPCTSTR lpszFileName, UINT nOpenFlags )
{
	m_fVersion = 0.0f;
}


int CUSLFile::Open(LPCTSTR lpszFileName,UINT nOpenFlags,CFileException *pError)
{
	int nResult;
	CFileStatus status;
	CFileException e;

	if(pError == NULL) {
		pError = &e;
	}

	if((nResult=CFile::Open( lpszFileName, nOpenFlags, pError)) == 0) {
		if(pError->m_cause==CFileException::accessDenied) {
			CFile::GetStatus( lpszFileName, status );
			if((status.m_attribute==1)||(status.m_attribute==2)) {
				status.m_attribute = 0;
				CFile::SetStatus( lpszFileName, status );
				nResult=Open( lpszFileName, nOpenFlags, pError);
			}else{
				return 0;
			}
		}
	}

	return nResult;


}

int CUSLFile::ReadString(CString *pBuff)
{
	char *Array;
	int	nn=0;

	if(CFile::Read(&nn,sizeof nn)>0) {
		if((nn>=0) && (nn<=1000)) {
			Array = new char[nn+2];
			CFile::Read(Array,nn);
			Array[nn]=0;
			pBuff->Format(_T("%s"),Array);
			delete Array;
		}
	}
	return nn;


}

int CUSLFile::WriteString(CString *pBuff)
{
	int	nn = pBuff->GetLength();

	CFile::Write(&nn,sizeof nn);
	CFile::Write(pBuff->GetBuffer(nn),nn);
	return nn;

}

int CUSLFile::ReadStruct(void *pArray, int nMaxSize)
{
	int	nn=0;
	int	jj=sizeof (int);

	CFile::Read(&nn,sizeof (int));
	if(nn > 0) {
		if (nn > nMaxSize) nn = nMaxSize;
		CFile::Seek(jj * -1,CFile::current);
		CFile::Read(pArray,nn);
	};

	return nn;
}

int CUSLFile::WriteStruct(void *pArray, int nSize)
{
	CFile::Write(pArray,nSize);

	return nSize;
}

int CUSLFile::Write(void *pArray, int nSize)
{
	CFile::Write(pArray,nSize);

	return nSize;

}

int CUSLFile::Read(void *pArray, int nSize)
{
	int nn = CFile::Read(pArray,nSize);
	return nn;
}




int CUSLFile::GetAttributes(CString FileName)
{

	CFileStatus status;
	CFile::GetStatus( FileName, status);

	return status.m_attribute;

	

}

int CUSLFile::WriteUnicodeStringAsAscii(CString *pBuff)
{
	int nLength=pBuff->GetLength();
	char *str = new char[nLength * 4];
	
	int ii,jj;
	for(ii=0, jj=0;ii<nLength;jj++,ii++) {
		str[jj]=(char)pBuff->GetAt(ii);
		if(str[jj]=='\r') {
			str[++jj]='\n';
		}
	}
	str[jj]=0;
	CFile::Write(str,jj);

	delete str;
	return jj;
}

void CUSLFile::DecodeFileException(CFileException* pExcep, CString *pBuff,CString FileName)
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

int CUSLFile::ReadXMLLine(CString &Buff)
{
	CString Temp;
	char *pAscii = new char[4096];
	ULONGLONG nStart = CFile::GetPosition();
	int nLength = CFile::Read(pAscii,4096);
	int ii,jj;

	Buff.Empty();
	for(ii=0,jj=0;ii<nLength;ii++,jj++) {
		if((pAscii[ii] != '\r') && (pAscii[ii] != '\n')) {
			Temp.Format(_T("%c"),pAscii[ii]);
			Buff += Temp;
		} else {
			for(ii;ii<nLength;ii++) {
				if(isprint(pAscii[ii])) {
					goto finished;
				}
			}
		}
	}
finished:
	CFile::Seek(nStart + ii, CFile::begin);
	delete pAscii;
	return jj;

}

void CUSLFile::WriteStringChar(LPCTSTR lpsz)
{
	int nLength = wcslen(lpsz);
	char *str = new char[nLength + 2];

	int ii, jj;
	for (ii = 0, jj = 0; ii<nLength; jj++, ii++) {
		str[jj] = (char)lpsz[ii];
	}
	str[jj] = 0;
	Write(str, jj);

	delete str;
}

bool CUSLFile::ZeroBlockOfMemory(ULONGLONG n64StartOffset, unsigned int nSize)
{

	Seek(n64StartOffset, CFile::begin);

	char *pZeroBuffer = new char[nSize];
	ZeroMemory(pZeroBuffer, nSize);
	Write(pZeroBuffer, nSize);
	delete pZeroBuffer;

	Seek(n64StartOffset, CFile::begin);

	return true;
}

bool CUSLFile::IsAvailable(CString FileName)
{
	CFile File;
	CFileException e;

	if (!File.Open(FileName, CFile::modeReadWrite | CFile::modeNoTruncate, &e)) {
		return false;
	}
	else {
		ULONGLONG Length = File.GetLength();
		File.Close();
		if (Length <= 0) return false;
		return true;
	}
}
