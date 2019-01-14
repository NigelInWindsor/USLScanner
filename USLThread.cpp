#include "stdafx.h"
#include "USLThread.h"
#include "USLScanner.h"


extern UINT CyclicThread(LPVOID pParam);

CUSLThread::CUSLThread()
{
	m_hEvent = NULL;
	m_hThread = NULL;
	m_pThread = NULL;
	m_bThreadEnabled = false;
	m_bThreadFinished = false;

	m_pfnCallback = NULL;
	m_pParam = NULL;
	m_nPriority = THREAD_PRIORITY_NORMAL;
	m_nRefreshRate_ms = 1;
}


CUSLThread::~CUSLThread()
{
	Suspend();
}

void CUSLThread::Initialize(CString Name, AFX_THREADPROC pfnCallback, LPVOID pParam, int nRefreshRate_ms, int nPriority)
{
	m_Name = Name;
	m_pfnCallback = pfnCallback;
	m_pParam = pParam;
	m_nPriority = nPriority;
	m_nRefreshRate_ms = nRefreshRate_ms;
}

void CUSLThread::Start()
{
	if (m_bThreadEnabled == false) {
		m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		m_bThreadEnabled = true;
		m_pThread = AfxBeginThread(CyclicThread, this, m_nPriority, 0, NULL);
	}
}

void CUSLThread::Suspend()
{
	int nRet;

	if (m_bThreadEnabled) {
		m_bThreadEnabled = false;
		ResetEvent(m_hEvent);
		nRet = WaitForSingleObject(m_hEvent, 200);
		TRACE1("USLThread suspending %s\n",m_Name);
		nRet = WaitForSingleObject(CyclicThread, INFINITE);
		TRACE1("USLThread succeeded %s\n", m_Name);
		CloseHandle(m_hEvent);

		m_Name.Empty();
		m_pfnCallback = NULL;
		m_pParam = NULL;
		m_nPriority = THREAD_PRIORITY_NORMAL;
		m_nRefreshRate_ms = 1;
	}
}

UINT CyclicThread(LPVOID pParam)
{
	int nRet;
	CUSLThread* pParent = (CUSLThread *)pParam;
	pParent->m_bThreadFinished = FALSE;
	LARGE_INTEGER StartCount;
	int nRefreshTime;

	while (pParent->m_bThreadEnabled == true) {
		QueryPerformanceCounter(&StartCount);
		nRefreshTime = pParent->m_nRefreshRate_ms - (pParent->m_nProcessTime_us / 1000);
		if (nRefreshTime < 0) nRefreshTime = 0;

		if ((nRet = WaitForSingleObject(pParent->m_hEvent, nRefreshTime)) == WAIT_TIMEOUT) {
			pParent->m_nDeadTime_us = theApp.getElapsedTime_us(StartCount);

			QueryPerformanceCounter(&StartCount);
			if (pParent->m_pfnCallback) 
				if(pParent->m_pfnCallback(pParent->m_pParam) == SUSPEND_MYSELF) pParent->m_bThreadEnabled = false;	//if -1 returned, it kills itself
			pParent->m_nProcessTime_us = theApp.getElapsedTime_us(StartCount);
		}
	}
	SetEvent(pParent->m_hEvent);
	return 0;
}

bool CUSLThread::IsActive()
{
	return m_bThreadEnabled;
}

CString &CUSLThread::getName()
{
	return m_Name;
}

LPVOID &CUSLThread::getParam()
{
	return m_pParam;
}

CString &CUSLThread::getRefreshRate()
{
	static CString Buff;

	Buff.Format(L"%d Hz", 1000 / m_nRefreshRate_ms);

	return Buff;
}

CString &CUSLThread::getProcessTime()
{
	static CString Buff;

	if (m_nProcessTime_us < 1000) {
		Buff.Format(L"%d %cs", m_nProcessTime_us, 0xb5);
	}
	else {
		Buff.Format(L"%d ms", m_nProcessTime_us / 1000);
	}

	return Buff;
}

CString &CUSLThread::getDeadTime()
{
	static CString Buff;

	if (m_nDeadTime_us < 1000) {
		Buff.Format(L"%d %cs", m_nDeadTime_us, 0xb5);
	}
	else {
		Buff.Format(L"%d ms", m_nDeadTime_us / 1000);
	}

	return Buff;
}

