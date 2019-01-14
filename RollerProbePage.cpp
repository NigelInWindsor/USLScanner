// RollerProbePage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RollerProbePage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRollerProbePage property page

IMPLEMENT_DYNCREATE(CRollerProbePage, CPropertyPage)

CRollerProbePage::CRollerProbePage() : CPropertyPage(CRollerProbePage::IDD)
{
	//{{AFX_DATA_INIT(CRollerProbePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bLBDown = false;
	
	m_nIndex = 0;
	m_pEditSpinItem = NULL;
}

CRollerProbePage::~CRollerProbePage()
{
}

void CRollerProbePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRollerProbePage)
	DDX_Control(pDX, IDC_CHECK_UNIDIRECTIONAL_SCAN, m_checkUnidirectionalScan);
	DDX_Control(pDX, IDC_LIST_RANDOM_COORDS, m_listRandomCoords);
	DDX_Control(pDX, IDC_SPIN_EXTRA_ARM_LENGTH, m_spinExtraArmLength);
	DDX_Control(pDX, IDC_EDIT_EXTRA_ARM_LENGTH, m_editExtraArmLength);
	DDX_Control(pDX, IDC_CHECK_STEP_OFF, m_checkGotoWithStepOff);
	DDX_Control(pDX, IDC_SPIN_SCAN_SPEED, m_spinScanSpeed);
	DDX_Control(pDX, IDC_SPIN_MOVETO_SPEED, m_spinMoveToSpeed);
	DDX_Control(pDX, IDC_SPIN_JOG_SPEED, m_spinJogSpeed);
	DDX_Control(pDX, IDC_EDIT_SCAN_SPEED, m_editScanSpeed);
	DDX_Control(pDX, IDC_EDIT_MOVETO_SPEED, m_editMoveToSpeed);
	DDX_Control(pDX, IDC_EDIT_JOG_SPEED, m_editJogSpeed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRollerProbePage, CPropertyPage)
	//{{AFX_MSG_MAP(CRollerProbePage)
	ON_EN_CHANGE(IDC_EDIT_JOG_SPEED, OnChangeEditJogSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_JOG_SPEED, OnDeltaposSpinJogSpeed)
	ON_EN_CHANGE(IDC_EDIT_MOVETO_SPEED, OnChangeEditMovetoSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVETO_SPEED, OnDeltaposSpinMovetoSpeed)
	ON_EN_CHANGE(IDC_EDIT_SCAN_SPEED, OnChangeEditScanSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCAN_SPEED, OnDeltaposSpinScanSpeed)
	ON_BN_CLICKED(IDC_CHECK_STEP_OFF, OnCheckStepOff)
	ON_WM_CREATE()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_EXTRA_ARM_LENGTH, OnDeltaposSpinExtraArmLength)
	ON_EN_CHANGE(IDC_EDIT_EXTRA_ARM_LENGTH, OnChangeEditExtraArmLength)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RANDOM_COORDS, OnClickListRandomCoords)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_RANDOM_COORDS, OnRclickListRandomCoords)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_RANDOM_COORDS, OnGetdispinfoListRandomCoords)
	ON_COMMAND(ID_ROLLER_ADD, OnRollerAdd)
	ON_COMMAND(ID_ROLLER_DELETE, OnRollerDelete)
	ON_COMMAND(ID_ROLLER_GOTO, OnRollerGoto)
	ON_COMMAND(ID_ROLLER_MODIFY, OnRollerModify)
	ON_BN_CLICKED(IDC_CHECK_UNIDIRECTIONAL_SCAN, OnCheckUnidirectionalScan)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRollerProbePage message handlers
int CRollerProbePage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_RR_ROLLER_TOOLBAR)) {
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

BOOL CRollerProbePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_spinScanSpeed.SetRange(0,10000);
	m_spinMoveToSpeed.SetRange(0,10000);
	m_spinJogSpeed.SetRange(0,10000);
	m_spinExtraArmLength.SetRange(0,50);
	
	
	CreateColumns();
	UpdateAllControls();

	SetToolBarCheckedState();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRollerProbePage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	CString Buff;

	if(theApp.m_nUnitsPrecision<1) theApp.m_nUnitsPrecision = 1;
	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,PROFILE->m_fScanSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editScanSpeed.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,theApp.m_LastSettings.fJogSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editJogSpeed.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,theApp.m_LastSettings.fMoveSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editMoveToSpeed.SetWindowText(Buff);

	m_checkGotoWithStepOff.SetCheck( theApp.m_LastSettings.nGotoWithStepOff);

	Buff.Format(L"%d mm",theApp.m_LastSettings.nExtraArmLength);
	m_editExtraArmLength.SetWindowText(Buff);
	m_spinExtraArmLength.SetPos(theApp.m_LastSettings.nExtraArmLength);

	m_checkUnidirectionalScan.SetCheck(	PROFILE->m_nBiUniDirectionalScan);

	FillList();

}

