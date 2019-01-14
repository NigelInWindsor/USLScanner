// PmacPcommServer.h: interface for the CPmacPcommServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMACPCOMMSERVER_H__0EC5E4B0_42D8_4E61_9BF5_63163F7D3232__INCLUDED_)
#define AFX_PMACPCOMMSERVER_H__0EC5E4B0_42D8_4E61_9BF5_63163F7D3232__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPmacPcommServer  
{
public:
	LPUNKNOWN	m_pDisp;
	DWORD		m_dwCookie ;
	LPUNKNOWN	m_pDisp1;
	DWORD		m_dwCookie1 ;
	LPUNKNOWN	m_pUnkSink;
	LPUNKNOWN	m_pUnkSink1;
	long		m_nPmacType;
	BOOL		m_bDriverOpen;
	DWORD		m_dwDevice;
	bool		m_bThreadEnabled;
	bool		m_bThreadFinished;
	CWinThread* m_pThread;

	
	void Save(CUSLFile *pFile);
	void WriteDpram(int nOffset,int nValue);
	int ReadDpram(int nOffset);
	int DpramSetMem(int nOffset,int nLength,PVOID pMem);
	int DpramGetMem(int nOffset,int nLength,PVOID pMem);

	void InterruptServiceRoutine();
	void DisableInterrupt();
	void EnableInterrupt();

	bool DownLoad(CString FilePath);
	int RequestVariable(CString VariableName, int nOffset, CString &RxBuff);
	void SetVariable(CString VariableName, int nOffset, CString Value);
	void SetVariable(CString VariableName, int nOffset, float fValue);
	void SetVariable(CString VariableName, int nOffset, int nValue);
	int RequestMultipleVariables(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff);
	int RequestMVariableDefinitions(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff);
	int RequestResponce(CString TxBuff, CString *pRxBuff);
	BOOL GetStr(CString &RxBuff);
	int SendStr(CString TxBuff,bool bWaitForResponce=FALSE);
	bool StartDriver(CString &strPmacType,int *nPmacType);
	bool SuspendDriver();

	CPmacPcommServer();
	virtual ~CPmacPcommServer();

};

#endif // !defined(AFX_PMACPCOMMSERVER_H__0EC5E4B0_42D8_4E61_9BF5_63163F7D3232__INCLUDED_)
