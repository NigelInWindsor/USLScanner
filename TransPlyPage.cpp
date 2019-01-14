// TransPlyPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "TransPlyPage.h"
#include "MainFrm.h"
#include "FileThumbNailDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransPlyPage property page

IMPLEMENT_DYNCREATE(CTransPlyPage, CPropertyPage)

CTransPlyPage::CTransPlyPage() : CPropertyPage(CTransPlyPage::IDD)
{
	//{{AFX_DATA_INIT(CTransPlyPage)
	//}}AFX_DATA_INIT
	
	m_bUseCalPath[0] = m_bUseCalPath[1] = false;
}

CTransPlyPage::~CTransPlyPage()
{
}

void CTransPlyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransPlyPage)
	DDX_Control(pDX, IDC_EDIT_GAIN2, m_editGain2);
	DDX_Control(pDX, IDC_EDIT_GAIN, m_editGain1);
	DDX_Control(pDX, IDC_SPIN_GAIN2, m_spinGain2);
	DDX_Control(pDX, IDC_SPIN_GAIN, m_spinGain1);
	DDX_Control(pDX, IDC_SPIN_PULSE_WIDTH2, m_spinPulseWidth2);
	DDX_Control(pDX, IDC_SPIN_PULSE_WIDTH, m_spinPulseWidth1);
	DDX_Control(pDX, IDC_EDIT_PULSE_WIDTH2, m_editPulseWidth2);
	DDX_Control(pDX, IDC_EDIT_PULSE_WIDTH, m_editPulseWidth1);
	DDX_Control(pDX, IDC_COMBO_SENTENCE_1, m_comboSentence1);
	DDX_Control(pDX, IDC_COMBO_SENTENCE_0, m_comboSentence0);
	DDX_Control(pDX, IDC_EDIT_UNIQUE_ID_1, m_editUniqueID1);
	DDX_Control(pDX, IDC_EDIT_UNIQUE_ID_0, m_editUniqueID0);
	DDX_Control(pDX, IDC_EDIT_PROBE_1, m_editProbe1);
	DDX_Control(pDX, IDC_EDIT_PROBE_0, m_editProbe0);
	DDX_Control(pDX, IDC_SPIN_SPEED, m_spinScanSpeed);
	DDX_Control(pDX, IDC_EDIT_SPEED, m_editScanSpeed);
	DDX_Control(pDX, IDC_EDIT_RESUME_LINE, m_editResumeLine);
	DDX_Control(pDX, IDC_EDIT_CURRENT_LINE, m_editScanLine);
	DDX_Control(pDX, IDC_SPIN_INCREMENT_1, m_spinSlowIncrement);
	DDX_Control(pDX, IDC_SPIN_INCREMENT_0, m_spinFastIncrement);
	DDX_Control(pDX, IDC_EDIT_INCREMENT_1, m_editSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_INCREMENT_0, m_editFastIncrement);
	DDX_Control(pDX, IDC_EDIT_PT1, m_editFinishPt);
	DDX_Control(pDX, IDC_EDIT_PT_0, m_editStartPt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransPlyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTransPlyPage)
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_EDIT_PT_0, OnChangeEditPt0)
	ON_EN_CHANGE(IDC_EDIT_PT1, OnChangeEditPt1)
	ON_EN_CHANGE(IDC_EDIT_INCREMENT_0, OnChangeEditIncrement0)
	ON_EN_CHANGE(IDC_EDIT_INCREMENT_1, OnChangeEditIncrement1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_INCREMENT_0, OnDeltaposSpinIncrement0)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_INCREMENT_1, OnDeltaposSpinIncrement1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPEED, OnDeltaposSpinSpeed)
	ON_COMMAND(ID_BUTTON_STORE_START, OnButtonStoreStart)
	ON_COMMAND(ID_BUTTON_STORE_FINISH, OnButtonStoreFinish)
	ON_COMMAND(ID_BUTTON_GOTO_START, OnButtonGotoStart)
	ON_COMMAND(ID_BUTTON_GOTO_FINISH, OnButtonGotoFinish)
	ON_COMMAND(ID_BUTTON_START, OnButtonStart)
	ON_COMMAND(ID_BUTTON_ABORT, OnButtonAbort)
	ON_EN_CHANGE(IDC_EDIT_UNIQUE_ID_0, OnChangeEditUniqueId0)
	ON_EN_CHANGE(IDC_EDIT_UNIQUE_ID_1, OnChangeEditUniqueId1)
	ON_EN_CHANGE(IDC_EDIT_PROBE_0, OnChangeEditProbe0)
	ON_EN_CHANGE(IDC_EDIT_PROBE_1, OnChangeEditProbe1)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_RESUME_LINE, OnChangeEditResumeLine)
	ON_CBN_SELCHANGE(IDC_COMBO_SENTENCE_0, OnSelchangeComboSentence0)
	ON_CBN_SELCHANGE(IDC_COMBO_SENTENCE_1, OnSelchangeComboSentence1)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_COMMAND(ID_BUTTON_CAL_0, OnButtonCal0)
	ON_COMMAND(ID_BUTTON_CAL_1, OnButtonCal1)
	ON_EN_CHANGE(IDC_EDIT_GAIN, OnChangeEditGain)
	ON_EN_CHANGE(IDC_EDIT_GAIN2, OnChangeEditGain2)
	ON_EN_CHANGE(IDC_EDIT_PULSE_WIDTH, OnChangeEditPulseWidth)
	ON_EN_CHANGE(IDC_EDIT_PULSE_WIDTH2, OnChangeEditPulseWidth2)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransPlyPage message handlers

int CTransPlyPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_TRANSPLY_TOOLBAR)) {
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

BOOL CTransPlyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	UDACCEL	Accel;
	Accel.nInc=50;
	Accel.nSec=1;
	
	m_spinFastIncrement.SetRange(0,200);
	m_spinSlowIncrement.SetRange(0,200);

	m_spinScanSpeed.SetRange(1,2000);
	
	m_comboSentence0.AddString(L"");
	m_comboSentence0.AddString(L"Pass");
	m_comboSentence0.AddString(L"Fail");
	m_comboSentence0.AddString(L"Concession");
	m_comboSentence1.AddString(L"");
	m_comboSentence1.AddString(L"Pass");
	m_comboSentence1.AddString(L"Fail");
	m_comboSentence1.AddString(L"Concession");

	m_spinPulseWidth1.SetRange(0,0xfff);
	m_spinPulseWidth2.SetRange(0,0xfff);
	m_spinGain1.SetRange(0,1000);
	m_spinGain2.SetRange(0,1000);


	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTransPlyPage::OnSetActive() 
{
	
	
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	SetTimer(1,200,NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CTransPlyPage::OnKillActive() 
{
	KillTimer(1);
	return CPropertyPage::OnKillActive();
}

void CTransPlyPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	CString Buff;

	if(theApp.m_nUnitsPrecision<1) theApp.m_nUnitsPrecision = 1;


	Buff.Format(_T("X: %.1f Y: %.1f"),PROFILE->m_CpRect[0].Side[0].fX * theApp.m_fUnits,PROFILE->m_CpRect[0].Side[0].fY * theApp.m_fUnits);
	m_editStartPt.SetWindowText(Buff);

	PROFILE->m_nRectilinearScanMode = 0;

	switch(PROFILE->m_nRectilinearScanMode) {
	case 0:	
		Buff.Format(_T("X: %.1f Y: %.1f"),PROFILE->m_CpRect[1].Side[0].fX * theApp.m_fUnits,PROFILE->m_CpRect[1].Side[0].fY * theApp.m_fUnits);
		break;
	case 1:	
		Buff.Format(_T("X: %.1f Y: %.1f"),PROFILE->m_fFastScanSize * theApp.m_fUnits,PROFILE->m_fSlowScanSize * theApp.m_fUnits);
		break;
	}
	m_editFinishPt.SetWindowText(Buff);


	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fFastIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editFastIncrement.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fSlowIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editSlowIncrement.SetWindowText(Buff);


	Buff.Format(_T("%d"),PROFILE->m_nScanResumeLine+1);
	m_editResumeLine.SetWindowText(Buff);


	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,PROFILE->m_fScanSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editScanSpeed.SetWindowText(Buff);

	m_editProbe0.SetWindowText(theApp.m_ProbeSerialNumber[0]);
	m_editProbe1.SetWindowText(theApp.m_ProbeSerialNumber[1]);

	m_comboSentence0.SetCurSel(m_nSentence[0]);
	m_comboSentence1.SetCurSel(m_nSentence[1]);

	m_editUniqueID0.SetWindowText(m_FileName[0]);
	m_editUniqueID1.SetWindowText(m_FileName[1]);


	Buff.Format(_T("%d"),theApp.m_UtUser.m_TS[0].Pr30.nPulseWidth);
	m_editPulseWidth1.SetWindowText(Buff);
	Buff.Format(_T("%.01f dB"),theApp.m_UtUser.m_TS[0].Pr30.fLinGain);
	m_editGain1.SetWindowText(Buff);
	m_spinGain1.SetPos((int)(theApp.m_UtUser.m_TS[0].Pr30.fLinGain*10.0));
	m_spinPulseWidth1.SetPos(theApp.m_UtUser.m_TS[0].Pr30.nPulseWidth);

	Buff.Format(_T("%d"),theApp.m_UtUser.m_TS[1].Pr30.nPulseWidth);
	m_editPulseWidth2.SetWindowText(Buff);
	Buff.Format(_T("%.01f dB"),theApp.m_UtUser.m_TS[1].Pr30.fLinGain);
	m_editGain2.SetWindowText(Buff);
	m_spinGain2.SetPos((int)(theApp.m_UtUser.m_TS[1].Pr30.fLinGain*10.0));
	m_spinPulseWidth2.SetPos(theApp.m_UtUser.m_TS[1].Pr30.nPulseWidth);
	

	SetAccessPrivelage();
	SetToolBarCheckedState();

}

BOOL CTransPlyPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
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
	case ID_BUTTON_MODE_COORDINATES: strTipText.LoadString(IDS_Coordinates);
	   break;
	case ID_BUTTON_MODE_SIZE: strTipText.LoadString(IDS_Size);
		break;
	case ID_BUTTON_STORE_START: strTipText.LoadString(IDS_Store_Start);
		break;
	case ID_BUTTON_STORE_FINISH: strTipText.LoadString(IDS_Store_Finish);
		break;
	case ID_BUTTON_GOTO_START: strTipText.LoadString(IDS_Goto_Start);
		break;
	case ID_BUTTON_GOTO_FINISH: strTipText.LoadString(IDS_Goto_Finish);
		break;
	case ID_BUTTON_START: strTipText.LoadString(IDS_Start_Scan);
		break;
	case ID_BUTTON_ABORT: strTipText.LoadString(IDS_Pause_Scan);
		break;
	case ID_BUTTON_LEFT_PUMP: strTipText.LoadString(IDS_Pump);
		break;
	case ID_BUTTON_NOMALIZE_AMPLITUDES: strTipText.LoadString(IDS_Normalize_Amplitude);
		break;
	case ID_BUTTON_ORIGIN: strTipText.LoadString(IDS_Origin);
		break;
	case ID_BUTTON_GOTO_ZERO: strTipText.LoadString(IDS_Goto_Zero);
		break;
	case ID_FILE_OPEN: strTipText.LoadString(IDS_Open);
		break;
	case ID_FILE_SAVEAS: strTipText.LoadString(IDS_Save_As);
		break;
	case ID_BUTTON_CAL_0: strTipText.LoadString(IDS_Calibrate_sheet_1);
		break;
	case ID_BUTTON_CAL_1: strTipText.LoadString(IDS_Calibrate_sheet_2);
		break;
	case ID_BUTTON_JOYSTICK: strTipText.LoadString(IDS_Joystick);
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

void CTransPlyPage::SetAccessPrivelage()
{

}

void CTransPlyPage::SetToolBarCheckedState()
{
	
	int nStyle;


	if(theApp.m_Thread.m_nThreadAction==CONTOUR_SCAN) {
		nStyle = TBBS_DISABLED;
	} else {
		nStyle = 0;
	}
	
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_JOYSTICK),nStyle );
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_STORE_START),nStyle );
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_STORE_FINISH),nStyle );
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_GOTO_START),nStyle );
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_GOTO_FINISH),nStyle );
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_START),nStyle);
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_ABORT),nStyle  ^  TBBS_DISABLED);
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_CAL_0),nStyle );
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_BUTTON_CAL_1),nStyle );
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_FILE_OPEN),nStyle );
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_FILE_SAVEAS),nStyle );

}

