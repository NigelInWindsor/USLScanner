// ToolChangerPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ToolChangerPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolChangerPage property page

IMPLEMENT_DYNCREATE(CToolChangerPage, CPropertyPage)

CToolChangerPage::CToolChangerPage() : CPropertyPage(CToolChangerPage::IDD)
{
	//{{AFX_DATA_INIT(CToolChangerPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pEditSpinItem = NULL;

	m_nSortDir = 0;
	m_nSortColumn = 0;


}

CToolChangerPage::~CToolChangerPage()
{
}

void CToolChangerPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolChangerPage)
	DDX_Control(pDX, IDC_EDIT_Z_LATCH_DISTANCE, m_editZLatchDistance);
	DDX_Control(pDX, IDC_EDIT_LATCH_STANDOFF_DISTANCE, m_editLatchStandoffDistance);
	DDX_Control(pDX, IDC_EDIT_STATUS_1, m_editStatus1);
	DDX_Control(pDX, IDC_EDIT_STATUS_0, m_editStatus0);
	DDX_Control(pDX, IDC_LIST_TOOLS, m_listTools);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolChangerPage, CPropertyPage)
	//{{AFX_MSG_MAP(CToolChangerPage)
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TOOLS, OnGetdispinfoListTools)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_TOOLS, OnClickListTools)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TOOLS, OnDblclkListTools)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_TOOLS, OnRclickListTools)
	ON_COMMAND(ID_TOOLCHANGER_STORECURRENTPOSITION, OnToolchangerStorecurrentposition)
	ON_COMMAND(ID_TOOLCHANGER_DELETE, OnToolchangerDelete)
	ON_EN_CHANGE(IDC_EDIT_Z_LATCH_DISTANCE, OnChangeEditZLatchDistance)
	ON_EN_CHANGE(IDC_EDIT_LATCH_STANDOFF_DISTANCE, OnChangeEditLatchStandoffDistance)
	ON_WM_TIMER()
	ON_COMMAND(ID_BUTTON_LOCK_TOOL_0, OnButtonLockTool0)
	ON_COMMAND(ID_BUTTON_LOCK_TOOL_1, OnButtonLockTool1)
	ON_COMMAND(ID_BUTTON_UNLOCK_TOOL_0, OnButtonUnlockTool0)
	ON_COMMAND(ID_BUTTON_UNLOCK_TOOL_1, OnButtonUnlockTool1)
	ON_COMMAND(ID_BUTTON_RETRIEVE_TOOL_0, OnButtonRetrieveTool0)
	ON_COMMAND(ID_BUTTON_RETRIEVE_TOOL_1, OnButtonRetrieveTool1)
	ON_COMMAND(ID_BUTTON_RETURN_TOOL_0, OnButtonReturnTool0)
	ON_COMMAND(ID_BUTTON_RETURN_TOOL_1, OnButtonReturnTool1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_TOOLS, OnColumnclickListTools)
	ON_COMMAND(ID_TOOLCHANGER_ROTATEHEAD, OnToolchangerRotatehead)
	ON_COMMAND(ID_BUTTON_ORIGIN_FINGER_MANIPULATOR, OnButtonOriginFingerManipulator)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_MESSAGE(UI_ITEMCHANGED,ItemChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolChangerPage message handlers

int CToolChangerPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect temp;
	CSize size;

	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_TOOLCHANGER_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndOriginFingerToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndOriginFingerToolBar.LoadToolBar(IDR_ORIGIN_FINGER_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,270,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolBar);
	m_wndRebar.AddBar(&m_wndOriginFingerToolBar);
	
	return 0;

}

BOOL CToolChangerPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	for(int ii=0;ii<16;ii++) {
		m_strID[ii].Format(L"%d",ii);
	}

	

	CreateColumns();
	FillList();

	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CToolChangerPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}


void CToolChangerPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	CString Buff;


	Buff.Format(L"%.01f mm/s",theApp.m_Tank.vToolRegion[0].x);
	m_editLatchStandoffDistance.SetWindowText(Buff);

	Buff.Format(L"%.01f mm/s",theApp.m_Tank.vToolLatch[0].z);
	m_editZLatchDistance.SetWindowText(Buff);

}


