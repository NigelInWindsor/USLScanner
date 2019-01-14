// ImageFilterPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ImageFilterPage.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageFilterPage property page

IMPLEMENT_DYNCREATE(CImageFilterPage, CPropertyPage)

CImageFilterPage::CImageFilterPage(CWnd* pViewSheet) : CPropertyPage(CImageFilterPage::IDD)
{
	//{{AFX_DATA_INIT(CImageFilterPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pViewSheet = pViewSheet;
	

}

CImageFilterPage::~CImageFilterPage()
{
}

void CImageFilterPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageFilterPage)
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_progressBar);
	DDX_Control(pDX, IDC_COMBO_TIME_PRIORRITY, m_comboTimePriority);
	DDX_Control(pDX, IDC_SPIN_OD, m_spinOD);
	DDX_Control(pDX, IDC_SPIN_ID, m_spinID);
	DDX_Control(pDX, IDC_EDIT_OD, m_editOD);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	DDX_Control(pDX, IDC_COMBO_ORIENTATION, m_comboOrientation);
	DDX_Control(pDX, IDC_COMBO_AMP_PRIORRITY, m_comboAmpPriority);
	DDX_Control(pDX, IDC_COMBO_WHICH_LINES, m_comboWhichLines);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RADIO_RECTILINEAR, m_radioRectilinear);
	DDX_Control(pDX, IDC_RADIO_POLAR, m_radioPolar);
	DDX_Control(pDX, IDC_COMBO_POLAR_ORIENTATION, m_comboPolarOrientation);
}


BEGIN_MESSAGE_MAP(CImageFilterPage, CPropertyPage)
	//{{AFX_MSG_MAP(CImageFilterPage)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_WHICH_LINES, OnSelchangeComboWhichLines)
	ON_CBN_SELCHANGE(IDC_COMBO_AMP_PRIORRITY, OnSelchangeComboAmpPriorrity)
	ON_CBN_SELCHANGE(IDC_COMBO_ORIENTATION, OnSelchangeComboOrientation)
	ON_BN_CLICKED(IDC_CHECK_POLAR, OnCheckPolar)
	ON_BN_CLICKED(IDC_CHECK_RECTILINEAR, OnCheckRectilinear)
	ON_EN_CHANGE(IDC_EDIT_ID, OnChangeEditId)
	ON_EN_CHANGE(IDC_EDIT_OD, OnChangeEditOd)
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_TIME_PRIORRITY, OnSelchangeComboTimePriorrity)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_90, OnButtonRotate90)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY_MODE, OnSelchangeComboDisplayMode)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_RECTILINEAR, &CImageFilterPage::OnBnClickedRadioRectilinear)
	ON_BN_CLICKED(IDC_RADIO_POLAR, &CImageFilterPage::OnBnClickedRadioPolar)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageFilterPage message handlers

BOOL CImageFilterPage::OnInitDialog() 
{
	CString Buff;
	CPropertyPage::OnInitDialog();
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	
	m_comboWhichLines.AddString(_T("All"));
	m_comboWhichLines.AddString(_T("Odd"));
	m_comboWhichLines.AddString(_T("Even"));
	m_comboWhichLines.AddString(_T("Average"));

	m_comboAmpPriority.AddString(_T("Min"));
	m_comboAmpPriority.AddString(_T("Max"));

	m_comboTimePriority.AddString(_T("Min"));
	m_comboTimePriority.AddString(_T("Max"));

	m_comboOrientation.AddString(_T("Top Left"));
	m_comboOrientation.AddString(_T("Bottom Left"));
	m_comboOrientation.AddString(_T("Top Right"));
	m_comboOrientation.AddString(_T("Bottom Right"));

	//	Buff.LoadString(IDS_Rectilinear);	m_comboDisplayMode.AddString(Buff);
	//	Buff.LoadString(IDS_Polar);			m_comboDisplayMode.AddString(Buff);

	Buff.LoadString(IDS_Normal);	m_comboPolarOrientation.AddString(Buff);
	Buff.LoadString(IDS_Mirrored);	m_comboPolarOrientation.AddString(Buff);

	m_spinID.SetRange(0,32000);
	m_spinOD.SetRange(0,32000);

	
	UpdateAllControls();
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CImageFilterPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CImageFilterPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	CString Buff;

	m_comboWhichLines.SetCurSel(theApp.m_LastSettings.nDisplayWhichLines);
	m_comboAmpPriority.SetCurSel(theApp.m_LastSettings.nAmpPriority);
	m_comboTimePriority.SetCurSel(theApp.m_LastSettings.nTimePriority);
	m_comboOrientation.SetCurSel(theApp.m_LastSettings.nImageOrientation );

//	m_comboDisplayMode.SetCurSel(pSheet->m_nRectPolarDisplayMode);

	Buff.Format(_T("%.01f mm"),pSheet->m_Data.m_fComponentDiameter[0]);
	m_editID.SetWindowText(Buff);
	Buff.Format(_T("%.01f mm"),pSheet->m_Data.m_fComponentDiameter[1]);
	m_editOD.SetWindowText(Buff);
	m_spinID.SetPos((int)(pSheet->m_Data.m_fComponentDiameter[0] * 10.0));
	m_spinOD.SetPos((int)(pSheet->m_Data.m_fComponentDiameter[1] * 10.0));

	pSheet->m_nRectPolarDisplayMode == 0 ? m_radioRectilinear.SetCheck(1) : m_radioPolar.SetCheck(1);

}

