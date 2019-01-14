// GatesPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "GatesPage.h"
#include "UltrasonicsSheet.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGatesPage property page

IMPLEMENT_DYNCREATE(CGatesPage, CPropertyPage)

CGatesPage::CGatesPage() : CPropertyPage(CGatesPage::IDD)
{
	//{{AFX_DATA_INIT(CGatesPage)
	//}}AFX_DATA_INIT
	
	m_nGate = 0;
	m_nTOF = 0;
	m_Units[0] = "ns";
	m_Units[1] = "\x0b5s";
	m_Units[2] = "mm";
	m_Units[3] = "mm";
	m_Units[4] = "\x022";
	m_Units[5] = "\x022";
	m_nAccel[0] = 1;
	m_nAccel[1] = 10;
	m_nAccel[2] = 10;
	m_nAccel[3] = 10;
	m_nAccel[4] = 10;
	m_nAccel[5] = 10;
}

CGatesPage::~CGatesPage()
{
}

void CGatesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGatesPage)
	DDX_Control(pDX, IDC_STATIC_PHASE_POSITIVE, m_staticPosPhaseColor);
	DDX_Control(pDX, IDC_STATIC_PHASE_NEGATIVE, m_staticNegPhaseColor);
	DDX_Control(pDX, IDC_COMBO_COLLECT_PHASE, m_comboCollectPhase);
	DDX_Control(pDX, IDC_EDIT_PEAK_WIDTH, m_editPeakWidth);
	DDX_Control(pDX, IDC_SPIN_PEAK_WIDTH, m_spinPeakWidth);
	DDX_Control(pDX, IDC_SPIN_FAR_DEADZONE, m_spinFarDeadZone);
	DDX_Control(pDX, IDC_EDIT_FAR_DELAY, m_editFarDeadZone);
	DDX_Control(pDX, IDC_SPIN_POST_TRIGGER, m_spinPostTrigger);
	DDX_Control(pDX, IDC_SPIN_PRE_TRIGGER, m_spinPreTrigger);
	DDX_Control(pDX, IDC_EDIT_PRE_TRIGGER, m_editPreTrigger);
	DDX_Control(pDX, IDC_EDIT_POST_TRIGGER, m_editPostTrigger);
	DDX_Control(pDX, IDC_COMBO_COLLECT_A_SCAN, m_comboCollectAScan);
	DDX_Control(pDX, IDC_CHECK_TOF_0, m_checkTof0);
	DDX_Control(pDX, IDC_CHECK_TOF_1, m_checkTof1);
	DDX_Control(pDX, IDC_CHECK_TOF_2, m_checkTof2);
	DDX_Control(pDX, IDC_CHECK_TOF_3, m_checkTof3);
	DDX_Control(pDX, IDC_CHECK_TOF_4, m_checkTof4);
	DDX_Control(pDX, IDC_CHECK_TOF_5, m_checkTof5);
	DDX_Control(pDX, IDC_CHECK_TOF_6, m_checkTof6);
	DDX_Control(pDX, IDC_CHECK_TOF_7, m_checkTof7);
	DDX_Control(pDX, IDC_COMBO_TOF_LAST, m_comboTOFStopGate);
	DDX_Control(pDX, IDC_COMBO_TOF_FIRST, m_comboTOFStartGate);
	DDX_Control(pDX, IDC_COMBO_TOF, m_comboTOFNumber);
	DDX_Control(pDX, IDC_CHECK_GATE_7, m_checkGate7);
	DDX_Control(pDX, IDC_CHECK_GATE_6, m_checkGate6);
	DDX_Control(pDX, IDC_CHECK_GATE_5, m_checkGate5);
	DDX_Control(pDX, IDC_CHECK_GATE_4, m_checkGate4);
	DDX_Control(pDX, IDC_CHECK_GATE_3, m_checkGate3);
	DDX_Control(pDX, IDC_CHECK_GATE_2, m_checkGate2);
	DDX_Control(pDX, IDC_CHECK_GATE_1, m_checkGate1);
	DDX_Control(pDX, IDC_CHECK_GATE_0, m_checkGate0);
	DDX_Control(pDX, IDC_STATIC_GATE_COLOR_7, m_staticGateColor7);
	DDX_Control(pDX, IDC_STATIC_GATE_COLOR_5, m_staticGateColor5);
	DDX_Control(pDX, IDC_STATIC_GATE_COLOR_6, m_staticGateColor6);
	DDX_Control(pDX, IDC_STATIC_GATE_COLOR_4, m_staticGateColor4);
	DDX_Control(pDX, IDC_STATIC_GATE_COLOR_3, m_staticGateColor3);
	DDX_Control(pDX, IDC_STATIC_GATE_COLOR_2, m_staticGateColor2);
	DDX_Control(pDX, IDC_STATIC_GATE_COLOR_1, m_staticGateColor1);
	DDX_Control(pDX, IDC_STATIC_GATE_COLOR_0, m_staticGateColor0);
	DDX_Control(pDX, IDC_COMBO_TOF_MODE, m_comboTOFMode);
	DDX_Control(pDX, IDC_COMBO_WHICH_TIMESLOT, m_comboTimeSlot);
	DDX_Control(pDX, IDC_SPIN_GATE_THRESHOLD, m_spinThreshold);
	DDX_Control(pDX, IDC_EDIT_GATE_THRESHOLD, m_editThreshold);
	DDX_Control(pDX, IDC_SPIN_WIDTH, m_spinGateWidth);
	DDX_Control(pDX, IDC_SPIN_DELAY, m_spinGateDelay);
	DDX_Control(pDX, IDC_STATIC_DELAY, m_staticDelay);
	DDX_Control(pDX, IDC_STATIC_WIDTH, m_staticWidth);
	DDX_Control(pDX, IDC_STATIC_THRESHOLD, m_staticThreshold);
	DDX_Control(pDX, IDC_EDIT_GATE_WIDTH, m_editGateWidth);
	DDX_Control(pDX, IDC_EDIT_GATE_DELAY, m_editGateDelay);
	DDX_Control(pDX, IDC_COMBO_GATE_NUMBER, m_comboGateNumber);
	DDX_Control(pDX, IDC_COMBO_GATE_MODE, m_comboGateMode);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_NUMBER_TIMESLOTS, m_comboNumberTimeSlots);
	DDX_Control(pDX, IDC_COMBO_AVERAGES, m_comboAverages);
}


