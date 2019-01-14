
#pragma once
//#include <pmacserver_i.c>  // Accsee from Additional directory include of Project setting.
#include "PCommServer Pro2 Library\Include\PmacServer.H"    // Accsee from Additional directory include of Project setting.


// CPmacDeviceEvents command target

class CPmacDeviceEvents : public CCmdTarget
{
	DECLARE_DYNAMIC(CPmacDeviceEvents)

public:
	CPmacDeviceEvents();
	virtual ~CPmacDeviceEvents();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
    afx_msg void OnProgress(long Device,long dwPercent);
    afx_msg void OnMessage(long Device,LPCTSTR Msg,BOOL bNewLine);
    afx_msg void OnError(long Device,LPCTSTR FileName,long ErrorID,long LineNum,LPCTSTR Line);
    afx_msg void OnInterrupt(long Device,long dwLevel);
		afx_msg void OnUnSolicited(long lDevice,LPCTSTR Msg);

    afx_msg void OnDataReady(long lDevice);
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


