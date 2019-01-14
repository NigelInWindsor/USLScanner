// ThicknessCalibrationpage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ThicknessCalibrationpage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThicknessCalibrationpage property page

IMPLEMENT_DYNCREATE(CThicknessCalibrationpage, CPropertyPage)

CThicknessCalibrationpage::CThicknessCalibrationpage() : CPropertyPage(CThicknessCalibrationpage::IDD)
{
	//{{AFX_DATA_INIT(CThicknessCalibrationpage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nIndex = 0;
	m_pEditSpinItem=NULL;

	m_Action[0] = "";
	m_Action[1] = "Full";
	m_Action[2] = "Fast";
}

CThicknessCalibrationpage::~CThicknessCalibrationpage()
{
}

void CThicknessCalibrationpage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThicknessCalibrationpage)
	DDX_Control(pDX, IDC_EDIT_TEST_DATE, m_editTestDate);
	DDX_Control(pDX, IDC_EDIT_PT1, m_editPt1);
	DDX_Control(pDX, IDC_EDIT_PT0, m_editPt0);
	DDX_Control(pDX, IDC_EDIT_CALIBRATION_BLOCK_NAME, m_editCalibrationBlockName);
	DDX_Control(pDX, IDC_SPIN_MATERIAL_VELOCITY, m_spinMaterialVelocity);
	DDX_Control(pDX, IDC_SPIN_MATERIAL_CONSTANT, m_spinMaterialConstant);
	DDX_Control(pDX, IDC_COMBO_MATERIAL_VELOCITY_MODE, m_comboMaterialVelocityMode);
	DDX_Control(pDX, IDC_EDIT_MATERIAL_VELOCITY, m_editMaterialVelocity);
	DDX_Control(pDX, IDC_EDIT_MATERIAL_CONSTANT, m_editMaterialConstant);
	DDX_Control(pDX, IDC_LIST_CAL, m_listCoords);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThicknessCalibrationpage, CPropertyPage)
	//{{AFX_MSG_MAP(CThicknessCalibrationpage)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_CAL, OnGetdispinfoListCal)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_CAL, OnRclickListCal)
	ON_COMMAND(ID_CALIBRATION_ADD, OnCalibrationAdd)
	ON_COMMAND(ID_CALIBRATION_MODIFYCOORD, OnCalibrationModifyCoord)
	ON_COMMAND(ID_CALIBRATION_UPDATETIME, OnCalibrationUpdatetime)
	ON_COMMAND(ID_CALIBRATION_GOTO, OnCalibrationGoto)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CAL, OnDblclkListCal)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CAL, OnClickListCal)
	ON_BN_CLICKED(IDC_BUTTON_COMPUTE_LOOKUP, OnButtonComputeLookup)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATE_THICKNESS, OnButtonCalibrateThickness)
	ON_COMMAND(ID_CALIBRATION_DELETE, OnCalibrationDelete)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_MATERIAL_CONSTANT, OnChangeEditMaterialConstant)
	ON_EN_CHANGE(IDC_EDIT_MATERIAL_VELOCITY, OnChangeEditMaterialVelocity)
	ON_CBN_SELCHANGE(IDC_COMBO_MATERIAL_VELOCITY_MODE, OnSelchangeComboMaterialVelocityMode)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_CALIBRATION_SORT, OnCalibrationSort)
	ON_EN_CHANGE(IDC_EDIT_PT0, OnChangeEditPt0)
	ON_EN_CHANGE(IDC_EDIT_PT1, OnChangeEditPt1)
	ON_COMMAND(ID_BUTTON_CALIBRATE, OnButtonCalibrate)
	ON_COMMAND(ID_BUTTON_CHECK_GAUGE, OnButtonCheckGauge)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,TableChanged)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThicknessCalibrationpage message handlers
