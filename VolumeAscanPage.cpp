// VolumeAscanPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "VolumeAscanPage.h"
#include "VolumeViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolumeAscanPage property page

IMPLEMENT_DYNCREATE(CVolumeAscanPage, CPropertyPage)

CVolumeAscanPage::CVolumeAscanPage(CWnd* pParent,GatesData* pGate,CData* pData) : CPropertyPage(CVolumeAscanPage::IDD)
{
	//{{AFX_DATA_INIT(CVolumeAscanPage)
	//}}AFX_DATA_INIT
	m_pParent= pParent;
	m_pGate = pGate;
	m_nGate = 0;
	m_pData = pData;

	m_Units[0] = "ns";
	m_Units[1] = "\x0b5s";
	m_Units[2] = "mm";
	m_Units[3] = "mm";
	m_Units[4] = "\x022";
	m_Units[5] = "\x022";

}

CVolumeAscanPage::~CVolumeAscanPage()
{
}

void CVolumeAscanPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolumeAscanPage)
	DDX_Control(pDX, IDC_COMBO_CROSSHAIRVALUE, m_comboCrossHairValue);
	DDX_Control(pDX, IDC_COMBO_GATE_CSCAN_GATE, m_comboCScanGate);
	DDX_Control(pDX, IDC_SPIN_FAR_DEADZONE, m_spinFarDeadZone);
	DDX_Control(pDX, IDC_EDIT_FAR_DELAY, m_editFarDeadZone);
	DDX_Control(pDX, IDC_COMBO_LOCK_SURFACE, m_comboLockSurface);
	DDX_Control(pDX, IDC_SPIN_WIDTH, m_spinGateWidth);
	DDX_Control(pDX, IDC_SPIN_GATE_THRESHOLD, m_spinGateThreshold);
	DDX_Control(pDX, IDC_SPIN_DELAY, m_spinGateDelay);
	DDX_Control(pDX, IDC_EDIT_GATE_WIDTH, m_editGateWidth);
	DDX_Control(pDX, IDC_EDIT_GATE_THRESHOLD, m_editGateThreshold);
	DDX_Control(pDX, IDC_EDIT_GATE_DELAY, m_editGateDelay);
	DDX_Control(pDX, IDC_COMBO_TOF_MODE, m_comboTOFMode);
	DDX_Control(pDX, IDC_COMBO_GATE_NUMBER, m_comboGateNumber);
	DDX_Control(pDX, IDC_COMBO_RFTYPE, m_comboRfType);
	DDX_Control(pDX, IDC_COMBO_GATE_MODE, m_comboGateMode);
	DDX_Control(pDX, IDC_COMBO_GRATICULE, m_comboGraticule);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolumeAscanPage, CPropertyPage)
	//{{AFX_MSG_MAP(CVolumeAscanPage)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_GATE_MODE, OnSelchangeComboGateMode)
	ON_CBN_SELCHANGE(IDC_COMBO_GRATICULE, OnSelchangeComboGraticule)
	ON_CBN_SELCHANGE(IDC_COMBO_RFTYPE, OnSelchangeComboRftype)
	ON_EN_CHANGE(IDC_EDIT_GATE_DELAY, OnChangeEditGateDelay)
	ON_EN_CHANGE(IDC_EDIT_GATE_THRESHOLD, OnChangeEditGateThreshold)
	ON_EN_CHANGE(IDC_EDIT_GATE_WIDTH, OnChangeEditGateWidth)
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_GATE_NUMBER, OnSelchangeComboGateNumber)
	ON_CBN_SELCHANGE(IDC_COMBO_TOF_MODE, OnSelchangeComboTofMode)
	ON_CBN_SELCHANGE(IDC_COMBO_LOCK_SURFACE, OnSelchangeComboLockSurface)
	ON_EN_CHANGE(IDC_EDIT_FAR_DELAY, OnChangeEditFarDelay)
	ON_CBN_SELCHANGE(IDC_COMBO_GATE_CSCAN_GATE, OnSelchangeComboGateCscanGate)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DELAY, OnDeltaposSpinDelay)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WIDTH, OnDeltaposSpinWidth)
	ON_CBN_SELCHANGE(IDC_COMBO_CROSSHAIRVALUE, OnSelchangeComboCrosshairvalue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolumeAscanPage message handlers

BOOL CVolumeAscanPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	m_spinGateDelay.SetRange(0,20000);
	m_spinGateWidth.SetRange(0,20000);
	m_spinFarDeadZone.SetRange(0,20000);
//	m_spinThreshold.SetRange(0,200);
//	m_spinPeakWidth.SetRange(0,20000);

	Buff.LoadString(IDS_Off); m_comboGraticule.AddString(Buff);
	Buff.LoadString(IDS_On); m_comboGraticule.AddString(Buff);

	Buff.LoadString(IDS_Off); m_comboGateMode.AddString(Buff);
	Buff.LoadString(IDS_Main_Bang); m_comboGateMode.AddString(Buff);
	Buff.LoadString(IDS_Interface); m_comboGateMode.AddString(Buff);

	Buff.LoadString(IDS_RF); m_comboRfType.AddString(Buff);
	Buff.LoadString(IDS_Positive_Half_Wave); m_comboRfType.AddString(Buff);
	Buff.LoadString(IDS_Negative_Half_Wave); m_comboRfType.AddString(Buff);
	Buff.LoadString(IDS_Full_Wave); m_comboRfType.AddString(Buff);

	Buff.LoadString(IDS_Off); m_comboTOFMode.AddString(Buff);
	Buff.LoadString(IDS_Rising_Edge); m_comboTOFMode.AddString(Buff);
	Buff.LoadString(IDS_Pos_Peak); m_comboTOFMode.AddString(Buff);
	Buff.LoadString(IDS_Last_Edge); m_comboTOFMode.AddString(Buff);
	Buff.LoadString(IDS_Last_Peak); m_comboTOFMode.AddString(Buff);

	Buff.LoadString(IDS_No); m_comboLockSurface.AddString(Buff);
	Buff.LoadString(IDS_Yes); m_comboLockSurface.AddString(Buff);

	Buff.Empty();						m_comboCrossHairValue.AddString(Buff);
	Buff.LoadString(IDS_Sample_Line);	m_comboCrossHairValue.AddString(Buff);
	
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CVolumeAscanPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CVolumeAscanPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	int	gg;
	CString Buff,StopBefore;
	UDACCEL	Accel;
	Accel.nSec=1;
	static int nOldGate = -1;

	m_spinGateDelay.SetPos(m_pGate->nNsDelay[m_nGate]/10);
	m_spinGateWidth.SetPos(m_pGate->nNsWidth[m_nGate]/10);
	m_spinGateThreshold.SetPos(m_pGate->nThreshold[m_nGate]+100);
	m_spinFarDeadZone.SetPos(m_pGate->nNsFarDeadZone[m_nGate]/10);

	theApp.FormatTime(Buff, m_pGate->nNsDelay[m_nGate], theApp.m_Scope.m_nGateDelayUnits, m_Units[theApp.m_Scope.m_nGateDelayUnits]);
	m_editGateDelay.SetWindowText(Buff);
	
	theApp.FormatTime(Buff, m_pGate->nNsWidth[m_nGate], theApp.m_Scope.m_nGateWidthUnits, m_Units[theApp.m_Scope.m_nGateWidthUnits]);
	m_editGateWidth.SetWindowText(Buff);

	theApp.FormatTime(Buff, m_pGate->nNsFarDeadZone[m_nGate], theApp.m_Scope.m_nGateDelayUnits, m_Units[theApp.m_Scope.m_nGateDelayUnits]);
	m_editFarDeadZone.SetWindowText(Buff);

	switch(theApp.m_Scope.m_nGateDelayUnits) {
	case 0:
		Accel.nInc=1;
		m_spinGateDelay.SetAccel(1,&Accel);
		m_spinFarDeadZone.SetAccel(1, &Accel);
		break;
	default:
		Accel.nInc=3;
		m_spinGateDelay.SetAccel(1,&Accel);
		m_spinFarDeadZone.SetAccel(1, &Accel);
		break;
	}

	switch (theApp.m_Scope.m_nGateWidthUnits) {
	case 0:
		Accel.nInc = 1;
		m_spinGateWidth.SetAccel(1, &Accel);
		break;
	default:
		Accel.nInc = 3;
		m_spinGateWidth.SetAccel(1, &Accel);
		break;
	}

	Buff.Format(_T("%d"),m_pGate->nThreshold[m_nGate]);
	m_editGateThreshold.SetWindowText(Buff);

	if (nOldGate != m_nGate) {
		m_comboGateNumber.ResetContent();
		m_comboCScanGate.ResetContent();
		m_comboGateNumber.LimitText(sizeof m_pGate->cName[0] - 1);
		for (gg = 0; gg < 8; gg++) {
			m_comboGateNumber.AddString(m_pGate->cName[gg]);
			m_comboCScanGate.AddString(m_pGate->cName[gg]);
		}

		m_comboGateMode.ResetContent();
		Buff.LoadString(IDS_Off); m_comboGateMode.AddString(Buff);
		Buff.LoadString(IDS_Main_Bang); m_comboGateMode.AddString(Buff);
		Buff.LoadString(IDS_Interface); m_comboGateMode.AddString(Buff);
		StopBefore.LoadString(IDS_StopBefore);
		for (gg = 1; gg < m_nGate; gg++) {
			Buff.Format(_T("<%s"), m_pGate->cName[gg]);
			m_comboGateMode.AddString(Buff);
		}
	}
	m_comboGateMode.SetCurSel(m_pGate->nGateMode[m_nGate]);

	m_comboGateNumber.SetCurSel(m_nGate);
	m_comboCScanGate.SetCurSel(	theApp.m_LastSettings.nVolumeCScanGate);


	m_comboGraticule.SetCurSel(theApp.m_LastSettings.nVolumeAscanGraticule);
	m_comboRfType.SetCurSel(theApp.m_LastSettings.nVolumeRfType);
	m_comboTOFMode.SetCurSel(m_pGate->nTimsDetectionMode[m_nGate]);

	m_comboLockSurface.SetCurSel(theApp.m_LastSettings.nLockSurface);

	m_comboCrossHairValue.SetCurSel(theApp.m_LastSettings.nVolumeCrossHairValue &= 1);
	nOldGate = m_nGate;
}

