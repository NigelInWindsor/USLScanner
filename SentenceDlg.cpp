// SentenceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "SentenceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSentenceDlg dialog


CSentenceDlg::CSentenceDlg(CWnd* pParent /*=NULL*/,CData *pData)
	: CDialog(CSentenceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSentenceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pData = pData;
}


void CSentenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSentenceDlg)
	DDX_Control(pDX, IDC_COMBO_SENTENCE, m_comboSentence);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSentenceDlg, CDialog)
	//{{AFX_MSG_MAP(CSentenceDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSentenceDlg message handlers

BOOL CSentenceDlg::OnInitDialog() 
{
	
	CDialog::OnInitDialog();
	
	CString Buff;
	Buff.Empty();						m_comboSentence.AddString(Buff);
	Buff.LoadString(IDS_Pass);			m_comboSentence.AddString(Buff);
	Buff.LoadString(IDS_Fail);			m_comboSentence.AddString(Buff);
	Buff.LoadString(IDS_Concession);	m_comboSentence.AddString(Buff);
	m_comboSentence.SetCurSel(MinMax(&m_pData->m_nSentence,0,3));

	if((theApp.m_LastSettings.nScanMask & _LOCK_FILE_ON_SENTENCE) && (m_pData->m_nSentence!=0)) {
		m_comboSentence.EnableWindow(false);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSentenceDlg::OnOK() 
{
	m_pData->m_nSentence = m_comboSentence.GetCurSel();
	m_pData->m_bModified = true;
	
	CDialog::OnOK();
}