void CToolChangerPage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	CRect rr, rrClient, rrToolBar;
	int nBorder = 0;

	if(GetSafeHwnd()) {
		GetClientRect(&rrClient);
		m_wndRebar.GetWindowRect(&rrToolBar);
		ScreenToClient(&rrToolBar);

		if (m_listTools.GetSafeHwnd()) {
			m_listTools.GetWindowRect(&rr);
			ScreenToClient(&rr);
			nBorder = rr.left - rrClient.left;
			rr.right = rrClient.right - nBorder;
			rr.bottom = rrClient.bottom - nBorder;
			m_listTools.SetWindowPos(NULL, rr.left, rr.top, rr.Width(), rr.Height(), NULL);
		}
		if (m_editStatus0.GetSafeHwnd()) {
			m_editStatus0.GetWindowRect(&rr);
			ScreenToClient(&rr);
			rr.top = rrToolBar.bottom + nBorder / 2;
			rr.right = rrClient.Width() / 2 - nBorder / 2;
			m_editStatus0.SetWindowPos(NULL, rr.left, rr.top, rr.Width(), rr.Height(), NULL);
		}
		if (m_editStatus1.GetSafeHwnd()) {
			m_editStatus1.GetWindowRect(&rr);
			ScreenToClient(&rr);
			rr.top = rrToolBar.bottom + nBorder / 2;
			rr.left = rrClient.Width() / 2 + nBorder / 2;
			rr.right = rrClient.Width() - nBorder;
			m_editStatus1.SetWindowPos(NULL, rr.left, rr.top, rr.Width(), rr.Height(), NULL);
		}
	}
	
}

BOOL CToolChangerPage::OnSetActive() 
{
	
	
	theApp.m_LastSettings.nLastCompensationToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	m_nToolHolderStatus[0] = m_nToolHolderStatus[1] = m_nToolID[0] = m_nToolID[1] = 0xffffffff;
	SetTimer(1,250,NULL);

	SetAccessPrivelages();
	
	return CPropertyPage::OnSetActive();
}

BOOL CToolChangerPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CToolChangerPage::CreateColumns()
{
	
	int ColumnWidth[10] = { 0,100,30,60,60,50,50,50,60,60};
	CString ColumnName[10];
	ColumnName[0]= "#";
	ColumnName[1].LoadString(IDS_Type);
	ColumnName[2].LoadString(IDS_ID);
	ColumnName[3].LoadString(IDS_Overall_L);
	ColumnName[4].LoadString(IDS_Side);
	ColumnName[5] = L"X";
	ColumnName[6] = L"Y";
	ColumnName[7] = L"Z";
	ColumnName[8].LoadString(IDS_Probe_L);
	ColumnName[9] = L"Wp";
	int ColumnJust[10] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT };

	CDC* pDC = m_listTools.GetDC();
	CString Buff = L"-99999";
	CSize size = pDC->GetTextExtent(Buff);
	m_listTools.ReleaseDC(pDC);

	ColumnWidth[5] = ColumnWidth[6] = ColumnWidth[7] = size.cx;

	int nColumnCount = (&m_listTools.GetHeaderCtrl())->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listTools.DeleteColumn(0);
	}
	for (int ii=0;ii<10;ii++) {
		m_listTools.InsertColumn(ii,ColumnName[ii], ColumnJust[ii],ColumnWidth[ii]);
	}
	m_listTools.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CToolChangerPage::FillList()
{
	

	CString Buff;

	m_listTools.DeleteAllItems();
	for (int ii=0;ii<theApp.m_ToolChangers.getEndEffectorSize();ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listTools.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listTools.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<theApp.m_ToolChangers.getEndEffectorSize();ii++) 
		m_listTools.SetItemState(ii , 0,LVIS_SELECTED);
	m_listTools.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listTools.SetFocus();

}

void CToolChangerPage::OnClickListTools(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	SAFE_DELETE( m_pEditSpinItem );
	m_nIndex = pDispInfo->item.mask;
	Invalidate(FALSE);

	
	*pResult = 0;
}

