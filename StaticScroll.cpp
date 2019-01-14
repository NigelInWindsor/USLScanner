// StaticScroll.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "StaticScroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticScroll
IMPLEMENT_DYNCREATE(CStaticScroll, CStatic)

CStaticScroll::CStaticScroll()
{
}

CStaticScroll::~CStaticScroll()
{
}


BEGIN_MESSAGE_MAP(CStaticScroll, CStatic)
	//{{AFX_MSG_MAP(CStaticScroll)
	ON_WM_VSCROLL()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticScroll message handlers

void CStaticScroll::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CStatic::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CStaticScroll::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CStatic::OnNotify(wParam, lParam, pResult);
}

int CStaticScroll::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	return 0;
}

BOOL CStaticScroll::PreTranslateMessage(MSG* pMsg) 
{
	TCHAR ch;
	bool m_bShiftKeyDown;

	switch(pMsg->message) {
	case WM_KEYDOWN: //keyboa
		ch = TCHAR(pMsg->wParam);
		if(ch == 0x10) {
			m_bShiftKeyDown ^= TRUE;
			if(m_bShiftKeyDown == FALSE) {
			}
		}
		break;
	case WM_KEYUP:
		break;
	case 0x200: // Mouse move
		switch(pMsg->wParam) {
		case 1:
			break;
		}
		break;
	case 0x201://LBUTTONDOWN
		TRACE2("Msg->%d Param->%d\r\n",pMsg->message,pMsg->wParam);
		switch(pMsg->wParam) {
		case 1:	
//			switch(pFrame->m_CtrlType) {
//			case 0:	
//				break;
//			case TEXT_CTL:
//			case EDIT_CTL:
//			case BUTTON_CTL:
//				break;
//			}
//			return TRUE;
			break;
		}
		break;

	case 0x202:	//LBUTTONUP
		TRACE2("Msg->%d Param->%d\r\n",pMsg->message,pMsg->wParam);
		break;

	case 0x203: //LBUTTONDOUBLECLICK
		switch(pMsg->wParam) {
		case 1:
			return TRUE;
			break;
		}
		break;

	case 0x204://RBUTTONDOWN
		switch(pMsg->wParam) {
		case 2:
			break;
		}
		break;

	case 273:
		TRACE2("Msg->%d Param->%d\r\n",pMsg->message,pMsg->wParam);
		switch(pMsg->wParam) {
		case ID_EDIT_CUT:
			return TRUE;
			break;				
		case ID_EDIT_PROPERTIES:
			return TRUE;
			break;
		case ID_EDIT_TABORDER:
			return TRUE;
			break;
		}
		break;
	default:
		TRACE1("Msg->%d\r\n",pMsg->message);
		break;
			
	}

	
	return CStatic::PreTranslateMessage(pMsg);
}