void CVolumeAscanPage::OnSelchangeComboGateMode() 
{

	m_pGate->nGateMode[m_nGate]=m_comboGateMode.GetCurSel();
}

void CVolumeAscanPage::OnSelchangeComboGraticule() 
{
	

	theApp.m_LastSettings.nVolumeAscanGraticule=m_comboGraticule.GetCurSel();

}


void CVolumeAscanPage::OnSelchangeComboRftype() 
{
	

	theApp.m_LastSettings.nVolumeRfType=m_comboRfType.GetCurSel();
	InvalidateParent();


}

void CVolumeAscanPage::OnChangeEditGateDelay() 
{
	CString	Buff;
	int nValue=m_pGate->nNsDelay[m_nGate];

	m_editGateDelay.GetWindowText(Buff);
	m_pGate->nNsDelay[m_nGate] = theApp.ScanfDistanceTimens(Buff, theApp.m_Scope.m_nGateDelayUnits);
	if(nValue - m_pGate->nNsDelay[m_nGate]) {
		m_spinGateDelay.SetPos(m_pGate->nNsDelay[m_nGate]/10);
	}
}


void CVolumeAscanPage::OnChangeEditGateWidth() 
{
	CString	Buff;
	int nValue = m_pGate->nNsWidth[m_nGate];

	m_editGateWidth.GetWindowText(Buff);
	m_pGate->nNsWidth[m_nGate] = theApp.ScanfDistanceTimens(Buff, theApp.m_Scope.m_nGateWidthUnits);
	if (nValue - m_pGate->nNsWidth[m_nGate]) {
		m_spinGateWidth.SetPos(m_pGate->nNsWidth[m_nGate] / 10);
	}
}

