// PmacNCEvents.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PmacNCEvents.h"


// CPmacNCEvents

IMPLEMENT_DYNAMIC(CPmacNCEvents, CCmdTarget)
CPmacNCEvents::CPmacNCEvents()
{
	EnableAutomation();
}

CPmacNCEvents::~CPmacNCEvents()
{
}


void CPmacNCEvents::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CPmacNCEvents, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CPmacNCEvents, CCmdTarget)
    DISP_FUNCTION_ID(CPmacNCEvents,"Error",1,OnError,VT_EMPTY,VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I2 VTS_BSTR)
    DISP_FUNCTION_ID(CPmacNCEvents,"Message",2,OnMessage,VT_EMPTY,VTS_I4 VTS_BSTR)
    DISP_FUNCTION_ID(CPmacNCEvents, "Progress",3,OnProgress,VT_EMPTY,VTS_I4 VTS_I4)
    DISP_FUNCTION_ID(CPmacNCEvents, "MessageQuery",4,OnMessageQuery,VT_I4,VTS_I4 VTS_I4 VTS_BSTR)
    DISP_FUNCTION_ID(CPmacNCEvents, "SearchProgress",5,OnSearchProgress,VT_I4,VTS_I4 VTS_I4 )
    DISP_FUNCTION_ID(CPmacNCEvents, "Change",6,OnChange,VT_I4,VTS_I4 VTS_I4 VTS_VARIANT)
END_DISPATCH_MAP()

// Note: we add support for IID_IPmacNCEvents to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {B2E01AD5-6DEB-43DE-BF32-35569913B332}
/*Comment out Not Needed static const IID IID_IPmacNCEvents =
{ 0xB2E01AD5, 0x6DEB, 0x43DE, { 0xBF, 0x32, 0x35, 0x56, 0x99, 0x13, 0xB3, 0x32 } };
*/
BEGIN_INTERFACE_MAP(CPmacNCEvents, CCmdTarget)
	INTERFACE_PART(CPmacNCEvents, DIID__IPmacNCEvents, Dispatch)
END_INTERFACE_MAP()


// CPmacNCEvents message handlers
void CPmacNCEvents::OnMessage(long Control,LPCTSTR Msg)
{
   AfxMessageBox(_T("Event notification handled in .NET client -OnMessage "));
}

void CPmacNCEvents::OnProgress(long lDevice,long dwPercent)
{
	int Test = 0;
}

void CPmacNCEvents::OnError(long Control,NCERRORMODE ErrMode,NCERRORTYPE ErrType,long ErrId,long ErrLine,short ErrAxis,LPCTSTR ErrMsg)
{
   AfxMessageBox(_T("Event notification handled in .NET client - OnError "));
}

long CPmacNCEvents::OnMessageQuery(long Control,long msgID,BSTR msg)
{
	int Test = 0;
	return Test;
}
long  CPmacNCEvents::OnSearchProgress(long Control, long Line)
{
	int Test = 0;
	return Test;
}

long CPmacNCEvents::OnChange(long Control, long changeID, const VARIANT& vVal)
{
	int Test = 0;
	return Test;
}

