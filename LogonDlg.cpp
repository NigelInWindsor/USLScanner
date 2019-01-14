// LogonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "LogonDlg.h"
#include "Users.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogonDlg dialog


CLogonDlg::CLogonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogonDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLogonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogonDlg)
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogonDlg, CDialog)
	//{{AFX_MSG_MAP(CLogonDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogonDlg message handlers

void CLogonDlg::OnOK() 
{
	CString Name,Password;
	CUsers Users;
	CString	Buff,Error;

	m_editName.GetWindowText(Name);
	m_editPassword.GetWindowText(Password);

	if(Users.Logon(Name,Password) != TRUE) {

		Buff.LoadString(IDS_Invalid_Name_or_Password);
		Error.LoadString(IDS_Logon_Failure);

		MessageBox(Buff,Error,MB_ICONERROR);
		m_editPassword.SetWindowText(_T(""));

		return;
	}

	CDialog::OnOK();
}

void CLogonDlg::OnCancel() 
{
	theApp.m_nLogonLevel = ANYONE;
	theApp.m_nLogonLevelMask=0;
	theApp.m_Operator = "Any One";

	CDialog::OnCancel();
}
