// DataProperties.cpp: implementation of the CDataProperties class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "DataProperties.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CDataProperties::CDataProperties()
{
	wcscpy_s(m_wFileDescription,30, L"USL Data Properties");
	m_pnMode = NULL;
	Zero();
}

CDataProperties::~CDataProperties()
{
	SAFE_DELETE( m_pnMode );

}

void CDataProperties::Zero()
{
	m_nSize = 0;
	m_Caption[0].RemoveAll();
	m_Caption[1].RemoveAll();
	m_Value.RemoveAll();
	SAFE_DELETE( m_pnMode );
}

int CDataProperties::GetSize()
{
	return m_nSize;
}

bool CDataProperties::Save(CUSLFile *pFile)
{
	int nEmptyArray[1024];
	ZeroMemory(nEmptyArray,sizeof nEmptyArray);

	CArchive* pArchFile;
	CString Buff;
	int	ii,nLanguage;

	UINT64 n64StartPosition = pFile->GetPosition();
	UINT64 n64ArrayOffset = 0;

	pFile->Seek(n64StartPosition,CFile::begin);
	pFile->Write(nEmptyArray,sizeof nEmptyArray);
	pFile->Seek(n64StartPosition,CFile::begin);

	pFile->Write(m_wFileDescription,wcslen(m_wFileDescription) * sizeof WCHAR);
	pFile->Write(&theApp.m_fVersion, sizeof theApp.m_fVersion);
	pFile->Write(&n64ArrayOffset, sizeof n64ArrayOffset);
	pFile->Write(&m_nSize,sizeof m_nSize);


	Buff.Empty();

	pFile->Seek(0x200,CFile::end);
	n64ArrayOffset = pFile->GetPosition();
	pArchFile = new CArchive(pFile,CArchive::store);

	//write the first and second captions
	for(nLanguage = 0; nLanguage < 2; nLanguage++) {
		for(ii=0;ii<m_nSize;ii++) {
			if(ii<m_Caption[nLanguage].GetSize()) {
				pArchFile->WriteString(m_Caption[nLanguage].GetAt(ii)+L"\r\n");
			} else {
				pArchFile->WriteString(Buff+L"\r\n");
			}
		}
	}
	//Write values
	for(ii=0;ii<m_nSize;ii++) {
		if(ii<m_Value.GetSize()) {
			pArchFile->WriteString(m_Value.GetAt(ii)+L"\r\n");
		} else {
			pArchFile->WriteString(Buff+L"\r\n");
		}
	}

	pArchFile->Close();
	delete pArchFile;

	if(m_pnMode) {
		pFile->Write(m_pnMode,m_nSize * sizeof (int));
	}

	pFile->Seek(n64StartPosition,CFile::begin);
	pFile->Write(m_wFileDescription,wcslen(m_wFileDescription) * sizeof WCHAR);
	pFile->Write(&theApp.m_fVersion, sizeof theApp.m_fVersion);
	pFile->Write(&n64ArrayOffset, sizeof n64ArrayOffset);


	pFile->SeekToEnd();

	return true;
}

void CDataProperties::Retrieve(CUSLFile *pFile)
{
	CArchive* pArchFile;
	CString Buff;
	int	ii,nLanguage;
	float fVersion;
	WCHAR	wDescription[30];

	ZeroMemory(wDescription,sizeof wDescription);
	Zero();

	UINT64 n64ArrayOffset;

	pFile->Read(wDescription,wcslen(m_wFileDescription) * sizeof WCHAR);

	if(wcscmp(wDescription, m_wFileDescription) == 0) {
		pFile->Read(&fVersion, sizeof fVersion);
		pFile->Read(&n64ArrayOffset, sizeof n64ArrayOffset);
		pFile->Read(&m_nSize,sizeof m_nSize);


		if((n64ArrayOffset>0) && (m_nSize > 0)) {
			pFile->Seek(n64ArrayOffset,CFile::begin);

			pArchFile = new CArchive(pFile,CArchive::load);
			for(nLanguage=0;nLanguage<2;nLanguage++) {
				for(ii=0;ii<m_nSize;ii++) {
					pArchFile->ReadString(Buff);
					m_Caption[nLanguage].Add(Buff);
				}
			}
			for(ii=0;ii<m_nSize;ii++) {
				pArchFile->ReadString(Buff);
				m_Value.Add(Buff);
			}
			pArchFile->Close();
			delete pArchFile;

			if(m_nSize > 0) {
				m_pnMode = new int[m_nSize];
				pFile->Read(m_pnMode,m_nSize * sizeof (int));
			}
		}
	}
}



