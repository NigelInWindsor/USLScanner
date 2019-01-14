// Pr20Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "MainFrm.h"
#include "Pr20Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPr20Dlg dialog



CPr20Dlg::CPr20Dlg(UINT nCap,CWnd* pParent,CWnd** DlgPtr,int nPulserType)
	: CPropertyPage(CPr20Dlg::IDD, nCap)
{
	//{{AFX_DATA_INIT(CPr20Dlg)
	//}}AFX_DATA_INIT

	switch(nPulserType) {
	case 20:	m_pPr = &theApp.m_UtUser.m_TS[0].Pr20;
		break;
	case 50:	m_pPr = &theApp.m_UtUser.m_TS[0].Pr50;
		break;
	}
	m_pGlobal = &theApp.m_UtUser.m_Global;
	m_pCards = &theApp.m_UtUser;
	m_bComboListSet=FALSE;
	m_DlgPtr=DlgPtr;
	m_nPulserType = nPulserType;

//	m_pCards->MscDefaults();
//	m_pCards->Pr2050Default(m_pPr,USL_PR20,0);

//	m_pGlobal->nTriggerSource = USL_PR20;
//	m_pGlobal->nTimeSlot = 0;

	m_pCards->MscInitialize();
	m_pCards->Pr2050Initialize(m_pPr);
}

CPr20Dlg::~CPr20Dlg()
{
	*m_DlgPtr=NULL;
}
	