int CThicknessCalibrationpage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_THICKNESS_TOOLBAR)) {
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

BOOL CThicknessCalibrationpage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;
	
	Buff.LoadString(IDS_Velocity_and_constant);	m_comboMaterialVelocityMode.AddString(Buff);
	Buff.LoadString(IDS_Velocity_Table);		m_comboMaterialVelocityMode.AddString(Buff);

	m_spinMaterialVelocity.SetRange(0,12000);
	m_spinMaterialConstant.SetRange(0,400);

	CreateColumns();
	

	UpdateAllControls();
	SetAccessPrivelage();
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CThicknessCalibrationpage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CThicknessCalibrationpage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	m_spinMaterialVelocity.SetPos(theApp.m_LastSettings.nMaterialVelocity);
	m_spinMaterialConstant.SetPos(theApp.m_LastSettings.nMaterialConstant + 200);

	Buff.Format(_T("%d m/s"),theApp.m_LastSettings.nMaterialVelocity);
	m_editMaterialVelocity.SetWindowText(Buff);
	Buff.Format(_T("%d ns"),theApp.m_LastSettings.nMaterialConstant);
	m_editMaterialConstant.SetWindowText(Buff);

	m_comboMaterialVelocityMode.SetCurSel(theApp.m_LastSettings.nMaterialVelocityMode);

	int nn=theApp.m_ThicknessCalibrationPathName.ReverseFind('\\');
	Buff = theApp.m_ThicknessCalibrationPathName.Right(theApp.m_ThicknessCalibrationPathName.GetLength()-nn-1);
	if((nn=Buff.Find(L"."))>0) {
		Buff.GetBufferSetLength(nn);
	};
	m_editCalibrationBlockName.SetWindowText(Buff);

	Buff.Format(L"%d",theApp.m_LastSettings.nThicknessTestPt0+1);
	m_editPt0.SetWindowText(Buff);
	Buff.Format(L"%d",theApp.m_LastSettings.nThicknessTestPt1+1);
	m_editPt1.SetWindowText(Buff);

	CTime time(theApp.m_LastSettings.osThicknessCalibrationDate);
	Buff = time.Format(_T("%d/%m/%Y %Hh%M"));
	m_editTestDate.SetWindowText(Buff);

	CTime CurrentTime = CTime::GetCurrentTime();
	CTimeSpan TimeDiff(CurrentTime.GetTime() - ((theApp.m_LastSettings.osThicknessCalibrationDate + theApp.m_LastSettings.osThicknessReCalibrationPeriodMinutes) * 3600));

	int nNumberMinutes = TimeDiff.GetMinutes();
	if((nNumberMinutes > 0) ||(theApp.m_LastSettings.bThicknessCalibrationStatus==false)) {
		m_editTestDate.SetTextColour(RGB(255,0,0));
	} else {
		m_editTestDate.SetTextColour(RGB(0,0,255));
	}

	FillList();
	SetAccessPrivelage();
}

void CThicknessCalibrationpage::CreateColumns()
{
	int ColumnWidth[15] = { 30,55,55,55,55,55,45,45,45,45,45,45,45,45,45};
	CString ColumnName[15];
	ColumnName[0]= "#";
	ColumnName[1]= "Status";
	ColumnName[2]= "mm";
	ColumnName[3]= "Nom mm";
	ColumnName[4]= "Min mm";
	ColumnName[5]= "Max mm";
	ColumnName[6]= "X";
	ColumnName[7]= "Y";
	ColumnName[8]= "Z";
	ColumnName[9]= "i";
	ColumnName[10]= "j";
	ColumnName[11]= "k";
	ColumnName[12]= "vel";
	ColumnName[13]= "con";


	for (int ii=0;ii<14;ii++) {
		m_listCoords.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}
	m_listCoords.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CThicknessCalibrationpage::FillList()
{
	

	CString Buff;

	m_listCoords.DeleteAllItems();
	for (int ii=0;ii<theApp.m_ThickCal.m_nCoordL;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listCoords.InsertItem(ii, Buff.GetBuffer(255), ii);
	};
	m_listCoords.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<theApp.m_ThickCal.m_nCoordL;ii++) {
		m_listCoords.SetItemState(ii , 0,LVIS_SELECTED);
	}
	m_listCoords.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listCoords.SetFocus();

}

