// BscopeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "DscopeCorrectedWnd.h"
#include "SectorApp.h"
#include "DscopeCorrectedView.h"
#include "UTKernelLayoutFile.h"

// CDscopeCorrectedWnd

IMPLEMENT_DYNCREATE(CDscopeCorrectedWnd, CMDIChildWndEx)

CDscopeCorrectedWnd::CDscopeCorrectedWnd()
{
	m_iIndexLayoutFile = -1;
}

CDscopeCorrectedWnd::~CDscopeCorrectedWnd()
{
}

BEGIN_MESSAGE_MAP(CDscopeCorrectedWnd, CMDIChildWndEx)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_LBUTTONUP()
	ON_WM_NCLBUTTONUP()
	ON_MESSAGE(WM_EXITSIZEMOVE, &CDscopeCorrectedWnd::OnExitSizeMove)
END_MESSAGE_MAP()

// CDscopeCorrectedWnd message handlers

BOOL CDscopeCorrectedWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

BOOL CDscopeCorrectedWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{//integration
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;

	if(m_iIndexLayoutFile<0)
	if(UTKernel_LayoutFile()->RegisterWindow(L"Bscope (//)",m_iIndexLayoutFile,bRegistered,bCreateNew) && bRegistered)
	{
		if(UTKernel_LayoutFile()->ReadPositionWindow(m_iIndexLayoutFile,L"Bscope (//)",sysTime,iResHorz,iResVert,wndPlacement,iData))
		{
			if(wndPlacement.showCmd==SW_MAXIMIZE)
				MDIMaximize();
			else if(UTKernel_LayoutFile()->CheckPlacement(wndPlacement))
				SetWindowPlacement(&wndPlacement);
		}
	}

	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}

void CDscopeCorrectedWnd::OnMove(int x, int y)
{//integration
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnMove(x, y);
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"Bscope (//)",iResHorz,iResVert,wndPlacement,0);
}

void CDscopeCorrectedWnd::OnSize(UINT nType, int cx, int cy)
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnSize(nType, cx, cy);

	//integration
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"Bscope (//)",iResHorz,iResVert,wndPlacement,0);
	switch (nType)
	{
		case SIZE_MAXIMIZED:
			if (g_pDscopeCorrected)
				g_pDscopeCorrected->SetEvent(1);
			
			break;
		case SIZE_MINIMIZED:
			if (g_pDscopeCorrected)
				g_pDscopeCorrected->SetEvent(1);

			break;
		//case SIZE_RESTORED:
		//	if (g_pDscopeCorrected)
		//		g_pDscopeCorrected->SetEvent(1);

		//	break;
	}
}

void CDscopeCorrectedWnd::OnSizing(UINT fwSide, LPRECT pRect)
{
	CMDIChildWndEx::OnSizing(fwSide, pRect);

	if (g_pDscopeCorrected)
		g_pDscopeCorrected->OnSizing(fwSide, pRect);
}

void CDscopeCorrectedWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (g_pDscopeCorrected)
		g_pDscopeCorrected->OnLButtonUp(nFlags, point);

	CMDIChildWndEx::OnLButtonUp(nFlags, point);
}

void CDscopeCorrectedWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CMDIChildWndEx::OnNcLButtonUp(nHitTest, point);
}

LRESULT CDscopeCorrectedWnd::OnExitSizeMove(WPARAM wParam, LPARAM lParam)
{
	if (g_pDscopeCorrected)
		g_pDscopeCorrected->SetEvent(1);

	return 0;
}