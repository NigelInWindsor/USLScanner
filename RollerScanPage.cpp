// RollerScanPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RollerScanPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRollerScanPage property page

IMPLEMENT_DYNCREATE(CRollerScanPage, CPropertyPage)

CRollerScanPage::CRollerScanPage() : CPropertyPage(CRollerScanPage::IDD)
{
	//{{AFX_DATA_INIT(CRollerScanPage)
	//}}AFX_DATA_INIT
	
}

CRollerScanPage::~CRollerScanPage()
{
}

void CRollerScanPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRollerScanPage)
	DDX_Control(pDX, IDC_EDIT_SCAN_SPEED, m_editScanSpeed);
	DDX_Control(pDX, IDC_BUTTON_START, m_buttonStartPause);
	DDX_Control(pDX, IDC_EDIT_RESUME_LINE, m_editResumeLine);
	DDX_Control(pDX, IDC_EDIT_SCAN_LINE, m_editScanLine);
	DDX_Control(pDX, IDC_EDIT_CIRCUM_DISTANCE, m_editCircumDistance);
	DDX_Control(pDX, IDC_SPIN_SLOW_INCREMENT, m_spinSlowIncrement);
	DDX_Control(pDX, IDC_SPIN_FAST_INCREMENT, m_spinFastIncrement);
	DDX_Control(pDX, IDC_EDIT_FAST_INCREMENT, m_editFastIncrement);
	DDX_Control(pDX, IDC_EDIT_SLOW_INCREMENT, m_editSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_CURRENT_POS, m_editCurrentPos);
	DDX_Control(pDX, IDC_EDIT_START_COORD, m_editStartCoord);
	DDX_Control(pDX, IDC_EDIT_FINISH_COORD, m_editFinishCoord);
	DDX_Control(pDX, IDC_EDIT_DIAMETER, m_editDiameter);
	DDX_Control(pDX, IDC_SPIN_DIAMETER, m_spinDiameter);
	DDX_Control(pDX, IDC_COMBO_SCAN_TYPE, m_comboScanType);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_editFilePath);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_DIAMETER_FROM_COORDS, m_checkDiameterFromCoords);
}


BEGIN_MESSAGE_MAP(CRollerScanPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRollerScanPage)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_FILEPATH, OnChangeEditFilepath)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_CBN_SELCHANGE(IDC_COMBO_SCAN_TYPE, OnSelchangeComboScanType)
	ON_EN_CHANGE(IDC_EDIT_DIAMETER, OnChangeEditDiameter)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DIAMETER, OnDeltaposSpinDiameter)
	ON_BN_CLICKED(IDC_BUTTON_START_COORD, OnButtonStartCoord)
	ON_BN_CLICKED(IDC_BUTTON_FINISH_COORD, OnButtonFinishCoord)
	ON_EN_CHANGE(IDC_EDIT_START_COORD, OnChangeEditStartCoord)
	ON_EN_CHANGE(IDC_EDIT_FINISH_COORD, OnChangeEditFinishCoord)
	ON_BN_CLICKED(IDC_BUTTON_ZERO, OnButtonZero)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_GOTO_START, OnButtonGotoStart)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_FINISH, OnButtonGotoFinish)
	ON_EN_CHANGE(IDC_EDIT_FAST_INCREMENT, OnChangeEditFastIncrement)
	ON_EN_CHANGE(IDC_EDIT_SLOW_INCREMENT, OnChangeEditSlowIncrement)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FAST_INCREMENT, OnDeltaposSpinFastIncrement)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SLOW_INCREMENT, OnDeltaposSpinSlowIncrement)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_EN_CHANGE(IDC_EDIT_SCAN_LINE, OnChangeEditScanLine)
	ON_EN_CHANGE(IDC_EDIT_RESUME_LINE, OnChangeEditResumeLine)
	ON_EN_CHANGE(IDC_EDIT_SCAN_SPEED, OnChangeEditScanSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCAN_SPEED, OnDeltaposSpinScanSpeed)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_DIAMETER_FROM_COORDS, &CRollerScanPage::OnBnClickedCheckDiameterFromCoords)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRollerScanPage message handlers