BEGIN_MESSAGE_MAP(CGatesPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGatesPage)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_WHICH_TIMESLOT, OnSelchangeComboWhichTimeslot)
	ON_CBN_SELCHANGE(IDC_COMBO_GATE_NUMBER, OnSelchangeComboGateNumber)
	ON_CBN_SELCHANGE(IDC_COMBO_GATE_MODE, OnSelchangeComboGateMode)
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_TOF_MODE, OnSelchangeComboTofMode)
	ON_EN_CHANGE(IDC_EDIT_GATE_DELAY, OnChangeEditGateDelay)
	ON_EN_CHANGE(IDC_EDIT_GATE_WIDTH, OnChangeEditGateWidth)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_TOF_LAST, OnSelchangeComboTofStopGate)
	ON_CBN_SELCHANGE(IDC_COMBO_TOF_FIRST, OnSelchangeComboTofStartGate)
	ON_CBN_SELCHANGE(IDC_COMBO_TOF, OnSelchangeComboTof)
	ON_BN_CLICKED(IDC_CHECK_TOF_0, OnCheckTof0)
	ON_BN_CLICKED(IDC_CHECK_TOF_1, OnCheckTof1)
	ON_BN_CLICKED(IDC_CHECK_TOF_2, OnCheckTof2)
	ON_BN_CLICKED(IDC_CHECK_TOF_3, OnCheckTof3)
	ON_BN_CLICKED(IDC_CHECK_TOF_4, OnCheckTof4)
	ON_BN_CLICKED(IDC_CHECK_TOF_5, OnCheckTof5)
	ON_BN_CLICKED(IDC_CHECK_TOF_6, OnCheckTof6)
	ON_BN_CLICKED(IDC_CHECK_TOF_7, OnCheckTof7)
	ON_CBN_SELCHANGE(IDC_COMBO_COLLECT_A_SCAN, OnSelchangeComboCollectAScan)
	ON_EN_CHANGE(IDC_EDIT_PRE_TRIGGER, OnChangeEditPreTrigger)
	ON_EN_CHANGE(IDC_EDIT_POST_TRIGGER, OnChangeEditPostTrigger)
	ON_WM_HELPINFO()
	ON_EN_CHANGE(IDC_EDIT_FAR_DELAY, OnChangeEditFarDelay)
	ON_EN_CHANGE(IDC_EDIT_PEAK_WIDTH, OnChangeEditPeakWidth)
	ON_CBN_SELCHANGE(IDC_COMBO_DEFAULT_TOF, OnSelchangeComboDefaultTof)
	ON_BN_CLICKED(IDC_CHECK_GATE_0, OnCheckGate0)
	ON_BN_CLICKED(IDC_CHECK_GATE_1, OnCheckGate1)
	ON_BN_CLICKED(IDC_CHECK_GATE_2, OnCheckGate2)
	ON_BN_CLICKED(IDC_CHECK_GATE_3, OnCheckGate3)
	ON_BN_CLICKED(IDC_CHECK_GATE_4, OnCheckGate4)
	ON_BN_CLICKED(IDC_CHECK_GATE_5, OnCheckGate5)
	ON_BN_CLICKED(IDC_CHECK_GATE_6, OnCheckGate6)
	ON_BN_CLICKED(IDC_CHECK_GATE_7, OnCheckGate7)
	ON_BN_CLICKED(IDC_CHECK_TOF, OnCheckTof)
	ON_BN_CLICKED(IDC_CHECK_PHASE, OnCheckPhase)
	ON_CBN_SELCHANGE(IDC_COMBO_COLLECT_PHASE, OnSelchangeComboCollectPhase)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_CBN_SELCHANGE(IDC_COMBO_NUMBER_TIMESLOTS, &CGatesPage::OnCbnSelchangeComboNumberTimeslots)
	ON_CBN_SELCHANGE(IDC_COMBO_AVERAGES, &CGatesPage::OnCbnSelchangeComboAverages)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DELAY, &CGatesPage::OnDeltaposSpinDelay)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGatesPage message handlers