void CToolChangerPage::OnDblclkListTools(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;


	SAFE_DELETE( m_pEditSpinItem );

	if(theApp.m_nLogonLevelMask &  theApp.m_cAccess[IDD_TOOL_CHANGER_PAGE]) {

		m_nIndex=pDispInfo->item.mask;

		if(m_nIndex<0) {
			m_fEdit = 0.0f;
			theApp.m_ToolChangers.AddEmptyEndEffector();
			FillList();
			m_nIndex = theApp.m_ToolChangers.getEndEffectorSize() - 1;
		}

		if((m_nIndex>=0) && (m_nIndex<theApp.m_ToolChangers.getEndEffectorSize())) {
			m_listTools.GetWindowRect(CtrlRect);
			m_listTools.GetSubItemRect(m_nIndex,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
			Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
			Rect.InflateRect(0,1,0,2);

			m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

			switch(m_nColumn=pDispInfo->item.iItem) {
			case 0:
				break;
	//		case 1:	m_pEditSpinItem->Initialize(theApp.m_ToolChangers.getPtrDescription(m_nIndex));
	//			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
	//			break;
			case 1: m_pEditSpinItem->Initialize(theApp.m_ToolChangers.getPtrType(m_nIndex),theApp.m_ToolChangers.m_strType,7);
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 2: m_pEditSpinItem->Initialize(theApp.m_ToolChangers.getPtrUniqueID(m_nIndex),m_strID,16);
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 3: m_pEditSpinItem->Initialize(theApp.m_ToolChangers.getPtrLength(m_nIndex),0.0f, 800.0f,10.0f,L"%.0f mm");
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 4: m_pEditSpinItem->Initialize(theApp.m_ToolChangers.getPtrSide(m_nIndex),theApp.m_DlgSideName,2);
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 8:
				theApp.m_ToolChangers.getClampToolTipLength(m_nIndex, &m_fEdit);
				m_pEditSpinItem->Initialize(&m_fEdit,0.0f, 800.0f,0.1f,L"%.01f");
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 9:
				theApp.m_ToolChangers.getWaterPath(m_nIndex, &m_fEdit);
				m_pEditSpinItem->Initialize(&m_fEdit,0.0f, 800.0f,0.1f,L"%.01f");
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			}
		}	
	}

	*pResult = 0;
}

HRESULT CToolChangerPage::ItemChanged(WPARAM wParam, LPARAM lParam)
{
	

	switch(m_nColumn) {
	case 8:	theApp.m_ToolChangers.setClampToolTipLength(m_nIndex, m_fEdit);
		break;
	case 9:	theApp.m_ToolChangers.setWaterPath(m_nIndex, m_fEdit);
		break;
	}

	return true;
}

void CToolChangerPage::OnGetdispinfoListTools(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	float fTemp;

	str[0] = 0;

	if(theApp.m_ToolChangers.getEndEffectorSize()<=0) return;
	int nRow = pDispInfo->item.iItem;

	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:
		swprintf_s(str,L"%s",theApp.m_ToolChangers.getType(nRow).GetBuffer());
		break;
	case 2:
		swprintf_s(str,L"%d",theApp.m_ToolChangers.getUniqueID(nRow));
		break;
	case 3:
		swprintf_s(str,L"%.0f mm",theApp.m_ToolChangers.getLength(nRow));
		break;
	case 4:
		swprintf_s(str,L"%s",theApp.m_DlgSideName[theApp.m_ToolChangers.getSide(nRow)].GetBuffer());
		break;
	case 5:
		swprintf_s(str,L"%s",theApp.m_ToolChangers.getCoordAtParkPos(nRow,'X').GetBuffer());
		break;
	case 6:
		swprintf_s(str,L"%s",theApp.m_ToolChangers.getCoordAtParkPos(nRow,'Y').GetBuffer());
		break;
	case 7:
		swprintf_s(str,L"%s",theApp.m_ToolChangers.getCoordAtParkPos(nRow,'Z').GetBuffer());
		break;
	case 8:
		swprintf_s(str,L"%s",theApp.m_ToolChangers.getClampToolTipLength(nRow, &fTemp).GetBuffer());
		break;
	case 9:
		swprintf_s(str,L"%s",theApp.m_ToolChangers.getWaterPath(nRow, &fTemp).GetBuffer());
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

BOOL CToolChangerPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
	
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   CString strReturn,strRetrieve,strLock,strUnlock, strUnacceptable;

   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

   strReturn.LoadString(IDS_Return);
   strRetrieve.LoadString(IDS_Retrieve);
   strLock.LoadString(IDS_Lock);
   strUnlock.LoadString(IDS_Unlock);
   strUnacceptable.LoadString(IDS_Unacceptable_tool_highlighted);

   int nSide = theApp.m_ToolChangers.getSide(m_nIndex);

   switch(nID) {
   case ID_BUTTON_LOCK_TOOL_0: strTipText.Format(L"%s %s", strLock, theApp.m_DlgSideName[0]);
	   break;
   case ID_BUTTON_UNLOCK_TOOL_0: strTipText.Format(L"%s %s", strUnlock, theApp.m_DlgSideName[0]);
	   break;
   case ID_BUTTON_RETRIEVE_TOOL_0:
	   if(nSide==0) {
		   strTipText.Format(L"%s %s %s", strRetrieve, theApp.m_DlgSideName[0], theApp.m_ToolChangers.getType(m_nIndex));
	   } else {
		   strTipText = strUnacceptable;
	   }
	   break;
   case ID_BUTTON_RETURN_TOOL_0: 
	   if(theApp.m_FBCtrl.m_nToolID[0] > 0 && theApp.m_FBCtrl.m_nToolID[0] < 10) {
		   strTipText.Format(L"%s %s %s", strReturn, theApp.m_DlgSideName[0], theApp.m_ToolChangers.getTypeFromID(theApp.m_FBCtrl.m_nToolID[0], NULL));
	   } else {
		   strTipText = strUnacceptable;
	   }
	   break;
   case ID_BUTTON_LOCK_TOOL_1: strTipText.Format(L"%s %s", strLock, theApp.m_DlgSideName[1]);
	   break;
   case ID_BUTTON_UNLOCK_TOOL_1: strTipText.Format(L"%s %s", strUnlock, theApp.m_DlgSideName[1]);
	   break;
   case ID_BUTTON_RETRIEVE_TOOL_1:
	   if(nSide == 1) {
		   strTipText.Format(L"%s %s %s", strRetrieve, theApp.m_DlgSideName[1], theApp.m_ToolChangers.getType(m_nIndex));
   	   } else {
		   strTipText = strUnacceptable;
	   }

	   break;
   case ID_BUTTON_RETURN_TOOL_1: 
	   if(theApp.m_FBCtrl.m_nToolID[1] > 0 && theApp.m_FBCtrl.m_nToolID[1] < 10) {
			strTipText.Format(L"%s %s %s", strReturn, theApp.m_DlgSideName[1], theApp.m_ToolChangers.getTypeFromID(theApp.m_FBCtrl.m_nToolID[1], NULL));
   	   } else {
		   strTipText = strUnacceptable;
	   }

	   break;
   case ID_BUTTON_ORIGIN_FINGER_MANIPULATOR: strTipText.LoadString(IDS_Origin_finger_manipulator);
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

void CToolChangerPage::OnRclickListTools(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* pMenu;
	CMenu* pPopup;
	CPoint	Point;

	if(theApp.m_nLogonLevelMask &  theApp.m_cAccess[IDD_TOOL_CHANGER_PAGE]) {
		pMenu = (CMenu *) new CMenu;

		POSITION pos = m_listTools.GetFirstSelectedItemPosition();
		m_nIndex=m_listTools.GetNextSelectedItem(pos);

		GetCursorPos(&Point);

		if (pMenu->LoadMenu(IDR_RB_TOOLCHANGER_MENU)) {

			pPopup = pMenu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

		};
		delete pMenu;
	}	
	
	*pResult = 0;
}

void CToolChangerPage::OnToolchangerStorecurrentposition() 
{
	if(m_nIndex>=0 && m_nIndex < theApp.m_ToolChangers.getEndEffectorSize()) {
		theApp.m_ToolChangers.RefreshParkPosition(m_nIndex);
		FillList();
		theApp.SaveToolChanger();
	}
}

void CToolChangerPage::OnToolchangerDelete() 
{
	if(m_nIndex>=0 && m_nIndex < theApp.m_ToolChangers.getEndEffectorSize()) {
		theApp.m_ToolChangers.Delete(m_nIndex);
		FillList();
		theApp.SaveToolChanger();
	}
}




void CToolChangerPage::OnChangeEditZLatchDistance() 
{
	
	CString Buff;

	m_editZLatchDistance.GetWindowText(Buff);
	ZeroMemory(theApp.m_Tank.vToolLatch, sizeof theApp.m_Tank.vToolLatch);
	_WTOF(Buff, theApp.m_Tank.vToolLatch[0].z);
	theApp.m_Tank.vToolLatch[1].z = theApp.m_Tank.vToolLatch[0].z;
}

void CToolChangerPage::OnChangeEditLatchStandoffDistance() 
{
	
	CString Buff;

	m_editLatchStandoffDistance.GetWindowText(Buff);
	ZeroMemory(theApp.m_Tank.vToolRegion, sizeof theApp.m_Tank.vToolRegion);
	_WTOF(Buff, theApp.m_Tank.vToolRegion[0].x);
	theApp.m_Tank.vToolRegion[1].x = theApp.m_Tank.vToolRegion[0].x;
	
}

void CToolChangerPage::OnTimer(UINT nIDEvent) 
{
	
	int nIndex;
	CString Buff;
	int nStatus;

	int nSide = 0;
	if(m_nToolHolderStatus[nSide] - theApp.m_FBCtrl.m_nToolHolderStatus[nSide]) {
		m_nToolHolderStatus[nSide] = theApp.m_FBCtrl.m_nToolHolderStatus[nSide];

		if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_LOCK_TOOL_0))>=0)
			m_nToolHolderStatus[nSide] & 1 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
		if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_UNLOCK_TOOL_0))>=0)
			(m_nToolHolderStatus[nSide] & 1) == 0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);


		if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_RETRIEVE_TOOL_0))>=0)
			((m_nToolHolderStatus[nSide] & 1) == 1) ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_DISABLED) : m_wndToolBar.SetButtonStyle(nIndex,0);

		if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_RETURN_TOOL_0))>=0)
			((m_nToolHolderStatus[nSide] & 1) == 0) ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_DISABLED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	}
	if(m_nToolID[nSide] - theApp.m_FBCtrl.m_nToolID[nSide] || m_nDoorStatus[nSide] - theApp.m_FBCtrl.m_nDoorStatus[nSide]) {
		m_nToolID[nSide] = theApp.m_FBCtrl.m_nToolID[nSide];
		m_nDoorStatus[nSide] = theApp.m_FBCtrl.m_nDoorStatus[nSide];

		Buff = theApp.m_ToolChangers.getTypeFromID(m_nToolID[nSide], &nStatus);
		switch(nStatus) {
		case 0: m_editStatus0.SetTextColour(RGB(0,0,0));
			m_editStatus0.SetBkColor(RGB(200,200,200));
			break;
		case 1: m_editStatus0.SetTextColour(RGB(0,0,255));
			m_editStatus0.SetBkColor(RGB(255,255,255));
			break;
		case 2: m_editStatus0.SetTextColour(RGB(255,255,255));
			m_editStatus0.SetBkColor(RGB(255,0,0));
			break;
		}
		if(m_nDoorStatus[nSide] == 1) {
			m_editStatus0.SetBkColor(RGB(255,0,0));
			Buff += L"\r\nDoor Open";
		}

		m_editStatus0.SetWindowText(Buff);
	}


	nSide = 1;
	if(m_nToolHolderStatus[nSide] - theApp.m_FBCtrl.m_nToolHolderStatus[nSide]) {
		m_nToolHolderStatus[nSide] = theApp.m_FBCtrl.m_nToolHolderStatus[nSide];

		if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_LOCK_TOOL_1))>=0)
			m_nToolHolderStatus[nSide] & 1 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
		if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_UNLOCK_TOOL_1))>=0)
			(m_nToolHolderStatus[nSide] & 1) == 0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);


		
		if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_RETRIEVE_TOOL_1))>=0)
			((m_nToolHolderStatus[nSide] & 1) == 1) ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_DISABLED) : m_wndToolBar.SetButtonStyle(nIndex,0);

		if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_RETURN_TOOL_1))>=0)
			((m_nToolHolderStatus[nSide] & 1) == 0) ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_DISABLED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	}
	if(m_nToolID[nSide] - theApp.m_FBCtrl.m_nToolID[nSide] || m_nDoorStatus[nSide] - theApp.m_FBCtrl.m_nDoorStatus[nSide]) {
		m_nToolID[nSide] = theApp.m_FBCtrl.m_nToolID[nSide];
		m_nDoorStatus[nSide] = theApp.m_FBCtrl.m_nDoorStatus[nSide];

		Buff = theApp.m_ToolChangers.getTypeFromID(m_nToolID[nSide], &nStatus);
		switch(nStatus) {
		case 0: m_editStatus1.SetTextColour(RGB(0,0,0));
			m_editStatus1.SetBkColor(RGB(200,200,200));
			break;
		case 1: m_editStatus1.SetTextColour(RGB(0,0,255));
			m_editStatus1.SetBkColor(RGB(255,255,255));
			break;
		case 2: m_editStatus1.SetTextColour(RGB(255,255,255));
			m_editStatus1.SetBkColor(RGB(255,0,0));
			break;
		}
		if(m_nDoorStatus[nSide] == 1) {
			m_editStatus1.SetBkColor(RGB(255,0,0));
			Buff += L"\r\nDoor Open";
		}
		m_editStatus1.SetWindowText(Buff);
	}
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CToolChangerPage::OnButtonLockTool0() 
{
	

	theApp.m_Motors.LockTool(0);
	
}