BOOL CRollerScanPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	Buff = L"XYZ fast: Circumferential step" ;	m_comboScanType.AddString(Buff);
	Buff = L"Circumferential fast: XYZ step";	m_comboScanType.AddString(Buff);

	
	UpdateAllControls();
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CRollerScanPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CRollerScanPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	CString Buff;

	m_checkDiameterFromCoords.SetCheck(PROFILE->m_bDiametersDervivedFromCoords);

	m_comboScanType.SetCurSel((PROFILE->m_nComputeAlgorithm - 100) % 2);

	Buff.Format(_T("%.02f %s"),PROFILE->m_fComponentDiameter[0]*theApp.m_fUnits,theApp.m_Units);
	m_editDiameter.SetWindowText(Buff);
	m_editDiameter.SetReadOnly(PROFILE->m_bDiametersDervivedFromCoords);

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		Buff.Format(_T("X: %.1f Y: %.1f Z: %.1f R: %.1f%s"),PROFILE->m_CpRect[0].Side[0].fX,
													PROFILE->m_CpRect[0].Side[0].fY,
													PROFILE->m_CpRect[0].Side[0].fZ,
													PROFILE->m_CpRect[0].Side[0].fR,DEGREES);
		m_editStartCoord.SetWindowText(Buff);

		Buff.Format(_T("X: %.1f Y: %.1f Z: %.1f R: %.1f%s"),PROFILE->m_CpRect[1].Side[0].fX,
													PROFILE->m_CpRect[1].Side[0].fY,
													PROFILE->m_CpRect[1].Side[0].fZ,
													PROFILE->m_CpRect[1].Side[0].fR,DEGREES);
		m_editFinishCoord.SetWindowText(Buff);
	} else {
		Buff.Format(_T("X: %.1f Y: %.1f Z: %.1f R: %.1f%s"),PROFILE->m_CpRect[0].Side[0].fX * theApp.m_fUnits,
													PROFILE->m_CpRect[0].Side[0].fY * theApp.m_fUnits,
													PROFILE->m_CpRect[0].Side[0].fZ * theApp.m_fUnits,
													PROFILE->m_CpRect[0].Side[0].fR,DEGREES);
		m_editStartCoord.SetWindowText(Buff);

		Buff.Format(_T("X: %.1f Y: %.1f Z: %.1f R: %.1f%s"),PROFILE->m_CpRect[1].Side[0].fX * theApp.m_fUnits,
													PROFILE->m_CpRect[1].Side[0].fY * theApp.m_fUnits,
													PROFILE->m_CpRect[1].Side[0].fZ * theApp.m_fUnits,
													PROFILE->m_CpRect[1].Side[0].fR,DEGREES);
		m_editFinishCoord.SetWindowText(Buff);
	}

	switch(PROFILE->m_nFastAxis) {
	case 0:
		Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fFastIncrement * theApp.m_fUnits,theApp.m_Units);
		m_editFastIncrement.SetWindowText(Buff);
		Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fSlowIncrement * theApp.m_fUnits,DEGREES);
		m_editSlowIncrement.SetWindowText(Buff);

		Buff.Format(L"%.01f mm",(PROFILE->m_fComponentDiameter[0] * PIf) * PROFILE->m_fSlowIncrement / 360.0f);
		m_editCircumDistance.SetWindowText(Buff);

		Buff.Format(_T("%.0f mm/s"),PROFILE->m_fScanSpeed);
		m_editScanSpeed.SetWindowText(Buff);

		break;
	case 5:
		Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fFastIncrement * theApp.m_fUnits,DEGREES);
		m_editFastIncrement.SetWindowText(Buff);
		Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fSlowIncrement * theApp.m_fUnits,theApp.m_Units);
		m_editSlowIncrement.SetWindowText(Buff);

		Buff.Format(L"%.01fmm",(PROFILE->m_fComponentDiameter[0] * PIf) * PROFILE->m_fFastIncrement / 360.0f);
		m_editCircumDistance.SetWindowText(Buff);

		Buff.Format(_T("%.1f %s/s"),PROFILE->m_fScanSpeed,DEGREES);
		m_editScanSpeed.SetWindowText(Buff);

		break;
	}

	Buff.Format(_T("%d"),PROFILE->m_nScanResumeLine+1);
	m_editResumeLine.SetWindowText(Buff);
}

