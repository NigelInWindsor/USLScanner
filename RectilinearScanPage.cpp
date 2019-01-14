// RectilinearScanPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "RectilinearScanPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRectilinearScanPage property page

IMPLEMENT_DYNCREATE(CRectilinearScanPage, CPropertyPage)

CRectilinearScanPage::CRectilinearScanPage() : CPropertyPage(CRectilinearScanPage::IDD)
{
	//{{AFX_DATA_INIT(CRectilinearScanPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	
	m_nOldScanLine = -1;

}

CRectilinearScanPage::~CRectilinearScanPage()
{
}

void CRectilinearScanPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRectilinearScanPage)
	DDX_Control(pDX, IDC_COMBO_BI_UNI, m_comboBiUniDirectional);
	DDX_Control(pDX, IDC_BUTTON_MOVE_LEFT_TO_RIGHT, m_buttonRightToLeft);
	DDX_Control(pDX, IDC_EDIT_CURRENT_LINE, m_editCurrentLine);
	DDX_Control(pDX, IDC_EDIT_RESUME_LINE, m_editResumeLine);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_editFilePath);
	DDX_Control(pDX, IDC_SPIN_SPEED, m_spinScanSpeed);
	DDX_Control(pDX, IDC_EDIT_SPEED, m_editScanSpeed);
	DDX_Control(pDX, IDC_COMBO_SCAN_SIDE, m_comboScanSide);
	DDX_Control(pDX, IDC_SPIN_LENGTH_1, m_spinSlowLength);
	DDX_Control(pDX, IDC_SPIN_LENGTH_0, m_spinFastLength);
	DDX_Control(pDX, IDC_SPIN_INCREMENT_1, m_spinSlowIncrement);
	DDX_Control(pDX, IDC_SPIN_INCREMENT_0, m_spinFastIncrement);
	DDX_Control(pDX, IDC_EDIT_PT_1, m_editFinishPt);
	DDX_Control(pDX, IDC_EDIT_PT_0, m_editStartPt);
	DDX_Control(pDX, IDC_EDIT_LENGTH_1, m_editSlowLength);
	DDX_Control(pDX, IDC_EDIT_LENGTH_0, m_editFastLength);
	DDX_Control(pDX, IDC_EDIT_INCREMENT_1, m_editSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_INCREMENT_0, m_editFastIncrement);
	DDX_Control(pDX, IDC_COMBO_SECONDARY_AXIS, m_comboSlowAxis);
	DDX_Control(pDX, IDC_COMBO_PRIMARY_AXIS, m_comboFastAxis);
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRectilinearScanPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRectilinearScanPage)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_PT_0, OnChangeEditPt0)
	ON_EN_CHANGE(IDC_EDIT_PT_1, OnChangeEditPt1)
	ON_EN_CHANGE(IDC_EDIT_INCREMENT_0, OnChangeEditIncrement0)
	ON_EN_CHANGE(IDC_EDIT_INCREMENT_1, OnChangeEditIncrement1)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_LENGTH_0, OnChangeEditLength0)
	ON_EN_CHANGE(IDC_EDIT_LENGTH_1, OnChangeEditLength1)
	ON_CBN_SELCHANGE(IDC_COMBO_PRIMARY_AXIS, OnSelchangeComboPrimaryAxis)
	ON_CBN_SELCHANGE(IDC_COMBO_SECONDARY_AXIS, OnSelchangeComboSecondaryAxis)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BUTTON_STORE_0, OnButtonStore0)
	ON_BN_CLICKED(IDC_BUTTON_STORE_1, OnButtonStore1)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_0, OnButtonGoto0)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_1, OnButtonGoto1)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_ABORT, OnButtonAbort)
	ON_CBN_SELCHANGE(IDC_COMBO_SCAN_SIDE, OnSelchangeComboScanSide)
	ON_EN_CHANGE(IDC_EDIT_SPEED, OnChangeEditSpeed)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_EN_CHANGE(IDC_EDIT_FILEPATH, OnChangeEditFilepath)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_LEFT_TO_RIGHT, OnButtonMoveLeftToRight)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_RIGHT_TO_LEFT, OnButtonMoveRightToLeft)
	ON_EN_CHANGE(IDC_EDIT_RESUME_LINE, OnChangeEditResumeLine)
	ON_WM_TIMER()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_INCREMENT_0, OnDeltaposSpinIncrement0)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_INCREMENT_1, OnDeltaposSpinIncrement1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LENGTH_0, OnDeltaposSpinLength0)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LENGTH_1, OnDeltaposSpinLength1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPEED, OnDeltaposSpinSpeed)
	ON_CBN_SELCHANGE(IDC_COMBO_BI_UNI, OnSelchangeComboBiUni)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRectilinearScanPage message handlers

