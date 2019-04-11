// LeadScrewCompensationPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "LeadScrewCompensationPage.h"
#include "MainFrm.h"
#include "FillLeadscrewTableDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeadScrewCompensationPage property page

IMPLEMENT_DYNCREATE(CLeadScrewCompensationPage, CPropertyPage)

CLeadScrewCompensationPage::CLeadScrewCompensationPage() : CPropertyPage(CLeadScrewCompensationPage::IDD)
{
	//{{AFX_DATA_INIT(CLeadScrewCompensationPage)
	//}}AFX_DATA_INIT
	
	m_nAxis = 0;
	m_fStepSize = 0.01f;
	m_fRelativeMoveDistance = 100.0f;
	m_pEditSpinItem=NULL;
	m_nIndex = 0;
	m_nCompensationOffOn=0;
	m_bTimerActive=false;
	ZeroMemory(m_fPos,sizeof m_fPos);

}

CLeadScrewCompensationPage::~CLeadScrewCompensationPage()
{
}

void CLeadScrewCompensationPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeadScrewCompensationPage)
	DDX_Control(pDX, IDC_COMBO_LASER_AXIS, m_comboLaserAxis);
	DDX_Control(pDX, IDC_EDIT_POS2, m_editPos2);
	DDX_Control(pDX, IDC_EDIT_POS1, m_editPos1);
	DDX_Control(pDX, IDC_EDIT_POS0, m_editPos0);
	DDX_Control(pDX, IDC_EDIT_ENCODER_COUNT, m_editEncoderPosition);
	DDX_Control(pDX, IDC_EDIT_POSITION, m_editCurrentPos);
	DDX_Control(pDX, IDC_COMBO_OFF_ON, m_comboOffOn);
	DDX_Control(pDX, IDC_LIST_COORDS, m_listCoords);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE, m_editStepSize);
	DDX_Control(pDX, IDC_COMBO_AXIS_NUMBER, m_comboCompensationAxis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeadScrewCompensationPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLeadScrewCompensationPage)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_AXIS_NUMBER, OnSelchangeComboAxisNumber)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_COORDS, OnGetdispinfoListCoords)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COORDS, OnClickListCoords)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COORDS, OnDblclkListCoords)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_COORDS, OnRclickListCoords)
	ON_COMMAND(ID_LEADSCREW_ADD, OnLeadscrewAdd)
	ON_COMMAND(ID_LEADSCREW_DELETE, OnLeadscrewDelete)
	ON_COMMAND(ID_LEADSCREW_DELETEALL, OnLeadscrewDeleteall)
	ON_COMMAND(ID_LEADSCREW_GOTO, OnLeadscrewGoto)
	ON_CBN_SELCHANGE(IDC_COMBO_OFF_ON, OnSelchangeComboOffOn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STEP_FORWARD, OnButtonStepForward)
	ON_BN_CLICKED(IDC_BUTTON_STEP_BACK, OnButtonStepBack)
	ON_COMMAND(ID_LEADSCREW_SORT, OnLeadscrewSort)
	ON_COMMAND(ID_LEADSCREW_COPYVEVE, OnLeadscrewCopyveve)
	ON_COMMAND(ID_LEADSCREW_COPYVETOVE, OnLeadscrewCopyvetove)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE, OnChangeEditStepsize)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_COMMAND(ID_LEADSCREW_COPYMASTERTOVEVE, OnLeadscrewCopymastertoveve)
	ON_EN_CHANGE(IDC_EDIT_POS0, OnChangeEditPos0)
	ON_EN_CHANGE(IDC_EDIT_POS1, OnChangeEditPos1)
	ON_EN_CHANGE(IDC_EDIT_POS2, OnChangeEditPos2)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_0, OnButtonGoto0)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_1, OnButtonGoto1)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_2, OnButtonGoto2)
	ON_BN_CLICKED(IDC_BUTTON_STORE, OnButtonStore)
	ON_COMMAND(ID_LEADSCREW_FILLTABLE, OnLeadscrewFilltable)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_IT_SO, OnButtonMakeItSo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,ListChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeadScrewCompensationPage message handlers

