// FLatPackToolsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "FLatPackToolsDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFLatPackToolsDlg dialog


CFLatPackToolsDlg::CFLatPackToolsDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CFLatPackToolsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFLatPackToolsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	
	m_pDlg = pDlg;
}


void CFLatPackToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFLatPackToolsDlg)
	DDX_Control(pDX, IDC_EDIT_PTS_PER_LINE, m_editPtsPerLine);
	DDX_Control(pDX, IDC_EDIT_NUMBER_LINES, m_editNumberLines);
	DDX_Control(pDX, IDC_EDIT_DELTA_WP, m_editWaterPathVariation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFLatPackToolsDlg, CDialog)
	//{{AFX_MSG_MAP(CFLatPackToolsDlg)
	ON_EN_CHANGE(IDC_EDIT_DELTA_WP, OnChangeEditDeltaWp)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_WP, OnButtonWp)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, OnButtonGenerate)
	ON_EN_CHANGE(IDC_EDIT_PTS_PER_LINE, OnChangeEditPtsPerLine)
	ON_EN_CHANGE(IDC_EDIT_NUMBER_LINES, OnChangeEditNumberLines)
	ON_BN_CLICKED(IDC_BUTTON_NORMALS, OnButtonNormals)
	ON_BN_CLICKED(IDC_BUTTON_ALL_NORMALS, OnButtonAllNormals)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFLatPackToolsDlg message handlers


void CFLatPackToolsDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CFLatPackToolsDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CFLatPackToolsDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CFLatPackToolsDlg::OnDestroy() 
{
	if (CFLatPackToolsDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CFLatPackToolsDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
}

BOOL CFLatPackToolsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (CFLatPackToolsDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CFLatPackToolsDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CFLatPackToolsDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(_T("%.01f mm"),theApp.m_LastSettings.fWpVariation);
	m_editWaterPathVariation.SetWindowText(Buff);

	Buff.Format(_T("%.0f"),theApp.m_LastSettings.fMeshFastSize);
	m_editPtsPerLine.SetWindowText(Buff);

	Buff.Format(_T("%.0f"),theApp.m_LastSettings.fMeshSlowSize);
	m_editNumberLines.SetWindowText(Buff);

}

void CFLatPackToolsDlg::OnChangeEditDeltaWp() 
{
	CString Buff;

	m_editWaterPathVariation.GetWindowText(Buff);

	_WTOF(Buff,theApp.m_LastSettings.fWpVariation);
}

void CFLatPackToolsDlg::OnButtonWp() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = TRACK_ALONG_LINE;
}


void CFLatPackToolsDlg::OnChangeEditPtsPerLine() 
{
	CString Buff;

	m_editPtsPerLine.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fMeshFastSize);
}

void CFLatPackToolsDlg::OnChangeEditNumberLines() 
{
	CString Buff;

	m_editNumberLines.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fMeshSlowSize);
}

void CFLatPackToolsDlg::OnButtonGenerate() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	PROFILE->GenerateFlatPackMesh();
	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG, NULL, NULL);
}

void CFLatPackToolsDlg::OnButtonNormals() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	PROFILE->CalculateMathematicalNormals();
	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG, NULL,NULL);
}

void CFLatPackToolsDlg::OnButtonAllNormals() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CCoord Cp;

	theApp.m_Motors.GetSurfacePos(&Cp);
	PROFILE->SetAllNormals(SCANLINES,Cp.Side0.norm);
	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG, NULL, NULL);
}