void CTransPlyPage::OnChangeEditPt0() 
{
	
	CString Buff;
	float fX,fY;

	m_editStartPt.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%f Y:%f"),&fX,&fY);

	if(	PROFILE->m_CpRect[0].Side[0].fX - (fX / theApp.m_fUnits)) {
		PROFILE->m_CpRect[0].Side[1].fX = PROFILE->m_CpRect[0].Side[0].fX = fX / theApp.m_fUnits;
	}
	if(	PROFILE->m_CpRect[0].Side[0].fY - (fY / theApp.m_fUnits)) {
		PROFILE->m_CpRect[0].Side[1].fY = PROFILE->m_CpRect[0].Side[0].fY = fY / theApp.m_fUnits;
	}
}

void CTransPlyPage::OnChangeEditPt1() 
{
	
	CString Buff;
	float fX,fY;

	m_editFinishPt.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%f Y:%f"),&fX,&fY);
	fX /= theApp.m_fUnits;
	fY /= theApp.m_fUnits;

	switch(PROFILE->m_nRectilinearScanMode) {
	case 0:	
		PROFILE->m_CpRect[1].Side[0].fX = fX;
		PROFILE->m_CpRect[1].Side[0].fY = fY;
		break;
	case 1:
		PROFILE->m_CpRect[1].Side[0].fX = PROFILE->m_CpRect[0].Side[0].fX + fX;
		PROFILE->m_CpRect[1].Side[0].fY = PROFILE->m_CpRect[0].Side[0].fY + fY;
		PROFILE->m_fFastScanSize = fX;
		PROFILE->m_fSlowScanSize = fY;
		break;
	}
}