void CToolChangerPage::OnButtonLockTool1() 
{
	

	theApp.m_Motors.LockTool(1);
}

void CToolChangerPage::OnButtonUnlockTool0() 
{
	
	CString Warning,Buff;

	Warning.LoadString(IDS_Warning);
	Buff.LoadString(IDS_Are_you_sure);

	if(MessageBox(Buff, Warning, MB_YESNO|MB_ICONEXCLAMATION)!=IDYES) return;

	theApp.m_Motors.UnlockTool(0);
}

void CToolChangerPage::OnButtonUnlockTool1() 
{
	
	CString Warning,Buff;

	Warning.LoadString(IDS_Warning);
	Buff.LoadString(IDS_Are_you_sure);

	if(MessageBox(Buff, Warning, MB_YESNO|MB_ICONEXCLAMATION)!=IDYES) return;

	theApp.m_Motors.UnlockTool(1);
}

void CToolChangerPage::OnButtonRetrieveTool0() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nToolIndexToRetrieveDeposit[0] = m_nIndex;
	theApp.m_Thread.m_nThreadAction=RETRIEVE_TOOL_0;
}

void CToolChangerPage::OnButtonRetrieveTool1() 
{
	

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nToolIndexToRetrieveDeposit[1] = m_nIndex;
	theApp.m_Thread.m_nThreadAction=RETRIEVE_TOOL_1;
}

