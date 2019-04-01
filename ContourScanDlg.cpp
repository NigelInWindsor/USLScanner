// ContourScanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ContourScanDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContourScanPage CPropertyPage
IMPLEMENT_DYNCREATE(CContourScanPage, CPropertyPage)

CContourScanPage::CContourScanPage() : CPropertyPage(CContourScanPage::IDD)
{
	//{{AFX_DATA_INIT(CContourScanPage)
	//}}AFX_DATA_INIT
	
	m_nOldScanLine=-1;
}

CContourScanPage::~CContourScanPage()
{
}


void CContourScanPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContourScanPage)
	DDX_Control(pDX, IDC_COMBO_BI_UNI, m_comboBiUinDirectional);
	DDX_Control(pDX, IDC_CHECK_REVERSE_SCAN_LINES, m_checkReverseScanLines);
	DDX_Control(pDX, IDC_COMBO_LINE_INDEX_MODE, m_comboLineIndexMode);
	DDX_Control(pDX, IDC_COMBO_LINE_INDEX, m_comboLineIndex);
	DDX_Control(pDX, IDC_SPIN_SCAN_SPEED, m_spinScanSpeed);
	DDX_Control(pDX, IDC_EDIT_SCAN_SPEED, m_editScanSpeed);
	DDX_Control(pDX, IDC_CHECK_USE_BRAIN, m_checkUseBrain);
	DDX_Control(pDX, IDC_EDIT_RESUME_LINE, m_editResumeLine);
	DDX_Control(pDX, IDC_EDIT_SCAN_LINE, m_editScanLine);
	DDX_Control(pDX, IDC_EDIT_START_LINE, m_editStartLine);
	DDX_Control(pDX, IDC_EDIT_FINISH_LINE, m_editFinishLine);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_editFilePath);
	DDX_Control(pDX, IDC_COMBO_SCAN_TYPE, m_comboScanType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContourScanPage, CPropertyPage)
	//{{AFX_MSG_MAP(CContourScanPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_ABORT, OnButtonAbort)
	ON_BN_CLICKED(IDC_BUTTON_MOVETO_START, OnButtonMovetoStart)
	ON_EN_CHANGE(IDC_EDIT_FILEPATH, OnChangeEditFilepath)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_FINISH_LINE, OnChangeEditFinishLine)
	ON_EN_CHANGE(IDC_EDIT_START_LINE, OnChangeEditStartLine)
	ON_CBN_SELCHANGE(IDC_COMBO_SCAN_TYPE, OnSelchangeComboScanType)
	ON_BN_CLICKED(IDC_BUTTON_START_LINE, OnButtonStartLine)
	ON_BN_CLICKED(IDC_BUTTON_FINISH_LINE, OnButtonFinishLine)
	ON_EN_CHANGE(IDC_EDIT_RESUME_LINE, OnChangeEditResumeLine)
	ON_BN_CLICKED(IDC_CHECK_USE_BRAIN, OnCheckUseBrain)
	ON_EN_CHANGE(IDC_EDIT_SCAN_SPEED, OnChangeEditScanSpeed)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_INDEX_MODE, OnSelchangeComboLineIndexMode)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_INDEX, OnSelchangeComboLineIndex)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCAN_SPEED, OnDeltaposSpinScanSpeed)
	ON_BN_CLICKED(IDC_CHECK_REVERSE_SCAN_LINES, OnCheckReverseScanLines)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_BI_UNI, OnSelchangeComboBiUni)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContourScanPage message handlers


BOOL CContourScanPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString	Buff;

	switch(theApp.m_nSide0Orientation) {
	case 0:
		m_comboScanType.AddString(theApp.m_DlgSideName[0]);
		m_comboScanType.AddString(theApp.m_DlgSideName[1]);
		Buff.LoadString(IDS_Both); m_comboScanType.AddString(Buff);
		break;
	case 1:
		m_comboScanType.AddString(theApp.m_DlgSideName[1]);
		m_comboScanType.AddString(theApp.m_DlgSideName[0]);
		Buff.LoadString(IDS_Both); m_comboScanType.AddString(Buff);
		break;
	}