BOOL CGatesPage::OnInitDialog() 
{
	CString Buff,Language;
	int ii;
	CPropertyPage::OnInitDialog();

	m_spinGateDelay.SetRange(0,20000);
	m_spinGateWidth.SetRange(0,20000);
	m_spinFarDeadZone.SetRange(0,20000);
	m_spinThreshold.SetRange(0,200);
	m_spinPeakWidth.SetRange(0,20000);

	Buff.LoadString(IDS_Off);					m_comboTOFMode.AddString(Buff);
	Buff.LoadString(IDS_First_Edge);			m_comboTOFMode.AddString(Buff);
	Buff.LoadString(IDS_Any_peak);				m_comboTOFMode.AddString(Buff);
	Buff.LoadString(IDS_Last_Edge);				m_comboTOFMode.AddString(Buff);
	Buff.LoadString(IDS_First_Peak);			m_comboTOFMode.AddString(Buff);
	Buff.LoadString(IDS_Peak_above_threshold);	m_comboTOFMode.AddString(Buff);

	Buff.LoadString(IDS_Water_Path); m_comboTOFNumber.AddString(Buff);
	for(ii=1;ii<8;ii++) {
		Buff.Format(_T("TOF %d"),ii);
		m_comboTOFNumber.AddString(Buff);
	}
	Buff.LoadString(IDS_Interface); m_comboTOFStopGate.AddString(Buff);
	for(ii=1;ii<8;ii++) {
		Language.LoadString(IDS_Gate);
		Buff.Format(_T("%s %d"),Language,ii);
		m_comboTOFStopGate.AddString(Buff);
	}
	Buff.LoadString(IDS_Main_Bang); m_comboTOFStartGate.AddString(Buff);
	Buff.LoadString(IDS_Interface); m_comboTOFStartGate.AddString(Buff);
	for(ii=1;ii<8;ii++) {
		Language.LoadString(IDS_Gate);
		Buff.Format(_T("%s %d"),Language,ii);
		m_comboTOFStartGate.AddString(Buff);
	}

	Buff.LoadString(IDS_Off); m_comboCollectAScan.AddString(Buff);
	Buff.LoadString(IDS_Entire_Trace); m_comboCollectAScan.AddString(Buff);
	Buff.LoadString(IDS_Interface_Gate); m_comboCollectAScan.AddString(Buff);

	UDACCEL	Accel;
	Accel.nInc=10;
	Accel.nSec=1;

	m_spinPreTrigger.SetRange(0,30000);
	m_spinPreTrigger.SetAccel(1,&Accel);
	m_spinPostTrigger.SetRange(0,30000);
	m_spinPostTrigger.SetAccel(1,&Accel);

//	Buff.LoadString(IDS_Error);						m_comboDefaultTOF.AddString(Buff);
//	Buff.LoadString(IDS_Zero);						m_comboDefaultTOF.AddString(Buff);
//	Buff.LoadString(IDS_Minimum_thickness);			m_comboDefaultTOF.AddString(Buff);
//	Buff.LoadString(IDS_Maximum_thickness);			m_comboDefaultTOF.AddString(Buff);

	m_comboNumberTimeSlots.AddString(_T("1"));
	m_comboNumberTimeSlots.AddString(_T("2"));
	m_comboNumberTimeSlots.AddString(_T("3"));
	m_comboNumberTimeSlots.AddString(_T("4"));
	m_comboNumberTimeSlots.AddString(_T("5"));
	m_comboNumberTimeSlots.AddString(_T("6"));
	m_comboNumberTimeSlots.AddString(_T("7"));
	m_comboNumberTimeSlots.AddString(_T("8"));

	m_comboAverages.AddString(_T("Off"));
	m_comboAverages.AddString(_T("2"));
	m_comboAverages.AddString(_T("4"));
	m_comboAverages.AddString(_T("8"));
	m_comboAverages.AddString(_T("16"));
	m_comboAverages.AddString(_T("32"));

	UpdateAllControls();
	EnableToolTips(TRUE);

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CGatesPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CGatesPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	CString	Buff,StopBefore;
	UDACCEL	Accel;
	Accel.nInc=3;
	Accel.nSec=1;
	m_nTimeSlot = theApp.m_UtUser.m_Global.nTimeSlot;
	GatesData* pGate = &theApp.m_UtUser.m_TS[m_nTimeSlot].Gate;
	int ii;

	//Combo Timeslot
	m_comboTimeSlot.ResetContent();
	if(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]==2) {
		m_comboTimeSlot.InsertString(0,_T("LSA"));
		m_comboTimeSlot.SetCurSel(0);
		m_comboTimeSlot.EnableWindow(false);
	}else{
		for(int nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
			m_comboTimeSlot.AddString(theApp.m_UtUser.m_TS[nTS].cName);
		}
		m_comboTimeSlot.SetCurSel(theApp.m_UtUser.m_Global.nTimeSlot);
	}	

	m_comboGateNumber.SetCurSel(m_nGate);
	m_comboNumberTimeSlots.SetCurSel(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots - 1);
	m_comboAverages.SetCurSel(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nAverages);

	m_spinGateDelay.SetPos(pGate->nNsDelay[m_nGate]/10);
	m_spinGateWidth.SetPos(pGate->nNsWidth[m_nGate]/10);
	m_spinFarDeadZone.SetPos(pGate->nNsFarDeadZone[m_nGate]/10);
	m_spinThreshold.SetPos(pGate->nThreshold[m_nGate]+100);

	theApp.FormatTime(Buff, pGate->nNsDelay[m_nGate], theApp.m_Scope.m_nGateDelayUnits, m_Units[theApp.m_Scope.m_nGateDelayUnits]);
	m_editGateDelay.SetWindowText(Buff);
	Accel.nInc = m_nAccel[theApp.m_Scope.m_nGateDelayUnits];
	m_spinGateDelay.SetAccel(1, &Accel);

	theApp.FormatTime(Buff, pGate->nNsWidth[m_nGate], theApp.m_Scope.m_nGateWidthUnits, m_Units[theApp.m_Scope.m_nGateWidthUnits]);
	m_editGateWidth.SetWindowText(Buff);
	Accel.nInc = m_nAccel[theApp.m_Scope.m_nGateWidthUnits];
	m_spinGateWidth.SetAccel(1, &Accel);

	theApp.FormatTime(Buff, pGate->nNsFarDeadZone[m_nGate], theApp.m_Scope.m_nGateDelayUnits, m_Units[theApp.m_Scope.m_nGateDelayUnits]);
	m_editFarDeadZone.SetWindowText(Buff);
	Accel.nInc = m_nAccel[theApp.m_Scope.m_nGateDelayUnits];
	m_spinFarDeadZone.SetAccel(1, &Accel);

	theApp.FormatTime(Buff, pGate->nNsPeakWidth, theApp.m_Scope.m_nGateWidthUnits, m_Units[theApp.m_Scope.m_nGateWidthUnits]);
	m_editPeakWidth.SetWindowText(Buff);
	Accel.nInc = m_nAccel[theApp.m_Scope.m_nGateWidthUnits];
	m_spinPeakWidth.SetAccel(1, &Accel);

	Buff.Format(_T("%d"),pGate->nThreshold[m_nGate]);
	m_editThreshold.SetWindowText(Buff);

	m_comboGateNumber.ResetContent();
	m_comboCollectPhase.ResetContent();
	m_comboCollectPhase.AddString(L" ");
	m_comboGateNumber.LimitText(sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.cName[0] - 1);
	for(int gg=0;gg<8;gg++) {
		m_comboGateNumber.AddString(theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.cName[gg]);
		m_comboCollectPhase.AddString(theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.cName[gg]);
	}
	m_comboGateNumber.SetCurSel(m_nGate);

	for(ii=0;ii<8;ii++) {
		if(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectPhase[ii] == 1) {
			m_comboCollectPhase.SetCurSel(ii + 1);
		}
	}

	m_comboGateMode.ResetContent();
	Buff.LoadString(IDS_Off); m_comboGateMode.AddString(Buff);
	Buff.LoadString(IDS_Main_Bang); m_comboGateMode.AddString(Buff);
	Buff.LoadString(IDS_Interface); m_comboGateMode.AddString(Buff);
	StopBefore.LoadString(IDS_StopBefore);
	for(int gg=1;gg<m_nGate;gg++) {
		Buff.Format(_T("<%s"),theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.cName[gg]);
		m_comboGateMode.AddString(Buff);
	}
	m_comboGateMode.SetCurSel(pGate->nGateMode[m_nGate]);

	m_comboTOFMode.SetCurSel(pGate->nTimsDetectionMode[m_nGate]);

	m_checkGate0.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[0]&1);
	m_checkGate1.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[1]&1);
	m_checkGate2.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[2]&1);
	m_checkGate3.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[3]&1);
	m_checkGate4.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[4]&1);
	m_checkGate5.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[5]&1);
	m_checkGate6.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[6]&1);
	m_checkGate7.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[7]&1);
	m_checkTof0.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[0]&1);
	m_checkTof1.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[1]&1);
	m_checkTof2.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[2]&1);
	m_checkTof3.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[3]&1);
	m_checkTof4.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[4]&1);
	m_checkTof5.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[5]&1);
	m_checkTof6.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[6]&1);
	m_checkTof7.SetCheck(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[7]&1);

	m_comboTOFNumber.SetCurSel(m_nTOF);
	m_comboTOFStartGate.SetCurSel(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nTOFStartGate[m_nTOF]);
	m_comboTOFStopGate.SetCurSel(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nTOFStopGate[m_nTOF]);

	CString Micro = L"\x0b5";
	Buff.Format(_T("%.02f%ss"),(double)theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanPreTrigger/1000,Micro);
	m_editPreTrigger.SetWindowText(Buff);
	Buff.Format(_T("%.02f%ss"),(double)theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanPostTrigger/1000,Micro);
	m_editPostTrigger.SetWindowText(Buff);
	m_spinPreTrigger.SetPos(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanPreTrigger);
	m_spinPostTrigger.SetPos(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanPostTrigger);

	m_comboCollectAScan.SetCurSel(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanMode);

