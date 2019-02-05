#include "stdafx.h"
#include "USLScanner.h"
#include "DiscsShaftsPage.h"
#include "MainFrm.h"

IMPLEMENT_DYNCREATE(CDiscsShaftsPage, CPropertyPage)


CDiscsShaftsPage::CDiscsShaftsPage() : CPropertyPage(CDiscsShaftsPage::IDD)
{
}


CDiscsShaftsPage::~CDiscsShaftsPage()
{
}

BEGIN_MESSAGE_MAP(CDiscsShaftsPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_SPEED, &CDiscsShaftsPage::OnEnChangeEditSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPEED, &CDiscsShaftsPage::OnDeltaposSpinSpeed)
	ON_EN_CHANGE(IDC_EDIT_START_COORD, &CDiscsShaftsPage::OnEnChangeEditStartCoord)
	ON_EN_CHANGE(IDC_EDIT_FINISH_COORD, &CDiscsShaftsPage::OnEnChangeEditFinishCoord)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CDiscsShaftsPage::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_START, &CDiscsShaftsPage::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_ABORT, &CDiscsShaftsPage::OnBnClickedButtonAbort)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_START, &CDiscsShaftsPage::OnBnClickedButtonGotoStart)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_FINISH, &CDiscsShaftsPage::OnBnClickedButtonGotoFinish)
	ON_BN_CLICKED(IDC_BUTTON_STORE_START, &CDiscsShaftsPage::OnBnClickedButtonStoreStart)
	ON_BN_CLICKED(IDC_BUTTON_STORE_FINISH, &CDiscsShaftsPage::OnBnClickedButtonStoreFinish)
	ON_BN_CLICKED(IDC_BUTTON_ZERO_TT, &CDiscsShaftsPage::OnBnClickedButtonZeroTt)
	ON_EN_CHANGE(IDC_EDIT_FAST_INDEX, &CDiscsShaftsPage::OnEnChangeEditFastIndex)
	ON_EN_CHANGE(IDC_EDIT_INCREMENT_1, &CDiscsShaftsPage::OnEnChangeEditIncrement1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FAST_INDEX, &CDiscsShaftsPage::OnDeltaposSpinFastIndex)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_INCREMENT_1, &CDiscsShaftsPage::OnDeltaposSpinIncrement1)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_RESUME_LINE, &CDiscsShaftsPage::OnEnChangeEditResumeLine)
	ON_EN_CHANGE(IDC_EDIT_ADDITIONAL_TIP, &CDiscsShaftsPage::OnEnChangeEditAdditionalTip)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ADDITIONAL_TIP, &CDiscsShaftsPage::OnDeltaposSpinAdditionalTip)
	ON_BN_CLICKED(IDC_CHECK_ADDITIONAL_TIP_ANGLE, &CDiscsShaftsPage::OnBnClickedCheckAdditionalTipAngle)
	ON_MESSAGE(UI_SET_ACCESS_PRIVELAGES, SetAccessPrivelages)
END_MESSAGE_MAP()

void CDiscsShaftsPage::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class

	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_editFilePath);
	DDX_Control(pDX, IDC_EDIT_SPEED, m_editScanSpeed);
	DDX_Control(pDX, IDC_SPIN_SPEED, m_spinScanSpeed);
	DDX_Control(pDX, IDC_EDIT_CURRENT_LINE, m_editCurrentLine);
	DDX_Control(pDX, IDC_EDIT_RESUME_LINE, m_editResumeLine);
	DDX_Control(pDX, IDC_EDIT_START_COORD, m_editStartCoordinate);
	DDX_Control(pDX, IDC_EDIT_FINISH_COORD, m_editFinishCoordinate);
	DDX_Control(pDX, IDC_EDIT_FAST_INDEX, m_editFastIncrement);
	DDX_Control(pDX, IDC_EDIT_INCREMENT_1, m_editSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_FAST_INDEX_EQUIVELANCE, m_editFastEquivelantIncrement);
	DDX_Control(pDX, IDC_SPIN_FAST_INDEX, m_spinFastIncrement);
	DDX_Control(pDX, IDC_SPIN_INCREMENT_1, m_spinSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_ADDITIONAL_TIP, m_editAdditionalAngle);
	DDX_Control(pDX, IDC_SPIN_ADDITIONAL_TIP, m_spinAdditionalAngle);
	DDX_Control(pDX, IDC_CHECK_ADDITIONAL_TIP_ANGLE, m_checkAdditionalAngle);
}

