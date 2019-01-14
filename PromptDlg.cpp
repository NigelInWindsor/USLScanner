// PromptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PromptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPromptDlg dialog


CPromptDlg::CPromptDlg(CWnd* pParent /*=NULL*/,CString Text,CString strWindowName, bool bShowVariableRequest)
	: CDialog(CPromptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPromptDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_Text = Text;
	m_WindowName = strWindowName;
	m_bShowVariableRequest = bShowVariableRequest;
}


void CPromptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPromptDlg)
	DDX_Control(pDX, IDC_EDIT_VARIABLE, m_editVariable);
	DDX_Control(pDX, IDC_STATIC_EDIT, m_editText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPromptDlg, CDialog)
	//{{AFX_MSG_MAP(CPromptDlg)
	ON_EN_CHANGE(IDC_EDIT_VARIABLE, OnChangeEditVariable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPromptDlg message handlers

BOOL CPromptDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_editText.SetWindowText(m_Text);
	m_editVariable.ShowWindow(m_bShowVariableRequest);
	
	SetWindowText(m_WindowName);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPromptDlg::SetText(CString Buff)
{

	m_Text = Buff;
	m_editText.SetWindowText(m_Text);
}

void CPromptDlg::OnChangeEditVariable() 
{
	m_editVariable.GetWindowText(m_strVariable);
}
