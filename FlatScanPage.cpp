// FlatScanPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "FlatScanPage.h"
#include "FileThumbNailDlg.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlatScanPage property page

IMPLEMENT_DYNCREATE(CFlatScanPage, CPropertyPage)

CFlatScanPage::CFlatScanPage() : CPropertyPage(CFlatScanPage::IDD)
{
	//{{AFX_DATA_INIT(CFlatScanPage)
	//}}AFX_DATA_INIT
	
	m_pFont = NULL;
	m_pArray = NULL;

	//This shouldnt be here
	m_bPumpStatus = false;
}

CFlatScanPage::~CFlatScanPage()
{
	SAFE_DELETE( m_pArray );
	SAFE_DELETE( m_pFont );
}

void CFlatScanPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlatScanPage)
	DDX_Control(pDX, IDC_COMBO_FIRST_PROBE, m_comboFirstProbe);
	DDX_Control(pDX, IDC_EDIT_SCAN_TIME, m_editScanTime);
	DDX_Control(pDX, IDC_STATIC_FINISH_LENGTH, m_staticFinishLength);
	DDX_Control(pDX, IDC_EDIT_CURRENT_LINE, m_editScanLine);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_EDIT_PROBE_SEPARATION, m_editProbeSeparation);
	DDX_Control(pDX, IDC_SPIN_SPEED, m_spinScanSpeed);
	DDX_Control(pDX, IDC_SPIN_INCREMENT_1, m_spinSlowIncrement);
	DDX_Control(pDX, IDC_SPIN_INCREMENT_0, m_spinFastIncrement);
	DDX_Control(pDX, IDC_EDIT_SPEED, m_editScanSpeed);
	DDX_Control(pDX, IDC_EDIT_RESUME_LINE, m_editResumeLine);
	DDX_Control(pDX, IDC_EDIT_INCREMENT_1, m_editSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_INCREMENT_0, m_editFastIncrement);
	DDX_Control(pDX, IDC_EDIT_PT_1, m_editFinishPt);
	DDX_Control(pDX, IDC_EDIT_PT_0, m_editStartPt);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_editFilePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFlatScanPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFlatScanPage)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_EN_CHANGE(IDC_EDIT_SPEED, OnChangeEditSpeed)
	ON_EN_CHANGE(IDC_EDIT_PT_0, OnChangeEditPt0)
	ON_EN_CHANGE(IDC_EDIT_PT_1, OnChangeEditPt1)
	ON_EN_CHANGE(IDC_EDIT_INCREMENT_0, OnChangeEditIncrement0)
	ON_EN_CHANGE(IDC_EDIT_INCREMENT_1, OnChangeEditIncrement1)
	ON_EN_CHANGE(IDC_EDIT_FILEPATH, OnChangeEditFilepath)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_INCREMENT_0, OnDeltaposSpinIncrement0)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_INCREMENT_1, OnDeltaposSpinIncrement1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPEED, OnDeltaposSpinSpeed)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_EN_CHANGE(IDC_EDIT_PROBE_SEPARATION, OnChangeEditProbeSeparation)
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_MODE_COORDINATES, OnButtonModeCoordinates)
	ON_COMMAND(ID_BUTTON_MODE_SIZE, OnButtonModeSize)
	ON_BN_CLICKED(IDC_BUTTON_ABORT, OnButtonAbort)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_FLAT_GOTOHERE, OnFlatGotohere)
	ON_COMMAND(ID_FLAT_GOTOSTART, OnFlatGotostart)
	ON_COMMAND(ID_FLAT_GOTOFINISH, OnFlatGotofinish)
	ON_COMMAND(ID_FLAT_STORESTART, OnFlatStorestart)
	ON_COMMAND(ID_FLAT_STOREFINISH, OnFlatStorefinish)
	ON_COMMAND(ID_BUTTON_STORE_START, OnButtonStoreStart)
	ON_COMMAND(ID_BUTTON_STORE_FINISH, OnButtonStoreFinish)
	ON_COMMAND(ID_BUTTON_LEFT_PUMP, OnButtonLeftPump)
	ON_COMMAND(ID_BUTTON_NOMALIZE_AMPLITUDES, OnButtonNomalizeAmplitudes)
	ON_EN_CHANGE(IDC_EDIT_RESUME_LINE, OnChangeEditResumeLine)
	ON_COMMAND(ID_BUTTON_GOTO_ZERO, OnButtonGotoZero)
	ON_COMMAND(ID_BUTTON_ORIGIN, OnButtonOrigin)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_BUTTON_START, OnButtonStart)
	ON_CBN_SELCHANGE(IDC_COMBO_FIRST_PROBE, OnSelchangeComboFirstProbe)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlatScanPage message handlers

int CFlatScanPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_FLAT_SCAN_TOOLBAR)) {
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

BOOL CFlatScanPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	UDACCEL	Accel;
	Accel.nInc=50;
	Accel.nSec=1;
	
	m_spinFastIncrement.SetRange(0,200);
	m_spinSlowIncrement.SetRange(0,200);

	m_spinScanSpeed.SetRange(1,2000);

	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&m_LogFont);

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFlatScanPage::OnSetActive() 
{
	UpdateAllControls();

	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	SetTimer(1,250,NULL);
	m_bInvalidateFlag = true;
	m_nOldScanLine = -1;

	return CPropertyPage::OnSetActive();
}

BOOL CFlatScanPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}


void CFlatScanPage::UpdateAllControls()
{
	if(GetSafeHwnd() == NULL) return;
	CString Buff;
	int nTS;

	if(theApp.m_nUnitsPrecision<1) theApp.m_nUnitsPrecision = 1;

	m_editFilePath.SetWindowText(theApp.m_Data.m_FileName);


	Buff.Format(_T("X: %.1f Y: %.1f"),PROFILE->m_CpRect[0].Side[0].fX * theApp.m_fUnits,PROFILE->m_CpRect[0].Side[0].fY * theApp.m_fUnits);
	m_editStartPt.SetWindowText(Buff);

	switch(PROFILE->m_nRectilinearScanMode) {
	case 0:	
		Buff.LoadString(IDS_Finish);
		m_staticFinishLength.SetWindowText(Buff);
		Buff.Format(_T("X: %.1f Y: %.1f"),PROFILE->m_CpRect[1].Side[0].fX * theApp.m_fUnits,PROFILE->m_CpRect[1].Side[0].fY * theApp.m_fUnits);
		break;
	case 1:	
		Buff.LoadString(IDS_Length);
		m_staticFinishLength.SetWindowText(Buff);
		Buff.Format(_T("X: %.1f Y: %.1f"),PROFILE->m_fFastScanSize * theApp.m_fUnits,PROFILE->m_fSlowScanSize * theApp.m_fUnits);
		break;
	}
	m_editFinishPt.SetWindowText(Buff);


	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fFastIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editFastIncrement.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fSlowIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editSlowIncrement.SetWindowText(Buff);


	Buff.Format(_T("%d %d"),PROFILE->m_nScanResumeLine+1,PROFILE->m_nScanResumeLine+1);
	m_editResumeLine.SetWindowText(Buff);


	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,PROFILE->m_fScanSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editScanSpeed.SetWindowText(Buff);

	Buff.Format(L"%.01fmm",theApp.m_LastSettings.fProbeSeparation);
	m_editProbeSeparation.SetWindowText(Buff);


	m_comboFirstProbe.ResetContent();
	for(nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		Buff.Format(L"%d",nTS + 1);
		m_comboFirstProbe.AddString(Buff);
	}
	MinMax(&PROFILE->m_nFirstProbe,0,theApp.m_UtUser.m_Global.Mux.nMaxTimeslots-1);
	m_comboFirstProbe.SetCurSel(PROFILE->m_nFirstProbe);

	SetAccessPrivelage();
	SetToolBarCheckedState();


}



void CFlatScanPage::SetAccessPrivelage()
{

}

void CFlatScanPage::OnChangeEditSpeed() 
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

void CFlatScanPage::OnChangeEditPt0() 
{
	CString Buff;
	float fX,fY,fZ,fR;

	m_editStartPt.GetWindowText(Buff);
	if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable) {
		swscanf_s(Buff,_T("X:%f Y:%f Z:%f R:%f"),&fX,&fY,&fZ,&fR);
	}else{
		swscanf_s(Buff,_T("X:%f Y:%f"),&fX,&fY);
	}
	if(	PROFILE->m_CpRect[0].Side[0].fX - (fX / theApp.m_fUnits)) {
		PROFILE->m_CpRect[0].Side[1].fX = PROFILE->m_CpRect[0].Side[0].fX = fX / theApp.m_fUnits;
	}
	if(	PROFILE->m_CpRect[0].Side[0].fY - (fY / theApp.m_fUnits)) {
		PROFILE->m_CpRect[0].Side[1].fY = PROFILE->m_CpRect[0].Side[0].fY = fY / theApp.m_fUnits;
	}
	if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable) {
		if(	PROFILE->m_CpRect[0].Side[0].fZ - (fZ / theApp.m_fUnits)) {
			PROFILE->m_CpRect[0].Side[1].fZ = PROFILE->m_CpRect[0].Side[0].fZ = fZ / theApp.m_fUnits;
		}
	}
	if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable) {
		if(	PROFILE->m_CpRect[0].Side[0].fR - (fR / theApp.m_fUnits)) {
			PROFILE->m_CpRect[0].Side[1].fR = PROFILE->m_CpRect[0].Side[0].fR = fR / theApp.m_fUnits;
		}
	}

}