void CVolumeAscanPage::OnChangeEditGateThreshold() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CVolumeAscanPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString	Buff;
	int	nTemp;


	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_FAR_DEADZONE:
		nTemp = m_pGate->nNsFarDeadZone[m_nGate];
		m_pGate->nNsFarDeadZone[m_nGate]=(pSpin->GetPos()&0xffff)*10;
		if(nTemp - m_pGate->nNsFarDeadZone[m_nGate]) {
			switch(theApp.m_Scope.m_nGateWidthUnits) {
			case 0: Buff.Format(_T("%.02f"),(float)m_pGate->nNsFarDeadZone[m_nGate]/1000);
				break;
			case 1: Buff.Format(_T("%.02f"),(float)(m_pGate->nNsFarDeadZone[m_nGate]*theApp.m_LastSettings.nMaterialVelocity)/2e6);
				break;
			case 2: Buff.Format(_T("%.04f"),(float)(m_pGate->nNsFarDeadZone[m_nGate]*theApp.m_LastSettings.nMaterialVelocity)/50.8e6);
				break;
			}
			Buff += m_Units[theApp.m_Scope.m_nGateWidthUnits];
			m_editFarDeadZone.SetWindowText(Buff);
			Dsp200CalculateHardware(theApp.m_UtUser.m_Global.nTimeSlot,m_nGate);
		}
		break;
	case IDC_SPIN_GATE_THRESHOLD:
		m_pGate->nThreshold[m_nGate]=(pSpin->GetPos()&0xffff)-100;
		Buff.Format(_T("%d"),m_pGate->nThreshold[m_nGate]);
		m_editGateThreshold.SetWindowText(Buff);
		break;
	}
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CVolumeAscanPage::OnSelchangeComboGateNumber() 
{
	m_nGate=m_comboGateNumber.GetCurSel();
	UpdateAllControls();
	
}