BOOL CDiscsShaftsPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_spinScanSpeed.SetRange(1, 2000);
	m_spinFastIncrement.SetRange(0, 2000);
	m_spinSlowIncrement.SetRange(0, 2000);
	m_spinAdditionalAngle.SetRange(0, 2000);

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDiscsShaftsPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}


void CDiscsShaftsPage::UpdateAllControls()
{
	CString Buff;
	if (GetSafeHwnd() == NULL) return;

	if (theApp.m_nUnitsPrecision < 1) theApp.m_nUnitsPrecision = 1;

	m_editFilePath.SetWindowText(theApp.m_Data.m_FileName);

	Buff.Format(_T("%.*f %s/s"), theApp.m_nUnitsPrecision - 1, PROFILE->m_fScanSpeed*theApp.m_fUnits, theApp.m_Units);
	m_editScanSpeed.SetWindowText(Buff);

	CCoord *pCp = &PROFILE->m_CpRect[0];
	Buff.Format(L"X: %.01f Y: %.01f Z: %.01f I: %.02f J: %.02f K: %.02f", pCp->Side0.fX, pCp->Side0.fY, pCp->Side0.fZ, pCp->Side0.fI, pCp->Side0.fJ, pCp->Side0.fK);
	m_editStartCoordinate.SetWindowTextW(Buff);
	float fRadius0 = D3DXVec3Length(&(pCp->Side0.pt - theApp.m_Tank.vTTCentre));

	pCp = &PROFILE->m_CpRect[1];
	Buff.Format(L"X: %.01f Y: %.01f Z: %.01f I: %.02f J: %.02f K: %.02f", pCp->Side0.fX, pCp->Side0.fY, pCp->Side0.fZ, pCp->Side0.fI, pCp->Side0.fJ, pCp->Side0.fK);
	m_editFinishCoordinate.SetWindowTextW(Buff);
	float fRadius1 = D3DXVec3Length(&(pCp->Side0.pt - theApp.m_Tank.vTTCentre));

	Buff.Format(_T("%.02f%s"), PROFILE->m_fFastIncrement, DEGREES);
	m_editFastIncrement.SetWindowText(Buff);

	Buff.Format(_T("%.02f mm"), PROFILE->m_fSlowIncrement);
	m_editSlowIncrement.SetWindowText(Buff);

	Buff.Format(_T("%d"), PROFILE->m_nScanResumeLine + 1);
	m_editResumeLine.SetWindowText(Buff);

	Buff.Format(L"%.01f%s", PROFILE->m_vecAdditionalAngle[0].x, DEGREES);
	m_editAdditionalAngle.SetWindowTextW(Buff);

	m_checkAdditionalAngle.SetCheck(PROFILE->m_bUseAdditionalAngleDuringScanning[0]);

	float fCircumference;
	if (fRadius0 < fRadius1) {
		fCircumference = 2.0f * PIf * fRadius1;
	}
	else {
		fCircumference = 2.0f * PIf * fRadius0;
	}
	Buff.Format(L"%.02f mm", PROFILE->m_fFastIncrement * fCircumference / 360.0f);
	m_editFastEquivelantIncrement.SetWindowTextW(Buff);

	SetAccessPrivelages();
}

