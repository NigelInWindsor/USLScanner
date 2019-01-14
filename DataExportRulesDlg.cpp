// DataExportRulesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "DataExportRulesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataExportRulesDlg dialog


CDataExportRulesDlg::CDataExportRulesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CDataExportRulesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataExportRulesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
}


void CDataExportRulesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataExportRulesDlg)
	DDX_Control(pDX, IDC_CHECK_TWIST, m_checkTwist);
	DDX_Control(pDX, IDC_COMBO_COORDINATE_FRAME, m_comboCoordinateFrame);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataExportRulesDlg, CDialog)
	//{{AFX_MSG_MAP(CDataExportRulesDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_COORDINATE_FRAME, OnSelchangeComboCoordinateFrame)
	ON_BN_CLICKED(IDC_CHECK_TWIST, OnCheckTwist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataExportRulesDlg message handlers

void CDataExportRulesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CDataExportRulesDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CDataExportRulesDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CDataExportRulesDlg::OnDestroy() 
{
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[CDataExportRulesDlg::IDD % MAXIMUMWINDOWSIDNUMBER]);

	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CDataExportRulesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	Buff;

	CRect rect = theApp.m_LastSettings.RectWindowPos[CDataExportRulesDlg::IDD % MAXIMUMWINDOWSIDNUMBER];
	theApp.ConfineToScreen(&rect);
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);

	Buff.LoadString(IDS_Scanner);	m_comboCoordinateFrame.AddString(Buff);
	Buff.LoadString(IDS_CAD);		m_comboCoordinateFrame.AddString(Buff);

	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataExportRulesDlg::UpdateAllControls()
{
	if(theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame & EXPORT_CAD_COORDINATES) {
		m_comboCoordinateFrame.SetCurSel(1);
	} else {
		m_comboCoordinateFrame.SetCurSel(0);
	}
	if(theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame & EXPORT_TWIST_COORDINATES) {
		m_checkTwist.SetCheck(true);
	} else {
		m_checkTwist.SetCheck(false);
	}

}

void CDataExportRulesDlg::OnSelchangeComboCoordinateFrame() 
{
	if(m_comboCoordinateFrame.GetCurSel() == 0) {
		theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame &= ~EXPORT_CAD_COORDINATES;
	} else {
		theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame |= EXPORT_CAD_COORDINATES;
	}
}

void CDataExportRulesDlg::OnCheckTwist() 
{
	if(m_checkTwist.GetCheck()) {
		theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame |= EXPORT_TWIST_COORDINATES;
	} else {
		theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame &= ~EXPORT_TWIST_COORDINATES;
	}
}