BOOL CRollerProbePage::PreTranslateMessage(MSG* pMsg) 
{
	
	CCoord CpSurface;
	float fHAngle;
	CString Buff;
	CButton *pButton;
	CRect rr;
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	int nAxis = -1;
	float fDistance = 0.0f;
	float fXDistance = 0.0f;
	float fYDistance = 0.0f;
	float fZDistance = 0.0f;
	float fXtDistance = 0.0f;
	float fYtDistance = 0.0f;

	switch(pMsg->message) {
	case 0x100:
		switch(pMsg->wParam) {
		case CURSOR_DOWN:
			break;
		case CURSOR_UP:
			break;
		}
		break;
	case 0x200: // Mouse move
		switch(pMsg->wParam) {
		case 1:
			break;
		};
		break;
	case 0x201://LBUTTONDOWN
		if(m_bLBDown == false) {
			m_bLBDown = true;
			theApp.m_Motors.GetSurfacePos(&CpSurface);
			fHAngle = 180.0f - atan2f(CpSurface.Side0.fJ,CpSurface.Side0.fI) * RAD_TO_DEG;
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_FORWARD); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				fDistance = theApp.m_LastSettings.fXJogDistance * 1.0f * theApp.m_LastSettings.fJogSpeed;
				fXDistance = fDistance * cosf(fHAngle * DEG_TO_RAD);
				fYDistance = fDistance * sinf(fHAngle * DEG_TO_RAD * -1.0f);
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_BACK); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				fDistance = theApp.m_LastSettings.fXJogDistance * -1.0f * theApp.m_LastSettings.fJogSpeed;
				fXDistance = fDistance * cosf(fHAngle * DEG_TO_RAD);
				fYDistance = fDistance * sinf(fHAngle * DEG_TO_RAD * -1.0f);
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_LEFT); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				fDistance = theApp.m_LastSettings.fYJogDistance * 1.0f * theApp.m_LastSettings.fJogSpeed;
				fYDistance = fDistance * cosf(fHAngle * DEG_TO_RAD);
				fXDistance = fDistance * sinf(fHAngle * DEG_TO_RAD);
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_RIGHT); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				fDistance = theApp.m_LastSettings.fYJogDistance * -1.0f * theApp.m_LastSettings.fJogSpeed;
				fYDistance = fDistance * cosf(fHAngle * DEG_TO_RAD);
				fXDistance = fDistance * sinf(fHAngle * DEG_TO_RAD);
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_DOWN); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				fZDistance = fDistance = theApp.m_LastSettings.fZJogDistance * -1.0f * theApp.m_LastSettings.fJogSpeed;
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_UP); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				fZDistance = fDistance = theApp.m_LastSettings.fZJogDistance * theApp.m_LastSettings.fJogSpeed;
			}
		//TIP AXES
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_ROLL_LEFT); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				fXtDistance = fDistance = -0.1f * theApp.m_LastSettings.fJogSpeed;
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_ROLL_RIGHT); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				fXtDistance = fDistance = 0.1f * theApp.m_LastSettings.fJogSpeed;
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_ROLL_DOWN); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				fYtDistance = fDistance = -0.1f * theApp.m_LastSettings.fJogSpeed;
			}
			pButton = (CButton*)GetDlgItem(IDC_BUTTON_ROLL_UP); pButton->GetWindowRect(rr);	ScreenToClient(rr);
			if(rr.PtInRect(point)) {
				fYtDistance = fDistance = 0.1f * theApp.m_LastSettings.fJogSpeed;
			}
			

			Buff.Format(L"XJogMove=%.05f",fXDistance);				theApp.m_FBCtrl.SendStr(Buff);
			Buff.Format(L"YJogMove=%.05f",fYDistance);				theApp.m_FBCtrl.SendStr(Buff);
			Buff.Format(L"ZJogMove=%.05f",fZDistance);				theApp.m_FBCtrl.SendStr(Buff);
			Buff.Format(L"RXJogMove=%.05f",fXtDistance);				theApp.m_FBCtrl.SendStr(Buff);
			Buff.Format(L"RYJogMove=%.05f",fYtDistance);				theApp.m_FBCtrl.SendStr(Buff);

		};
		break;

	case 0x202:	//LBUTTONUP
		if(m_bLBDown == true) {
			m_bLBDown = false;
			Buff.Format(L"XJogMove=%.0f",0.0f);				theApp.m_FBCtrl.SendStr(Buff);
			Buff.Format(L"YJogMove=%.0f",0.0f);				theApp.m_FBCtrl.SendStr(Buff);
			Buff.Format(L"ZJogMove=%.0f",0.0f);				theApp.m_FBCtrl.SendStr(Buff);
			Buff.Format(L"RXJogMove=%.0f",0.0f);			theApp.m_FBCtrl.SendStr(Buff);
			Buff.Format(L"RYJogMove=%.0f",0.0f);			theApp.m_FBCtrl.SendStr(Buff);
		};

		break;
	}
		
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CRollerProbePage::OnChangeEditJogSpeed() 
{
	
	CString Buff;
	float fTemp = theApp.m_LastSettings.fJogSpeed;

	m_editJogSpeed.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	fTemp/=theApp.m_fUnits;
	MinMax(&fTemp, 1.0f, 1000.0f);

	if(theApp.m_LastSettings.fJogSpeed - fTemp) {
		theApp.m_LastSettings.fJogSpeed = fTemp;
	}
}

