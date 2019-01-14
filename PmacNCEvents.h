
#pragma once

#include "PCommServer Pro2 Library\Include\pmacserver_i.c"  // Accsee from Additional directory include of Project setting.
#include "PCommServer Pro2 Library\Include\PmacServer.H"    // Accsee from Additional directory include of Project setting.

class CPmacNCEvents : public CCmdTarget
{
	DECLARE_DYNAMIC(CPmacNCEvents)
	CPmacNCEvents();           // protected constructor used by dynamic creation

public:
//	CPmacNCEvents();
	virtual ~CPmacNCEvents();

	virtual void OnFinalRelease();
protected:
	DECLARE_MESSAGE_MAP()
    afx_msg void OnProgress(long Device,long dwPercent);
    afx_msg void OnMessage(long Control,LPCTSTR Msg);
    afx_msg void OnError(long Control,NCERRORMODE ErrMode,NCERRORTYPE ErrType,long ErrId,long ErrLine,short ErrAxis,LPCTSTR ErrMsg);
    afx_msg long OnMessageQuery(long Control,long msgID,BSTR msg);
    afx_msg long OnSearchProgress(long Control, long Line);
//    afx_msg void OnDataReady(long lDevice);
    afx_msg long OnChange(long Control, long changeID, const VARIANT& vVal);

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


