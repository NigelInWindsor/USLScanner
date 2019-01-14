// BscopeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BscopeWnd.h"
#include "SectorApp.h"
#include "BscopeView.h"
#include "UTKernelLayoutFile.h"

// CBscopeWnd

IMPLEMENT_DYNCREATE(CBscopeWnd, CMDIChildWndEx)

CBscopeWnd::CBscopeWnd()
{
	m_iIndexLayoutFile = -1;
}

CBscopeWnd::~CBscopeWnd()
{
}

BEGIN_MESSAGE_MAP(CBscopeWnd, CMDIChildWndEx)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONUP()
	ON_WM_NCLBUTTONUP()
	ON_MESSAGE(WM_EXITSIZEMOVE, &CBscopeWnd::OnExitSizeMove)
END_MESSAGE_MAP()

// CBscopeWnd message handlers

BOOL CBscopeWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

BOOL CBscopeWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{//integration
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;

	if(m_iIndexLayoutFile<0)
	if(UTKernel_LayoutFile()->RegisterWindow(L"Bsccope",m_iIndexLayoutFile,bRegistered,bCreateNew) && bRegistered)
	{
		if(UTKernel_LayoutFile()->ReadPositionWindow(m_iIndexLayoutFile,L"Bsccope",sysTime,iResHorz,iResVert,wndPlacement,iData))
		{
			if(wndPlacement.showCmd==SW_MAXIMIZE)
				MDIMaximize();
			else if(UTKernel_LayoutFile()->CheckPlacement(wndPlacement))
				SetWindowPlacement(&wndPlacement);
		}
	}

	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}

void CBscopeWnd::OnMove(int x, int y)
{//integration
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnMove(x, y);
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"Bsccope",iResHorz,iResVert,wndPlacement,0);
}

void CBscopeWnd::OnSize(UINT nType, int cx, int cy)
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnSize(nType, cx, cy);

	//integration
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"Bsccope",iResHorz,iResVert,wndPlacement,0);
	switch (nType)
	{
		case SIZE_MAXIMIZED:
			if (g_pBscope)
				g_pBscope->SetEvent(1);
			
			break;
		case SIZE_MINIMIZED:
			if (g_pBscope)
				g_pBscope->SetEvent(1);

			break;
		//case SIZE_RESTORED:
		//	if (g_pBscope)
		//		g_pBscope->SetEvent(1);

		//	break;
	}
}

void CBscopeWnd::OnSizing(UINT fwSide, LPRECT pRect)
{
	CMDIChildWndEx::OnSizing(fwSide, pRect);

	if (g_pBscope)
		g_pBscope->OnSizing(fwSide, pRect);
}

void CBscopeWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (g_pBscope)
		g_pBscope->OnLButtonUp(nFlags, point);

	CMDIChildWndEx::OnLButtonUp(nFlags, point);
}

void CBscopeWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CMDIChildWndEx::OnNcLButtonUp(nHitTest, point);
}

LRESULT CBscopeWnd::OnExitSizeMove(WPARAM wParam, LPARAM lParam)
{
	if (g_pBscope)
		g_pBscope->SetEvent(1);

	return 0;
}