BOOL CRectilinearScanPage::OnInitDialog() 
{
	
	CString Buff;
	CPropertyPage::OnInitDialog();
	UDACCEL	Accel;
	Accel.nInc=50;
	Accel.nSec=1;
	
	m_comboFastAxis.AddString(_T("X"));
	m_comboFastAxis.AddString(_T("Y"));
	m_comboFastAxis.AddString(_T("Z"));
	m_comboFastAxis.AddString(_T("Xt"));
	m_comboFastAxis.AddString(_T("Yt"));
	m_comboFastAxis.AddString(_T("T"));


	m_comboSlowAxis.AddString(_T("X"));
	m_comboSlowAxis.AddString(_T("Y"));
	m_comboSlowAxis.AddString(_T("Z"));
	m_comboSlowAxis.AddString(_T("Xt"));
	m_comboSlowAxis.AddString(_T("Yt"));
	m_comboSlowAxis.AddString(_T("T"));
//	m_comboSlowAxis.AddString(_T("Roller"));


	switch(theApp.m_nSide0Orientation) {
	case 0:
		Buff.LoadString(IDS_Left); m_comboScanSide.AddString(Buff);
		Buff.LoadString(IDS_Right); m_comboScanSide.AddString(Buff);
		Buff.LoadString(IDS_Both); m_comboScanSide.AddString(Buff);
		break;
	case 1:
		Buff.LoadString(IDS_Right); m_comboScanSide.AddString(Buff);
		Buff.LoadString(IDS_Left); m_comboScanSide.AddString(Buff);
		Buff.LoadString(IDS_Both); m_comboScanSide.AddString(Buff);
		break;
	}

	Buff.LoadString(IDS_Coordinates);	m_comboMode.AddString(Buff);
	Buff.LoadString(IDS_Length);		m_comboMode.AddString(Buff);

	m_spinFastIncrement.SetRange(0,200);
	m_spinSlowIncrement.SetRange(0,200);
	m_spinFastLength.SetRange(0,32000);
	m_spinSlowLength.SetRange(0,32000);

	m_spinScanSpeed.SetRange(1,2000);

	Buff.LoadString(IDS_Bidirectional);		m_comboBiUniDirectional.AddString(Buff);
	Buff.LoadString(IDS_Unidirectional);	m_comboBiUniDirectional.AddString(Buff);

	
	UpdateAllControls();

	SetTimer(1,1000,NULL);

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CRectilinearScanPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CRectilinearScanPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	CString Buff;

	if(theApp.m_nUnitsPrecision<1) theApp.m_nUnitsPrecision = 1;

	m_editFilePath.SetWindowText(theApp.m_Data.m_FileName);

	if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable) {

		if(theApp.m_LastSettings.nMeasurementUnits==0) {
			Buff.Format(_T("X: %.1f Y: %.1f Z: %.1f T: %.1f"),PROFILE->m_CpRect[0].Side[0].fX,
														PROFILE->m_CpRect[0].Side[0].fY,
														PROFILE->m_CpRect[0].Side[0].fZ,
														PROFILE->m_CpRect[0].Side[0].fR);
			m_editStartPt.SetWindowText(Buff);

			Buff.Format(_T("X: %.1f Y: %.1f Z: %.1f T: %.1f"),PROFILE->m_CpRect[1].Side[0].fX,
														PROFILE->m_CpRect[1].Side[0].fY,
														PROFILE->m_CpRect[1].Side[0].fZ,
														PROFILE->m_CpRect[1].Side[0].fR);
			m_editFinishPt.SetWindowText(Buff);
		} else {
			Buff.Format(_T("X: %.1f Y: %.1f Z: %.1f T: %.1f"),PROFILE->m_CpRect[0].Side[0].fX * theApp.m_fUnits,
														PROFILE->m_CpRect[0].Side[0].fY * theApp.m_fUnits,
														PROFILE->m_CpRect[0].Side[0].fZ * theApp.m_fUnits,
														PROFILE->m_CpRect[0].Side[0].fR);
			m_editStartPt.SetWindowText(Buff);

			Buff.Format(_T("X: %.1f Y: %.1f Z: %.1f T: %.1f"),PROFILE->m_CpRect[1].Side[0].fX * theApp.m_fUnits,
														PROFILE->m_CpRect[1].Side[0].fY * theApp.m_fUnits,
														PROFILE->m_CpRect[1].Side[0].fZ * theApp.m_fUnits,
														PROFILE->m_CpRect[1].Side[0].fR);
			m_editFinishPt.SetWindowText(Buff);
		}

	}else{

		if(theApp.m_LastSettings.nMeasurementUnits==0) {
			Buff.Format(_T("X: %.1f Y: %.1f Z: %.1f"),PROFILE->m_CpRect[0].Side[0].fX,
														PROFILE->m_CpRect[0].Side[0].fY,
														PROFILE->m_CpRect[0].Side[0].fZ);
			m_editStartPt.SetWindowText(Buff);

			Buff.Format(_T("X: %.1f Y: %.1f Z: %.1f"),PROFILE->m_CpRect[1].Side[0].fX,
														PROFILE->m_CpRect[1].Side[0].fY,
														PROFILE->m_CpRect[1].Side[0].fZ);
			m_editFinishPt.SetWindowText(Buff);
		} else {
			Buff.Format(_T("X: %.1f Y: %.1f Z: %.1f"),PROFILE->m_CpRect[0].Side[0].fX * theApp.m_fUnits,
														PROFILE->m_CpRect[0].Side[0].fY * theApp.m_fUnits,
														PROFILE->m_CpRect[0].Side[0].fZ * theApp.m_fUnits);
			m_editStartPt.SetWindowText(Buff);

			Buff.Format(_T("X: %.1f Y: %.1f Z: %.1f"),PROFILE->m_CpRect[1].Side[0].fX * theApp.m_fUnits,
														PROFILE->m_CpRect[1].Side[0].fY * theApp.m_fUnits,
														PROFILE->m_CpRect[1].Side[0].fZ * theApp.m_fUnits);
			m_editFinishPt.SetWindowText(Buff);
		}
	}

	Buff.Format(_T("%.*f %s"),theApp.m_LastSettings.nStepSizePrecision,PROFILE->m_fFastIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editFastIncrement.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s"),theApp.m_LastSettings.nStepSizePrecision,PROFILE->m_fSlowIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editSlowIncrement.SetWindowText(Buff);

	Buff.Format(_T("%.*f %s"),theApp.m_LastSettings.nStepSizePrecision,PROFILE->m_fFastScanSize * theApp.m_fUnits,theApp.m_Units);
	m_editFastLength.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s"),theApp.m_LastSettings.nStepSizePrecision,PROFILE->m_fSlowScanSize * theApp.m_fUnits,theApp.m_Units);
	m_editSlowLength.SetWindowText(Buff);

	Buff.Format(_T("%d"),PROFILE->m_nScanResumeLine+1);
	m_editResumeLine.SetWindowText(Buff);

	m_comboFastAxis.SetCurSel(PROFILE->m_nFastAxis);
	switch(PROFILE->m_nSlowAxis) {
	default: m_comboSlowAxis.SetCurSel(PROFILE->m_nSlowAxis);
		break;
	case 15: m_comboSlowAxis.SetCurSel(6);
		break;
	};

	m_comboMode.SetCurSel(PROFILE->m_nRectilinearScanMode);

	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,PROFILE->m_fScanSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editScanSpeed.SetWindowText(Buff);

	m_comboScanSide.SetCurSel(PROFILE->m_nScanSide);
	m_comboBiUniDirectional.SetCurSel( PROFILE->m_nBiUniDirectionalScan );

	SetAccessPrivelage();
}

