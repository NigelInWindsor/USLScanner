
#pragma once

class CRunToolbox;
typedef DWORD (WINAPI *pTypeCallbackThreadToolbox)(HWND hWnd,CRunToolbox *pRunToolbox);
typedef LRESULT (WINAPI* pCallbackTimerProcess)(int iStep,void *pThis,DWORD &dwNextSleep);//return 0 to stop the thread, otherwise 1 in this case dwNextSleep is also an output parameter.

class UTKERNEL_DRIVER_API_ALIGN CTimerProcess
{
//to optimize stream package time before sending data.
public:
	CTimerProcess();
	~CTimerProcess();

	bool Start(DWORD dwSleep,pCallbackTimerProcess pFunc,void *pThis);
	bool Stop(bool *pbError=NULL);
	bool IsStarted() const;
	DWORD GetThreadId();
	HANDLE GetThread();
	void SetThreadPriority(int iPriority);
	int GetThreadPriority();

protected:
	static DWORD WINAPI ThreadDelay(LPVOID pParam);

	void *m_pThis;
	pCallbackTimerProcess m_pFunc;
	DWORD m_dwSleep;
	HANDLE m_hThread;
	DWORD m_dwThreadExit;
	DWORD m_dwThreadId;//1:0:4:0
	int m_iPriority;
};

class UTKERNEL_DRIVER_API_ALIGN CRunToolbox
{
public:
	CRunToolbox();

	bool IsRunning();
	bool Run(HWND hWnd,pTypeCallbackThreadToolbox pCallbackThreadToolbox);
	void ResetPostFeedback();
	void SignalPostFeedback();
	bool WaitPostFeedback(DWORD dwTimeOut/*millisecond*/,UINT uiErrorMsg,WPARAM wErrorParam,LPARAM lErrorParam);
private:
	static DWORD WINAPI ThreadToolbox(LPVOID pParam);
	bool m_bPostFeedback;
	HWND m_hWnd;
	UINT m_uiErrorMsg;WPARAM m_wErrorParam;LPARAM m_lErrorParam;
	pTypeCallbackThreadToolbox m_pCallbackThreadToolbox;
	HANDLE m_hThread;
	DWORD m_dwThreadId;
};
