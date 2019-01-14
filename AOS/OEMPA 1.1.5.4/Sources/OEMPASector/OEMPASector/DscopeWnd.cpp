// DscopeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "DscopeWnd.h"
#include "SectorApp.h"
#include "DscopeView.h"
#include "UTKernelLayoutFile.h"

// CDscopeWnd

IMPLEMENT_DYNCREATE(CDscopeWnd, CMDIChildWndEx)

CDscopeWnd::CDscopeWnd()
{
	m_iIndexLayoutFile = -1;
}

CDscopeWnd::~CDscopeWnd()
{
}

BEGIN_MESSAGE_MAP(CDscopeWnd, CMDIChildWndEx)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONUP()
	ON_WM_NCLBUTTONUP()
	ON_MESSAGE(WM_EXITSIZEMOVE, &CDscopeWnd::OnExitSizeMove)
END_MESSAGE_MAP()

// CDscopeWnd message handlers

BOOL CDscopeWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

BOOL CDscopeWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{//integration
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;

	if(m_iIndexLayoutFile<0)
	if(UTKernel_LayoutFile()->RegisterWindow(L"Dscope",m_iIndexLayoutFile,bRegistered,bCreateNew) && bRegistered)
	{
		if(UTKernel_LayoutFile()->ReadPositionWindow(m_iIndexLayoutFile,L"Dscope",sysTime,iResHorz,iResVert,wndPlacement,iData))
		{
			if(wndPlacement.showCmd==SW_MAXIMIZE)
				MDIMaximize();
			else if(UTKernel_LayoutFile()->CheckPlacement(wndPlacement))
				SetWindowPlacement(&wndPlacement);
		}
	}

	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}

void CDscopeWnd::OnMove(int x, int y)
{//integration
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnMove(x, y);
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"Dscope",iResHorz,iResVert,wndPlacement,0);
}

void CDscopeWnd::OnSize(UINT nType, int cx, int cy)
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnSize(nType, cx, cy);

	//integration
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"Dscope",iResHorz,iResVert,wndPlacement,0);
	switch (nType)
	{
		case SIZE_MAXIMIZED:
			if (g_pDscope)
				g_pDscope->SetEvent(1);
			
			break;
		case SIZE_MINIMIZED:
			if (g_pDscope)
				g_pDscope->SetEvent(1);

			break;
		//case SIZE_RESTORED:
		//	if (g_pDscope)
		//		g_pDscope->SetEvent(1);

		//	break;
	}
}

void CDscopeWnd::OnSizing(UINT fwSide, LPRECT pRect)
{
	CMDIChildWndEx::OnSizing(fwSide, pRect);

	if (g_pDscope)
		g_pDscope->OnSizing(fwSide, pRect);
}

void CDscopeWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (g_pDscope)
		g_pDscope->OnLButtonUp(nFlags, point);

	CMDIChildWndEx::OnLButtonUp(nFlags, point);
}

void CDscopeWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CMDIChildWndEx::OnNcLButtonUp(nHitTest, point);
}

LRESULT CDscopeWnd::OnExitSizeMove(WPARAM wParam, LPARAM lParam)
{
	if (g_pDscope)
		g_pDscope->SetEvent(1);

	return 0;
}