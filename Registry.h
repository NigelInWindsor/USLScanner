// Registry.h: interface for the CRegistry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTRY_H__E33BFC60_B590_4778_B533_F123732DBCDB__INCLUDED_)
#define AFX_REGISTRY_H__E33BFC60_B590_4778_B533_F123732DBCDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "winsvc.h"
#include <atlbase.h>

class CRegistry  
{
public:
	void ReportError(int nErrorCode);
	bool QueryStringValue(CRegKey* pRegKey,CString Name, CString Format, bool *pbValue);
	bool QueryStringValue(CRegKey* pRegKey,CString Name, CString Format, int *pnValue);
	bool QueryStringValue(CRegKey* pRegKey,CString Name, CString Format, float *pfValue);
	void SetKeyValue(CRegKey* pRegKey,CString Key,CString Name,CString Format,double dValue);
	void SetKeyValue(CRegKey* pRegKey,CString Key,CString Name,CString Format,int nValue);

	void RetrieveCardEntries();
	void CreateCardEntries();

	bool FindRegistryVariable(CString Name, int *pnResult);
	bool FindRegistryVariable(CString Name, CString *Buff);
	bool FindRegistryVariable(CString Name, bool *pbResult);
	void LookupDriver(CString Driver,CString *pStatus,CDialog* pDlg);
	void StopDriver(CString Driver,CString *pStatus,CDialog* pDlg);
	void StartDriver(CString Driver,CString *pStatus,CDialog* pDlg);
	void CopyDriverToWindows(CString Filepath,CString DisplayName, CString *pStatus,CDialog *pDlg);
	void CreateHardwareKey(CString Name);
	void SetString(CString String, int nValue);
	void SetString(CString String, CString Value);
	void CreateString(CString String,CString Value);
	CString m_ProgramName;
	void CreateSoftwareKey();
	void SetRegistryVariables();
	bool FindRegistryVariables();
	CString m_SoftwareKey;
	CRegistry(CString ProgramName);
	CRegistry();
	virtual ~CRegistry();
private:
	SC_HANDLE m_hSCManager;
};

#endif // !defined(AFX_REGISTRY_H__E33BFC60_B590_4778_B533_F123732DBCDB__INCLUDED_)
