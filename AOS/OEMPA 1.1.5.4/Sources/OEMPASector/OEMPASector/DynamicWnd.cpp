// DynamicWnd.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicWnd.h"
#include "SectorApp.h"
#include "UTKernelLayoutFile.h"

// CDynamicWnd

IMPLEMENT_DYNCREATE(CDynamicWnd, CMDIChildWndEx)

CDynamicWnd::CDynamicWnd()
{
	m_iIndexLayoutFile = -1;
}

CDynamicWnd::~CDynamicWnd()
{
}

BEGIN_MESSAGE_MAP(CDynamicWnd, CMDIChildWndEx)
	ON_WM_MOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CDynamicWnd message handlers

BOOL CDynamicWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

BOOL CDynamicWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{//integration
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;

	if(m_iIndexLayoutFile<0)
	if(UTKernel_LayoutFile()->RegisterWindow(L"Dynamic",m_iIndexLayoutFile,bRegistered,bCreateNew) && bRegistered)
	{
		if(UTKernel_LayoutFile()->ReadPositionWindow(m_iIndexLayoutFile,L"Dynamic",sysTime,iResHorz,iResVert,wndPlacement,iData))
		{
			if(wndPlacement.showCmd==SW_MAXIMIZE)
				MDIMaximize();
			else if(UTKernel_LayoutFile()->CheckPlacement(wndPlacement))
				SetWindowPlacement(&wndPlacement);
		}
	}

	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}

void CDynamicWnd::OnMove(int x, int y)
{//integration
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnMove(x, y);
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"Dynamic",iResHorz,iResVert,wndPlacement,0);
}

void CDynamicWnd::OnSize(UINT nType, int cx, int cy)
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnSize(nType, cx, cy);

	//integration
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"Dynamic",iResHorz,iResVert,wndPlacement,0);
}