void CRectilinearScanPage::OnChangeEditPt0() 
{
	
	CString Buff;
	float fX,fY,fZ,fR;

	m_editStartPt.GetWindowText(Buff);
	if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable) {
		swscanf_s(Buff,_T("X:%f Y:%f Z:%f T:%f"),&fX,&fY,&fZ,&fR);
	}else{
		swscanf_s(Buff,_T("X:%f Y:%f Z:%f"),&fX,&fY,&fZ);
	}
	if(	PROFILE->m_CpRect[0].Side[0].fX - (fX / theApp.m_fUnits)) {
		PROFILE->m_CpRect[0].Side[1].fX = PROFILE->m_CpRect[0].Side[0].fX = fX / theApp.m_fUnits;
	}
	if(	PROFILE->m_CpRect[0].Side[0].fY - (fY / theApp.m_fUnits)) {
		PROFILE->m_CpRect[0].Side[1].fY = PROFILE->m_CpRect[0].Side[0].fY = fY / theApp.m_fUnits;
	}
	if(	PROFILE->m_CpRect[0].Side[0].fZ - (fZ / theApp.m_fUnits)) {
		PROFILE->m_CpRect[0].Side[1].fZ = PROFILE->m_CpRect[0].Side[0].fZ = fZ / theApp.m_fUnits;
	}
	if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable) {
		if(	PROFILE->m_CpRect[0].Side[0].fR - (fR / theApp.m_fUnits)) {
			PROFILE->m_CpRect[0].Side[1].fR = PROFILE->m_CpRect[0].Side[0].fR = fR / theApp.m_fUnits;
		}
	}

}

