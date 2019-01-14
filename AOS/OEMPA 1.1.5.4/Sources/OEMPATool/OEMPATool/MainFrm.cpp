// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DlgHW.h"
#include "DlgSetupFiles.h"
#include "DlgCscanDisplay.h"
#include "DlgKernelMsgBox.h"
#include "DlgTestFWMemory.h"

#include "MainFrm.h"
#include "UTKernelLayoutFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_COMMAND(ID_EDIT_DIALOG_HARDWARE, &CMainFrame::OnEditHWDialog)
	ON_COMMAND(ID_EDIT_SETUPFILES, &CMainFrame::OnEditSetupFilesDialog)
	ON_COMMAND(ID_EDIT_MAXDIALOG, &CMainFrame::OnEditMaxdialog)
	ON_COMMAND(ID_EDIT_KERNELMESSAGEBOX, &CMainFrame::OnEditKernelmessagebox)
	ON_COMMAND(ID_EDIT_TESTFWMEMORY, &CMainFrame::OnEditTestfwmemory)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TESTFWMEMORY, &CMainFrame::OnUpdateEditTestfwmemory)
END_MESSAGE_MAP()


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_iIndexLayoutFile = -1;
#ifdef _EVALUATION_
	CUTPreference *pPreference=CUTPreference::GetCurrentPreference();
	if(pPreference)
	{//to load automatically the setup at connection time.
		pPreference->SetDefaultSetupAuto(true);
	}
#endif _EVALUATION_
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;
	CString string;

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	if(m_iIndexLayoutFile<0)
	if(UTKernel_LayoutFile()->RegisterMainWnd(m_iIndexLayoutFile,bRegistered,bCreateNew) && bRegistered)
	{
		if(UTKernel_LayoutFile()->ReadPositionMainWnd(m_iIndexLayoutFile,sysTime,iResHorz,iResVert,wndPlacement,iData))
			SetWindowPlacement(&wndPlacement);
	}

	if(IsWow64())
		string.Format(L"OEMPATool (x86, PID %d)",GetCurrentProcessId());
	else
		string.Format(L"OEMPATool (PID %d)",GetCurrentProcessId());
	SetWindowText(string);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

__declspec(dllimport) bool WINAPI OEMPA_Test(bool bEnable,int iPort);
void CMainFrame::OnEditHWDialog()
{
	CDlgHW::Display();
}

void CMainFrame::OnEditSetupFilesDialog()
{
	OnEditHWDialog();//required to initialize the device id.
	CDlgSetupFiles::Display(CDlgHW::GetDeviceId());
}

void CMainFrame::OnEditMaxdialog()
{
	CDlgCscanDisplay::Display();
}

void CMainFrame::OnEditKernelmessagebox()
{
	CDlgKernelMsgBox::Display();
}

void CMainFrame::OnEditTestfwmemory()
{
	CDlgTestFWMemory dlg(CDlgHW::GetKernelDriverOEMPA());

	dlg.DoModal();
}

void CMainFrame::OnUpdateEditTestfwmemory(CCmdUI *pCmdUI)
{
	SHORT nState;
	nState = GetKeyState(VK_CONTROL);
	if(nState&0x8000)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnClose()
{
	CDlgHW::Destroy();
	CFrameWnd::OnClose();
}

void CMainFrame::OnMove(int x, int y)
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;
	CFrameWnd::OnMove(x, y);

	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemMainWnd(GetSafeHwnd(),iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionMainWnd(m_iIndexLayoutFile,iResHorz,iResVert,wndPlacement,0);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CFrameWnd::OnSize(nType, cx, cy);
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemMainWnd(GetSafeHwnd(),iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionMainWnd(m_iIndexLayoutFile,iResHorz,iResVert,wndPlacement,0);
}