void CRollerProbePage::OnDeltaposSpinJogSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		theApp.m_LastSettings.fJogSpeed += ((float)pNMUpDown->iDelta);
		MinMax(&theApp.m_LastSettings.fJogSpeed, 1.0f, 1000.0f);
	} else {
		theApp.m_LastSettings.fJogSpeed += ((float)pNMUpDown->iDelta * 1.016f);
		MinMax(&theApp.m_LastSettings.fJogSpeed, 25.4f, 1016.0f);
	}
	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,theApp.m_LastSettings.fJogSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editJogSpeed.SetWindowText(Buff);

	*pResult = 0;
}

void CRollerProbePage::OnChangeEditMovetoSpeed() 
{
	
	CString Buff;
	float fTemp = theApp.m_LastSettings.fMoveSpeed;

	m_editMoveToSpeed.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	fTemp/=theApp.m_fUnits;
	MinMax(&fTemp, 1.0f, 1000.0f);

	if(theApp.m_LastSettings.fMoveSpeed - fTemp) {
		theApp.m_LastSettings.fMoveSpeed = fTemp;
	}

}

void CRollerProbePage::OnDeltaposSpinMovetoSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		theApp.m_LastSettings.fMoveSpeed += ((float)pNMUpDown->iDelta);
		MinMax(&theApp.m_LastSettings.fMoveSpeed, 1.0f, 1000.0f);
	} else {
		theApp.m_LastSettings.fMoveSpeed += ((float)pNMUpDown->iDelta * 1.016f);
		MinMax(&theApp.m_LastSettings.fMoveSpeed, 25.4f, 1016.0f);
	}
	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,theApp.m_LastSettings.fMoveSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editMoveToSpeed.SetWindowText(Buff);

	*pResult = 0;
}

void CRollerProbePage::OnChangeEditScanSpeed() 
{
	
	CString Buff;
	float fTemp = PROFILE->m_fScanSpeed;

	m_editScanSpeed.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	fTemp/=theApp.m_fUnits;
	MinMax(&fTemp, 1.0f, 1000.0f);

	if(PROFILE->m_fScanSpeed - fTemp) {
		PROFILE->m_fScanSpeed = fTemp;
	}

}

void CRollerProbePage::OnDeltaposSpinScanSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta);
		MinMax(&PROFILE->m_fScanSpeed, 1.0f, 1000.0f);
	} else {
		PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta * 1.016f);
		MinMax(&PROFILE->m_fScanSpeed, 25.4f, 1016.0f);
	}
	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,PROFILE->m_fScanSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editScanSpeed.SetWindowText(Buff);

	*pResult = 0;
}



BOOL CRollerProbePage::OnSetActive() 
{
	
	
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	SetTimer(1,500,NULL);

	return CPropertyPage::OnSetActive();
}

void CRollerProbePage::OnCheckStepOff() 
{
	
	
	theApp.m_LastSettings.nGotoWithStepOff = m_checkGotoWithStepOff.GetCheck();
}

BOOL CRollerProbePage::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
   case ID_BUTTON_PARK: strTipText.LoadString(IDS_Move_to_load_unload_position);
	   break;
   case ID_BUTTON_STEP_OFF_ON: strTipText.LoadString(IDS_Step_off_step_on);
	   break;
   case ID_BUTTON_PRE_WET_ROLLER: strTipText.LoadString(IDS_Pre_wet_roller);
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

