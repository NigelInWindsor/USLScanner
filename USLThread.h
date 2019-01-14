#pragma once

#define	THREAD_CONTINUE	0
#define	SUSPEND_MYSELF	1

class CUSLThread
{
public:
	CUSLThread();
	~CUSLThread();
	void Initialize(CString Name, AFX_THREADPROC pfnThreadProc, LPVOID pParam, int nRefreshRate_ms = 100, int nPriority = THREAD_PRIORITY_NORMAL);

	CCriticalSection	m_CriticalSection;

	HANDLE m_hSemaphore;
	HANDLE m_hEvent;
	HANDLE m_hThread;
	CWinThread* m_pThread;
	void Start();
	void Suspend();
	bool IsActive();
	CString & getName();
	LPVOID & getParam();
	CString & getRefreshRate();
	CString & getProcessTime();
	CString & getDeadTime();
	bool m_bThreadEnabled;
	bool m_bThreadFinished;
	AFX_THREADPROC  m_pfnCallback;
	LPVOID m_pParam;
	int m_nPriority;
	int m_nRefreshRate_ms;
	CString m_Name;

	int	m_nProcessTime_us;
	int m_nDeadTime_us;

};