void CVolumeAscanPage::OnSelchangeComboTofMode() 
{
	m_pGate->nTimsDetectionMode[m_nGate] = m_comboTOFMode.GetCurSel();
	
}

void CVolumeAscanPage::OnSelchangeComboLockSurface() 
{
	

	theApp.m_LastSettings.nLockSurface = m_comboLockSurface.GetCurSel();
	
}

void CVolumeAscanPage::OnChangeEditFarDelay() 
{
	CString	Buff;
	int nValue = m_pGate->nNsFarDeadZone[m_nGate];

	m_editFarDeadZone.GetWindowText(Buff);
	m_pGate->nNsFarDeadZone[m_nGate] = theApp.ScanfDistanceTimens(Buff, theApp.m_Scope.m_nGateDelayUnits);
	if (nValue - m_pGate->nNsFarDeadZone[m_nGate]) {
		m_spinFarDeadZone.SetPos(m_pGate->nNsDelay[m_nGate] / 10);
	}
}

BOOL CVolumeAscanPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastVolumeToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}


BOOL CVolumeAscanPage::Dsp200CalculateHardware(int nTimeSlot,int nWhich)
{
	
	int	nDelay,nWidth;
	struct DSP200Data* pDsp=&m_pData->m_TS[nTimeSlot].Dsp;
	struct ADC200Data* pAdc=&m_pData->m_TS[nTimeSlot].Adc;
	struct GatesData* pGate=&m_pData->m_TS[nTimeSlot].Gate;

	if(pAdc->fSamplePeriod==0.0) return FALSE;
	pDsp->IntGate = 0;

	switch(pGate->nGateMode[nWhich]) {
	case 0:	//off;
		nDelay=0;
		nWidth=0;
		break;
	case 1:	//main bang
		pDsp->TriggerMode[nWhich]=1;
		nDelay=(int)((float)(pGate->nNsDelay[nWhich]-pAdc->nDelay)/pAdc->fSamplePeriod);
		nWidth=(int)((float)pGate->nNsWidth[nWhich]/pAdc->fSamplePeriod);
		break;
	case 2:	//interface
		pDsp->TriggerMode[nWhich]=2;
		nDelay=(int)((float)pGate->nNsDelay[nWhich]/pAdc->fSamplePeriod);
		nWidth=(int)((float)pGate->nNsWidth[nWhich]/pAdc->fSamplePeriod);
		break;
	default:
		pDsp->TriggerMode[nWhich]=3;
		nDelay=(int)((float)pGate->nNsDelay[nWhich]/pAdc->fSamplePeriod);
		nWidth=(int)((((float)(pGate->nTimeSample[1]-pGate->nTimeSample[0])*pAdc->fSamplePeriod) - (float)pGate->nNsFarDeadZone[nWhich])/pAdc->fSamplePeriod) - nDelay;
		break;
	};
	pGate->nSampleDelay[nWhich] = nDelay;
	pGate->nSampleWidth[nWhich] = nWidth;
	
	pDsp->Delay[nWhich]=nDelay;
	pDsp->Width[nWhich]=nWidth;
	pDsp->InterfaceThreshold[nWhich]=abs((pGate->nThreshold[nWhich]*127)/100);
	if(pGate->nThreshold[nWhich]>=0){
		pDsp->SignDetect[nWhich]=0;
		pDsp->InterfaceSlope[nWhich]=0;
	} else {
		pDsp->SignDetect[nWhich]=1;
		pDsp->InterfaceSlope[nWhich]=1;
	};



	return TRUE;
}

