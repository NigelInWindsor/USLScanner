// PmacDeviceEvents.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PmacDeviceEvents.h"
extern CProgressCtrl m_ProgressBar;
extern CEdit m_EditData;
extern CString *MyString;
extern HWND MainhWnd;
#define IDDOWNLOADMSG WM_USER+0x100


// CPmacDeviceEvents

IMPLEMENT_DYNAMIC(CPmacDeviceEvents, CCmdTarget)
CPmacDeviceEvents::CPmacDeviceEvents()
{
	EnableAutomation();
}

CPmacDeviceEvents::~CPmacDeviceEvents()
{
}


void CPmacDeviceEvents::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CPmacDeviceEvents, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CPmacDeviceEvents, CCmdTarget)
    DISP_FUNCTION_ID(CPmacDeviceEvents,"Message",1,OnMessage,VT_EMPTY,VTS_I4 VTS_BSTR VTS_BOOL)
    DISP_FUNCTION_ID(CPmacDeviceEvents,"Progress",2,OnProgress,VT_EMPTY,VTS_I4 VTS_I4)
    DISP_FUNCTION_ID(CPmacDeviceEvents,"Error",3,OnError,VT_EMPTY,VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR)
    DISP_FUNCTION_ID(CPmacDeviceEvents,"Interrupt",4,OnInterrupt,VT_EMPTY,VTS_I4 VTS_I4)
    DISP_FUNCTION_ID(CPmacDeviceEvents,"UnSolicited",5,OnUnSolicited,VT_EMPTY,VTS_I4 VTS_BSTR)
    DISP_FUNCTION_ID(CPmacDeviceEvents, "DataReady",6,OnDataReady,VT_EMPTY,VTS_I4)
END_DISPATCH_MAP()

// Note: we add support for IID_IPmacDeviceEvents to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {25C3A48B-AD8D-41E6-9D97-6969085AE3EC}
/* Comment out Not Needed --- static const IID IID_IPmacDeviceEvents =
{ 0x25C3A48B, 0xAD8D, 0x41E6, { 0x9D, 0x97, 0x69, 0x69, 0x8, 0x5A, 0xE3, 0xEC } };
*/
BEGIN_INTERFACE_MAP(CPmacDeviceEvents, CCmdTarget)
	INTERFACE_PART(CPmacDeviceEvents, DIID__IPmacDeviceEvents, Dispatch)
END_INTERFACE_MAP()


// CPmacDeviceEvents message handlers
void CPmacDeviceEvents::OnMessage(long lDevice,LPCTSTR Msg,BOOL bNewLine)
{
//	*MyString = Msg;
//	::PostMessage(MainhWnd,IDDOWNLOADMSG,(WPARAM)MyString,0);

}

void CPmacDeviceEvents::OnProgress(long lDevice,long dwPercent)
{
//  m_ProgressBar.SetPos(dwPercent);
}

void CPmacDeviceEvents::OnError(long lDevice,LPCTSTR FileName,long ErrorID,long LineNum,LPCTSTR Line)
{
   AfxMessageBox(_T("Event notification handled in .NET client -OnError "));

}
void CPmacDeviceEvents::OnInterrupt(long lDevice,long dwLevel)
{
	if(dwLevel == 6) {
		theApp.m_PmacUser.m_CommsServer.InterruptServiceRoutine();
	}
	return;
/*
	LPCTSTR Msg;

	switch(dwLevel)
	{
	case 0: Msg = _T("Interrupt>> In Position"); break;
	case 1: Msg = _T("Interrupt>> Buffer Request"); break;
	case 2: Msg = _T("Interrupt>> Fatal Following Error"); break;
	case 3: Msg = _T("Interrupt>> Warning Following Error"); break;
	case 4: Msg = _T("Interrupt>> Host Request"); break;
	case 5: Msg = _T("Interrupt>> Custom IR5"); break;
	case 6: Msg = _T("Interrupt>> Custom IR6");
		theApp.m_PmacUser.InterruptServiceRoutine();
		break;
	case 7: Msg = _T("Interrupt>> Custom IR7"); break;
	default: Msg = _T("Interrupt>> Unknown"); break;
	}
	*/
//	*MyString = Msg;
//	::PostMessage(MainhWnd,IDDOWNLOADMSG,(WPARAM)MyString,0);
}

void CPmacDeviceEvents::OnUnSolicited(long lDevice,LPCTSTR Msg)
{
//	*MyString = Msg;
//	::PostMessage(MainhWnd,IDDOWNLOADMSG,(WPARAM)MyString,0);
}
void CPmacDeviceEvents::OnDataReady(long lDevice)
{
   AfxMessageBox(_T("Event notification handled in .NET client -OnDataReady "));

}
