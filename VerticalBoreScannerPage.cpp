// VerticalBoreScannerPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "VerticalBoreScannerPage.h"
#include "FileThumbNailDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVerticalBoreScannerPage property page

IMPLEMENT_DYNCREATE(CVerticalBoreScannerPage, CPropertyPage)

CVerticalBoreScannerPage::CVerticalBoreScannerPage() : CPropertyPage(CVerticalBoreScannerPage::IDD)
{
	//{{AFX_DATA_INIT(CVerticalBoreScannerPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nOldScanLine = -1;
	m_nOldThreadAction = -1;
}

CVerticalBoreScannerPage::~CVerticalBoreScannerPage()
{
}

void CVerticalBoreScannerPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVerticalBoreScannerPage)
	DDX_Control(pDX, IDC_EDIT_TOTAL_DEFECT_AREA, m_editTotalDefectArea);
	DDX_Control(pDX, IDC_EDIT_SENTENCE, m_editSentence);
	DDX_Control(pDX, IDC_EDIT_REF_RANGE, m_editRefZone);
	DDX_Control(pDX, IDC_EDIT_EVAL_ZONE, m_editEvalZone);
	DDX_Control(pDX, IDC_EDIT_DB_RANGE, m_editDBRange);
	DDX_Control(pDX, IDC_EDIT_SCAN_LINE, m_editScanLine);
	DDX_Control(pDX, IDC_EDIT_RESUME_LINE, m_editResumeLine);
	DDX_Control(pDX, IDC_BUTTON_START, m_buttonStart);
	DDX_Control(pDX, IDC_SPIN_DIAMETER, m_spinDiameter);
	DDX_Control(pDX, IDC_EDIT_DIAMETER, m_editDiameter);
	DDX_Control(pDX, IDC_EDIT_ANGLE, m_editAngle);
	DDX_Control(pDX, IDC_EDIT_FAST_INCREMENT, m_editFastIncrement);
	DDX_Control(pDX, IDC_EDIT_FINISH_HEIGHT, m_editFinishHeight);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_editHeight);
	DDX_Control(pDX, IDC_EDIT_SLOW_INCREMENT, m_editSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_START_HEIGHT, m_editStartHeight);
	DDX_Control(pDX, IDC_SPIN_FAST_INCREMENT, m_spinFastIncrement);
	DDX_Control(pDX, IDC_SPIN_SLOW_INCREMENT, m_spinSlowIncrement);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVerticalBoreScannerPage, CPropertyPage)
	//{{AFX_MSG_MAP(CVerticalBoreScannerPage)
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_EDIT_SLOW_INCREMENT, OnChangeEditSlowIncrement)
	ON_EN_CHANGE(IDC_EDIT_FAST_INCREMENT, OnChangeEditFastIncrement)
	ON_EN_CHANGE(IDC_EDIT_START_HEIGHT, OnChangeEditStartHeight)
	ON_EN_CHANGE(IDC_EDIT_FINISH_HEIGHT, OnChangeEditFinishHeight)
	ON_WM_TIMER()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SLOW_INCREMENT, OnDeltaposSpinSlowIncrement)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FAST_INCREMENT, OnDeltaposSpinFastIncrement)
	ON_COMMAND(ID_BUTTON_TURNTABLE_UP, OnButtonTurntableUp)
	ON_COMMAND(ID_BUTTON_TURNTABLE_DOWN, OnButtonTurntableDown)
	ON_COMMAND(ID_BUTTON_PROBES_UP, OnButtonProbesUp)
	ON_BN_CLICKED(IDC_BUTTON_FINISH_HEIGHT, OnButtonFinishHeight)
	ON_BN_CLICKED(IDC_BUTTON_START_HEIGHT, OnButtonStartHeight)
	ON_COMMAND(ID_BUTTON_PROBES_TO_START_HEIGHT, OnButtonProbesToStartHeight)
	ON_COMMAND(ID_BUTTON_PROBES_TO_FINISH_HEIGHT, OnButtonProbesToFinishHeight)
	ON_EN_CHANGE(IDC_EDIT_DIAMETER, OnChangeEditDiameter)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DIAMETER, OnDeltaposSpinDiameter)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_EN_CHANGE(IDC_EDIT_RESUME_LINE, OnChangeEditResumeLine)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_BUTTON_ZERO_ROTATE, OnButtonZeroRotate)
	ON_EN_CHANGE(IDC_EDIT_DB_RANGE, OnChangeEditDbRange)
	ON_EN_CHANGE(IDC_EDIT_EVAL_ZONE, OnChangeEditEvalZone)
	ON_EN_CHANGE(IDC_EDIT_REF_RANGE, OnChangeEditRefRange)
	ON_COMMAND(ID_BUTTON_DB_RANGE, OnButtonDbRange)
	ON_COMMAND(ID_BUTTON_EVAL_ZONE, OnButtonEvalZone)
	ON_COMMAND(ID_BUTTON_REF_ZONE, OnButtonRefZone)
	ON_COMMAND(ID_BUTTON_SENTENCE, OnButtonSentence)
	ON_EN_CHANGE(IDC_EDIT_TOTAL_DEFECT_AREA, OnChangeEditTotalDefectArea)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVerticalBoreScannerPage message handlers


int CVerticalBoreScannerPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_VERTICAL_BORESCAN_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+210,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolBar);
	
	
	return 0;
}

BOOL CVerticalBoreScannerPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_spinFastIncrement.SetRange(0,200);
	m_spinSlowIncrement.SetRange(0,200);
	
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CVerticalBoreScannerPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
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
	case ID_BUTTON_PROBES_UP: strTipText.LoadString(IDS_Probes_up);
	   break;
	case ID_BUTTON_PROBES_TO_START_HEIGHT: strTipText.LoadString(IDS_Probes_to_start);
		break;
	case ID_BUTTON_PROBES_TO_FINISH_HEIGHT: strTipText.LoadString(IDS_Probes_to_finish);
		break;
	case ID_BUTTON_TURNTABLE_UP: strTipText.LoadString(IDS_Turntable_up);
		break;
	case ID_BUTTON_TURNTABLE_DOWN: strTipText.LoadString(IDS_Turntable_down);
		break;
	case ID_BUTTON_ZERO_ROTATE: strTipText.LoadString(IDS_Set_Zero_degrees);
		break;
	case ID_FILE_OPEN: strTipText.LoadString(IDS_Open);
		break;
	case ID_FILE_SAVEAS: strTipText.LoadString(IDS_Save_As);
		break;
	case ID_BUTTON_DB_RANGE: strTipText.LoadString(IDS_Update_dB_Range);
		break;
	case ID_BUTTON_EVAL_ZONE: strTipText.LoadString(IDS_Update_Evaluation_Z_Heights);
		break;
	case ID_BUTTON_REF_ZONE: strTipText.LoadString(IDS_Update_Reference_Z_Heights);
		break;
	case ID_BUTTON_SENTENCE: strTipText.LoadString(IDS_Sentence_the_component);
		break;

	}
//      strTipText.Format(_T("Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

void CVerticalBoreScannerPage::SetToolBarCheckedState()
{
	
	int	nIndex,nFlag;

	if((theApp.m_Axes[theApp.m_Tank.nZRight].nStatus & PMAC_HOMED) && (abs(theApp.m_Axes[theApp.m_Tank.nZRight].nPos - theApp.m_Axes[theApp.m_Tank.nZRight].nMinTravel) < 10)) {
		nFlag = 0;
	} else {
		nFlag = TBBS_DISABLED;
	}
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_PROBES_TO_START_HEIGHT))>=0) m_wndToolBar.SetButtonStyle(nIndex,nFlag);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_PROBES_TO_FINISH_HEIGHT))>=0) m_wndToolBar.SetButtonStyle(nIndex,nFlag);

	if((theApp.m_Axes[theApp.m_Tank.nZLeft].nStatus & PMAC_HOMED) && (((float)abs(theApp.m_Axes[theApp.m_Tank.nZLeft].nPos) * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize) < 1.0f)) {
		nFlag = 0;
	} else {
		nFlag = TBBS_DISABLED;
	}
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_TURNTABLE_UP))>=0) m_wndToolBar.SetButtonStyle(nIndex,nFlag);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_TURNTABLE_DOWN))>=0) m_wndToolBar.SetButtonStyle(nIndex,nFlag);

}

