// NonCadVariablesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "NonCadVariablesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNonCadVariablesDlg dialog


CNonCadVariablesDlg::CNonCadVariablesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CNonCadVariablesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNonCadVariablesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pDlg = pDlg;

	m_nStartLine = 0;
	m_nFinishLine = PROFILE->m_nScanLineL - 1;
	

}


void CNonCadVariablesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNonCadVariablesDlg)
	DDX_Control(pDX, IDC_CHECK_RIGHT_WP, m_checkRightWp);
	DDX_Control(pDX, IDC_CHECK_LEFT_WP, m_checkLeftWp);
	DDX_Control(pDX, IDC_EDIT_WP_RIGHT_1, m_editWpRightFinish);
	DDX_Control(pDX, IDC_EDIT_WP_RIGHT_0, m_editWpRightStart);
	DDX_Control(pDX, IDC_EDIT_WP_LEFT_1, m_editWpLeftFinish);
	DDX_Control(pDX, IDC_EDIT_WP_LEFT_0, m_editWpLeftStart);
	DDX_Control(pDX, IDC_EDIT_LINE_1, m_editFinishLine);
	DDX_Control(pDX, IDC_EDIT_LINE_0, m_editStartLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNonCadVariablesDlg, CDialog)
	//{{AFX_MSG_MAP(CNonCadVariablesDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNonCadVariablesDlg message handlers

void CNonCadVariablesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
}

void CNonCadVariablesDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CNonCadVariablesDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CNonCadVariablesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNonCadVariablesDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format(_T("%.1f mm"),theApp.m_Kinematics.m_fDesiredWaterPath[PORTSIDE]);
	m_editWpLeftStart.SetWindowText(Buff);
	m_editWpLeftFinish.SetWindowText(Buff);

	Buff.Format(_T("%.1f mm"),theApp.m_Kinematics.m_fDesiredWaterPath[STARBOARD]);
	m_editWpRightStart.SetWindowText(Buff);
	m_editWpRightFinish.SetWindowText(Buff);

	Buff.Format(_T("%d"),m_nStartLine + 1);
	m_editStartLine.SetWindowText(Buff);
	Buff.Format(_T("%d"),m_nFinishLine + 1);
	m_editFinishLine.SetWindowText(Buff);

}

void CNonCadVariablesDlg::OnButtonApply() 
{
	CString Buff;

	m_editStartLine.GetWindowText(Buff);
	m_nStartLine = _ttoi(Buff) -1;
	m_editFinishLine.GetWindowText(Buff);
	m_nFinishLine = _ttoi(Buff) -1;

	if((m_nStartLine < 0) || (m_nStartLine>=PROFILE->m_nScanLineL)) {
		MessageBox(_T("Start line is invalid"),_T("Error"),MB_ICONERROR);
		return;
	}
	if((m_nFinishLine < 0) || (m_nFinishLine>=PROFILE->m_nScanLineL)) {
		MessageBox(_T("Finish line is invalid"),_T("Error"),MB_ICONERROR);
		return;
	}
	
	m_nLeftWp = m_checkLeftWp.GetCheck()&1;
	m_nRightWp = m_checkRightWp.GetCheck()&1;

	m_editWpLeftStart.GetWindowText(Buff);
	_WTOF(Buff,m_fLeftWp[0]); 
	m_editWpLeftFinish.GetWindowText(Buff);
	_WTOF(Buff,m_fLeftWp[1]); 

	m_editWpRightStart.GetWindowText(Buff);
	_WTOF(Buff,m_fRightWp[0]); 
	m_editWpRightFinish.GetWindowText(Buff);
	_WTOF(Buff,m_fRightWp[1]); 

	PROFILE->ModifyCoordWp(SCANLINES,m_nStartLine,m_nFinishLine,m_nLeftWp,m_fLeftWp[0],m_fLeftWp[1],m_nRightWp,m_fRightWp[0],m_fRightWp[1]);

	GetParent()->Invalidate(FALSE);
	
}
