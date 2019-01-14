// TranslatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "TranslatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTranslatDlg dialog


CTranslatDlg::CTranslatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTranslatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTranslatDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_fXDev = 0.0f;
	m_fYDev = 0.0f;
	m_fZDev = 0.0f;

}


void CTranslatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTranslatDlg)
	DDX_Control(pDX, IDC_EDIT_Z, m_editZDev);
	DDX_Control(pDX, IDC_EDIT_Y, m_editYDev);
	DDX_Control(pDX, IDC_EDIT_X, m_editXDev);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTranslatDlg, CDialog)
	//{{AFX_MSG_MAP(CTranslatDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTranslatDlg message handlers

BOOL CTranslatDlg::OnInitDialog() 
{
	CString	Buff;
	CDialog::OnInitDialog();
	
	Buff.Format(_T("%.01f mm"),m_fXDev);
	m_editXDev.SetWindowText(Buff);
	Buff.Format(_T("%.01f mm"),m_fYDev);
	m_editYDev.SetWindowText(Buff);
	Buff.Format(_T("%.01f mm"),m_fZDev);
	m_editZDev.SetWindowText(Buff);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTranslatDlg::OnOK() 
{
	CString Buff;
	
	m_editXDev.GetWindowText(Buff);
	_WTOF(Buff,m_fXDev);
	m_editYDev.GetWindowText(Buff);
	_WTOF(Buff,m_fYDev);
	m_editZDev.GetWindowText(Buff);
	_WTOF(Buff,m_fZDev);

	CDialog::OnOK();
}
