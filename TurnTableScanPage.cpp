// TurnTableScanPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "TurnTableScanPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTurnTableScanPage property page

IMPLEMENT_DYNCREATE(CTurnTableScanPage, CPropertyPage)

CTurnTableScanPage::CTurnTableScanPage() : CPropertyPage(CTurnTableScanPage::IDD)
{
	//{{AFX_DATA_INIT(CTurnTableScanPage)
	//}}AFX_DATA_INIT
	
}

CTurnTableScanPage::~CTurnTableScanPage()
{
}

void CTurnTableScanPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTurnTableScanPage)
	DDX_Control(pDX, IDC_EDIT_MAX_SPEED, m_editMaxSpeed);
	DDX_Control(pDX, IDC_COMBO_SLOW_INDEX, m_comboLineIndex);
	DDX_Control(pDX, IDC_CHECK_USE_ECCENTRIC, m_buttonEccentricCompensation);
	DDX_Control(pDX, IDC_EDIT_FAST_INDEX_EQUIVELANCE, m_editFastIndexEquivelance);
	DDX_Control(pDX, IDC_SPIN_FAST_INDEX, m_spinFastIncrement);
	DDX_Control(pDX, IDC_EDIT_FAST_INDEX, m_editFastIncrement);
	DDX_Control(pDX, IDC_EDIT_POSITION, m_editRPos);
	DDX_Control(pDX, IDC_SPIN_SCAN_SPEED, m_spinScanSpeed);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_editFilePath);
	DDX_Control(pDX, IDC_COMBO_SCAN_TYPE, m_comboScanType);
	DDX_Control(pDX, IDC_EDIT_RESUME_LINE, m_editResumeLine);
	DDX_Control(pDX, IDC_EDIT_START_LINE, m_editStartLine);
	DDX_Control(pDX, IDC_EDIT_SCAN_SPEED, m_editScanSpeed);
	DDX_Control(pDX, IDC_EDIT_SCAN_LINE, m_editScanLine);
	DDX_Control(pDX, IDC_EDIT_FINISH_LINE, m_editFinishLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTurnTableScanPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTurnTableScanPage)
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_START_LINE, OnChangeEditStartLine)
	ON_EN_CHANGE(IDC_EDIT_FINISH_LINE, OnChangeEditFinishLine)
	ON_EN_CHANGE(IDC_EDIT_INDEX, OnChangeEditIndex)
	ON_EN_CHANGE(IDC_EDIT_SCAN_SPEED, OnChangeEditScanSpeed)
	ON_EN_CHANGE(IDC_EDIT_RESUME_LINE, OnChangeEditResumeLine)
	ON_EN_CHANGE(IDC_EDIT_FILEPATH, OnChangeEditFilepath)
	ON_CBN_SELCHANGE(IDC_COMBO_SCAN_TYPE, OnSelchangeComboScanType)
	ON_BN_CLICKED(IDC_BUTTON_START_LINE, OnButtonStartLine)
	ON_BN_CLICKED(IDC_BUTTON_FINISH_LINE, OnButtonFinishLine)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_ABORT, OnButtonAbort)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_FAST_INDEX, OnChangeEditFastIndex)
	ON_BN_CLICKED(IDC_CHECK_USE_ECCENTRIC, OnCheckUseEccentric)
	ON_CBN_SELCHANGE(IDC_COMBO_SLOW_INDEX, OnSelchangeComboSlowIndex)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCAN_SPEED, OnDeltaposSpinScanSpeed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTurnTableScanPage message handlers