void CRollerScanPage::OnChangeEditFilepath() 
{
	m_editFilePath.GetWindowText(theApp.m_Data.m_FileName);
	theApp.m_Data.m_FilePath = theApp.m_StorePath[0] + L"\\" + theApp.m_Data.m_FileName + L".dat";
}

void CRollerScanPage::OnButtonBrowse() 
{
	int nPos;

	CFileDialog File(TRUE,_T(".dat"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Data (*.dat)|*.dat|All Files (*.*)|*.*||"),NULL);
	File.m_ofn.lpstrInitialDir = theApp.m_StorePath[0];
	if(File.DoModal()==IDOK) {
		theApp.m_Data.m_FilePath.Format(_T("%s"),File.GetPathName());
		theApp.m_Data.m_FileName.Format(_T("%s"),File.GetFileTitle());

		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = theApp.m_Data.m_FilePath.ReverseFind('\\')) >0) {
				theApp.m_StorePath[0] = theApp.m_Data.m_FilePath.Left(nPos);
			}
		}

		m_editFilePath.SetWindowText(File.GetFileTitle());
	};	
}



void CRollerScanPage::OnSelchangeComboScanType() 
{
	PROFILE->m_nComputeAlgorithm = m_comboScanType.GetCurSel() + 100;

	GenerateScanLines();
	UpdateAllControls();
}

void CRollerScanPage::OnChangeEditDiameter() 
{
	FRAME;
	CString Buff;
	float fTemp;

	m_editDiameter.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;

	if(PROFILE->m_fComponentDiameter[0]-fTemp) {
		PROFILE->m_fComponentDiameter[1]=PROFILE->m_fComponentDiameter[0]=fTemp;
		theApp.m_Kinematics.CalculateRollerKinematics(PROFILE->m_fComponentDiameter[0], false, false);
		pFrame->SendMessage(UI_INVALIDATE_FOR_ROLLER_KINEMATICS);
	};
}

void CRollerScanPage::OnDeltaposSpinDiameter(NMHDR* pNMHDR, LRESULT* pResult) 
{
	FRAME;
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fComponentDiameter[0] -= ((float)pNMUpDown->iDelta * 0.01f);
	} else {
		PROFILE->m_fComponentDiameter[0] -= ((float)pNMUpDown->iDelta * 0.0254f);
	}
	PROFILE->m_fComponentDiameter[1] = PROFILE->m_fComponentDiameter[0];
	theApp.m_Kinematics.CalculateRollerKinematics(PROFILE->m_fComponentDiameter[0], false, false);
	pFrame->SendMessage(UI_INVALIDATE_FOR_ROLLER_KINEMATICS);
	UpdateAllControls();
	
	*pResult = 0;
}

void CRollerScanPage::OnButtonStartCoord() 
{
	theApp.m_Motors.GetSurfacePos(&PROFILE->m_CpRect[0]);
	GenerateScanLines();
	UpdateAllControls();
}

void CRollerScanPage::OnButtonFinishCoord() 
{
	theApp.m_Motors.GetSurfacePos(&PROFILE->m_CpRect[1]);
	GenerateScanLines();
	UpdateAllControls();
}