void CVerticalBoreScannerPage::OnChangeEditSlowIncrement() 
{
	
	CString Buff;
	float fTemp;

	m_editSlowIncrement.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fSlowIncrement - fTemp) {
		PROFILE->m_fSlowIncrement = fTemp;
		UpdateAllControls();
		GenerateScanLines();

	}
}

void CVerticalBoreScannerPage::OnChangeEditFastIncrement() 
{
	
	CString Buff;
	float fTemp;

	m_editFastIncrement.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fFastIncrement - fTemp) {
		PROFILE->m_fFastIncrement = fTemp;
		UpdateAllControls();
		GenerateScanLines();

	}
}

void CVerticalBoreScannerPage::OnChangeEditStartHeight() 
{
	
	CString Buff;

	m_editStartHeight.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_CpRect[0].Side0.fZ);
}

void CVerticalBoreScannerPage::OnChangeEditFinishHeight() 
{
	
	CString Buff;

	m_editFinishHeight.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_CpRect[1].Side0.fZ);
}

void CVerticalBoreScannerPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;
	
	CCoord CpSurface;
	CString Unit[] = {L"mm", L"\x022", L"\x0b0"};
	int nOf;

	theApp.m_Motors.GetSurfacePos(&CpSurface);

	if(m_OldCpSurface.Side0.fZ - CpSurface.Side0.fZ) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPrecision,CpSurface.Side0.fZ * theApp.m_fUnits, theApp.m_Units);
		m_editHeight.SetWindowText(Buff);
	}

	if(m_OldCpSurface.Side0.fR - CpSurface.Side0.fR) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPrecision,CpSurface.Side0.fR, Unit[theApp.m_Axes[theApp.m_Tank.nRLeft].nUnitSelected]);
		m_editAngle.SetWindowText(Buff);
	}

	m_OldCpSurface = CpSurface;
	SetToolBarCheckedState();

	if(m_nOldScanLine-theApp.m_Thread.m_nScanLine) {
		nOf = 1;
		if(PROFILE->m_ScanLine) nOf = PROFILE->m_ScanLine[0].GetSize();
		Buff.Format(_T("%d of %d"),(m_nOldScanLine=theApp.m_Thread.m_nScanLine) + 1 , nOf);
		m_editScanLine.SetWindowText(Buff);
	}

	if( theApp.m_Thread.m_nThreadAction - m_nOldThreadAction) {
		if(	theApp.m_Thread.m_nThreadAction != CONTINUOUS_TURNTABLE_SCAN) {
			m_buttonStart.SetWindowText(L"Start");
		} else {
			m_buttonStart.SetWindowText(L"Pause");
		}
	}


	CPropertyPage::OnTimer(nIDEvent);
}

BOOL CVerticalBoreScannerPage::OnSetActive() 
{
	
	
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	
	SetTimer(1,200,NULL);

	
	return CPropertyPage::OnSetActive();
}

BOOL CVerticalBoreScannerPage::OnKillActive() 
{

	KillTimer(1);
	return CPropertyPage::OnKillActive();
}

