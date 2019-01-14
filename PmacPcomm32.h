// PmacPcomm32.h: interface for the CPmacPcomm32 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMACPCOMM32_H__1A3CF64B_A5A2_4386_8A55_E037A0A8AE53__INCLUDED_)
#define AFX_PMACPCOMM32_H__1A3CF64B_A5A2_4386_8A55_E037A0A8AE53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Include\Intr.h"

void __stdcall InterruptServiceRoutine(DWORD dwUser = 0,PINTRBUFFER pBuff = NULL);
UINT PmacThread(LPVOID pParam);

class CPmacPcomm32
{
public:
	int			m_nPmacType;
	BOOL		m_bDriverOpen;
	DWORD		m_dwDevice;
	HINSTANCE	m_hPmacLib;
	bool		m_bThreadEnabled;
	bool		m_bThreadFinished;
	CWinThread* m_pThread;

	void Retrieve(CString FilePath);
	void Save(CUSLFile* pFile);
	int ReadDpram(int nOffset);
	void WriteDpram(int nOffset, int nValue);
	int DpramGetMem(int nOffset,int nLength,PVOID pMem);
	int DpramSetMem(int nOffset,int nLength,PVOID pMem, bool bDoMemCheck = TRUE);
	void StartThread();
	void StopThread();
	void DisableInterrupt();
	void EnableInterrupt();
	bool DownLoad(CString FilePath);
	int RequestMVariableDefinitions(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff);
	int RequestMultipleVariables(CString VariableName, int nStartOffset,int nFinishOffset, char *Result);
	int RequestMultipleVariables(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff);
	int RequestVariable(CString VariableName, int nOffset, char *Result);
	int RequestVariable(CString VariableName, int nOffset, CString *pRxBuff);
	int RequestResponce(CString TxBuff, CString *pRxBuff);
	BOOL GetStr(CString &RxBuff);
	int SendStr(CString TxBuff,bool bWaitForResponce=FALSE);
	void ConfigureComms(HWND Wnd);
	bool StartDriver(CString &strPmacType,int *nPmacType);

	CPmacPcomm32();
	virtual ~CPmacPcomm32();

};

#endif // !defined(AFX_PMACPCOMM32_H__1A3CF64B_A5A2_4386_8A55_E037A0A8AE53__INCLUDED_)