BOOL CLeadScrewCompensationPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;
	int	ii;

	for(ii=0;ii<16;ii++) {
		Buff.Format(L"%s / %d",theApp.m_Axes[ii].cName,theApp.m_Axes[ii].nPhysicalAxis);
		m_comboCompensationAxis.AddString(Buff);
	}
	Buff.LoadString(IDS_Off);	m_comboOffOn.AddString(Buff);
	Buff.LoadString(IDS_On);	m_comboOffOn.AddString(Buff);

	m_comboLaserAxis.AddString(L"X");
	m_comboLaserAxis.AddString(L"Y");
	m_comboLaserAxis.AddString(L"Z");
	m_comboLaserAxis.AddString(L"^");

	
	CreateColumns();
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CLeadScrewCompensationPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CLeadScrewCompensationPage::UpdateAllControls()
{
	CString Buff;
	if (GetSafeHwnd() == NULL) return;

	m_comboCompensationAxis.SetCurSel(m_nAxis);
	m_fStepSize = theApp.m_Axes[m_nAxis].fStepSize;

	if(m_fStepSize >= 0.001f) {
		Buff.Format(L"%.07f",m_fStepSize);
	} else {
		Buff.Format(L"%11.05e",m_fStepSize);
	}
	m_editStepSize.SetWindowText(Buff);

	theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].m_fStepSize = m_fStepSize;

	Buff.Format(_T("%.03f"),m_fPos[0][m_nAxis]);
	m_editPos0.SetWindowText(Buff);
	Buff.Format(_T("%.03f"),m_fPos[1][m_nAxis]);
	m_editPos1.SetWindowText(Buff);
	Buff.Format(_T("%.03f"),m_fPos[2][m_nAxis]);
	m_editPos2.SetWindowText(Buff);

	m_comboOffOn.SetCurSel(theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].m_bOffOn);
	m_comboLaserAxis.SetCurSel(theApp.m_LastSettings.nLaserAxis);

	FillList();
}

BOOL CLeadScrewCompensationPage::OnKillActive() 
{
	if(m_bTimerActive == TRUE) KillTimer(1);
	m_bTimerActive = FALSE;
	
	return CPropertyPage::OnKillActive();
}

BOOL CLeadScrewCompensationPage::OnSetActive() 
{
	
	UpdateAllControls();

	if(m_bTimerActive != TRUE) SetTimer(1,200,NULL);
	m_bTimerActive = TRUE;

	theApp.m_LastSettings.nLastPmacTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	return CPropertyPage::OnSetActive();
}

void CLeadScrewCompensationPage::OnSelchangeComboAxisNumber() 
{
	m_nAxis = m_comboCompensationAxis.GetCurSel();
	UpdateAllControls();
}

void CLeadScrewCompensationPage::OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	Position Pos;

	str[0]=0;

	if(pDispInfo->item.iItem < theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].m_nCoordL) {
		theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].GetPosition(pDispInfo->item.iItem,&Pos);

		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:
			swprintf_s(str,_T("%.03f"),Pos.fMachine);
			break;
		case 2:
			swprintf_s(str,_T("%.03f"),Pos.fActualPos);
			break;
		case 3:
			swprintf_s(str,_T("%.03f"),Pos.fActualNeg);
			break;
		}
		
	}
	pDispInfo->item.pszText=str;

	*pResult = 0;
}

void CLeadScrewCompensationPage::OnClickListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	CloseEditSpin();
	m_nIndex = pDispInfo->item.mask;
	
	*pResult = 0;
}