void CTransPlyPage::OnChangeEditIncrement0() 
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

void CTransPlyPage::OnChangeEditIncrement1() 
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

void CTransPlyPage::OnDeltaposSpinSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta * 50.0f);
		if(PROFILE->m_fScanSpeed<1.0f) PROFILE->m_fScanSpeed = 1.0f;
		if(PROFILE->m_fScanSpeed>1000.0f) PROFILE->m_fScanSpeed = 1000.0f;
	} else {
		PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta * 25.4f);
		if(PROFILE->m_fScanSpeed<25.4f) PROFILE->m_fScanSpeed = 25.4f;
		if(PROFILE->m_fScanSpeed>1016.0f) PROFILE->m_fScanSpeed = 1016.0f;
	}
	UpdateAllControls();

	
	*pResult = 0;
}

void CTransPlyPage::OnDeltaposSpinIncrement0(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fFastIncrement += ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		PROFILE->m_fFastIncrement += ((float)pNMUpDown->iDelta * 0.0254f);
	}
	UpdateAllControls();
	
	*pResult = 0;
}

void CTransPlyPage::OnDeltaposSpinIncrement1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * 0.0254f);
	}
	UpdateAllControls();
	
	*pResult = 0;
}

void CTransPlyPage::OnButtonStoreStart() 
{
	

	theApp.m_Motors.GetSurfacePos(&PROFILE->m_CpRect[0]);
	UpdateAllControls();
}

void CTransPlyPage::OnButtonStoreFinish() 
{
	

	theApp.m_Motors.GetSurfacePos(&PROFILE->m_CpRect[1]);
	PROFILE->m_fFastScanSize = fabsf(PROFILE->m_CpRect[1].Side0.fX - PROFILE->m_CpRect[0].Side0.fX);
	PROFILE->m_fSlowScanSize = fabsf(PROFILE->m_CpRect[1].Side0.fY - PROFILE->m_CpRect[0].Side0.fY);
	UpdateAllControls();
}

void CTransPlyPage::OnButtonGotoStart() 
{
	

	theApp.m_Motors.GotoSurfaceCoord(&PROFILE->m_CpRect[0],PROFILE->m_nScanSide);
	
}

void CTransPlyPage::OnButtonGotoFinish() 
{
	

	theApp.m_Motors.GotoSurfaceCoord(&PROFILE->m_CpRect[1],PROFILE->m_nScanSide);
}

