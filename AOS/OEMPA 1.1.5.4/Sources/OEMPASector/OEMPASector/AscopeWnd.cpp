// AscopeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SectorApp.h"
#include "AscopeWnd.h"
#include "UTKernelLayoutFile.h"


// CAscopeWnd

IMPLEMENT_DYNCREATE(CAscopeWnd, CMDIChildWnd)

CAscopeWnd::CAscopeWnd()
{
	m_iIndexLayoutFile = -1;
}

CAscopeWnd::~CAscopeWnd()
{
}


BEGIN_MESSAGE_MAP(CAscopeWnd, CMDIChildWnd)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CAscopeWnd message handlers


int CAscopeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_Toolbar.Create(this, IDD_DIALOG_ASCOPE_TOOLBAR, CBRS_TOP, IDD_DIALOG_ASCOPE_TOOLBAR))
	{
		TRACE0("Failed to create A-scan Toolbar");
		return -1;
	}

	return 0;
}

BOOL CAscopeWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{//integration
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;

	if(m_iIndexLayoutFile<0)
	if(UTKernel_LayoutFile()->RegisterWindow(L"Ascope",m_iIndexLayoutFile,bRegistered,bCreateNew) && bRegistered)
	{
		if(UTKernel_LayoutFile()->ReadPositionWindow(m_iIndexLayoutFile,L"Ascope",sysTime,iResHorz,iResVert,wndPlacement,iData))
		{
			if(wndPlacement.showCmd==SW_MAXIMIZE)
				MDIMaximize();
			else if(UTKernel_LayoutFile()->CheckPlacement(wndPlacement))
				SetWindowPlacement(&wndPlacement);
		}
	}

	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CAscopeWnd::OnMove(int x, int y)
{//integration
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWnd::OnMove(x, y);
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"Ascope",iResHorz,iResVert,wndPlacement,0);
}

void CAscopeWnd::OnSize(UINT nType, int cx, int cy)
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CMDIChildWnd::OnSize(nType, cx, cy);

	//integration
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),0,NULL,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,L"Ascope",iResHorz,iResVert,wndPlacement,0);
}