void CLeadScrewCompensationPage::OnDblclkListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;
	int nAxis = m_nAxis;
	Position *pPos;

	m_listCoords.GetWindowRect(CtrlRect);
	m_listCoords.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	CloseEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**)&m_pEditSpinItem,&Rect);
	m_nIndex = pDispInfo->item.mask;

	if((m_nIndex>=0) && (m_nIndex<theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].m_nCoordL)) {

		pPos = theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].GetPtrPosition(m_nIndex);
		switch(pDispInfo->item.iItem) {
		case 1:	m_pEditSpinItem->Initialize(&pPos->fMachine,-20000.0f,20000.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 2:	m_pEditSpinItem->Initialize(&pPos->fActualPos,-20000.0f,20000.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 3:	m_pEditSpinItem->Initialize(&pPos->fActualNeg,-20000.0f,20000.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		}

	}
	
	*pResult = 0;
}

void CLeadScrewCompensationPage::CreateColumns()
{
	CString MasterSlave[4] = { L"#",L"Machine", L"Actual", L"-Actual"};
	CString LaserName[4] = { L"#",L"Machine", L"Laser", L"-Laser"};
	int ColumnWidth[4] = { 0,60,50,50 };
	CRect rr;

	m_listCoords.GetWindowRect(&rr);

	// Delete all of the columns.
	int nColumnCount = m_listCoords.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listCoords.DeleteColumn(0);
	}

	switch(theApp.m_LastSettings.nLeadScrewCompensationMode) {
	case 0:
		m_listCoords.InsertColumn(0,MasterSlave[0], LVCFMT_RIGHT,0);
		m_listCoords.InsertColumn(1,MasterSlave[1], LVCFMT_RIGHT,rr.Width()/3-1);
		m_listCoords.InsertColumn(2,MasterSlave[2], LVCFMT_RIGHT,rr.Width()/3-2);
//		m_listCoords.InsertColumn(3,MasterSlave[3], LVCFMT_RIGHT,rr.Width()/3-2);
		break;
	case 1:
		m_listCoords.InsertColumn(0,LaserName[0], LVCFMT_RIGHT,0);
		m_listCoords.InsertColumn(1,LaserName[1], LVCFMT_RIGHT,rr.Width()/3-1);
		m_listCoords.InsertColumn(2,LaserName[2], LVCFMT_RIGHT,rr.Width()/3-2);
//		m_listCoords.InsertColumn(3,LaserName[3], LVCFMT_RIGHT,rr.Width()/3-2);
		break;
	}
	m_listCoords.SetExtendedStyle( LVS_EX_FULLROWSELECT  );

}

void CLeadScrewCompensationPage::FillList()
{
	CString Buff;
	int nCoordL = theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].m_nCoordL;

	m_listCoords.DeleteAllItems();
	for (int ii=0;ii<nCoordL;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listCoords.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listCoords.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<nCoordL;ii++) 
		m_listCoords.SetItemState(ii , 0,LVIS_SELECTED);
	m_listCoords.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listCoords.SetFocus();


}

void CLeadScrewCompensationPage::OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	CloseEditSpin();

	m_nIndex=pDispInfo->item.mask;

	CMenu* menu = (CMenu *) new CMenu;
	CPoint	Point;

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_LEADSCREW_MENU)) {
		CMenu* pPopup = menu->GetSubMenu(0);
		ASSERT(pPopup != NULL);
				
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;

	*pResult = 0;
}

void CLeadScrewCompensationPage::CloseEditSpin()
{
	SAFE_DELETE( m_pEditSpinItem );
}

void CLeadScrewCompensationPage::OnLeadscrewAdd() 
{
	Position Pos;
	int nAxis = m_nAxis;

	Pos.Zero();
	if(theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[nAxis].nPhysicalAxis - 1 ].m_nCoordL < 1) {
		theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[nAxis].nPhysicalAxis - 1 ].Zero();
		theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[nAxis].nPhysicalAxis - 1 ].Add(Pos);
	}

	switch(theApp.m_LastSettings.nLeadScrewCompensationMode) {
	case 0:
		Pos.fMachine = (float)theApp.m_PmacUser.m_nPosition[m_nAxis] * m_fStepSize;
		Pos.fActualPos = (float)theApp.m_PmacUser.m_nPosition[m_nAxis] * m_fStepSize;
		Pos.fActualNeg = (float)theApp.m_PmacUser.m_nPosition[m_nAxis] * m_fStepSize;
		break;
	case 1:
		Pos.fMachine = (float)theApp.m_PmacUser.m_nPosition[m_nAxis] * m_fStepSize;
		Pos.fActualPos = 0.0f;
		Pos.fActualNeg = 0.0f;
		break;
	}
	Pos.nMachine = (int)(Pos.fMachine / m_fStepSize);
	Pos.nActualPos = (int)(Pos.fActualPos / m_fStepSize);
	Pos.nActualNeg = (int)(Pos.fActualNeg / m_fStepSize);

	theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].Add(Pos);
	theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].Sort();

	UpdateAllControls();
	
}

void CLeadScrewCompensationPage::OnLeadscrewDelete() 
{
	theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].Delete(m_nIndex);
	UpdateAllControls();
}