void CTransPlyPage::OnButtonStart() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString Buff,Error;
	Error.LoadString(IDS_Error);
	int nLine,nNumber,nStartLine,nFinishLine;

	if(pFrame->IsScanImageStillOpen() == FALSE) {

		PROFILE->m_nFastAxis = 0;
		PROFILE->m_nSlowAxis = 1;
		PROFILE->m_nScanSide = 0;

		PROFILE->GenerateRectilinearScanFromCoordinates(PROFILE->m_nScanSide);
		theApp.m_Thread.m_bMoveToStartAtBeginningOfScan=TRUE;

		if(PROFILE->m_nScanLineL==0){
			Buff.LoadString(IDS_Scan_Size_Zero);
			MessageBox(Buff,Error,MB_ICONERROR);
			return;
		}

		PROFILE->m_nCollectAxis = PROFILE->m_nFastAxis;
		PROFILE->m_nScanStartLine = 0;
		
		PROFILE->m_nScanFinishLine = PROFILE->m_nScanLineL-1;

		PROFILE->m_nLineIncrement = 1;
		theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide = PORTSIDE;

		pFrame->SendMessage(UI_UPDATE_PROFILE_DLG, NULL,NULL);

		if(theApp.m_Data.IsNameRequired() == TRUE) {
			Buff.LoadString(IDS_Volume_scan_Requested);
			MessageBox(Buff,Error,MB_ICONERROR);
			return;
		}
		theApp.m_nScanWhat = 0;

	} else {

		m_editResumeLine.GetWindowText(Buff);
		nNumber = swscanf_s(Buff,_T("%d %d"),&nStartLine,&nFinishLine);
		if(nNumber == 2) {
			nStartLine = MinMax(&nStartLine,0,theApp.m_pScanData->m_nNumberLines-1);
			nFinishLine = MinMax(&nFinishLine,0,theApp.m_pScanData->m_nNumberLines-1);
			for(nLine=nStartLine;nLine<=nFinishLine;nLine++) {
				theApp.m_pScanData->m_nLineScannedStatus[nLine]=0;
			}
		}
	}
		
		
	pFrame->StartScan(CONTOUR_SCAN);
	SetToolBarCheckedState();
}

void CTransPlyPage::OnButtonAbort() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);

	theApp.m_Thread.m_bAbortFlag = TRUE;
	theApp.ActivityLog(L"Scan aborted");
	SetToolBarCheckedState();
}

void CTransPlyPage::OnChangeEditUniqueId0() 
{
	

	m_editUniqueID0.GetWindowText(m_FileName[0]);
	if(m_bUseCalPath[0] == 1) {
		theApp.m_CalibrationComponent[0] = m_FileName[0];
		theApp.SaveRegistryVariables();
	}

}

void CTransPlyPage::OnChangeEditUniqueId1() 
{
	

	m_editUniqueID1.GetWindowText(m_FileName[1]);
	if(m_bUseCalPath[1] == 1) {
		theApp.m_CalibrationComponent[1] = m_FileName[1];
		theApp.SaveRegistryVariables();
	}

}

void CTransPlyPage::OnChangeEditProbe0() 
{
	
	
	m_editProbe0.GetWindowText(theApp.m_ProbeSerialNumber[0]);
}

void CTransPlyPage::OnChangeEditProbe1() 
{
	
	
	m_editProbe1.GetWindowText(theApp.m_ProbeSerialNumber[1]);
}

void CTransPlyPage::OnFileOpen() 
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

void CTransPlyPage::OnFileSaveas() 
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

void CTransPlyPage::OnTimer(UINT nIDEvent) 
{
	
	CString Buff;


	if(m_nOldScanLine-theApp.m_Thread.m_nScanLine) {
		Buff.Format(_T("%d of %d"),(m_nOldScanLine=theApp.m_Thread.m_nScanLine) + 1, PROFILE->m_nScanFinishLine + 1);
		m_editScanLine.SetWindowText(Buff);
	}

	SetToolBarCheckedState();
	CPropertyPage::OnTimer(nIDEvent);
}

void CTransPlyPage::OnChangeEditResumeLine() 
{
	
	CString Buff;

	m_editResumeLine.GetWindowText(Buff);
	PROFILE->m_nScanResumeLine = _ttoi(Buff) -1;
	if(PROFILE->m_nScanResumeLine<PROFILE->m_nScanStartLine) {
		PROFILE->m_nScanResumeLine = PROFILE->m_nScanStartLine;
	}
	if(PROFILE->m_nScanResumeLine>=PROFILE->m_nScanFinishLine) {
		PROFILE->m_nScanResumeLine = PROFILE->m_nScanFinishLine;
	}
}

void CTransPlyPage::OnSelchangeComboSentence0() 
{
	m_nSentence[0] = m_comboSentence0.GetCurSel();
	
}

void CTransPlyPage::OnSelchangeComboSentence1() 
{
	m_nSentence[1] = m_comboSentence1.GetCurSel();
	
}


