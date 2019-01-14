// ScopeOptionsPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ScopeOptionsPage.h"
#include "UltrasonicsSheet.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScopeOptionsPage property page

IMPLEMENT_DYNCREATE(CScopeOptionsPage, CPropertyPage)

CScopeOptionsPage::CScopeOptionsPage() : CPropertyPage(CScopeOptionsPage::IDD)
{
	//{{AFX_DATA_INIT(CScopeOptionsPage)
	//}}AFX_DATA_INIT
}

CScopeOptionsPage::~CScopeOptionsPage()
{
}

void CScopeOptionsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScopeOptionsPage)
	DDX_Control(pDX, IDC_COMBO_TOF, m_comboTOF);
	DDX_Control(pDX, IDC_COMBOAVERAGES, m_comboAverages);
	DDX_Control(pDX, IDC_COMBO_DAC_DELAY_UNITS, m_comboDacDelayUnits);
	DDX_Control(pDX, IDC_COMBO_DAC_BLANKING_UNITS, m_comboDacBlankingUnits);
	DDX_Control(pDX, IDC_CHECK_TIMESLOT, m_checkTimeSlot);
	DDX_Control(pDX, IDC_COMBO_NUMBER_TIMESLOTS, m_comboNumberTimeSlots);
	DDX_Control(pDX, IDC_STATIC_GRATICULE_COLOR, m_staticGraticuleColor);
	DDX_Control(pDX, IDC_STATIC_BK_COLOR, m_staticBKColor);
	DDX_Control(pDX, IDC_STATIC_TRACE_COLOR, m_staticTraceColor);
	DDX_Control(pDX, IDC_CHECK_METALPATH, m_checkMetalPath);
	DDX_Control(pDX, IDC_STATIC_TEXT_COLOR, m_staticTextColor);
	DDX_Control(pDX, IDC_SPIN_TEXT_SIZE, m_spinTextSize);
	DDX_Control(pDX, IDC_EDIT_TEXT_SIZE, m_editTextSize);
	DDX_Control(pDX, IDC_CHECK_GATE_AMP, m_checkGateAmp);
	DDX_Control(pDX, IDC_CHECK_WATERPATH, m_checkWaterPath);
	DDX_Control(pDX, IDC_COMBO_TRACE_WIDTH_UNITS, m_comboTraceWidthUnits);
	DDX_Control(pDX, IDC_COMBO_TRACE_DELAY_UNITS, m_comboTraceDelayUnits);
	DDX_Control(pDX, IDC_CHECK_GRATICULE_OFF_ON, m_checkGraticuleOffOn);
	DDX_Control(pDX, IDC_COMBO_GATE_WIDTH_UNITS, m_comboGateWidthUnits);
	DDX_Control(pDX, IDC_COMBO_GATE_DELAY_UNITS, m_comboGateDelayUnits);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScopeOptionsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CScopeOptionsPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_GRATICULE_OFF_ON, OnCheckGraticuleOffOn)
	ON_CBN_SELCHANGE(IDC_COMBO_GATE_DELAY_UNITS, OnSelchangeComboGateDelayUnits)
	ON_CBN_SELCHANGE(IDC_COMBO_GATE_WIDTH_UNITS, OnSelchangeComboGateWidthUnits)
	ON_CBN_SELCHANGE(IDC_COMBO_TRACE_WIDTH_UNITS, OnSelchangeComboTraceWidthUnits)
	ON_CBN_SELCHANGE(IDC_COMBO_TRACE_DELAY_UNITS, OnSelchangeComboTraceDelayUnits)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CHECK_WATERPATH, OnCheckWaterpath)
	ON_BN_CLICKED(IDC_CHECK_GATE_AMP, OnCheckGateAmp)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_METALPATH, OnCheckMetalpath)
	ON_EN_CHANGE(IDC_EDIT_TEXT_SIZE, OnChangeEditTextSize)
	ON_CBN_SELCHANGE(IDC_COMBO_NUMBER_TIMESLOTS, OnSelchangeComboNumberTimeslots)
	ON_BN_CLICKED(IDC_CHECK_TIMESLOT, OnCheckTimeslot)
	ON_CBN_SELCHANGE(IDC_COMBO_DAC_DELAY_UNITS, OnSelchangeComboDacDelayUnits)
	ON_CBN_SELCHANGE(IDC_COMBO_DAC_BLANKING_UNITS, OnSelchangeComboDacBlankingUnits)
	ON_WM_HELPINFO()
	ON_CBN_SELCHANGE(IDC_COMBOAVERAGES, OnSelchangeComboaverages)
	ON_CBN_SELCHANGE(IDC_COMBO_TOF, OnSelchangeComboTof)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScopeOptionsPage message handlers

