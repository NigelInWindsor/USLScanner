// PhasePage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PhasePage.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhasePage property page

IMPLEMENT_DYNCREATE(CPhasePage, CPropertyPage)

CPhasePage::CPhasePage(CWnd* pViewSheet, CData* pData) : CPropertyPage(CPhasePage::IDD)
{
	//{{AFX_DATA_INIT(CPhasePage)
	//}}AFX_DATA_INIT
	m_pViewSheet = pViewSheet;
	m_pData = pData;
	
}

CPhasePage::~CPhasePage()
{
}

void CPhasePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhasePage)
	DDX_Control(pDX, IDC_COMBO_NEGATIVE_SIGN, m_comboNegativeSign);
	DDX_Control(pDX, IDC_COMBO_POSITIVE_SIGN, m_comboPositiveSign);
	DDX_Control(pDX, IDC_COMBO_OVERLAY_WHICH, m_comboOverlayWhich);
	DDX_Control(pDX, IDC_CHECK_POSITIVE, m_checkPositive);
	DDX_Control(pDX, IDC_CHECK_NEGATIVE, m_checkNegative);
	DDX_Control(pDX, IDC_EDIT_NEGATIVE_AMPLITUDE, m_editNegativeAmp);
	DDX_Control(pDX, IDC_SPIN_POSITIVE_AMPLITUDE, m_spinPositiveAmp);
	DDX_Control(pDX, IDC_SPIN_NEGATIVE_AMPLITUDE, m_spinNegativeAmp);
	DDX_Control(pDX, IDC_EDIT_POSITIVE_AMPLITUDE, m_editPositiveAmp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhasePage, CPropertyPage)
	//{{AFX_MSG_MAP(CPhasePage)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_POSITIVE_AMPLITUDE, OnChangeEditPositiveAmplitude)
	ON_EN_CHANGE(IDC_EDIT_NEGATIVE_AMPLITUDE, OnChangeEditNegativeAmplitude)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NEGATIVE_AMPLITUDE, OnDeltaposSpinNegativeAmplitude)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_POSITIVE_AMPLITUDE, OnDeltaposSpinPositiveAmplitude)
	ON_BN_CLICKED(IDC_CHECK_POSITIVE, OnCheckPositive)
	ON_BN_CLICKED(IDC_CHECK_NEGATIVE, OnCheckNegative)
	ON_CBN_SELCHANGE(IDC_COMBO_OVERLAY_WHICH, OnSelchangeComboOverlayWhich)
	ON_CBN_SELCHANGE(IDC_COMBO_NEGATIVE_SIGN, OnSelchangeComboNegativeSign)
	ON_CBN_SELCHANGE(IDC_COMBO_POSITIVE_SIGN, OnSelchangeComboPositiveSign)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhasePage message handlers

BOOL CPhasePage::OnInitDialog() 
{
	CString Buff;
	CPropertyPage::OnInitDialog();
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;

	m_spinPositiveAmp.SetRange(0, 100);
	m_spinNegativeAmp.SetRange(0, 100);
	m_spinPositiveAmp.SetPos(50);
	m_spinNegativeAmp.SetPos(50);

	m_comboPositiveSign.AddString(L">=");
	m_comboPositiveSign.AddString(L"<=");
	m_comboNegativeSign.AddString(L">=");
	m_comboNegativeSign.AddString(L"<=");


	m_comboOverlayWhich.ResetContent();
	for(int nPage = 0;nPage<256;nPage++) {
		if(pSheet->m_pViewPage[nPage]) {
			if(pSheet->m_pViewPage[nPage]->m_nImageType == AMP_IMAGE) {
				m_comboOverlayWhich.AddString(pSheet->m_Name[nPage]);
			}
		}
	}

	m_comboOverlayWhich.SetCurSel(theApp.m_LastSettings.nPhaseOverlayImageNumber);
	
	UpdateAllControls();
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPhasePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CPhasePage::UpdateAllControls() 
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	theApp.m_LastSettings.nPhaseMask & DISPLAY_PHASE_POSITIVE ? m_checkPositive.SetCheck(true) : m_checkPositive.SetCheck(false);
	theApp.m_LastSettings.nPhaseMask & DISPLAY_PHASE_NEGATIVE ? m_checkNegative.SetCheck(true) : m_checkNegative.SetCheck(false);

	m_comboOverlayWhich.SetCurSel(theApp.m_LastSettings.nPhaseOverlayImageNumber );


	MinMax(&theApp.m_LastSettings.nPhasePositiveAmp, 0, 100);
	MinMax(&theApp.m_LastSettings.nPhaseNegativeAmp, 0, 100);

	Buff.Format(L"%d%%", theApp.m_LastSettings.nPhasePositiveAmp);
	m_editPositiveAmp.SetWindowText(Buff);
	Buff.Format(L"%d%%", theApp.m_LastSettings.nPhaseNegativeAmp);
	m_editNegativeAmp.SetWindowText(Buff);


	theApp.m_LastSettings.nPhaseMask & PHASE_POSITIVE_GREATER_LESS ?	m_comboPositiveSign.SetCurSel(1) : m_comboPositiveSign.SetCurSel(0);
	theApp.m_LastSettings.nPhaseMask & PHASE_NEGATIVE_GREATER_LESS ?	m_comboNegativeSign.SetCurSel(1) : m_comboNegativeSign.SetCurSel(0);

}