void CTransPlyPage::OnButtonCal0() 
{
	
	CTime NowTime;
	NowTime = CTime::GetCurrentTime();

	m_bUseCalPath[0]=false;
	if(m_FileName[0].IsEmpty()) {
		m_FileName[0].Format(L"%s %s",theApp.m_CalibrationComponent[0], NowTime.Format(_T("%d,%B")));
		m_bUseCalPath[0]=true;
	} else {
		if(m_FileName[0].Find(theApp.m_CalibrationComponent[0]) >= 0) {
			m_FileName[0].Format(L"%s %s",theApp.m_CalibrationComponent[1], NowTime.Format(_T("%d,%B")));
			m_bUseCalPath[0]=true;
		} else {
			if(m_FileName[0].Find(theApp.m_CalibrationComponent[1]) >= 0) {
				m_FileName[0].Empty();
			}
		}
	}

	UpdateAllControls();
}
	

void CTransPlyPage::OnButtonCal1() 
{
	
	CTime NowTime;
	NowTime = CTime::GetCurrentTime();

	m_bUseCalPath[1]=false;
	if(m_FileName[1].IsEmpty()) {
		m_FileName[1].Format(L"%s %s",theApp.m_CalibrationComponent[0], NowTime.Format(_T("%d,%B")));
		m_bUseCalPath[1]=true;
	} else {
		if(m_FileName[1].Find(theApp.m_CalibrationComponent[0]) >= 0) {
			m_FileName[1].Format(L"%s %s",theApp.m_CalibrationComponent[1], NowTime.Format(_T("%d,%B")));
			m_bUseCalPath[1]=true;
		} else {
			if(m_FileName[1].Find(theApp.m_CalibrationComponent[1]) >= 0) {
				m_FileName[1].Empty();
			}
		}
	}

	UpdateAllControls();
}


void CTransPlyPage::OnButtonSave() 
{
	
	CString FilePathName;
	CUSLFile File;
	CFileException e;
	CString Buff;

	if(theApp.m_pScanData == NULL) return;

	for(int nImage=0;nImage<2;nImage++) {
		if(m_FileName[nImage].IsEmpty() == false) {

			if(m_bUseCalPath[nImage] == false) {
				FilePathName = theApp.m_StorePath[0] + L"\\" + m_FileName[nImage] + L".dat";
			} else {
				FilePathName = theApp.m_CalibrationPathName + L"\\" + m_FileName[nImage] + L".dat";
			}
			if(!File.Open(FilePathName, CFile::modeReadWrite | CFile::typeBinary | CFile::modeCreate | CFile::modeNoTruncate, &e)) {
				File.DecodeFileException(&e,&Buff,FilePathName);
				MessageBox(Buff,_T("Error"),MB_ICONERROR);
			} else {
				theApp.m_pScanData->SaveImageNumber(&File,nImage,m_nSentence[nImage]);
				File.Close();
			}
		}
	}

	CMainFrame *pFrame =   (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(UI_CLOSE_ALL_IMAGES);

	m_nSentence[0] = m_nSentence[1] = 0;
	m_bUseCalPath[0] = m_bUseCalPath[1] = false;
	m_FileName[0].Empty();
	m_FileName[1].Empty();
	UpdateAllControls();
}

void CTransPlyPage::OnChangeEditGain() 
{
	
	CString Buff;
	float fTemp = theApp.m_UtUser.m_TS[0].Pr30.fLinGain;

	m_editGain1.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_UtUser.m_TS[0].Pr30.fLinGain);
	if(fTemp != theApp.m_UtUser.m_TS[0].Pr30.fLinGain) {
		theApp.m_UtUser.Pr30LinFloatGain(&theApp.m_UtUser.m_TS[0].Pr30);
		m_spinGain1.SetPos((int)(theApp.m_UtUser.m_TS[0].Pr30.fLinGain*10.0));
	}
	
}

void CTransPlyPage::OnChangeEditGain2() 
{
	
	CString Buff;
	float fTemp = theApp.m_UtUser.m_TS[1].Pr30.fLinGain;

	m_editGain2.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_UtUser.m_TS[1].Pr30.fLinGain);
	if(fTemp != theApp.m_UtUser.m_TS[1].Pr30.fLinGain) {
		theApp.m_UtUser.Pr30LinFloatGain(&theApp.m_UtUser.m_TS[1].Pr30);
		m_spinGain2.SetPos((int)(theApp.m_UtUser.m_TS[1].Pr30.fLinGain*10.0));
	}
}