BOOL CTurnTableScanPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CString	Buff;
	UDACCEL	Accel;
	Accel.nInc=10;
	Accel.nSec=1;

	switch(theApp.m_nSide0Orientation) {
	case 0:
		Buff.LoadString(IDS_Left); m_comboScanType.AddString(Buff);
		Buff.LoadString(IDS_Right); m_comboScanType.AddString(Buff);
		Buff.LoadString(IDS_Both); m_comboScanType.AddString(Buff);
		break;
	case 1:
		Buff.LoadString(IDS_Right); m_comboScanType.AddString(Buff);
		Buff.LoadString(IDS_Left); m_comboScanType.AddString(Buff);
		Buff.LoadString(IDS_Both); m_comboScanType.AddString(Buff);
		break;
	}

	m_spinFastIncrement.SetRange(0,10000);
	m_spinScanSpeed.SetRange(1,1000);
	
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CTurnTableScanPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CTurnTableScanPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	CString Micro = L"\x0b5";

	m_comboScanType.SetCurSel(PROFILE->m_nScanSide);

	m_editFilePath.SetWindowText(theApp.m_Data.m_FileName);

	Buff.Format(_T("%d"),PROFILE->m_nScanStartLine+1);
	m_editStartLine.SetWindowText(Buff);
	Buff.Format(_T("%d"),PROFILE->m_nScanFinishLine+1);
	m_editFinishLine.SetWindowText(Buff);
	Buff.Format(_T("%d"),PROFILE->m_nScanResumeLine+1);
	m_editResumeLine.SetWindowText(Buff);

	Buff.Format(_T("%.02f%s"),PROFILE->m_fFastIncrement,DEGREES);
	m_editFastIncrement.SetWindowText(Buff);
	m_spinFastIncrement.SetPos((int)(PROFILE->m_fFastIncrement*100.0f));

	if(PROFILE->m_nLineIncrement<=0) PROFILE->m_nLineIncrement=1;
	Buff.Format(_T("%.01f mm"),(float)PROFILE->m_nLineIncrement * PROFILE->m_fSlowIncrement);

	m_comboLineIndex.ResetContent();
	if(PROFILE->m_nLineIncrement<=0) PROFILE->m_nLineIncrement=1;
	for(int ii=1;ii<10;ii++) {
		Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,(float)ii * PROFILE->m_fSlowIncrement * theApp.m_fUnits,theApp.m_Units);
		m_comboLineIndex.AddString(Buff);
	}
	m_comboLineIndex.SetCurSel(PROFILE->m_nLineIncrement - 1);

	Buff.Format(_T("%.1f %s/s"),PROFILE->m_fScanSpeed,DEGREES);
	m_editScanSpeed.SetWindowText(Buff);


	UpdateFastIndexEquivelance();

	m_buttonEccentricCompensation.SetCheck(theApp.m_LastSettings.nUseEccentricCompensation);

	SetAccessPrivelage();


}


void CTurnTableScanPage::SetAccessPrivelage()
{
	bool bEnableFlag;
	CButton *pButton;

	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDD_TURNTABLE_SCAN_PAGE] ? bEnableFlag = TRUE : bEnableFlag = FALSE;

	m_editMaxSpeed.EnableWindow(bEnableFlag);
	m_comboLineIndex.EnableWindow(bEnableFlag);
	m_buttonEccentricCompensation.EnableWindow(bEnableFlag);
	m_editFastIndexEquivelance.EnableWindow(bEnableFlag);
	m_spinFastIncrement.EnableWindow(bEnableFlag);
	m_editFastIncrement.EnableWindow(bEnableFlag);
//	m_editRPos.EnableWindow(bEnableFlag);
	m_spinScanSpeed.EnableWindow(bEnableFlag);
	m_editFilePath.EnableWindow(bEnableFlag);
	m_comboScanType.EnableWindow(bEnableFlag);
	m_editResumeLine.EnableWindow(bEnableFlag);
	m_editStartLine.EnableWindow(bEnableFlag);
	m_editScanSpeed.EnableWindow(bEnableFlag);
