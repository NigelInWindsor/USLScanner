// Registry.cpp: implementation of the CRegistry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "Registry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRegistry::CRegistry()
{
	if((m_hSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS)) == NULL) {
		MessageBox(NULL,_T("Could not open Service Control Manager"),_T("Failure"), MB_ICONERROR);
	}

}

CRegistry::CRegistry(CString	ProgramName)
{
	CString	Buff;
	
	m_hSCManager = NULL;
	m_ProgramName = ProgramName;
	m_SoftwareKey = L"SOFTWARE\\USL\\" + ProgramName;

//	CreateSoftwareKey();
//	CreateString(_T("LastSettings"),m_ptheApp.m_LastSettingsPath);
//	CreateHardwareKey(_T("USL"));

}

CRegistry::~CRegistry()
{
	if( m_hSCManager!=NULL)	CloseServiceHandle(m_hSCManager);
	m_hSCManager = NULL;

}


void CRegistry::CreateSoftwareKey()
{
	CRegKey	RegKey;
	CRegKey SubKey;
	CString Buff;

	//Check for USL
	if(RegKey.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\USL"),KEY_READ)) {
		if(!RegKey.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE"),KEY_READ)) {
			SubKey.Create(RegKey.m_hKey,_T("USL"),REG_NONE,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS);
			SubKey.Close();
			RegKey.Close();
		}
	}
	//Check for Program
	if(RegKey.Open(HKEY_LOCAL_MACHINE,m_SoftwareKey,KEY_READ)) {
		if(!RegKey.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\USL"),KEY_READ)) {
			SubKey.Create(RegKey.m_hKey,m_ProgramName,REG_NONE,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS);
			SubKey.Close();
			RegKey.Close();
		}
	}
	Buff.Format(_T("%s\\Cards"),m_SoftwareKey);
	if(RegKey.Open(HKEY_LOCAL_MACHINE,Buff,KEY_READ)) {
		if(!RegKey.Open(HKEY_LOCAL_MACHINE,m_SoftwareKey,KEY_READ)) {
			SubKey.Create(RegKey.m_hKey,_T("Cards"),REG_NONE,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS);
			SubKey.Close();
			RegKey.Close();
		}
	}
}

void CRegistry::CreateHardwareKey(CString Name)
{
	return;
	CString	Key = L"SYSTEM\\CurrentControlSet\\Services\\" + Name;
	CRegKey	RegKey;
	CRegKey SubKey;
	int nErrorCode;

	if(RegKey.Open(HKEY_LOCAL_MACHINE,Key,KEY_READ)) {
		if(!RegKey.Open(HKEY_LOCAL_MACHINE,_T("SYSTEM\\CurrentControlSet\\Services"),KEY_READ)) {
			if(nErrorCode=SubKey.Create(RegKey.m_hKey,Name,REG_NONE,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS)) {
				ReportError(nErrorCode);
			} else {
				RegKey.SetKeyValue(Name,Name,_T("DisplayName"));
				SubKey.SetDWORDValue(_T("ErrorControl"), 1);
				SubKey.SetDWORDValue(_T("Start"), 2);
				SubKey.SetDWORDValue(_T("Type"), 1);
				SubKey.Close();
				RegKey.Close();
			}
		}
	}

}

void CRegistry::CreateString(CString String, CString Value)
{
	CRegKey	RegKey;
	CRegKey SubKey;
	CString	Buff = m_SoftwareKey + String;
	WCHAR str[256];
	unsigned long	nLength = sizeof str;

	if(!RegKey.Open(HKEY_LOCAL_MACHINE,m_SoftwareKey,KEY_READ)) {
		if(RegKey.QueryStringValue(String,str,&nLength)) {
			RegKey.Close();
			if(!RegKey.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\USL"),KEY_READ)) {
				RegKey.SetKeyValue(m_ProgramName,Value,String);
				RegKey.Close();
				return;
			}
		}
		RegKey.Close();
	}
}

void CRegistry::SetString(CString String, CString Value)
{
	CRegKey	RegKey;

	if(!RegKey.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\USL"),KEY_READ)) {
		RegKey.SetKeyValue(m_ProgramName,Value,String);
		RegKey.Close();
	}
}

void CRegistry::SetString(CString String, int nValue)
{
	CRegKey	RegKey;
	CString Buff;

	Buff.Format(_T("%d"),nValue);

	if(!RegKey.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\USL"),KEY_READ)) {
		RegKey.SetKeyValue(m_ProgramName,Buff,String);
		RegKey.Close();
	}
}

