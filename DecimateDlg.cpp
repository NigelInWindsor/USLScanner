// DecimateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "DecimateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDecimateDlg dialog


CDecimateDlg::CDecimateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDecimateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDecimateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nMaxPts = 10;
}


void CDecimateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDecimateDlg)
	DDX_Control(pDX, IDC_COMBO_MAX_PTS, m_comboMaxPts);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDecimateDlg, CDialog)
	//{{AFX_MSG_MAP(CDecimateDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDecimateDlg message handlers
BOOL CDecimateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_comboMaxPts.AddString(_T("2"));	
	m_comboMaxPts.AddString(_T("3"));	
	m_comboMaxPts.AddString(_T("4"));	
	m_comboMaxPts.AddString(_T("5"));	
	m_comboMaxPts.AddString(_T("6"));	
	m_comboMaxPts.AddString(_T("7"));	
	m_comboMaxPts.AddString(_T("8"));	
	m_comboMaxPts.AddString(_T("9"));	
	m_comboMaxPts.AddString(_T("10"));	
	m_comboMaxPts.AddString(_T("20"));	
	m_comboMaxPts.AddString(_T("50"));	
	m_comboMaxPts.AddString(_T("100"));	
	m_comboMaxPts.SetCurSel(2);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDecimateDlg::OnOK() 
{
	CString Buff;
	
	m_comboMaxPts.GetWindowText(Buff);
	m_nMaxPts = _ttoi(Buff);

	CDialog::OnOK();
}