void CThicknessCalibrationpage::OnGetdispinfoListCal(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	CCoord Cp;
	CCoord Cp0;
	float fVelocity, fConstant,fThickness0,fThickness1;
	int nT0,nT1;

	str[0]=0;
//	if(theApp.m_ThickCal) {
		theApp.m_ThickCal.GetCoord(pDispInfo->item.iItem,&Cp);
		if((pDispInfo->item.iItem-1) >= 0) {
			theApp.m_ThickCal.GetCoord(pDispInfo->item.iItem-1,&Cp0);
		} else {
			Cp0.Zero();
		}

		nT0 = Cp0.nTimens;
		fThickness0 = Cp0.fNomThickness;

		nT1 = Cp.nTimens;
		fThickness1 = Cp.fNomThickness;

		fConstant = (-fThickness0 * (float)(nT1 - nT0) / (fThickness1 - fThickness0) ) + (float)nT0;
		fConstant *= -1.0f;

		fVelocity = (fThickness1 - fThickness0) / (float)(nT1 - nT0) * 2e6f;


		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:
			if((Cp.fActThickness>=Cp.fMinThickness) && (Cp.fActThickness<=Cp.fMaxThickness)) {
				swprintf_s(str,L"%s",L"....Pass");
			} else {
				swprintf_s(str,L"%s",L"Fail...");
			}
			break;
		case 2:
			swprintf_s(str,_T("%.03f"),Cp.fActThickness);
			break;
		case 3:
			swprintf_s(str,_T("%.03f"),Cp.fNomThickness);
			break;
		case 4:
			swprintf_s(str,_T("%.03f"),(float)Cp.fMinThickness);
			break;
		case 5:
			swprintf_s(str,_T("%.03f"),(float)Cp.fMaxThickness);
			break;
		case 6:
			swprintf_s(str,_T("%.01f"),Cp.Side0.fX);
			break;
		case 7:
			swprintf_s(str,_T("%.01f"),Cp.Side0.fY);
			break;
		case 8:
			swprintf_s(str,_T("%.01f"),Cp.Side0.fZ);
			break;
		case 9:
			swprintf_s(str,_T("%.02f"),Cp.Side0.fI);
			break;
		case 10:
			swprintf_s(str,_T("%.02f"),Cp.Side0.fJ);
			break;
		case 11:
			swprintf_s(str,_T("%.02f"),Cp.Side0.fK);
			break;
		case 12:
			swprintf_s(str,_T("%.0f"),fVelocity);
			break;
		case 13:
			swprintf_s(str,_T("%.0f"),fConstant);
			break;
		}
//	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}



void CThicknessCalibrationpage::OnRclickListCal(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	m_nIndex=m_listCoords.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_THICK_CAL_MENU)) {

		pPopup = menu->GetSubMenu(0);
		if(!(theApp.m_nLogonLevelMask & theApp.m_cAccess[IDC_LIST_CAL])) {
			pPopup->EnableMenuItem( ID_CALIBRATION_ADD   , MF_GRAYED|MF_BYCOMMAND);
			pPopup->EnableMenuItem( ID_CALIBRATION_DELETE   , MF_GRAYED|MF_BYCOMMAND);
			pPopup->EnableMenuItem( ID_CALIBRATION_SORT   , MF_GRAYED|MF_BYCOMMAND);
			pPopup->EnableMenuItem( ID_CALIBRATION_MODIFYCOORD   , MF_GRAYED|MF_BYCOMMAND);
			pPopup->EnableMenuItem( ID_CALIBRATION_UPDATETIME   , MF_GRAYED|MF_BYCOMMAND);
		}

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	*pResult = 0;
}

void CThicknessCalibrationpage::OnCalibrationAdd() 
{
	
	CCoord	CpSurface;

	CpSurface.Zero();

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	CpSurface.fWp[PORTSIDE] = theApp.m_Kinematics.m_fDesiredWaterPath[PORTSIDE];
	CpSurface.nTimens = theApp.m_UtUser.GetGateMaterialTimeNs(0,1);
	if(CpSurface.nTimens<0) CpSurface.nTimens=0;
	theApp.m_ThickCal.Add(CpSurface);
	m_nColumn=0;
	TableChanged(NULL,NULL);
	FillList();
	Invalidate(FALSE);
	
}