//	m_editScanLine.EnableWindow(bEnableFlag);
	m_editFinishLine.EnableWindow(bEnableFlag);

	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_START_LINE)) pButton->EnableWindow(bEnableFlag);
	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_FINISH_LINE)) pButton->EnableWindow(bEnableFlag);
	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_START)) pButton->EnableWindow(bEnableFlag);
	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_ABORT)) pButton->EnableWindow(bEnableFlag);
	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_BROWSE)) pButton->EnableWindow(bEnableFlag);
	if(pButton = (CButton*)GetDlgItem(IDC_CHECK_USE_ECCENTRIC)) pButton->EnableWindow(bEnableFlag);

}

void CTurnTableScanPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
//	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int nTemp;
	float fTemp;
	CString Buff;
	CString Micro = L"\x0b5";

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_INDEX:
		nTemp = PROFILE->m_nLineIncrement;
		PROFILE->m_nLineIncrement = pSpin->GetPos()&0xffff;
		if(nTemp - PROFILE->m_nLineIncrement) {
			UpdateAllControls();
		}
		break;
	case IDC_SPIN_FAST_INDEX:
		fTemp = PROFILE->m_fFastIncrement;
		PROFILE->m_fFastIncrement = (float)(pSpin->GetPos()&0xffff) / 100.0f;
		if(fTemp - PROFILE->m_fFastIncrement) {
			Buff.Format(_T("%.02f%s"),PROFILE->m_fFastIncrement,DEGREES);
			m_editFastIncrement.SetWindowText(Buff);
			UpdateFastIndexEquivelance();
		}
		break;
	}


	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTurnTableScanPage::OnChangeEditStartLine()
{
	CString	Buff;
	int nTemp = PROFILE->m_nScanStartLine;
	int nMaxLine = 0;

	if((PROFILE->m_nScanLineL>=1) && (PROFILE->m_ScanLine!=NULL)) {
		nMaxLine = PROFILE->m_ScanLine[0].m_nCoordL;
	}

	m_editStartLine.GetWindowText(Buff);
	PROFILE->m_nScanStartLine = _ttoi(Buff) -1;
	if(PROFILE->m_nScanStartLine<0) {
		PROFILE->m_nScanStartLine = 0;
	}
	if(PROFILE->m_nScanStartLine>=nMaxLine) {
		PROFILE->m_nScanStartLine = nMaxLine-1;
	}
	if(nTemp - PROFILE->m_nScanStartLine) {
//		UpdateInformation();
	}
}

void CTurnTableScanPage::OnChangeEditFinishLine()
{
	CString	Buff;
	int nTemp = PROFILE->m_nScanFinishLine;
	int	nMaxLine = 0;

	if((PROFILE->m_nScanLineL>=1) && (PROFILE->m_ScanLine!=NULL)) {
		nMaxLine = PROFILE->m_ScanLine[0].m_nCoordL;
	}


	m_editFinishLine.GetWindowText(Buff);
	PROFILE->m_nScanFinishLine = _ttoi(Buff) -1;
	if(PROFILE->m_nScanFinishLine<0) {
		PROFILE->m_nScanFinishLine = 0;
	}
	if(PROFILE->m_nScanFinishLine>=nMaxLine) {
		PROFILE->m_nScanFinishLine = nMaxLine;
	}
	if(nTemp - PROFILE->m_nScanFinishLine) {
//		UpdateInformation();
	}
}

void CTurnTableScanPage::OnChangeEditIndex()
{
	CString Buff;
	int nTemp = PROFILE->m_nLineIncrement;
	float fTemp;

	m_editIndex.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	nTemp = (int)(fTemp / PROFILE->m_fSlowIncrement);
	if(nTemp - PROFILE->m_nLineIncrement) {
		m_spinIndex.SetPos(PROFILE->m_nLineIncrement = nTemp);
//		UpdateInformation();
	}
}


void CTurnTableScanPage::OnChangeEditScanSpeed()
{
	CString Buff;
	float fTemp = PROFILE->m_fScanSpeed;

	m_editScanSpeed.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	if(fTemp<1.0f) fTemp = 1.0f;
	if(fTemp>1000.0f) fTemp = 1000.0f;
	if(PROFILE->m_fScanSpeed - fTemp) {
		PROFILE->m_fScanSpeed = fTemp;
//		UpdateInformation();
	}
}