//	m_comboScanWhat.AddString(_T("Component"));
//	m_comboScanWhat.AddString(_T("Test Piece"));
	
	m_editFilePath.SetWindowText(theApp.m_Data.m_FileName);
	m_spinScanSpeed.SetRange(1,1000);
	Buff.LoadString(IDS_Fixed_Index_Below);	m_comboLineIndexMode.AddString(Buff);
	Buff.LoadString(IDS_Variable_Index);	m_comboLineIndexMode.AddString(Buff);

	Buff.LoadString(IDS_Bidirectional);		m_comboBiUinDirectional.AddString(Buff);
	Buff.LoadString(IDS_Unidirectional);	m_comboBiUinDirectional.AddString(Buff);

	
	UpdateAllControls();

	SetTimer(1,1000,NULL);

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CContourScanPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}


void CContourScanPage::UpdateAllControls()
{
	CString	Buff;

	if (GetSafeHwnd() == NULL) return;

//	PROFILE->m_nLineIncrement = 1;

	m_comboScanType.SetCurSel(PROFILE->m_nScanSide);
//	m_comboScanWhat.SetCurSel(theApp.m_nScanWhat);

	m_editFilePath.SetWindowText(theApp.m_Data.m_FileName);

	Buff.Format(_T("%d"),PROFILE->m_nScanStartLine+1);
	m_editStartLine.SetWindowText(Buff);
	Buff.Format(_T("%d"),PROFILE->m_nScanFinishLine+1);
	m_editFinishLine.SetWindowText(Buff);
	Buff.Format(_T("%d"),PROFILE->m_nScanResumeLine+1);
	m_editResumeLine.SetWindowText(Buff);

	m_comboLineIndex.ResetContent();
	if(PROFILE->m_nLineIncrement<=0) PROFILE->m_nLineIncrement=1;
	for(int ii=1;ii<10;ii++) {
		Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,(float)ii * PROFILE->m_fSlowIncrement * theApp.m_fUnits,theApp.m_Units);
		m_comboLineIndex.AddString(Buff);
	}
	m_comboLineIndex.SetCurSel(PROFILE->m_nLineIncrement - 1);
	m_comboLineIndexMode.SetCurSel(PROFILE->m_nLineIncrementMode);


	if(PROFILE->m_nBrainLineL > 0) {
		m_checkUseBrain.SetCheck(theApp.m_LastSettings.bUseBrainCompensation & 1);
	} else {
		m_checkUseBrain.SetCheck(theApp.m_LastSettings.bUseBrainCompensation = FALSE);
	}

	if(theApp.m_nUnitsPrecision<1) theApp.m_nUnitsPrecision = 1;
	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,PROFILE->m_fScanSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editScanSpeed.SetWindowText(Buff);

	m_checkReverseScanLines.SetCheck(theApp.m_LastSettings.nReverseScanLines &= 1);

	m_comboBiUinDirectional.SetCurSel( PROFILE->m_nBiUniDirectionalScan );

	SetAccessPrivelage();

}


void CContourScanPage::OnChangeEditFilepath() 
{
	m_editFilePath.GetWindowText(theApp.m_Data.m_FileName);
	theApp.m_Data.m_FilePath = theApp.m_StorePath[0] + L"\\" + theApp.m_Data.m_FileName + L".dat";
}


void CContourScanPage::OnButtonBrowse() 
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



void CContourScanPage::OnButtonAbort() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);

	theApp.m_Thread.m_bAbortFlag = TRUE;
	theApp.ActivityLog(L"Scan aborted");
	
}