BOOL CScopeOptionsPage::OnInitDialog() 
{
	CString Buff;
	CPropertyPage::OnInitDialog();
	



	Buff.LoadString(IDS_Off);	m_comboTOF.AddString(Buff);
	m_comboTOF.AddString(_T("\x0b5s"));
	m_comboTOF.AddString(_T("mm"));
	m_comboTOF.AddString(_T("\x022"));

	CString Micro = L"\x0b5s";

	m_comboGateDelayUnits.AddString(Micro);
	m_comboGateDelayUnits.AddString(_T("mm"));
	m_comboGateDelayUnits.AddString(_T("\x022"));
	m_comboGateWidthUnits.AddString(Micro);
	m_comboGateWidthUnits.AddString(_T("mm"));
	m_comboGateWidthUnits.AddString(_T("\x022"));


	m_comboTraceDelayUnits.AddString(L"ns");
	m_comboTraceDelayUnits.AddString(L"\x0b5s");
	Buff.LoadString(IDS_mm_Water); m_comboTraceDelayUnits.AddString(Buff);
	Buff.LoadString(IDS_mm_Material); m_comboTraceDelayUnits.AddString(Buff);
	Buff.LoadString(IDS_inches_Water); m_comboTraceDelayUnits.AddString(Buff);
	Buff.LoadString(IDS_inches_Material); m_comboTraceDelayUnits.AddString(Buff);

	m_comboTraceWidthUnits.AddString(L"ns");
	m_comboTraceWidthUnits.AddString(L"\x0b5s");
	Buff.LoadString(IDS_mm_Water); m_comboTraceWidthUnits.AddString(Buff);
	Buff.LoadString(IDS_mm_Material); m_comboTraceWidthUnits.AddString(Buff);
	Buff.LoadString(IDS_inches_Water); m_comboTraceWidthUnits.AddString(Buff);
	Buff.LoadString(IDS_inches_Material); m_comboTraceWidthUnits.AddString(Buff);


	Buff.Format(_T("%d"),theApp.m_Scope.m_nTextSize);
	m_editTextSize.SetWindowText(Buff);
	m_spinTextSize.SetRange(1,30);
	m_spinTextSize.SetPos(theApp.m_Scope.m_nTextSize);


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

	m_comboDacDelayUnits.AddString(Micro);
	Buff.LoadString(IDS_mm_Water); m_comboDacDelayUnits.AddString(Buff);
	Buff.LoadString(IDS_mm_Material); m_comboDacDelayUnits.AddString(Buff);

	m_comboDacBlankingUnits.AddString(Micro);
	Buff.LoadString(IDS_mm_Water); m_comboDacBlankingUnits.AddString(Buff);
	Buff.LoadString(IDS_mm_Material); m_comboDacBlankingUnits.AddString(Buff);

	UpdateAllControls();
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CScopeOptionsPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CScopeOptionsPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	m_comboTOF.SetCurSel(theApp.m_Scope.m_nTOFText[1]);

	m_checkTimeSlot.SetCheck(theApp.m_Scope.m_bTimeSlotName);

	m_comboGateDelayUnits.SetCurSel(theApp.m_Scope.m_nGateDelayUnits);
	m_comboGateWidthUnits.SetCurSel(theApp.m_Scope.m_nGateWidthUnits);
	m_comboTraceDelayUnits.SetCurSel(theApp.m_Scope.m_nTraceDelayUnits);
	m_comboTraceWidthUnits.SetCurSel(theApp.m_Scope.m_nTraceWidthUnits);
	m_comboNumberTimeSlots.SetCurSel(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots - 1);
	m_comboDacDelayUnits.SetCurSel(theApp.m_LastSettings.nDacDelayUnits);
	m_comboDacBlankingUnits.SetCurSel(theApp.m_LastSettings.nDacBlankingUnits);
	m_comboAverages.SetCurSel(theApp.m_Scope.m_nAverages);

	m_checkGraticuleOffOn.SetCheck( theApp.m_LastSettings.nScopeMask & SCOPE_GRATICULE ? true : false );
	m_checkWaterPath.SetCheck(theApp.m_LastSettings.nScopeMask & SCOPE_RENDER_WATER_PATH ? true : false);
	m_checkMetalPath.SetCheck(theApp.m_LastSettings.nScopeMask & SCOPE_RENDER_MATERIAL_PATH ? true : false);
	m_checkGateAmp.SetCheck(theApp.m_LastSettings.nScopeMask & SCOPE_RENDER_COLLECT_TEXT ? true : false);

}

void CScopeOptionsPage::OnCheckGraticuleOffOn() 
{
	m_checkGraticuleOffOn.GetCheck() == TRUE ? theApp.m_LastSettings.nScopeMask |= SCOPE_GRATICULE : theApp.m_LastSettings.nScopeMask &= ~SCOPE_GRATICULE;
}