void CDataProperties::operator = (CDataProperties* pProperties)
{
	int ii;
	Zero();

	m_nSize = pProperties->m_nSize;
	for(ii=0;ii<pProperties->m_Caption[0].GetSize();ii++) m_Caption[0].Add( pProperties->m_Caption[0].GetAt(ii));
	for(ii=0;ii<pProperties->m_Caption[1].GetSize();ii++) m_Caption[1].Add( pProperties->m_Caption[0].GetAt(ii));
	for(ii=0;ii<pProperties->m_Value.GetSize();ii++) m_Value.Add( pProperties->m_Value.GetAt(ii));
	if(m_nSize > 0) {
		m_pnMode = new int[m_nSize];
		ZeroMemory(m_pnMode, m_nSize * sizeof(int));
		if((pProperties->m_pnMode) && (pProperties->m_nSize > 0)) {
			CopyMemory(m_pnMode, pProperties->m_pnMode, m_nSize * sizeof(int));
		}
	}
}

void CDataProperties::Add(int nIndex)
{
	CString Buff;
	Buff.Empty();

	if((nIndex == -1) || (nIndex >= m_nSize) || (m_nSize==0)) {
		m_Caption[0].Add(Buff);
		m_Caption[1].Add(Buff);
		m_Value.Add(Buff);
		m_nSize = m_Caption[0].GetSize();
		nIndex = m_nSize -1;
	} else {
		m_Caption[0].InsertAt(nIndex,Buff);
		m_Caption[1].InsertAt(nIndex,Buff);
		m_Value.InsertAt(nIndex,Buff);
		m_nSize = m_Caption[0].GetSize();
	}

	if(m_pnMode == NULL) {
		m_pnMode = new int[m_nSize];
		ZeroMemory(m_pnMode, sizeof (int) * m_nSize);
	} else {
		m_pnMode = (int *)realloc(m_pnMode,m_nSize * sizeof (int));
		m_pnMode[nIndex]=0;
	}


}

CString CDataProperties::GetCaptionAt(int nLanguage, int nIndex)
{
	if((nIndex>=0) && (nIndex<m_Caption[nLanguage].GetSize())) {
		return m_Caption[nLanguage].GetAt(nIndex);
	}
	return L"";
}

CString CDataProperties::GetValueAt(int nIndex)
{
	if((nIndex>=0) && (nIndex<m_Value.GetSize())) {
		return m_Value.GetAt(nIndex);
	}
	return L"";

}

bool CDataProperties::SetCaptionAt(int nLanguage, int nIndex, CString Buff)
{

	if(nIndex>=m_Caption[nLanguage].GetSize()) {
		m_Caption[nLanguage].InsertAt(nIndex,Buff);
		return true;
	}

	m_Caption[nLanguage].SetAt(nIndex, Buff);

	return true;
}


void CDataProperties::SetValueAt(int nIndex, CString Buff)
{
	if(nIndex>=m_Value.GetSize()) {
		m_Value.InsertAt(nIndex,Buff);
	}

	m_Value.SetAt(nIndex, Buff);


}

CString CDataProperties::GetReportFromID(int nIndex)
{
	int nColumn = nIndex % 3;
	int nRow = nIndex / 3;

	switch(nColumn) {
	case 0: return GetCaptionAt(0,nRow);
		break;
	case 1: return GetCaptionAt(1,nRow);
		break;
	case 2: return GetValueAt(nRow);
		break;
	}

	return L"";

}

int CDataProperties::GetModeAt(int nIndex)
{

	if( m_pnMode && (nIndex < m_nSize)) {
		return m_pnMode[nIndex];
	}
	return -1;
}

void CDataProperties::SetModeAt(int nIndex,int nMode)
{
	if(nIndex == -1) {
		SAFE_DELETE(m_pnMode);
		m_pnMode = new int[1];
		m_pnMode[0] = nMode;
		return;
	}
	if(nIndex >= m_nSize) {
		Add(nIndex);
	}
	m_pnMode[nIndex] = nMode;

}




bool CDataProperties::IsAllEntriesComplete()
{
	CString Buff;

	for(int ii=0;ii<m_nSize;ii++) {
		Buff = GetValueAt(ii);
		if(Buff.IsEmpty() == TRUE) return false;
	}
	return true;
}