void CRollerScanPage::OnChangeEditStartCoord() 
{
	CString Buff;
	float fX,fY,fZ,fR;

	m_editStartCoord.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%f Y:%f Z:%f R:%f"),&fX,&fY,&fZ,&fR);

	if(	PROFILE->m_CpRect[0].Side[0].fX - (fX / theApp.m_fUnits)) {
		PROFILE->m_CpRect[0].Side[1].fX = PROFILE->m_CpRect[0].Side[0].fX = fX / theApp.m_fUnits;
	}
	if(	PROFILE->m_CpRect[0].Side[0].fY - (fY / theApp.m_fUnits)) {
		PROFILE->m_CpRect[0].Side[1].fY = PROFILE->m_CpRect[0].Side[0].fY = fY / theApp.m_fUnits;
	}
	if(	PROFILE->m_CpRect[0].Side[0].fZ - (fZ / theApp.m_fUnits)) {
		PROFILE->m_CpRect[0].Side[1].fZ = PROFILE->m_CpRect[0].Side[0].fZ = fZ / theApp.m_fUnits;
	}
	if(	PROFILE->m_CpRect[0].Side[0].fR - (fR / theApp.m_fUnits)) {
		PROFILE->m_CpRect[0].Side[1].fR = PROFILE->m_CpRect[0].Side[0].fR = fR / theApp.m_fUnits;
	}
}

void CRollerScanPage::OnChangeEditFinishCoord() 
{
	CString Buff;
	float fX,fY,fZ,fR;

	m_editFinishCoord.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%f Y:%f Z:%f R:%f"),&fX,&fY,&fZ,&fR);
	if(	PROFILE->m_CpRect[1].Side[0].fX - (fX / theApp.m_fUnits)) {
		PROFILE->m_CpRect[1].Side[1].fX = PROFILE->m_CpRect[1].Side[0].fX = fX / theApp.m_fUnits;
	}
	if(	PROFILE->m_CpRect[1].Side[0].fY - (fY / theApp.m_fUnits)) {
		PROFILE->m_CpRect[1].Side[1].fY = PROFILE->m_CpRect[1].Side[0].fY = fY / theApp.m_fUnits;
	}
	if(	PROFILE->m_CpRect[1].Side[0].fZ - (fZ / theApp.m_fUnits)) {
		PROFILE->m_CpRect[1].Side[1].fZ = PROFILE->m_CpRect[1].Side[0].fZ = fZ / theApp.m_fUnits;
	}

	if(	PROFILE->m_CpRect[1].Side[0].fR - (fR / theApp.m_fUnits)) {
		PROFILE->m_CpRect[1].Side[1].fR = PROFILE->m_CpRect[1].Side[0].fR = fR / theApp.m_fUnits;
	}
}