void CContourScanPage::OnButtonMovetoStart() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);
	CCoord	CpSurface,CpHead,Cp;

	PROFILE->m_nScanSide = m_comboScanType.GetCurSel();
	theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction=MOVETO_CONTOUR_START;

	/*
	theApp.m_Motors.GetHeadPos(&Cp);

		CpSurface = PROFILE->m_ScanLine[PROFILE->m_nScanStartLine].m_pCp[0];
		theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);
//		PROFILE->CalculateBrainCoord(&CpSurface);

		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
//		if(theApp.m_LastSettings.nBrainOffOn == 0) 
//			theApp.m_Kinematics.Compensate(&CpHead);

		switch(PROFILE->m_nScanSide) {
		case 0:	theApp.m_Thread.m_nMoveSide = 1;
			CpHead.Side1.fX=Cp.Side1.fX;
			CpHead.Side1.fY=Cp.Side1.fY;
			CpHead.Side1.fZ=Cp.Side1.fZ;
			CpHead.Side1.fXt=Cp.Side1.fXt;
			CpHead.Side1.fYt=Cp.Side1.fYt;
			break;
		case 1:	theApp.m_Thread.m_nMoveSide = 2;
			CpHead.Side0.fX=Cp.Side0.fX;
			CpHead.Side0.fY=Cp.Side0.fY;
			CpHead.Side0.fZ=Cp.Side0.fZ;
			CpHead.Side0.fXt=Cp.Side0.fXt;
			CpHead.Side0.fYt=Cp.Side0.fYt;
			break;
		case 2:	theApp.m_Thread.m_nMoveSide = 3;
			break;
		}
		theApp.m_Thread.m_CpMoveTo = CpHead;
		theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
		*/
}


void CContourScanPage::OnButtonStart() 
{
	FRAME;
	CString Buff, Buff1,Error;

	if(PROFILE->m_fScanSpeed < 10.0f) {
		if(MessageBox(L"The scan speed seems very low\r\nDo you wish to continue at this speed?",L"Warning",MB_ICONWARNING|MB_YESNO)==IDNO) {
			return;
		}
	}

	if(pFrame->IsScanImageStillOpen() == FALSE) {
		if(theApp.m_Data.IsNameRequired() == TRUE) {
			Buff.LoadString(IDS_Please_enter_a_scan_name);
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
		theApp.m_nScanWhat = 0;
		PROFILE->setRUsed(false);
		PROFILE->m_nCollectAxis = PROFILE->m_nFastAxis;
	}
	PROFILE->m_nScanSide = m_comboScanType.GetCurSel();
	theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide;


	switch(theApp.m_Tank.nScannerDescription) {
	default:
		pFrame->StartScan(CONTOUR_SCAN);
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TANK_5AXIS:
	case TANK_6AXIS:
	case TANK_2AXIS:
	case TWIN_TOWER_KINEMATIC:
		if(theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			pFrame->StartScan(ROBOT_ROLLER_PROBE_SCAN);
		} else {
			pFrame->StartScan(ROBOT_CONTOUR_SCAN);
		}
		break;
	}

}

void CContourScanPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;

	if(m_nOldScanLine-theApp.m_Thread.m_nScanLine) {
		Buff.Format(_T("%d"),(m_nOldScanLine=theApp.m_Thread.m_nScanLine) + 1);
		m_editScanLine.SetWindowText(Buff);
	}

	CPropertyPage::OnTimer(nIDEvent);
}


void CContourScanPage::OnChangeEditFinishLine() 
{
	CString	Buff;
	int nTemp = PROFILE->m_nScanFinishLine;

	m_editFinishLine.GetWindowText(Buff);
	PROFILE->m_nScanFinishLine = _ttoi(Buff) -1;
	if(PROFILE->m_nScanFinishLine<0) {
		PROFILE->m_nScanFinishLine = 0;
	}
	if(PROFILE->m_nComponentDescription == RECTILINEAR) {
		if(PROFILE->m_nScanFinishLine>=PROFILE->m_nScanLineL) {
			PROFILE->m_nScanFinishLine = PROFILE->m_nScanLineL - 1;
		}
	}
	if(nTemp - PROFILE->m_nScanFinishLine) {
		UpdateInformation();
	}
	
}