void CRegistry::CopyDriverToWindows(CString FilePath,CString DisplayName, CString *pStatus,CDialog *pDlg)
{
	CString Buff;
	CString	DriverPath;
	CFile File;
	
	if (!File.Open(FilePath, CFile::typeBinary|CFile::modeRead)) { 
		pStatus->Format(_T("Unable to open %s"),FilePath);
		pDlg->UpdateData(FALSE);
	} else {
		_TCHAR System32Directory[_MAX_PATH];
		if(!GetSystemDirectory(System32Directory,_MAX_PATH)) {
			pStatus->Format(_T("Could not find Windows system directory"));
			pDlg->UpdateData(FALSE);
			return;
		}
		
		int	nFileLength = (int)File.GetLength();
		char *pBuffer = new char[nFileLength];
		File.Read(pBuffer,nFileLength);
		File.Close();


		DriverPath.Format(_T("%s\\DRIVERS\\%s.sys"),System32Directory,DisplayName);
		if (!File.Open(DriverPath, CFile::typeBinary|CFile::modeWrite)) { 
			pStatus->Format(_T("Unable to open %s"),DriverPath);
		} else {
			File.Write(pBuffer,nFileLength);
			File.Close();
			pStatus->Format(_T("Copied to %s"),DriverPath);
		}
		delete pBuffer;
	}
	pDlg->UpdateData(FALSE);
}

void CRegistry::StartDriver(CString Driver,CString *pStatus,CDialog* pDlg)
{

	SC_HANDLE hDriver = OpenService(m_hSCManager,Driver,SERVICE_ALL_ACCESS);
	if( hDriver==NULL)
	{
		_TCHAR System32Directory[_MAX_PATH];
		if( 0==GetSystemDirectory(System32Directory,_MAX_PATH))	{
			pStatus->Format(_T("Could not find Windows system directory"));
			pDlg->UpdateData(FALSE);
			return;
		}
		CString Drivers = System32Directory;
		Drivers += _T("\\Drivers\\")+Driver+_T(".sys");

		hDriver = CreateService(m_hSCManager,Driver,Driver,SERVICE_ALL_ACCESS,
								SERVICE_KERNEL_DRIVER,SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,
								Drivers,NULL,NULL,NULL,NULL,NULL);
		if( hDriver==NULL)
		{
			pStatus->Format(_T("Could not create Service %s"),Driver);
			goto StartExit;
		}
	}

	SERVICE_STATUS ss;
	ControlService(hDriver,SERVICE_CONTROL_INTERROGATE,&ss);
	if( ss.dwCurrentState!=SERVICE_RUNNING)	{
		if( !StartService(hDriver,0,NULL)) {
			pStatus->Format(_T("Could not start %s %d"), Driver, GetLastError());
			goto StartExit;
		}
		pStatus->Format(_T("Starting"));
		pDlg->UpdateData(FALSE);
//		pDlg->GetDlgItem(IDC_EDIT_STATUS)->UpdateWindow();
		// Give it 10 seconds to start
		BOOL Done = FALSE;
		for(int seconds=0;seconds<10;seconds++)	{
			Sleep(1000);
			if( ControlService(hDriver,SERVICE_CONTROL_INTERROGATE,&ss) &&	ss.dwCurrentState==SERVICE_RUNNING)	{
				pStatus->Format(_T("Running"));
				pDlg->UpdateData(FALSE);
				Done = TRUE;
				break;
			}
		}
		if( !Done) {
			pStatus->Format(_T("Could not start %s"),Driver);
			goto StartExit;
		}
	} else {
		pStatus->Format(_T("Running"));
	}
StartExit:
	pDlg->UpdateData(FALSE);
	CloseServiceHandle(hDriver);


}