/*
void CRollerProbePage::OnButtonAddEnvelopePt() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);

	if(PROFILE->m_nEnvelopeLineL) {
		PROFILE->m_EnvelopeLine[0].InsertCoord(CpSurface);
	} else {
		CPolyCoord CpLine;
		CpLine.AddCoord(CpSurface);
		theApp.m_nLine=PROFILE->AddLine(ENVELOPELINES, &CpLine,PROFILE->m_nSlowAxis);
	}

	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG);
	
}
*/

void CRollerProbePage::OnDeltaposSpinExtraArmLength(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	int nTemp = theApp.m_LastSettings.nExtraArmLength;
	CString Buff;

	theApp.m_LastSettings.nExtraArmLength += pNMUpDown->iDelta;
	MinMax(&theApp.m_LastSettings.nExtraArmLength,0,50);
	Buff.Format(L"%d mm",theApp.m_LastSettings.nExtraArmLength);
	m_editExtraArmLength.SetWindowText(Buff);

	if(nTemp - theApp.m_LastSettings.nExtraArmLength) {
		theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(0,TRUE);
	}
	
	*pResult = 0;
}

void CRollerProbePage::OnChangeEditExtraArmLength() 
{
	
	CString Buff;
	int nTemp = theApp.m_LastSettings.nExtraArmLength;

	m_editExtraArmLength.GetWindowText(Buff);
	theApp.m_LastSettings.nExtraArmLength = _ttoi(Buff);
	MinMax(&theApp.m_LastSettings.nExtraArmLength,0,50);

	if(nTemp - theApp.m_LastSettings.nExtraArmLength) {
		theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(0,TRUE);
	}
}


void CRollerProbePage::CreateColumns()
{
	if (GetSafeHwnd() == NULL) return;
	CRect rr;
	
	int ColumnWidth[10] = { 0,170};
	CString ColumnName[10];
	ColumnName[0]= "#";
	ColumnName[1] = "Coordinate";


	m_listRandomCoords.GetWindowRect(rr);
	ColumnWidth[1] = rr.Width() - 2;

	int nColumnCount = m_listRandomCoords.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listRandomCoords.DeleteColumn(0);
	}
	for (int ii=0;ii<2;ii++) {
		m_listRandomCoords.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}
	m_listRandomCoords.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CRollerProbePage::FillList()
{
	
	if (GetSafeHwnd() == NULL) return;
			CString Buff;

	if(theApp.m_nLadderL >= 2) {

		m_listRandomCoords.DeleteAllItems();
		for (int ii=0;ii<theApp.m_pLadder[1].GetSize();ii++) {
			Buff.Format(_T("%d"),ii+1);
			m_listRandomCoords.InsertItem(ii, Buff.GetBuffer(255), ii);
		};

		m_listRandomCoords.EnsureVisible(m_nIndex,FALSE);
		for(int ii=0;ii<theApp.m_pLadder[1].GetSize();ii++) 
			m_listRandomCoords.SetItemState(ii , 0,LVIS_SELECTED);
		m_listRandomCoords.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
		m_listRandomCoords.SetFocus();
	}

}

void CRollerProbePage::OnGetdispinfoListRandomCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nn;
	CCoord Cp;

	str[0]=0;

	if(theApp.m_nLadderL >= 2) {
		nn = pDispInfo->item.iItem;
		if((nn>=0) && (nn<theApp.m_pLadder[1].GetSize())) {
			theApp.m_pLadder[1].GetCoord(nn,&Cp);

			switch(pDispInfo->item.iSubItem) {
			case 0:
				break;
			case 1:
				swprintf_s(str,_T("%.0f %.0f %.0f"),Cp.Side0.fX,Cp.Side0.fY,Cp.Side0.fZ);
				break;
			}
		}
	}
		
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CRollerProbePage::OnClickListRandomCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CRollerProbePage::OnRclickListRandomCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listRandomCoords.GetFirstSelectedItemPosition();
	m_nIndex=m_listRandomCoords.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_ROLLER_PAGE_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	
	*pResult = 0;
}

void CRollerProbePage::ClearEditSpin()
{
	if(m_pEditSpinItem!=NULL) {
		delete m_pEditSpinItem;
		m_pEditSpinItem=NULL;
	}

}