void CVerticalBoreScannerPage::UpdateAllControls()
{
	
	CString Micro = L"\x0b5";
	if (GetSafeHwnd() == NULL) return;
	CString Buff;


	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fFastIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editFastIncrement.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fSlowIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editSlowIncrement.SetWindowText(Buff);

	Buff.Format(L"%.01f mm",PROFILE->m_CpRect[0].Side0.fZ);
	m_editStartHeight.SetWindowText(Buff);
	Buff.Format(L"%.01f mm",PROFILE->m_CpRect[1].Side0.fZ);
	m_editFinishHeight.SetWindowText(Buff);

	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fComponentDiameter[0]*theApp.m_fUnits,theApp.m_Units);
	m_editDiameter.SetWindowText(Buff);

	Buff.Format(_T("%d"),PROFILE->m_nScanResumeLine+1);
	m_editResumeLine.SetWindowText(Buff);

	Buff.Format(L"%.01f dB , %.01f dB",	PROFILE->m_BoreScanHeader.fdBTop,	PROFILE->m_BoreScanHeader.fdBBottom);
	m_editDBRange.SetWindowText(Buff);

	Buff.Format(L"Z top %.01f bottom %.01f",	PROFILE->m_BoreScanHeader.fEvalZTop,	PROFILE->m_BoreScanHeader.fEvalZBottom);
	m_editEvalZone.SetWindowText(Buff);

	Buff.Format(L"Z top %.01f bottom %.01f",	PROFILE->m_BoreScanHeader.fRefZTop,	PROFILE->m_BoreScanHeader.fRefZBottom);
	m_editRefZone.SetWindowText(Buff);

	Buff.Format(L"%.0f mm\x0b2",PROFILE->m_BoreScanHeader.fDefectArea);
	m_editTotalDefectArea.SetWindowText(Buff);

//	Buff.Format(_T("%.1f %s/s"),PROFILE->m_fScanSpeed,DEGREES);
//	m_editScanSpeed.SetWindowText(Buff);


}

void CVerticalBoreScannerPage::OnDeltaposSpinSlowIncrement(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * 0.0254f);
	}
	UpdateAllControls();
//	UpdateMainFrameTools();

	GenerateScanLines();

	
	*pResult = 0;
}

void CVerticalBoreScannerPage::OnDeltaposSpinFastIncrement(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fFastIncrement += ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		PROFILE->m_fFastIncrement += ((float)pNMUpDown->iDelta * 0.0254f);
	}
	UpdateAllControls();
//	GenerateScanLines();
	
	*pResult = 0;
}

void CVerticalBoreScannerPage::OnButtonTurntableUp() 
{
	
	
	theApp.m_PmacUser.VerticalBoreScannerMoveTurntable(0);
}

void CVerticalBoreScannerPage::OnButtonTurntableDown() 
{
	
	theApp.m_PmacUser.VerticalBoreScannerMoveTurntable(theApp.m_Axes[theApp.m_Tank.nZRight].nMinTravel);
	
}

void CVerticalBoreScannerPage::OnButtonProbesUp() 
{
	
	
	theApp.m_PmacUser.VerticalBoreScannerMoveProbes(0.0f);
}

void CVerticalBoreScannerPage::OnButtonFinishHeight() 
{
	
	CCoord Cp;
	
	theApp.m_Motors.GetSurfacePos(&PROFILE->m_CpRect[1]);
	UpdateAllControls();
	GenerateScanLines();
}

void CVerticalBoreScannerPage::OnButtonStartHeight() 
{
	
	CCoord Cp;
	
	theApp.m_Motors.GetSurfacePos(&PROFILE->m_CpRect[0]);
	UpdateAllControls();
	GenerateScanLines();

}

void CVerticalBoreScannerPage::OnButtonProbesToStartHeight() 
{
	
	
	theApp.m_PmacUser.VerticalBoreScannerMoveProbes(PROFILE->m_CpRect[0].Side0.fZ);
	GenerateScanLines();

}

void CVerticalBoreScannerPage::OnButtonProbesToFinishHeight() 
{
	
	
	theApp.m_PmacUser.VerticalBoreScannerMoveProbes(PROFILE->m_CpRect[1].Side0.fZ);
}


