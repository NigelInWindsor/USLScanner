// Users.cpp: implementation of the CUsers class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "Users.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUsers::CUsers()
{
	m_pID = NULL;
	m_nIDL = 0;
	Retrieve();
}

CUsers::~CUsers()
{
	Zero();
}

void CUsers::Zero()
{
	if(m_pID) delete m_pID;
	m_pID=NULL;
	m_nIDL = 0;
}

bool CUsers::Retrieve()
{
	
	CUSLFile File;
	CFileException e;
	int	nFileType,nCount,nn;
	CString FilePath,Buff,Field;
	FilePath = theApp.m_LastSettingsPath + L"\\access.bin";
	IDStruct ID;


	if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(NULL,Buff,_T("Error"),MB_ICONERROR);
		return FALSE;
	} else {
		Zero();

		File.Seek(0x100,CFile::begin);

		File.Read(&nFileType, sizeof nFileType);

		if(nFileType == 0) { ///ascii 
			CArchive ar(&File,CArchive::load);
			while(ar.ReadString(Buff)) {
				if(Buff.Find(_T("USER\t"))==0) {
					ZeroMemory(&ID,sizeof ID);
					ExtractToken(ID.wName,Buff);
				}
				if(Buff.Find(_T("\tPASSWORD\t"))==0) {
					ExtractToken(ID.wPassword,Buff);
				}
				if(Buff.Find(_T("\tID\t\t"))==0) {
					ExtractToken(ID.wID,Buff);
				}
				if(Buff.Find(_T("\tACCESS\t\tAny One"))==0)			ID.nAccessLevel=0;
				if(Buff.Find(_T("\tACCESS\t\tOperator"))==0)		ID.nAccessLevel=1;
				if(Buff.Find(_T("\tACCESS\t\tLevel 1"))==0)			ID.nAccessLevel=2;
				if(Buff.Find(_T("\tACCESS\t\tLevel 2"))==0)			ID.nAccessLevel=3;
				if(Buff.Find(_T("\tACCESS\t\tLevel 3"))==0)			ID.nAccessLevel=4;
				if(Buff.Find(_T("\tACCESS\t\tSupervisor"))==0)		ID.nAccessLevel=5;
				if(Buff.Find(_T("\tACCESS\t\tManagement"))==0)		ID.nAccessLevel=6;
				if(Buff.Find(_T("\tACCESS\t\tMaintenance"))==0)		ID.nAccessLevel=7;
				if(Buff.Find(_T("\tACCESS\t\tManufacturer"))==0)	ID.nAccessLevel=8;

				if(Buff.Find(_T("END"))==0) {
					Add(&ID);
					ZeroMemory(&ID,sizeof ID);
				}
			}
			ar.Close();
		} else {
			File.Read(&nCount, sizeof nCount);

			for(nn=0;nn<nCount;nn++) {
				File.Read(&ID, sizeof ID);
				Add(&ID);
			}
		}


		File.Close();
	}

	return TRUE;
}

