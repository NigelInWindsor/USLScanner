// ArtificialSignalPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ArtificialSignalPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArtificialSignalPage property page

IMPLEMENT_DYNCREATE(CArtificialSignalPage, CPropertyPage)

CArtificialSignalPage::CArtificialSignalPage() : CPropertyPage(CArtificialSignalPage::IDD)
{
	//{{AFX_DATA_INIT(CArtificialSignalPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

CArtificialSignalPage::~CArtificialSignalPage()
{
}

void CArtificialSignalPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArtificialSignalPage)
	DDX_Control(pDX, IDC_COMBOWINDOW, m_comboWindow);
	DDX_Control(pDX, IDC_SPIN_AMPLITUDE, m_spinAmplitude);
	DDX_Control(pDX, IDC_EDIT_AMPLITUDE, m_editAmplitude);
	DDX_Control(pDX, IDC_SPIN_FREQUENCY, m_spinFrequency);
	DDX_Control(pDX, IDC_EDIT_FREQUENCY, m_editFrequency);
	DDX_Control(pDX, IDC_COMBO_SHAPE, m_comboShape);
	DDX_Control(pDX, IDC_COMBO_LENGTH, m_comboLength);
	DDX_Control(pDX, IDC_CHECK_USE_SIGNAL_GENERATOR, m_checkUse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArtificialSignalPage, CPropertyPage)
	//{{AFX_MSG_MAP(CArtificialSignalPage)
	ON_CBN_SELCHANGE(IDC_COMBO_LENGTH, OnSelchangeComboLength)
	ON_CBN_SELCHANGE(IDC_COMBO_SHAPE, OnSelchangeComboShape)
	ON_BN_CLICKED(IDC_CHECK_USE_SIGNAL_GENERATOR, OnCheckUseSignalGenerator)
	ON_EN_CHANGE(IDC_EDIT_FREQUENCY, OnChangeEditFrequency)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_AMPLITUDE, OnChangeEditAmplitude)
	ON_CBN_SELCHANGE(IDC_COMBOWINDOW, OnSelchangeCombowindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArtificialSignalPage message handlers

BOOL CArtificialSignalPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	Buff.LoadString(IDS_Sine_Wave);			m_comboShape.AddString(Buff);
	Buff.LoadString(IDS_Square_Wave);		m_comboShape.AddString(Buff);
	Buff.LoadString(IDS_Triangle_Wave);		m_comboShape.AddString(Buff);
	Buff.LoadString(IDS_Saw_Tooth_Wave);	m_comboShape.AddString(Buff);

	Buff.LoadString(IDS_Continuous);	m_comboLength.AddString(Buff);
	Buff.LoadString(IDS_Half_Cycle);	m_comboLength.AddString(Buff);
	Buff.LoadString(IDS_Full_Cycle);	m_comboLength.AddString(Buff);
	Buff.LoadString(IDS_2_Cycles);		m_comboLength.AddString(Buff);
	Buff.LoadString(IDS_5_Cycles);		m_comboLength.AddString(Buff);
	Buff.LoadString(IDS_10_Cycles);		m_comboLength.AddString(Buff);

	Buff.LoadString(IDS_Square);	m_comboWindow.AddString(Buff);
	Buff.LoadString(IDS_Parzen);	m_comboWindow.AddString(Buff);
	Buff.LoadString(IDS_Welch);		m_comboWindow.AddString(Buff);
	Buff.LoadString(IDS_Hanning);	m_comboWindow.AddString(Buff);

	m_spinFrequency.SetRange(0,1000);
	m_spinAmplitude.SetRange(0,100);

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CArtificialSignalPage::UpdateAllControls()
{
	CString Buff;

	Buff.Format(_T("%.01f MHz"),theApp.m_Scope.m_fSGFrequency[0] / 1e6);
	m_editFrequency.SetWindowText(Buff);
	m_spinFrequency.SetPos((int)(theApp.m_Scope.m_fSGFrequency[0] / 1e5));

	Buff.Format(_T("%d %%FSH"),theApp.m_Scope.m_nSGAmplitude[0]);
	m_editAmplitude.SetWindowText(Buff);
	m_spinAmplitude.SetPos(theApp.m_Scope.m_nSGAmplitude[0]);

	m_comboShape.SetCurSel(theApp.m_Scope.m_nSGType[0]);
	m_comboLength.SetCurSel(theApp.m_Scope.m_nSGLengthMode[0]);
	m_comboWindow.SetCurSel(theApp.m_Scope.m_nSGWindow[0]);

}

void CArtificialSignalPage::OnSelchangeComboLength() 
{
	theApp.m_Scope.m_nSGLengthMode[0] = m_comboLength.GetCurSel();
}

void CArtificialSignalPage::OnSelchangeComboShape() 
{
	theApp.m_Scope.m_nSGType[0] = m_comboShape.GetCurSel();
}

void CArtificialSignalPage::OnCheckUseSignalGenerator() 
{
	theApp.m_Scope.m_nPlayRecording[0] == 0 ? theApp.m_Scope.m_nPlayRecording[0] = 3 : theApp.m_Scope.m_nPlayRecording[0] = 0;
}

void CArtificialSignalPage::OnChangeEditFrequency() 
{
	CString Buff;
	float fTemp = theApp.m_Scope.m_fSGFrequency[0];

	m_editFrequency.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Scope.m_fSGFrequency[0]);
	theApp.m_Scope.m_fSGFrequency[0] *= 1e6f;
	if(fTemp - theApp.m_Scope.m_fSGFrequency[0]) {
		m_spinFrequency.SetPos((int)(theApp.m_Scope.m_fSGFrequency[0] / 1e5));
	}
}

void CArtificialSignalPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
//	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int	nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_FREQUENCY:
		nTemp=(pSpin->GetPos()&0xffff);
		if(nTemp - (int)(theApp.m_Scope.m_fSGFrequency[0] / 1e5f)) {
			theApp.m_Scope.m_fSGFrequency[0] = (float)nTemp * 1e5f;
			Buff.Format(_T("%.01f MHz"),theApp.m_Scope.m_fSGFrequency[0] / 1e6);
			m_editFrequency.SetWindowText(Buff);
		}
		break;
	case IDC_SPIN_AMPLITUDE:
		nTemp = theApp.m_Scope.m_nSGAmplitude[0];
		theApp.m_Scope.m_nSGAmplitude[0]=(pSpin->GetPos()&0xffff);
		if(nTemp - theApp.m_Scope.m_nSGAmplitude[0]) {
			Buff.Format(_T("%d %%FSH"),theApp.m_Scope.m_nSGAmplitude[0]);
			m_editAmplitude.SetWindowText(Buff);
		}
		break;
	}	
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CArtificialSignalPage::OnChangeEditAmplitude() 
{
	CString Buff;
	int nTemp = theApp.m_Scope.m_nSGAmplitude[0];

	m_editAmplitude.GetWindowText(Buff);
	theApp.m_Scope.m_nSGAmplitude[0] = _ttoi(Buff);
	if(nTemp - theApp.m_Scope.m_nSGAmplitude[0]) {
		m_spinAmplitude.SetPos(theApp.m_Scope.m_nSGAmplitude[0]);
	}
}

void CArtificialSignalPage::OnSelchangeCombowindow() 
{
	theApp.m_Scope.m_nSGWindow[0] = m_comboWindow.GetCurSel();
}