void CVerticalBoreScannerPage::OnChangeEditDiameter() 
{
	
	CString Buff;
	float fTemp;

	m_editDiameter.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;

	if(PROFILE->m_fComponentDiameter[0]-fTemp) {
		PROFILE->m_fComponentDiameter[0]=fTemp;
		theApp.m_Kinematics.CalculateRollerKinematics(PROFILE->m_fComponentDiameter[0], true, true);
//		UpdateSurfaceSpeed();
	};
}

void CVerticalBoreScannerPage::OnDeltaposSpinDiameter(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fComponentDiameter[0] -= ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		PROFILE->m_fComponentDiameter[0] -= ((float)pNMUpDown->iDelta * 0.254f);
	}
	theApp.m_Kinematics.CalculateRollerKinematics(PROFILE->m_fComponentDiameter[0], true, true);
	UpdateAllControls();
	
	*pResult = 0;
}

void CVerticalBoreScannerPage::GenerateScanLines()
{
	PROFILE->m_nComponentDescription = CIRCULAR;

	if(PROFILE->m_TaughtLine==NULL) {
		PROFILE->AddCoordToLine(TAUGHTLINES,0, PROFILE->m_CpRect[0]);
		PROFILE->AddCoordToLine(TAUGHTLINES,0, PROFILE->m_CpRect[1]);
	} else {
		PROFILE->m_TaughtLine[0].m_pCp[0] = PROFILE->m_CpRect[0];
		PROFILE->m_TaughtLine[0].m_pCp[1] = PROFILE->m_CpRect[1];
		PROFILE->m_TaughtLine[0].m_bModified = TRUE;
	}

	PROFILE->GenerateRotoSymetricScanLineV1();
}


void CVerticalBoreScannerPage::OnChangeEditResumeLine() 
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

void CVerticalBoreScannerPage::OnButtonStart() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);
	CString Buff,Error,Buff1;
	int nOldFastAxis;

	if(	theApp.m_Thread.m_nThreadAction != CONTINUOUS_TURNTABLE_SCAN) {

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
		PROFILE->m_nScanSide = 0;
		PROFILE->m_nScanStartLine = 0;
		PROFILE->m_nScanFinishLine = PROFILE->m_ScanLine[0].m_nCoordL - 2;

		pFrame->StartScan(CONTINUOUS_TURNTABLE_SCAN);

		m_StartTime = CTime::GetCurrentTime();

	} else {
		theApp.m_Thread.m_bAbortFlag = TRUE;
	}
}

void CVerticalBoreScannerPage::OnFileOpen() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	if(PROFILE->CheckNewProfileAllowed()!=TRUE) return;

	CFileThumbNailDlg FileDlg(TRUE,_T(".pro"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Profile (*.pro)|*.pro|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_ProfilePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_ProfilePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			CWaitCursor Wait;
			PROFILE->Retrieve(&File);
			if(theApp.m_LastSettings.nLoadUltrasonicsFromProfile==1) {
				PROFILE->CopyLocalUtToGlobal();
			}
			File.Close();
			Wait.Restore();
			pFrame->SendMessage(UI_PROFILE_OPENED, (WPARAM)this, NULL);
			theApp.ActivityLog(L"Profile loaded : " + FilePath);

			UpdateAllControls();
		}
	}
}

void CVerticalBoreScannerPage::OnFileSaveas() 
{
	
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = PROFILE->m_FileName;


	if((nPos = PROFILE->m_FileName.ReverseFind('.')) >0) {
		InitialFile = PROFILE->m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,_T(".pro"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Profile (*.pro)|*.pro|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_ProfilePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_ProfilePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			PROFILE->Save(&File);
			File.Close();
			theApp.ActivityLog(L"Profile saved : " + FilePath);
		}
	}
}

void CVerticalBoreScannerPage::OnButtonZeroRotate() 
{
	

	theApp.m_Motors.ZeroAxis(theApp.m_Tank.nRLeft);
	
}

