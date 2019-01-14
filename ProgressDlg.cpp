// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog


CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressDlg)
	m_stringText = _T("");
	//}}AFX_DATA_INIT

	m_nMin = 0;
	m_nMax=100;

	m_stringText = "Hello";

}


void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDlg)
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_progressBar);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_stringText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CProgressDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg message handlers

BOOL CProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_progressBar.SetRange(m_nMin,m_nMax);
	m_progressBar.SetPos(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgressDlg::SetProgressPos(int nPos)
{
	m_progressBar.SetPos(nPos);
	UpdateData(FALSE);
}

void CProgressDlg::SetProgressRange(int nMin, int nMax)
{
	m_nMin=nMin;
	m_nMax=nMax;
	m_progressBar.SetRange32(m_nMin,m_nMax);
}

void CProgressDlg::SetProgressText(CString String)
{
	m_stringText=String;
	UpdateData(FALSE);

}
