// CscopeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CscopeWnd.h"
#include "SectorApp.h"
#include "CscopeView.h"
#include "UTKernelLayoutFile.h"

// CCscopeWnd

IMPLEMENT_DYNCREATE(CCscopeWnd, CMDIChildWndEx)

CCscopeWnd::CCscopeWnd()
{
	m_iIndexLayoutFile = -1;
}

CCscopeWnd::~CCscopeWnd()
{
}

BEGIN_MESSAGE_MAP(CCscopeWnd, CMDIChildWndEx)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONUP()
	ON_WM_NCLBUTTONUP()
	ON_MESSAGE(WM_EXITSIZEMOVE, &CCscopeWnd::OnExitSizeMove)
END_MESSAGE_MAP()

// CCscopeWnd message handlers

BOOL CCscopeWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

BOOL CCscopeWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{//integration
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;

	if(m_iIndexLayoutFile<0)
	if(UTKernel_LayoutFile()->RegisterWindow(L"Cscope",m_iIndexLayoutFile,bRegistered,bCreateNew) && bRegistered)
	{
		if(UTKernel_LayoutFile()->ReadPositionWindow(m_iIndexLayoutFile,L"Cscope",sysTime,iResHorz,iResVert,wndPlacement,iData))
		{
			if(wndPlacement.showCmd==SW_MAXIMIZE)
				MDIMaximize();
			else if(UTKernel_LayoutFile()->CheckPlacement(wndPlacement))
				SetWindowPlacement(&wndPlacement);
		}
	}

	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}

void CCscopeWnd::OnMove(int x, int y)
{//integration
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnMove(x, y);
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"Cscope",iResHorz,iResVert,wndPlacement,0);
}

void CCscopeWnd::OnSize(UINT nType, int cx, int cy)
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnSize(nType, cx, cy);

	//integration
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"Cscope",iResHorz,iResVert,wndPlacement,0);
	switch (nType)
	{
		case SIZE_MAXIMIZED:
			if (g_pCscope)
				g_pCscope->SetEvent(1);
			
			break;
		case SIZE_MINIMIZED:
			if (g_pCscope)
				g_pCscope->SetEvent(1);

			break;
		//case SIZE_RESTORED:
		//	if (g_pCscope)
		//		g_pCscope->SetEvent(1);

		//	break;
	}
}

void CCscopeWnd::OnSizing(UINT fwSide, LPRECT pRect)
{
	CMDIChildWndEx::OnSizing(fwSide, pRect);

	if (g_pCscope)
		g_pCscope->OnSizing(fwSide, pRect);
}

void CCscopeWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (g_pCscope)
		g_pCscope->OnLButtonUp(nFlags, point);

	CMDIChildWndEx::OnLButtonUp(nFlags, point);
}

void CCscopeWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CMDIChildWndEx::OnNcLButtonUp(nHitTest, point);
}

LRESULT CCscopeWnd::OnExitSizeMove(WPARAM wParam, LPARAM lParam)
{
	if (g_pCscope)
		g_pCscope->SetEvent(1);

	return 0;
}