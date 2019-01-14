// PermissionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PermissionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPermissionDlg dialog


CPermissionDlg::CPermissionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPermissionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPermissionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bAcceptable = FALSE;
}


void CPermissionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPermissionDlg)
	DDX_Control(pDX, IDC_EDIT_CODE, m_editCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPermissionDlg, CDialog)
	//{{AFX_MSG_MAP(CPermissionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPermissionDlg message handlers

BOOL CPermissionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPermissionDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CPermissionDlg::OnOK() 
{
	CString Buff;

	m_editCode.GetWindowText(Buff);

	if(Buff == "0347") {
		m_bAcceptable=TRUE;
	} else {
		m_editCode.SetWindowText(NULL);
		GotoDlgCtrl((CEdit*) GetDlgItem(IDC_EDIT_CODE));
		return;
	}
	
	CDialog::OnOK();
}