void CScopeOptionsPage::OnCheckWaterpath()
{
	m_checkWaterPath.GetCheck() == TRUE ? theApp.m_LastSettings.nScopeMask |= SCOPE_RENDER_WATER_PATH : theApp.m_LastSettings.nScopeMask &= ~SCOPE_RENDER_WATER_PATH;
}

void CScopeOptionsPage::OnCheckGateAmp()
{
	m_checkGateAmp.GetCheck() == TRUE ? theApp.m_LastSettings.nScopeMask |= SCOPE_RENDER_COLLECT_TEXT : theApp.m_LastSettings.nScopeMask &= ~SCOPE_RENDER_COLLECT_TEXT;
}

void CScopeOptionsPage::OnSelchangeComboGateDelayUnits() 
{
	theApp.m_Scope.m_nGateDelayUnits=m_comboGateDelayUnits.GetCurSel();
}

void CScopeOptionsPage::OnSelchangeComboGateWidthUnits() 
{
	theApp.m_Scope.m_nGateWidthUnits=m_comboGateWidthUnits.GetCurSel();
}

void CScopeOptionsPage::OnSelchangeComboTraceWidthUnits() 
{
	theApp.m_Scope.m_nTraceWidthUnits=m_comboTraceWidthUnits.GetCurSel();
}

void CScopeOptionsPage::OnSelchangeComboTraceDelayUnits() 
{
	theApp.m_Scope.m_nTraceDelayUnits=m_comboTraceDelayUnits.GetCurSel();
}




void CScopeOptionsPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString	Buff;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_TEXT_SIZE:
		theApp.m_Scope.m_nTextSize = (pSpin->GetPos()&0xffff);
		Buff.Format(_T("%d"),theApp.m_Scope.m_nTextSize);
		m_editTextSize.SetWindowText(Buff);
		break;
	};

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}



void CScopeOptionsPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;

	m_staticTextColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_Scope.m_rgbTextColor=dlg.GetColor();
			Invalidate(FALSE);
		}
	}

	m_staticTraceColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_Scope.m_rgbTraceColor=dlg.GetColor();
			Invalidate(FALSE);
		}
	}

	m_staticBKColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_Scope.m_rgbBkColor=dlg.GetColor();
			Invalidate(FALSE);
		}
	}

	m_staticGraticuleColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_Scope.m_rgbGraticuleColor=dlg.GetColor();
			Invalidate(FALSE);
		}
	}

	
	CPropertyPage::OnLButtonDown(nFlags, point);
}

void CScopeOptionsPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr;
	

	BlockFill(&dc,&m_staticTextColor,theApp.m_Scope.m_rgbTextColor);
	BlockFill(&dc,&m_staticTraceColor,theApp.m_Scope.m_rgbTraceColor);
	BlockFill(&dc,&m_staticBKColor,theApp.m_Scope.m_rgbBkColor);
	BlockFill(&dc,&m_staticGraticuleColor,theApp.m_Scope.m_rgbGraticuleColor);

	// Do not call CPropertyPage::OnPaint() for painting messages
}

void CScopeOptionsPage::OnCheckMetalpath() 
{
	theApp.m_Scope.m_nTOFText[0]=m_checkMetalPath.GetCheck();
}

void CScopeOptionsPage::OnChangeEditTextSize() 
{
	CString Buff;

	m_editTextSize.GetWindowText(Buff);
	theApp.m_Scope.m_nTextSize=_ttoi(Buff);
	m_spinTextSize.SetPos(theApp.m_Scope.m_nTextSize);
}

void CScopeOptionsPage::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}



BOOL CScopeOptionsPage::OnSetActive() 
{

	UpdateAllControls();
	SetAccessPrivelage();
	return CPropertyPage::OnSetActive();
}



void CScopeOptionsPage::OnSelchangeComboNumberTimeslots() 
{
	CString Buff;
	CUltrasonicsSheet* pSheet = (CUltrasonicsSheet*)GetParent();
	CMainFrame*  pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	theApp.m_UtUser.m_Global.Mux.nMaxTimeslots=m_comboNumberTimeSlots.GetCurSel()+1;

//	m_spinTimeSlot.SetRange(0,theApp.m_UtUser.m_Global.nMaxTimeslots-1);

	pSheet->SendMessage(UPDATE_TIMESLOTS, NULL,NULL);
	theApp.m_UtUser.ResetMaxTimeSlots();
	theApp.m_UtUser.RefreshAllVariables(0);
	if(pFrame->m_pMotionToolsSheet) {
		pFrame->m_pMotionToolsSheet->UpdateAllPages();
	}

}

