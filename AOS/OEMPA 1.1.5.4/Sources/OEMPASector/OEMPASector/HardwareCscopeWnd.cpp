// CscopeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "HardwareCscopeWnd.h"
#include "SectorApp.h"
#include "HardwareCscopeView.h"
#include "UTKernelLayoutFile.h"

// CHardwareCscopeWnd

IMPLEMENT_DYNCREATE(CHardwareCscopeWnd, CMDIChildWndEx)

extern int g_iGateId;
CHardwareCscopeWnd::CHardwareCscopeWnd()
{
	m_iIndexLayoutFile = -1;
	m_iGateId = g_iGateId;
}

CHardwareCscopeWnd::~CHardwareCscopeWnd()
{
}

BEGIN_MESSAGE_MAP(CHardwareCscopeWnd, CMDIChildWndEx)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONUP()
	ON_WM_NCLBUTTONUP()
	ON_MESSAGE(WM_EXITSIZEMOVE, &CHardwareCscopeWnd::OnExitSizeMove)
END_MESSAGE_MAP()

// CHardwareCscopeWnd message handlers

BOOL CHardwareCscopeWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

BOOL CHardwareCscopeWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{//integration
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;
	CString string;

	if(m_iGateId<4)
		string.Format(L"Cscan Amp %d",m_iGateId);
	else
		string.Format(L"Cscan Tof %d",m_iGateId-4);
	wcscpy_s(m_pTitle,20,string);
	if(m_iIndexLayoutFile<0)
	if(UTKernel_LayoutFile()->RegisterWindow(m_pTitle,m_iIndexLayoutFile,bRegistered,bCreateNew) && bRegistered)
	{
		if(UTKernel_LayoutFile()->ReadPositionWindow(m_iIndexLayoutFile,m_pTitle,sysTime,iResHorz,iResVert,wndPlacement,iData))
		{
			if(wndPlacement.showCmd==SW_MAXIMIZE)
				MDIMaximize();
			else if(UTKernel_LayoutFile()->CheckPlacement(wndPlacement))
				SetWindowPlacement(&wndPlacement);
		}
	}
	SetWindowText(string);

	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}

void CHardwareCscopeWnd::OnMove(int x, int y)
{//integration
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnMove(x, y);
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,m_pTitle,iResHorz,iResVert,wndPlacement,0);
}

void CHardwareCscopeWnd::OnSize(UINT nType, int cx, int cy)
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnSize(nType, cx, cy);

	//integration
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,m_pTitle,iResHorz,iResVert,wndPlacement,0);
	//switch (nType)
	//{
	//	case SIZE_MAXIMIZED:
	//		if (g_pHardwareCscope)
	//			g_pHardwareCscope->SetEvent(1);
	//		
	//		break;
	//	case SIZE_MINIMIZED:
	//		if (g_pHardwareCscope)
	//			g_pHardwareCscope->SetEvent(1);

	//		break;
	//	//case SIZE_RESTORED:
	//	//	if (g_pHardwareCscope)
	//	//		g_pHardwareCscope->SetEvent(1);

	//	//	break;
	//}
}

void CHardwareCscopeWnd::OnSizing(UINT fwSide, LPRECT pRect)
{
	CMDIChildWndEx::OnSizing(fwSide, pRect);

	if (g_pHardwareCscope[m_iGateId])
		g_pHardwareCscope[m_iGateId]->OnSizing(fwSide, pRect);
}

void CHardwareCscopeWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (g_pHardwareCscope[m_iGateId])
		g_pHardwareCscope[m_iGateId]->OnLButtonUp(nFlags, point);

	CMDIChildWndEx::OnLButtonUp(nFlags, point);
}

void CHardwareCscopeWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CMDIChildWndEx::OnNcLButtonUp(nHitTest, point);
}

LRESULT CHardwareCscopeWnd::OnExitSizeMove(WPARAM wParam, LPARAM lParam)
{
	//if (g_pHardwareCscope)
	//	g_pHardwareCscope->SetEvent(1);

	return 0;
}