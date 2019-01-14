// CscopeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicCscopeWnd.h"
#include "SectorApp.h"
#include "DynamicCscopeView.h"
#include "UTKernelLayoutFile.h"

// CDynamicCscopeWnd

IMPLEMENT_DYNCREATE(CDynamicCscopeWnd, CMDIChildWndEx)

CDynamicCscopeWnd::CDynamicCscopeWnd()
{
	m_iIndexLayoutFile = -1;
}

CDynamicCscopeWnd::~CDynamicCscopeWnd()
{
}

BEGIN_MESSAGE_MAP(CDynamicCscopeWnd, CMDIChildWndEx)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONUP()
	ON_WM_NCLBUTTONUP()
	ON_MESSAGE(WM_EXITSIZEMOVE, &CDynamicCscopeWnd::OnExitSizeMove)
END_MESSAGE_MAP()

// CDynamicCscopeWnd message handlers

BOOL CDynamicCscopeWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

BOOL CDynamicCscopeWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{//integration
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;

	if(m_iIndexLayoutFile<0)
	if(UTKernel_LayoutFile()->RegisterWindow(L"DynamicCscope",m_iIndexLayoutFile,bRegistered,bCreateNew) && bRegistered)
	{
		if(UTKernel_LayoutFile()->ReadPositionWindow(m_iIndexLayoutFile,L"DynamicCscope",sysTime,iResHorz,iResVert,wndPlacement,iData))
		{
			if(wndPlacement.showCmd==SW_MAXIMIZE)
				MDIMaximize();
			else if(UTKernel_LayoutFile()->CheckPlacement(wndPlacement))
				SetWindowPlacement(&wndPlacement);
		}
	}
	SetWindowText(L"Dynamic Cscope");

	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}

void CDynamicCscopeWnd::OnMove(int x, int y)
{//integration
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnMove(x, y);
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"DynamicCscope",iResHorz,iResVert,wndPlacement,0);
}

void CDynamicCscopeWnd::OnSize(UINT nType, int cx, int cy)
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnSize(nType, cx, cy);

	//integration
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"DynamicCscope",iResHorz,iResVert,wndPlacement,0);
	//switch (nType)
	//{
	//	case SIZE_MAXIMIZED:
	//		if (g_pDynamicCscope)
	//			g_pDynamicCscope->SetEvent(1);
	//		
	//		break;
	//	case SIZE_MINIMIZED:
	//		if (g_pDynamicCscope)
	//			g_pDynamicCscope->SetEvent(1);

	//		break;
	//	//case SIZE_RESTORED:
	//	//	if (g_pDynamicCscope)
	//	//		g_pDynamicCscope->SetEvent(1);

	//	//	break;
	//}
}

void CDynamicCscopeWnd::OnSizing(UINT fwSide, LPRECT pRect)
{
	CMDIChildWndEx::OnSizing(fwSide, pRect);

	if (g_pDynamicCscope)
		g_pDynamicCscope->OnSizing(fwSide, pRect);
}

void CDynamicCscopeWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (g_pDynamicCscope)
		g_pDynamicCscope->OnLButtonUp(nFlags, point);

	CMDIChildWndEx::OnLButtonUp(nFlags, point);
}

void CDynamicCscopeWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CMDIChildWndEx::OnNcLButtonUp(nHitTest, point);
}

LRESULT CDynamicCscopeWnd::OnExitSizeMove(WPARAM wParam, LPARAM lParam)
{
	//if (g_pDynamicCscope)
	//	g_pDynamicCscope->SetEvent(1);

	return 0;
}