void CLeadScrewCompensationPage::OnLeadscrewDeleteall() 
{
	theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].Zero();
	UpdateAllControls();	
}


void CLeadScrewCompensationPage::OnSelchangeComboOffOn() 
{
	theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].m_bOffOn = m_comboOffOn.GetCurSel() & 0x01;
}

void CLeadScrewCompensationPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;
	int nAxis = m_nAxis;
//	int nPos = theApp.m_PmacUser.m_nPosition[nAxis];
	int nPos = theApp.m_Axes[nAxis].nPos+theApp.m_Axes[nAxis].nOffset;
	static float fOldStepSize;

	if(m_fStepSize - theApp.m_Axes[m_nAxis].fStepSize) {
		m_fStepSize = theApp.m_Axes[m_nAxis].fStepSize;
		if(m_fStepSize >= 0.001f) {
			Buff.Format(L"%.07f",m_fStepSize);
		} else {
			Buff.Format(L"%11.05e",m_fStepSize);
		}
		m_editStepSize.SetWindowText(Buff);
	}

	if((nPos - m_nOldPos) || (fOldStepSize - m_fStepSize)) {
		m_nOldPos = nPos;
		fOldStepSize = m_fStepSize;
		Buff.Format(_T("%d"), m_nOldPos);
		m_editEncoderPosition.SetWindowText(Buff);

		Buff.Format(_T("%.03f mm"), (float)m_nOldPos * m_fStepSize);
		m_editCurrentPos.SetWindowText(Buff);
	}

	
	CPropertyPage::OnTimer(nIDEvent);
}


HRESULT CLeadScrewCompensationPage::ListChanged(WPARAM,LPARAM)
{
	theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].CopyPositiveToNegative();
	theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].InvalidateIntegers();
//	UpdateAllControls();
	return NULL;
}

void CLeadScrewCompensationPage::OnButtonStepForward() 
{
	CString Buff;
	CCoord Cp;
	int nAxis = m_nAxis;

	m_editRelativeDistance.GetWindowText(Buff);
	_WTOF(Buff,m_fRelativeMoveDistance);

	int nUnit = (int)((((float)theApp.m_PmacUser.m_nPosition[nAxis] * m_fStepSize) + (m_fRelativeMoveDistance / 3.0f)) / m_fRelativeMoveDistance);
	nUnit++;

	int nPos = (int)(((float)nUnit * m_fRelativeMoveDistance) / m_fStepSize);
	theApp.m_PmacUser.m_nTunningAxis = m_nAxis;
	theApp.m_PmacUser.JogTo(m_nAxis, nPos);


}

void CLeadScrewCompensationPage::OnButtonStepBack() 
{
	CString Buff;
	CCoord Cp;
	int nAxis = m_nAxis;
	float fUnit,fRemainder;
	int nUnit;

	m_editRelativeDistance.GetWindowText(Buff);
	_WTOF(Buff,m_fRelativeMoveDistance);

	fUnit = ((float)theApp.m_PmacUser.m_nPosition[nAxis] * m_fStepSize) / m_fRelativeMoveDistance;

	fRemainder = fmodf(fUnit,1.0f);
	if(0.5f <= fRemainder) nUnit = (int)fUnit;
	if((0.5 > fRemainder) && (fRemainder > -0.5)) nUnit = (int)fUnit - 1;
	if(-0.5 <= fRemainder) nUnit = (int)fUnit;

	int nPos = (int)(((float)nUnit * m_fRelativeMoveDistance) / m_fStepSize);
	theApp.m_PmacUser.m_nTunningAxis = m_nAxis;
	theApp.m_PmacUser.JogTo(m_nAxis, nPos);
	
}

void CLeadScrewCompensationPage::OnLeadscrewSort() 
{
	theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].Sort();
	UpdateAllControls();
}


void CLeadScrewCompensationPage::OnLeadscrewCopyveve() 
{
	theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].CopyPositiveToNegative();
	FillList();
}

void CLeadScrewCompensationPage::OnLeadscrewCopyvetove() 
{
	theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].CopyNegativeToPositive();
	FillList();
}

void CLeadScrewCompensationPage::OnLeadscrewCopymastertoveve() 
{
	theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].CopyMasterToNegativePositive();
	FillList();
}

