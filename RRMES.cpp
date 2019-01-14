// RRMES.cpp: implementation of the CRRMES class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "RRMES.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRRMES::CRRMES()
{
	m_bEnabled = false;
	m_nStateAction = 0;
}

CRRMES::~CRRMES()
{

}


void CRRMES::RetrieveRegistryVariables()
{
	m_ControlPathToGauge =		theApp.GetProfileString(L"RRMES",L"mesControlPathToGauge",m_ControlPathToGauge.GetBuffer(1));
	m_AssociatedPathToGauge =	theApp.GetProfileString(L"RRMES",L"mesAssociatedPathToGauge",m_AssociatedPathToGauge.GetBuffer(1));
	m_MessagePathToMes =		theApp.GetProfileString(L"RRMES",L"mesMessagePathToMes",m_MessagePathToMes.GetBuffer(1));
	m_AssociatedPathToMes =		theApp.GetProfileString(L"RRMES",L"mesAssociatedPathToMes",m_AssociatedPathToMes.GetBuffer(1));
	m_strResourceName =			theApp.GetProfileString(L"RRMES",L"mesResourceName",m_strResourceName.GetBuffer(1));

	unsigned int nSize;
	unsigned char *pEnabledData = NULL;
	theApp.GetProfileBinary(L"RRMES",L"mesEnabled",(LPBYTE*)&pEnabledData,&nSize);
	CopyMemory(&m_bEnabled,pEnabledData,nSize);
	SAFE_DELETE_ARRAY( pEnabledData );

}


void CRRMES::SaveRegitryVariables()
{
	theApp.WriteProfileString(L"RRMES",L"mesControlPathToGauge",m_ControlPathToGauge.GetBuffer(1));
	theApp.WriteProfileString(L"RRMES",L"mesAssociatedPathToGauge",m_AssociatedPathToGauge.GetBuffer(1));
	theApp.WriteProfileString(L"RRMES",L"mesMessagePathToMes",m_MessagePathToMes.GetBuffer(1));
	theApp.WriteProfileString(L"RRMES",L"mesAssociatedPathToMes",m_AssociatedPathToMes.GetBuffer(1));
	theApp.WriteProfileString(L"RRMES",L"mesResourceName",m_strResourceName.GetBuffer(1));

	theApp.WriteProfileBinary(L"RRMES",L"mesEnabled",(unsigned char*)&m_bEnabled,sizeof m_bEnabled);
}

bool CRRMES::IsThereAMessageFromMES(CString &FileName)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = NULL;
	WCHAR FilePath[260];
	CString DateStamp,Temp;
	__int64 nDateStamp,nTemp;

	nDateStamp = 30001231999999;

	if(m_bEnabled != true) return false;
 
	swprintf_s(FilePath,260,_T("%s\\*.xml"), (LPCWSTR)m_ControlPathToGauge);
 
	if((hFind=FindFirstFile(FilePath, &ffd)) == INVALID_HANDLE_VALUE ) {
		return false;
	} else {
		do {
			Temp = ffd.cFileName;
			DateStamp = Temp.Right(18);
			DateStamp.GetBufferSetLength(14);
			nTemp = _wtoi64(DateStamp);
			if(nTemp < nDateStamp) {
				FileName = m_ControlPathToGauge + _T("\\") + ffd.cFileName;
				nDateStamp = nTemp;
			}
		} while( FindNextFile(hFind, &ffd)!= 0 );

		return true;
	}

}

bool CRRMES::FillFileNameList(int nFileType, CStringArray *pArray)
{
	WIN32_FIND_DATA  ffd;
	HANDLE hFind = NULL;
	WCHAR FilePath[260];

	pArray->RemoveAll();
	if(m_bEnabled != true) return false;
 
	switch(nFileType) {
	case 0: swprintf_s(FilePath,260,_T("%s\\*.xml"), (LPCWSTR)m_ControlPathToGauge);
		break;
	case 1: swprintf_s(FilePath,260,_T("%s\\*.xml"), (LPCWSTR)m_AssociatedPathToGauge);
		break;
	case 2: swprintf_s(FilePath,260,_T("%s\\*.xml"), (LPCWSTR)m_MessagePathToMes);
		break;
	case 3: swprintf_s(FilePath,260,_T("%s\\*.xml"),(LPCWSTR)m_AssociatedPathToMes);
		break;
	}
	
	hFind = FindFirstFile(FilePath, &ffd);
	if (hFind == INVALID_HANDLE_VALUE )	{
		return false;
	} else {
		do {
			pArray->Add(ffd.cFileName);

		} while( FindNextFile(hFind, &ffd)!= 0 );
 	}
	return true;
}

bool CRRMES::ReadMessageFromMES(CString FileName)
{
	CFileException e;
	CUSLFile File;
	CString Buff;

	if(!File.Open(FileName,CFile::modeReadWrite|CFile::typeBinary,&e)) {
		File.DecodeFileException(&e,&Buff,FileName);
		MessageBox(NULL,Buff,_T("Error"),MB_ICONERROR);
		return false;
	} else {
		while(File.ReadXMLLine(Buff)) {
			if(Buff.Find(_T("<Message Type="),0) > -1) {
				if(Buff.Find(_T("MES_2_OEM_OpStart"),0) > -1) {

				}
			}

			if(Buff.Find(_T("<MessageInfo "),0) > -1) {
				ExtractLabelsAndValues(Buff,13);
			}
		
		
		}

		File.Close();
		return true;
	}
}

void CRRMES::ExtractLabelsAndValues(CString &Buff, int nStart)
{
	CString Temp;
	int nStop;

	m_WIPLabels.RemoveAll();
	m_WIPValues.RemoveAll();

	for(int ii=nStart;ii<Buff.GetLength();ii++) {
		if((nStop = Buff.Find(_T("=\x022"),ii)) >= 0) {
			m_WIPLabels.Add(Buff.Mid(ii,nStop-ii));
			ii=nStop+2;
			nStop = Buff.Find(_T("\x022"),ii);
			m_WIPValues.Add(Buff.Mid(ii,nStop-ii));
			ii=nStop+1;
		} else {
			break;
		}
	}

}

bool CRRMES::GetWIPValue(CString Label, CString &Value)
{
	Value.Empty();
	for(int ii=0;ii<m_WIPLabels.GetSize();ii++) {
		if(m_WIPLabels.GetAt(ii) == Label) {
			Value = m_WIPValues.GetAt(ii);
			return true;
		}
	}
	return false;
}

bool CRRMES::StateMachine()
{
	CString FileName;
	if(m_bEnabled != true) return false;

	switch(m_nStateAction) {
	case 0:
		if(IsThereAMessageFromMES(FileName) == true) {
			ReadMessageFromMES(FileName);
			m_nStateAction = 10;
		}
		break;
	case 10:
		break;
	}

	return true;
}