void CThicknessCalibrationpage::OnCalibrationModifyCoord() 
{
	
	CCoord	CpSurface,Cp;

	CpSurface.Zero();

	theApp.m_Motors.GetSurfacePos(&CpSurface);

	theApp.m_ThickCal.GetCoord(m_nIndex,&Cp);
	Cp.SetSide0(CpSurface);
	theApp.m_ThickCal.ModifyCoord(m_nIndex,Cp);
	m_nColumn=0;
	TableChanged(NULL,NULL);

}

void CThicknessCalibrationpage::OnCalibrationUpdatetime() 
{
	

	theApp.m_ThickCal.m_pCp[m_nIndex].nTimens = theApp.m_UtUser.GetGateMaterialTimeNs(0,1);
	m_nColumn=0;
	TableChanged(NULL, NULL);
	Invalidate(FALSE);
}

void CThicknessCalibrationpage::OnCalibrationGoto() 
{
	
	CCoord	CpSurface;

	theApp.m_Thread.StopCurrentThreadAction();

	theApp.m_ThickCal.GetCoord(m_nIndex,&CpSurface);

	theApp.m_Motors.GotoSurfaceCoord(&CpSurface, PORTSIDE);
}

void CThicknessCalibrationpage::OnDblclkListCal(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	m_listCoords.GetWindowRect(CtrlRect);
	m_listCoords.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	ClearEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

	if(	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDC_LIST_CAL]) {

		switch(m_nColumn=pDispInfo->item.iItem) {
		default:
			break;
		case 3:
			m_pEditSpinItem->Initialize(&theApp.m_ThickCal.m_pCp[m_nIndex=pDispInfo->item.mask].fNomThickness,0.0f,100.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 4:
			m_pEditSpinItem->Initialize(&theApp.m_ThickCal.m_pCp[m_nIndex=pDispInfo->item.mask].fMinThickness,0.0f,100.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 5:
			m_pEditSpinItem->Initialize(&theApp.m_ThickCal.m_pCp[m_nIndex=pDispInfo->item.mask].fMaxThickness,0.0f,100.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 6:
			m_pEditSpinItem->Initialize(&theApp.m_ThickCal.m_pCp[m_nIndex=pDispInfo->item.mask].Side0.fX,-1000.0f,1000.0f,0.001f,_T("%.01f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 7:
			m_pEditSpinItem->Initialize(&theApp.m_ThickCal.m_pCp[m_nIndex=pDispInfo->item.mask].Side0.fY,-1000.0f,1000.0f,0.001f,_T("%.01f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 8:
			m_pEditSpinItem->Initialize(&theApp.m_ThickCal.m_pCp[m_nIndex=pDispInfo->item.mask].Side0.fZ,-1000.0f,1000.0f,0.001f,_T("%.01f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		}
	}

	*pResult = 0;
}

void CThicknessCalibrationpage::OnClickListCal(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ClearEditSpin();
	
	*pResult = 0;
}

void CThicknessCalibrationpage::ClearEditSpin()
{
	if(m_pEditSpinItem!=NULL) {
		delete m_pEditSpinItem;
		m_pEditSpinItem=NULL;
	}

}

void CThicknessCalibrationpage::OnButtonComputeLookup() 
{
	
	
	theApp.ComputeVelocityTable();
}

void CThicknessCalibrationpage::OnButtonCalibrateThickness() 
{
//	
	
//	theApp.m_Thread.m_pThicknessCalibrationpage = this;
//	theApp.m_Thread.StopCurrentThreadAction();
//	theApp.m_Thread.m_nThreadAction = THICKNESS_CALIBRATION;
}

void CThicknessCalibrationpage::OnCalibrationDelete() 
{
	
	theApp.m_ThickCal.DeleteCoord(m_nIndex);

	FillList();
	m_nColumn=0;
	TableChanged(NULL, NULL);
	Invalidate(FALSE);
	
}

void CThicknessCalibrationpage::SetAccessPrivelage()
{
	if (GetSafeHwnd() == NULL) return;
	
	bool bFlagEnable;

	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDD_THICKNESS_CALIBRATION_DLG] ? bFlagEnable = TRUE : bFlagEnable = FALSE;

	if(bFlagEnable == TRUE) {
		m_listCoords.EnableWindow(true);
		switch(theApp.m_LastSettings.nMaterialVelocityMode) {
		case 0:
			m_editMaterialVelocity.EnableWindow(true);
			m_editMaterialConstant.EnableWindow(true);
			m_spinMaterialVelocity.EnableWindow(true);
			m_spinMaterialConstant.EnableWindow(true);
			m_editPt0.EnableWindow(true);
			m_editPt1.EnableWindow(true);
			break;
		case 1:
			m_editMaterialVelocity.EnableWindow(false);
			m_editMaterialConstant.EnableWindow(false);
			m_spinMaterialVelocity.EnableWindow(false);
			m_spinMaterialConstant.EnableWindow(false);
			m_editPt0.EnableWindow(false);
			m_editPt1.EnableWindow(false);
			break;
		}
	} else {
		m_editMaterialVelocity.EnableWindow(false);
		m_editMaterialConstant.EnableWindow(false);
		m_spinMaterialVelocity.EnableWindow(false);
		m_spinMaterialConstant.EnableWindow(false);
		m_listCoords.EnableWindow(false);
		m_editPt0.EnableWindow(false);
		m_editPt1.EnableWindow(false);
	}


}

BOOL CThicknessCalibrationpage::OnSetActive() 
{
	

	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	return CPropertyPage::OnSetActive();
}


void CThicknessCalibrationpage::OnPaint() 
{
	
	CPaintDC dc(this); // device context for painting
	
	return;

	CRect rr;
	CBrush Brush(RGB(255,255,255));
	CSize size;
	CString Buff;
	CBrush brushBK(GetSysColor(COLOR_BTNFACE));

	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = 10;
	CFont Font;
	CFont* pOldFont;
	Font.CreateFontIndirect(&lf);

	Buff = "Hello";
	size = dc.GetTextExtent(Buff);

	pOldFont = dc.SelectObject(&Font);
	dc.SetBkMode(TRANSPARENT);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.top=rr.bottom;
	rr.bottom=rr.top+3 + size.cy;
	rr.left-=size.cx;
	rr.right+=size.cx;
	dc.FillRect(rr,&brushBK);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2);
	dc.FillRect(&rr,&Brush);

	RenderThicknessAgainstTime(&dc,&rr);

	dc.SelectObject(pOldFont);

}

void CThicknessCalibrationpage::OnChangeEditMaterialConstant() 
{
	
	CString Buff;
	int nTemp = theApp.m_LastSettings.nMaterialConstant;

	m_editMaterialConstant.GetWindowText(Buff);
	theApp.m_LastSettings.nMaterialConstant = _ttoi(Buff);

	if(nTemp - theApp.m_LastSettings.nMaterialConstant) {
		m_spinMaterialConstant.SetPos(theApp.m_LastSettings.nMaterialConstant+200);
	}
	
}

void CThicknessCalibrationpage::OnChangeEditMaterialVelocity() 
{
	
	CString Buff;
	int nTemp = theApp.m_LastSettings.nMaterialVelocity;

	m_editMaterialVelocity.GetWindowText(Buff);
	theApp.m_LastSettings.nMaterialVelocity = _ttoi(Buff);
	if(nTemp - theApp.m_LastSettings.nMaterialVelocity) {
		m_spinMaterialVelocity.SetPos(theApp.m_LastSettings.nMaterialVelocity);
	}
}

void CThicknessCalibrationpage::OnSelchangeComboMaterialVelocityMode() 
{
	
	
	theApp.m_LastSettings.nMaterialVelocityMode = m_comboMaterialVelocityMode.GetCurSel();
	SetAccessPrivelage();
	Invalidate(false);
}

HRESULT CThicknessCalibrationpage::TableChanged(WPARAM, LPARAM)
{
	
	CCoord Cp;

	theApp.m_ThickCal.GetCoord(m_nIndex,&Cp);
	/*
	switch(m_nColumn) {
	case 2: Cp.nTimens = (int)(m_fTemp * 1000.0f);
		theApp.m_ThickCal.ModifyCoord(m_nIndex,Cp);
		break;
	}
	*/
	theApp.ComputeVelocityTable();
	Invalidate(false);
	return NULL;
}

BOOL CThicknessCalibrationpage::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
   case ID_FILE_SAVEAS: strTipText.LoadString(IDS_Save);
	   break;
   case ID_FILE_OPEN: strTipText.LoadString(IDS_Open);
	   break;
   case ID_BUTTON_CALIBRATE: strTipText.LoadString(IDS_Run_calibration);
	   break;
   case ID_BUTTON_CHECK_GAUGE: strTipText.LoadString(IDS_Check_gauge);
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



void CThicknessCalibrationpage::OnFileSaveas() 
{
	
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = theApp.m_ThicknessCalibrationPathName;

	if((nPos = PROFILE->m_FileName.ReverseFind('.')) >0) {
		InitialFile = PROFILE->m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,_T(".cal"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Calibration blocks (*.cal)|*.cal|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_UltrasonicsPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_UltrasonicsPath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {

			File.Write(&theApp.m_LastSettings.nMaterialVelocityMode, sizeof theApp.m_LastSettings.nMaterialVelocityMode);
			File.Write(&theApp.m_LastSettings.nMaterialVelocity, sizeof theApp.m_LastSettings.nMaterialVelocity);
			File.Write(&theApp.m_LastSettings.nMaterialConstant, sizeof theApp.m_LastSettings.nMaterialConstant);
			theApp.m_ThickCal.Save(&File);
			File.Close();
			theApp.m_ThicknessCalibrationPathName = FilePath;
			UpdateAllControls();
		}
	}		
	
}

void CThicknessCalibrationpage::OnFileOpen() 
{
	
	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	CFileDialog FileDlg(TRUE,_T(".cal"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Calibration blocks (*.cal)|*.cal|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_UltrasonicsPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_UltrasonicsPath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			File.Read(&theApp.m_LastSettings.nMaterialVelocityMode, sizeof theApp.m_LastSettings.nMaterialVelocityMode);
			File.Read(&theApp.m_LastSettings.nMaterialVelocity, sizeof theApp.m_LastSettings.nMaterialVelocity);
			File.Read(&theApp.m_LastSettings.nMaterialConstant, sizeof theApp.m_LastSettings.nMaterialConstant);
			theApp.m_ThickCal.Retrieve(&File);
			File.Close();
			theApp.m_ThicknessCalibrationPathName = FilePath;
			m_nColumn=0;
			TableChanged(NULL,NULL);
			UpdateAllControls();
			Invalidate(FALSE);
		}
	}
}


void CThicknessCalibrationpage::RenderThicknessAgainstTime(CPaintDC *pDC, CRect *rr)
{
	
	CCoord Cp;
	CPoint pt;
	CString Buff;
	float fThickness;


	theApp.m_ThickCal.GetLastCoord(&Cp);
	float fMaxThickness =  Cp.fNomThickness * 1.10f;
	int nMaxTime = (Cp.nTimens * 110) / 100;

	Buff.Format(L"%.03f",fMaxThickness);
	CSize size = pDC->GetTextExtent(Buff);

	rr->DeflateRect(size.cx+4,size.cy,5,size.cy);
	pDC->MoveTo(rr->TopLeft());
	pDC->LineTo(rr->left,rr->bottom);
	pDC->LineTo(rr->BottomRight());

	CPen penGrey(PS_SOLID,1,RGB(200,200,200));
	CPen penRed(PS_SOLID,1,RGB(255,0,0));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPen* pOldPen = pDC->SelectObject(&penRed);

	for(int ii=0;ii<theApp.m_ThickCal.GetSize();ii++) {
		theApp.m_ThickCal.GetCoord(ii,&Cp);

		pt.x = rr->left + (int)(Cp.fNomThickness * (float)rr->Width() / fMaxThickness);
		pt.y = rr->bottom - MulDiv(Cp.nTimens, rr->Height(), nMaxTime);

		if((Cp.nMinTimens<=Cp.nTimens) && (Cp.nTimens<=Cp.nMaxTimens)) {
			pDC->SelectObject(&penBlue);
		} else {
			pDC->SelectObject(&penRed);
		}

		pDC->MoveTo(pt.x-3,pt.y-3);
		pDC->LineTo(pt.x+4,pt.y+4);
		pDC->MoveTo(pt.x-3,pt.y+3);
		pDC->LineTo(pt.x+4,pt.y-4);

		pt.y = rr->bottom - MulDiv(Cp.nMinTimens, rr->Height(), nMaxTime);
		pDC->MoveTo(pt);
		pt.y = rr->bottom - MulDiv(Cp.nMaxTimens, rr->Height(), nMaxTime);
		pDC->LineTo(pt);


		Buff.Format(L"%.02f",Cp.fNomThickness);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(pt.x-size.cx/2,rr->bottom,Buff);

		Buff.Format(L"%.03f",(float)Cp.nTimens/1000.0f);
		if(Buff.GetLength() > 5) Buff.GetBufferSetLength(5);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr->left-size.cx-2,pt.y-size.cy/2,Buff);

	}

	pDC->SelectObject(&penGrey);
	int nStep = (nMaxTime/100) + 1;
	for(int ii=0;ii<nMaxTime;ii += nStep) {

		theApp.TimeToThickness(ii,&fThickness);
		pt.x = rr->left + (int)((fThickness/2.0f) * (float)rr->Width() / fMaxThickness);
		pt.y = rr->bottom - MulDiv(ii, rr->Height(), nMaxTime);

		if(ii==0) {
			pDC->MoveTo(pt);
		} else {
			pDC->LineTo(pt);
		}
	}

	pDC->SelectObject(pOldPen);
}

void CThicknessCalibrationpage::OnCalibrationSort() 
{
	
	
	theApp.m_ThickCal.SortVariable(0,0);
	TableChanged(NULL, NULL);
	Invalidate(FALSE);
}

void CThicknessCalibrationpage::OnChangeEditPt0() 
{
	
	CString Buff;

	m_editPt0.GetWindowText(Buff);
	theApp.m_LastSettings.nThicknessTestPt0 = _ttoi(Buff) - 1;
}

void CThicknessCalibrationpage::OnChangeEditPt1() 
{
	
	CString Buff;

	m_editPt1.GetWindowText(Buff);
	theApp.m_LastSettings.nThicknessTestPt1 = _ttoi(Buff) - 1;
}

void CThicknessCalibrationpage::OnButtonCalibrate() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = THICKNESS_CALIBRATION_TWO_PT;
}

void CThicknessCalibrationpage::OnButtonCheckGauge() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = CHECK_THICKNESS_GAUGE;
}

void CThicknessCalibrationpage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString Buff;
	int nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_MATERIAL_VELOCITY:
		nTemp = theApp.m_LastSettings.nMaterialVelocity;
		theApp.m_LastSettings.nMaterialVelocity=pSpin->GetPos()&0xffff;
		if(nTemp - theApp.m_LastSettings.nMaterialVelocity) {
			Buff.Format(_T("%d m/s"),theApp.m_LastSettings.nMaterialVelocity);
			m_editMaterialVelocity.SetWindowText(Buff);
		}
		break;
	case IDC_SPIN_MATERIAL_CONSTANT:
		nTemp = theApp.m_LastSettings.nMaterialConstant;
		theApp.m_LastSettings.nMaterialConstant=(pSpin->GetPos()&0xffff) - 200;
		if(nTemp - theApp.m_LastSettings.nMaterialConstant) {
			Buff.Format(_T("%d ns"),theApp.m_LastSettings.nMaterialConstant);
			m_editMaterialConstant.SetWindowText(Buff);
		}
		break;
	}
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}