void CToolChangerPage::OnButtonReturnTool0() 
{
	
	int nStatus, nIndex;

	nIndex = theApp.m_ToolChangers.getIndexFromID(theApp.m_FBCtrl.m_nToolID[0], &nStatus);

	if(nStatus == 1 && nIndex >= 0) {

		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nToolIndexToRetrieveDeposit[0] = nIndex;
		theApp.m_Thread.m_nThreadAction=DEPOSIT_TOOL_0;
	}
}

void CToolChangerPage::OnButtonReturnTool1() 
{
	
	int nStatus, nIndex;

	nIndex = theApp.m_ToolChangers.getIndexFromID(theApp.m_FBCtrl.m_nToolID[1], &nStatus);

	if(nStatus == 1 && nIndex >= 0) {

		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nToolIndexToRetrieveDeposit[1] = nIndex;
		theApp.m_Thread.m_nThreadAction=DEPOSIT_TOOL_1;
	}
}

void CToolChangerPage::OnColumnclickListTools(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(m_nSortColumn == pNMListView->iSubItem) {
		m_nSortDir ^= 0x01;
	}

	switch(m_nSortColumn = pNMListView->iSubItem) {
	case 2: theApp.m_ToolChangers.Sort(m_nSortColumn, m_nSortDir);
		break;
	}

	FillList();

	*pResult = 0;
}