void CContourScanPage::OnChangeEditStartLine() 
{
	CString	Buff;
	int nTemp = PROFILE->m_nScanStartLine;

	m_editStartLine.GetWindowText(Buff);
	PROFILE->m_nScanStartLine = _ttoi(Buff) -1;
	if(PROFILE->m_nScanStartLine<0) {
		PROFILE->m_nScanStartLine = 0;
	}
	if(PROFILE->m_nScanStartLine>=PROFILE->m_nScanLineL) {
		PROFILE->m_nScanStartLine = PROFILE->m_nScanLineL;
	}
	if(nTemp - PROFILE->m_nScanStartLine) {
		UpdateInformation();
	}
}

void CContourScanPage::OnSelchangeComboScanType() 
{
	PROFILE->m_nScanSide = m_comboScanType.GetCurSel();
}


void CContourScanPage::OnButtonStartLine() 
{
	CString	Buff;

	PROFILE->m_nScanStartLine = 0;
	Buff.Format(_T("%d"),PROFILE->m_nScanStartLine+1);
	m_editStartLine.SetWindowText(Buff);
	UpdateInformation();
}

void CContourScanPage::OnButtonFinishLine() 
{
	CString	Buff;

	PROFILE->m_nScanFinishLine = PROFILE->m_nScanLineL-1;
	Buff.Format(_T("%d"),PROFILE->m_nScanFinishLine+1);
	m_editFinishLine.SetWindowText(Buff);
	UpdateInformation();

}


void CContourScanPage::SetAccessPrivelage()
{
	bool bEnableFlag;
	CButton *pButton;

	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDD_CONTOUR_SCAN_DIALOG] ? bEnableFlag = TRUE : bEnableFlag = FALSE;

	if(PROFILE->m_nLineIncrementMode == 0) {
		m_comboLineIndex.EnableWindow(TRUE);
	} else {
		m_comboLineIndex.EnableWindow(FALSE);
	}

	m_comboLineIndexMode.EnableWindow(bEnableFlag);
	m_comboLineIndex.EnableWindow(bEnableFlag);
	m_spinScanSpeed.EnableWindow(bEnableFlag);
	m_editScanSpeed.EnableWindow(bEnableFlag);
	m_checkUseBrain.EnableWindow(bEnableFlag);
	m_editResumeLine.EnableWindow(bEnableFlag);
	m_editStartLine.EnableWindow(bEnableFlag);
	m_editFinishLine.EnableWindow(bEnableFlag);
	m_editFilePath.EnableWindow(bEnableFlag);
	m_comboScanType.EnableWindow(bEnableFlag);

	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_BROWSE)) pButton->EnableWindow(bEnableFlag);
	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_START)) pButton->EnableWindow(bEnableFlag);
	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_ABORT)) pButton->EnableWindow(bEnableFlag);
	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_MOVETO_START)) pButton->EnableWindow(bEnableFlag);
	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_START_LINE)) pButton->EnableWindow(bEnableFlag);
	if(pButton = (CButton*)GetDlgItem(IDC_BUTTON_FINISH_LINE)) pButton->EnableWindow(bEnableFlag);

}




void CContourScanPage::OnChangeEditResumeLine() 
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



BOOL CContourScanPage::OnSetActive() 
{
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	return CPropertyPage::OnSetActive();
}

void CContourScanPage::OnCheckUseBrain() 
{
	if(PROFILE->m_nBrainLineL > 0) {
		theApp.m_LastSettings.bUseBrainCompensation = m_checkUseBrain.GetCheck() & 1;
	} else {
		m_checkUseBrain.SetCheck(theApp.m_LastSettings.bUseBrainCompensation = FALSE);
		MessageBox(_T("There is no acceptable brain defined"),_T("Brain Error"),NULL);
	}
}

void CContourScanPage::OnChangeEditScanSpeed() 
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
		UpdateInformation();
	}

}