void CVerticalBoreScannerPage::OnChangeEditDbRange() 
{
	
	CString Buff,strValue;
	WCHAR ch;
	int nn;
	float fTemp0,fTemp1;

	m_editDBRange.GetWindowText(Buff);

	strValue.Empty();
	for(int ii=0;ii<Buff.GetLength();ii++) {
		ch = Buff.GetAt(ii);
		if(iswdigit(ch) || ch == '.' || ch == '-' || ch == ' ' ) {
			strValue += ch;
		}
	}
	if(strValue.GetLength()>0) {
		nn=swscanf_s(strValue,L"%f %f",&fTemp0,&fTemp1);
		if(nn == 2) {
			PROFILE->m_BoreScanHeader.fdBTop = fTemp0;
			PROFILE->m_BoreScanHeader.fdBBottom = fTemp1;
		}
	};
}

void CVerticalBoreScannerPage::OnChangeEditEvalZone() 
{
	
	CString Buff,strValue;
	WCHAR ch;
	int nn;
	float fTemp0,fTemp1;

	m_editEvalZone.GetWindowText(Buff);

	strValue.Empty();
	for(int ii=0;ii<Buff.GetLength();ii++) {
		ch = Buff.GetAt(ii);
		if(iswdigit(ch) || ch == '.' || ch == '-' || ch == ' ' ) {
			strValue += ch;
		}
	}
	if(strValue.GetLength()>0) {
		nn=swscanf_s(strValue,L"%f %f",&fTemp0,&fTemp1);
		if(nn == 2) {
			PROFILE->m_BoreScanHeader.fEvalZTop = fTemp0;
			PROFILE->m_BoreScanHeader.fEvalZBottom = fTemp1;
		}
	};
}

void CVerticalBoreScannerPage::OnChangeEditRefRange() 
{
	
	CString Buff,strValue;
	WCHAR ch;
	int nn;
	float fTemp0,fTemp1;

	m_editRefZone.GetWindowText(Buff);

	strValue.Empty();
	for(int ii=0;ii<Buff.GetLength();ii++) {
		ch = Buff.GetAt(ii);
		if(iswdigit(ch) || ch == '.' || ch == '-' || ch == ' ' ) {
			strValue += ch;
		}
	}
	if(strValue.GetLength()>0) {
		nn=swscanf_s(strValue,L"%f %f",&fTemp0,&fTemp1);
		if(nn == 2) {
			PROFILE->m_BoreScanHeader.fRefZTop = fTemp0;
			PROFILE->m_BoreScanHeader.fRefZBottom = fTemp1;
		}
	};
}

void CVerticalBoreScannerPage::OnButtonDbRange() 
{
	

	PROFILE->m_BoreScanHeader.fdBTop = theApp.m_LastSettings.fHistogramRelativedB[0];
	PROFILE->m_BoreScanHeader.fdBBottom = theApp.m_LastSettings.fHistogramRelativedB[1];
	UpdateAllControls();
}

void CVerticalBoreScannerPage::OnButtonEvalZone() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	float fTemp;
	CData *pData;

	if(pFrame->m_pViewSheet[0]) {
		pData = &pFrame->m_pViewSheet[0]->m_Data;
		PROFILE->m_BoreScanHeader.fEvalZTop = (float)pData->m_Hist.rr.top * pData->m_fSlowIncrement;
		PROFILE->m_BoreScanHeader.fEvalZBottom = (float)pData->m_Hist.rr.bottom * pData->m_fSlowIncrement;
		if(PROFILE->m_BoreScanHeader.fEvalZTop < PROFILE->m_BoreScanHeader.fEvalZBottom) {
			fTemp = PROFILE->m_BoreScanHeader.fEvalZBottom;
			PROFILE->m_BoreScanHeader.fEvalZBottom = PROFILE->m_BoreScanHeader.fEvalZTop;
			PROFILE->m_BoreScanHeader.fEvalZTop = fTemp;
		}
		UpdateAllControls();
	};

}