//	m_comboDefaultTOF.SetCurSel( theApp.m_LastSettings.nDefaultTOF );

	SetAccessPrivelages();
}


BOOL CGatesPage::OnSetActive() 
{
	UpdateAllControls();
	return CPropertyPage::OnSetActive();
}


void CGatesPage::OnSelchangeComboWhichTimeslot() 
{
	theApp.m_UtUser.m_Global.nTimeSlot = m_comboTimeSlot.GetCurSel();
	if(m_nTimeSlot - theApp.m_UtUser.m_Global.nTimeSlot) {
		UpdateAllControls();
	}
	
}

BOOL CGatesPage::PreTranslateMessage(MSG* pMsg) 
{
	CString Buff;
	CUltrasonicsSheet* pParent;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(pMsg->message==0x100) {

		switch(pMsg->wParam) {
		case 13:
			m_comboGateNumber.GetWindowText(Buff);
			m_comboGateNumber.DeleteString(m_nGate);
			m_comboGateNumber.InsertString(m_nGate,Buff);
			m_comboGateNumber.SetCurSel(m_nGate);
			wcscpy_s(theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.cName[m_nGate],60,(WCHAR*)LPCTSTR(Buff));

			m_comboTimeSlot.GetWindowText(Buff);
			m_comboTimeSlot.DeleteString(m_nTimeSlot);
			m_comboTimeSlot.InsertString(m_nTimeSlot,Buff);
			m_comboTimeSlot.SetCurSel(m_nTimeSlot);
			wcscpy_s(theApp.m_UtUser.m_TS[m_nTimeSlot].cName,60,(WCHAR*)LPCTSTR(Buff));
			pParent = (CUltrasonicsSheet*) GetParent();
			pParent->SetTabNames();
			if(pFrame->m_pMotionToolsSheet) pFrame->m_pMotionToolsSheet->UpdateAllPages();

			UpdateAllControls();
			return TRUE;
			break;
		}
	}

	
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CGatesPage::OnSelchangeComboGateNumber() 
{
	m_nGate = m_comboGateNumber.GetCurSel();
	UpdateAllControls();
}

void CGatesPage::OnSelchangeComboGateMode() 
{
	theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nGateMode[m_nGate] = m_comboGateMode.GetCurSel();
	SetAccessPrivelages();
}

void CGatesPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString	Buff;
	GatesData* pGate = &theApp.m_UtUser.m_TS[m_nTimeSlot].Gate;
	int nTemp;

	CString Micro = L"\x0b5";

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_PRE_TRIGGER:
		theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanPreTrigger=(pSpin->GetPos()&0xffff);
		Buff.Format(_T("%.02f%ss"),(double)theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanPreTrigger/1000,Micro);
		m_editPreTrigger.SetWindowText(Buff);
		break;
	case IDC_SPIN_POST_TRIGGER:
		theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanPostTrigger=(pSpin->GetPos()&0xffff);
		Buff.Format(_T("%.02f%ss"),(double)theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanPostTrigger/1000,Micro);
		m_editPostTrigger.SetWindowText(Buff);
		break;
		/*
	case IDC_SPIN_DELAY:
		nTemp = pGate->nNsDelay[m_nGate];
		pGate->nNsDelay[m_nGate]=(pSpin->GetPos()&0xffff)*10;
		if(nTemp - pGate->nNsDelay[m_nGate]) {
			theApp.FormatTime(Buff, pGate->nNsDelay[m_nGate], theApp.m_Scope.m_nGateDelayUnits, m_Units[theApp.m_Scope.m_nGateDelayUnits]);
			m_editGateDelay.SetWindowText(Buff);
			theApp.m_UtUser.Dsp200CalculateHardware(theApp.m_UtUser.m_Global.nTimeSlot,m_nGate);
			CopyGates();
		}
		break;
		*/
	case IDC_SPIN_WIDTH:
		nTemp = pGate->nNsWidth[m_nGate];
		pGate->nNsWidth[m_nGate]=(pSpin->GetPos()&0xffff)*10;
		if(nTemp - pGate->nNsWidth[m_nGate]) {
			theApp.FormatTime(Buff, pGate->nNsWidth[m_nGate], theApp.m_Scope.m_nGateWidthUnits, m_Units[theApp.m_Scope.m_nGateWidthUnits]);
			m_editGateWidth.SetWindowText(Buff);
			theApp.m_UtUser.Dsp200CalculateHardware(theApp.m_UtUser.m_Global.nTimeSlot,m_nGate);
			CopyGates();
		}
		break;
	case IDC_SPIN_GATE_THRESHOLD:
		nTemp = pGate->nThreshold[m_nGate];
		pGate->nThreshold[m_nGate]=(pSpin->GetPos()&0xffff)-100;
		if(nTemp - pGate->nThreshold[m_nGate] ) {
			Buff.Format(_T("%d"),pGate->nThreshold[m_nGate]);
			m_editThreshold.SetWindowText(Buff);
			theApp.m_UtUser.Dsp200CalculateHardware(theApp.m_UtUser.m_Global.nTimeSlot,m_nGate);
			CopyGates();
		}
		break;
	case IDC_SPIN_FAR_DEADZONE:
		nTemp = pGate->nNsFarDeadZone[m_nGate];
		pGate->nNsFarDeadZone[m_nGate]=(pSpin->GetPos()&0xffff)*10;
		if(nTemp - pGate->nNsFarDeadZone[m_nGate]) {
			theApp.FormatTime(Buff, pGate->nNsFarDeadZone[m_nGate], theApp.m_Scope.m_nGateDelayUnits, m_Units[theApp.m_Scope.m_nGateDelayUnits]);
			m_editFarDeadZone.SetWindowText(Buff);
			theApp.m_UtUser.Dsp200CalculateHardware(theApp.m_UtUser.m_Global.nTimeSlot,m_nGate);
			CopyGates();
		}
		break;
	case IDC_SPIN_PEAK_WIDTH:
		nTemp = pGate->nNsPeakWidth;
		pGate->nNsPeakWidth=(pSpin->GetPos()&0xffff)*10;
		if(nTemp - pGate->nNsPeakWidth) {
			theApp.FormatTime(Buff, pGate->nNsPeakWidth, theApp.m_Scope.m_nGateWidthUnits, m_Units[theApp.m_Scope.m_nGateWidthUnits]);
			m_editPeakWidth.SetWindowText(Buff);
			theApp.m_UtUser.Dsp200CalculateHardware(theApp.m_UtUser.m_Global.nTimeSlot,m_nGate);
			CopyGates();
		}
		break;
	}
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CGatesPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
   LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

	switch(nID) {
	case IDC_COMBO_GATE_NUMBER:
	case IDC_COMBO_WHICH_TIMESLOT:  strTipText = "This field is editable";
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
  //    ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

void CGatesPage::OnSelchangeComboTofMode() 
{
	GatesData* pGate = &theApp.m_UtUser.m_TS[m_nTimeSlot].Gate;
	pGate->nTimsDetectionMode[m_nGate] = m_comboTOFMode.GetCurSel();
	theApp.m_UtUser.Dsp200CalculateHardware(theApp.m_UtUser.m_Global.nTimeSlot,m_nGate);
	CopyGates();
	
}



void CGatesPage::OnChangeEditGateDelay() 
{
	GatesData* pGate = &theApp.m_UtUser.m_TS[m_nTimeSlot].Gate;
	CString	Buff;
	int nDelay;
	int nValue=pGate->nNsDelay[m_nGate];

	m_editGateDelay.GetWindowText(Buff);
	nDelay = theApp.ScanfDistanceTimens(Buff, theApp.m_Scope.m_nGateDelayUnits);

	if(abs(nValue-nDelay) > 20) {
		pGate->nNsDelay[m_nGate] = nDelay;
		m_spinGateDelay.SetPos(pGate->nNsDelay[m_nGate]/10);
		theApp.m_UtUser.Dsp200CalculateHardware(m_nTimeSlot,m_nGate);
	}
	CopyGates();
}

void CGatesPage::OnChangeEditGateWidth() 
{
	GatesData* pGate = &theApp.m_UtUser.m_TS[m_nTimeSlot].Gate;
	CString	Buff;
	int nWidth;
	int nValue=pGate->nNsWidth[m_nGate];

	m_editGateWidth.GetWindowText(Buff);
	nWidth = theApp.ScanfDistanceTimens(Buff, theApp.m_Scope.m_nGateWidthUnits);

	if(abs(nValue-nWidth)>20) {
		pGate->nNsWidth[m_nGate] = nWidth;
		m_spinGateWidth.SetPos(pGate->nNsWidth[m_nGate]/10);
		theApp.m_UtUser.Dsp200CalculateHardware(m_nTimeSlot,m_nGate);
	}
	CopyGates();
}

void CGatesPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	int	gg;
	CMFCColorDialog dlg;

	m_staticNegPhaseColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_Scope.m_rgbNegPhaseColor=dlg.GetColor();
			Invalidate(FALSE);
		}
	}

	m_staticPosPhaseColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_Scope.m_rgbPosPhaseColor=dlg.GetColor();
			Invalidate(FALSE);
		}
	}

	for(gg=0;gg<8;gg++) {
		switch(gg) {
		case 0: m_staticGateColor0.GetWindowRect(&rr);
			break;
		case 1: m_staticGateColor1.GetWindowRect(&rr);
			break;
		case 2: m_staticGateColor2.GetWindowRect(&rr);
			break;
		case 3: m_staticGateColor3.GetWindowRect(&rr);
			break;
		case 4: m_staticGateColor4.GetWindowRect(&rr);
			break;
		case 5: m_staticGateColor5.GetWindowRect(&rr);
			break;
		case 6: m_staticGateColor6.GetWindowRect(&rr);
			break;
		case 7: m_staticGateColor7.GetWindowRect(&rr);
			break;
		}
		ScreenToClient(&rr);

		if(rr.PtInRect(point)) {
			if(dlg.DoModal()==IDOK) {
				theApp.m_Scope.m_rgbGateColor[gg]=dlg.GetColor();
				Invalidate(FALSE);
			};
		}
	}
	
	CPropertyPage::OnLButtonDown(nFlags, point);
}

void CGatesPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	BlockFill(&dc,&m_staticPosPhaseColor,theApp.m_Scope.m_rgbPosPhaseColor);
	BlockFill(&dc,&m_staticNegPhaseColor,theApp.m_Scope.m_rgbNegPhaseColor);

	BlockFill(&dc,&m_staticGateColor0,theApp.m_Scope.m_rgbGateColor[0]);
	BlockFill(&dc,&m_staticGateColor1,theApp.m_Scope.m_rgbGateColor[1]);
	BlockFill(&dc,&m_staticGateColor2,theApp.m_Scope.m_rgbGateColor[2]);
	BlockFill(&dc,&m_staticGateColor3,theApp.m_Scope.m_rgbGateColor[3]);
	BlockFill(&dc,&m_staticGateColor4,theApp.m_Scope.m_rgbGateColor[4]);
	BlockFill(&dc,&m_staticGateColor5,theApp.m_Scope.m_rgbGateColor[5]);
	BlockFill(&dc,&m_staticGateColor6,theApp.m_Scope.m_rgbGateColor[6]);
	BlockFill(&dc,&m_staticGateColor7,theApp.m_Scope.m_rgbGateColor[7]);

}

void CGatesPage::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}

void CGatesPage::OnCheckGate0() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[0]=m_checkGate0.GetCheck()&1;	
	UpdateAllControls();
	CopyGates();
}