BOOL CRollerScanPage::OnSetActive() 
{
	m_nOldThreadAction = -1;
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	SetTimer(1,200,NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CRollerScanPage::OnKillActive() 
{
	KillTimer(1);
	return CPropertyPage::OnKillActive();
}

void CRollerScanPage::OnButtonZero() 
{
	theApp.m_Motors.ZeroAxis(theApp.m_Tank.nRLeft);
}

void CRollerScanPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;

	if(m_nOldScanLine-theApp.m_Thread.m_nScanLine) {
		Buff.Format(_T("%d"),(m_nOldScanLine=theApp.m_Thread.m_nScanLine) + 1);
		m_editScanLine.SetWindowText(Buff);
	}

	Buff.Format(L"%.01f%s",(float)theApp.m_Axes[theApp.m_Tank.nRLeft].nPos * theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize,DEGREES);
	m_editCurrentPos.SetWindowText(Buff);

	if( theApp.m_Thread.m_nThreadAction - m_nOldThreadAction) {
		if(	theApp.m_Thread.m_nThreadAction <= MOVETO_STRAITE_LINE) {
			m_buttonStartPause.SetWindowText(L"Start");
		} else {
			m_buttonStartPause.SetWindowText(L"Pause");
		}
	}

	CPropertyPage::OnTimer(nIDEvent);
}

void CRollerScanPage::OnButtonGotoStart() 
{
	Goto(0);
}

void CRollerScanPage::OnButtonGotoFinish() 
{
	Goto(1);
}

void CRollerScanPage::Goto(int nWhich) 
{
	CCoord	Cp,CpSurface,CpHead;

	if(theApp.m_ServoUser.AreMotorsPowerd()==FALSE) return;

	theApp.m_Thread.StopCurrentThreadAction();

	theApp.m_Motors.GetHeadPos(&CpHead);

	CpSurface = PROFILE->m_CpRect[nWhich];


	switch(theApp.m_Tank.nScannerDescription) {
	default:
		if(theApp.m_LastSettings.bGotoViaDangerPlane == FALSE) {
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,PORTSIDE,TRUE,TRUE, NULL, CURRENT_FL);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,STARBOARD,TRUE,TRUE, NULL, CURRENT_FL);

			theApp.m_Thread.m_CpMoveTo = Cp;
			theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
		} else {
			theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide = 2;
			theApp.m_Thread.m_CpMoveTo = CpSurface;
			theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
		}
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
		if(theApp.m_LastSettings.bGotoViaDangerPlane == FALSE) {
			theApp.m_Thread.m_CpMoveTo = CpSurface;
			theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
		} else {
			theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide = 2;
			theApp.m_Thread.m_CpMoveTo = CpSurface;
			theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
		}
		break;
	};
	
}


void CRollerScanPage::OnChangeEditFastIncrement() 
{
	
	CString Buff;
	float fTemp;

	m_editFastIncrement.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fFastIncrement - fTemp) {
		PROFILE->m_fFastIncrement = fTemp;
		UpdateAllControls();
		UpdateMainFrameTools();
		GenerateScanLines();
	}
}

void CRollerScanPage::OnChangeEditSlowIncrement() 
{
	
	CString Buff;
	float fTemp;

	m_editSlowIncrement.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fSlowIncrement - fTemp) {
		PROFILE->m_fSlowIncrement = fTemp;
		UpdateAllControls();
		UpdateMainFrameTools();
		GenerateScanLines();
	}
}

void CRollerScanPage::OnDeltaposSpinFastIncrement(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fFastIncrement -= ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		PROFILE->m_fFastIncrement -= ((float)pNMUpDown->iDelta * 0.0254f);
	}
	UpdateAllControls();
	UpdateMainFrameTools();

	GenerateScanLines();

	*pResult = 0;
}

void CRollerScanPage::OnDeltaposSpinSlowIncrement(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fSlowIncrement -= ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		PROFILE->m_fSlowIncrement -= ((float)pNMUpDown->iDelta * 0.0254f);
	}
	UpdateAllControls();
	UpdateMainFrameTools();

	GenerateScanLines();

	*pResult = 0;
}

void CRollerScanPage::GenerateScanLines()
{
	PROFILE->m_nComputeAlgorithm = m_comboScanType.GetCurSel() + 100;
	switch(PROFILE->m_nComputeAlgorithm) {
	case 100: 
		PROFILE->m_nFastAxis = 0;
		PROFILE->m_nSlowAxis = 5;
		PROFILE->setRUsed(true);
		break;
	case 101: 
		PROFILE->m_nFastAxis = 5;
		PROFILE->m_nSlowAxis = 0;
		PROFILE->setRUsed(true);
		break;
	}
	PROFILE->GenerateScanLines();
}

void CRollerScanPage::UpdateMainFrameTools()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(	pFrame->m_pMotionToolsSheet ) {
		pFrame->m_pMotionToolsSheet->UpdateAllPages();
	}

}