void CPr20Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPr20Dlg)
	DDX_Control(pDX, IDC_RECTIFIER_COMBO, m_comboRectifier);
	DDX_Control(pDX, IDC_PULSE_VOLTAGE_SPIN, m_PulseVoltageSpin);
	DDX_Control(pDX, IDC_PULSE_VOLTAGE_EDIT, m_PulseVoltageEdit);
	DDX_Control(pDX, IDC_PRF_SPIN, m_PrfSpin);
	DDX_Control(pDX, IDC_PRF_EDIT, m_PrfEdit);
	DDX_Control(pDX, IDC_ATTENUATION, m_AttenuationEdit);
	DDX_Control(pDX, IDC_FILTERS_COMBO, m_FilterCombo);
	DDX_Control(pDX, IDC_ATTENUATION_SLIDER, m_AttenuationSlider);
	DDX_Control(pDX, IDC_SPIN_ATTENUATION, m_SpinAttenuation);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CPr20Dlg, CDialog)
	//{{AFX_MSG_MAP(CPr20Dlg)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_FILTERS_COMBO, OnSelchangeFiltersCombo)
	ON_EN_CHANGE(IDC_ATTENUATION, OnChangeAttenuation)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_PRF_EDIT, OnChangePrfEdit)
	ON_EN_CHANGE(IDC_PULSE_VOLTAGE_EDIT, OnChangePulseVoltageEdit)
	ON_CBN_SELCHANGE(IDC_RECTIFIER_COMBO, OnSelchangeRectifierCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPr20Dlg message handlers





BOOL CPr20Dlg::OnInitDialog() 
{
	WCHAR Str[40];
	UDACCEL	Accel;

	CDialog::OnInitDialog();

	if(m_bComboListSet==FALSE) {
		m_bComboListSet=TRUE;
	}
	swprintf_s(Str,_T("%ddB"),m_pPr->nAttenuation);
	m_AttenuationEdit.SetWindowText(Str);

	m_AttenuationSlider.SetRange(0,63);
	m_AttenuationSlider.SetPos(m_pPr->nAttenuation);
	m_AttenuationSlider.SetTicFreq(5);

	m_SpinAttenuation.SetRange(0,63);
	m_SpinAttenuation.SetPos(m_pPr->nAttenuation);

	m_PulseVoltageSpin.SetRange(0,600);
	m_PulseVoltageSpin.SetPos(m_pPr->nPulseVoltage);
	swprintf_s(Str,_T("%dV"),m_pPr->nPulseVoltage);
	m_PulseVoltageEdit.SetWindowText(Str);

	m_FilterCombo.InsertString(0,_T("0"));
	m_FilterCombo.InsertString(1,_T("5"));
	m_FilterCombo.InsertString(2,_T("10"));
	m_FilterCombo.InsertString(3,_T("15"));
	m_FilterCombo.InsertString(4,_T("18"));
	m_FilterCombo.InsertString(5,_T("23"));
	m_FilterCombo.InsertString(6,_T("28"));
	m_FilterCombo.InsertString(7,_T("33"));
	m_FilterCombo.SetCurSel(m_pPr->nFilter);


	m_comboRectifier.InsertString(0,_T("RF"));
	m_comboRectifier.InsertString(1,_T("Rectified"));
	m_comboRectifier.SetCurSel(m_pPr->nRectification);

	switch(m_comboRectifier.GetCurSel()){
	case 0:
		theApp.m_UtUser.m_TS[m_pPr->nRfChannel].Adc.nRfType=RFTYPELINEAR;
		break;
	case 1:
		theApp.m_UtUser.m_TS[m_pPr->nRfChannel].Adc.nRfType=RFTYPERECTIFIED;
		break;
	}

	Accel.nInc=10;
	Accel.nSec=1;
	swprintf_s(Str,_T("%dHz"),m_pGlobal->nPrf);
	m_PrfEdit.SetWindowText(Str);
	m_PrfSpin.SetRange(100,10000);
	m_PrfSpin.SetAccel(1,&Accel);
	m_PrfSpin.SetPos(m_pGlobal->nPrf);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPr20Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPr20Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	WCHAR	Str[20];
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;


	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_ATTENUATION:
		m_pPr->nAttenuation=(pSpin->GetPos()&0xffff);
		m_pCards->Pr2050Attenuation(m_pPr);
		break;
	case IDC_ATTENUATION_SLIDER:
		m_pPr->nAttenuation=(pSlider->GetPos()&0xffff);
		m_pCards->Pr2050Attenuation(m_pPr);
		break;
	case IDC_PRF_SPIN:
		m_pGlobal->nPrf=pSpin->GetPos()&0xffff;
		m_pCards->SetPrf();
		break;
	case IDC_PULSE_VOLTAGE_SPIN:
		m_pPr->nPulseVoltage=pSpin->GetPos()&0xffff;
		m_pCards->Pr2050PulseVoltage(m_pPr);
		m_pCards->MailboxHt(TRUE,m_pPr->nPulseVoltage);
		break;
	};
	swprintf_s(Str,_T("%ddB"),m_pPr->nAttenuation);
	m_AttenuationEdit.SetWindowText(Str);
	m_AttenuationSlider.SetPos(m_pPr->nAttenuation);
	m_SpinAttenuation.SetPos(m_pPr->nAttenuation);

	swprintf_s(Str,_T("%dHz"),m_pGlobal->nPrf);
	m_PrfEdit.SetWindowText(Str);

	swprintf_s(Str,_T("%dV"),m_pPr->nPulseVoltage);
	m_PulseVoltageEdit.SetWindowText(Str);


	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPr20Dlg::OnSelchangeFiltersCombo() 
{

	m_pPr->nFilter=m_FilterCombo.GetCurSel();
	m_pCards->Pr2050FiltersRectifier(m_pPr);
}



void CPr20Dlg::OnChangeAttenuation() 
{
	WCHAR Str[40];

	m_AttenuationEdit.GetWindowText(Str,40);
	swscanf_s(Str,_T("%d"),&m_pPr->nAttenuation);
	m_AttenuationSlider.SetPos(m_pPr->nAttenuation);
	m_SpinAttenuation.SetPos(m_pPr->nAttenuation);
	m_pCards->Pr2050Attenuation(m_pPr);

}




void CPr20Dlg::OnClose() 
{

	CDialog::OnClose();
	delete *m_DlgPtr;
}


void CPr20Dlg::OnChangePrfEdit() 
{
	WCHAR Str[40];

	m_PrfEdit.GetWindowText(Str,40);
	swscanf_s(Str,_T("%d"),&m_pGlobal->nPrf);
	m_PrfSpin.SetPos(m_pGlobal->nPrf);
	m_pCards->SetPrf();
	
}

void CPr20Dlg::OnChangePulseVoltageEdit() 
{
	WCHAR Str[40];

	m_PulseVoltageEdit.GetWindowText(Str,40);
	swscanf_s(Str,_T("%d"),&m_pPr->nPulseVoltage);
	m_PulseVoltageSpin.SetPos(m_pPr->nPulseVoltage);
	m_pCards->Pr2050PulseVoltage(m_pPr);
	m_pCards->MailboxHt(TRUE,m_pPr->nPulseVoltage);

	
}

void CPr20Dlg::OnSelchangeRectifierCombo() 
{
	m_pPr->nRectification=m_comboRectifier.GetCurSel();
	m_pCards->Pr2050FiltersRectifier(m_pPr);
	m_pCards->Pr2050Attenuation(m_pPr);
	
	switch(m_comboRectifier.GetCurSel()){
	case 0:
		theApp.m_UtUser.m_TS[m_pPr->nRfChannel].Adc.nRfType=RFTYPELINEAR;
		break;
	case 1:
		theApp.m_UtUser.m_TS[m_pPr->nRfChannel].Adc.nRfType=RFTYPERECTIFIED;
		break;
	}

}
BOOL CPr20Dlg::OnSetActive() 
{
	CMainFrame*  pFrame =  (CMainFrame*)theApp.m_pMainWnd;
	
	switch(m_nPulserType) {
	case 20:	theApp.m_UtUser.RfAndClockSource(&theApp.m_UtUser.m_Global.Msc,0,0);
		break;
	case 50:	theApp.m_UtUser.RfAndClockSource(&theApp.m_UtUser.m_Global.Msc,1,1);
		break;
	}

	m_pCards->Pr2050PulseVoltage(m_pPr);
	m_pCards->MailboxHt(TRUE,m_pPr->nPulseVoltage);

	return CPropertyPage::OnSetActive();
}