HRESULT CDiscsShaftsPage::SetAccessPrivelages(WPARAM wp, LPARAM lp)
{
	bool bEnableFlag;

	theApp.m_nLogonLevelMask & theApp.m_cAccess[CDiscsShaftsPage::IDD] ? bEnableFlag = TRUE : bEnableFlag = FALSE;
	m_editFilePath.EnableWindow(bEnableFlag);
	m_editScanSpeed.EnableWindow(bEnableFlag);
	m_spinScanSpeed.EnableWindow(bEnableFlag);
	m_editCurrentLine.EnableWindow(bEnableFlag);
	m_editResumeLine.EnableWindow(bEnableFlag);
	m_editStartCoordinate.EnableWindow(bEnableFlag);
	m_editFinishCoordinate.EnableWindow(bEnableFlag);
	m_editFastIncrement.EnableWindow(bEnableFlag);
	m_editSlowIncrement.EnableWindow(bEnableFlag);
	m_editFastEquivelantIncrement.EnableWindow(bEnableFlag);
	m_spinFastIncrement.EnableWindow(bEnableFlag);
	m_spinSlowIncrement.EnableWindow(bEnableFlag);
	m_editAdditionalAngle.EnableWindow(bEnableFlag);
	m_spinAdditionalAngle.EnableWindow(bEnableFlag);
	m_checkAdditionalAngle.EnableWindow(bEnableFlag);

	CButton *pButton;
	if (pButton = (CButton*)GetDlgItem(IDC_BUTTON_BROWSE)) pButton->EnableWindow(bEnableFlag);
	if (pButton = (CButton*)GetDlgItem(IDC_BUTTON_START)) pButton->EnableWindow(bEnableFlag);
	if (pButton = (CButton*)GetDlgItem(IDC_BUTTON_ABORT)) pButton->EnableWindow(bEnableFlag);
	if (pButton = (CButton*)GetDlgItem(IDC_BUTTON_GOTO_START)) pButton->EnableWindow(bEnableFlag);
	if (pButton = (CButton*)GetDlgItem(IDC_BUTTON_GOTO_FINISH)) pButton->EnableWindow(bEnableFlag);
	if (pButton = (CButton*)GetDlgItem(IDC_BUTTON_STORE_START)) pButton->EnableWindow(bEnableFlag);
	if (pButton = (CButton*)GetDlgItem(IDC_BUTTON_STORE_FINISH)) pButton->EnableWindow(bEnableFlag);
	if (pButton = (CButton*)GetDlgItem(IDC_BUTTON_ZERO_TT)) pButton->EnableWindow(bEnableFlag);
	
	return 0;
}


BOOL CDiscsShaftsPage::OnKillActive()
{
	KillTimer(1);
	return CPropertyPage::OnKillActive();
}


BOOL CDiscsShaftsPage::OnSetActive()
{
	UpdateAllControls();

	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	SetTimer(1, 200, NULL);

	return CPropertyPage::OnSetActive();
}




void CDiscsShaftsPage::OnBnClickedButtonBrowse()
{
	int nPos;

	CFileDialog File(TRUE, _T(".dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Data (*.dat)|*.dat|All Files (*.*)|*.*||"), NULL);
	File.m_ofn.lpstrInitialDir = theApp.m_StorePath[0];
	if (File.DoModal() == IDOK) {
		m_editFilePath.SetWindowText(File.GetFileTitle());
		theApp.m_Data.m_FilePath.Format(_T("%s"), File.GetPathName());
		theApp.m_Data.m_FileName.Format(_T("%s"), File.GetFileTitle());

		if (theApp.m_LastSettings.bUpdateDirectoryPath) {
			if ((nPos = theApp.m_Data.m_FilePath.ReverseFind('\\')) > 0) {
				theApp.m_StorePath[0] = theApp.m_Data.m_FilePath.Left(nPos);
			}
		}
	};
}


void CDiscsShaftsPage::OnEnChangeEditSpeed()
{
	CString Buff;
	float fTemp = PROFILE->m_fScanSpeed;

	m_editScanSpeed.GetWindowText(Buff);
	_WTOF(Buff, fTemp);
	fTemp /= theApp.m_fUnits;
	if (fTemp < 1.0f) fTemp = 1.0f;
	if (fTemp > 1000.0f) fTemp = 1020.0f;
	if (PROFILE->m_fScanSpeed - fTemp) {
		PROFILE->m_fScanSpeed = fTemp;
	}
}


void CDiscsShaftsPage::OnDeltaposSpinSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta * MinMax(&theApp.m_LastSettings.fDeltaScanSpeed, 1, 100));
	if (PROFILE->m_fScanSpeed < 1.0f) PROFILE->m_fScanSpeed = 1.0f;
	if (PROFILE->m_fScanSpeed > 1000.0f) PROFILE->m_fScanSpeed = 1000.0f;
	UpdateAllControls();

	*pResult = 0;
}


void CDiscsShaftsPage::OnEnChangeEditStartCoord()
{
	return;
	CString Buff;
	CCoord Cp = PROFILE->m_CpRect[0];

	m_editStartCoordinate.GetWindowTextW(Buff);
	ExtractVector(Buff, &PROFILE->m_CpRect[0].Side0.pt, (D3DXVECTOR3*)&PROFILE->m_CpRect[0].Side0.norm);
	if (Cp.Side0.pt != PROFILE->m_CpRect[0].Side0.pt || Cp.Side0.norm != PROFILE->m_CpRect[0].Side0.norm) {
		GenerateScanProfile();
	}
}


