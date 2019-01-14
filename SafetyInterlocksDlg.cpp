// SafetyInterlocksDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "SafetyInterlocksDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSafetyInterlocksDlg dialog


CSafetyInterlocksDlg::CSafetyInterlocksDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CSafetyInterlocksDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSafetyInterlocksDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pEditSpinItem = NULL;
	m_pDlg = pDlg;
	
	m_nIndex = 0;

	m_nL=0;
	m_Menu[m_nL++] = " ";
	m_Menu[m_nL++] = "Joystick";
	m_Menu[m_nL++] = "Moveto straite line";
	m_Menu[m_nL++] = "Poly move";
	m_Menu[m_nL++] = "Origin";

	m_Logic[0] = "X";
	m_Logic[1] = "0";
	m_Logic[2] = "1";

}


void CSafetyInterlocksDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSafetyInterlocksDlg)
	DDX_Control(pDX, IDC_COMBO_IOCONTROLLER, m_comboIOController);
	DDX_Control(pDX, IDC_EDIT_IN_PORT, m_editInPort);
	DDX_Control(pDX, IDC_LIST_SAFETY, m_listSafety);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSafetyInterlocksDlg, CDialog)
	//{{AFX_MSG_MAP(CSafetyInterlocksDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_SAFETY, OnClickListSafety)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SAFETY, OnDblclkListSafety)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_SAFETY, OnGetdispinfoListSafety)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_IOCONTROLLER, OnSelchangeComboIocontroller)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SAFETY, OnRclickListSafety)
	ON_COMMAND(ID_SAFETY_PASTECURRENTSTATUS, OnSafetyPastecurrentstatus)
	ON_COMMAND(ID_SAFETY_CLEARALL, OnSafetyClearall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSafetyInterlocksDlg message handlers
void CSafetyInterlocksDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CSafetyInterlocksDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CSafetyInterlocksDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CSafetyInterlocksDlg::OnDestroy() 
{
	if (CSafetyInterlocksDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CSafetyInterlocksDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CSafetyInterlocksDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();
	

	m_StaticPosList.Initialize(this, m_listSafety, TRUE, TRUE, TRUE, TRUE);

	if (CSafetyInterlocksDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CSafetyInterlocksDlg::IDD];
		theApp.ConfineToScreen(&rect);
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}

	m_comboIOController.InsertString(0,_T("PMAC"));
	m_comboIOController.InsertString(1,_T("SI10"));
	m_comboIOController.InsertString(2,_T("Safety Unit"));
	m_comboIOController.InsertString(3,_T("Field Bus"));
	switch(theApp.m_LastSettings.nIOController) {
	default:
	case SERVO_TURBO:
	case SERVO:
		m_comboIOController.SetCurSel(0);
		break;
	case SI10:
		m_comboIOController.SetCurSel(1);
		break;
	case SAFETY_UNIT:
		m_comboIOController.SetCurSel(2);
		break;
	case FIELD_BUS:
		m_comboIOController.SetCurSel(3);
		break;
	}//switch


	m_StaticPosList.SetNewPos();

	

	CreateColumns();
	FillList();
	
	SetTimer(1,300,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSafetyInterlocksDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosList.SetNewPos();
	
}

void CSafetyInterlocksDlg::UpdateAllControls()
{

}

void CSafetyInterlocksDlg::CreateColumns()
{
	int ColumnWidth[2] = { 0,170};
	CString ColumnName[2];
	ColumnName[0]= "#";
	ColumnName[1].LoadString(IDS_Function_Name);
	CString Buff;
	int	ii,nn;
	CDC * pDC = GetDC();
	CSize size;
	CString Temp;

	int nColumnCount = m_listSafety.GetHeaderCtrl()->GetItemCount();
	for (ii=0;ii < nColumnCount;ii++) {
		m_listSafety.DeleteColumn(0);
	}
	for (ii=0;ii<2;ii++) {
		m_listSafety.InsertColumn(ii,ColumnName[ii], LVCFMT_LEFT,ColumnWidth[ii]);
	}
	for(ii,nn=0;ii<18;ii++,nn++) {
		size = pDC->GetTextExtent(theApp.m_SafetyBitDescription[ii-2]);
		size.cx += 10;
		if(size.cx<30) size.cx = 30;
		Buff.Format(_T("%d"),15-nn);
		m_listSafety.InsertColumn(ii,Buff, LVCFMT_CENTER,size.cx);
	}

	m_listSafety.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

	ReleaseDC(pDC);
}

void CSafetyInterlocksDlg::FillList()
{
	CString Buff;

	m_listSafety.DeleteAllItems();
	for (int ii=0;ii<m_nL;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listSafety.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listSafety.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<m_nL;ii++) 
		m_listSafety.SetItemState(ii , 0,LVIS_SELECTED);
	m_listSafety.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listSafety.SetFocus();

}


void CSafetyInterlocksDlg::OnClickListSafety(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ClearEditSpin();
	
	*pResult = 0;
}

void CSafetyInterlocksDlg::OnDblclkListSafety(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	m_listSafety.GetWindowRect(CtrlRect);
	m_listSafety.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	ClearEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

	if(pDispInfo->item.mask == 0) {
		switch(pDispInfo->item.iItem) {
		case 0:
			break;
		case 1:
			break;
		default: 
			m_pEditSpinItem->Initialize(&theApp.m_SafetyBitDescription[17-pDispInfo->item.iItem]);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		}

	} else {
		switch(pDispInfo->item.iItem) {
		case 0:
			break;
		case 1:
			break;
		default: 
			m_pEditSpinItem->Initialize(&theApp.m_nSafetyLogic[pDispInfo->item.mask][17-pDispInfo->item.iItem],m_Logic,3);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		}
	}
	
	*pResult = 0;
}

void CSafetyInterlocksDlg::OnGetdispinfoListSafety(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];

	if(pDispInfo->item.iItem == 0) {
		switch(pDispInfo->item.iSubItem) {
		case 0:
		case 1: str[0]=0;
			break;
		default:
			swprintf_s(str,100,_T("%s"),(LPCWSTR)theApp.m_SafetyBitDescription[17-pDispInfo->item.iSubItem]);
			break;
		}
	} else {
		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:
			swprintf_s(str,100,_T("%s"),(LPCWSTR)m_Menu[pDispInfo->item.iItem]);
			break;
		default:
			swprintf_s(str,100,L"%s",m_Logic[theApp.m_nSafetyLogic[pDispInfo->item.iItem][17-pDispInfo->item.iSubItem]].GetBuffer());
			break;
		}
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CSafetyInterlocksDlg::ClearEditSpin()
{
	if(m_pEditSpinItem!=NULL) {
		delete m_pEditSpinItem;
		m_pEditSpinItem=NULL;
	}

}

void CSafetyInterlocksDlg::OnTimer(UINT nIDEvent) 
{
	CString Buff,Temp;

	int nPort = theApp.m_Motors.GetInputPort();

	int nMask = 0x8000;
	Buff.Empty();
	for(int ii=0;ii<16;ii++,nMask>>=1) {
		if((nPort & nMask) == 0) {
			Temp.Format(_T("0 "));
		} else {
			Temp.Format(_T("1 "));
		}
		Buff += Temp;
	}
	Temp.Format(_T("= %.04x h"),nPort);
	Buff += Temp;

	m_editInPort.SetWindowText(Buff);

	CDialog::OnTimer(nIDEvent);
}

void CSafetyInterlocksDlg::OnSelchangeComboIocontroller() 
{
	int nComboPos = 0;

	nComboPos = m_comboIOController.GetCurSel();

	switch(nComboPos) {
	case 0:
		theApp.m_LastSettings.nIOController = SERVO_TURBO;
		break;
	case 1:
		theApp.m_LastSettings.nIOController = SI10;
		break;
	case 2:
		theApp.m_LastSettings.nIOController = SAFETY_UNIT;
		break;
	case 3:
		theApp.m_LastSettings.nIOController = FIELD_BUS;
		break;
	}//switch
	
}

void CSafetyInterlocksDlg::OnRclickListSafety(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SAFE_DELETE( m_pEditSpinItem );
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;


	GetCursorPos(&Point);
	m_nIndex = pDispInfo->item.mask;

	switch(pDispInfo->item.iItem) {
	default:
		if (menu->LoadMenu(IDR_RB_SAFETYINTERLOCKS_MENU)) {
			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

		};
		break;
	case 999: //Shut compiler up
		break;
	}
	delete menu;
	
	
	*pResult = 0;
}

void CSafetyInterlocksDlg::OnSafetyPastecurrentstatus() 
{
	int nPort = theApp.m_Motors.GetInputPort();
	int nMask = 0x01;

	for(int ii=0;ii<16;ii++,nMask<<=1) {
		if((nPort & nMask) == 0) {
			theApp.m_nSafetyLogic[m_nIndex][ii] = 1;
		} else {
			theApp.m_nSafetyLogic[m_nIndex][ii] = 2;
		}
	}

	FillList();
}

void CSafetyInterlocksDlg::OnSafetyClearall() 
{
	for(int ii=0;ii<16;ii++) {
		theApp.m_nSafetyLogic[m_nIndex][ii] = 0;
	};
	FillList();
}