void CLeadScrewCompensationPage::OnChangeEditStepsize() 
{
	CMainFrame* pFrame = (CMainFrame*)theApp.m_pMainWnd;
	CString Buff;
	float fTemp;

	m_editStepSize.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	if(fTemp - m_fStepSize) {
		m_fStepSize = fTemp;
		theApp.m_Axes[m_nAxis].fStepSize = m_fStepSize;

		theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis-1 ].InvalidateIntegers();
		pFrame->SendMessage(UI_UPDATE_AXES,NULL,NULL);
	}

}

void CLeadScrewCompensationPage::OnButtonSave() 
{
	theApp.SaveTank();
}

void CLeadScrewCompensationPage::OnLeadscrewGoto() 
{
	Position Pos;
	int nAxis = m_nAxis;

	if((m_nIndex>=0) && (m_nIndex<=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[nAxis].nPhysicalAxis - 1 ].m_nCoordL)) {
		theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].GetPosition(m_nIndex,&Pos);
		Goto(Pos.fMachine);
	}
}


void CLeadScrewCompensationPage::OnChangeEditPos0() 
{
	CString Buff;

	m_editPos0.GetWindowText(Buff);
	_WTOF(Buff,m_fPos[0][m_nAxis]);
}

void CLeadScrewCompensationPage::OnChangeEditPos1() 
{
	CString Buff;

	m_editPos1.GetWindowText(Buff);
	_WTOF(Buff,m_fPos[1][m_nAxis]);
}

void CLeadScrewCompensationPage::OnChangeEditPos2() 
{
	CString Buff;

	m_editPos2.GetWindowText(Buff);
	_WTOF(Buff,m_fPos[2][m_nAxis]);
}




void CLeadScrewCompensationPage::Goto(float fPos) 
{
	int nAbsoluteMove;

	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:
		theApp.m_PmacUser.m_nTunningAxis = m_nAxis;
		theApp.m_PmacUser.m_nTunningPos = (int)(fPos / m_fStepSize);
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction=MOVETO_HIGH_GAIN;
		break;
	case SI6:
		break;
	case SI10:
		break;
	case FIELD_BUS:
		nAbsoluteMove = (int)(fPos / theApp.m_Axes[m_nAxis].fStepSize);
		nAbsoluteMove=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].ActualToMachine(nAbsoluteMove,theApp.m_Axes[m_nAxis].nPos);

		theApp.m_Motors.AbsoluteMove(&theApp.m_Axes[m_nAxis],nAbsoluteMove);
		break;
	}
}

void CLeadScrewCompensationPage::OnButtonGoto0()
{
	Goto(m_fPos[0][m_nAxis]);
}

void CLeadScrewCompensationPage::OnButtonGoto1() 
{
	Goto(m_fPos[1][m_nAxis]);
}

void CLeadScrewCompensationPage::OnButtonGoto2() 
{
	Goto(m_fPos[2][m_nAxis]);
}

void CLeadScrewCompensationPage::OnButtonStore() 
{
	CString Buff;
	CCoord Cp;

	m_fPos[1][m_nAxis] = (float)theApp.m_Axes[m_nAxis].nPos * theApp.m_Axes[m_nAxis].fStepSize;

	Buff.Format(_T("%.03f"),m_fPos[1][m_nAxis]);
	m_editPos1.SetWindowText(Buff);
	
}

void CLeadScrewCompensationPage::OnLeadscrewFilltable() 
{
	CFillLeadscrewTableDlg dlg(this,m_nAxis);

	dlg.DoModal();
}

void CLeadScrewCompensationPage::OnButtonMakeItSo() 
{
	if(theApp.m_FaroWorkerThread.IsCollectingData() != true || theApp.m_FaroWorkerThread.IsConnected() != true) {
		MessageBox(L"Please switch on FARO laser\r\nand collect measurements", L"Error",MB_OK);
		return;
	}


	if(theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].m_bOffOn == true) {
		MessageBox(L"Please switch off compensation\r\nand try again", L"Error",MB_OK);
		return;
	}
	
	theApp.m_LastSettings.nLaserAxis = m_comboLaserAxis.GetCurSel();
	theApp.m_Thread.m_nLeadscrewCompensationAxis = m_nAxis;
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction=LEADSCREW_COMPENSATION;
}
