// ViewValuePage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ViewValuePage.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewValuePage property page

IMPLEMENT_DYNCREATE(CViewValuePage, CPropertyPage)

CViewValuePage::CViewValuePage(CWnd* pViewSheet) : CPropertyPage(CViewValuePage::IDD)
{
	//{{AFX_DATA_INIT(CViewValuePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pViewSheet = pViewSheet;
}

CViewValuePage::~CViewValuePage()
{
}

void CViewValuePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewValuePage)
	DDX_Control(pDX, IDC_SPIN_LOG_RANGE, m_spinLogRange);
	DDX_Control(pDX, IDC_EDIT_LOG_RANGE, m_editLogRange);
	DDX_Control(pDX, IDC_CHECK_POSITION_CROSS, m_checkPositionCross);
	DDX_Control(pDX, IDC_CHECK_TS_NAME, m_checkTSName);
	DDX_Control(pDX, IDC_CHECK_GATE_NAME, m_checkGateName);
	DDX_Control(pDX, IDC_COMBO_VALUE_DATA, m_comboDataType);
	DDX_Control(pDX, IDC_SPIN_SIZE, m_spinSize);
	DDX_Control(pDX, IDC_EDIT_SIZE, m_editSize);
	DDX_Control(pDX, IDC_COMBO_ALGORITHM, m_comboAlgorithm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewValuePage, CPropertyPage)
	//{{AFX_MSG_MAP(CViewValuePage)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_SIZE, OnChangeEditSize)
	ON_CBN_SELCHANGE(IDC_COMBO_ALGORITHM, OnSelchangeComboAlgorithm)
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_VALUE_DATA, OnSelchangeComboValueData)
	ON_BN_CLICKED(IDC_CHECK_TS_NAME, OnCheckTsName)
	ON_BN_CLICKED(IDC_CHECK_GATE_NAME, OnCheckGateName)
	ON_BN_CLICKED(IDC_CHECK_POSITION_CROSS, OnCheckPositionCross)
	ON_EN_CHANGE(IDC_EDIT_LOG_RANGE, OnChangeEditLogRange)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LOG_RANGE, OnDeltaposSpinLogRange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewValuePage message handlers

BOOL CViewValuePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

	m_spinSize.SetRange(1,200);

	m_comboAlgorithm.AddString(_T("Minimum"));
	m_comboAlgorithm.AddString(_T("Maximum"));
	m_comboAlgorithm.AddString(_T("Average"));
	m_comboDataType.AddString(_T("Sample / Line"));
	m_comboDataType.AddString(_T("Coordinate"));
	m_comboDataType.AddString(_T("Value"));

	
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CViewValuePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CViewValuePage::UpdateAllControls()
{
	
	CString Buff;
	CViewSheet* m_pSheet = (CViewSheet*)m_pViewSheet;

	if (GetSafeHwnd() == NULL) return;

	Buff.Format(_T("%.1fmm"),theApp.m_LastSettings.fValueCursorSize);
	m_editSize.SetWindowText(Buff);
	m_spinSize.SetPos((int)(theApp.m_LastSettings.fValueCursorSize * 10.0f));
	m_comboAlgorithm.SetCurSel(theApp.m_LastSettings.nValueAlgorithm);
	m_comboDataType.SetCurSel(theApp.m_LastSettings.nValueDataType);
	m_checkTSName.SetCheck(theApp.m_LastSettings.nViewTabTSName&1);
	m_checkGateName.SetCheck(theApp.m_LastSettings.nViewTabGateName&1);
	m_checkPositionCross.SetCheck(theApp.m_LastSettings.nDisplayPositionCross&1);

	Buff.Format(L"%ddB",m_pSheet->m_Data.m_nLogRange);
	m_editLogRange.SetWindowText(Buff);

}

void CViewValuePage::OnChangeEditSize() 
{
	
	CString Buff;
	float fTemp = theApp.m_LastSettings.fValueCursorSize;

	m_editSize.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	if(fTemp - theApp.m_LastSettings.fValueCursorSize) {
		theApp.m_LastSettings.fValueCursorSize = fTemp;
		m_spinSize.SetPos((int)(theApp.m_LastSettings.fValueCursorSize * 10.0f));
	}

}

void CViewValuePage::OnSelchangeComboAlgorithm() 
{
	
	theApp.m_LastSettings.nValueAlgorithm = m_comboAlgorithm.GetCurSel();
	
}

void CViewValuePage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
//	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	float fTemp;
	CViewSheet* m_pSheet = (CViewSheet*)m_pViewSheet;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_SIZE:
		fTemp = theApp.m_LastSettings.fValueCursorSize;
		theApp.m_LastSettings.fValueCursorSize = (float)(pSpin->GetPos()&0xffff) / 10.0f;
		if(fTemp - theApp.m_LastSettings.fValueCursorSize) {
			Buff.Format(_T("%.01f mm"),theApp.m_LastSettings.fValueCursorSize);
			m_editSize.SetWindowText(Buff);
		}
		break;
	}	
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CViewValuePage::OnSelchangeComboValueData() 
{
	
	theApp.m_LastSettings.nValueDataType = m_comboDataType.GetCurSel();
	
}

void CViewValuePage::OnCheckTsName() 
{
		
	theApp.m_LastSettings.nViewTabTSName = m_checkTSName.GetCheck()&1;
}

void CViewValuePage::OnCheckGateName() 
{
		
	theApp.m_LastSettings.nViewTabGateName = m_checkGateName.GetCheck()&1;
}

void CViewValuePage::OnCheckPositionCross() 
{
	
	theApp.m_LastSettings.nDisplayPositionCross = m_checkPositionCross.GetCheck()&1;
}

void CViewValuePage::OnChangeEditLogRange() 
{
	CString Buff;
	CViewSheet* m_pSheet = (CViewSheet*)m_pViewSheet;

	m_editLogRange.GetWindowText(Buff);

	m_pSheet->m_Data.m_nLogRange = _ttoi(Buff);
}

void CViewValuePage::OnDeltaposSpinLogRange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CViewSheet* m_pSheet = (CViewSheet*)m_pViewSheet;
	CString Buff;

	m_pSheet->m_Data.m_nLogRange -= pNMUpDown->iDelta;
	Buff.Format(L"%ddB",m_pSheet->m_Data.m_nLogRange);
	m_editLogRange.SetWindowText(Buff);
	
	*pResult = 0;
}