void CDiscsShaftsPage::OnEnChangeEditFinishCoord()
{
	return;
	CString Buff;
	CCoord Cp = PROFILE->m_CpRect[1];

	m_editStartCoordinate.GetWindowTextW(Buff);
	ExtractVector(Buff, &PROFILE->m_CpRect[1].Side0.pt, (D3DXVECTOR3*)&PROFILE->m_CpRect[1].Side0.norm);
	if (Cp.Side0.pt != PROFILE->m_CpRect[1].Side0.pt || Cp.Side0.norm != PROFILE->m_CpRect[1].Side0.norm) {
		GenerateScanProfile();
	}
}


void CDiscsShaftsPage::OnBnClickedButtonGotoStart()
{
	CCoord Cp;
	D3DXMATRIXA16 matRotate;

	Cp = PROFILE->m_CpRect[0];

	if (PROFILE->m_bUseAdditionalAngleDuringScanning[0]) {
		PROFILE->ApplyAdditionalAngle(&Cp);
	}

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
}


void CDiscsShaftsPage::OnBnClickedButtonGotoFinish()
{
	CCoord Cp;

	Cp = PROFILE->m_CpRect[1];

	if (PROFILE->m_bUseAdditionalAngleDuringScanning[0]) {
		PROFILE->ApplyAdditionalAngle(&Cp);
	}
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
}


void CDiscsShaftsPage::OnBnClickedButtonStoreStart()
{
	AddCoordinate(0);
}


void CDiscsShaftsPage::OnBnClickedButtonStoreFinish()
{
	AddCoordinate(1);
}

void CDiscsShaftsPage::AddCoordinate(int nIndex)
{
	CCoord	CpSurface;
	CpSurface.Zero();

	theApp.m_Motors.GetSurfacePos(&CpSurface);

	theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface, PORTSIDE);
	theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface, STARBOARD);
	theApp.m_Kinematics.GetComponentThickness(&CpSurface);
	theApp.m_UtUser.GetAllUt(&CpSurface);

	PROFILE->m_CpRect[nIndex] = CpSurface;
//	switch (nIndex) {
//	case 0: PROFILE->m_CpRect[0].Side0.fR = 0.0f;
//		break;
//	case 1: PROFILE->m_CpRect[1].Side0.fR = 360.0f;
//		break;
//	}

	GenerateScanProfile();
	UpdateAllControls();
}

void CDiscsShaftsPage::OnEnChangeEditFastIndex()
{
	CString Buff;
	float fTemp = PROFILE->m_fFastIncrement;

	m_editFastIncrement.GetWindowTextW(Buff);
	_WTOF(Buff, PROFILE->m_fFastIncrement);
	if (fTemp - PROFILE->m_fFastIncrement) {
		GenerateScanProfile();
		UpdateAllControls();
	}
}


void CDiscsShaftsPage::OnEnChangeEditIncrement1()
{
	CString Buff;
	float fTemp = PROFILE->m_fSlowIncrement;

	m_editSlowIncrement.GetWindowTextW(Buff);
	_WTOF(Buff, PROFILE->m_fSlowIncrement);
	if (fTemp - PROFILE->m_fSlowIncrement) {
		GenerateScanProfile();
		UpdateAllControls();
	}
}


void CDiscsShaftsPage::OnDeltaposSpinFastIndex(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	PROFILE->m_fFastIncrement += ((float)pNMUpDown->iDelta * theApp.m_LastSettings.fDeltaScanIncrement);
	GenerateScanProfile();
	UpdateAllControls();
	m_editFastIncrement.SetFocus();
	*pResult = 0;
}


void CDiscsShaftsPage::OnDeltaposSpinIncrement1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * theApp.m_LastSettings.fDeltaScanIncrement);
	GenerateScanProfile();
	UpdateAllControls();
	m_editSlowIncrement.SetFocus();
	*pResult = 0;
}

void CDiscsShaftsPage::GenerateScanProfile()
{
	CMainFrame*  pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	PROFILE->GenerateTubeScan(1);
	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG);
}