void CTransPlyPage::OnChangeEditPulseWidth() 
{
	
	int	nWidth=theApp.m_UtUser.m_TS[0].Pr30.nPulseWidth;
	CString	Buff;

	m_editPulseWidth1.GetWindowText(Buff);
	theApp.m_UtUser.m_TS[0].Pr30.nPulseWidth=_ttoi(Buff);
	m_spinPulseWidth1.SetPos(theApp.m_UtUser.m_TS[0].Pr30.nPulseWidth);
	if(nWidth-theApp.m_UtUser.m_TS[0].Pr30.nPulseWidth) {
		theApp.m_UtUser.Pr30PulseWidth(&theApp.m_UtUser.m_TS[0].Pr30);
	}
	
}

void CTransPlyPage::OnChangeEditPulseWidth2() 
{
	
	int	nWidth=theApp.m_UtUser.m_TS[1].Pr30.nPulseWidth;
	CString	Buff;

	m_editPulseWidth2.GetWindowText(Buff);
	theApp.m_UtUser.m_TS[1].Pr30.nPulseWidth=_ttoi(Buff);
	m_spinPulseWidth2.SetPos(theApp.m_UtUser.m_TS[1].Pr30.nPulseWidth);
	if(nWidth-theApp.m_UtUser.m_TS[1].Pr30.nPulseWidth) {
		theApp.m_UtUser.Pr30PulseWidth(&theApp.m_UtUser.m_TS[1].Pr30);
	}
}

void CTransPlyPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
//	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int	nTemp;
	float fTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_PULSE_WIDTH:
		nTemp = theApp.m_UtUser.m_TS[0].Pr30.nPulseWidth;
		theApp.m_UtUser.m_TS[0].Pr30.nPulseWidth=(pSpin->GetPos()&0xffff);
		if(nTemp - theApp.m_UtUser.m_TS[0].Pr30.nPulseWidth) {
			Buff.Format(_T("%d"),theApp.m_UtUser.m_TS[0].Pr30.nPulseWidth);
			m_editPulseWidth1.SetWindowText(Buff);
			theApp.m_UtUser.Pr30PulseWidth(&theApp.m_UtUser.m_TS[0].Pr30);
		}
		break;
	case IDC_SPIN_GAIN:
		fTemp = theApp.m_UtUser.m_TS[0].Pr30.fLinGain;
		theApp.m_UtUser.m_TS[0].Pr30.fLinGain = (float)(pSpin->GetPos()&0xffff) / 10.0f;
		if(fTemp - theApp.m_UtUser.m_TS[0].Pr30.fLinGain) {
			Buff.Format(_T("%.01f dB"),theApp.m_UtUser.m_TS[0].Pr30.fLinGain);
			m_editGain1.SetWindowText(Buff);
			theApp.m_UtUser.Pr30LinFloatGain(&theApp.m_UtUser.m_TS[0].Pr30);
		}
		break;

	case IDC_SPIN_PULSE_WIDTH2:
		nTemp = theApp.m_UtUser.m_TS[1].Pr30.nPulseWidth;
		theApp.m_UtUser.m_TS[1].Pr30.nPulseWidth=(pSpin->GetPos()&0xffff);
		if(nTemp - theApp.m_UtUser.m_TS[1].Pr30.nPulseWidth) {
			Buff.Format(_T("%d"),theApp.m_UtUser.m_TS[1].Pr30.nPulseWidth);
			m_editPulseWidth2.SetWindowText(Buff);
			theApp.m_UtUser.Pr30PulseWidth(&theApp.m_UtUser.m_TS[1].Pr30);
		}
		break;
	case IDC_SPIN_GAIN2:
		fTemp = theApp.m_UtUser.m_TS[1].Pr30.fLinGain;
		theApp.m_UtUser.m_TS[1].Pr30.fLinGain = (float)(pSpin->GetPos()&0xffff) / 10.0f;
		if(fTemp - theApp.m_UtUser.m_TS[1].Pr30.fLinGain) {
			Buff.Format(_T("%.01f dB"),theApp.m_UtUser.m_TS[1].Pr30.fLinGain);
			m_editGain2.SetWindowText(Buff);
			theApp.m_UtUser.Pr30LinFloatGain(&theApp.m_UtUser.m_TS[1].Pr30);
		}
		break;
	}	
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}