void CGatesPage::OnCheckGate1() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[1]=m_checkGate1.GetCheck()&1;	
	UpdateAllControls();
	CopyGates();
}

void CGatesPage::OnCheckGate2() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[2]=m_checkGate2.GetCheck()&1;	
	UpdateAllControls();
	CopyGates();
}

void CGatesPage::OnCheckGate3() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[3]=m_checkGate3.GetCheck()&1;	
	UpdateAllControls();
	CopyGates();
}

void CGatesPage::OnCheckGate4() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[4]=m_checkGate4.GetCheck()&1;
	UpdateAllControls();
	CopyGates();
}

void CGatesPage::OnCheckGate5() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[5]=m_checkGate5.GetCheck()&1;	
	UpdateAllControls();
	CopyGates();
}

void CGatesPage::OnCheckGate6() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[6]=m_checkGate6.GetCheck()&1;	
	UpdateAllControls();
	CopyGates();
}

void CGatesPage::OnCheckGate7() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectAmp[7]=m_checkGate7.GetCheck()&1;	
	UpdateAllControls();
	CopyGates();
}

void CGatesPage::OnCheckTof() 
{
//	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[1]=m_checkTOF.GetCheck()&1;
	CopyGates();
	
}

void CGatesPage::OnCheckPhase() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectPhase[0]=m_checkPhase.GetCheck()&1;
	UpdateAllControls();
	CopyGates();
	
}

void CGatesPage::OnSelchangeComboTof() 
{
	m_nTOF = m_comboTOFNumber.GetCurSel();
	UpdateAllControls();
	CopyGates();
}


void CGatesPage::OnSelchangeComboTofStartGate() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nTOFStartGate[m_nTOF] = m_comboTOFStartGate.GetCurSel();
	UpdateAllControls();
	CopyGates();
}

void CGatesPage::OnSelchangeComboTofStopGate() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nTOFStopGate[m_nTOF] = m_comboTOFStopGate.GetCurSel();
	UpdateAllControls();
	CopyGates();
}


void CGatesPage::OnCheckTof0() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[0]=m_checkTof0.GetCheck()&1;	
	CopyGates();
}

void CGatesPage::OnCheckTof1() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[1]=m_checkTof1.GetCheck()&1;	
	CopyGates();
}

void CGatesPage::OnCheckTof2() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[2]=m_checkTof2.GetCheck()&1;	
	CopyGates();
}

void CGatesPage::OnCheckTof3() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[3]=m_checkTof3.GetCheck()&1;	
	CopyGates();
}

void CGatesPage::OnCheckTof4() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[4]=m_checkTof4.GetCheck()&1;	
	CopyGates();
}

void CGatesPage::OnCheckTof5() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[5]=m_checkTof5.GetCheck()&1;	
	CopyGates();
}

void CGatesPage::OnCheckTof6() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[6]=m_checkTof6.GetCheck()&1;	
	CopyGates();
}

void CGatesPage::OnCheckTof7() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[7]=m_checkTof7.GetCheck()&1;	
	CopyGates();

}

void CGatesPage::OnSelchangeComboCollectAScan() 
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanMode=m_comboCollectAScan.GetCurSel();
	theApp.m_UtUser.Dsp200CalculateHardware(m_nTimeSlot,m_nGate);
	EnableDisablePrePostTrigger();
}