void CToolChangerPage::OnToolchangerRotatehead() 
{
	
	CCoord CpHead, CpPark;
	int nSide;

	if((m_nIndex>=0) && (m_nIndex<theApp.m_ToolChangers.getEndEffectorSize())) {

		nSide = theApp.m_ToolChangers.getSide(m_nIndex);

		theApp.m_ToolChangers.getParkPos(m_nIndex, &CpPark);
		theApp.m_Kinematics.TipsFromNormals(&CpPark, nSide);
		theApp.m_Motors.GetHeadPos(&CpHead);

		CpHead.Side[nSide].norm = CpPark.Side[nSide].norm;
		CpHead.Side[nSide].fXt = CpPark.Side[nSide].fXt;
		CpHead.Side[nSide].fYt = CpPark.Side[nSide].fYt;

		theApp.m_Motors.GotoHeadCoord(CpHead);
	} else {
		MessageBox(L"No suitable table entry selected",L"Error", MB_OK);
	}
}

void CToolChangerPage::SetAccessPrivelages()
{
	if (GetSafeHwnd() == NULL) return;
	if(m_hWnd == NULL) return;

	

	bool bReadOnly = true;
	if(theApp.m_nLogonLevelMask &  theApp.m_cAccess[IDD_TOOL_CHANGER_PAGE]) {
		bReadOnly = false;
	}

	m_editZLatchDistance.SetReadOnly(bReadOnly );
	m_editLatchStandoffDistance.SetReadOnly(bReadOnly );

}

void CToolChangerPage::OnButtonOriginFingerManipulator() 
{
	

	theApp.m_Motors.OriginFingerManipulator();
	
}