void CImageFilterPage::OnSelchangeComboWhichLines() 
{
	theApp.m_LastSettings.nDisplayWhichLines=m_comboWhichLines.GetCurSel();

	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	pSheet->InvalidateChild();
}

void CImageFilterPage::OnSelchangeComboAmpPriorrity() 
{
	theApp.m_LastSettings.nAmpPriority=m_comboAmpPriority.GetCurSel();

	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	pSheet->InvalidateChild();
}

void CImageFilterPage::OnSelchangeComboOrientation() 
{
	theApp.m_LastSettings.nImageOrientation = m_comboOrientation.GetCurSel();
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	pSheet->InvalidateChild();
}


BOOL CImageFilterPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastViewOptionsTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	return CPropertyPage::OnSetActive();
}

void CImageFilterPage::OnCheckPolar() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;

	pSheet->m_nRectPolarDisplayMode = 1;
	pSheet->InvalidateChild();
	UpdateAllControls();
}

void CImageFilterPage::OnCheckRectilinear() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;

	pSheet->m_nRectPolarDisplayMode = 0;
	pSheet->InvalidateChild();
	UpdateAllControls();
}

void CImageFilterPage::OnChangeEditId() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	CString Buff;
	float fTemp = pSheet->m_Data.m_fComponentDiameter[0];

	m_editID.GetWindowText(Buff);
	_WTOF(Buff,pSheet->m_Data.m_fComponentDiameter[0]);
	if(fTemp - pSheet->m_Data.m_fComponentDiameter[0]) {
		m_spinID.SetPos((int)(pSheet->m_Data.m_fComponentDiameter[0] * 10.0));
		pSheet->InvalidateChild();
	}
}

void CImageFilterPage::OnChangeEditOd() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	CString Buff;
	float fTemp = pSheet->m_Data.m_fComponentDiameter[1];

	m_editOD.GetWindowText(Buff);
	_WTOF(Buff,pSheet->m_Data.m_fComponentDiameter[1]);
	if(fTemp - pSheet->m_Data.m_fComponentDiameter[1]) {
		m_spinOD.SetPos((int)(pSheet->m_Data.m_fComponentDiameter[1] * 10.0));
		pSheet->InvalidateChild();
	}
}

void CImageFilterPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
//	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	float fTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_ID:
		fTemp = pSheet->m_Data.m_fComponentDiameter[0];
		pSheet->m_Data.m_fComponentDiameter[0] = (float)(pSpin->GetPos()&0xffff) / 10.0f;
		Buff.Format(_T("%.01f mm"),pSheet->m_Data.m_fComponentDiameter[0]);
		m_editID.SetWindowText(Buff);
		pSheet->InvalidateChild();
		break;
	case IDC_SPIN_OD:
		fTemp = pSheet->m_Data.m_fComponentDiameter[1];
		pSheet->m_Data.m_fComponentDiameter[1] = (float)(pSpin->GetPos()&0xffff) / 10.0f;
		Buff.Format(_T("%.01f mm"),pSheet->m_Data.m_fComponentDiameter[1]);
		m_editOD.SetWindowText(Buff);
		pSheet->InvalidateChild();
		break;
	}	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CImageFilterPage::OnSelchangeComboTimePriorrity() 
{
	theApp.m_LastSettings.nTimePriority=m_comboTimePriority.GetCurSel();

	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	pSheet->InvalidateChild();
}

void CImageFilterPage::OnButtonRotate90() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;

	pSheet->m_Data.Rotate90Degrees(&m_progressBar);

	pSheet->m_rrSamples.SetRect(0,0,pSheet->m_Data.m_nSamplesLine - 1,pSheet->m_Data.m_nNumberLines-1);
	pSheet->InvalidateChild();
}

void CImageFilterPage::OnSelchangeComboDisplayMode() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;

	pSheet->m_nRectPolarDisplayMode = m_comboDisplayMode.GetCurSel();
	pSheet->InvalidateChild();
	pSheet->One_to_One();
	UpdateAllControls();
}


void CImageFilterPage::OnBnClickedRadioRectilinear()
{
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;

	pSheet->m_nRectPolarDisplayMode = 0;
	pSheet->InvalidateChild();
	pSheet->One_to_One();
}


void CImageFilterPage::OnBnClickedRadioPolar()
{
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;

	pSheet->m_nRectPolarDisplayMode = 1;
	pSheet->InvalidateChild();
	pSheet->One_to_One();
}