void CRectilinearScanPage::OnChangeEditPt1() 
{
	
	CString Buff;
	float fX,fY,fZ,fR;

	m_editFinishPt.GetWindowText(Buff);
	if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable) {
		swscanf_s(Buff,_T("X:%f Y:%f Z:%f T:%f"),&fX,&fY,&fZ,&fR);
	}else{
		swscanf_s(Buff,_T("X:%f Y:%f Z:%f"),&fX,&fY,&fZ);
	}
	if(	PROFILE->m_CpRect[1].Side[0].fX - (fX / theApp.m_fUnits)) {
		PROFILE->m_CpRect[1].Side[1].fX = PROFILE->m_CpRect[1].Side[0].fX = fX / theApp.m_fUnits;
	}
	if(	PROFILE->m_CpRect[1].Side[0].fY - (fY / theApp.m_fUnits)) {
		PROFILE->m_CpRect[1].Side[1].fY = PROFILE->m_CpRect[1].Side[0].fY = fY / theApp.m_fUnits;
	}
	if(	PROFILE->m_CpRect[1].Side[0].fZ - (fZ / theApp.m_fUnits)) {
		PROFILE->m_CpRect[1].Side[1].fZ = PROFILE->m_CpRect[1].Side[0].fZ = fZ / theApp.m_fUnits;
	}

	if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable) {
		if(	PROFILE->m_CpRect[1].Side[0].fR - (fR / theApp.m_fUnits)) {
			PROFILE->m_CpRect[1].Side[1].fR = PROFILE->m_CpRect[1].Side[0].fR = fR / theApp.m_fUnits;
		}
	};
}

void CRectilinearScanPage::OnChangeEditIncrement0() 
{
	
	CString Buff;
	float fTemp;

	m_editFastIncrement.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fFastIncrement - fTemp) {
		PROFILE->m_fFastIncrement = fTemp;
	}
}

void CRectilinearScanPage::OnChangeEditIncrement1() 
{
	CString Buff;
	float fTemp;

	m_editSlowIncrement.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fSlowIncrement - fTemp) {
		PROFILE->m_fSlowIncrement = fTemp;
	}
}

void CRectilinearScanPage::OnChangeEditLength0() 
{
	CString Buff;
	float fTemp;

	m_editFastLength.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fFastScanSize - fTemp) {
		PROFILE->m_fFastScanSize = fTemp;
	}

}

void CRectilinearScanPage::OnChangeEditLength1() 
{
	CString Buff;
	float fTemp;

	m_editSlowLength.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fSlowScanSize - fTemp) {
		PROFILE->m_fSlowScanSize = fTemp;
	}
}

void CRectilinearScanPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
//	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;

	
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CRectilinearScanPage::OnSelchangeComboPrimaryAxis() 
{

	PROFILE->m_nFastAxis = m_comboFastAxis.GetCurSel();
	SetAccessPrivelage();
}

void CRectilinearScanPage::OnSelchangeComboSecondaryAxis() 
{
	int nAxis;

	switch(nAxis = m_comboSlowAxis.GetCurSel()) {
	default: PROFILE->m_nSlowAxis = nAxis;
		break;
	case 6: PROFILE->m_nSlowAxis = 15;
		break;
	};

	SetAccessPrivelage();
}

