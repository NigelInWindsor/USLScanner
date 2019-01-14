// CopyCoordsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "CopyCoordsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopyCoordsDlg dialog


CCopyCoordsDlg::CCopyCoordsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCopyCoordsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCopyCoordsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCopyCoordsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCopyCoordsDlg)
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	DDX_Control(pDX, IDC_EDIT_NUMBER_PTS, m_editNumberPts);
	DDX_Control(pDX, IDC_COMBO_PERCENTAGE_LINES, m_comboPercentageLines);
	DDX_Control(pDX, IDC_COMBO_DESTINATION_LINES, m_comboDestinationLines);
	DDX_Control(pDX, IDC_COMBO_SOURCE_LINES, m_comboSourceLines);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCopyCoordsDlg, CDialog)
	//{{AFX_MSG_MAP(CCopyCoordsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyCoordsDlg message handlers

BOOL CCopyCoordsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;
	
	Buff.LoadString(IDS_Move_and_Delete);	m_comboMode.AddString(Buff);
	Buff.LoadString(IDS_Copy);				m_comboMode.AddString(Buff);

	m_comboSourceLines.AddString(_T("Taught"));
	m_comboSourceLines.AddString(_T("Scan"));
	m_comboSourceLines.AddString(_T("Brain"));
	m_comboSourceLines.AddString(_T("Surface"));

	m_comboDestinationLines.AddString(_T("Taught"));
	m_comboDestinationLines.AddString(_T("Scan"));
	m_comboDestinationLines.AddString(_T("Brain"));
	m_comboDestinationLines.AddString(_T("Surface"));

	m_comboPercentageLines.AddString(_T("100%"));
	m_comboPercentageLines.AddString(_T("50%"));
	m_comboPercentageLines.AddString(_T("20%"));
	m_comboPercentageLines.AddString(_T("10%"));
	m_comboPercentageLines.AddString(_T("5%"));
	m_comboPercentageLines.AddString(_T("2%"));
	m_comboPercentageLines.AddString(_T("1%"));

	Buff = "10";
	m_editNumberPts.SetWindowText(Buff);

	UpdateAllControls();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCopyCoordsDlg::OnOK() 
{
	CString Buff;

	bool bMode = m_comboMode.GetCurSel() & 0x01;
	m_comboPercentageLines.GetWindowText(Buff);
	int nPercentage = _ttoi(Buff);
	int nSrc = m_comboSourceLines.GetCurSel();
	int nDest = m_comboDestinationLines.GetCurSel();
	m_editNumberPts.GetWindowText(Buff);
	int	nMaxPts = _ttoi(Buff);

	if(nSrc == 3) nSrc = SURFACELINES;
	if(nDest == 3) nDest = SURFACELINES;

	PROFILE->CopyAllLines(nDest,nSrc,bMode);

	CDialog::OnOK();
}

void CCopyCoordsDlg::UpdateAllControls()
{

	m_comboMode.SetCurSel(0);
	m_comboSourceLines.SetCurSel(0);
	m_comboDestinationLines.SetCurSel(0);
	m_comboPercentageLines.SetCurSel(0);

}