void CFlatScanPage::OnChangeEditPt1() 
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

void CFlatScanPage::OnChangeEditIncrement0() 
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

void CFlatScanPage::OnChangeEditIncrement1() 
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


void CFlatScanPage::OnChangeEditFilepath() 
{
	m_editFilePath.GetWindowText(theApp.m_Data.m_FileName);
}



void CFlatScanPage::OnDeltaposSpinSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CFlatScanPage::OnDeltaposSpinIncrement0(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CFlatScanPage::OnDeltaposSpinIncrement1(NMHDR* pNMHDR, LRESULT* pResult) 
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
/*
void CFlatScanPage::OnDeltaposSpinLength0(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CFlatScanPage::OnDeltaposSpinLength1(NMHDR* pNMHDR, LRESULT* pResult) 
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
*/

void CFlatScanPage::OnButtonBrowse() 
{
	CFileDialog File(TRUE,_T(".dat"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Data (*.dat)|*.dat|All Files (*.*)|*.*||"),NULL);
	File.m_ofn.lpstrInitialDir = theApp.m_StorePath[0];
	if(File.DoModal()==IDOK) {
		m_editFilePath.SetWindowText(File.GetFileTitle());
		theApp.m_Data.m_FilePath.Format(_T("%s"),File.GetPathName());
		theApp.m_Data.m_FileName.Format(_T("%s"),File.GetFileTitle());
	};	
	
}
/*
void CFlatScanPage::OnSelchangeComboProbeCount() 
{
	PROFILE->m_nNumberProbesPerImage = m_comboProbeCount.GetCurSel() + 1;
	
}
*/
void CFlatScanPage::OnChangeEditProbeSeparation() 
{
	CString Buff;

	m_editProbeSeparation.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fProbeSeparation);
	if(theApp.m_LastSettings.fProbeSeparation<0.0f) theApp.m_LastSettings.fProbeSeparation = 0.0f;
	
}

BOOL CFlatScanPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
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
	}
//      strTipText.Format(_T("Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

void CFlatScanPage::SetToolBarCheckedState()
{
	int	nIndex;

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_MODE_COORDINATES))>=0)
		PROFILE->m_nRectilinearScanMode == 0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_MODE_SIZE))>=0)
		PROFILE->m_nRectilinearScanMode == 1 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_LEFT_PUMP))>=0)
		m_bPumpStatus == true ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

}

void CFlatScanPage::OnButtonModeCoordinates() 
{
	PROFILE->m_nRectilinearScanMode = 0;
	UpdateAllControls();
}

void CFlatScanPage::OnButtonModeSize() 
{
	PROFILE->m_nRectilinearScanMode = 1;
	UpdateAllControls();
}