void CRegistry::StopDriver(CString Driver,CString *pStatus,CDialog* pDlg)
{

	SC_HANDLE hDriver = OpenService(m_hSCManager,Driver,SERVICE_ALL_ACCESS);
	if( hDriver==NULL)	{
		pStatus->Format(_T("Could not open Service %s"),Driver);
		pDlg->UpdateData(FALSE);
		return;
	}

	SERVICE_STATUS ss;
	ControlService(hDriver,SERVICE_CONTROL_INTERROGATE,&ss);
	if( ss.dwCurrentState!=SERVICE_STOPPED)	{
		if( !ControlService(hDriver,SERVICE_CONTROL_STOP,&ss)){
			pStatus->Format(_T("Could not stop %s"),Driver);
			goto StartExit;
		}
		pStatus->Format( _T("Stopping"));
		pDlg->UpdateData(FALSE);
//		pDlg->GetDlgItem(IDC_EDIT_STATUS)->UpdateWindow();
		// Give it 10 seconds to stop
		BOOL Done = FALSE;
		for(int seconds=0;seconds<10;seconds++)	{
			Sleep(1000);
			if( ControlService(hDriver,SERVICE_CONTROL_INTERROGATE,&ss)==0 ||
				ss.dwCurrentState==SERVICE_STOPPED)
			{
				pStatus->Format(_T("Stopped"));
				Done = TRUE;
				break;
			}
		}
		if( !Done)	{
			pStatus->Format(_T("Could not stop %s"),Driver);
			goto StartExit;
		}
	}
StartExit:
	pDlg->UpdateData(FALSE);
	CloseServiceHandle(hDriver);


}

void CRegistry::LookupDriver(CString Driver, CString *pStatus, CDialog *pDlg)
{
	SC_HANDLE hDriver = OpenService(m_hSCManager,Driver,SERVICE_ALL_ACCESS);
	if( hDriver==NULL) {
		pStatus->Format(_T("Could not open Service %s"),Driver);
		pDlg->UpdateData(FALSE);
		return;
	}

	SERVICE_STATUS ss;
	if( ControlService(hDriver,SERVICE_CONTROL_INTERROGATE,&ss)) {
		switch( ss.dwCurrentState)
		{
		case SERVICE_STOPPED:
			pStatus->Format(_T("Stopped"));
			break;
		case SERVICE_START_PENDING:
			pStatus->Format(_T("Start pending"));
			break;
		case SERVICE_STOP_PENDING:
			pStatus->Format(_T("Stop pending"));
			break;
		case SERVICE_RUNNING:
			pStatus->Format(_T("Running"));
			break;
		case SERVICE_CONTINUE_PENDING:
			pStatus->Format(_T("Continue pending"));
			break;
		case SERVICE_PAUSE_PENDING:
			pStatus->Format(_T("Pause pending"));
			break;
		case SERVICE_PAUSED:
			pStatus->Format(_T("Paused"));
			break;
		default:
			pStatus->Format(_T("Unknown"));
			break;
		}
	} else {
		pStatus->Format(_T("Unknown"));
	}
	pDlg->UpdateData(FALSE);
	CloseServiceHandle(hDriver);
}

bool CRegistry::FindRegistryVariable(CString Name, CString *Buff)
{
	WCHAR	str[100];
	unsigned long	nLength = sizeof str;
	bool bFlag = TRUE;
	CRegKey	RegKey;
	CString	ErrorBuff;


	if(!RegKey.Open(HKEY_LOCAL_MACHINE,m_SoftwareKey,KEY_READ)) {
		if(!RegKey.QueryStringValue(Name,str,&(nLength=sizeof str))) {
			Buff->Format(_T("%s"),str);
		} else {
			MessageBox(NULL,_T("There is no registry entry for ")+Name,_T("Registry"),NULL);
			bFlag = FALSE;
		};
		RegKey.Close();
	} else {
		ErrorBuff =  _T("There is no registry entry for \r\n") + m_SoftwareKey;
		MessageBox(NULL,ErrorBuff,_T("Registry"),NULL);
		bFlag = FALSE;
	};

	return bFlag;
}

bool CRegistry::FindRegistryVariable(CString Name, bool *pbResult)
{
	WCHAR	str[100];
	unsigned long	nLength = sizeof str;
	bool bFlag = TRUE;
	CRegKey	RegKey;
	CString	ErrorBuff;


	if(!RegKey.Open(HKEY_LOCAL_MACHINE,m_SoftwareKey,KEY_READ)) {
		if(!RegKey.QueryStringValue(Name,str,&(nLength=sizeof str))) {
			if(wcscmp(str,_T("Not Present"))==NULL) {
				*pbResult=FALSE;
			}
			if(wcscmp(str,_T("Present"))==NULL) {
				*pbResult=TRUE;
			}
			if(wcscmp(str,_T("Disabled"))==NULL) {
				*pbResult=FALSE;
			}
			if(wcscmp(str,_T("Enabled"))==NULL) {
				*pbResult=TRUE;
			}
			if(wcscmp(str,_T("0"))==NULL) {
				*pbResult=FALSE;
			}
			if(wcscmp(str,_T("1"))==NULL) {
				*pbResult=TRUE;
			}
		} else {
			MessageBox(NULL,_T("There is no registry entry for ")+Name,_T("Registry"),NULL);
			bFlag = FALSE;
		};
	} else {
		ErrorBuff =  _T("There is no registry entry for \r\n") + m_SoftwareKey;
		MessageBox(NULL,ErrorBuff,_T("Registry"),NULL);
		bFlag = FALSE;
	};

	return bFlag;
}