bool CUsers::Save()
{
	
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff ;
	FilePath = theApp.m_LastSettingsPath + L"\\access.bin";
	char cArray[256];
	int nFileType=1;
	int	nn;

	if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(NULL,Buff,_T("Warning"), MB_ICONERROR);
		return FALSE;
	} else {

		ZeroMemory(cArray,sizeof cArray);
		File.Write(cArray, sizeof cArray);
		File.Seek(0x100,CFile::begin);

		File.Write(&nFileType,sizeof nFileType);

		if(nFileType==0) {				//Save as ascii readable
			CArchive arWrite(&File,CArchive::store);

			for(nn=0;nn<m_nIDL;nn++) {
				Buff.Format(_T("USER\t%s\r\n"),m_pID[nn].wName);
				arWrite.WriteString(Buff);

				Buff.Format(_T("\tPASSWORD\t%s\r\n"),m_pID[nn].wPassword);
				arWrite.WriteString(Buff);

				Buff.Format(_T("\tID\t\t%s\r\n"),m_pID[nn].wID);
				arWrite.WriteString(Buff);

				switch(m_pID[nn].nAccessLevel) {
				case 0:		Buff.Format(_T("\tACCESS\t\tAny One\r\n"));
					break;
				case 1:		Buff.Format(_T("\tACCESS\t\tOperator\r\n"));
					break;
				case 2:		Buff.Format(_T("\tACCESS\t\tLevel 1\r\n"));
					break;
				case 3:		Buff.Format(_T("\tACCESS\t\tLevel 2\r\n"));
					break;
				case 4:		Buff.Format(_T("\tACCESS\t\tLevel 3\r\n"));
					break;
				case 5:		Buff.Format(_T("\tACCESS\t\tSupervisor\r\n"));
					break;
				case 6:		Buff.Format(_T("\tACCESS\t\tManagement\r\n"));
					break;
				case 7:		Buff.Format(_T("\tACCESS\t\tMaintenance\r\n"));
					break;
				case 8:		Buff.Format(_T("\tACCESS\t\tManufacturer\r\n"));
					break;
				}
				arWrite.WriteString(Buff);

				arWrite.WriteString(_T("END\r\n"));
			}
			arWrite.Close();
		} else {
			File.Write(&m_nIDL, sizeof m_nIDL);
			for(nn=0;nn<m_nIDL;nn++) {
				File.Write(&m_pID[nn], sizeof IDStruct);
			}
		}


		File.Close();
	}

	return TRUE;
}

void CUsers::AddEmpty()
{
	IDStruct* pIDTemp;

	if(m_nIDL==0) {
		m_nIDL=1;
		m_pID = new IDStruct[m_nIDL];
		ZeroMemory(m_pID,(sizeof IDStruct) * m_nIDL);
	} else {
		pIDTemp = new IDStruct[m_nIDL];
		CopyMemory(pIDTemp,m_pID,(sizeof IDStruct) * m_nIDL);
		delete m_pID;
		m_nIDL++;
		m_pID = new IDStruct[m_nIDL];
		ZeroMemory(m_pID,(sizeof IDStruct) * m_nIDL);
		CopyMemory(m_pID,pIDTemp,(sizeof IDStruct) * (m_nIDL-1));
		delete pIDTemp;
	}
}

void CUsers::Add(IDStruct *pID)
{
	AddEmpty();
	m_pID[m_nIDL-1] = *pID;
}

void CUsers::ExtractToken(WCHAR *pwStr, CString Buff)
{
	int nLength = Buff.GetLength();
	int nn,ii;

	for(nn=0, ii=0 ;nn<nLength;nn++) {
		if(Buff.GetAt(nn) != '\t') {
			pwStr[ii++] = Buff.GetAt(nn);
			pwStr[ii] = 0;
		}
	}

}

bool CUsers::Logon(CString Name, CString Password)
{
	

	if(m_nIDL==0) return TRUE;

	//Backdoor
	if((wcscmp(L"USL",Name)==0) && (wcscmp(L"0347",Password)==0)) {
		theApp.m_nLogonLevelMask = 0x80;
		theApp.m_nLogonLevel = 8;
		theApp.m_Operator = Name;
		FillMemory(&theApp.m_cAccess,sizeof theApp.m_cAccess, 0xff);
		return TRUE;
	}

	for(int nn=0;nn<m_nIDL;nn++) {
		if((wcscmp(m_pID[nn].wName,Name)==0) && (wcscmp(m_pID[nn].wPassword,Password)==0)) {
			theApp.m_nLogonLevel = m_pID[nn].nAccessLevel;
			theApp.m_nLogonLevelMask=0;
			if(theApp.m_nLogonLevel) {
				theApp.m_nLogonLevelMask = 1;
				theApp.m_nLogonLevelMask <<= (theApp.m_nLogonLevel - 1);
			}
			theApp.m_Operator = m_pID[nn].wName;
			return TRUE;
		}
	}

	return FALSE;
}