void CDiscsShaftsPage::OnBnClickedButtonStart()
{
	CMainFrame*  pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);
	CString Buff, Error, Buff1;

	if (theApp.m_Data.IsNameRequired() == TRUE) {
		Buff.LoadString(IDS_Volume_scan_Requested);
		Error.LoadString(IDS_Error);
		MessageBox(Buff, Error, MB_ICONERROR);
		return;
	}
	if ((theApp.m_Data.m_FileName.IsEmpty()) && (theApp.m_LastSettings.nSaveScanEveryLine != 0)) {
		Buff.LoadString(IDS_Save_every_line_is_selected);
		Buff1.LoadString(IDS_please_enter_file_name);
		Error.LoadString(IDS_Error);
		MessageBox(Buff + "\r\n" + Buff1, Error, MB_ICONERROR);
		return;
	}

	PROFILE->m_nScanStartLine = 0;
	if ((PROFILE->m_nScanLineL >= 1) && (PROFILE->m_ScanLine != NULL)) {
		PROFILE->m_nScanFinishLine = PROFILE->m_ScanLine[0].m_nCoordL - 1;
	}

	theApp.m_nScanWhat = 0;
	PROFILE->m_nCollectAxis = 5;
	PROFILE->m_nScanSide = 2;

	pFrame->StartScan(TURNTABLE_SCAN);
}


void CDiscsShaftsPage::OnBnClickedButtonAbort()
{
	theApp.m_Thread.m_bAbortFlag = TRUE;
}


void CDiscsShaftsPage::OnTimer(UINT_PTR nIDEvent)
{
	CString Buff;
//	CCoord CpSurface, Cp;
//	float	fMaxSpeed;
//	static float fScanSpeed = -0.1f;
//	CString Micro = L"\x0b5";

	if (m_nOldScanLine - theApp.m_Thread.m_nScanLine) {
		Buff.Format(_T("%d of %d"), (m_nOldScanLine = theApp.m_Thread.m_nScanLine) + 1, PROFILE->m_nScanFinishLine + 1);
		m_editCurrentLine.SetWindowText(Buff);
	}

	CPropertyPage::OnTimer(nIDEvent);
}


void CDiscsShaftsPage::OnEnChangeEditResumeLine()
{
	CString	Buff;

	m_editResumeLine.GetWindowText(Buff);
	PROFILE->m_nScanResumeLine = _ttoi(Buff) - 1;
	if (PROFILE->m_nScanResumeLine<PROFILE->m_nScanStartLine) {
		PROFILE->m_nScanResumeLine = PROFILE->m_nScanStartLine;
	}
	if (PROFILE->m_nScanResumeLine >= PROFILE->m_nScanFinishLine) {
		PROFILE->m_nScanResumeLine = PROFILE->m_nScanFinishLine;
	}
}


void CDiscsShaftsPage::OnEnChangeEditAdditionalTip()
{
	CString Buff;

	m_editAdditionalAngle.GetWindowTextW(Buff);
	_WTOF(Buff, PROFILE->m_vecAdditionalAngle[0].x);
}


void CDiscsShaftsPage::OnDeltaposSpinAdditionalTip(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	PROFILE->m_vecAdditionalAngle[0].x += ((float)pNMUpDown->iDelta * 0.1f);
	UpdateAllControls();
	*pResult = 0;
}


void CDiscsShaftsPage::OnBnClickedCheckAdditionalTipAngle()
{
	m_checkAdditionalAngle.GetCheck() ? PROFILE->m_bUseAdditionalAngleDuringScanning[0] = true : PROFILE->m_bUseAdditionalAngleDuringScanning[0] = false;
}


void CDiscsShaftsPage::OnBnClickedButtonZeroTt()
{
	int nAxis = theApp.m_Tank.nRLeft;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.SuspendMotors();
	switch (theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Axes[nAxis].nOffset = 0;
		Sleep(100);
		theApp.m_Ethercat.InvalidateRobotJoints();
		theApp.m_Axes[nAxis].nOffset = theApp.m_Axes[nAxis].nPos;
		break;
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
		if (theApp.m_Motors.WaitMotionFinished(1.0) == true) {
			theApp.m_Motors.ZeroAxis(nAxis);
		}
		break;
	}
	theApp.SaveTank();
	theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
}