void CFlatScanPage::OnPaint() 
{
	if(GetSafeHwnd() == NULL) return;

	CPaintDC dc(this); // device context for painting
	CRect rr,rect;
	CDC			dcMem;
	CBitmap		bmp;
	CString Buff;
	CCoord CpSurface;
	int nProbe;
	CPoint pt;

	static	int	nWidth = 0;
	static	int	nHeight = 0;
	static CCoord CpOld;
	static CCoord CpRect[2];
	static float fSeparation;
	static int nOldProbe;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	theApp.m_Motors.GetSurfacePos(&CpSurface);


	if(nWidth - rr.Width()) m_bInvalidateFlag = true;
	if(nHeight - rr.Height()) m_bInvalidateFlag = true;
	if(CpSurface != CpOld) m_bInvalidateFlag = true;
	if(PROFILE->m_CpRect[0] != CpRect[0]) m_bInvalidateFlag = true;
	if(PROFILE->m_CpRect[1] != CpRect[1]) m_bInvalidateFlag = true;
	if(fSeparation - theApp.m_LastSettings.fProbeSeparation) m_bInvalidateFlag = true;
	if(nOldProbe - GetCurrentProbeNumber()) m_bInvalidateFlag = true;

	if(m_bInvalidateFlag == false) return;

	CPen *penBlack = new CPen(PS_SOLID,2,RGB(0,0,0));
	CPen *penRed = new CPen(PS_SOLID,2,RGB(255,0,0));
	CPen *penBlue = new CPen(PS_SOLID,2,RGB(0,0,255));

	if((m_pArray==NULL) || (rr.Width()-nWidth) || (rr.Height()-nHeight)) {
		SAFE_DELETE( m_pArray );
		m_pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
		memset(m_pArray,0xff,rr.Width()*sizeof(COLORREF)*rr.Height());
	}




	rect.left=0;
	rect.top=0;
	rect.right = rr.Width();
	rect.bottom = rr.Height();

	bmp.CreateCompatibleBitmap(&dc,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	CFont* pOldFont = dcMem.SelectObject(m_pFont);
	dcMem.SetBkMode(TRANSPARENT);
	CPen* pOldPen = dcMem.SelectObject(penBlack);

	dcMem.FillRect(&rect,&CBrush(RGB(255,255,255)));

	float fX=0.0f,fY=0.0f;
	CSize size;


	//Draw component
	CRect rrComponent;
	WorldToClient(PROFILE->m_CpRect[0],pt);
	rrComponent.TopLeft() = pt;
	WorldToClient(PROFILE->m_CpRect[1],pt);
	rrComponent.BottomRight() = pt;
	dcMem.FillRect(&rrComponent,&CBrush(theApp.m_LastSettings.rgbDisplayScanColor));





	//Draw Graticule
	for(fX=1000.0f;fX<theApp.m_Tank.fXLength;fX+=1000.0f) {
		WorldToClient(fX,fY,pt);

		dcMem.MoveTo(pt.x,0);
		dcMem.LineTo(pt.x,3);
		Buff.Format(L"%.0f",fX);
		size = dcMem.GetTextExtent(Buff);
		dcMem.TextOut(pt.x-size.cx/2,3,Buff);
	}

	for(fY=500.0f;fY<theApp.m_Tank.fYLength;fY+=500.0f) {
		WorldToClient(fX,fY,pt);

		dcMem.MoveTo(0,pt.y);
		dcMem.LineTo(3,pt.y);
		Buff.Format(L"%.0f",fY);
		size = dcMem.GetTextExtent(Buff);
		dcMem.TextOut(3,pt.y-size.cy/2,Buff);
	}


	//Draw Probes
	for(nProbe = 0;nProbe<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nProbe++) {
		fX = CpSurface.Side0.fX;
		fY = CpSurface.Side0.fY + nProbe * theApp.m_LastSettings.fProbeSeparation;

		WorldToClient(fX,fY,pt);

		if(nProbe == GetCurrentProbeNumber()) {
			dcMem.SelectObject(penBlue);
		} else {
			dcMem.SelectObject(penBlack);
		}
		if(nProbe<PROFILE->m_nFirstProbe) {
			dcMem.SelectObject(penRed);
		}

		dcMem.MoveTo(pt.x-4,pt.y-4);
		dcMem.LineTo(pt.x+4,pt.y+4);
		dcMem.MoveTo(pt.x-4,pt.y+4);
		dcMem.LineTo(pt.x+4,pt.y-4);
	}


	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(pOldPen);
	delete penBlack;
	delete penRed;
	delete penBlue;

	nWidth = rr.Width();
	nHeight = rr.Height();
	CpOld = CpSurface;
	CpRect[0] = PROFILE->m_CpRect[0];
	CpRect[1] = PROFILE->m_CpRect[1];
	fSeparation = theApp.m_LastSettings.fProbeSeparation;
	nOldProbe = GetCurrentProbeNumber();
	m_bInvalidateFlag = false;

}

void CFlatScanPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;

	PROFILE->m_nNumberProbesPerImage = theApp.m_UtUser.m_Global.Mux.nMaxTimeslots - PROFILE->m_nFirstProbe;

	if(m_nOldScanLine-theApp.m_Thread.m_nScanLine) {
		Buff.Format(_T("%d"),(m_nOldScanLine=theApp.m_Thread.m_nScanLine) + 1);
		m_editScanLine.SetWindowText(Buff);
	}

	Invalidate(false);

	CPropertyPage::OnTimer(nIDEvent);
}

void CFlatScanPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CRect rr;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup = NULL;

	m_ptClient = point;
	ScreenToClient(&m_ptClient);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);
	
	if(rr.PtInRect(m_ptClient)) {

		menu->LoadMenu(IDR_RB_FLAT_MENU);
		CMenu* pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);
	}

	delete menu;

}

void CFlatScanPage::OnFlatGotohere() 
{
	CCoord CpSurface;

	ClientToWorld(m_ptClient, CpSurface);
	CpSurface.Side0.fY -= (theApp.m_LastSettings.fProbeSeparation * GetCurrentProbeNumber());

	theApp.m_Motors.GotoSurfaceCoord(&CpSurface,PORTSIDE);
}

void CFlatScanPage::OnFlatGotostart() 
{
	CCoord CpSurface = PROFILE->m_CpRect[0];
	CpSurface.Side0.fY -= (theApp.m_LastSettings.fProbeSeparation * GetCurrentProbeNumber());

	theApp.m_Motors.GotoSurfaceCoord(&CpSurface,PORTSIDE);
}

void CFlatScanPage::OnFlatGotofinish() 
{
	CCoord CpSurface = PROFILE->m_CpRect[1];
	CpSurface.Side0.fY -= (theApp.m_LastSettings.fProbeSeparation * GetCurrentProbeNumber());

	theApp.m_Motors.GotoSurfaceCoord(&CpSurface,PORTSIDE);
}

void CFlatScanPage::OnFlatStorestart() 
{
	theApp.m_Motors.GetSurfacePos(&PROFILE->m_CpRect[0]);
	PROFILE->m_CpRect[0].Side0.fY += (theApp.m_LastSettings.fProbeSeparation * GetCurrentProbeNumber());
	UpdateAllControls();
}

void CFlatScanPage::OnFlatStorefinish() 
{
	theApp.m_Motors.GetSurfacePos(&PROFILE->m_CpRect[1]);
	PROFILE->m_CpRect[1].Side0.fY += (theApp.m_LastSettings.fProbeSeparation * GetCurrentProbeNumber());
	UpdateAllControls();
}

void CFlatScanPage::ClientToWorld(CPoint &pt, CCoord &CpSurface)
{
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	CpSurface.Side0.fX = ((float)(pt.x-rr.left) * theApp.m_Tank.fXLength) / (float)rr.Width() - theApp.m_Tank.fXSurfaceZero;
	CpSurface.Side0.fY = ((float)(pt.y-rr.top) * theApp.m_Tank.fYLength) / (float)rr.Height() - theApp.m_Tank.fYSurfaceZero;
}

void CFlatScanPage::WorldToClient(CCoord &CpSurface, CPoint &pt)
{

	WorldToClient(CpSurface.Side0.fX,CpSurface.Side0.fY,pt);
}

void CFlatScanPage::WorldToClient(float fX, float fY, CPoint &pt)
{
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	pt.x = (int)(((fX + theApp.m_Tank.fXSurfaceZero) * (float)rr.Width()) / theApp.m_Tank.fXLength);
	pt.y = (int)(((fY + theApp.m_Tank.fYSurfaceZero) * (float)rr.Height()) / theApp.m_Tank.fYLength);

}

void CFlatScanPage::OnButtonStoreStart() 
{
	theApp.m_Motors.GetSurfacePos(&PROFILE->m_CpRect[0]);
	PROFILE->m_CpRect[0].Side0.fY += (theApp.m_LastSettings.fProbeSeparation * GetCurrentProbeNumber());
	UpdateAllControls();
}

void CFlatScanPage::OnButtonStoreFinish() 
{
	theApp.m_Motors.GetSurfacePos(&PROFILE->m_CpRect[1]);
	PROFILE->m_CpRect[1].Side0.fY += (theApp.m_LastSettings.fProbeSeparation * GetCurrentProbeNumber());
	PROFILE->m_fFastScanSize = fabsf(PROFILE->m_CpRect[1].Side0.fX - PROFILE->m_CpRect[0].Side0.fX);
	PROFILE->m_fSlowScanSize = fabsf(PROFILE->m_CpRect[1].Side0.fY - PROFILE->m_CpRect[0].Side0.fY);
	UpdateAllControls();
}

void CFlatScanPage::OnButtonLeftPump() 
{
	//	if(theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nLeftPump)==FALSE) {
	if(m_bPumpStatus == false) {
		theApp.m_Motors.PumpStart(theApp.m_Tank.nLeftPump, 0);
		theApp.m_Motors.PumpStart(theApp.m_Tank.nRightPump, 1);
		m_bPumpStatus = true;
	} else {
		theApp.m_Motors.PumpStop(theApp.m_Tank.nLeftPump, 0);
		theApp.m_Motors.PumpStop(theApp.m_Tank.nRightPump, 1);
		m_bPumpStatus = false;
	}
	UpdateAllControls();
}