void CContourScanPage::OnDeltaposSpinScanSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta * MinMax(&theApp.m_LastSettings.fDeltaScanSpeed, 1, 100));
	if(PROFILE->m_fScanSpeed<1.0f) PROFILE->m_fScanSpeed = 1.0f;
	if(PROFILE->m_fScanSpeed>1000.0f) PROFILE->m_fScanSpeed = 1000.0f;
	UpdateAllControls();
	
	*pResult = 0;
}


void CContourScanPage::UpdateInformation()
{
	return;
	CString Buff;
	int gg,nTS,nNumberAmpImages,nNumberTimeImages,nNumberBScans;
	DWORD nMemReq=0;
	DWORD nSize;
	int	nSamplesLine=0,nNumberLines=0;
	MEMORYSTATUS mem;
	int	nAscanLength=0;


	if(theApp.m_Thread.m_nThreadAction!=CONTOUR_SCAN) {
		//Amount of memory required
		nNumberAmpImages=0;
		nNumberTimeImages=0;
		nNumberBScans=0;
		for(nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
			for(gg=0;gg<8;gg++) {
				if(theApp.m_UtUser.m_TS[nTS].Gate.nCollectAmp[gg]>0) nNumberAmpImages++;
				if(theApp.m_UtUser.m_TS[nTS].Gate.nCollectTime[gg]>0) nNumberTimeImages++;
			}
			if(theApp.m_UtUser.m_TS[nTS].Adc.nBscanMode) {
				nAscanLength+=theApp.m_UtUser.m_TS[nTS].Adc.nBscanLength;
				nNumberBScans++;
			}
		}
		PROFILE->FindMaximumSamplesLine(&nSamplesLine);
		PROFILE->FindNumberScanLines(&nNumberLines);
		nSize = (DWORD)nSamplesLine * (DWORD)nNumberLines;
		nMemReq += (nSamplesLine * sizeof CCoord);
		nMemReq += (nSize * (DWORD)nNumberAmpImages);
		nMemReq += (nSize * (DWORD)nNumberTimeImages * 4);
		nMemReq += (DWORD)(nSamplesLine * nAscanLength);

		GlobalMemoryStatus(&mem);

		Buff.Format(_T("Req:%.01f MByte Avail:%.01f MByte"),(float)nMemReq / 1048576.0f,(float)mem.dwAvailPhys / 1048576.0f);
		m_editMemory.SetWindowText(Buff);
		//Scan time
		//This is extremely nieve

		if(PROFILE->m_fScanSpeed>0) {
			float fScanLength = (float)nSamplesLine * PROFILE->m_fFastIncrement;
			float fTimePerLine = (fScanLength / PROFILE->m_fScanSpeed) * 1.5f; //As I said very nieve
			int nTotalTime = (int)(fTimePerLine * (float)nNumberLines);
			int nHours = nTotalTime / 3600;
			int nMinutes = (nTotalTime % 3600) / 60;
			Buff.Format(_T("%d hrs: %d min"),nHours,nMinutes);
			m_editScantime.SetWindowText(Buff);
		}

		CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
		pFrame->SendMessage(UI_UPDATE_PROFILE_DLG, NULL,NULL);

	}

}


void CContourScanPage::OnSelchangeComboLineIndexMode() 
{
	PROFILE->m_nLineIncrementMode = m_comboLineIndexMode.GetCurSel();
	SetAccessPrivelage();
}

void CContourScanPage::OnSelchangeComboLineIndex() 
{
	PROFILE->m_nLineIncrement = m_comboLineIndex.GetCurSel() + 1;
}



void CContourScanPage::OnCheckReverseScanLines() 
{
	theApp.m_LastSettings.nReverseScanLines = m_checkReverseScanLines.GetCheck() & 1;
}

void CContourScanPage::OnSelchangeComboBiUni() 
{
	PROFILE->m_nBiUniDirectionalScan = m_comboBiUinDirectional.GetCurSel();
}