void CVolumeAscanPage::OnSelchangeComboGateCscanGate() 
{
	theApp.m_LastSettings.nVolumeCScanGate = m_comboCScanGate.GetCurSel();
}

void CVolumeAscanPage::InvalidateParent()
{
	CVolumeViewDlg* pVolumeView = (CVolumeViewDlg*)m_pParent;

	pVolumeView->SendMessage(WM_PAINT);
}

void CVolumeAscanPage::OnDeltaposSpinDelay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	int nValue = m_pGate->nNsDelay[m_nGate];

	m_pGate->nNsDelay[m_nGate]+=(pNMUpDown->iDelta*10);
	theApp.FormatTime(Buff, m_pGate->nNsDelay[m_nGate], theApp.m_Scope.m_nGateDelayUnits, m_Units[theApp.m_Scope.m_nGateDelayUnits]);

	if(nValue = m_pGate->nNsDelay[m_nGate]) {
		Buff += m_Units[theApp.m_Scope.m_nGateDelayUnits];
		m_editGateDelay.SetWindowText(Buff);
		InvalidateParent();
	}
	
	*pResult = 0;
}

void CVolumeAscanPage::OnDeltaposSpinWidth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	int nValue = m_pGate->nNsWidth[m_nGate];

	m_pGate->nNsWidth[m_nGate]+=(pNMUpDown->iDelta * 10);
	theApp.FormatTime(Buff, m_pGate->nNsWidth[m_nGate], theApp.m_Scope.m_nGateWidthUnits, m_Units[theApp.m_Scope.m_nGateWidthUnits]);

	if(nValue - m_pGate->nNsWidth[m_nGate]) {
		Buff += m_Units[theApp.m_Scope.m_nGateWidthUnits];
		m_editGateWidth.SetWindowText(Buff);
		InvalidateParent();
	}

	*pResult = 0;
}

void CVolumeAscanPage::OnSelchangeComboCrosshairvalue() 
{
	theApp.m_LastSettings.nVolumeCrossHairValue = m_comboCrossHairValue.GetCurSel();
}