void CTurnTableScanPage::OnChangeEditResumeLine()
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

void CTurnTableScanPage::OnChangeEditFilepath()
{
	m_editFilePath.GetWindowText(theApp.m_Data.m_FileName);
	theApp.m_Data.m_FilePath = theApp.m_StorePath[0] + L"\\" + theApp.m_Data.m_FileName + L".dat";
}


void CTurnTableScanPage::OnSelchangeComboScanType()
{
	PROFILE->m_nScanSide = m_comboScanType.GetCurSel();
}

BOOL CTurnTableScanPage::OnSetActive()
{
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	m_fMaxSpeed = 0.0f;
	SetTimer(1, 300, NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CTurnTableScanPage::OnKillActive()
{
	KillTimer(1);

	return CPropertyPage::OnKillActive();
}



void CTurnTableScanPage::OnButtonStartLine()
{
	CString	Buff;

	PROFILE->m_nScanStartLine = 0;
	Buff.Format(_T("%d"),PROFILE->m_nScanStartLine+1);
	m_editStartLine.SetWindowText(Buff);
//	UpdateInformation();
}

void CTurnTableScanPage::OnButtonFinishLine()
{
	CString	Buff;

	if((PROFILE->m_nScanLineL>=1) && (PROFILE->m_ScanLine!=NULL)) {
		PROFILE->m_nScanFinishLine = PROFILE->m_ScanLine[0].m_nCoordL-1;
	}

	Buff.Format(_T("%d"),PROFILE->m_nScanFinishLine+1);
	m_editFinishLine.SetWindowText(Buff);
//	UpdateInformation();
}


void CTurnTableScanPage::OnButtonStart()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);
	CString Buff,Error,Buff1;
	int nOldFastAxis;

	if(theApp.m_Data.IsNameRequired() == TRUE) {
		Buff.LoadString(IDS_Volume_scan_Requested);
		Error.LoadString(IDS_Error);
		MessageBox(Buff,Error,MB_ICONERROR);
		return;
	}
	if((theApp.m_Data.m_FileName.IsEmpty()) && (theApp.m_LastSettings.nSaveScanEveryLine!=0)) {
		Buff.LoadString(IDS_Save_every_line_is_selected);
		Buff1.LoadString(IDS_please_enter_file_name);
		Error.LoadString(IDS_Error);
		MessageBox(Buff + "\r\n" + Buff1,Error,MB_ICONERROR);
		return;
	}

	nOldFastAxis = PROFILE->m_nFastAxis;
	theApp.m_nScanWhat = 0;
	PROFILE->m_nCollectAxis = 5; //Collect axis uses 1 incremental whereas Tank uses 0 hence +1
	PROFILE->m_nScanSide = m_comboScanType.GetCurSel();

	pFrame->StartScan(TURNTABLE_SCAN);

}

void CTurnTableScanPage::OnButtonAbort()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);

	theApp.m_Thread.m_bAbortFlag = TRUE;

}