BOOL CPhasePage::OnSetActive() 
{
	theApp.m_LastSettings.nLastViewOptionsTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

void CPhasePage::OnChangeEditPositiveAmplitude() 
{
	CString Buff;

	m_editPositiveAmp.GetWindowText(Buff);
	theApp.m_LastSettings.nPhasePositiveAmp = _ttoi(Buff);
}

void CPhasePage::OnChangeEditNegativeAmplitude() 
{
	CString Buff;

	m_editNegativeAmp.GetWindowText(Buff);
	theApp.m_LastSettings.nPhaseNegativeAmp = _ttoi(Buff);
}

void CPhasePage::OnDeltaposSpinNegativeAmplitude(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString Buff;

	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	int nTemp = theApp.m_LastSettings.nPhaseNegativeAmp;

	theApp.m_LastSettings.nPhaseNegativeAmp += pNMUpDown->iDelta;
	MinMax(&theApp.m_LastSettings.nPhaseNegativeAmp, 0, 100);

	if(nTemp - theApp.m_LastSettings.nPhaseNegativeAmp) {
		Buff.Format(L"%d%%", theApp.m_LastSettings.nPhaseNegativeAmp);
		m_editNegativeAmp.SetWindowText(Buff);
	}

	*pResult = 0;
}

void CPhasePage::OnDeltaposSpinPositiveAmplitude(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString Buff;

	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	int nTemp = theApp.m_LastSettings.nPhasePositiveAmp;

	theApp.m_LastSettings.nPhasePositiveAmp += pNMUpDown->iDelta;
	MinMax(&theApp.m_LastSettings.nPhasePositiveAmp, 0, 100);

	if(nTemp - theApp.m_LastSettings.nPhasePositiveAmp) {
		Buff.Format(L"%d%%", theApp.m_LastSettings.nPhasePositiveAmp);
		m_editPositiveAmp.SetWindowText(Buff);
	}
	
	*pResult = 0;
}

void CPhasePage::OnCheckPositive() 
{
	m_checkPositive.GetCheck() == 1 ? theApp.m_LastSettings.nPhaseMask |= DISPLAY_PHASE_POSITIVE : theApp.m_LastSettings.nPhaseMask &= ~DISPLAY_PHASE_POSITIVE;
}

void CPhasePage::OnCheckNegative() 
{
	m_checkNegative.GetCheck() == 1 ? theApp.m_LastSettings.nPhaseMask |= DISPLAY_PHASE_NEGATIVE : theApp.m_LastSettings.nPhaseMask &= ~DISPLAY_PHASE_NEGATIVE;
}

void CPhasePage::OnSelchangeComboOverlayWhich() 
{
	theApp.m_LastSettings.nPhaseOverlayImageNumber = m_comboOverlayWhich.GetCurSel();
}

void CPhasePage::OnSelchangeComboNegativeSign() 
{
	m_comboNegativeSign.GetCurSel() == 0 ? theApp.m_LastSettings.nPhaseMask &= ~PHASE_NEGATIVE_GREATER_LESS : theApp.m_LastSettings.nPhaseMask |= PHASE_NEGATIVE_GREATER_LESS;
}

void CPhasePage::OnSelchangeComboPositiveSign() 
{
	m_comboPositiveSign.GetCurSel() == 0 ? theApp.m_LastSettings.nPhaseMask &= ~PHASE_POSITIVE_GREATER_LESS : theApp.m_LastSettings.nPhaseMask |= PHASE_POSITIVE_GREATER_LESS;
}

void CPhasePage::OnButtonDisplay() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;

	pSheet->CreatePhaseOverlayPage(0);
}


