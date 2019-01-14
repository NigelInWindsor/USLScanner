// DynamicNCWnd.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicNCWnd.h"
#include "SectorApp.h"
#include "UTKernelLayoutFile.h"

// CDynamicNCWnd

IMPLEMENT_DYNCREATE(CDynamicNCWnd, CMDIChildWndEx)

CDynamicNCWnd::CDynamicNCWnd()
{
	m_iIndexLayoutFile = -1;
}

CDynamicNCWnd::~CDynamicNCWnd()
{
}

BEGIN_MESSAGE_MAP(CDynamicNCWnd, CMDIChildWndEx)
	ON_WM_MOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CDynamicNCWnd message handlers

BOOL CDynamicNCWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

BOOL CDynamicNCWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{//integration
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;

	if(m_iIndexLayoutFile<0)
	if(UTKernel_LayoutFile()->RegisterWindow(L"DynamicNC",m_iIndexLayoutFile,bRegistered,bCreateNew) && bRegistered)
	{
		if(UTKernel_LayoutFile()->ReadPositionWindow(m_iIndexLayoutFile,L"DynamicNC",sysTime,iResHorz,iResVert,wndPlacement,iData))
		{
			if(wndPlacement.showCmd==SW_MAXIMIZE)
				MDIMaximize();
			else if(UTKernel_LayoutFile()->CheckPlacement(wndPlacement))
				SetWindowPlacement(&wndPlacement);
		}
	}
	SetWindowText(L"DynamicNC");

	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}

void CDynamicNCWnd::OnMove(int x, int y)
{//integration
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnMove(x, y);
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"DynamicNC",iResHorz,iResVert,wndPlacement,0);
}

void CDynamicNCWnd::OnSize(UINT nType, int cx, int cy)
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWndEx::OnSize(nType, cx, cy);

	//integration
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"DynamicNC",iResHorz,iResVert,wndPlacement,0);
}