void CVerticalBoreScannerPage::OnButtonRefZone() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	float fTemp;
	CData *pData;

	if(pFrame->m_pViewSheet[0]) {
		pData = &pFrame->m_pViewSheet[0]->m_Data;
		PROFILE->m_BoreScanHeader.fRefZTop = (float)pData->m_Hist.rr.top * pData->m_fSlowIncrement;
		PROFILE->m_BoreScanHeader.fRefZBottom = (float)pData->m_Hist.rr.bottom * pData->m_fSlowIncrement;
		if(PROFILE->m_BoreScanHeader.fRefZTop < PROFILE->m_BoreScanHeader.fRefZBottom) {
			fTemp = PROFILE->m_BoreScanHeader.fRefZBottom;
			PROFILE->m_BoreScanHeader.fRefZBottom = PROFILE->m_BoreScanHeader.fRefZTop;
			PROFILE->m_BoreScanHeader.fRefZTop = fTemp;
		}
		UpdateAllControls();
	};
}

void CVerticalBoreScannerPage::OnButtonSentence() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CData *pData;
	CRect rr;
	CString Buff;
	float fPixelSize;
	int	nMaxNumberOfDefects;

	if(pFrame->m_pViewSheet[0]) {

		CViewPage* pPage=(CViewPage*)pFrame->m_pViewSheet[0]->GetActivePage();
		int nImageType = pPage->m_nImageType;
		int	nImageNumber = pPage->m_nImageNumber;
		int	nPaletteNumber = pPage->m_nPaletteNumber;

		if(nImageType != 0) {
			MessageBox(L"Current view is not an amplitude C-scan",L"Error",MB_ICONERROR);
		} else {
			pData = &pFrame->m_pViewSheet[0]->m_Data;
		
			rr.left = 0;
			rr.right = pData->m_nSamplesLine-1;
			rr.top = (int)(PROFILE->m_BoreScanHeader.fRefZTop / pData->m_fSlowIncrement);
			rr.bottom = (int)(PROFILE->m_BoreScanHeader.fRefZBottom / pData->m_fSlowIncrement);
			MinMax(&rr.top,0,pData->m_nNumberLines-1);
			MinMax(&rr.bottom,0,pData->m_nNumberLines-1);

			theApp.m_bCtrlButton = true;
			pData->BuildHistogramData(nImageType,nImageNumber,rr);
			theApp.m_bCtrlButton = false;

			rr.top = (int)(PROFILE->m_BoreScanHeader.fEvalZTop / pData->m_fSlowIncrement);
			rr.bottom = (int)(PROFILE->m_BoreScanHeader.fEvalZBottom / pData->m_fSlowIncrement);
			MinMax(&rr.top,0,pData->m_nNumberLines-1);
			MinMax(&rr.bottom,0,pData->m_nNumberLines-1);

			pData->BuildHistogramData(nImageType,nImageNumber,rr);

			pPage->m_bHistogramPalette = true;
			pFrame->m_pViewSheet[0]->InvalidateDueToHistogramChange();

			fPixelSize = pData->m_fFastIncrement * pData->m_fSlowIncrement;
			nMaxNumberOfDefects = (int)(PROFILE->m_BoreScanHeader.fDefectArea / fPixelSize);
			if(pData->m_Hist.nTotalBelowThreshold[0] > nMaxNumberOfDefects) {
				Buff.Format(L"Failed Area = %.0fmm\x0b2", (float)pData->m_Hist.nTotalBelowThreshold[0] * fPixelSize);
				pData->m_nSentence = 2;
			} else {
				Buff = "Passed";
				pData->m_nSentence = 1;
			}
			m_editSentence.SetWindowText(Buff);
		}

	}
}

void CVerticalBoreScannerPage::OnChangeEditTotalDefectArea() 
{
	
	CString Buff;

	m_editTotalDefectArea.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_BoreScanHeader.fDefectArea);
}