void CGatesPage::OnChangeEditPostTrigger() 
{
	CString	Buff;
	float fTemp;
	
	m_editPostTrigger.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanPostTrigger=(int)(fTemp * 1000.0f);
	m_spinPostTrigger.SetPos(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanPostTrigger);
	theApp.m_UtUser.Dsp200CalculateHardware(m_nTimeSlot,m_nGate);

}

void CGatesPage::OnChangeEditPreTrigger() 
{
	CString	Buff;
	float fTemp;
	
	m_editPreTrigger.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanPreTrigger=(int)(fTemp * 1000.0f);
	m_spinPreTrigger.SetPos(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanPreTrigger);
	theApp.m_UtUser.Dsp200CalculateHardware(m_nTimeSlot,m_nGate);
	
}

void CGatesPage::EnableDisablePrePostTrigger()
{
	switch(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanMode) {
	case 0:
	case 1:
		m_editPreTrigger.EnableWindow(FALSE);
		m_editPostTrigger.EnableWindow(FALSE);
		break;
	case 2:
		m_editPreTrigger.EnableWindow(TRUE);
		m_editPostTrigger.EnableWindow(TRUE);
		break;
	}


}

BOOL CGatesPage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	theApp.m_Help.Open(_T("UltrasonicGatesDlg.html"),NULL);
	
	return CPropertyPage::OnHelpInfo(pHelpInfo);
}

void CGatesPage::OnChangeEditFarDelay() 
{
	GatesData* pGate = &theApp.m_UtUser.m_TS[m_nTimeSlot].Gate;
	CString	Buff;
	int nDelay;
	int nValue=pGate->nNsFarDeadZone[m_nGate];

	m_editFarDeadZone.GetWindowText(Buff);
	nDelay = theApp.ScanfDistanceTimens(Buff, theApp.m_Scope.m_nGateDelayUnits);

	if(abs(nValue-nDelay)>20) {
		pGate->nNsFarDeadZone[m_nGate] = nDelay;
		m_spinFarDeadZone.SetPos(pGate->nNsFarDeadZone[m_nGate]/10);
		theApp.m_UtUser.Dsp200CalculateHardware(m_nTimeSlot,m_nGate);
	}
	CopyGates();
}

void CGatesPage::SetAccessPrivelages()
{
	if (GetSafeHwnd() == NULL) return;

	bool bFlag = false;
	if(theApp.m_nLogonLevelMask & theApp.m_cAccess[IDD_GATES_PAGE]) {
		bFlag = true;
	}
	if((theApp.m_LastSettings.nScanMask & _LOCK_UT) && (theApp.m_pScanData != NULL)) {
		bFlag = false;
	}
	if((m_nGate == 0) && (theApp.m_nLogonLevelMask & theApp.m_cAccess[UI_CHANGE_INTERFACE_GATE])) {
		bFlag = true;
	}
	if((m_nGate > 0) && (theApp.m_nLogonLevelMask & theApp.m_cAccess[UI_CHANGE_OTHER_GATES])) {
		bFlag = true;
	}

	if(theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nGateMode[m_nGate] < 3) {
		m_editFarDeadZone.EnableWindow(FALSE);
		m_spinFarDeadZone.EnableWindow(FALSE);
	} else {
		m_editFarDeadZone.EnableWindow(bFlag);
		m_spinFarDeadZone.EnableWindow(bFlag);
	}


	m_checkTof0.EnableWindow(bFlag);
	m_checkTof1.EnableWindow(bFlag);
	m_checkTof2.EnableWindow(bFlag);
	m_checkTof3.EnableWindow(bFlag);
	m_checkTof4.EnableWindow(bFlag);
	m_checkTof5.EnableWindow(bFlag);
	m_checkTof6.EnableWindow(bFlag);
	 m_checkTof7.EnableWindow(bFlag);
	m_comboTOFStopGate.EnableWindow(bFlag);
	m_comboTOFStartGate.EnableWindow(bFlag);
	m_comboTOFNumber.EnableWindow(bFlag);
//	m_staticPosPhaseColor.EnableWindow(bFlag);
//	m_staticNegPhaseColor.EnableWindow(bFlag);
	m_checkGate7.EnableWindow(bFlag);
	m_checkGate6.EnableWindow(bFlag);
	m_checkGate5.EnableWindow(bFlag);
	m_checkGate4.EnableWindow(bFlag);
	m_checkGate3.EnableWindow(bFlag);
	m_checkGate2.EnableWindow(bFlag);
	m_checkGate1.EnableWindow(bFlag);
	m_checkGate0.EnableWindow(bFlag);
	m_staticGateColor7.EnableWindow(bFlag);
	m_staticGateColor5.EnableWindow(bFlag);
	m_staticGateColor6.EnableWindow(bFlag);
	m_staticGateColor4.EnableWindow(bFlag);
	m_staticGateColor3.EnableWindow(bFlag);
	m_staticGateColor2.EnableWindow(bFlag);
	m_staticGateColor1.EnableWindow(bFlag);
	m_staticGateColor0.EnableWindow(bFlag);
	m_comboTOFMode.EnableWindow(bFlag);
//	m_comboTimeSlot.EnableWindow(bFlag);
	m_spinThreshold.EnableWindow(bFlag);
	m_editThreshold.EnableWindow(bFlag);
	m_spinGateWidth.EnableWindow(bFlag);
	m_spinGateDelay.EnableWindow(bFlag);
	m_staticDelay.EnableWindow(bFlag);
	m_staticWidth.EnableWindow(bFlag);
	m_staticThreshold.EnableWindow(bFlag);
	m_editGateWidth.EnableWindow(bFlag);
	m_editGateDelay.EnableWindow(bFlag);
	m_comboGateNumber.EnableWindow(bFlag);
	m_comboGateMode.EnableWindow(bFlag);
	m_editPeakWidth.EnableWindow(bFlag);
	m_spinPeakWidth.EnableWindow(bFlag);
//	m_comboDefaultTOF.EnableWindow(bFlag);
//	m_checkCollectScanLines.EnableWindow(bFlag);
//	m_checkCollectToolPath.EnableWindow(bFlag);


	if(m_nTimeSlot != 0) {
		bFlag = false;
		m_comboCollectAScan.SetCurSel(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nBscanMode = 0);
	};
	m_editFarDeadZone.EnableWindow(bFlag);
	m_spinFarDeadZone.EnableWindow(bFlag);
	m_spinPostTrigger.EnableWindow(bFlag);
	m_spinPreTrigger.EnableWindow(bFlag);
	m_editPreTrigger.EnableWindow(bFlag);
	m_editPostTrigger.EnableWindow(bFlag);
	m_comboCollectAScan.EnableWindow(bFlag);


}