BOOL CRectilinearScanPage::OnSetActive() 
{
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

BOOL CRectilinearScanPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

void CRectilinearScanPage::OnSelchangeComboMode() 
{
	PROFILE->m_nRectilinearScanMode = m_comboMode.GetCurSel();
	UpdateAllControls();
}

void CRectilinearScanPage::SetAccessPrivelage()
{
	switch(theApp.m_Tank.nScannerDescription) {
	case SINGLE_BRIDGE_TT_ROLLERS:
	case DOUBLE_BRIDGE_TANK:
	case SINGLE_BRIDGE_TANK_WITH_R:
	case SINGLE_BRIDGE_TANK:
		m_buttonRightToLeft.EnableWindow(FALSE);
		m_comboScanSide.EnableWindow(FALSE);
		break;
	default:
		m_buttonRightToLeft.EnableWindow(TRUE);
		m_comboScanSide.EnableWindow(TRUE);
		break;
	}

	switch(PROFILE->m_nRectilinearScanMode) {
	case 0: 
		m_spinSlowLength.EnableWindow(FALSE);
		m_spinFastLength.EnableWindow(FALSE);
		m_spinSlowIncrement.EnableWindow(TRUE);
		m_spinFastIncrement.EnableWindow(TRUE);
		m_editFinishPt.EnableWindow(TRUE);
		m_editStartPt.EnableWindow(TRUE);
		m_editSlowLength.EnableWindow(FALSE);
		m_editFastLength.EnableWindow(FALSE);
		m_editSlowIncrement.EnableWindow(TRUE);
		m_editFastIncrement.EnableWindow(TRUE);
		m_comboSlowAxis.EnableWindow(TRUE);
		m_comboFastAxis.EnableWindow(TRUE);
		break;
	case 1: 
		m_spinSlowLength.EnableWindow(TRUE);
		m_spinFastLength.EnableWindow(TRUE);
		m_spinSlowIncrement.EnableWindow(TRUE);
		m_spinFastIncrement.EnableWindow(TRUE);
		m_editFinishPt.EnableWindow(FALSE);
		m_editStartPt.EnableWindow(FALSE);
		m_editSlowLength.EnableWindow(TRUE);
		m_editFastLength.EnableWindow(TRUE);
		m_editSlowIncrement.EnableWindow(TRUE);
		m_editFastIncrement.EnableWindow(TRUE);
		m_comboSlowAxis.EnableWindow(TRUE);
		m_comboFastAxis.EnableWindow(TRUE);
		if(m_comboFastAxis.GetCurSel()==3)
			m_editFastLength.EnableWindow(FALSE);
		break;
	}

}

void CRectilinearScanPage::OnButtonStore0() 
{
	theApp.m_Motors.GetSurfacePos(&PROFILE->m_CpRect[0]);
	UpdateAllControls();
}

void CRectilinearScanPage::OnButtonStore1() 
{
	

	theApp.m_Motors.GetSurfacePos(&PROFILE->m_CpRect[1]);
	UpdateAllControls();
}

void CRectilinearScanPage::OnButtonGoto0() 
{
	int nMask[3] = { 1,2,3 };
	theApp.m_Motors.IntelligentGotoSurfaceCoord(&PROFILE->m_CpRect[0], nMask[PROFILE->m_nScanSide], theApp.m_LastSettings.nDangerPlaneMoveMask);
}

void CRectilinearScanPage::OnButtonGoto1() 
{
	int nMask[3] = { 1,2,3 };
	theApp.m_Motors.IntelligentGotoSurfaceCoord(&PROFILE->m_CpRect[1], nMask[PROFILE->m_nScanSide], theApp.m_LastSettings.nDangerPlaneMoveMask);
}

void CRectilinearScanPage::OnButtonStart() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString Buff,Error;
	Error.LoadString(IDS_Error);

	if(pFrame->IsScanImageStillOpen() == FALSE) {
		if (PROFILE->m_nFastAxis == PROFILE->m_nSlowAxis) {
			MessageBox(_T("It is not acceptable to set both primary\r\nand secondary scan axes the same"), Error, MB_ICONERROR);
			return;
		}
		if (PROFILE->m_CpRect[0].Side0.fPos[PROFILE->m_nFastAxis] == PROFILE->m_CpRect[1].Side0.fPos[PROFILE->m_nFastAxis]) {
			MessageBox(_T("The primary scan length is zero\r\nthis is unaccetable"), Error, MB_ICONERROR);
			return;
		}
		if (PROFILE->m_CpRect[0].Side0.fPos[PROFILE->m_nSlowAxis] == PROFILE->m_CpRect[1].Side0.fPos[PROFILE->m_nSlowAxis]) {
			MessageBox(_T("The secondary scan length is zero\r\nthis is unaccetable"), Error, MB_ICONERROR);
			return;
		}


		switch(PROFILE->m_nRectilinearScanMode) {
		case 0:
			PROFILE->GenerateRectilinearScanFromCoordinates(PROFILE->m_nScanSide);
			theApp.m_Thread.m_bMoveToStartAtBeginningOfScan=TRUE;
			break;
		case 1:
			theApp.m_Motors.GetSurfacePos(&PROFILE->m_CpRect[0]);
			PROFILE->GenerateRectilinearScanFromSizes(PROFILE->m_nScanSide);
			theApp.m_Thread.m_bMoveToStartAtBeginningOfScan=FALSE;
			break;
		}
		if(PROFILE->m_nScanLineL==0){
			Buff.LoadString(IDS_Scan_Size_Zero);
			MessageBox(Buff,Error,MB_ICONERROR);
			return;
		}

		PROFILE->setRUsed(false);
		PROFILE->m_nCollectAxis = PROFILE->m_nFastAxis;
		PROFILE->m_nScanStartLine = 0;
		PROFILE->m_nWayPointNumber = 2;
		
		if(PROFILE->m_nFastAxis == theApp.m_Tank.nRLeft) {
			PROFILE->m_nScanFinishLine = PROFILE->m_ScanLine[0].m_nCoordL-1;
		}else{
			PROFILE->m_nScanFinishLine = PROFILE->m_nScanLineL-1;
		}

		PROFILE->m_nLineIncrement = 1;
		theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide;

		pFrame->SendMessage(UI_UPDATE_PROFILE_DLG, NULL, NULL);

		if(theApp.m_Data.IsNameRequired() == TRUE) {
			Buff.LoadString(IDS_Volume_scan_Requested);
			MessageBox(Buff,Error,MB_ICONERROR);
			return;
		}
		theApp.m_nScanWhat = 0;

	}

	switch (theApp.m_Tank.nScannerDescription) {
	default:
		pFrame->StartScan(CONTOUR_SCAN);
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
		if (theApp.m_LastSettings.nDesiredDevice == PHASED_ARRAY) {
			pFrame->StartScan(ROBOT_ROLLER_PROBE_SCAN);
		}
		else {
			pFrame->StartScan(ROBOT_CONTOUR_SCAN);
		}
		break;
	}
}