int CFlatScanPage::GetCurrentProbeNumber()
{
	return theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Pr30.nMuxTx - 1;
}

void CFlatScanPage::OnButtonStart() 
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

		pFrame->SendMessage(UI_UPDATE_PROFILE_DLG, NULL, NULL);

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
		
		
	pFrame->StartScan(MULTI_PROBE_SCAN);

}


void CFlatScanPage::OnButtonAbort() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);

	theApp.m_Thread.m_bAbortFlag = TRUE;
	theApp.ActivityLog(L"Scan aborted");
}

void CFlatScanPage::OnButtonNomalizeAmplitudes() 
{
	int nTS;
	int nAverageAmplitude[8];
	int	nMaxAmplitude=-1000;
	int	nMinAmplitude=1000;
	int nMaxTS,nMinTS;

	CWaitCursor Wait;

	int nCurrentTimeSlot = theApp.m_UtUser.m_Global.nTimeSlot;

	for(nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nTS].Pr30);
		theApp.m_UtUser.m_TS[nTS].Pr30.fLogGain =  0.0f;
		theApp.m_UtUser.Pr30LogFloatGain(&theApp.m_UtUser.m_TS[nTS].Pr30);
	}

	Sleep(3000);


	for(nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		nAverageAmplitude[nTS] = theApp.m_UtUser.GetAverageAmplitude(nTS,0);
		switch(theApp.m_UtUser.m_TS[0].Adc.nRfType) {
		case 0:
		case 1:
			nAverageAmplitude[nTS] = MulDiv(nAverageAmplitude[nTS],100,128);
			break;
		case 2:
			nAverageAmplitude[nTS] = -theApp.m_LastSettings.nLogRange-MulDiv(nAverageAmplitude[nTS],-theApp.m_LastSettings.nLogRange,127);
			break;
		};
		if(nMinAmplitude > nAverageAmplitude[nTS]) {
			nMinAmplitude = nAverageAmplitude[nTS];
			nMinTS = nTS;
		}
		if(nMaxAmplitude < nAverageAmplitude[nTS]) {
			nMaxAmplitude = nAverageAmplitude[nTS];
			nMaxTS = nTS;
		}
	}

	for(nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nTS].Pr30);
		theApp.m_UtUser.m_TS[nTS].Pr30.fLogGain +=  (nMaxAmplitude - nAverageAmplitude[nTS]);
		theApp.m_UtUser.Pr30LogFloatGain(&theApp.m_UtUser.m_TS[nTS].Pr30);
	}
	theApp.m_UtUser.m_Global.nTimeSlot = nCurrentTimeSlot;
	theApp.m_UtUser.Pr30Timeslot(&theApp.m_UtUser.m_TS[nCurrentTimeSlot].Pr30);

	Wait.Restore();
}

void CFlatScanPage::OnChangeEditResumeLine() 
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

void CFlatScanPage::OnButtonGotoZero() 
{
	CCoord CpSurface;

	CpSurface.Zero();

	theApp.m_Motors.GotoSurfaceCoord(&CpSurface,PORTSIDE);
	
}

void CFlatScanPage::OnButtonOrigin() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_LEFT : theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_RIGHT;
}

BOOL CFlatScanPage::RedrawWindow(LPCRECT lpRectUpdate, CRgn *prgnUpdate, UINT flags)
{

	CPropertyPage::RedrawWindow(lpRectUpdate, prgnUpdate, flags);



	return TRUE;
}

void CFlatScanPage::Invalidate(bool bErase)
{
	CPropertyPage::Invalidate(bErase);

	
}

void CFlatScanPage::OnFileOpen() 
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
//			pFrame->SendMessage(UI_PROFILE_OPENED);
			pFrame->SendMessage(UI_PROFILE_OPENED, (WPARAM)this,NULL);
			theApp.ActivityLog(L"Profile loaded : " + FilePath);

			UpdateAllControls();
		}
	}
}

void CFlatScanPage::OnFileSaveas() 
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

void CFlatScanPage::OnSelchangeComboFirstProbe() 
{
	PROFILE->m_nFirstProbe = m_comboFirstProbe.GetCurSel();
	Invalidate(false);
}