bool CRegistry::FindRegistryVariable(CString Name, int *pnResult)
{
	WCHAR	str[100];
	unsigned long	nLength = sizeof str;
	bool bFlag = TRUE;
	CRegKey	RegKey;
	CString	ErrorBuff;


	if(!RegKey.Open(HKEY_LOCAL_MACHINE,m_SoftwareKey,KEY_READ)) {
		if(!RegKey.QueryStringValue(Name,str,&(nLength=sizeof str))) {
			*pnResult = _ttoi(str);
		} else {
			MessageBox(NULL,_T("There is no registry entry for ")+Name,_T("Registry"),NULL);
			bFlag = FALSE;
		};
	} else {
		ErrorBuff =  _T("There is no registry entry for \r\n") + m_SoftwareKey;
		MessageBox(NULL,ErrorBuff,_T("Registry"),NULL);
		bFlag = FALSE;
	};

	return bFlag;
}


	
void CRegistry::SetKeyValue(CRegKey* pRegKey,CString Key, CString Name, CString Format, double dValue)
{
	CString Value;

	Value.Format(Format,dValue);
	pRegKey->SetKeyValue(Key,Value,Name);

}

void CRegistry::SetKeyValue(CRegKey* pRegKey,CString Key, CString Name, CString Format, int nValue)
{
	CString Value;

	Value.Format(Format,nValue);
	pRegKey->SetKeyValue(Key,Value,Name);

}

bool CRegistry::QueryStringValue(CRegKey* pRegKey,CString Name, CString Format, float *pfValue)
{
	WCHAR Value[100];
	unsigned long	nLength = sizeof Value;

	if(pRegKey->QueryStringValue(Name, Value, &nLength) == ERROR_SUCCESS) {
		_WTOF(Value,*pfValue);
	}

	return TRUE;
}

bool CRegistry::QueryStringValue(CRegKey* pRegKey,CString Name, CString Format, int *pnValue)
{
	WCHAR Value[100];
	unsigned long	nLength = sizeof Value;

	if(pRegKey->QueryStringValue(Name, Value, &nLength) == ERROR_SUCCESS) {
		swscanf_s(Value,Format,pnValue);
	}

	return TRUE;
}

bool CRegistry::QueryStringValue(CRegKey* pRegKey,CString Name, CString Format, bool *pbValue)
{
	WCHAR Value[100];
	unsigned long	nLength = sizeof Value;

	if(pRegKey->QueryStringValue(Name, Value, &nLength) == ERROR_SUCCESS) {
		*pbValue = (bool*)_ttoi(Value);
	}

	return TRUE;
}


void CRegistry::ReportError(int nErrorCode)
{
	CString Buff;
	Buff = "Error code not recognised";

	switch(nErrorCode) {
	case ERROR_INVALID_FUNCTION:	Buff = _T("Registry Invalid Function");
		break;
	case ERROR_FILE_NOT_FOUND:	Buff = _T("Registry File not found");
		break;
	case ERROR_PATH_NOT_FOUND:	Buff = _T("Registry path not found");
		break;
	case ERROR_TOO_MANY_OPEN_FILES:	Buff = _T("Registry too many open files");
		break;
	case ERROR_ACCESS_DENIED:	Buff = _T("Registry access denied");
		break;
	case ERROR_INVALID_HANDLE:	Buff = _T("Registry invalid handle");
		break;
	case ERROR_ARENA_TRASHED:	Buff = _T("Registry arena trashed");
		break;
	case ERROR_NOT_ENOUGH_MEMORY:	Buff = _T("Registry invalid Function");
		break;
	case ERROR_INVALID_BLOCK:	Buff = _T("Registry invalid block");
		break;
	case ERROR_BAD_ENVIRONMENT:	Buff = _T("Registry bad enviroment");
		break;
	case ERROR_BAD_FORMAT:	Buff = _T("Registry bad format");
		break;
	case ERROR_INVALID_ACCESS:	Buff = _T("Registry invalid access");
		break;
	case ERROR_INVALID_DATA:	Buff = _T("Registry invalid data");
		break;
	}

	MessageBox(NULL,Buff,_T("Registry error"),MB_ICONERROR);
}