void CTurnTableScanPage::OnButtonBrowse()
{
	CFileDialog File(TRUE,_T(".dat"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Data (*.dat)|*.dat|All Files (*.*)|*.*||"),NULL);
	File.m_ofn.lpstrInitialDir = theApp.m_StorePath[0];
	if(File.DoModal()==IDOK) {
		m_editFilePath.SetWindowText(File.GetFileTitle());
		theApp.m_Data.m_FilePath.Format(_T("%s"),File.GetPathName());
		theApp.m_Data.m_FileName.Format(_T("%s"),File.GetFileTitle());
	};

}

void CTurnTableScanPage::OnTimer(UINT nIDEvent)
{
	CString Buff;
	CCoord CpSurface,Cp;
	float	fMaxSpeed;
	static float fScanSpeed = -0.1f;
	CString Micro = L"\x0b5";


	if(m_nOldScanLine-theApp.m_Thread.m_nScanLine) {
		Buff.Format(_T("%d"),(m_nOldScanLine=theApp.m_Thread.m_nScanLine) + 1);
		m_editScanLine.SetWindowText(Buff);
	}

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	switch(theApp.m_nUseTurntableOrRollers) {
	default:
		if(m_CpSurface.Side0.fR - CpSurface.Side0.fR) {
			Buff.Format(_T("%.*f%s"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPrecision,CpSurface.Side0.fR,DEGREES);
			m_editRPos.SetWindowText(Buff);
		}
		break;
	case 1:
		if(m_CpSurface.Side1.fR - CpSurface.Side1.fR) {
			Buff.Format(_T("%.*f%s"),theApp.m_Axes[theApp.m_Tank.nRRight].nPrecision,CpSurface.Side1.fR,DEGREES);
			m_editRPos.SetWindowText(Buff);
		}
		break;
	}

	m_CpSurface = CpSurface;

	if(theApp.m_nRealPRFPerTS > 0) {
		fMaxSpeed = (float)theApp.m_nRealPRFPerTS * PROFILE->m_fFastIncrement;
	} else {
		fMaxSpeed = (float)theApp.m_UtUser.m_Global.nPrf * PROFILE->m_fFastIncrement;
	}
	if((m_fMaxSpeed != fMaxSpeed) || (fScanSpeed - PROFILE->m_fScanSpeed)) {
		fScanSpeed = PROFILE->m_fScanSpeed;
		Buff.Format(_T("%.02f%s/s"),m_fMaxSpeed = fMaxSpeed,DEGREES);
		if(m_fMaxSpeed < PROFILE->m_fScanSpeed) {
			m_editMaxSpeed.SetTextColour(RGB(255,0,0));
		} else {
			m_editMaxSpeed.SetTextColour(RGB(0,0,0));
		}
		m_editMaxSpeed.SetWindowText(Buff);
	}


	CPropertyPage::OnTimer(nIDEvent);
}

void CTurnTableScanPage::OnChangeEditFastIndex()
{
	CString Buff;
	float fTemp = PROFILE->m_fFastIncrement;

	m_editFastIncrement.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_fFastIncrement);
	if(fTemp - PROFILE->m_fFastIncrement) {
		m_spinFastIncrement.SetPos((int)(PROFILE->m_fFastIncrement*100.0f));
		UpdateFastIndexEquivelance();
	}

}

void CTurnTableScanPage::UpdateFastIndexEquivelance()
{
	CString Buff;

	float fEquivelantFastIndex = (PROFILE->m_fComponentDiameter[0] * PIf * PROFILE->m_fFastIncrement) / 360.0f;
	Buff.Format(_T("%.*f%s"),theApp.m_nUnitsPrecision,fEquivelantFastIndex * theApp.m_fUnits, theApp.m_Units);
	m_editFastIndexEquivelance.SetWindowText(Buff);
}

void CTurnTableScanPage::OnCheckUseEccentric()
{
	theApp.m_LastSettings.nUseEccentricCompensation = m_buttonEccentricCompensation.GetCheck() & 1;
}

void CTurnTableScanPage::OnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here

}

void CTurnTableScanPage::OnSelchangeComboSlowIndex()
{
	PROFILE->m_nLineIncrement = m_comboLineIndex.GetCurSel() + 1;
}

void CTurnTableScanPage::OnDeltaposSpinScanSpeed(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta * 0.1f);
	if(PROFILE->m_fScanSpeed<0.0f) PROFILE->m_fScanSpeed = 0.0f;
	if(PROFILE->m_fScanSpeed>1080.0f) PROFILE->m_fScanSpeed = 1080.0f;
	UpdateAllControls();

	*pResult = 0;
}