void CRectilinearScanPage::OnButtonAbort() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);

	theApp.m_Thread.m_bAbortFlag = TRUE;
	
}

void CRectilinearScanPage::OnSelchangeComboScanSide() 
{
	

	PROFILE->m_nScanSide = m_comboScanSide.GetCurSel();
	
}

void CRectilinearScanPage::OnChangeEditSpeed() 
{
	CString Buff;
	float fTemp = PROFILE->m_fScanSpeed;

	m_editScanSpeed.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp/=theApp.m_fUnits;
	if(fTemp<1.0f) fTemp = 1.0f;
	if(fTemp>1000.0f) fTemp = 1020.0f;
	if(PROFILE->m_fScanSpeed - fTemp) {
		PROFILE->m_fScanSpeed = fTemp;
	}
}

void CRectilinearScanPage::OnDeltaposSpinSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta * MinMax(&theApp.m_LastSettings.fDeltaScanSpeed, 1, 100));
	if (PROFILE->m_fScanSpeed < 1.0f) PROFILE->m_fScanSpeed = 1.0f;
	if (PROFILE->m_fScanSpeed > 1000.0f) PROFILE->m_fScanSpeed = 1000.0f;
	UpdateAllControls();
	
	*pResult = 0;
}

void CRectilinearScanPage::OnButtonBrowse() 
{
	int nPos;

	CFileDialog File(TRUE,_T(".dat"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Data (*.dat)|*.dat|All Files (*.*)|*.*||"),NULL);
	File.m_ofn.lpstrInitialDir = theApp.m_StorePath[0];
	if(File.DoModal()==IDOK) {
		m_editFilePath.SetWindowText(File.GetFileTitle());
		theApp.m_Data.m_FilePath.Format(_T("%s"),File.GetPathName());
		theApp.m_Data.m_FileName.Format(_T("%s"),File.GetFileTitle());

		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = theApp.m_Data.m_FilePath.ReverseFind('\\')) >0) {
				theApp.m_StorePath[0] = theApp.m_Data.m_FilePath.Left(nPos);
			}
		}
	};	
	
}