void CRollerProbePage::OnRollerAdd() 
{
	
	CCoord CpSurface;
	CPolyCoord* pLine = NULL;

	theApp.m_Motors.GetSurfacePos(&CpSurface);

	if(theApp.m_pLadder==NULL) {
		theApp.m_pLadder = new CPolyCoord[theApp.m_nLadderL=2];
	} else {
		if(theApp.m_nLadderL<2) {
			pLine = new CPolyCoord[theApp.m_nLadderL];
			pLine[0] = &theApp.m_pLadder[0];
			SAFE_DELETE_ARRAY( theApp.m_pLadder );

			theApp.m_pLadder = new CPolyCoord[ theApp.m_nLadderL = 2];
			theApp.m_pLadder[0] = &pLine[0];
			SAFE_DELETE_ARRAY( pLine );
		}
	}
	theApp.m_pLadder[1].Add(CpSurface);

	FillList();
}

void CRollerProbePage::OnRollerDelete() 
{
	
	int nn;
	CPolyCoord *pLine = NULL;

	
	if(theApp.m_nLadderL < 2) return;
	if(m_nIndex >= theApp.m_pLadder[1].GetSize()) return;

	if(theApp.m_pLadder[1].GetSize() >= 2) {
		theApp.m_pLadder[1].DeleteCoords(m_nIndex,m_nIndex);
	} else {

		pLine = new CPolyCoord[theApp.m_nLadderL];
		pLine[0] = &theApp.m_pLadder[0];
		for(nn=1; nn<(theApp.m_nLadderL-1) ; nn++) {
			pLine[nn] = &theApp.m_pLadder[nn+1];
		}
		theApp.m_nLadderL--;
		SAFE_DELETE_ARRAY( theApp.m_pLadder );
		if(theApp.m_nLadderL) {
			theApp.m_pLadder = new CPolyCoord[theApp.m_nLadderL];
			for(nn=0;nn<theApp.m_nLadderL;nn++) {
				theApp.m_pLadder[nn] = &pLine[nn];
			}
		}
		SAFE_DELETE_ARRAY( pLine );
	}
	FillList();

}

void CRollerProbePage::OnRollerModify() 
{
	
	CCoord CpSurface;
	CPolyCoord* pLine = NULL;

	theApp.m_Motors.GetSurfacePos(&CpSurface);

	if(theApp.m_pLadder==NULL) {
		theApp.m_pLadder = new CPolyCoord[theApp.m_nLadderL=2];
	} else {
		if(theApp.m_nLadderL<2) {
			pLine = new CPolyCoord[theApp.m_nLadderL];
			pLine[0] = &theApp.m_pLadder[0];
			SAFE_DELETE_ARRAY( theApp.m_pLadder );

			theApp.m_pLadder = new CPolyCoord[ theApp.m_nLadderL = 2];
			theApp.m_pLadder[0] = &pLine[0];
			SAFE_DELETE_ARRAY( pLine );
		}
	}
	if(m_nIndex>=theApp.m_pLadder[1].GetSize()) {
		theApp.m_pLadder[1].Add(CpSurface);
	} else {
		theApp.m_pLadder[1].ModifyCoord(m_nIndex,CpSurface);
	}

	FillList();
	
}

void CRollerProbePage::OnRollerGoto() 
{
	
	CCoord CpSurface;

	if(theApp.m_nLadderL<2) return;

	if((m_nIndex>=0) && (m_nIndex<theApp.m_pLadder[1].GetSize()) ) {
		theApp.m_pLadder[1].GetCoord(m_nIndex,&CpSurface);

		theApp.m_Motors.GotoSurfaceCoord(&CpSurface,MOVE_LEFT);
	}
	
}


void CRollerProbePage::OnCheckUnidirectionalScan() 
{
	
	
	PROFILE->m_nBiUniDirectionalScan = m_checkUnidirectionalScan.GetCheck();
}

void CRollerProbePage::SetToolBarCheckedState()
{
	
	int	nIndex,nStyle;

	if(theApp.m_Motors.IsAlive() == false) {
		nStyle = TBBS_DISABLED;
	} else {
		nStyle = 0;
	}

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_JOYSTICK))>=0)
		m_wndToolBar.SetButtonStyle(nIndex,nStyle);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_PARK))>=0)
		m_wndToolBar.SetButtonStyle(nIndex,nStyle);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_STEP_OFF_ON))>=0)
		m_wndToolBar.SetButtonStyle(nIndex,nStyle);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_WATER_SPRAY))>=0)
		m_wndToolBar.SetButtonStyle(nIndex,nStyle);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_PRE_WET_ROLLER))>=0)
		m_wndToolBar.SetButtonStyle(nIndex,nStyle);
	
	


}

void CRollerProbePage::OnTimer(UINT nIDEvent) 
{
	SetToolBarCheckedState();
	
	CPropertyPage::OnTimer(nIDEvent);
}

BOOL CRollerProbePage::OnKillActive() 
{

	KillTimer(1);

	return CPropertyPage::OnKillActive();
}