void CScopeOptionsPage::OnCheckTimeslot() 
{
	theApp.m_Scope.m_bTimeSlotName=m_checkTimeSlot.GetCheck() & 0x01;
}

void CScopeOptionsPage::OnSelchangeComboDacDelayUnits() 
{
	int nTS,ii;

	//convert back to Nano seconds
	for(nTS=0;nTS<8;nTS++) {
		switch(theApp.m_LastSettings.nDacDelayUnits) {
		case 0:
			break;
		case 1:
			for(ii=0;ii<64;ii++) {
				theApp.m_UtUser.m_TS[nTS].Pr30.fDacDelay[0][ii] = theApp.m_UtUser.m_TS[nTS].Pr30.fDacDelay[0][ii] / (float)theApp.m_LastSettings.nWaterVelocity * 2000.0f;		//mm
			}
			break;
		case 2:
			for(ii=0;ii<64;ii++) {
				theApp.m_UtUser.m_TS[nTS].Pr30.fDacDelay[0][ii] = theApp.m_UtUser.m_TS[nTS].Pr30.fDacDelay[0][ii] / (float)theApp.m_LastSettings.nMaterialVelocity * 2000.0f;		//mm
			}
			break;
		}
	}

	theApp.m_LastSettings.nDacDelayUnits = m_comboDacDelayUnits.GetCurSel();

	for(nTS=0;nTS<8;nTS++) {
		switch(theApp.m_LastSettings.nDacDelayUnits) {
		case 0:
			break;
		case 1:
			for(ii=0;ii<64;ii++) {
				theApp.m_UtUser.m_TS[nTS].Pr30.fDacDelay[0][ii] = theApp.m_UtUser.m_TS[nTS].Pr30.fDacDelay[0][ii] * (float)theApp.m_LastSettings.nWaterVelocity / 2000.0f;		//mm
			}
			break;
		case 2:
			for(ii=0;ii<64;ii++) {
				theApp.m_UtUser.m_TS[nTS].Pr30.fDacDelay[0][ii] = theApp.m_UtUser.m_TS[nTS].Pr30.fDacDelay[0][ii] * (float)theApp.m_LastSettings.nMaterialVelocity / 2000.0f;		//mm
			}
			break;
		}
	}
	
}

void CScopeOptionsPage::OnSelchangeComboDacBlankingUnits() 
{
	theApp.m_LastSettings.nDacBlankingUnits = m_comboDacBlankingUnits.GetCurSel();
}

BOOL CScopeOptionsPage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	theApp.m_Help.Open(_T("Scope_OptionsDlg.html"),NULL);
	
	return CPropertyPage::OnHelpInfo(pHelpInfo);
}

void CScopeOptionsPage::OnSelchangeComboaverages() 
{
	theApp.m_Scope.m_nAverages = m_comboAverages.GetCurSel();
}


void CScopeOptionsPage::OnSelchangeComboTof() 
{
	theApp.m_Scope.m_nTOFText[1]=m_comboTOF.GetCurSel();
}

void CScopeOptionsPage::SetAccessPrivelage()
{
	if(GetSafeHwnd() == NULL) return;

	bool bFlag = false;
	if(theApp.m_nLogonLevelMask & theApp.m_cAccess[IDD_SCOPE_DIALOG1]) {
		bFlag = true;
	}
	if((theApp.m_LastSettings.nScanMask & _LOCK_UT) && (theApp.m_pScanData != NULL)) {
		bFlag = false;
	}

	m_comboTOF.EnableWindow(bFlag);
	m_comboAverages.EnableWindow(bFlag);
	m_comboDacDelayUnits.EnableWindow(bFlag);
	m_comboDacBlankingUnits.EnableWindow(bFlag);
	m_checkTimeSlot.EnableWindow(bFlag);
	m_comboNumberTimeSlots.EnableWindow(bFlag);
	m_staticGraticuleColor.EnableWindow(bFlag);
	m_staticBKColor.EnableWindow(bFlag);
	m_staticTraceColor.EnableWindow(bFlag);
	m_checkMetalPath.EnableWindow(bFlag);
	m_staticTextColor.EnableWindow(bFlag);
	m_spinTextSize.EnableWindow(bFlag);
	m_editTextSize.EnableWindow(bFlag);
	m_checkGateAmp.EnableWindow(bFlag);
	m_checkWaterPath.EnableWindow(bFlag);
	m_comboTraceWidthUnits.EnableWindow(bFlag);
	m_comboTraceDelayUnits.EnableWindow(bFlag);
	m_checkGraticuleOffOn.EnableWindow(bFlag);
	m_comboGateWidthUnits.EnableWindow(bFlag);
	m_comboGateDelayUnits.EnableWindow(bFlag);

}