void CRectilinearScanPage::OnChangeEditFilepath() 
{
	

	m_editFilePath.GetWindowText(theApp.m_Data.m_FileName);
}

void CRectilinearScanPage::OnButtonMoveLeftToRight() 
{
	
	CCoord CpHead,CpSurface;

	if(theApp.m_LastSettings.nDisplayAreYouSure!=0)
		if(MessageBox(_T("Are You Sure\nYou want to move the Left to the Right"),_T("Warning"),MB_YESNO) != IDYES) return;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.GetHeadPos(&CpHead);
	if(theApp.m_nSide0Orientation==0) {
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
		theApp.m_Kinematics.CalculateOppositeCoord(PORTSIDE,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
	} else {
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
	}
	theApp.m_Thread.m_CpMoveTo = CpHead;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	
}

void CRectilinearScanPage::OnButtonMoveRightToLeft() 
{
	
	CCoord CpHead,CpSurface;

	if(theApp.m_LastSettings.nDisplayAreYouSure!=0)
		if(MessageBox(_T("Are You Sure\nYou want to move the Left to the Right"),_T("Warning"),MB_YESNO) != IDYES) return;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.GetHeadPos(&CpHead);
	if(theApp.m_nSide0Orientation!=0) {
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
		theApp.m_Kinematics.CalculateOppositeCoord(PORTSIDE,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
	} else {
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
	}
	theApp.m_Thread.m_CpMoveTo = CpHead;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	
}

void CRectilinearScanPage::OnChangeEditResumeLine() 
{
	CString	Buff;

	m_editResumeLine.GetWindowText(Buff);
	PROFILE->m_nScanResumeLine = _ttoi(Buff) -1;
	if(PROFILE->m_nScanResumeLine<PROFILE->m_nScanStartLine) {
		PROFILE->m_nScanResumeLine = PROFILE->m_nScanStartLine;
	}
	if(PROFILE->m_nScanResumeLine>=PROFILE->m_nScanFinishLine) {
		PROFILE->m_nScanResumeLine = PROFILE->m_nScanFinishLine;
	}
}

void CRectilinearScanPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;

	if(m_nOldScanLine-theApp.m_Thread.m_nScanLine) {
		Buff.Format(_T("%d of %d"),(m_nOldScanLine=theApp.m_Thread.m_nScanLine) + 1,PROFILE->m_nScanFinishLine);
		m_editCurrentLine.SetWindowText(Buff);
	}

	CPropertyPage::OnTimer(nIDEvent);
}

void CRectilinearScanPage::OnDeltaposSpinIncrement0(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	float fStep = 1.0f / powf(10.0f, (float)theApp.m_LastSettings.nStepSizePrecision);

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fFastIncrement += ((float)pNMUpDown->iDelta * fStep);
	} else {
		fStep *= 25.4f;
		PROFILE->m_fFastIncrement += ((float)pNMUpDown->iDelta * fStep);
	}
	UpdateAllControls();
	
	*pResult = 0;
}

void CRectilinearScanPage::OnDeltaposSpinIncrement1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	float fStep = 1.0f / powf(10.0f, (float)theApp.m_LastSettings.nStepSizePrecision);
	
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * fStep);
	} else {
		fStep *= 25.4f;
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * fStep);
	}
	UpdateAllControls();
	
	*pResult = 0;
}

void CRectilinearScanPage::OnDeltaposSpinLength0(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fFastScanSize += ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		PROFILE->m_fFastScanSize += ((float)pNMUpDown->iDelta * 2.54f);
	}
	UpdateAllControls();
	
	*pResult = 0;
}

void CRectilinearScanPage::OnDeltaposSpinLength1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fSlowScanSize += ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		PROFILE->m_fSlowScanSize += ((float)pNMUpDown->iDelta * 2.54f);
	}
	UpdateAllControls();
	
	*pResult = 0;
}


void CRectilinearScanPage::OnSelchangeComboBiUni() 
{
	PROFILE->m_nBiUniDirectionalScan = m_comboBiUniDirectional.GetCurSel();
}