void CRollerScanPage::OnButtonStart() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);
	CString Buff,Error,Buff1;

	if(	theApp.m_Thread.m_nThreadAction <= MOVETO_STRAITE_LINE) {

		Error.LoadString(IDS_Error);

		if(theApp.m_Data.IsNameRequired() == TRUE) {
			Buff.LoadString(IDS_Volume_scan_Requested);
			MessageBox(Buff,Error,MB_ICONERROR);
			return;
		}
		if((theApp.m_Data.m_FileName.IsEmpty()) && (theApp.m_LastSettings.nSaveScanEveryLine!=0)) {
			Buff.LoadString(IDS_Save_every_line_is_selected);
			Buff1.LoadString(IDS_please_enter_file_name);
			MessageBox(Buff + "\r\n" + Buff1,Error,MB_ICONERROR);
			return;
		}

		if (PROFILE->m_CpRect[0].Length(PROFILE->m_CpRect[1]) == 0.0f) {
			Buff = "The XYZ scan length is too short for a scan";
			MessageBox(Buff, Error, MB_ICONERROR);
			return;
		}
		if (fabsf(PROFILE->m_CpRect[0].Side0.fR - PROFILE->m_CpRect[1].Side0.fR) == 0.0f) {
			Buff = "The start and finish angle are the same";
			MessageBox(Buff, Error, MB_ICONERROR);
			return;
		}
		if (PROFILE->m_fComponentDiameter[0] == 0.0f) {
			Buff = "The component diameter == 0";
			MessageBox(Buff, Error, MB_ICONERROR);
			return;
		}

		theApp.m_nScanWhat = 0;

		if(pFrame->m_pViewSheet[0]==NULL) {
			theApp.m_nUseTurntableOrRollers = 0;
			GenerateScanLines();
			PROFILE->m_nScanStartLine = 0;
			switch(PROFILE->m_nComputeAlgorithm) {
			case 100: 
				if((PROFILE->m_nScanLineL>=1) && (PROFILE->m_ScanLine!=NULL)) {
					PROFILE->m_nScanFinishLine = PROFILE->m_nScanLineL-1;
				}
				break;
			case 101: 
				if((PROFILE->m_nScanLineL>=1) && (PROFILE->m_ScanLine!=NULL)) {
					PROFILE->m_nScanFinishLine = PROFILE->m_ScanLine[0].m_nCoordL-1;
				}
				break;
			};
		}

		switch(PROFILE->m_nComputeAlgorithm) {
		case 100: 
			PROFILE->m_nCollectAxis = 0;
			pFrame->StartScan(CONTOUR_SCAN);
			break;
		case 101:
			PROFILE->m_nCollectAxis = 5;
			pFrame->StartScan(SEGMENT_TURNTABLE_SCAN);
			break;
		}

	} else {
		theApp.m_Thread.m_bAbortFlag = TRUE;
	}
}

void CRollerScanPage::OnChangeEditScanLine() 
{
	
}

void CRollerScanPage::OnChangeEditResumeLine() 
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

void CRollerScanPage::OnChangeEditScanSpeed() 
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
		UpdateAllControls();
	}
}

void CRollerScanPage::OnDeltaposSpinScanSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fScanSpeed -= ((float)pNMUpDown->iDelta);
		if(PROFILE->m_fScanSpeed<1.0f) PROFILE->m_fScanSpeed = 1.0f;
		if(PROFILE->m_fScanSpeed>1000.0f) PROFILE->m_fScanSpeed = 1000.0f;
	} else {
		PROFILE->m_fScanSpeed -= ((float)pNMUpDown->iDelta * 1.016f);
		if(PROFILE->m_fScanSpeed<25.4f) PROFILE->m_fScanSpeed = 25.4f;
		if(PROFILE->m_fScanSpeed>1016.0f) PROFILE->m_fScanSpeed = 1016.0f;
	}
	UpdateAllControls();
	
	*pResult = 0;
}


void CRollerScanPage::OnBnClickedCheckDiameterFromCoords()
{
	float fTempDiameter[2];

	PROFILE->m_bDiametersDervivedFromCoords = m_checkDiameterFromCoords.GetCheck();
	PROFILE->GetDiameters(fTempDiameter);
	UpdateAllControls();
}