void CGatesPage::SetGateTimeSlot(int nGate,int nTimeSlot)
{
	if((m_nGate - nGate) || (m_nTimeSlot - nTimeSlot)) {
		m_nGate = nGate;
		m_nTimeSlot = nTimeSlot;
		UpdateAllControls();
	}
}


void CGatesPage::OnCheckScanLines() 
{
	PROFILE->m_nCollectScanLines = m_checkCollectScanLines.GetCheck() & 1;

}

void CGatesPage::OnCheckToolPath() 
{
	PROFILE->m_nCollectSurface  = m_checkCollectToolPath.GetCheck() & 1;
}

void CGatesPage::OnChangeEditPeakWidth() 
{
	GatesData* pGate = &theApp.m_UtUser.m_TS[m_nTimeSlot].Gate;
	CString	Buff;
	int nWidth;
	int nValue=pGate->nNsPeakWidth;

	m_editPeakWidth.GetWindowText(Buff);
	nWidth = theApp.ScanfDistanceTimens(Buff, theApp.m_Scope.m_nGateWidthUnits);

	if(abs(nValue-nWidth)>20) {
		pGate->nNsPeakWidth = nWidth;
		m_spinPeakWidth.SetPos(pGate->nNsPeakWidth/10);
	}
	CopyGates();

}

void CGatesPage::CopyGates()
{
	for(int nTS=0;nTS<256;nTS++) {
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_MODE || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nGateMode, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nGateMode, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nGateMode);
		}
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_DELAY || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nNsDelay, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsDelay, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsDelay);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nSampleDelay, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nSampleDelay, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nSampleDelay);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Dsp.Delay, theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.Delay, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.Delay);
		}
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_WIDTH || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nNsWidth, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsWidth, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsWidth);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nSampleWidth, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nSampleWidth, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nSampleWidth);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Dsp.Width, theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.Width, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.Width);
		}
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_THRESHOLD || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nThreshold, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nThreshold, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nThreshold);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Dsp.InterfaceThreshold, theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.InterfaceThreshold, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.InterfaceThreshold);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Dsp.SignDetect, theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.SignDetect, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.SignDetect);
		}
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_FAR_DEAD_ZONE || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nNsFarDeadZone, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsFarDeadZone, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsFarDeadZone);
		}
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_FAR_PEAK_WIDTH || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			theApp.m_UtUser.m_TS[nTS].Gate.nNsPeakWidth = theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsPeakWidth;
		}
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_AMPLITUDE_COLLECT || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nCollectAmp, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nCollectAmp, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nCollectAmp);
		}
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_TOF_COLLECT || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nCollectTime, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nCollectTime, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nCollectTime);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nTOFStopGate, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nTOFStopGate, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nTOFStopGate);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nTOFStartGate, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nTOFStartGate, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nTOFStartGate);
		}
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_TOF_MODE || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nTimsDetectionMode, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nTimsDetectionMode, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nTimsDetectionMode);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Dsp.TimsTriggerMode, theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.TimsTriggerMode, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.TimsTriggerMode);
		}
	}

}

void CGatesPage::OnSelchangeComboDefaultTof() 
{
	theApp.m_LastSettings.nDefaultTOF = m_comboDefaultTOF.GetCurSel();
}

void CGatesPage::OnSelchangeComboCollectPhase() 
{
	int nSel = m_comboCollectPhase.GetCurSel();
	
	ZeroMemory(theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectPhase, sizeof theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectPhase);

	switch(nSel) {
	case 0:
		break;
	default: theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nCollectPhase[nSel - 1] = 1;
		break;
	}

}


void CGatesPage::OnCbnSelchangeComboNumberTimeslots()
{
	CUltrasonicsSheet* pSheet = (CUltrasonicsSheet*)GetParent();
	CMainFrame*  pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	theApp.m_UtUser.m_Global.Mux.nMaxTimeslots = m_comboNumberTimeSlots.GetCurSel() + 1;

	pSheet->SendMessage(UPDATE_TIMESLOTS, NULL, NULL);
	theApp.m_UtUser.ResetMaxTimeSlots();
	theApp.m_UtUser.RefreshAllVariables(0);
	if (pFrame->m_pMotionToolsSheet) {
		pFrame->m_pMotionToolsSheet->UpdateAllPages();
	}
	UpdateAllControls();
}



void CGatesPage::OnCbnSelchangeComboAverages()
{
	theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc.nAverages = m_comboAverages.GetCurSel();
}


void CGatesPage::OnDeltaposSpinDelay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	GatesData* pGate = &theApp.m_UtUser.m_TS[m_nTimeSlot].Gate;
	int nTemp = pGate->nNsDelay[m_nGate];
	CString Buff;

	pGate->nNsDelay[m_nGate] +=  (pNMUpDown ->iDelta * 10);
	if (nTemp - pGate->nNsDelay[m_nGate]) {
		theApp.FormatTime(Buff, pGate->nNsDelay[m_nGate], theApp.m_Scope.m_nGateDelayUnits, m_Units[theApp.m_Scope.m_nGateDelayUnits]);
		m_editGateDelay.SetWindowText(Buff);
		theApp.m_UtUser.Dsp200CalculateHardware(theApp.m_UtUser.m_Global.nTimeSlot, m_nGate);
		CopyGates();
	}
	*pResult = 0;
}
